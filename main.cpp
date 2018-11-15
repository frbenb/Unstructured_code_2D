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
#include "FluxComputer.h"

int main()
{

    //Entry point. run program from here.

    //Objects
    Mesh_Data* meshData = new Mesh_Data();
    NSCData* nscData = new NSCData();
    cout << "Main, Objects, Done!" << endl;

    //Attach data to all computers
    MeshInitializer* meshInit = new MeshInitializer(nscData, meshData);
    NSCInitializer* nscInit = new NSCInitializer(nscData, meshData);
    DataUpdater* dataUpdater = new DataUpdater(nscData, meshData);
    FluxComputer* fluxComputer = new FluxComputer(nscData, meshData);
    cout << "Main, Attach data, Done!" << endl;

    //Solver objects
    MainSolver* solver = new MainSolver(nscData, meshData, dataUpdater);
    cout << "Main, Solver, Done!" << endl;

    //Attach object to outputData
    OutputDataManager* outputDataManager = new OutputDataManager(nscData, meshData);
    cout << "Main, outputData, Done!" << endl;

    //Attach computers and data objects to inputManager
    InputDataManager* inputDataManager  = new InputDataManager(nscInit, solver, meshInit, dataUpdater, meshData, nscData, outputDataManager);
    cout << "Main, computer object, Done!" << endl;

    //Attach inputManager to event manager
    EventManager* eventManager = new EventManager(inputDataManager);
    cout << "Main, inputManager, Done!" << endl;

    string inputfile = "input";
    
    //User operations here:
    eventManager->enterInputFile(inputfile);
    cout << "Main, entreInputFile, Done!" << endl;

    //outputDataManager->calculate_node_coefficients();

    eventManager->testSU2Function();
    cout << "Main, eventManager, Done!" << endl;

    meshData->write_stuff();
    cout << "Main, writestuff, Done!" << endl;
    
    fluxComputer->calculateArtificialDissipRoe(); 

    cout << "Done!" << endl;
    return 0;
}
