#ifndef MESH_DATA_H_
#define MESH_DATA_H_

using namespace std;

#include <string>
#include <iostream>

class Mesh_Data{
    public:
    Mesh_Data();    //Constructor
    ~Mesh_Data();   //Desctructor

    public:
    //Input
    double* Nodes_x_;               
    double* Nodes_y_;               

    //Connectivity
    unsigned int** Cell2Node_;               
    unsigned int** Cell2Face_;
    unsigned int** Face2Node_;
    unsigned int** Face2Cell_;
    unsigned int** Cell2Cell_;
    unsigned int** Node2Cell_;

    //Helper
    unsigned int* CellNfaces_;     
    unsigned int NCells_;        
    unsigned int NCellsGhost_;     
    unsigned int NCellsTotal_;      
    unsigned int NFaces_;           
    unsigned int NFacesGhost_;      // should not exist I think
    unsigned int NFacesTotal_;      // should not exist I think
    unsigned int NNodes_;           
    double* Volume_;
    double* Residu_;

    //Output
    double* rho_;
    double* u_;
    double* v_;
    double* p_;
};

#endif
