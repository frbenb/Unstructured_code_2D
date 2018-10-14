#include <iostream>

using namespace std;


//Projet files

#include "EventManager.h"

#include "Mesh_Data.h"
#include "NSCData.h"

#include "MeshInitializer.h"
#include "NSCInitializer.h"
#include "SolverInitializer.h"

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
    SolverInitializer solverInit(&nscData, &meshData);
    NSCInitializer nscInit(&nscData, &meshData);
    DataUpdater dataUpdater(&nscData, &meshData);

    //Attach object to outputData
    OutputDataManager outputDataManager(&nscData, &meshData);

    //Attach computers and data objects to inputManager
    InputDataManager inputDataManager(&nscInit, &solverInit, &meshInit, &dataUpdater, &meshData, &nscData);

    //Attach inputManager to event manager
    EventManager eventManager(&inputDataManager);


    //User operations here:
    eventManager.enterInputFile("input");

    cout << "Done!" << endl;
    return 0;
}
