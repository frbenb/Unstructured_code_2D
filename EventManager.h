#ifndef DEF_EVENTMANAGER_H
#define DEF_EVENTMANAGER_H

//Project files
#include "InputDataManager.h"
#include "OutputDataManager.h"

//Standard lib.
#include <iostream>
#include <string>


// This class allows to control the input of event from user.
// For now, only calls a process to solve the data. Later, this
// class will be used as the console or GUI.
class EventManager
{

    public:

        //Default Constructor
        EventManager(InputDataManager *iInputDataManager);

        //Default Destructor
        virtual ~EventManager();


        //This method allows to send the input path from user entry.
        // it contains all the routine to run when is called
        void enterInputFile(std::string iPath);

        //This method allows the user to trigger the event of printing data.
        void testSU2Function();

        
    private:

        InputDataManager    *inputDataManager_;
        

};


#endif
