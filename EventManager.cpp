#include "EventManager.h"


EventManager::EventManager(InputDataManager *iInputDataManager) :
                                    _inputDataManager(iInputDataManager)
{

}

EventManager::~EventManager()
{

    if (_inputDataManager != nullptr)
    {
        delete _inputDataManager;
    }
    _inputDataManager = nullptr;
}

void EventManager::enterInputFile(std::string iPath)
{

}






