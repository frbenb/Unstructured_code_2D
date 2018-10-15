#include "NSCInitializer.h"


NSCInitializer::NSCInitializer(NSCData *iNSCData, Mesh_Data *iMeshData) :
                                    nscData_(iNSCData),
                                    meshData_(iMeshData)
{

}

NSCInitializer::~NSCInitializer()
{
    

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
    unsigned int i,j;
    double *ro,*uu,*vv,*pp;
  
    ro=meshData_->rho_;
    uu=meshData_->u_;
    vv=meshData_->v_;
    pp=meshData_->p_;

    //Loop on every cells
    for (i=0;i<=meshData_->NCells_;i++)
    {
        ro[i]=nscData_->rhoInfini_;
        uu[i]=nscData_->uInfini_;
        vv[i]=nscData_->vInfini_;
        pp[i]=nscData_->pInfini_;

    }


}