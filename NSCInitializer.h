#ifndef DEF_NSCINITIALIZER_H
#define DEF_NSCINITIALIZER_H


//Standard lib.
#include <iostream>
#include <math.h>

//Projet files
#include "NSCData.h"
#include "Mesh_Data.h"




// This class allows to
// Takes the roles of the following functions:
// initial_flow_parameters()
// initial_field()
//
class NSCInitializer
{

    public:
        //Default Constructor
        NSCInitializer(NSCData *iNSCData, Mesh_Data *iMeshData);

        //Default Destructor
        virtual ~NSCInitializer();

        //This method allows to initial the flow (primitive variable with infinity)
        void initial_flow_parameters();

        // This method allows to initialize all cells with initial infinity values.
        void initial_field();

        // This method allows to initialize some general parameters of the system.
        void initial_system();

        // This method allows to set the right alpha and beta constant for the solver.
        void initial_rk_scheme();

    private:

        NSCData     *nscData_;
        Mesh_Data   *meshData_;

};


#endif
