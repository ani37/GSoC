#include <cmath>
#include <algorithm>
#include <iostream>
#include <vector>
#include<stdio.h>
#include "happly.h"
#include <random>
#include <string>
#include <iterator>
//Ponca
#include <Ponca/Fitting>
#include "Eigen/Eigen"
#include <Ponca/src/SpatialPartitioning/KdTree/kdTree.h>


// Polyscope
#include "polyscope/point_cloud.h"
#include "polyscope/polyscope.h"
#include "polyscope/surface_mesh.h"
#include "geometrycentral/surface/manifold_surface_mesh.h"
#include "geometrycentral/surface/meshio.h"
#include "geometrycentral/surface/surface_mesh.h"
#include "geometrycentral/surface/vertex_position_geometry.h"


using namespace std;
using namespace Ponca;
using namespace geometrycentral;
using namespace geometrycentral::surface;

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////


#define DIMENSION 3
 
/*
   \brief Variant of the MyPoint class allowing to work with external raw data.
 
   Using this approach, ones can use the patate library with already existing
   data-structures and without any data-duplication.
 
   In this example, we use this class to map an interlaced raw array containing
   both point normals and coordinates.
 */
// This class defines the input data format
class MyPoint
{
public:
    enum {Dim = DIMENSION};
    typedef double Scalar;
    typedef Eigen::Matrix<Scalar, Dim, 1>   VectorType;
    typedef Eigen::Matrix<Scalar, Dim, Dim> MatrixType;
 
    PONCA_MULTIARCH inline MyPoint(const std::array<Scalar, 3>&poss,
                                   const std::array<Scalar, 3>&  norm)
        : m_pos    (Eigen::Map< const VectorType >(poss.begin())),
          m_normal (Eigen::Map< const VectorType >(norm.begin()))
    {}
 
    PONCA_MULTIARCH inline const Eigen::Map< const VectorType >& pos()    const { return m_pos; }
    PONCA_MULTIARCH inline const Eigen::Map< const VectorType >& normal() const { return m_normal; }
 
private:
    Eigen::Map< const VectorType > m_pos, m_normal;
}; 


typedef MyPoint::Scalar Scalar;
typedef MyPoint::VectorType VectorType;
 
std::vector< std::array<double, 3> > positions, normals;
vector<MyPoint> points;



// Define related structure
typedef DistWeightFunc<MyPoint,SmoothWeightKernel<Scalar> > WeightFunc;


void loadPointCloud(std::string filename,
                    std::vector<std::array<double, 3>>& vertexPositionsOut) {

  happly::PLYData plyIn(filename);

  // Get mesh-style data from the object
  vertexPositionsOut = plyIn.getVertexPositions();
  //faceIndicesOut = plyIn.getFaceIndices<size_t>();

}



Scalar tmax = 0.1;
int knei = 10;

void ComputeNormals()
{
    // Making it local
    typedef Basket<MyPoint,WeightFunc,CovariancePlaneFit> PlaneFit;

    
    KdTree<MyPoint> structure(points);

    for(int i = 0; i < points.size(); i++){

        // set evaluation point and scale at the ith coordinate
        const VectorType& p = points.at(i).pos();
	
        // Here we now perform the fit
        PlaneFit _fit;
        // Set a weighting function instance
        _fit.setWeightFunc(WeightFunc(tmax));
        // Set the evaluation position
        _fit.init(p);

        for( auto idx : structure.k_nearest_neighbors(p, knei) ){
            _fit.addNeighbor( points[idx] );
        }
        _fit.finalize();
        
        
        if(_fit.isStable()){
            VectorType no = _fit.primitiveGradient(p).transpose();
            normals.push_back({no[0],no[1], no[2]});
        }
    }
    polyscope::getPointCloud("positions")->addVectorQuantity("normals", normals);
}

void ComputeCurvature()
{ 
    // Making it local
    typedef Basket<MyPoint,WeightFunc,OrientedSphereFit,   GLSParam> SphereFit;

    std::vector<Scalar> curvature;
    KdTree<MyPoint> structure(points);
    for(int i = 0; i < points.size(); i++){

        // set evaluation point and scale at the ith coordinate
        const VectorType& p = points.at(i).pos();
	
        // Here we now perform the fit
        SphereFit _fit;
        // Set a weighting function instance
        _fit.setWeightFunc(WeightFunc(tmax));
        // Set the evaluation position
        _fit.init(p);

        for( auto idx : structure.k_nearest_neighbors(p, knei) ){
            _fit.addNeighbor( points[idx] );
        }
        _fit.finalize();
        
        
        if(_fit.isStable()){
            curvature.push_back(_fit.kappa());
        }
    }
    polyscope::getPointCloud("positions")->addScalarQuantity("curvature", curvature);
}


// Your callback functions
void myCallback() {

  // Since options::openImGuiWindowForUserCallback == true by default, 
  // we can immediately start using ImGui commands to build a UI

  ImGui::PushItemWidth(100); // Make ui elements 100 pixels wide,
                             // instead of full width. Must have 
                             // matching PopItemWidth() below.
  static int numPoints = 2000;
  static float param = 3.14;

//   ImGui::InputInt("num points", &nPts);             // set a int variable
//   ImGui::InputFloat("param value", &anotherParam);  // set a float variable

  if (ImGui::Button("Compute Curvature")) {
    // executes when button is pressed
    ComputeCurvature();
  }
  ImGui::SameLine();
  if (ImGui::Button("Compute Normals")) {
    // executes when button is pressed
    ComputeNormals();
  } 
  ImGui::InputDouble("Select Scale Size", &tmax);  // set a double variable
  ImGui::InputInt("Change Size of K", &knei);  // set a float variable

  ImGui::PopItemWidth();
}




int main(int argc, char **argv) {

   
    //freopen("output.txt", "w", stdout);
    polyscope::init();

    string filename = "hippo.ply";

    std::ifstream testStream(filename);
    if (!testStream) {
        return 0;
    }

    // Load positions from file
    loadPointCloud(filename, positions);
    testStream.close();

    int n = positions.size();


    for(int i = 0; i < n; i++){
        points.push_back({positions[i], {0,0,0}});
    }
   

    // // run normal computation using CovariancePlaneFit
    // ComputeNormals(structure, points, normals);
    // //run curvature computation using OrientedSphereFit
    // ComputeCurvature(structure, points, curvature);


    // visualize!
    polyscope::registerPointCloud("positions", positions);
    
    // Add the callback  
    polyscope::state::userCallback = myCallback;
    // Show the gui
    polyscope::show(); 
    return 0;   
}