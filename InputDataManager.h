#ifndef DEF_INPUTDATAMANAGER_H
#define DEF_INPUTDATAMANAGER_H

//Project files
#include "NSCData.h"
#include "Mesh_Data.h"

#include "NSCInitializer.h"
#include "SolverInitializer.h"
#include "MeshInitializer.h"

#include "DataUpdater.h"

//Standard lib.
#include <iostream>
#include <string>


// This class allows to handle and manage the events given from the userinterface.
// Takes the roles of the following functions:
// initial_system()
// readctrl()
// connectivity
// readmesh (?? should it be in MeshInitializer object ??)
// new_mesh (?? should it be in MeshInitializer object ??)
// allocate_2D ()
// allocate_1D ()

class InputDataManager
{

    public:
        //Default Constructor
        InputDataManager(NSCInitializer *iNSCInit, SolverInitializer *iSolverInit, MeshInitializer *iMeshInit, DataUpdater *iDataUpdater, Mesh_Data *iMeshData, NSCData *iNSCData);

        //Default Destructor
        virtual ~InputDataManager();

        //TO DO: function of reading input file. Will be called by
        void manageInputFile(std::string iPath);

        //This method allows to destroy the initializers, the updater and the data.
        // organizing destructions of objects variable part of design.
        void destroyDesignObjects();

        //This method allows to set the input filename in the database and to call the starting
        //process of data initialization
        void manageEntryFileName(std::string iPath);

        //This method allows to process to the initialization of all the data.
        // Considered as the main routine calling the private object initializer.
        void doInitProcess();

        // This method allows to read the input commands of the user. 
        // the input are presented as a text file.
        void readInputFile();


    private:

        NSCInitializer      *_nscInit;
        SolverInitializer   *_solverInit;
        MeshInitializer     *_meshInit;

        DataUpdater         *_dataUpdater;

        Mesh_Data           *_meshData;
        NSCData             *_nscData;

};



#endif
