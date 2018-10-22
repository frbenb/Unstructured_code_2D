
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
CellNfaces_(nullptr), NCells_(0), NCellsGhost_(0), NFaces_(0), NFacesGhost_(0), NNodes_(0), Volume_(nullptr), Residu_(nullptr),
rho_(nullptr), u_(nullptr), v_(nullptr), p_(nullptr),
imax_(0),
jmax_(0), 
itl_(0), 
itu_(0),      
imaxGhost_(0), 
jmaxGhost_(0),        
rimax_(0),
rjmax_(0),                
inci_(0), 
incj_(0),                 
cellArea_(nullptr),         
normal_i_x_(nullptr), 		
normal_i_y_(nullptr),
normal_j_x_(nullptr),      
normal_j_y_(nullptr), 
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
speci_(nullptr),                 
specj_(nullptr)

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
    rho_ = deallocate1Ddbl(rho_);
    u_ = deallocate1Ddbl(u_);
    v_ = deallocate1Ddbl(v_);
    p_ = deallocate1Ddbl(p_);

    Cell2Node_ = deallocate2Dint(Cell2Node_, NCellsTotal_);
    Cell2Face_ = deallocate2Dint(Cell2Face_, NCellsTotal_);
    Face2Node_ = deallocate2Dint(Face2Node_, NFaces_);
    Face2Cell_ = deallocate2Dint(Face2Cell_, NFaces_);
    Cell2Cell_ = deallocate2Dint(Cell2Cell_, NCellsTotal_);
    Node2Cell_ = deallocate2Dint(Node2Cell_, NNodes_);

    NCells_ = 0;
    NCellsGhost_ = 0;
    NCellsTotal_ = 0;
    NFaces_ = 0;
    NFacesGhost_ = 0;
    NFacesTotal_ = 0;
    NNodes_ = 0;



}





