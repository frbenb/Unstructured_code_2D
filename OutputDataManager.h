#ifndef DEF_OUTPUTDATAMANAGER_H
#define DEF_OUTPUTDATAMANAGER_H


//Standard lib.
#include <iostream>

//Projet files
#include "NSCData.h"
#include "Mesh_Data.h"


// This class allows to organize some particular data for future use.
// output file for tecplot using.
//
class OutputDataManager
{

    public:
        //Default Constructor
        OutputDataManager(NSCData *iNSCData, Mesh_Data *iMeshData);

        //Default Destructor
        virtual ~OutputDataManager();


    private:

        NSCData     *nscData_;
        Mesh_Data   *meshData_;


};


#endif

