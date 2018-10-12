
#include "DataUpdater.h"


DataUpdater::DataUpdater(NSCData *iNSCData, Mesh_Data *iMeshData) :
                                _nscData(iNSCData),
                                _meshData(iMeshData)
{

}

DataUpdater::~DataUpdater()
{
    if(_nscData != nullptr )
    {
        delete _nscData;
    }
    _nscData = nullptr;

    if(_meshData != nullptr)
    {
        delete _meshData;
    }
    _meshData = nullptr;

}

