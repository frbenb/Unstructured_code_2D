#ifndef ARRAY_MEMORY_H_
#define ARRAY_MEMORY_H_

template <class T>
T* allocate1D(unsigned int dim);
template <class T>
T** allocate2D(unsigned int dim1, unsigned int dim2); 

template <class T>
T* deallocate1D(T* vec);
template <class T>
T** deallocate2D(T** vec, unsigned int dim1); 

#endif