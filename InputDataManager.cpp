

#include "InputDataManager.h"

using namespace std;

InputDataManager::InputDataManager(NSCInitializer *iNSCInit, MainSolver *iSolverInit, MeshInitializer *iMeshInit, DataUpdater *iDataUpdater, Mesh_Data *iMeshData, NSCData *iNSCData, OutputDataManager *iOutputDataManager):
                                nscInit_(iNSCInit),
                                mainSolver_(iSolverInit),
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

    // Initial rk scheme (work in progress)
    

    //Initial field: Put here. Implemented but need to allocate memory.
    nscInit_->initial_field();

    //Update Boundary
    dataUpdater_->update_boundary();

}

void InputDataManager::solve()
{
    //Here is called the method of MainSolver to solve problem 
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

    float mach, alpha, cl, dcl, reynolds;
    double xref, yref, cmac;
    int residualSmoothing, itc;
    int level, iter, mglevel, rk;
    double cfl;
    double dissip, vis2, vis4;

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

        inputFile >> str;
        inputFile >> str;
        inputFile >> str;
        inputFile >> str;
        inputFile >> str;

        inputFile >> mach;
        inputFile >> alpha;
        inputFile >> cl;    
        inputFile >> dcl;
        inputFile >> reynolds;

        inputFile >> str;
        inputFile >> str;
        inputFile >> str;

        inputFile >> xref;
        inputFile >> yref;
        inputFile >> cmac;

        inputFile >> str;
        inputFile >> str;
        inputFile >> str;

        inputFile >> dissip;
        inputFile >> vis2;
        inputFile >> vis4;

        inputFile >> str;
        inputFile >> str;
        inputFile >> str;

        inputFile >> residualSmoothing;

        inputFile >> str;

        inputFile >> itc;

        inputFile >> str;
        inputFile >> str;
        inputFile >> str;
        inputFile >> str;
        inputFile >> str;
        inputFile >> str;
        inputFile >> str;

        inputFile >> level;
        inputFile >> iter;
        inputFile >> mglevel;
        inputFile >> rk;
        inputFile >> cfl;

        //Mapping.
        nscData_->meshfilename_ = meshFileName;
        nscData_->title_ = title;
        
      
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





