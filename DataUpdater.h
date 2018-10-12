#ifndef DEF_DATAUPDATER_H
#define DEF_DATAUPDATER_H


//Standard lib.
#include <iostream>

//Projet files
#include "NSCData.h"
#include "Mesh_Data.h"


// This method allows to make the update of the solution and of the boundary when called.
//Takes the roles of the following functions:
// update_solution()
// update_boundary()

class DataUpdater
{

    public:
        //Default Constructor
        DataUpdater(NSCData *iNSCData, Mesh_Data *iMeshData);

        //Default Destructor
        virtual ~DataUpdater();

        

    private:

        NSCData     *_nscData;
        Mesh_Data   *_meshData;


};


#endif




