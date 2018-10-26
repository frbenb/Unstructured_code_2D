#include <iostream>

using namespace std;


//Projet files

#include "EventManager.h"

#include "Mesh_Data.h"
#include "NSCData.h"

#include "MeshInitializer.h"
#include "NSCInitializer.h"
#include "MainSolver.h"

#include "OutputDataManager.h"

#include "DataUpdater.h"

int main()
{

    //Entry point. run program from here.

    //Objects
    Mesh_Data* meshData = new Mesh_Data();
    NSCData* nscData = new NSCData();

    //Attach data to all computers
    MeshInitializer* meshInit = new MeshInitializer(nscData, meshData);
    NSCInitializer* nscInit = new NSCInitializer(nscData, meshData);
    DataUpdater* dataUpdater = new DataUpdater(nscData, meshData);

    //Solver objects
    MainSolver* solver = new MainSolver(nscData, meshData, dataUpdater);

    //Attach object to outputData
    OutputDataManager* outputDataManager = new OutputDataManager(nscData, meshData);

    //Attach computers and data objects to inputManager
    InputDataManager* inputDataManager  = new InputDataManager(nscInit, solver, meshInit, dataUpdater, meshData, nscData, outputDataManager);

    //Attach inputManager to event manager
    EventManager* eventManager = new EventManager(inputDataManager);

    string inputfile = "naca0012_129x129_1B_JAMESON.x";

    //User operations here:
    eventManager->enterInputFile(inputfile);

    eventManager->testSU2Function();

    cout << "Done!" << endl;
    return 0;
}
