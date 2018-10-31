
#include "DataUpdater.h"


DataUpdater::DataUpdater(NSCData *iNSCData, Mesh_Data *iMeshData) :
                                nscData_(iNSCData),
                                meshData_(iMeshData)
{

}

DataUpdater::~DataUpdater()
{
    
}
void DataUpdater::update_boundary()
{
    
    double g = nscData_->gamma_;
    double gm1 = g - 1;

    double cfree = std::sqrt(g*nscData_->pInfini_/nscData_->rhoInfini_);

    int nbCells = meshData_->NCells_;
    int nbCellsTotal = meshData_->NCellsTotal_;

    double ro_wall, u_wall, v_wall, p_wall;
    double ro_farField, u_farField, v_farField, p_farField;
    double normal_x, normal_y, normal_length;
    double robc, ubc, vbc, pbc;

    int ghostType;
    int cellDomainID;
    int farFieldCellID;
    int faceID;

    double ro1, uu1, vv1, pp1, cc1, un1;
    double uubc, vvbc, ppbc;
    double unf, chav_in, R4e, R4f, R4, chav_out, R5e, R5f, R5, unbc, ccbc, dun;
    double uubc_inlet, vvbc_inlet, ssbc_inlet, uubc_outlet, vvbc_outlet, ssbc_outlet;
    double ela, elb, ssbc, cc2;
    int el;

    //Loop on ghost cells
    for(int i = nbCells; i < nbCellsTotal; i++)
    {   

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
        if (normal_length != 0)
        {
            //Normalizing vectors x and y.
            normal_x/=(normal_length);
            normal_y/=(normal_length);

            normal_x*=(-1);
            normal_y*=(-1);

            un1 = uu1*normal_x + vv1*normal_y;

        }
        else
        {
            std::cout << "Error: Length of vector cannot be 0";
            return;
        }

        // (arfoil = 0, farfield = 1)
        ghostType = meshData_->GhostType_[i - nbCells];

        if(ghostType == 0) // Wall
        {
            //4. Set right parameters to ghost cell by calculation (Blazek)
            robc = ro1;
            uubc = uu1 - un1*normal_x;
            vvbc = vv1 - un1*normal_y;
            ppbc = pp1;

            meshData_->rho_[i] = robc;
            meshData_->p_[i] = ppbc;
            meshData_->u_[i] = 2*uubc - uu1;
            meshData_->v_[i] = 2*vvbc - vv1;
        }
        else if (ghostType == 1) // Far-field
        {
            
            cc1 = std::sqrt(g * pp1/ro1);
            unf = nscData_->uInfini_*normal_x + nscData_->vInfini_*normal_y;
            chav_in = unf+cfree;
            el = chav_in / std::abs(chav_in);
            R4e = un1+2*cc1/gm1;
            R4f = unf+2.*cfree/gm1;
            R4=0.5*((1+el)*R4f+(1.-el)*R4e);
            chav_out = un1 - cc1;
            el = chav_out/std::abs(chav_out);
            R5e = un1 - 2*cc1/gm1;
            R5f = unf - 2*cfree/gm1;
            R5 = 0.5*((1+el)*R5f+(1-el)*R5e);
            unbc = 0.5*(R4+R5);
            ccbc = 0.25*(R4-R5)*gm1;
            el = unbc / std::abs(unbc);
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
        else
        {
            cout << "Error: Cannot regnonize type of ghost." << endl;
            return;
        }

    }


}
