
#include "Mesh_Data.h"
#include <iostream>
#include <fstream>
#include "arrayMemory.h"
using namespace std;

/***********************************CONSTRUCTOR BY DEFAULT****************************************/
Mesh_Data::Mesh_Data(): 
Nodes_x_(nullptr), Nodes_y_(nullptr), 
Cell2Node_(nullptr), Cell2Face_(nullptr), Face2Node_(nullptr), Face2Cell_(nullptr), Cell2Cell_(nullptr), Node2Cell_(nullptr),
CellNfaces_(nullptr), NCells_(0), NCellsGhost_(0), NFaces_(0), NFacesGhost_(0), NNodes_(0), Volume_(nullptr), Residu_(nullptr),
rho_(nullptr), u_(nullptr), v_(nullptr), p_(nullptr){}


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





