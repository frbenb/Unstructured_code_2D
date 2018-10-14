#include "MeshInitializer.h"
#include <string>


MeshInitializer::MeshInitializer(NSCData *iNSCData, Mesh_Data *iMeshData) :
                                    _nscData(iNSCData),
                                    _meshData(iMeshData)
{

}

MeshInitializer::~MeshInitializer()
{
   
}

void MeshInitializer::initializeMesh(string& meshFilename){

    // pre-pass should give the following results
    unsigned int npoints = 16512;
    unsigned int ncells = 16384;
    unsigned int nghost = 128 + 128; // far field and airfoil
    unsigned int nface = 100000; // calculate this

}



