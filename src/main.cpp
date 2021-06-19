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
 
// Define related structure
typedef DistWeightFunc<MyPoint,SmoothWeightKernel<Scalar> > WeightFunc;


void loadPointCloud(std::string filename,
                    std::vector<std::array<double, 3>>& vertexPositionsOut) {

  happly::PLYData plyIn(filename);

  // Get mesh-style data from the object
  vertexPositionsOut = plyIn.getVertexPositions();
  //faceIndicesOut = plyIn.getFaceIndices<size_t>();

}

void ComputeNormals(const KdTree<MyPoint> &_structure,const vector<MyPoint> &_points, vector<std::array<double, 3> > &_normals)
{
    // Making it local
    typedef Basket<MyPoint,WeightFunc,CovariancePlaneFit> PlaneFit;

    Scalar tmax = 0.1;
    int knei = 10;
    

    for(int i = 0; i < _points.size(); i++){

        // set evaluation point and scale at the ith coordinate
        const VectorType& p = _points.at(i).pos();
	
        // Here we now perform the fit
        PlaneFit _fit;
        // Set a weighting function instance
        _fit.setWeightFunc(WeightFunc(tmax));
        // Set the evaluation position
        _fit.init(p);

        for( auto idx : _structure.k_nearest_neighbors(p, knei) ){
            _fit.addNeighbor( _points[idx] );
        }
        _fit.finalize();
        
        
        if(_fit.isStable()){
            VectorType no = _fit.primitiveGradient(p).transpose();
            _normals[i] = {no[0],no[1], no[2]};
        }
    }
}

void ComputeCurvature(const KdTree<MyPoint> &_structure,const vector<MyPoint> &_points, std::vector<Scalar> &_Curvature)
{
    // Making it local
    typedef Basket<MyPoint,WeightFunc,OrientedSphereFit,   GLSParam> SphereFit;

    Scalar tmax = 0.1;
    int knei = 10;
    

    for(int i = 0; i < _points.size(); i++){

        // set evaluation point and scale at the ith coordinate
        const VectorType& p = _points.at(i).pos();
	
        // Here we now perform the fit
        SphereFit _fit;
        // Set a weighting function instance
        _fit.setWeightFunc(WeightFunc(tmax));
        // Set the evaluation position
        _fit.init(p);

        for( auto idx : _structure.k_nearest_neighbors(p, knei) ){
            _fit.addNeighbor( _points[idx] );
        }
        _fit.finalize();
        
        
        if(_fit.isStable()){
            _Curvature[i] = _fit.kappa();

        }
    }
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
    std::vector< std::array<double, 3> > positions;
    loadPointCloud(filename, positions);
    testStream.close();

    int n = positions.size();

    std::vector< std::array<double, 3> >normals (n);
    std::vector<Scalar> curvature(n);
    
 

    vector<MyPoint> points;
    for(int i = 0; i < n; i++){
        points.push_back({positions[i], normals[i]});
    }
   
	KdTree<MyPoint> structure(points);

    // run normal computation using CovariancePlaneFit
    ComputeNormals(structure, points, normals);
    //run curvature computation using OrientedSphereFit
    ComputeCurvature(structure, points, curvature);
    

    // visualize!
    polyscope::registerPointCloud("positions", positions);
    polyscope::getPointCloud("positions")->addVectorQuantity("normals", normals);
    polyscope::getPointCloud("positions")->addScalarQuantity("curvature", curvature);

      
      polyscope::show();    
}