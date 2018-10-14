

#include "InputDataManager.h"


InputDataManager::InputDataManager(NSCInitializer *iNSCInit, SolverInitializer *iSolverInit, MeshInitializer *iMeshInit, DataUpdater *iDataUpdater, Mesh_Data *iMeshData, NSCData *iNSCData):
                                _nscInit(iNSCInit),
                                _solverInit(iSolverInit),
                                _meshInit(iMeshInit),
                                _dataUpdater(iDataUpdater),
                                _meshData(iMeshData),
                                _nscData(iNSCData)
{

}

InputDataManager::~InputDataManager()
{
   destroyDesignObjects();
}

void InputDataManager::destroyDesignObjects()
{
     if (_nscInit !=nullptr)
    {
        delete _nscInit;
    }
    _nscInit = nullptr;

    if (_solverInit !=nullptr)
    {
        delete _solverInit;
    }
    _solverInit = nullptr;

    if (_meshInit !=nullptr)
    {
        delete _meshInit;
    }
    _meshInit = nullptr;

    if (_dataUpdater !=nullptr)
    {
        delete _dataUpdater;
    }
    _dataUpdater = nullptr;

    if (_meshData !=nullptr)
    {
        delete _meshData;
    }
    _meshData = nullptr;

    if (_nscData !=nullptr)
    {
        delete _nscData;
    }
    _nscData = nullptr;

}


void InputDataManager::manageEntryFileName(std::string iPath)
{
    //Set ctrlfilename in data base
    _nscData->ctrlfilename_ = iPath;

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




