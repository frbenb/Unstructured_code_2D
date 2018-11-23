
#include "Mesh_Data.h"
#include <iostream>
#include <fstream>
#include "arrayMemory.h"
using namespace std;

Mesh_Data::Mesh_Data(): 
Nodes_x_(nullptr), Nodes_y_(nullptr), 
Cell2Node_(nullptr), Cell2Face_(nullptr), Face2Node_(nullptr), Face2Cell_(nullptr), Cell2Cell_(nullptr), Node2Cell_(nullptr),
CellNfaces_(nullptr), NCells_(0), NCellsGhost_(0), NFaces_(0), NNodes_(0), nodeNCell_(nullptr), Volume_(nullptr), Residu_(nullptr), GhostType_(nullptr),
rho_(nullptr), u_(nullptr), v_(nullptr), p_(nullptr),
rho_nodes_(nullptr), u_nodes_(nullptr), v_nodes_(nullptr), p_nodes_(nullptr),
convectiveFlux0Faces_(nullptr), convectiveFlux1Faces_(nullptr), convectiveFlux2Faces_(nullptr), convectiveFlux3Faces_(nullptr),
itl_(0), 
itu_(0),                                      
cellArea_(nullptr),         
normal_x_(nullptr), 		
normal_y_(nullptr),
FaceCenter_x_(nullptr),
FaceCenter_y_(nullptr),  
cellCenter_x_(nullptr),
cellCenter_y_(nullptr),         
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
{}

Mesh_Data::~Mesh_Data()
{
    Nodes_x_ = deallocate1Ddbl(Nodes_x_);
    Nodes_y_ = deallocate1Ddbl(Nodes_y_);
    CellNfaces_ = deallocate1Dint(CellNfaces_);
    nodeNCell_ = deallocate1Dint(nodeNCell_);
    Volume_ = deallocate1Ddbl(Volume_);
    Residu_ = deallocate1Ddbl(Residu_);
    GhostType_ = deallocate1Dint(GhostType_);
    rho_ = deallocate1Ddbl(rho_);
    u_ = deallocate1Ddbl(u_);
    v_ = deallocate1Ddbl(v_);
    p_ = deallocate1Ddbl(p_);
    rho_nodes_ = deallocate1Ddbl(rho_nodes_);
    u_nodes_ = deallocate1Ddbl(u_nodes_);
    v_nodes_ = deallocate1Ddbl(v_nodes_);
    p_nodes_ = deallocate1Ddbl(p_nodes_);
    convectiveFlux0Faces_ = deallocate1Ddbl(convectiveFlux0Faces_);
    convectiveFlux1Faces_ = deallocate1Ddbl(convectiveFlux1Faces_);
    convectiveFlux2Faces_ = deallocate1Ddbl(convectiveFlux2Faces_);
    convectiveFlux3Faces_ = deallocate1Ddbl(convectiveFlux3Faces_);
    cellArea_ = deallocate1Ddbl(cellArea_);
    normal_x_ = deallocate1Ddbl(normal_x_);
    normal_y_ = deallocate1Ddbl(normal_y_);
    cellCenter_x_ = deallocate1Ddbl(cellCenter_x_); 
    cellCenter_y_ = deallocate1Ddbl(cellCenter_y_); 

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

    deltaT_ = deallocate1Ddbl(deltaT_);

    rho_0_ = deallocate1Ddbl(rho_0_);
    u_0_ = deallocate1Ddbl(u_0_);
    v_0_ = deallocate1Ddbl(v_0_);
    p_0_ = deallocate1Ddbl(p_0_);

}