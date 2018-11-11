

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


void InputDataManager::manageEntryFileName(std::string& iPath)
{
    //Set ctrlfilename in data base
    nscData_->ctrlfilename_ = iPath;

    //Call of Do init.
    doInitProcess();

    //Solve
    solve();

}


void InputDataManager::doInitProcess()
{
    // initial system
    nscInit_->initial_system();

    //1. Read input file. Equivalent as readtrcl()
    readInputFile();

    //2. Read mesh file. Equivalent as readmesh()
    meshInit_->initializeMesh(nscData_->meshfilename_);

    //Call metric.
    meshInit_->metric();

    // Initial_flow_parameters
    nscInit_->initial_flow_parameters();

    // Initial rk scheme (work in progress)
    nscInit_->initial_rk_scheme();

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

    string str, str1, str2;
    unsigned int level , mglevel;
    
    

    if(inputFile.is_open())
    {
        //Reading input.
        inputFile >> nscData_->title_;

        inputFile >> str;
        inputFile >> nscData_->topo_;
        inputFile >> str1;

        inputFile >> str; //imax_;
        inputFile >> str;
        inputFile >> str; //jmax_;
        inputFile >> str1;
        inputFile >> meshData_->itl_;
        inputFile >> str2;
        inputFile >> meshData_->itu_;

        inputFile >> str;
        inputFile >> nscData_->meshfilename_;

        inputFile >> str;
        inputFile >> str;
        inputFile >> str;
        inputFile >> str;
        inputFile >> str;

        inputFile >> nscData_->mach_;
        inputFile >> nscData_->alpha_;

        std::string str_cl;
        inputFile >> str_cl;
        if (str_cl == "no"){
            nscData_->cltarget_ = 0;
        }
        else{
            nscData_->cltarget_ = std::stoi(str_cl);
        }


        inputFile >> nscData_->dcl_;
        inputFile >> nscData_->reynolds_;

        inputFile >> str;
        inputFile >> str;
        inputFile >> str;

        inputFile >> nscData_->xref_;
        inputFile >> nscData_->yref_;
        inputFile >> nscData_->cmac_;

        inputFile >> str;
        inputFile >> str;
        inputFile >> str;

        inputFile >> nscData_->dissip_;
        inputFile >> nscData_->vis2_;
        inputFile >> nscData_->vis4_;

        inputFile >> str;
        inputFile >> str;
        inputFile >> str;

        inputFile >> nscData_->ressmo_;

        inputFile >> str;

        inputFile >> nscData_->nitc_;

        inputFile >> str;
        inputFile >> str;
        inputFile >> str;
        inputFile >> str;
        inputFile >> str;
        inputFile >> str;
        inputFile >> str;

        inputFile >> level;
        inputFile >> nscData_->niter_;
        inputFile >> mglevel;
        inputFile >> nscData_->rk_;
        inputFile >> nscData_->cfl_;        
      
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
    outputDataManager_-> writeOutputTecplot();
}





