
#include "Mesh_Data.h"
#include <iostream>
#include <fstream>
#include "arrayMemory.h"
using namespace std;

/***********************************CONSTRUCTOR BY DEFAULT****************************************/
/***********************************CONSTRUCTOR BY DEFAULT****************************************/
Mesh_Data::Mesh_Data(): 
Nodes_x_(nullptr), Nodes_y_(nullptr), 
Cell2Node_(nullptr), Cell2Face_(nullptr), Face2Node_(nullptr), Face2Cell_(nullptr), Cell2Cell_(nullptr), Node2Cell_(nullptr),
CellNfaces_(nullptr), NCells_(0), NCellsGhost_(0), NFaces_(0), NNodes_(0), Volume_(nullptr), Residu_(nullptr), GhostType_(nullptr),
rho_(nullptr), u_(nullptr), v_(nullptr), p_(nullptr),
itl_(0), 
itu_(0),                     
inci_(0), 
incj_(0),                 
cellArea_(nullptr),         
normal_x_(nullptr), 		
normal_y_(nullptr),
FaceCenter_x_(nullptr),
FaceCenter_y_(nullptr),
rho_nodes_(nullptr),         
u_nodes_(nullptr), 
v_nodes_(nullptr), 
p_nodes_(nullptr),           
rho_0_(nullptr),             
u_0_(nullptr), 
v_0_(nullptr), 
p_0_(nullptr), 
residualInviscid_rho_(nullptr),  
residualInviscid_u_(nullptr), 
residualInviscid_v_(nullptr), 
residualInviscid_p_(nullptr), 
residualDissip_rho_(nullptr),   
residualDissip_u_(nullptr), 
residualDissip_v_(nullptr), 
residualDissip_p_(nullptr), 
tmp_rho_(nullptr),               
tmp_u_(nullptr), 
tmp_v_(nullptr), 
tmp_p_(nullptr), 
deltaT_(nullptr),                
spec_x_(nullptr),                 
spec_y_(nullptr)
{
}


/************************************DESTRUCTOR BY DEFAULT****************************************/
Mesh_Data::~Mesh_Data()
{
    Nodes_x_ = deallocate1Ddbl(Nodes_x_);
    Nodes_y_ = deallocate1Ddbl(Nodes_y_);
    CellNfaces_ = deallocate1Dint(CellNfaces_);
    Volume_ = deallocate1Ddbl(Volume_);
    Residu_ = deallocate1Ddbl(Residu_);
    GhostType_ = deallocate1Dint(GhostType_);
    rho_ = deallocate1Ddbl(rho_);
    u_ = deallocate1Ddbl(u_);
    v_ = deallocate1Ddbl(v_);
    p_ = deallocate1Ddbl(p_);
    normal_x_ = deallocate1Ddbl(normal_x_);
    normal_y_ = deallocate1Ddbl(normal_y_);

    Cell2Node_ = deallocate2Dint(Cell2Node_, NCellsTotal_);
    Cell2Face_ = deallocate2Dint(Cell2Face_, NCellsTotal_);
    Face2Node_ = deallocate2Dint(Face2Node_, NFaces_);
    Face2Cell_ = deallocate2Dint(Face2Cell_, NFaces_);
    Cell2Cell_ = deallocate2Dint(Cell2Cell_, NCellsTotal_);
    Node2Cell_ = deallocate2Dint(Node2Cell_, NNodes_);

    spec_x_ = deallocate1Ddbl(spec_x_);
    spec_y_ = deallocate1Ddbl(spec_y_);

    residualInviscid_rho_ = deallocate1Ddbl(residualInviscid_rho_);
    residualInviscid_u_ = deallocate1Ddbl(residualInviscid_u_);
    residualInviscid_v_ = deallocate1Ddbl(residualInviscid_v_ );
    residualInviscid_p_ = deallocate1Ddbl(residualInviscid_p_);

    residualDissip_rho_ = deallocate1Ddbl(residualDissip_rho_);
    residualDissip_u_ = deallocate1Ddbl(residualDissip_u_);
    residualDissip_v_ = deallocate1Ddbl(residualDissip_v_);
    residualDissip_p_ = deallocate1Ddbl(residualDissip_p_);


}

void Mesh_Data::write_stuff(){
    string nodesFileName = "./bin/nodes_coord.txt";
    ofstream nodesFile;
    nodesFile.open(nodesFileName);
    nodesFile << NNodes_ << endl;
    nodesFile << "Nnode ID     x      y" << endl;

    for (unsigned int i = 0; i < NNodes_; i++){
        nodesFile << i << " " << Nodes_x_[i] << " " << Nodes_y_[i] << endl;
    }
    nodesFile.close();

    string cellNodesFileName = "./bin/cell_nodes.txt";
    ofstream cellNodesFile;
    cellNodesFile.open(cellNodesFileName);
    cellNodesFile << NCellsTotal_ << endl;
    cellNodesFile << "Cell ID     nNodes      nodes" << endl;

    for (unsigned int i = 0; i < NCellsTotal_; i++){
        cellNodesFile << i << " " << CellNfaces_[i];

        for (unsigned int j = 0; j < CellNfaces_[i]; j++){
            cellNodesFile << " " << Cell2Node_[i][j];
        }
        cellNodesFile << endl;
    }
    cellNodesFile.close();

    string cellFacesFileName = "./bin/cell_faces.txt";
    ofstream cellFacesFile;
    cellFacesFile.open(cellFacesFileName);
    cellFacesFile << NCellsTotal_ << endl;
    cellFacesFile << "Cell ID     nNodes      faces" << endl;

    for (unsigned int i = 0; i < NCellsTotal_; i++){
        cellFacesFile << i << " " << CellNfaces_[i];

        for (unsigned int j = 0; j < CellNfaces_[i]; j++){
            cellFacesFile << " " << Cell2Face_[i][j];
        }
        cellFacesFile << endl;
    }
    cellFacesFile.close();

    string cellCellsFileName = "./bin/cell_cells.txt";
    ofstream cellCellsFile;
    cellCellsFile.open(cellCellsFileName);
    cellCellsFile << NCellsTotal_ << endl;
    cellCellsFile << "Cell ID     nNodes      cells" << endl;

    for (unsigned int i = 0; i < NCellsTotal_; i++){
        cellCellsFile << i << " " << CellNfaces_[i];

        for (unsigned int j = 0; j < CellNfaces_[i]; j++){
            cellCellsFile << " " << Cell2Cell_[i][j];
        }
        cellCellsFile << endl;
    }
    cellCellsFile.close();

    string faceNodesFileName = "./bin/face_nodes.txt";
    ofstream faceNodesFile;
    faceNodesFile.open(faceNodesFileName);
    faceNodesFile << NFaces_ << endl;
    faceNodesFile << "Face ID     nodes" << endl;

    for (unsigned int i = 0; i < NFaces_; i++){
        faceNodesFile << i << " ";
        for (unsigned int j = 0; j < 2; j++){
            faceNodesFile << " " << Face2Node_[i][j];
        }
        faceNodesFile << endl;
    }
    faceNodesFile.close();

    string faceCellsFileName = "./bin/face_cells.txt";
    ofstream faceCellsFile;
    faceCellsFile.open(faceCellsFileName);
    faceCellsFile << NFaces_ << endl;
    faceCellsFile << "Face ID     cells" << endl;

    for (unsigned int i = 0; i < NFaces_; i++){
        faceCellsFile << i << " ";
        for (unsigned int j = 0; j < 2; j++){
            faceCellsFile << " " << Face2Cell_[i][j];
        }
        faceCellsFile << endl;
    }
    faceCellsFile.close();

    string ghostTypeFileName = "./bin/ghost_type.txt";
    ofstream ghostTypeFile;
    ghostTypeFile.open(ghostTypeFileName);
    ghostTypeFile << NCellsGhost_ << endl;
    ghostTypeFile << "Cell ID     type" << endl;

    for (unsigned int i = 0; i < NCellsGhost_; i++){
        ghostTypeFile << i + NCells_ << " ";
        ghostTypeFile << GhostType_[i];
        ghostTypeFile << endl;
    }
    ghostTypeFile.close();
}





