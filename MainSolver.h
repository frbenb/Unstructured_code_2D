#ifndef DEF_MAINSOLVER_H
#define DEF_MAINSOLVER_H


//Standard lib.
#include <iostream>
#include <math.h>

//Projet files
#include "NSCData.h"
#include "Mesh_Data.h"
#include "DataUpdater.h"
#include "FluxComputer.h"


// This class allows to store the data according to NSC structure presented in the
// NSCode. Is called by the solvers and initializer through multiple processes.
//
// 
//
class MainSolver
{

    public:
        //Default Constructor
        MainSolver(NSCData *iNSCData, Mesh_Data *iMeshData, DataUpdater *iDataUpdater, FluxComputer *iFluxComputer);

        //Default Destructor
        virtual ~MainSolver();

        //This function allows to be the generic update function for the MainSolver
        // contains the iterate steps function and monitor convergence (runs until convergence is
        // achieved)
        void doUpdate();

        // This function allows to compute the solution by using Runge-Kutta and 
        // Flux computation. Consists of a loop specified by the number of stage required.
        void computeSolution();

        //This function allows to compute the timestep parameter (delta_t)
        void timestep();

        //This function allows to save the initial state of fluxes on the mesh computation.
        void saveW0();

        // This function allows to compute the spectral radius.
        void spectral_radius();

        //This method allows to compute the residual value in each cell.
        void residual();

        // This method allows to smooth the residual values.
        void residual_smoothing();

        // This method allows to monitor convergence
        void monitor_convergence();

    private:

        NSCData     *nscData_;
        Mesh_Data   *meshData_;

        DataUpdater *dataUpdater_; 
        FluxComputer *fluxComputer_;
        

};


#endif




