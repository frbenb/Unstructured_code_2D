
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
    double normal_x, normal_y, normal_length, un1;
    double robc, ubc, vbc, pbc;

    int ghostType;
    int cellDomainID;
    int farFieldCellID;
    int faceID;

    double ro1, uu1, vv1, pp1, cc1, un1;

    //Loop on ghost cells
    for(int i = nbCells; i < nbCellsTotal; i++)
    {   

        //1. Get the Cell ID for the domain cell next to the ghost (index at 0)
        cellDomainID = meshData_->Cell2Cell_[i][0];

        //3. Get the normal vector at the face
        faceID = meshData_->Cell2Face_[i][0];
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
        }
        else
        {
            std::cout << "Error: Length of vector cannot be 0";
            return;
        }

        //Get the ghost type (arfoil = 0, farfield = 1)
        ghostType = meshData_->GhostType_[i - nbCells];

        if(ghostType == 0)
        {
            //4. Set right parameters to ghost cell by calculation (Blazek)
            meshData_->u_[i] = meshData_->u_[cellDomainID] - 2*meshData_->u_[cellDomainID]*(normal_x);
            meshData_->v_[i] = meshData_->v_[cellDomainID] - 2*meshData_->v_[cellDomainID]*(normal_y);
            meshData_->p_[i] = meshData_->p_[cellDomainID];
            meshData_->rho_[i] = meshData_->rho_[cellDomainID];
        }
        else if (ghostType == 1)
        {
            
            normal_x*=-1;
            normal_y*=-1;

            ro1 = meshData_->rho_[cellDomainID];
            uu1 = meshData_->u_[cellDomainID];
            vv1 = meshData_->v_[cellDomainID];
            pp1 = meshData_->p_[cellDomainID];
            cc1 = std::sqrt(g * pp1/ro1);




        }
        else
        {
            cout << "Error: Cannot regnonize type of ghost." << endl;
            return;
        }

    }


}
