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
        cout << "Bienvenue dans NSCode++!" << endl;
        cout << "Veuillez choisir parmis les options suivantes:" << endl << endl;
        cout << "1. Initialiser la solution." << endl;
        cout << "2. Initialiser et resoudre la solution." << endl;
        cout << "3. Generer un fichier ASCII de Tecplot." << endl;
        cout << "4. Afficher les valeurs d'entrees." << endl;
        cout << "5. Quitter" << endl;
        cin >> userChoice_;

        logicChoice(userChoice_);

    }
    while(!isProgramEnded_);

    cout << "Au revoir!" << endl;

}

void EventManager::logicChoice(int iUserChoice)
{
    //1. Enumration in order of display in startExectution method
    enum TypesChoices {INIT, INIT_SOLVE, GEN_TECPLOT, DISPLAY_VAR, QUIT};


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

         case DISPLAY_VAR:

         break;

        case QUIT:
            isProgramEnded_ = true;
        break;

        default:
            cout << "Veuillez entrer un nombre entier positif valide." << endl;
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







