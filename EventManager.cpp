#include "EventManager.h"


EventManager::EventManager(InputDataManager *iInputDataManager) :
                                    inputDataManager_(iInputDataManager)
{

}

EventManager::~EventManager()
{

    if (inputDataManager_ != nullptr)
    {
        delete inputDataManager_;
    }
    inputDataManager_ = nullptr;
}

void EventManager::enterInputFile(std::string iPath)
{
    //Manage the command of user in an input file.
    inputDataManager_->manageInputFile(iPath);
}

void EventManager::printData()
{
    

}







