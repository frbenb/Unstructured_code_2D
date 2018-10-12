#ifndef DEF_SOLVERINITIALIZER_H
#define DEF_SOLVERINITIALIZER_H


//Standard lib.
#include <iostream>

//Projet files
#include "NSCData.h"
#include "Mesh_Data.h"


// This class allows to store the data according to NSC structure presented in the
// NSCode. Is called by the solvers and initializer through multiple processes.
//
// initial_rk_scheme()
//
class SolverInitializer
{

    public:
        //Default Constructor
        SolverInitializer(NSCData *iNSCData, Mesh_Data *iMeshData);

        //Default Destructor
        virtual ~SolverInitializer();

        

    private:

        NSCData     *_nscData;
        Mesh_Data   *_meshData;


};


#endif




