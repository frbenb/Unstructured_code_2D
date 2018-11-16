#include <FluxComputer.h>

FluxComputer::FluxComputer(NSCData *iNSCData, Mesh_Data *iMeshData):nscData_(iNSCData),meshData_(iMeshData)
{}

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
    for (unsigned int i=0; i<meshData_->NFaces_; i++)
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
    

        //Calculation of the inviscid residuals:
        meshData_->residualDissip_rho_[cellLeft] -= meshData_->convectiveFlux0Faces_[i];
        meshData_->residualDissip_rho_[cellRight] += meshData_->convectiveFlux0Faces_[i];

        meshData_->residualDissip_u_[cellLeft] -= meshData_->convectiveFlux1Faces_[i];
        meshData_->residualDissip_u_[cellRight] += meshData_->convectiveFlux1Faces_[i];

        meshData_->residualDissip_v_[cellLeft] -= meshData_->convectiveFlux2Faces_[i];
        meshData_->residualDissip_v_[cellRight] += meshData_->convectiveFlux2Faces_[i];

        meshData_->residualDissip_p_[cellLeft] -= meshData_->convectiveFlux3Faces_[i];
        meshData_->residualDissip_p_[cellRight] += meshData_->convectiveFlux3Faces_[i];

        meshData_->residualDissip_rho_[cellLeft] = meshData_->residualDissip_rho_[cellLeft]/meshData_->cellArea_[i];
        meshData_->residualDissip_rho_[cellRight] = meshData_->residualDissip_rho_[cellLeft]/meshData_->cellArea_[i];

        meshData_->residualDissip_rho_[cellLeft] = meshData_->residualDissip_rho_[cellLeft]/meshData_->cellArea_[i];
        meshData_->residualDissip_rho_[cellRight] = meshData_->residualDissip_rho_[cellLeft]/meshData_->cellArea_[i];
        
        meshData_->residualDissip_rho_[cellLeft] = meshData_->residualDissip_rho_[cellLeft]/meshData_->cellArea_[i];
        meshData_->residualDissip_rho_[cellRight] = meshData_->residualDissip_rho_[cellLeft]/meshData_->cellArea_[i];

        meshData_->residualDissip_rho_[cellLeft] = meshData_->residualDissip_rho_[cellLeft]/meshData_->cellArea_[i];
        meshData_->residualDissip_rho_[cellRight] = meshData_->residualDissip_rho_[cellLeft]/meshData_->cellArea_[i];
    }
    cout << "Inviscid residuals calculated" << endl;
}

//deflux for a roe scheme and of first order:
void FluxComputer::calculateArtificialDissipRoe()
{
    //TBD
}