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
    unsigned int nghosts = 128 + 128; // far field and airfoil
    unsigned int nfaces = 100000; // calculate this




}

template <class T>
T* allocate1D(unsigned int dim){
    T* vec = new T[dim];
    return vec;
}

template <class T>
T** allocate2D(unsigned int dim1, unsigned int dim2){
    T** vec = new T*[dim1];
    for (unsigned int i = 0; i < dim1; i++){
        vec[i] = new T[dim2];
    }
    return vec;
}

template <class T>
T* deallocate1D(T* vec){
    delete [] vec;
    return nullptr;
}

template <class T>
T** deallocate2D(T** vec, unsigned int dim1){
    for (unsigned int i = 0; i < dim1){
        delete [] vec[i];
    }
    delete [] vec;
    return nullptr;
}

