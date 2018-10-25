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
    Mesh_Data meshData;
    NSCData nscData;

    //Attach data to all computers
    MeshInitializer meshInit(&nscData, &meshData);
    NSCInitializer nscInit(&nscData, &meshData);
    DataUpdater dataUpdater(&nscData, &meshData);

    //Solver objects
    MainSolver solver(&nscData, &meshData, &dataUpdater);

    //Attach object to outputData
    OutputDataManager outputDataManager(&nscData, &meshData);

    //Attach computers and data objects to inputManager
    InputDataManager inputDataManager(&nscInit, &solver, &meshInit, &dataUpdater, &meshData, &nscData, &outputDataManager);

    //Attach inputManager to event manager
    EventManager eventManager(&inputDataManager);

    //User operations here:
    eventManager.enterInputFile("input");

    eventManager.testSU2Function();

    cout << "Done!" << endl;
    return 0;
}
