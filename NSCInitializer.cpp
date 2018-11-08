#include "NSCInitializer.h"


NSCInitializer::NSCInitializer(NSCData *iNSCData, Mesh_Data *iMeshData) :
                                    nscData_(iNSCData),
                                    meshData_(iMeshData)
{

}

NSCInitializer::~NSCInitializer()
{
    //Memory deallocating
    deallocate1Ddbl(nscData_->rk_alfa_);
    deallocate1Ddbl(nscData_->rk_beta_);

}

void NSCInitializer::initial_system()
{
    nscData_->pi_=4.*atan(1.);
    nscData_->gamma_=1.4;
    nscData_->epsilon_=1.0e-28;
}

void NSCInitializer::initial_flow_parameters()
{
    double alpha,mach,g;

    g = nscData_->gamma_;

    alpha = nscData_->alpha_ * nscData_->pi_/180;
    mach = nscData_->mach_;
  
    nscData_->rhoInfini_=1.0;
    nscData_->uInfini_=mach*sqrt(g)*cos(alpha);
    nscData_->vInfini_=mach*sqrt(g)*sin(alpha);
    nscData_->pInfini_=1.0;

}

void NSCInitializer::initial_field()
{

    //Loop on every cells
    for (unsigned int i=0;i<=meshData_->NCells_;i++)
    {
        meshData_->rho_[i] = nscData_->rhoInfini_;
        meshData_->u_[i] = nscData_->uInfini_;
        meshData_->v_[i] = nscData_->vInfini_;
        meshData_->p_[i] = nscData_->pInfini_;

    }


}

void NSCInitializer::initial_rk_scheme()
{
    nscData_->nstage_ = nscData_->rk_;

    //Allocating memory
    nscData_->rk_alfa_ = allocate1Ddbl(nscData_->nstage_);
    nscData_->rk_beta_ = allocate1Ddbl(nscData_->nstage_);
    
    switch(nscData_->nstage_)
    {
        case 1:
            nscData_->rk_alfa_[0]=1.0; 
            nscData_->rk_beta_[0]=1.0; 
            break;
        case 2:
            nscData_->rk_alfa_[0]=0.6; 
            nscData_->rk_alfa_[1]=1.0; 
            nscData_->rk_beta_[0]=1.0; 
            nscData_->rk_beta_[1]=0.0; 
            break;
        case 3:
            nscData_->rk_alfa_[0]=0.6; 
            nscData_->rk_alfa_[1]=0.6; 
            nscData_->rk_alfa_[2]=1.0; 
            nscData_->rk_beta_[0]=1.0; 
            nscData_->rk_beta_[1]=0.0; 
            nscData_->rk_beta_[2]=0.0; 
            break;
        case 4:
            nscData_->rk_alfa_[0]=0.25; 
            nscData_->rk_alfa_[1]=0.333333; 
            nscData_->rk_alfa_[2]=0.5; 
            nscData_->rk_alfa_[3]=1.0; 
            nscData_->rk_beta_[0]=1.0; 
            nscData_->rk_beta_[1]=0.5; 
            nscData_->rk_beta_[2]=0.0; 
            nscData_->rk_beta_[3]=0.0; 
            break;
        case 5:
            nscData_->rk_alfa_[0]=0.25; 
            nscData_->rk_alfa_[1]=0.1666667; 
            nscData_->rk_alfa_[2]=0.375; 
            nscData_->rk_alfa_[3]=0.5; 
            nscData_->rk_alfa_[4]=1.0; 
            nscData_->rk_beta_[0]=1.0; 
            nscData_->rk_beta_[1]=0.0; 
            nscData_->rk_beta_[2]=0.56; 
            nscData_->rk_beta_[3]=0.0; 
            nscData_->rk_beta_[4]=0.44;
            break;
  
        default:
            std::cout << "Error: cannot initialize Runge-Kutta" << endl;
  }
 
}
