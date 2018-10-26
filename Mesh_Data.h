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

    unsigned int itl_, itu_;      
    unsigned int inci_, incj_;                  //inci, incj - address  increments in i,j

    double* cellArea_;         //area
    double* normal_i_x_; 		//six, siy - face i projections
    double* normal_i_y_;
    double* normal_j_x_;       //sjx, sjy - face j projections
    double* normal_j_y_;
    // How to define the normal on the faces

    double* rho_nodes_;        //rocv, uucv, vvcv, ppcv - primitive variables cell-vertex
    double* u_nodes_;
    double* v_nodes_;
    double* p_nodes_;          // vertexes
    double* rho_0_;            //ro0, ru0, rv0, re0 - conservative variables rk(0)
    double* u_0_;
    double* v_0_;
    double* p_0_;

    double* residualInviscid_rho_; //Ri_ro, Ri_uu, Ri_vv, Ri_pp - inviscid Residual
    double* residualInviscid_u_;
    double* residualInviscid_v_;
    double* residualInviscid_p_;
    double* residualDissip_rho_;   
    double* residualDissip_u_;
    double* residualDissip_v_;
    double* residualDissip_p_;
    double* tmp_rho_;              //tmp_ro, tmp_uu, tmp_vv, tmp_pp - artificial Residual
    double* tmp_u_;
    double* tmp_v_;
    double* tmp_p_;
    double* deltaT_;               //dt
    double* speci_;                //speci, specj
    double* specj_;

};

#endif
