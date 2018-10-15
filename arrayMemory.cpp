#include "arrayMemory.h"

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