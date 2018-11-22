
#include "MainSolver.h"

MainSolver::MainSolver(NSCData *iNSCData, Mesh_Data *iMeshData, DataUpdater *iDataUpdater, FluxComputer *iFluxComputer) :
                                    nscData_(iNSCData),
                                    meshData_(iMeshData),
                                    dataUpdater_(iDataUpdater),
                                    fluxComputer_(iFluxComputer)
{

}

MainSolver::~MainSolver()
{
    
}

void MainSolver::doUpdate()
{
    //Inside a loop, stop when convergence...
    for(unsigned int i(0);i < nscData_->niter_; i ++)
    {

        //1. Iterate_pseudo_timestep (computeSolution)
        computeSolution();

        //2. Monitor the convergence (empty).
        monitor_convergence();

    }
}

void MainSolver::computeSolution()
{

    //timestep
    timestep();

    // Save w0
    saveW0();

    //In a loop, for nstage...
    for(unsigned int i(0);i < nscData_->nstage_;i++)
    {
        //1. spectral_radius
        spectral_radius();

        //2. residual
        residual();

        //3. residual smoothing (for now empty)
        if(nscData_->ressmo_ > 0)
        {
            residual_smoothing();
        }
            
        //4. Update solution
        dataUpdater_->update_solution(nscData_->rk_alfa_[i]);

        //5. Update boundary
        dataUpdater_->update_boundary();
    }
        
}

void MainSolver::timestep()
{
    //Calculate spec_x_ and spec_y_
    spectral_radius();

    double cfl;
    double area, speci, specj;

    //Loop on cells
    for(unsigned int i(0); i < meshData_->NCells_;i++)
    {
        cfl = nscData_->cfl_;
        area = meshData_->cellArea_[i];
        speci = meshData_->spec_x_[i];
        specj = meshData_->spec_y_[i];

        // Apply formula of delta time
        if((speci + specj) != 0)
        {
            meshData_->deltaT_[i] = cfl*area/(speci+specj);
        }
        else
        {
            std::cout << "Error: (speci + specj) should be different than 0." << endl;
            return;
        }
            
    }

}

void MainSolver::saveW0()
{
    double g = nscData_->gamma_;

    //Loop on cells
    for(unsigned int i(0);i < meshData_->NCellsTotal_; i++)
    {
        meshData_->rho_0_[i] = meshData_->rho_[i];
        meshData_->u_0_[i] = meshData_->rho_[i] * meshData_->u_[i];
        meshData_->v_0_[i] = meshData_->rho_[i] * meshData_->v_[i];

        meshData_->p_0_[i] = 0.5 * meshData_->rho_[i] * (meshData_->u_[i]*meshData_->u_[i] + meshData_->v_[i]*meshData_->v_[i]);
        if(meshData_->p_[i] != 0)
        {
            meshData_->p_0_[i] += 1/(g - 1)*meshData_->p_[i];
        }
        else
        {
            std::cout << "Error: p_ should be different than 0" << endl;
            return;
        }

    }

}

void MainSolver::spectral_radius()
{
    double delta_S_x = 0;
    double delta_S_y = 0;

    double g = nscData_->gamma_;
    double c; // Speed of sound.

    //Loop on all faces
    for(unsigned int i(0);i<meshData_->NFaces_;i++)
    {
        delta_S_x += 0.5*std::abs(meshData_->normal_x_[i]); 
        delta_S_y += 0.5*std::abs(meshData_->normal_y_[i]);
    }

    //Loop on cells
    for (unsigned int i(0);i < meshData_->NCells_;i++)
    {   
        if (meshData_->rho_ != 0)
        {
            c = std::sqrt(g*meshData_->p_[i]/meshData_->rho_[i]);
        }
        else
        {
            std::cout << "Error: Rho should be different than 0." << endl;
            return;
        }

        meshData_->spec_x_[i] = (meshData_->u_[i] + c )*delta_S_x;
        meshData_->spec_y_[i] = (meshData_->v_[i] + c ) *delta_S_y;
    }
    
}

void MainSolver::residual()
{

    //Loop on cells domaine
    for(unsigned int i(0);i < meshData_->NCells_;i++)
    {
        // Set residuals to 0.
        meshData_->residualInviscid_rho_[i] = 0;
        meshData_->residualInviscid_u_[i] = 0;
        meshData_->residualInviscid_v_[i] = 0;
        meshData_->residualInviscid_p_[i] = 0;

        if(nscData_->nstage_ == 0)
        {
            meshData_->residualDissip_rho_[i] = 0;
            meshData_->residualDissip_u_[i] = 0;
            meshData_->residualDissip_v_[i] = 0;
            meshData_->residualDissip_p_[i] = 0;
        }
    }

     //Call eflux() here


    //Add artificial dissip. to inviscid. by looping on cells
    for(unsigned int i(0);i < meshData_->NCells_;i++)
    {
        meshData_->residualInviscid_rho_[i]+=meshData_->residualDissip_rho_[i];
        meshData_->residualInviscid_u_[i]+=meshData_->residualDissip_u_[i];
        meshData_->residualInviscid_v_[i]+=meshData_->residualDissip_v_[i];
        meshData_->residualInviscid_p_[i]+=meshData_->residualDissip_p_[i];

    }

}

void MainSolver::residual_smoothing()
{

}

void MainSolver::monitor_convergence()
{
    unsigned int iter;
    double res;
    double* R0;
    double* R;
    double RMSR0, RMSR; //Root mean square for R0 and for R

    iter = 0;

    double* R0 = new double[meshData_->NCells_];
    R0 = meshData_->residualInviscid_rho_;

    double* R = new double[meshData_->NCells_];
    R = meshData_->residualInviscid_rho_;

    for (unsigned int i=0; i<meshData_->NCells_; i++)
    {
        RMSR0 += R0[i];
        RMSR += R[i];
    }
    RMSR0 = std::sqrt(RMSR0/meshData_->NCells_);
    RMSR = std::sqrt(RMSR/meshData_->NCells_);

    res = log(RMSR) - log(RMSR0);

    //Loop until convergence or until we get the number of itereations max 
    while(iter < nscData_->niter_ && res > 10^-6)
    {
        R0 = meshData_->residualInviscid_rho_;
        for (unsigned int i=0; i<meshData_->NCells_; i++)
        {
            RMSR0 += R0[i];
        }
        RMSR0 = std::sqrt(RMSR0/meshData_->NCells_);

        residual();

        R = meshData_->residualInviscid_rho_;
        for (unsigned int i=0; i<meshData_->NCells_; i++)
        {
            RMSR += RMSR[i];
        }
        RMSR = std::sqrt(RMSR/meshData_->NCells_);

        res = log(RMSR) - log(RMSR0);
        iter ++;
    }

    if (R0 != nullptr)
    {
        delete R0;
    }

    if (R != nullptr)
    {
        delete R;
    }
}

