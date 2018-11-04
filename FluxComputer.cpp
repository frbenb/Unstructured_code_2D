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

    // //Initialization of fluxes (0 at each cell)
    // for (unsigned int i=0; i<meshData_->NCellsTotal_; i++)
    // {
    //     meshData_-> tmp_rho_[i] = 0;
    //     meshData_-> tmp_u_[i] = 0;
    //     meshData_-> tmp_v_[i] = 0;
    //     meshData_-> tmp_p_[i] = 0;
    // }

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
        meshData_->rhoFace_[i] = 0.5 * (rhoLeft + rhoRight);
        meshData_->uFace_[i] = 0.5 * (uLeft + uRight);
        meshData_->vFace_[i] = 0.5 * (vLeft + vRight);
        meshData_->pFace_[i] = 0.5 * (pLeft + pRight);


        //Flux total for each face
        for(unsigned int j=0; j<meshData_->NCellsTotal_; j++)
        {
            for (unsigned int k=0; k<meshData_->CellNfaces_[j]; k++)
            {
                meshData_->rho_[j] += meshData_->rhoFace_[j]; 
                meshData_->u_[j] += meshData_->uFace_[j];
                meshData_->v_[j] += meshData_->vFace_[j];
                meshData_->p_[j] += meshData_->pFace_[j];
            }
            
            if (meshData_->CellNfaces_[j]==4)
            {
                meshData_->rho_[j] = meshData_->rho_[j]/4;
                meshData_->u_[j] = meshData_->u_[j]/4;
                meshData_->v_[j] = meshData_->v_[j]/4;
                meshData_->p_[j] = meshData_->p_[j]/4;
            }
            else if (meshData_->CellNfaces_[j]==3)
            {
                meshData_->rho_[j] = meshData_->rho_[j]/3;
                meshData_->u_[j] = meshData_->u_[j]/3;
                meshData_->v_[j] = meshData_->v_[j]/3;
                meshData_->p_[j] = meshData_->p_[j]/3;
            }
            else if (meshData_->CellNfaces_[j]==2)
            {
                meshData_->rho_[j] = meshData_->rho_[j]/2;
                meshData_->u_[j] = meshData_->u_[j]/2;
                meshData_->v_[j] = meshData_->v_[j]/2;
                meshData_->p_[j] = meshData_->p_[j]/2;
            }
            else
            {
                cout << "Cells have to be a quads, triangles or a lines." << endl;
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