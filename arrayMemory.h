#ifndef ARRAY_MEMORY_H_
#define ARRAY_MEMORY_H_

unsigned int* allocate1Dint(unsigned int dim);
unsigned int** allocate1Dintstar(unsigned int dim);
unsigned int** allocate2Dint(unsigned int dim1, unsigned int dim2); 

double* allocate1Ddbl(unsigned int dim);
double** allocate2Ddbl(unsigned int dim1, unsigned int dim2); 

template <class T>
T* deallocate1D(T* vec);
template <class T>
T** deallocate2D(T** vec, unsigned int dim1); 

#endif