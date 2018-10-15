#include "EventManager.h"



EventManager::EventManager(InputDataManager *iInputDataManager):
                            inputDataManager_(iInputDataManager)                                                                
{

}

EventManager::~EventManager()
{

    
}

void EventManager::enterInputFile(std::string iPath)
{
    //Manage the command of user in an input file.
    inputDataManager_->manageEntryFileName(iPath);
}

void EventManager::testSU2Function()
{   
    // General function to allow user to print some data.

    inputDataManager_->printDataSU2();
    

}







