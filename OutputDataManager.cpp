#include "OutputDataManager.h"


using namespace std;

OutputDataManager::OutputDataManager(NSCData *iNSCData, Mesh_Data *iMeshData):
                                    nscData_(iNSCData), meshData_(iMeshData)
{
    // Nothing.
}

OutputDataManager::~OutputDataManager()
{
    if (nscData_ != nullptr)
    {
        delete nscData_;
    }
    nscData_ = nullptr;

    if (meshData_ != nullptr)
    {
        delete meshData_;
    }
    meshData_ = nullptr;

}

void OutputDataManager::showSU2ReadingData()
{
    cout << "Hello world!" << endl;
}

















