#include "MeshInitializer.h"
#include <iostream>
#include <fstream>
#include "arrayMemory.h"


MeshInitializer::MeshInitializer(NSCData *iNSCData, Mesh_Data *iMeshData) :
                                    _nscData(iNSCData),
                                    meshData_(iMeshData)
{

}

MeshInitializer::~MeshInitializer()
{
   
}

void MeshInitializer::initializeMesh(string& meshFilename){

    deallocateMesh();

    // pre-pass should give the following results
    unsigned int npoints = 16512;
    unsigned int ncellstot = 16384;
    unsigned int nghosts = 128 + 128; // far field and airfoil
    //calculate nfaces to allocate these vectors. 

    meshData_->Nodes_x_ = allocate1D<double>(npoints);
    meshData_->Nodes_y_ = allocate1D<double>(npoints);

    meshData_->Cell2Node_ = allocate1D<int*>(ncellstot);
    meshData_->Cell2Face_ = allocate1D<int*>(ncellstot);
    
    //Display of the file name
    cout << "File name: " << meshFilename << endl;

    //Opening the meshfile (in read mode only)
    ifstream meshfile;
    meshfile.open(meshFilename);

    //Checking if the file is open or not
    if(!meshfile.is_open())
    {
        cout << "Error: " << meshFilename << " could not open." << endl;
        return;
    }
    else
    {
        cout << meshFilename << " is open." << endl;
    }

    string token; //This token will be used to get different information from the SU2

    //To get the dimension of the mesh
    unsigned int ndim;
    meshfile >> token >> ndim; //Here, token = "NDIME="

    //To get the number of nodes of the mesh
    meshfile >> token >> meshData_->NNodes_; //Here, token = "NPOIN="

    if (meshData_->NNodes_ != npoints){
        cout << "Pre-pass and full read nNodes differ." << endl;
        return;
    }

    //To fill the vectors of x and y coordinates of the nodes
    for (unsigned int i=0; i<NNodes_; i++)
    {
        meshfile >> Nodes_x_[i] >> Nodes_y_[i];
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
                break;

            //Triangles
            case 5:
                CellNfaces_[i] = 3; //3 faces for a triangle
                break;
            
            //Lines
            case 3:
                CellNfaces_[i] = 2; //2 faces for a line
                break;
        }

        Cell2Node_[i] = new int[CellNfaces_[i]];
        for (unsigned int j = 0; j < CellNfaces_[i]; j++){
            meshfile >> Cell2Node_[i][j];
        }

        NFaces_ += CellNfaces_[i]; //Counting the number of faces (here, faces will be count 2 times, see further away in the code)
    }
    
    //To read all the boundary conditions, we need a "for" with the number of boundary conditions
    unsigned int nboundarytypes;
    meshfile >> token >> nboundarytypes; //Here, token = "NMARK="

    string boundarytype;
    unsigned int nelements;
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

void MeshInitializer::deallocateMesh(){
    meshData_->Nodes_x_ = deallocate1D(meshData_->Nodes_x_);
    meshData_->Nodes_y_ = deallocate1D(meshData_->Nodes_y_);
    meshData_->CellNfaces_ = deallocate1D(meshData_->CellNfaces_);
    meshData_->Volume_ = deallocate1D(meshData_->Volume_);
    meshData_->Residu_ = deallocate1D(meshData_->Residu_);
    meshData_->rho_ = deallocate1D(meshData_->rho_);
    meshData_->u_ = deallocate1D(meshData_->u_);
    meshData_->v_ = deallocate1D(meshData_->v_);
    meshData_->p_ = deallocate1D(meshData_->p_);

    meshData_->Cell2Node_ = deallocate2D(meshData_->Cell2Node_, meshData_->NCellsTotal_);
    meshData_->Cell2Face_ = deallocate2D(meshData_->Cell2Face_, meshData_->NCellsTotal_);
    meshData_->Face2Node_ = deallocate2D(meshData_->Face2Node_, meshData_->NFaces_);
    meshData_->Face2Cell_ = deallocate2D(meshData_->Face2Cell_, meshData_->NFaces_);
    meshData_->Cell2Cell_ = deallocate2D(meshData_->Cell2Cell_, meshData_->NCellsTotal_);
    meshData_->Node2Cell_ = deallocate2D(meshData_->Node2Cell_, meshData_->NNodes_);

    meshData_->NCells_ = 0;
    meshData_->NCellsGhost_ = 0;
    meshData_->NCellsTotal_ = 0;
    meshData_->NFaces_ = 0;
    meshData_->NFacesGhost_ = 0;
    meshData_->NFacesTotal_ = 0;
    meshData_->NNodes_ = 0;
}

