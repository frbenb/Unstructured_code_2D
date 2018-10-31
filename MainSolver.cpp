
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

        //1. Iterate_pseudo_timestep (computeSolution)

        //2. Monitor the convergence.

}

void MainSolver::computeSolution()
{

    //timestep

    // Save w0

    //In a loop, for nstage...

        //1. spectral_radius

        //2. residual

        //3. residual smoothing

        //4. Update solution

        //5. Update boundary

}

void MainSolver::timestep()
{
    spectral_radius();

    double cfl = nscData_->cfl_;
    double area, speci, specj;

    //Loop on cells
    for(int i(0); i < meshData_->NCells_;i++)
    {
        area = meshData_->cellArea_[i];

        // Apply formula of delta time
    }

}

void MainSolver::saveW0()
{
    double g = nscData_->gamma_;
    double ro,u,v,p;

    //Loop on cells
    for(int i(0);i < meshData_->NCellsTotal_; i++)
    {
        meshData_->rho_0_[i] = meshData_->rho_[i];
        meshData_->u_0_[i] = meshData_->rho_[i] * meshData_->u_[i];
        meshData_->v_0_[i] = meshData_->rho_[i] * meshData_->v_[i];

        meshData_->p_0_[i] = 0.5 * meshData_->rho_[i] * (meshData_->u_[i]*meshData_->u_[i] + meshData_->v_[i]*meshData_->v_[i]);
        meshData_->p_0_[i] += 1/(g - 1)*meshData_->p_[i];

    }

}

void MainSolver::spectral_radius()
{
    double delta_S_x = 0;
    double delta_S_y = 0;

    double g = nscData_->gamma_;
    double c; // Speed of sound.

    //Loop on all faces
    for(int i(0);i<meshData_->NFaces_;i++)
    {
        delta_S_x += 0.5*std::abs(meshData_->normal_x_[i]); 
        delta_S_y += 0.5*std::abs(meshData_->normal_y_[i]);
    }

    //Loop on cells
    for (int i(0);i < meshData_->NCells_;i++)
    {
        c = 

        spec_x[i] = meshData_->u_[i] + nscData_->;

    }
    

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



