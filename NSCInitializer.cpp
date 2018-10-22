#include "NSCInitializer.h"


NSCInitializer::NSCInitializer(NSCData *iNSCData, Mesh_Data *iMeshData) :
                                    nscData_(iNSCData),
                                    meshData_(iMeshData)
{

}

NSCInitializer::~NSCInitializer()
{
    

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
    for (unsigned i=0;i<=meshData_->NCells_;i++)
    {
        meshData_->rho_[i] = nscData_->rhoInfini_;
        meshData_->u_[i] = nscData_->uInfini_;
        meshData_->v_[i] = nscData_->vInfini_;
        meshData_->p_[i] = nscData_->pInfini_;

    }


}

void NSCInitializer::initial_rk_scheme()
{
    //TBD.
}
