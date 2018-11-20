#include "EventManager.h"


EventManager::EventManager(InputDataManager *iInputDataManager) :
                                    inputDataManager_(iInputDataManager),
                                    userChoice_(0),
                                    isProgramEnded_(false)
{

}

EventManager::~EventManager()
{

}

void EventManager::startExecution()
{
    //Main loop.
    do
    {
        //1. Display User Choices.
        cout << endl << "Welcome to NSCode++!" << endl;
        cout << "Please choose one of the following options:" << endl << endl;
        cout << "1. Initialize solution." << endl;
        cout << "2. Initialize and solve solution." << endl;
        cout << "3. Generate ASCII for Tecplot Visualization." << endl;
        cout << "4. Quit." << endl;
        cin >> userChoice_;

        logicChoice(userChoice_);

    }
    while(!isProgramEnded_);

    cout << "Goodbye!" << endl;

}

void EventManager::logicChoice(int iUserChoice)
{
    //1. Enumration in order of display in startExectution method
    enum TypesChoices {dummy, INIT, INIT_SOLVE, GEN_TECPLOT, QUIT};


    switch(iUserChoice)
    {

         case INIT:
            inputDataManager_->doInitProcess();
         break;

         case INIT_SOLVE:

            inputDataManager_->doInitProcess();
            inputDataManager_->solve();

         break;

         case GEN_TECPLOT:
            inputDataManager_->printDataSU2();
         break;

        case QUIT:
            isProgramEnded_ = true;
        break;

        default:
            cout << "Please enter a positive integer value." << endl;
        break;

    };


}


void EventManager::enterInputFile(std::string& iPath)
{
    //Manage the command of user in an input file.
    inputDataManager_->manageEntryFileName(iPath);
}

void EventManager::testSU2Function()
{   
    // General function to allow user to print some data.

    inputDataManager_->printDataSU2();
    

}







