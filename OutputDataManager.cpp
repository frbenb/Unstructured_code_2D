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

    if(outputTecplot.is_open())
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
        cout << "File " << filename << " closed." << endl;
    }

    else
    {
        cout << "Error: cannot create file for tecplot." << endl;
    }
}

void OutputDataManager::calculate_node_coefficients()
{
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
        meshData_->rho_nodes_[nodeID] = rho/meshData_->nodeNCell_[nodeID];
        meshData_->u_nodes_[nodeID] = u/meshData_->nodeNCell_[nodeID];
        meshData_->v_nodes_[nodeID] = v/meshData_->nodeNCell_[nodeID];
        meshData_->p_nodes_[nodeID] = p/meshData_->nodeNCell_[nodeID];
    }
}

void OutputDataManager::writeMeshContent(){
    string nodesFileName = "./bin/nodes_coord.txt";
    ofstream nodesFile;
    nodesFile.open(nodesFileName);
    nodesFile << meshData_->NNodes_ << endl;
    nodesFile << "NodeID     x      y" << endl;

    for (unsigned int i = 0; i < meshData_->NNodes_; i++){
        nodesFile << i << " " << meshData_->Nodes_x_[i] << " " << meshData_->Nodes_y_[i] << endl;
    }
    nodesFile.close();

    string cellNodesFileName = "./bin/cell_nodes.txt";
    ofstream cellNodesFile;
    cellNodesFile.open(cellNodesFileName);
    cellNodesFile << meshData_->NCellsTotal_ << endl;
    cellNodesFile << "CellID     nNodes      nodes" << endl;

    for (unsigned int i = 0; i < meshData_->NCellsTotal_; i++){
        cellNodesFile << i << " " << meshData_->CellNfaces_[i];

        for (unsigned int j = 0; j < meshData_->CellNfaces_[i]; j++){
            cellNodesFile << " " << meshData_->Cell2Node_[i][j];
        }
        cellNodesFile << endl;
    }
    cellNodesFile.close();

    string cellFacesFileName = "./bin/cell_faces.txt";
    ofstream cellFacesFile;
    cellFacesFile.open(cellFacesFileName);
    cellFacesFile << meshData_->NCellsTotal_ << endl;
    cellFacesFile << "CellID     nNodes      faces" << endl;

    for (unsigned int i = 0; i < meshData_->NCellsTotal_; i++){
        cellFacesFile << i << " " << meshData_->CellNfaces_[i];

        for (unsigned int j = 0; j < meshData_->CellNfaces_[i]; j++){
            cellFacesFile << " " << meshData_->Cell2Face_[i][j];
        }
        cellFacesFile << endl;
    }
    cellFacesFile.close();

    string cellCellsFileName = "./bin/cell_cells.txt";
    ofstream cellCellsFile;
    cellCellsFile.open(cellCellsFileName);
    cellCellsFile << meshData_->NCellsTotal_ << endl;
    cellCellsFile << "CellID     nNodes      cells" << endl;

    for (unsigned int i = 0; i < meshData_->NCellsTotal_; i++){
        cellCellsFile << i << " " << meshData_->CellNfaces_[i];

        for (unsigned int j = 0; j < meshData_->CellNfaces_[i]; j++){
            cellCellsFile << " " << meshData_->Cell2Cell_[i][j];
        }
        cellCellsFile << endl;
    }
    cellCellsFile.close();

    string faceNodesFileName = "./bin/face_nodes.txt";
    ofstream faceNodesFile;
    faceNodesFile.open(faceNodesFileName);
    faceNodesFile << meshData_->NFaces_ << endl;
    faceNodesFile << "FaceID     nodes" << endl;

    for (unsigned int i = 0; i < meshData_->NFaces_; i++){
        faceNodesFile << i << " ";
        for (unsigned int j = 0; j < 2; j++){
            faceNodesFile << " " << meshData_->Face2Node_[i][j];
        }
        faceNodesFile << endl;
    }
    faceNodesFile.close();

    string faceCellsFileName = "./bin/face_cells.txt";
    ofstream faceCellsFile;
    faceCellsFile.open(faceCellsFileName);
    faceCellsFile << meshData_->NFaces_ << endl;
    faceCellsFile << "FaceID     cells" << endl;

    for (unsigned int i = 0; i < meshData_->NFaces_; i++){
        faceCellsFile << i << " ";
        for (unsigned int j = 0; j < 2; j++){
            faceCellsFile << " " << meshData_->Face2Cell_[i][j];
        }
        faceCellsFile << endl;
    }
    faceCellsFile.close();

    string ghostTypeFileName = "./bin/ghost_type.txt";
    ofstream ghostTypeFile;
    ghostTypeFile.open(ghostTypeFileName);
    ghostTypeFile << meshData_->NCellsGhost_ << endl;
    ghostTypeFile << "CellID     type" << endl;

    for (unsigned int i = 0; i < meshData_->NCellsGhost_; i++){
        ghostTypeFile << i + meshData_->NCells_ << " ";
        ghostTypeFile << meshData_->GhostType_[i];
        ghostTypeFile << endl;
    }
    ghostTypeFile.close();

    string nodeCellsFileName = "./bin/node_cells.txt";
    ofstream nodeCellsFile;
    nodeCellsFile.open(nodeCellsFileName);
    nodeCellsFile << meshData_->NNodes_ << endl;
    nodeCellsFile << "CellID   nCells      cells" << endl;

    for (unsigned int i = 0; i < meshData_->NNodes_; i++){
        nodeCellsFile << i << " " << meshData_->nodeNCell_[i] << " " ;
        for (unsigned int j = 0; j < meshData_->nodeNCell_[i]; j++){
            nodeCellsFile << " " << meshData_->Node2Cell_[i][j];
        }
        nodeCellsFile << endl;
    }
    nodeCellsFile.close();

    string nodesDataName = "./bin/nodes_data.txt";
    ofstream nodesData;
    nodesData.open(nodesDataName);
    nodesData << meshData_->NNodes_ << endl;
    nodesData << "NodeID     rho    u    v    p" << endl;

    for (unsigned int i = 0; i < meshData_->NNodes_; i++){
        nodesData << i << " " << meshData_->rho_nodes_[i] << " " << meshData_->u_nodes_[i] << " " << meshData_->v_nodes_[i] << " " << meshData_->p_nodes_[i] << endl;
    }
    nodesData.close();

    string CellsDataName = "./bin/cells_data.txt";
    ofstream CellsData;
    CellsData.open(CellsDataName);
    CellsData << meshData_->NCellsTotal_ << endl;
    CellsData << "CellID     rho    u    v    p" << endl;

    for (unsigned int i = 0; i < meshData_->NCellsTotal_; i++){
        CellsData << i << " " << meshData_->rho_[i] << " " << meshData_->u_[i] << " " << meshData_->v_[i] << " " << meshData_->p_[i] << endl;
    }
    CellsData.close();

    string FacesNormalName = "./bin/normals.txt";
    ofstream FacesNormals;
    FacesNormals.open(FacesNormalName);
    FacesNormals << meshData_->NFaces_ << endl;
    FacesNormals << "FaceID     normal_x    normal_y" << endl;

    for (unsigned int i = 0; i < meshData_->NFaces_; i++){
        FacesNormals << i << " " << meshData_->normal_x_[i] << " " << meshData_->normal_y_[i] << endl;
    }
    FacesNormals.close();

    string FacesCenterName = "./bin/face_centers.txt";
    ofstream FacesCenters;
    FacesCenters.open(FacesCenterName);
    FacesCenters << meshData_->NFaces_ << endl;
    FacesCenters << "FaceID     center_x    center_y" << endl;

    for (unsigned int i = 0; i < meshData_->NFaces_; i++){
        FacesCenters << i << " " << meshData_->FaceCenter_x_[i] << " " << meshData_->FaceCenter_y_[i] << endl;
    }
    FacesCenters.close();

    string CellsCenterName = "./bin/cell_centers.txt";
    ofstream CellsCenters;
    CellsCenters.open(CellsCenterName);
    CellsCenters << meshData_->NCellsTotal_ << endl;
    CellsCenters << "CellID     center_x    center_y" << endl;

    for (unsigned int i = 0; i < meshData_->NCellsTotal_; i++){
        CellsCenters << i << " " << meshData_->cellCenter_x_[i] << " " << meshData_->cellCenter_y_[i] << endl;
    }
    CellsCenters.close();

    string cellAreaName = "./bin/cell_area.txt";
    ofstream cellAreaFile;
    cellAreaFile.open(cellAreaName);
    cellAreaFile << meshData_->NCellsTotal_ << endl;
    cellAreaFile << "CellID     Area" << endl;
    for (unsigned int i = 0; i < meshData_->NFaces_; i++){
        cellAreaFile << i << " " << meshData_->cellArea_[i] << endl;
    }
    cellAreaFile.close();

    string dFluxName = "./bin/dflux.txt";
    ofstream dFluxFile;
    dFluxFile.open(dFluxName);
    dFluxFile << meshData_->NCellsTotal_ << endl;
    dFluxFile << "CellID     DissipResRho      DissipResU     DissipResV      DissipResP " << endl;
    for (unsigned int i = 0; i < meshData_->NCellsTotal_; i++){
        dFluxFile << i << " " << meshData_->residualDissip_rho_[i] << " " << meshData_->residualDissip_u_[i] << " " << meshData_->residualDissip_v_[i] << " " << meshData_->residualDissip_p_[i] << endl;
    }
    dFluxFile.close();
}
