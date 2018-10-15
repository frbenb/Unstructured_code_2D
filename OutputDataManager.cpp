#include "OutputDataManager.h"


using namespace std;

OutputDataManager::OutputDataManager(NSCData *iNSCData, Mesh_Data *iMeshData):
                                    nscData_(iNSCData), meshData_(iMeshData)
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

















