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
    unsigned int cellLeft, cellRight;
    double rhoLeft, rhoRight, uLeft, uRight, vLeft, vRight, pLeft, pRight;
    double* rhoFace, uFace, vFace, pFace;

    //Initialization of fluxes (0 at each cell)
    for (unsigned int i=0; i<meshData_->NCellsTotal_; i++)
    {
        meshData_-> tmp_rho_[i] = 0;
        meshData_-> tmp_u_[i] = 0;
        meshData_-> tmp_v_[i] = 0;
        meshData_-> tmp_p_[i] = 0;
    }

    for (unsigned int i=0; i=meshData_->NFaces_; i++)
    {
        //Flux of each face from left cell and right cell
        cellLeft = meshData_->Face2Cell_[i][1];
        cellRight = meshData_->Face2Cell_[i][2];

        rhoLeft = meshData_->rho_[cellLeft];
        rhoRight = meshData_->rho_[cellRight];

        uLeft = meshData_->u_[cellLeft];
        uRight = meshData_->v_[cellRight];

        pLeft = meshData_->p_[cellLeft];
        pRight = meshData_->p_[cellRight];


        //Flux total for each face (fill vectors rhoFace, uFace, vFace and pFace)
        rhoFace[i] = 0.5 * (rhoLeft + rhoRight);
        uFace[i] = 0.5 * (uLeft + uRight);
        vFace[i] = 0.5 * (vLeft + vRight);
        pFace[i] = 0.5 * (pLeft + pRight);

        for (unsigned int j=0; j<meshData_-> )

        //Flux total for each face
        for(k=0; k<meshData_->NCellsTotal_; k++)
        {
            for (l=0; l<meshData_->CellNfaces_[k])
            {
                meshData_->rho_[k] += rhoFace[k]; 
                meshData_->u_[k] += uFace[k];
                meshData_->v_[k] += vFace[k];
                meshData_->p_[k] += pFace[k];
            }
            
            if (meshData_->CellNfaces_[k]==4)
            {
                meshData_->rho_[k] = meshData_->rho_[k]/4;
                meshData_->u_[k] = meshData_->u_[k]/4;
                meshData_->v_[k] = meshData_->v_[k]/4;
                meshData_->p_[k] = meshData_->p_[k]/4;
            }
            else if (meshData_->CellNfaces_[k]==3)
            {
                meshData_->rho[k] = meshData_->rho_[k]/3;
                meshData_->u_[k] = meshData_->u_[k]/3;
                meshData_->v_[k] = meshData_->v_[k]/3;
                meshData_->p_[k] = meshData_->p_[k]/3;
            }
            else if (meshData_->CellNfaces_[k]==2)
            {
                meshData_->rho[k] = meshData_->rho_[k]/2;
                meshData_->u_[k] = meshData_->u_[k]/2;
                meshData_->v_[k] = meshData_->v_[k]/2;
                meshData_->p_[k] = meshData_->p_[k]/2;
            }
            else
            {
                cout << "" << endl;
            }
        }
    }    
}

//deflux (first order):
void FluxComputer::calculateArtificialDissip()
{
    
}

void FluxComputer::roeScheme()
{

}