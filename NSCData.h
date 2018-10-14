#ifndef DEF_NSCDATA_H
#define DEF_NSCDATA_H


//Standard lib.
#include <iostream>


// This class allows to store the data according to NSC structure presented in the
// NSCode. Is called by the solvers and initializer through multiple processes.
class NSCData
{

    public:
        //Default Constructor
        NSCData();

        //Default Destructor
        virtual ~NSCData();

        //Data.
        //TBD.
        std::string ctrlfilename_;


};


#endif




