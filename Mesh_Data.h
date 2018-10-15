#ifndef MESH_DATA_H_
#define MESH_DATA_H_

using namespace std;

#include <string>
#include <iostream>

class Mesh_Data{
    public:
    Mesh_Data();    //Constructor
    Mesh_Data(unsigned int ncells, unsigned int nghost, unsigned int nfaces, unsigned int nnodes);
    ~Mesh_Data();   //Desctructor

    void read_SU2(string filename); //Reading the mesh file + connectivity?

    //Input
    double* Nodes_x_;               //Done
    double* Nodes_y_;               //Done

    //Connectivity
    int** Cell2Node_;               //Done
    int** Cell2Face_;
    int** Face2Node_;
    int** Face2Cell_;
    int** Cell2Cell_;
    int** Node2Cell_;

    //Helper
    int* CellNfaces_;               //Done
    unsigned int NCells_;           //Done
    unsigned int NCellsGhost_;      //lui
    unsigned int NFaces_;           //Done
    unsigned int NFacesGhost_;      //Done
    unsigned int NFacesTotal_;      //Done
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
