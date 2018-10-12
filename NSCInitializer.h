#ifndef DEF_NSCINITIALIZER_H
#define DEF_NSCINITIALIZER_H


//Standard lib.
#include <iostream>

//Projet files
#include "NSCData.h"
#include "Mesh_Data.h"


// This class allows to
// Takes the roles of the following functions:
// initial_flow_parameters()
// initial_field()
//
class NSCInitializer
{

    public:
        //Default Constructor
        NSCInitializer(NSCData *iNSCData, Mesh_Data *iMeshData);

        //Default Destructor
        virtual ~NSCInitializer();


    private:

        NSCData     *_nscData;
        Mesh_Data   *_meshData;

};


#endif
