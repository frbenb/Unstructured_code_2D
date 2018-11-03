#include <FluxComputer.h>

FluxComputer::FluxComputer()
{

}

FluxComputer::~FluxComputer()
{

}

//eflux:
void FluxComputer::calculateConvectiveFluxes()
{
    //Initialisation of fluxes (0 at each cell)
    for (unsigned int i=0; i<meshData_->NCellsTotal_; i++)
    {
        meshData_-> tmp_rho_[i] = 0;
        meshData_-> tmp_u_[i] = 0;
        meshData_-> tmp_v_[i] = 0;
        meshData_-> tmp_p_[i] = 0;
    }

    //Flux of faces from left cell and right cell
    for (unsigned int i=0; i=meshData_->NCellsTotal_; i++)
    {
        for(unsigned int j=0; j=2; j++)
        {

        }
    }

    //Flux total for each cell


}

//deflux (first order):
void FluxComputer::calculateArtificialDissip()
{
    
}

void FluxComputer::roeScheme()
{

}