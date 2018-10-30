
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

    double ro1, u1, v1, p1, normal_x, normal_y, normal_length, un1;
    double robc, ubc, vbc, pbc;

    int ghostType;

    //Loop on ghost cells
    for(int i = nbCells; i < nbCellsTotal; i++)
    {   
        //Get the ghost type (arfoil = 0, farfield = 1)
        ghostType = meshData_->GhostType_[i - nbCells];

        if(ghostType == 0)
        {
            //Wall case
            ro1 = meshData_->rho_[i];
            u1 = meshData_->u_[i];
            v1 = meshData_->v_[i];
            p1 = meshData_->p_[i];

            normal_x = meshData_->normal_x_[i];
            normal_y = meshData_->normal_y_[i];

            normal_length = std::sqrt(normal_x*normal_x + normal_y*normal_y);

            normal_x/=normal_length * -1;
            normal_y/=normal_length * -1;

            un1 = u1*normal_x + v1*normal_y;

            robc = ro1;
            ubc = u1;
            vbc = v1;
            pbc = p1;

            

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
