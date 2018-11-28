
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
    double RMSR0, RMSR; //Root mean square of R or R0 elements (from inviscid residual of rho)
    double result; //Result for the soustraction of the two logarithms

    string filename = "convergence.txt";
    ofstream outputConvergence(filename, ios::out | ios::trunc);

    if(outputConvergence.is_open())
    {
        cout << "File " << filename << " has been created." << endl;

    }
    else
    {
        cout << "File " << filename << " has not been created." << endl;
    }

    outputConvergence << " Iteration log(error) RMSR" << endl;

    //This loop has to be outside of the loop on the number of iterations, because RMSR0 stays 
    //the same throughout the iterations
    for (unsigned int i=0; i<meshData_->NCells_; i++)
    {
        RMSR0 += meshData_->residualInviscid_rho_[i] * meshData_->residualInviscid_rho_[i];
    }
    RMSR0 = std::sqrt(RMSR0/nscData_->niter_);
    
    cout << "RMSR0: " << RMSR0 << endl;

    //Inside a loop, stop when and if convergence...
    for(unsigned int i(0);i < nscData_->niter_; i ++)
    {
        if (i==0)
        {
            RMSR = RMSR0; //If it is the first iteration, R=R0 and result=0
            result = 0;
        }
        else
        {
            RMSR = monitor_convergence();
            result = log10(RMSR - RMSR0); //Error calculation
        }

        outputConvergence << i << " " << result << " " << RMSR << endl;

        cout << "RMSR: " << RMSR << endl;
        cout << "Result: " << result << endl;

        //If we have convergence, we stop the iterations
        if (result < -16)
        {
            break;
        }

        //Iterate_pseudo_timestep (computeSolution)
        computeSolution();
    }

    outputConvergence.close();
    cout << "File " << filename << " is close." << endl;
  
    //cout << endl << "Iteration:" << i << endl;
    //for(unsigned int j(0); j < meshData_->NCellsTotal_; j++)
    //{
    //   cout << meshData_->rho_[j]  << endl;
            
    //}
    //cout<< endl;

}

void MainSolver::computeSolution()
{


    // Save w0
    saveW0();

    //In a loop, for nstage...
    //for(unsigned int i(0);i < nscData_->nstage_;i++)
    //{
        //1. spectral_radius
        //timestep
         timestep();

        //2. residual
        //residual(nscData_->rk_beta_[i]);
        residual(1.0);

        //3. residual smoothing (for now empty)
        //if(nscData_->ressmo_ > 0)
        //{
         //   residual_smoothing();
       // }
            
        //4. Update solution
        //dataUpdater_->update_solution(nscData_->rk_alfa_[i]);
        dataUpdater_->update_solution(1.0);

        //5. Update boundary
        dataUpdater_->update_boundary();
    //}
        
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
        if((speci + specj) != 0.)
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
        meshData_->p_0_[i] = 0.5 * meshData_->rho_[i] * (meshData_->u_[i]*meshData_->u_[i] + meshData_->v_[i]*meshData_->v_[i]) + 1/(g - 1)*meshData_->p_[i];

    }

}

void MainSolver::spectral_radius()
{
    double delta_S_x;
    double delta_S_y;

    double g = nscData_->gamma_;
    double c; // Speed of sound.
    unsigned int faceindex;

    //Loop on all faces
    

    //Loop on cells
    for (unsigned int i(0);i < meshData_->NCells_;i++)
    {   
        delta_S_x = 0.;
        delta_S_y = 0.;
        for(unsigned int j(0);j<meshData_->CellNfaces_[i];j++){
            faceindex = meshData_->Cell2Face_[i][j];
            delta_S_x += 0.5*std::fabs(meshData_->normal_x_[faceindex]); 
            delta_S_y += 0.5*std::fabs(meshData_->normal_y_[faceindex]);
        }

        c = std::sqrt(g*meshData_->p_[i]/meshData_->rho_[i]);

        meshData_->spec_x_[i] = (fabs(meshData_->u_[i]) + c)*delta_S_x;
        meshData_->spec_y_[i] = (fabs(meshData_->v_[i]) + c) *delta_S_y;
    }
}

void MainSolver::residual(double iRk_beta)
{

    //Loop on cells domaine
    for(unsigned int i(0);i < meshData_->NCells_;i++)
    {
        // Set residuals to 0.
        meshData_->residualInviscid_rho_[i] = 0;
        meshData_->residualInviscid_u_[i] = 0;
        meshData_->residualInviscid_v_[i] = 0;
        meshData_->residualInviscid_p_[i] = 0;

        /// Current stage not total stage
        //if(nscData_->nstage_ == 0)
        //{
            meshData_->residualDissip_rho_[i] = 0;
            meshData_->residualDissip_u_[i] = 0;
            meshData_->residualDissip_v_[i] = 0;
            meshData_->residualDissip_p_[i] = 0;
       // }
    }

     fluxComputer_->calculateConvectiveFluxes();

    if(iRk_beta  >  nscData_->epsilon_) // Call dissipation dflux if dissip is indicated
    {
        fluxComputer_->calculateArtificialDissipRoe(); //dflux
    }


    //Add artificial dissip. to inviscid. by looping on cells
    for (unsigned int i(0);i < meshData_->NCells_;i++)
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


//This function calculates the root mean square of the inviscid residual of rho. In doUpdate(), 
//you can find the other part of the monitor_convergence()
double MainSolver::monitor_convergence()
{ 
    double RMSR; //Root mean square of R elements (from inviscid residual of rho)

    for (unsigned int i=0; i<meshData_->NCells_; i++)
    {
        RMSR += meshData_->residualInviscid_rho_[i] * meshData_->residualInviscid_rho_[i];
    }

    RMSR = std::sqrt(RMSR/nscData_->niter_);

    return RMSR;
}

