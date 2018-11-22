#ifndef DEF_INPUTDATAMANAGER_H
#define DEF_INPUTDATAMANAGER_H

//Project files
#include "NSCData.h"
#include "Mesh_Data.h"

#include "NSCInitializer.h"
#include "MainSolver.h"
#include "MeshInitializer.h"

#include "DataUpdater.h"

#include "OutputDataManager.h"

//Standard lib.
#include <iostream>
#include <string>
#include <fstream>

// This class allows to handle and manage the events given from the userinterface.
// Takes the roles of the following functions:
// readctrl()
// doInitProcess()


class InputDataManager
{

    public:
        //Default Constructor
        InputDataManager(NSCInitializer *iNSCInit, MainSolver *iSolverInit, MeshInitializer *iMeshInit, DataUpdater *iDataUpdater, Mesh_Data *iMeshData, NSCData *iNSCData, OutputDataManager *iOutputDataManager);

        //Default Destructor
        virtual ~InputDataManager();

        //This method allows to destroy the initializers, the updater and the data.
        // organizing destructions of objects variable part of design.
        void destroyDesignObjects();

        //This method allows to set the input filename in the database and to call the starting
        //process of data initialization
        void manageEntryFileName(std::string& iPath);

        //This method allows to process to the initialization of all the data.
        // Considered as the main routine calling the private object initializer.
        void doInitProcess();

        //This method allows to call the solving of the mesh and nsc data. 
        // Calls the MainSolver method responsible.
        void solve();

        // This method allows to read the input commands of the user. 
        // the input are presented as a text file.
        void readInputFile();

        // This method allows to print some data. For now, it calls a function to test
        // the SU2 reading method in Mesh_Data.
        void printDataSU2();

        // Prints the content of MeshData to text files
        void printMeshData();

    private:

        NSCInitializer      *nscInit_;
        MainSolver          *mainSolver_;
        MeshInitializer     *meshInit_;

        DataUpdater         *dataUpdater_;

        Mesh_Data           *meshData_;
        NSCData             *nscData_;

        OutputDataManager   *outputDataManager_;

};



#endif
