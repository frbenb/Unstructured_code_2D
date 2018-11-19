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
    }
    //cout << "Inviscid residuals calculated" << endl;
}

//deflux for a roe scheme and of first order:
void FluxComputer::calculateArtificialDissipRoe()
{

    unsigned int cellLeft, cellRight;
    double rhoLeft, rhoRight, uLeft, uRight, vLeft, vRight, pLeft, pRight;
    double rhoRoe, uRoe, vRoe, pRoe, qSquaredRoe, cRoe, enthalpyLeft, enthalpyRight, enthalpyRoe, VcontravariantRoe;
    double VcontravariantLeft, VcontravariantRight, normalised_x, normalised_y, vector_normal_x, vector_normal_y;
    double deltaF1Flux0, deltaF1Flux1,  deltaF1Flux2, deltaF1Flux3, radicalF1, radicalF23, radicalF5; 
    double deltaF5Flux0, deltaF5Flux1,  deltaF5Flux2, deltaF5Flux3;
    double deltaF23Flux0, deltaF23Flux1,  deltaF23Flux2, deltaF23Flux3;
    double dissipRoeFlux0, dissipRoeFlux1, dissipRoeFlux2, dissipRoeFlux3;

    double vector_length;

   for (unsigned int i = 0; i < meshData_->NFaces_; i++)
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

        //1.2 Normalised normal : nécessaire pour le calcul des variables de Roe

        vector_normal_x = meshData_->normal_x_[i];
        vector_normal_y = meshData_->normal_y_[i];

        vector_length = std::sqrt(vector_normal_x*vector_normal_x + vector_normal_y*vector_normal_y);

        normalised_x = vector_normal_x/vector_length;
        normalised_y = vector_normal_y/vector_length;

        VcontravariantLeft = uLeft * normalised_x + vLeft * normalised_y;
        VcontravariantRight = uRight * normalised_x + vRight * normalised_y;

        enthalpyLeft = 0.5 * (uLeft * uLeft + vLeft * vLeft) + nscData_->gamma_/(nscData_->gamma_ - 1) * pLeft/rhoLeft;
        enthalpyRight = 0.5 * (uRight * uRight + vRight * vRight) + nscData_->gamma_/(nscData_->gamma_ - 1) * pRight/rhoRight; 

        //2. Calculation of Roe's variables (ones with tilde)
        rhoRoe = sqrt(rhoLeft + rhoRight);
        uRoe = (uLeft*sqrt(rhoLeft) + uRight*sqrt(rhoRight))/(sqrt(rhoLeft) + sqrt(rhoRight));
        vRoe = (vLeft*sqrt(rhoLeft) + vRight*sqrt(rhoRight))/(sqrt(rhoLeft) + sqrt(rhoRight));
        enthalpyRoe = (enthalpyLeft*sqrt(rhoLeft) + enthalpyRight*sqrt(rhoRight))/(sqrt(rhoLeft) + sqrt(rhoRight));
        VcontravariantRoe = uRoe * normalised_x + vRoe * normalised_y;
        qSquaredRoe = uRoe*uRoe + vRoe*vRoe;
        cRoe = sqrt((nscData_->gamma_ - 1)*(enthalpyRoe - qSquaredRoe/2));

        //Dissipation calculations first term deltaF1 (4.89 p107)
        radicalF1 = abs(VcontravariantRoe - cRoe)*((pRight - pLeft - rhoRoe*cRoe*(VcontravariantRight - VcontravariantLeft))/(2*cRoe*cRoe));
        deltaF1Flux0 = radicalF1*1;
        deltaF1Flux1 = radicalF1*(uRoe - cRoe*normalised_x);
        deltaF1Flux2 = radicalF1*(vRoe - cRoe*normalised_y);
        deltaF1Flux3 = radicalF1*(enthalpyRoe - cRoe*VcontravariantRoe); 
        //cout << radicalF1 << endl;

        //Dissipation calculations second term deltaF23 (4.90 p107)
        radicalF23 = rhoRight - rhoLeft - (pRight - pLeft)/(cRoe*cRoe);
        deltaF23Flux0 = abs(VcontravariantRoe)*(radicalF23 * 1 + rhoRoe * 0);
        deltaF23Flux1 = abs(VcontravariantRoe)*(radicalF23 * uRoe + rhoRoe * ((uRight - uLeft) - (VcontravariantRight - VcontravariantLeft)*normalised_x));
        deltaF23Flux2 = abs(VcontravariantRoe)*(radicalF23 * vRoe + rhoRoe * ((vRight - vLeft) - (VcontravariantRight - VcontravariantLeft)*normalised_y));
        deltaF23Flux3 = abs(VcontravariantRoe)*(radicalF23 * qSquaredRoe/2 + rhoRoe*((uRoe*(uRight - uLeft)) + vRoe*(vRight - vLeft) 
                         - VcontravariantRoe*(VcontravariantRight - VcontravariantLeft))); 


        //Dissipation calculations THIRD term deltaF5 (4.90 p107)
        radicalF5 = abs(VcontravariantRoe - cRoe)*((pRight - pLeft + rhoRoe*cRoe*(VcontravariantRight - VcontravariantLeft))/(2*cRoe*cRoe));
        deltaF5Flux0 = radicalF5*1;
        deltaF5Flux1 = radicalF5*(uRoe + cRoe*normalised_x);
        deltaF5Flux2 = radicalF5*(vRoe + cRoe*normalised_y);
        deltaF5Flux3 = radicalF5*(enthalpyRoe + cRoe*VcontravariantRoe); 
     
        //Calcul des flux
        dissipRoeFlux0 = (deltaF1Flux0 + deltaF23Flux0 + deltaF5Flux0)*0.5*sqrt(pow(normalised_x,2) + pow(normalised_y,2));
        dissipRoeFlux1 = (deltaF1Flux1 + deltaF23Flux1 + deltaF5Flux1)*0.5*sqrt(pow(normalised_x,2) + pow(normalised_y,2));
        dissipRoeFlux2 = (deltaF1Flux2 + deltaF23Flux2 + deltaF5Flux2)*0.5*sqrt(pow(normalised_x,2) + pow(normalised_y,2));
        dissipRoeFlux3 = (deltaF1Flux3 + deltaF23Flux3 + deltaF5Flux3)*0.5*sqrt(pow(normalised_x,2) + pow(normalised_y,2));

        //Calcul du résidu dissip (négatif car R = InviscidResidual - residualDissip et dans le calcul de residual on a mis +)
        meshData_->residualDissip_rho_[cellLeft] += -dissipRoeFlux0; 
        meshData_->residualDissip_u_[cellLeft] += -dissipRoeFlux1;
        meshData_->residualDissip_v_[cellLeft] += -dissipRoeFlux2;
        meshData_->residualDissip_p_[cellLeft] += -dissipRoeFlux3;

        meshData_->residualDissip_rho_[cellRight] -= -dissipRoeFlux0; 
        meshData_->residualDissip_u_[cellRight] -= -dissipRoeFlux1;
        meshData_->residualDissip_v_[cellRight] -= -dissipRoeFlux2;
        meshData_->residualDissip_p_[cellRight] -= -dissipRoeFlux3;

    
    }
    //cout << "dflux DDDDDDOONNNE" << endl;
}