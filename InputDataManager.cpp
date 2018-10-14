

#include "InputDataManager.h"

using namespace std;

InputDataManager::InputDataManager(NSCInitializer *iNSCInit, SolverInitializer *iSolverInit, MeshInitializer *iMeshInit, DataUpdater *iDataUpdater, Mesh_Data *iMeshData, NSCData *iNSCData):
                                nscInit_(iNSCInit),
                                solverInit_(iSolverInit),
                                meshInit_(iMeshInit),
                                dataUpdater_(iDataUpdater),
                                meshData_(iMeshData),
                                nscData_(iNSCData)
{

}

InputDataManager::~InputDataManager()
{
   destroyDesignObjects();
}

void InputDataManager::destroyDesignObjects()
{
     if (nscInit_ !=nullptr)
    {
        delete nscInit_;
    }
    nscInit_ = nullptr;

    if (solverInit_ !=nullptr)
    {
        delete solverInit_;
    }
    solverInit_ = nullptr;

    if (meshInit_ !=nullptr)
    {
        delete meshInit_;
    }
    meshInit_ = nullptr;

    if (dataUpdater_ !=nullptr)
    {
        delete dataUpdater_;
    }
    dataUpdater_ = nullptr;

    if (meshData_ !=nullptr)
    {
        delete meshData_;
    }
    meshData_ = nullptr;

    if (nscData_ !=nullptr)
    {
        delete nscData_;
    }
    nscData_ = nullptr;

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
    //Here is the general routine to initialize all data.


    //1. Read input file.
    readInputFile();


    //2. Read mesh file.


}

void InputDataManager::readInputFile()
{   
    
    ifstream inputFile;
    inputFile.open(nscData_->ctrlfilename_);




    if(inputFile)
    {
        

    }
    else
    {
        cout << "ERROR: Cannot open " << nscData_->ctrlfilename_ << "." << endl;
    }
    

    // Get the readmesh filename.




}



