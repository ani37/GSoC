#include <cmath>
#include <algorithm>
#include <iostream>
#include <vector>
#include<stdio.h>
#include "happly.h"
//Ponca
#include <Ponca/src/Fitting/basket.h>
#include <Ponca/src/Fitting/gls.h>
#include <Ponca/src/Fitting/orientedSphereFit.h>
#include <Ponca/src/Fitting/weightFunc.h>
#include <Ponca/src/Fitting/weightKernel.h>
#include "Eigen/Eigen"

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
 
    PONCA_MULTIARCH inline MyPoint(std::array<Scalar, 3> poss, std::array<Scalar, 3>  tem_norm)
        : m_pos   (Eigen::Map< const VectorType >(poss.begin())),
        m_normal(Eigen::Map< const VectorType >(tem_norm.begin()))
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
 
 
template<typename Fit>
VectorType test_fit(Fit& _fit,
              std::vector<std::array<Scalar, 3> > poss,
              std::vector<std::array<Scalar, 3> > tem_norm,
              int _n,
              const VectorType& _p)
{
    Scalar tmax = 100.0;
 
    // Set a weighting function instance
    _fit.setWeightFunc(WeightFunc(tmax));
 
    // Set the evaluation position
    _fit.init(_p);

    VectorType vec;
 
    // Iterate over samples and _fit the primitive
    // A MyPoint instance is generated on the fly to bind the raw arrays to the
    // library representation. No copy is done at this step.
    for(int i = 0; i!= _n; i++)
    {
        _fit.addNeighbor(MyPoint(poss[i], tem_norm[i]));
    }
 
    //finalize fitting
    _fit.finalize();
 
    //Test if the fitting ended without errors
    if(_fit.isStable())
    {
        // cout << "Center: [" << _fit.center().transpose() << "] ;  radius: " << _fit.radius() << endl;
 
        // cout << "Pratt normalization"
        //     << (_fit.applyPrattNorm() ? " is now done." : " has already been applied.") << endl;
 
        // // Play with fitting output
        // cout << "Value of the scalar field at the initial point: "
        //     << _p.transpose()
        //     << " is equal to " << _fit.potential(_p)
        //     << endl;
 
        // cout << "It's gradient at this place is equal to: "
        //     << _fit.primitiveGradient(_p).transpose()
        //     << endl;
        vec = _fit.primitiveGradient(_p).transpose();
 
        // cout << "Fitted Sphere: " << endl
        //     << "\t Tau  : "      << _fit.tau()             << endl
        //     << "\t Eta  : "      << _fit.eta().transpose() << endl
        //     << "\t Kappa: "      << _fit.kappa()           << endl;
 
        // cout << "The initial point " << _p.transpose()              << endl
        //     << "Is projected at   " << _fit.project(_p).transpose() << endl;
    }

    return vec;
}


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
   
    std::vector<std::array<Scalar, 3> >  tem_normal(n);

    // //Find normals for each positions in test fit.
    std::vector<std::array<Scalar, 3> >  normals;

    int dim = 0;
    for(int i = 0; i < n; i++){

        // set evaluation point and scale at the first coordinate
        VectorType p ({positions[i][0], positions[i][1],positions[i][2]});
        
        // Here we now perform the fit, starting from a raw interlaced buffer, without
        // any data duplication
        Fit fit;
        VectorType  no = test_fit(fit, positions, tem_normal, n, p);
        
        //cout << no <<   "\n\n";
        
        normals.push_back({no[0], no[1], no[2]});   
        dim += 2*DIMENSION; 
    }
    
    // visualize!
      polyscope::registerPointCloud("positions", positions);
      polyscope::registerPointCloud("normals", normals);
      polyscope::show();
}