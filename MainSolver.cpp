
#include "MainSolver.h"


MainSolver::MainSolver(NSCData *iNSCData, Mesh_Data *iMeshData, DataUpdater *iDataUpdater) :
                                    nscData_(iNSCData),
                                    meshData_(iMeshData),
                                    dataUpdater_(iDataUpdater)
{

}

MainSolver::~MainSolver()
{
    
}

void MainSolver::doUpdate()
{
    //Inside a loop, stop when convergence...

        //1. Iterate_pseudo_timestep

        //2. Monitor the convergence.

}

void MainSolver::computeSolution()
{

    //In a loop, for nstage...

        //1. spectral_radius

        //2. residual

        //3. residual smoothing

        //4. Update solution

        //5. Update boundary

}

void MainSolver::timestep()
{

}

void MainSolver::saveW0()
{


}

void MainSolver::spectral_radius()
{


}

void MainSolver::residual()
{

}

void MainSolver::residual_smoothing()
{

}

void MainSolver::monitor_convergence()
{


}



