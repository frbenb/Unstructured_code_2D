
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
    double normal_x, normal_y, normal_length, un1;
    double robc, ubc, vbc, pbc;

    int ghostType;
    int wallCellID;
    int faceID;

    //Loop on ghost cells
    for(int i = nbCells; i < nbCellsTotal; i++)
    {   
        //Get the ghost type (arfoil = 0, farfield = 1)
        ghostType = meshData_->GhostType_[i - nbCells];

        if(ghostType == 0)
        {
            //1. Get the Cell ID for the domain cell next to the ghost (index at 0)
            wallCellID = meshData_->Cell2Cell_[i][0];

            //2. Get the parameters of the cell wall
            ro_wall = meshData_->rho_[i];
            u_wall = meshData_->u_[i];
            v_wall = meshData_->v_[i];
            p_wall = meshData_->p_[i];

            //3. Get the normal vector at the face
            faceID = meshData_->Cell2Face_[i][0];
            normal_x = meshData_->normal_x_[faceID];
            normal_y = meshData_->normal_y_[faceID];

            normal_length = std::sqrt(normal_x*normal_x + normal_y*normal_y);

            //3. Calculate normal and normalize
            if (normal_length != 0)
            {
                normal_x/=(normal_length);
                normal_y/=(normal_length);
            }
            else
            {
                std::cout << "Error: Normal vector cannot be 0";
                return;
            }

            //4. Set right parameters to ghost cell by calculation (Blazek)
            meshData_->u_[i] = meshData_->u_[wallCellID] - 2*meshData_->u_[wallCellID]*(normal_x);
            meshData_->v_[i] = meshData_->v_[wallCellID] - 2*meshData_->v_[wallCellID]*(normal_y);
            meshData_->p_[i] = meshData_->p_[wallCellID];
            meshData_->rho_[i] = meshData_->rho_[wallCellID];
        }
        else if (ghostType == 1)
        {
            // Far field case

            

        }
        else
        {
            cout << "Error: Cannot regnonize type of ghost." << endl;
            return;
        }

    }


}
