

#include "InputDataManager.h"

using namespace std;

InputDataManager::InputDataManager(NSCInitializer *iNSCInit, SolverInitializer *iSolverInit, MeshInitializer *iMeshInit, DataUpdater *iDataUpdater, Mesh_Data *iMeshData, NSCData *iNSCData, OutputDataManager *iOutputDataManager):
                                nscInit_(iNSCInit),
                                solverInit_(iSolverInit),
                                meshInit_(iMeshInit),
                                dataUpdater_(iDataUpdater),
                                meshData_(iMeshData),
                                nscData_(iNSCData),
                                outputDataManager_(iOutputDataManager)
{

}

InputDataManager::~InputDataManager()
{
   
}


void InputDataManager::manageEntryFileName(std::string iPath)
{
    //Set ctrlfilename in data base
    nscData_->ctrlfilename_ = iPath;

    //Call of Do init.
    doInitProcess();


}


void InputDataManager::doInitProcess()
{
    // initial system
    nscInit_->initial_system();

    //1. Read input file. Equivalent as readtrcl()
    readInputFile();

    //2. Read mesh file. Equivalent as readmesh()
    meshInit_->initializeMesh(nscData_->meshfilename_);

    // Initial_flow_parameters
    nscInit_->initial_flow_parameters();

    // Initial rk scheme (is Empty)
    nscInit_->initial_rk_scheme();

    //Initial field: Put here. Implemented but need to allocate memory.
   

    //Update Boundary
    dataUpdater_->update_boundary();

}

void InputDataManager::readInputFile()
{   
    //Open input file in reading mode.
    ifstream inputFile(nscData_->ctrlfilename_, ios::in);

    string title;
    string str, str1, str2;
    string meshFileName;

    int topo;
    int imax;
    int jmax;
    int itl;
    int itu;


    if(inputFile)
    {
        //Reading input.
        inputFile >> title;

        inputFile >> str;
        inputFile >> topo;
        inputFile >> str1;

        inputFile >> imax;
        inputFile >> str;
        inputFile >> jmax;
        inputFile >> str1;
        inputFile >> itl;
        inputFile >> str2;
        inputFile >> itu;

        inputFile >> str;
        inputFile >> meshFileName;

        //Mapping.
        nscData_->meshfilename_ = meshFileName;


        //Close file.
        inputFile.close();
    }
    else
    {
        cout << "ERROR: Cannot open " << nscData_->ctrlfilename_ << "." << endl;
        return;
    }
    

}

void InputDataManager::printDataSU2()
{
    outputDataManager_->showSU2ReadingData();
}





