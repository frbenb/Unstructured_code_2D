#ifndef DEF_OUTPUTDATAMANAGER_H
#define DEF_OUTPUTDATAMANAGER_H


//Standard lib.
#include <iostream>
#include <fstream>
#include <string>

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

        // This methos allows to show the results of storing the Mesh_Data
        // and NSCData by reading the SU2 file. Testing function read_SU2.
        void showSU2ReadingData();
    

    private:

        NSCData     *nscData_;
        Mesh_Data   *meshData_;


};


#endif
