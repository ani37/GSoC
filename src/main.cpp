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
#include <Ponca/src/Fitting/basket.h>
#include <Ponca/src/Fitting/gls.h>
#include <Ponca/src/Fitting/orientedSphereFit.h>
#include <Ponca/src/Fitting/weightFunc.h>
#include <Ponca/src/Fitting/weightKernel.h>
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
 
    PONCA_MULTIARCH inline MyPoint(const std::array<Scalar, 3>&poss,const std::array<Scalar, 3>&  normm)
        : m_pos   (Eigen::Map< const VectorType >(poss.begin())),
        m_normal(Eigen::Map< const VectorType >(normm.begin()))
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
typedef Basket<MyPoint,WeightFunc,OrientedSphereFit,   GLSParam> Fit;


void loadPolygonSoup_PLY(std::string filename, std::vector<std::array<double, 3>>& vertexPositionsOut,
                         std::vector<std::vector<size_t>>& faceIndicesOut) {

  happly::PLYData plyIn(filename);

  // Get mesh-style data from the object
  vertexPositionsOut = plyIn.getVertexPositions();
  //faceIndicesOut = plyIn.getFaceIndices<size_t>();

}
 

int main(int argc, char **argv) {

   
    //freopen("output.txt", "w", stdout);
    polyscope::init();

    string filename = "hippo.ply";

    std::ifstream testStream(filename);
    if (!testStream) {
        return 0;
    }
    std::vector<std::array<Scalar, 3> > positions;

    // not required here
    std::vector<std::vector<size_t>> faceIndicesOut;

    loadPolygonSoup_PLY(filename, positions, faceIndicesOut);
    testStream.close();

    int n = positions.size();
    std::vector<VectorType >  normals(n);
    

    vector<MyPoint> points;
    for(int i = 0; i < n; i++){
        points.push_back({positions[i], {0,0,0}});
    }

    Scalar tmax = 0.2;

	KdTree<MyPoint> structure(points);


    int dim = 0;
    for(int i = 0; i < n; i++){

        // set evaluation point and scale at the ith coordinate
        
        const VectorType& p = points.at(i).pos();
	
        // Here we now perform the fit
        Fit fit;
        fit.init(p);

        // Set a weighting function instance
        fit.setWeightFunc(WeightFunc(tmax));
    
        // Iterate over samples and _fit the primitive
        // A MyPoint instance is generated on the fly to bind the raw arrays to the
        // library representation. No copy is done at this step.

        vector<MyPoint> pos;
        for (int j : structure.range_neighbors(p, tmax)) {
			pos.push_back(points[i]);
		}
       
        //test_fit(fit, points, structure.range_neighbors(p, tmax), tmax);
        fit.compute( pos.begin(), pos.end() );
        
        //finalize fitting
        fit.finalize();
        
        if(fit.isStable()){
        normals[i] = fit.primitiveGradient(p).transpose();
        }
        
    }
    
    // visualize!
      polyscope::registerPointCloud("positions", positions);
      polyscope::registerPointCloud("normals", normals);
      polyscope::show();
}