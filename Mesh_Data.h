#ifndef MESH_DATA_H_
#define MESH_DATA_H_

using namespace std;

#include <string>
#include <iostream>

class Mesh_Data{
    public:
    Mesh_Data();    //Constructor
    ~Mesh_Data();   //Desctructor

    void read_SU2(string filename); //Reading the mesh file + connectivity?
  
    public:
    //Input
    double* Nodes_x_;               //Done
    double* Nodes_y_;               //Done

    //Connectivity
    unsigned int** Cell2Node_;               //Done
    unsigned int** Cell2Face_;
    unsigned int** Face2Node_;
    unsigned int** Face2Cell_;
    unsigned int** Cell2Cell_;
    unsigned int** Node2Cell_;

    //Helper
    unsigned int* CellNfaces_;               //Done
    unsigned int NCells_;           //Done
    unsigned int NCellsGhost_;      //lui
    unsigned int NCellsTotal_;      // ?
    unsigned int NFaces_;           //Done
    unsigned int NFacesGhost_;      //Done // should not exist I think
    unsigned int NFacesTotal_;      //Done // should not exist I think
    unsigned int NNodes_;           //Done
    double* Volume_;
    double* Residu_;

    //Output
    double* rho_;
    double* u_;
    double* v_;
    double* p_;
};

#endif
