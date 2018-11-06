#ifndef DEF_DATAUPDATER_H
#define DEF_DATAUPDATER_H


//Standard lib.
#include <iostream>
#include <math.h>

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
    
        //This method allows to update the boundary mesh on the primitive variables.
        // Update wall and far field.
        void update_boundary();

        // This method allows to update the solution in the computation loop.
        void update_solution(int iStage);

        

    private:

        NSCData     *nscData_;
        Mesh_Data   *meshData_;


};


#endif




