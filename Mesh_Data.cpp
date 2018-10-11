
#include "Mesh_Data.h"
#include <string>
#include <iostream>
#include <fstream>
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
    //Destruction of Nodes_x_
    if (Nodes_x_ != nullptr)
    {
        delete[] Nodes_x_;
        Nodes_x_ = nullptr;
    }

    //Destruction of Nodes_y_
    if (Nodes_y_ != nullptr)
    {
        delete[] Nodes_y_;
        Nodes_y_ = nullptr;
    }

    //Destruction of CellNFaces_
    if (CellNfaces_!= nullptr)
    {
        delete[] CellNfaces_;
        CellNfaces_ = nullptr;
    }

    //Destruction of Cell2Node_
    if (Cell2Node_ != nullptr)
    {
        for (unsigned int i=0; i<NCells_; i++)
        {
            delete[] Cell2Node_[i];
        }
        Cell2Node_ = nullptr;
    }

    //Destruction of Cell2Face_
    
    //Destruction of Face2Node_
    
    //Destruction of Face2Cell_

    //Destruction of Cell2Cell_

    //Destruction of Node2Cell_
}



/*****************************************READ MESH**********************************************/
void Mesh_Data::read_SU2(string filename){

    //Making sure that the array Nodes_x_ is empty
    if (Nodes_x_ != nullptr)
    {
        for (unsigned int i=0; i<NNodes_; i++)
        {
            delete[] Nodes_x_[i];
        }
        Nodes_x_ = nullptr;
        NNodes_ = 0;
    }

    //Making sure that the array Nodes_y_ is empty
    if (Nodes_y_ != nullptr)
    {
        for (unsigned int i=0; i<NNodes_; i++)
        {
            delete[] Nodes_y_[i];
        }
        Nodes_y_ = nullptr;
        NNodes_ = 0;
    }

    //Making sure that the array CellNFaces_ is empty
    if (CellNfaces_!= nullptr)
    {
        for (unsigned int i=0; i<NCells_; i++)
        {
            delete[] CellNfaces_[i];
        }
        CellNfaces_ = nullptr;
        NCells_ = 0;
        NFaces_ = 0;
    }

    //Making sure that the DOUBLE array Cell2Node_ is empty
    if (Cell2Node_ != nullptr)
    {
        for (unsigned int i=0; i<NCells_; i++)
        {
            delete[] Cell2Node_[i];
        }
        Cell2Node_ = nullptr;
    }
    
    //Display of the file name
    cout << "File name: " << filename << endl;

    //Opening the meshfile (in read mode only)
    ifstream meshfile;
    meshfile.open(filename);

    //Checking if the file is open or not
    if(!meshfile.is_open())
    {
        cout << "Error: " << filename << " could not open." << endl;
        return;
    }
    else
    {
        cout << filename << " is open." << endl;
    }

    string token; //This token will be used to get different information from the SU2

    //To get the dimension of the mesh
    unsigned int ndim;
    meshfile >> token >> ndim; //Here, token = "NDIME="

    //To get the number of nodes of the mesh
    meshfile >> token >> NNodes_; //Here, token = "NPOIN="

    Nodes_x_ = new double[NNodes_];
    Nodes_y_ = new double[NNodes_];

    //To fill the vectors of x and y coordinates of the nodes
    double x, y;
    for (unsigned int i=0; i<NNodes_; i++)
    {
        meshfile >> x >> y;
        Nodes_x_[i] = x;
        Nodes_y_[i] = y;
    }

    //To get the number of cells of the mesh
    meshfile >> token >> NCells_; //Here, token = "NELEM="

    CellNfaces_ = new int[NCells_];
    Cell2Node_ = new int*[NCells_]; //1st dimension of the array Cell2Node_

    //Array of the number of faces for each cell (CellNFaces_)
    //and array of nodes for each cell (Cell2Nodes_)
    unsigned int shape; //Type of shape for each cell
    unsigned int Node1, Node2, Node3, Node4;

    NFaces_ = 0;
    for (unsigned int i=0; i<NCells_; i++)
    {
        meshfile >> shape;

        switch (shape)
        {
            //Quadrilaterals
            case 9:
            CellNfaces_[i] = 4; //4 faces for a quadrilateral
            meshfile >> Node1 >> Node2 >> Node3 >> Node4; //4 nodes for a quadrilateral
            Cell2Node_[i] = new int(Node1, Node2, Node3, Node4); //2nd dim. of Cell2Node_ (with the 4 nodes)
            break;

            //Triangles
            case 5:
            CellNfaces_[i] = 3; //3 faces for a triangle
            meshfile >> Node1 >> Node2 >> Node3; //3 nodes for a line
            Cell2Node_[i] = new int(Node1, Node2, Node3); //2nd dim. of Cell2Node_ (with the 3 nodes)
            break;
            
            //Lines
            case 3:
            CellNfaces_[i] = 1; //1 face for a line
            meshfile >> Node1 >> Node2; //2 nodes for a line
            Cell2Node_[i] = new int(Node1, Node2); //2nd dim. of Cell2Node_ (with the 2 nodes)
            break;
        }

        NFaces_ += CellNfaces_[i]; //Counting the number of faces (here, faces will be count 2 times, see further away in the code)
    }
    
    //To read all the boundary conditions, we need a "for" with the number of boundary conditions
    unsigned int nboundarytypes;
    meshfile >> token >> nboundarytypes; //Here, token = "NMARK="

    string boundarytype;
    unsigned int nelements
    for (unsigned int i=0; i<nboundarytypes; i++)
    {
        meshfile >> token >> boundarytype; //Here, token = "MARKER_TAG="
        cout << "Boundary condition " << i+1 << ": " << boundarytype << endl;

        meshfile >> token >> nelements; //Here, token = "MARKER_ELEMS="
        for (unsigned int j=0; j<nelements; j++)
        {
            //TODO!!!
        }

        //For every bc, adding the number of elem. to the number of ghost faces (every element is a line in 2D)
        NFacesGhost_ += nelements; 
    }

    //Counting the thotal number of faces
    NFacesTotal_ = (NFaces_ + NFacesGhost_)/2;

    //Closing the mesh file
    meshfile.close();

    /*TODO:
        - Cell2Face_
        - Face2Node_
        - Face2Cell_
        - Cell2Cell_
        - Node2Cell_
    */
}

/*****************************************OUTPUT**************************************************/
/*{
    Calcul rho, u, v, p ici?
}*/






