

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

    if(outputDataManager_ != nullptr)
    {
        delete outputDataManager_;
    }
    outputDataManager_ = nullptr;
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
    //1. Read input file.
    readInputFile();

    //2. Read mesh file.
    meshData_->read_SU2(nscData_->meshfilename_);

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



