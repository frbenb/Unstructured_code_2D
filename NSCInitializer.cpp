#include "NSCInitializer.h"


NSCInitializer::NSCInitializer(NSCData *iNSCData, Mesh_Data *iMeshData) :
                                    _nscData(iNSCData),
                                    _meshData(iMeshData)
{

}

NSCInitializer::~NSCInitializer()
{
    if (_nscData != nullptr)
    {
        delete _nscData;
    }
    _nscData = nullptr;

    if (_meshData != nullptr)
    {
        delete _meshData;
    }
    _meshData = nullptr;

}
