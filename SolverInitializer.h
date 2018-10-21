#ifndef DEF_MAINSOLVER_H
#define DEF_MAINSOLVER_H


//Standard lib.
#include <iostream>

//Projet files
#include "NSCData.h"
#include "Mesh_Data.h"


// This class allows to store the data according to NSC structure presented in the
// NSCode. Is called by the solvers and initializer through multiple processes.
//
// 
//
class MainSolver
{

    public:
        //Default Constructor
        MainSolver(NSCData *iNSCData, Mesh_Data *iMeshData);

        //Default Destructor
        virtual ~MainSolver();

        

    private:

        NSCData     *_nscData;
        Mesh_Data   *_meshData;
        

};


#endif




