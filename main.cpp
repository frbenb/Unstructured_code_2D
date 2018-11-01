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
    string fichier = "naca0012_129x129_1B_JAMESON.x";
    meshInit->initializeMesh(fichier);

    meshData->write_stuff();
    cout << "done" << endl;
    return 0;
}
