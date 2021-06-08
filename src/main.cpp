#include <cmath>
#include <algorithm>
#include <iostream>
#include <vector>

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
class MyPoint
{
public:
    enum {Dim = DIMENSION};
    typedef double Scalar;
    typedef Eigen::Matrix<Scalar, Dim, 1>   VectorType;
    typedef Eigen::Matrix<Scalar, Dim, Dim> MatrixType;
 
    PONCA_MULTIARCH inline MyPoint(Scalar* _interlacedArray, int _pId)
        : m_pos   (Eigen::Map< const VectorType >(_interlacedArray + Dim*2*_pId  )),
        m_normal(Eigen::Map< const VectorType >(_interlacedArray + Dim*2*_pId+Dim))
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
              Scalar* _interlacedArray,
              int _n,
              const VectorType& _p)
{
    Scalar tmax = 100.0;
    
    //
    VectorType norm;
 
    // Set a weighting function instance
    _fit.setWeightFunc(WeightFunc(tmax));
 
    // Set the evaluation position
    _fit.init(_p);
 
    // Iterate over samples and _fit the primitive
    // A MyPoint instance is generated on the fly to bind the raw arrays to the
    // library representation. No copy is done at this step.
    for(int i = 0; i!= _n; i++)
    {
        _fit.addNeighbor(MyPoint(_interlacedArray, i));
    }
 
    //finalize fitting
    _fit.finalize();
 
    //Test if the fitting ended without errors
    if(_fit.isStable())
    {
        cout << "Center: [" << _fit.center().transpose() << "] ;  radius: " << _fit.radius() << endl;
 
        cout << "Pratt normalization"
            << (_fit.applyPrattNorm() ? " is now done." : " has already been applied.") << endl;
 
        // Play with fitting output
        cout << "Value of the scalar field at the initial point: "
            << _p.transpose()
            << " is equal to " << _fit.potential(_p)
            << endl;
 
        cout << "It's gradient at this place is equal to: "
            << _fit.primitiveGradient(_p).transpose()
            << endl;
        // set normals to return
        norm = _fit.primitiveGradient(_p).transpose();
 
        cout << "Fitted Sphere: " << endl
            << "\t Tau  : "      << _fit.tau()             << endl
            << "\t Eta  : "      << _fit.eta().transpose() << endl
            << "\t Kappa: "      << _fit.kappa()           << endl;
 
        cout << "The initial point " << _p.transpose()              << endl
            << "Is projected at   " << _fit.project(_p).transpose() << endl;
    }
    return norm;
}


 
// Build an interlaced array containing _n position and normal vectors
Scalar* buildInterlacedArray(vector<Vector3 > pos, vector<Vector3 > norms )
{
    int _n = pos.size();
    Scalar* interlacedArray = new Scalar[uint8_t(2*DIMENSION*_n)];
 
    for(int k=0; k<_n; ++k)
    {
        
        // Grab position coordinates and store them as raw buffer
        interlacedArray[2*DIMENSION*k] = pos[k][0];
        interlacedArray[2*DIMENSION*k+1] = pos[k][1];
        interlacedArray[2*DIMENSION*k+2] = pos[k][2];

        // Grab normal coordinates and store them as raw buffer
        interlacedArray[2*DIMENSION*k + DIMENSION ] = norms[k][0];
        interlacedArray[2*DIMENSION*k + DIMENSION + 1] =norms[k][1];
        interlacedArray[2*DIMENSION*k + DIMENSION + 2] = norms[k][2];
        
    }
 
    return interlacedArray;
}
 

int main(int argc, char **argv) {

   

    polyscope::init();

    std::unique_ptr<SurfaceMesh> mesh;
    std::unique_ptr<VertexPositionGeometry> geometry;
    std::tie(mesh, geometry) = readSurfaceMesh("hippo.ply");
    geometry->requireVertexPositions();


    std::vector<Vector3> pos, norms;


    // Build the node positions
    for (Face f : mesh->faces()) {

    // Compute center for face
    Vector3 c = Vector3::zero();
    for (Vertex v : f.adjacentVertices()) {
        c += geometry->inputVertexPositions[v];
    }
    c /= f.degree();

    pos.push_back(c);
    }

    // Set vertex tangent spaces
    geometry->requireVertexTangentBasis();
    for(Vertex v : mesh->vertices()) {
        norms.push_back(geometry->vertexTangentBasis[v][0]);
    }

    Scalar *interlacedArray = buildInterlacedArray(pos, norms);

    int n = pos.size();
    // Find normals for each positions in test fit.

    vector<VectorType> normals;
    int dim = 0;
    for(int i = 0; i < n; i++){

        // set evaluation point and scale at the first coordinate
        VectorType p (interlacedArray + dim);
        
        // Here we now perform the fit, starting from a raw interlaced buffer, without
        // any data duplication
        Fit fit;
        VectorType  n = test_fit(fit, interlacedArray, pos.size(), p);
        normals.push_back(n);
        dim += 2*DIMENSION;
    }
    
    
    // visualize!
    polyscope::registerPointCloud("positions", pos);
    polyscope::registerPointCloud("normals", norms);
    polyscope::show();
}