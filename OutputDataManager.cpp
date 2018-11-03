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

void OutputDataManager::writeOutputTecplot()
{
    //File opens for output operations (ios::out) and previous text in it is deleted (ios::trunc)
    string filename = "tecplot.dat";
    ofstream outputTecplot(filename, ios::out | ios::trunc);

    if(outputTecplot)
    {
        cout << "File " << filename << " has been created for tecplot" << endl;

        //HEADER
        outputTecplot << "VARIABLES=\"X\",\"Y\",\"RO\",\"U\",\"V\",\"P\"";
        outputTecplot << endl;

        outputTecplot << "ZONE T=\"Element0\"";
        outputTecplot << endl;

        outputTecplot << "Nodes=" << meshData_->NNodes_;
        outputTecplot << ",";
        outputTecplot << "Elements=" << meshData_->NCells_; 
        outputTecplot << ",";
        outputTecplot << "ZONETYPE=FEQuadrilateral"; //For quadrilateral elements
        outputTecplot << endl;

        outputTecplot << "DATAPACKING=BLOCK"; //OBLIGATORY TO USE BLOCK IF WE HAVE CELLCENTERED VARIABLES
        outputTecplot << endl;
        
        outputTecplot << "VARLOCATION=([3,4,5,6]=CELLCENTERED)"; //Variables Rho, U, V and P only
        outputTecplot << endl;

        cout << "Header: written." << endl;

        //VARIABLES
        //Writing X coordinates
        for (unsigned int i=0; i < meshData_->NNodes_; i++)
        {
            outputTecplot << meshData_->Nodes_x_[i];
            outputTecplot << endl;
        }

        //Writing Y coordinates
        for (unsigned int i=0; i < meshData_->NNodes_; i++)
        {
            outputTecplot <<  meshData_->Nodes_y_[i];
            outputTecplot << endl;
        } 

        //Writting Rho varibles (cell centered)
        for (unsigned int i=0; i < meshData_->NCells_; i++)
        {
            outputTecplot <<  meshData_->rho_[i];
            outputTecplot << endl;
        }

        //Writting U variables (cell vertex)
        for (unsigned int i=0; i < meshData_->NCells_; i++)
        {
            outputTecplot <<  meshData_->u_[i];
            outputTecplot << endl;
        }

        //Writting V variables (cell vertex)
        for (unsigned int i=0; i < meshData_->NCells_; i++)
        {
            outputTecplot <<  meshData_->v_[i];
            outputTecplot << endl;
        }

        //Writting P variables (cell vertex)
        for (unsigned int i=0; i < meshData_->NCells_; i++)
        {
            outputTecplot <<  meshData_->p_[i];
            outputTecplot << endl;
        }

        //Writting Cell2Node
        for (unsigned int i=0; i < meshData_->NCells_; i++)
        {
            for (unsigned int j=0; j < meshData_-> CellNfaces_[i]; j++)
            {
                outputTecplot << meshData_->Cell2Node_[i][j] + 1 << " ";
            }

            for (unsigned int k=0; k<4-meshData_-> CellNfaces_[i]; k++)
            {
                outputTecplot << meshData_->Cell2Node_[i][meshData_->CellNfaces_[i]-1] + 1 << " ";
            }
            outputTecplot << endl; 
        }

        cout << "Variables: written." << endl;
      
        outputTecplot.close();
        cout << "File " << filename << " is close." << endl;
    }

    else
    {
        cout << "Error: cannot create file for tecplot." << endl;
    }