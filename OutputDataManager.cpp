#include "OutputDataManager.h"


using namespace std;

OutputDataManager::OutputDataManager(NSCData *iNSCData, Mesh_Data *iMeshData):
                                    nscData_(iNSCData), 
                                    meshData_(iMeshData)
{
    // Nothing.
}

OutputDataManager::~OutputDataManager()
{
    
}

void OutputDataManager::showSU2ReadingData()
{
    
    //Create Tecplot file
    
    ofstream outputTecplot("tecplot.txt", ios::out | ios::trunc);

    if(outputTecplot)
    {
      
        outputTecplot << "VARIABLES=\"X\",\"Y\",\"RO\",\"U\",\"V\",\"P\"\n";
        outputTecplot << "ZONE T=\"Element0\"\n";

        outputTecplot << "Nodes=";
        outputTecplot << meshData_->NNodes_;
        outputTecplot << ",";
        outputTecplot << "Elements=";
        outputTecplot << meshData_->NCells_;
        outputTecplot << ",";
        outputTecplot << "ZONETYPE=FETRIANGLE\n";

        outputTecplot << "DATAPACKING=BLOCK\n";
        outputTecplot << "VARLOCATION=\n"; // Need to specify the Var location.

        //Writing X coordinates
        for (unsigned int i=0; i < meshData_->NNodes_; i++)
        {
            outputTecplot << meshData_->Nodes_x_[i];
            outputTecplot << "\n";
        }

        //Writing Y coordinates
        for (unsigned int i=0; i < meshData_->NNodes_; i++)
        {
            outputTecplot <<  meshData_->Nodes_y_[i];
        } 

      
        
        outputTecplot.close();
    }
    else
    {
        cout << "Error: cannot create file for tecplot." << endl;
    }
    
    


}

void OutputDataManager::calculate_node_coefficients(){
    double rho;
    double u;
    double v;
    double p;
    unsigned int cellID;

    for (unsigned int nodeID = 0; nodeID < meshData_->NNodes_; nodeID++){
        rho = 0;
        u = 0;
        v = 0;
        p = 0;
        for (unsigned int cellN = 0; cellN < meshData_->nodeNCell_[nodeID]; cellN++){
            cellID = meshData_->Node2Cell_[nodeID][cellN];
            rho += meshData_->rho_[cellID];
            u += meshData_->u_[cellID];
            v += meshData_->v_[cellID];
            p += meshData_->p_[cellID];
        }
        meshData_->rho_nodes_[cellID] = rho/meshData_->nodeNCell_[nodeID];
        meshData_->u_nodes_[cellID] = u/meshData_->nodeNCell_[nodeID];
        meshData_->v_nodes_[cellID] = v/meshData_->nodeNCell_[nodeID];
        meshData_->p_nodes_[cellID] = p/meshData_->nodeNCell_[nodeID];
    }
}









