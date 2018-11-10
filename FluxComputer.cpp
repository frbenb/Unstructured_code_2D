#include <FluxComputer.h>

FluxComputer::FluxComputer()
{
    //TBD
}

FluxComputer::~FluxComputer()
{
    //TDB
}

//eflux:
void FluxComputer::calculateConvectiveFluxes()
{
    unsigned int cellLeft, cellRight;
    double rhoLeft, rhoRight, uLeft, uRight, vLeft, vRight, pLeft, pRight;
    double VcontravariantLeft, VcontravariantRight, enthalpyLeft, enthalpyRight;
    double leftFlux0, leftFlux1, leftFlux2, leftFlux3;
    double rightFlux0, rightFlux1, rightFlux2, rightFlux3;
    unsigned int face;
    
    //Calculation of the convective fluxes:
    for (unsigned int i=0; i=meshData_->NFaces_; i++)
    {
        //Convective flux for each face of left cell and right cell
        cellLeft = meshData_->Face2Cell_[i][0];
        cellRight = meshData_->Face2Cell_[i][1];

        //Primitive variables for left and right cells
        rhoLeft = meshData_->rho_[cellLeft];
        rhoRight = meshData_->rho_[cellRight];

        uLeft = meshData_->u_[cellLeft];
        uRight = meshData_->u_[cellRight];

        vLeft = meshData_->v_[cellLeft];
        vRight = meshData_->v_[cellRight];

        pLeft = meshData_->p_[cellLeft];
        pRight = meshData_->p_[cellRight];

        //Contravariant speed (V) and enthalpy (H) for left and right cells
        VcontravariantLeft = uLeft * meshData_->normal_x_[i] + vLeft * meshData_->normal_y_[i];
        VcontravariantRight = uRight * meshData_->normal_x_[i] + vRight * meshData_->normal_y_[i];

        enthalpyLeft = 0.5 * (uLeft * uLeft + vLeft * vLeft) + nscData_->gamma_/(nscData_->gamma_ - 1) * pLeft/rhoLeft;
        enthalpyRight = 0.5 * (uRight * uRight + vRight * vRight) + nscData_->gamma_/(nscData_->gamma_ - 1) * pRight/rhoRight;

        //Convective fluxes (Fc) at the left cell
        leftFlux0 = rhoLeft * VcontravariantLeft;
        leftFlux1 = rhoLeft * VcontravariantLeft * uLeft + pLeft * meshData_->normal_x_[i];
        leftFlux2 = rhoLeft * VcontravariantLeft * vLeft + pLeft * meshData_->normal_y_[i];
        leftFlux3 = rhoLeft * VcontravariantLeft * enthalpyLeft;

        //Convective fluxes (Fc) at the right cell
        rightFlux0 = rhoRight * VcontravariantRight;
        rightFlux1 = rhoRight * VcontravariantRight * uRight + pRight * meshData_->normal_x_[i];
        rightFlux2 = rhoRight * VcontravariantRight * vRight + pRight * meshData_->normal_y_[i];
        rightFlux3 = rhoRight * VcontravariantRight * enthalpyRight;

        //Convective flux (Fc) totatl for each face
        meshData_->convectiveFlux0Faces_[i] = 0.5 * (leftFlux0 + rightFlux0);
        meshData_->convectiveFlux1Faces_[i] = 0.5 * (leftFlux1 + rightFlux1);
        meshData_->convectiveFlux2Faces_[i] = 0.5 * (leftFlux2 + rightFlux2);
        meshData_->convectiveFlux3Faces_[i] = 0.5 * (leftFlux3 + rightFlux3);
    }

    //Calculation of the inviscid residuals at each cell:
    // for (unsigned int i=0; i<meshData_->NCells_; i++) //for each cell...
    // {
    //     for (unsigned int j=0; j<meshData_->CellNfaces_[i]; j++) //...we parcour each face...
    //     {
    //         face = meshData_->Cell2Face_[i][j];

    //         //Cells associated with the face
    //         cellLeft = meshData_->Face2Cell_[face][0];
    //         cellRight = meshData_->Face2Cell_[face][1];

    //         //For each CELL:
    //         meshData_->residualDissip_rho_[i] -= 
    //         meshData_->residualDissip_rho_[i] +=

    //         meshData_->residualDissip_u_[i] -=
    //         meshData_->residualDissip_u_[i] +=

    //         meshData_->residualDissip_v_[i] -=
    //         meshData_->residualDissip_v_[i] +=

    //         meshData_->residualDissip_p_[i] -=
    //         meshData_->residualDissip_p_[i] += 
    //     }
    // }
}

//deflux for a roe scheme and of first order:
void FluxComputer::calculateArtificialDissipRoe()
{

    unsigned int cellLeft, cellRight;
    double rhoLeft, rhoRight, uLeft, uRight, vLeft, vRight, pLeft, pRight;
    double rhoRoe, uRoe, vRoe, pRoe, enthalpyLeft, enthalpyRight, enthalpyRoe, VcontravariantRoe;
    double VcontravariantLeft, VcontravariantRight;
    double rightFlux0, rightFlux1, rightFlux2, rightFlux3;
    unsigned int face;

   for (unsigned int i=0; i=meshData_->NFaces_; i++)
    {
        //1. Convective flux for each face of left cell and right cell
        cellLeft = meshData_->Face2Cell_[i][0];
        cellRight = meshData_->Face2Cell_[i][1];  

        //1.1 Primitive variables for left and right cells
        rhoLeft = meshData_->rho_[cellLeft];
        rhoRight = meshData_->rho_[cellRight];

        uLeft = meshData_->u_[cellLeft];
        uRight = meshData_->u_[cellRight];

        vLeft = meshData_->v_[cellLeft];
        vRight = meshData_->v_[cellRight];

        pLeft = meshData_->p_[cellLeft];
        pRight = meshData_->p_[cellRight];

        VcontravariantLeft = uLeft * meshData_->normal_x_[i] + vLeft * meshData_->normal_y_[i];
        VcontravariantRight = uRight * meshData_->normal_x_[i] + vRight * meshData_->normal_y_[i];

        enthalpyLeft = 0.5 * (uLeft * uLeft + vLeft * vLeft) + nscData_->gamma_/(nscData_->gamma_ - 1) * pLeft/rhoLeft;
        enthalpyRight = 0.5 * (uRight * uRight + vRight * vRight) + nscData_->gamma_/(nscData_->gamma_ - 1) * pRight/rhoRight;

        //2. Calculation of Roe's variables (ones with tilde)
        rhoRoe = sqrt(rhoLeft + rhoRight);
        uRoe = (uLeft*sqrt(rhoLeft) + uRight*sqrt(rhoRight))/(sqrt(rhoLeft) + sqrt(rhoRight));
        vRoe = (vLeft*sqrt(rhoLeft) + vRight*sqrt(rhoRight))/(sqrt(rhoLeft) + sqrt(rhoRight));
        enthalpyRoe = (enthalpyLeft*sqrt(rhoLeft) + enthalpyRight*sqrt(rhoRight))/(sqrt(rhoLeft) + sqrt(rhoRight));
        VcontravariantRoe = uRoe * meshData_->normal_x_[i] + vRoe * meshData_->normal_y_[i];

        //Dissipation calculations
        



    
}