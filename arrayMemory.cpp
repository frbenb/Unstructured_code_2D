#include "arrayMemory.h"

unsigned int* allocate1Dint(unsigned int dim){
    unsigned int* vec = new unsigned int[dim];
    return vec;
}

unsigned int** allocate1Dintstar(unsigned int dim){
    unsigned int** vec = new unsigned int*[dim];
    return vec;
}


unsigned int** allocate2Dint(unsigned int dim1, unsigned int dim2){
    unsigned int** vec = new unsigned int*[dim1];
    for (unsigned int i = 0; i < dim1; i++){
        vec[i] = new unsigned int[dim2];
    }
    return vec;
}

double* allocate1Ddbl(unsigned int dim){
    double* vec = new double[dim];
    return vec;
}


double** allocate2Ddbl(unsigned int dim1, unsigned int dim2){
    double** vec = new double*[dim1];
    for (unsigned int i = 0; i < dim1; i++){
        vec[i] = new double[dim2];
    }
    return vec;
}

template <class T>
T* deallocate1D(T* vec){
    if (vec != nullptr){
        delete [] vec;
    }
    return nullptr;
}

template <class T>
T** deallocate2D(T** vec, unsigned int dim1){
    if (vec != nullptr){
        for (unsigned int i = 0; i < dim1){
            if (vec[i] != nullptr){
                delete [] vec[i];
            }
        }
        delete [] vec;
    }
    return nullptr;
}