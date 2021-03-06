
#include "DataUpdater.h"


DataUpdater::DataUpdater(NSCData *iNSCData, Mesh_Data *iMeshData) :
                                nscData_(iNSCData),
                                meshData_(iMeshData)
{

}

DataUpdater::~DataUpdater()
{
    
}

void DataUpdater::update_solution(double iRkAlpha)
{
    double g, alpha, dt, ronew, unew, vnew, enew, ro, u, v, p;
    double ro0, u0, v0, p0;
    double Ri_ro, Ri_u, Ri_v, Ri_p;

    g = nscData_->gamma_;
    alpha = iRkAlpha;

    //Loop on cells of restricted domain
    for(unsigned int i(0);i < meshData_->NCells_;i++)
    {
        //Get values of solutions
        ro = meshData_->rho_[i];
        u = meshData_->u_[i];
        v = meshData_->v_[i];
        p = meshData_->p_[i];

        //Get conservative variables
        ro0 = meshData_->rho_0_[i];
        u0 = meshData_->u_0_[i];
        v0 = meshData_->v_0_[i];
        p0 = meshData_->p_0_[i]; // Energy here, not pressure

        //Get Inviscid residuals
        Ri_ro = meshData_->residualInviscid_rho_[i];
        Ri_u = meshData_->residualInviscid_u_[i];
        Ri_v = meshData_->residualInviscid_v_[i];
        Ri_p = meshData_->residualInviscid_p_[i];

        //Get delta time
        dt = meshData_->deltaT_[i];

        //Proceed to new solution values by Euler conversion.
        ronew = ro0 - (alpha * dt * Ri_ro)/meshData_->cellArea_[i];
        unew = u0 - (alpha * dt *  Ri_u)/meshData_->cellArea_[i];
        vnew = v0 - (alpha * dt*  Ri_v)/meshData_->cellArea_[i];
        enew = p0 - (alpha * dt * Ri_p)/meshData_->cellArea_[i];


        meshData_->rho_[i] = ronew;
        meshData_->u_[i] = unew/ronew;
        meshData_->v_[i] = vnew/ronew;
        
        meshData_->p_[i] = (g - 1.0) * (enew - 0.5*(unew*unew + vnew*vnew)/ ronew);       
    }
}

void DataUpdater::update_boundary()
{
    
    double g = nscData_->gamma_;
    double gm1 = g - 1;

    double cfree = std::sqrt(g*nscData_->pInfini_/nscData_->rhoInfini_);

    unsigned int nbCells = meshData_->NCells_;
    unsigned int nbCellsTotal = meshData_->NCellsTotal_;

    double ro_wall, u_wall, v_wall, p_wall;
    double ro_farField, u_farField, v_farField, p_farField;
    double normal_x, normal_y, normal_length;
    double robc, ubc, vbc, pbc;

    unsigned int ghostType;
    unsigned int cellDomainID;
    unsigned int farFieldCellID;
    unsigned int faceID;

    double ro1, uu1, vv1, pp1, cc1, un1;
    double uubc, vvbc, ppbc;
    double unf, chav_in, R4e, R4f, R4, chav_out, R5e, R5f, R5, unbc, ccbc, dun;
    double uubc_inlet, vvbc_inlet, ssbc_inlet, uubc_outlet, vvbc_outlet, ssbc_outlet;
    double ela, elb, ssbc, cc2;
    int el; // should be unsigned?

    //Loop on ghost cells //Normale qui pointe vers l'extérieure du domaine toujours mieux
    for(unsigned int i = nbCells; i < nbCellsTotal; i++)
    {   
        double p_face, rho_face, u_face, v_face;
        //1. Get the Cell ID for the domain cell next to the ghost (index at 0)
        cellDomainID = meshData_->Cell2Cell_[i][0];

        //3. Get the normal vector at the face
        faceID = meshData_->Cell2Face_[i][0];
        
        ro1 = meshData_->rho_[cellDomainID];
        uu1 = meshData_->u_[cellDomainID];
        vv1 = meshData_->v_[cellDomainID];
        pp1 = meshData_->p_[cellDomainID];

        normal_x = meshData_->normal_x_[faceID];
        normal_y = meshData_->normal_y_[faceID];
        normal_length = std::sqrt(normal_x*normal_x + normal_y*normal_y);

        //3. Calculate normal and normalize
            //Normalizing vectors x and y.
        normal_x/=(normal_length);
        normal_y/=(normal_length);

            //normal_x*=(-1);
            //normal_y*=(-1);

        un1 = uu1*normal_x + vv1*normal_y;


        // (arfoil = 0, farfield = 1)
        ghostType = meshData_->GhostType_[i - nbCells];

        if(ghostType == 0) // Wall
        {
            //4. Set right parameters to ghost cell by calculation (Blazek)
            
            robc = ro1;
            uubc = uu1 - 2.0 * un1*normal_x;
            vvbc = vv1 - 2.0 * un1*normal_y;
            ppbc = pp1;

            meshData_->rho_[i] = robc;
            meshData_->p_[i] = ppbc;
            meshData_->u_[i] = uubc;
            meshData_->v_[i] = vvbc;
        }
        else if (ghostType == 1) // Far-field Riemann
        {
           
            cc1 = std::sqrt(g * pp1/ro1);
            unf = nscData_->uInfini_*normal_x + nscData_->vInfini_*normal_y;
            chav_in = unf+cfree;
            el = chav_in / std::fabs(chav_in);
            R4e = un1+2*cc1/gm1;
            R4f = unf+2.*cfree/gm1;
            R4=0.5*((1+el)*R4f+(1.-el)*R4e);
            chav_out = un1 - cc1;
            el = chav_out/std::fabs(chav_out);
            R5e = un1 - 2*cc1/gm1;
            R5f = unf - 2*cfree/gm1;
            R5 = 0.5*((1+el)*R5f+(1-el)*R5e);
            unbc = 0.5*(R4+R5);
            ccbc = 0.25*(R4-R5)*gm1;
            el = unbc / std::fabs(unbc);
            dun = unbc-unf;
            uubc_inlet = nscData_->uInfini_ + dun*normal_x;
            vvbc_inlet = nscData_->vInfini_ + dun*normal_y;
            ssbc_inlet = nscData_->pInfini_ / std::pow(nscData_->rhoInfini_, g);
            dun = unbc - un1;
            uubc_outlet = uu1+dun*normal_x;
            vvbc_outlet = vv1+dun*normal_y;
            ssbc_outlet = pp1/ std::pow(ro1, g);

            ela=0.5*(1.+el);
            elb=0.5*(1.-el);

            uubc=ela*uubc_inlet+elb*uubc_outlet;
            vvbc=ela*vvbc_inlet+elb*vvbc_outlet;
            ssbc=ela*ssbc_inlet+elb*ssbc_outlet;

            cc2=ccbc*ccbc;
            robc=cc2/g/ssbc;
            robc=std::pow(robc,1./gm1);
            ppbc=robc*cc2/g;

            meshData_->rho_[i] = 2*robc - ro1;
            meshData_->u_[i] = 2*uubc - uu1;
            meshData_->v_[i] = 2*vvbc - vv1;
            meshData_->p_[i] = 2*ppbc - pp1;
        }
        else if (ghostType == 99) {
            // Blazek wall

            double pInf = nscData_->pInfini_;
            double uInf = nscData_->uInfini_;
            double vInf = nscData_->vInfini_;
            double rhoInf = nscData_->rhoInfini_;
            double gammaSqrt = sqrt(nscData_->gamma_);
            double localSpeedOfSound = gammaSqrt*sqrt(pp1/ro1); //pp1, ro1 donnée à l'intérieur du domaine
            double roC0 = ro1*localSpeedOfSound;     

            //si écoule entre (produit scalaire negatif), on suppose ghost cell à droite
            if (un1 <= 0.0) { 
                p_face = 0.5*(pInf + pp1 - roC0*(normal_x*(uInf - uu1) + normal_y*(vInf-vv1)));
                rho_face = rhoInf + (p_face - pInf)/(localSpeedOfSound*localSpeedOfSound);
                u_face = uInf - normal_x*(pInf - p_face)/roC0;
                u_face = vInf - normal_y*(pInf - p_face)/roC0;
            }
            
            else {
                p_face = pInf;
                rho_face = ro1 + (p_face - pp1)/(localSpeedOfSound*localSpeedOfSound);
                u_face = uu1 + normal_x*(pp1 - p_face)/roC0;
                u_face = vv1 + normal_y*(pp1 - p_face)/roC0;

            }

            meshData_->rho_[i] = 2*rho_face - ro1;
            meshData_->u_[i] = 2*u_face- uu1;
            meshData_->v_[i] = 2*v_face - vv1;
            meshData_->p_[i] = 2*p_face - pp1;

        }

        else if (ghostType == 2) { ///suo
        
        double pInf = nscData_->pInfini_;
        double uInf = nscData_->uInfini_;
        double vInf = nscData_->vInfini_;
        double rhoInf = nscData_->rhoInfini_;
        double gammaSqrt = sqrt(nscData_->gamma_);
        double localSpeedOfSound = gammaSqrt*sqrt(pp1/ro1); //pp1, ro1 donnée à l'intérieur du domaine
        double roC0 = ro1*localSpeedOfSound;
        double p_face, rho_face, u_face, v_face;

            //si écoule entre (produit scalaire negatif), on suppose ghost cell à droite
            // enlever runge kutta, alpha = 1, un seul stage, une itération par avancement, cfl < 1
            // faire une itération de ça 
            // remettre le meme if else, 

            if (uu1 <= 0) {  //entre
                p_face = pInf;
                u_face = uInf;
                v_face = vInf;
                rho_face = rhoInf;

            }
            
            else {

                p_face = pp1;
                u_face = uu1;
                v_face = vv1;
                rho_face = ro1;

            }

            meshData_->rho_[i] = 2.0*rho_face - ro1;
            meshData_->u_[i] = 2.0*u_face- uu1;
            meshData_->v_[i] = 2.0*v_face - vv1;
            meshData_->p_[i] = 2.0*p_face - pp1;

        }


        
        else
        {
            cout << "Error: Cannot regnonize type of ghost." << endl;
            return;
        }

    }


}
