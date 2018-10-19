#include "MeshInitializer.h"
#include <iostream>
#include <fstream>
#include "arrayMemory.h"


MeshInitializer::MeshInitializer(NSCData *iNSCData, Mesh_Data *iMeshData) :

                                    nscData_(iNSCData),
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
    unsigned int ncells = 16384;
    unsigned int nghosts = 128 + 128; // far field and airfoil
    unsigned int ncellstot = ncells + nghosts;
    //calculate nfaces to allocate these vectors. 

    meshData_->Nodes_x_ = allocate1Ddbl(npoints);
    meshData_->Nodes_y_ = allocate1Ddbl(npoints);

    meshData_->Cell2Node_ = allocate1Dintstar(ncellstot);
    meshData_->CellNfaces_ = allocate1Dint(ncellstot);

    // These are filled later
    meshData_->Cell2Face_ = allocate1Dintstar(ncellstot);
    meshData_->Cell2Cell_ = allocate1Dintstar(ncellstot);
    meshData_->Node2Cell_ = allocate1Dintstar(npoints); // 2nd level not allocated yet

    meshData_->Volume_ = allocate1Ddbl(ncellstot);
    meshData_->Residu_ = allocate1Ddbl(ncellstot);

    meshData_->rho_ = allocate1Ddbl(ncellstot);
    meshData_->u_ = allocate1Ddbl(ncellstot);
    meshData_->v_ = allocate1Ddbl(ncellstot);
    meshData_->p_ = allocate1Ddbl(ncellstot);
    
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
    for (unsigned int i=0; i<meshData_->NNodes_; i++)
    {
        meshfile >> meshData_->Nodes_x_[i] >> meshData_->Nodes_y_[i];
    }

    //To get the number of cells of the mesh
    meshfile >> token >> meshData_->NCells_; //Here, token = "NELEM="
    if (meshData_->NCells_ != ncells){
        cout << "Pre-pass and full read nCells differ." << endl;
        return;
    }


    //Array of the number of faces for each cell (CellNFaces_)
    //and array of nodes for each cell (Cell2Nodes_)
    unsigned int shape; //Type of shape for each cell
    unsigned int nFaces_double = 0; // Is not the number of faces, because will count all faces twice.

    for (unsigned int i=0; i<meshData_->NCells_; i++)
    {
        meshfile >> shape;

        switch (shape)
        {
            //Quadrilaterals
            case 9:
                meshData_->CellNfaces_[i] = 4; //4 faces for a quadrilateral
                break;

            //Triangles
            case 5:
                meshData_->CellNfaces_[i] = 3; //3 faces for a triangle
                break;
            
            //Lines
            case 3:
                meshData_->CellNfaces_[i] = 2; //2 faces for a line
                break;
        }

        meshData_->Cell2Node_[i] = allocate1Dint(meshData_->CellNfaces_[i]);
        meshData_->Cell2Face_[i] = allocate1Dint(meshData_->CellNfaces_[i]);
        meshData_->Cell2Cell_[i] = allocate1Dint(meshData_->CellNfaces_[i]);
        for (unsigned int j = 0; j < meshData_->CellNfaces_[i]; j++){
            meshfile >> meshData_->Cell2Node_[i][j];
        }

        nFaces_double += meshData_->CellNfaces_[i]; //Counting the number of faces (here, faces will be count 2 times, see further away in the code)
    }
    
    //To read all the boundary conditions, we need a "for" with the number of boundary conditions
    unsigned int nboundarytypes;
    meshfile >> token >> nboundarytypes; //Here, token = "NMARK="

    string boundarytype;
    unsigned int nelements;
    unsigned int boundary_counter = 0;


    for (unsigned int i=0; i<nboundarytypes; i++)
    {
        meshfile >> token >> boundarytype; //Here, token = "MARKER_TAG="
        cout << "Boundary condition " << i+1 << ": " << boundarytype << endl;

        meshfile >> token >> nelements; //Here, token = "MARKER_ELEMS="
        for (unsigned int j=0; j<nelements; j++)
        {
            meshfile >> shape;
            if (shape != 3){
                cout << "Boundaries can only be line elements." << endl;
                return;
            }
            
            meshData_->Cell2Face_[meshData_->NCells_ + boundary_counter] = allocate1Dint(2);
            meshData_->Cell2Cell_[meshData_->NCells_ + boundary_counter] = allocate1Dint(2);

            meshData_->CellNfaces_[meshData_->NCells_ + boundary_counter] = 2;
            meshData_->Cell2Node_[meshData_->NCells_ + boundary_counter] = allocate1Dint(2);
            meshfile >> meshData_->Cell2Node_[meshData_->NCells_ + boundary_counter][0] >> meshData_->Cell2Node_[meshData_->NCells_ + boundary_counter][1];
            
            boundary_counter++;
        }

        //For every bc, adding the number of elem. to the number of ghost faces (every element is a line in 2D)
        meshData_->NCellsGhost_ += nelements; 
    }

    meshData_->NCellsTotal_ = meshData_->NCellsGhost_ + meshData_->NCells_;

    if (meshData_->NCellsGhost_ != nghosts){
        cout << "Pre-pass and full read nCellsGhost differ." << endl;
        return;
    }

    //Counting the thotal number of faces
    meshData_->NFaces_ = (nFaces_double + meshData_->NCellsGhost_)/2;

    //Closing the mesh file
    meshfile.close();

    meshData_->Face2Node_ = allocate2Dint(meshData_->NFaces_, 2);
    meshData_->Face2Cell_ = allocate2Dint(meshData_->NFaces_, 2);

    unsigned int nFacesDone = 0;
    unsigned int node1, node2, min, max, found;

    for (unsigned int i = 0; i < meshData_->NCellsTotal_; i++){
        for (unsigned int j = 0; j < meshData_->CellNfaces_[i]; j++){
            found = -1;
            node1 = meshData_->Cell2Node_[i][j];
            if (j+1 >= meshData_->CellNfaces_[i]){
                node2 = meshData_->Cell2Node_[i][0];
            }
            else{
                node2 = meshData_->Cell2Node_[i][j+1];
            } 

            min = node1 * (node1 <= node2) + node2 * (node2 < node1);
            max = node1 * (node1 >= node2) + node2 * (node2 > node1);

            for (unsigned int k = 0; k < nFacesDone; k++){
                if ((meshData_->Face2Node_[k][0] == min) && (meshData_->Face2Node_[k][1] == max)){
                    found = k;
                    break;
                }
            }

            if (found == -1){
                meshData_->Face2Node_[nFacesDone][0] = min;
                meshData_->Face2Node_[nFacesDone][1] = max;
                found = nFacesDone;
                nFacesDone++;  
            }

            meshData_->Cell2Face_[i][j] = found;
        }
    }

    // Face2Cell_
    unsigned int ncellsdone, face1, face2;
    unsigned int facedone, facedonek;
    for (unsigned int i = 0; i < meshData_->NFaces_ ; i++){
        ncellsdone = 0;
        facedone = -1;
        facedonek = -1;
        for (unsigned int j = 0; j < meshData_->NCellsTotal_; j++){
            ncellsdone++;
            for (unsigned int k = 0; k < meshData_->CellNfaces_[j]; k++){
                if (meshData_->Cell2Face_[j][k] == i){
                    facedone = j;
                    facedonek = k;
                    break;
                }  
            }
            if (facedone != -1){
                break;
            }
        }

        if (facedone == -1){
            cout << "Face " << i << " cell 1 not found."  << endl;
            return;
        }

        face1 = facedone;

        facedone = -1;
        for (unsigned int j = ncellsdone; j < meshData_->NCellsTotal_; j++){
            for (unsigned int k = 0; k < meshData_->CellNfaces_[j]; k++){
                if (meshData_->Cell2Face_[j][k] == i){
                    facedone = j;
                    break;
                }  
            }
            if (facedone != -1){
                break;
            }
        }

        if (facedone == -1){
            if (meshData_->Cell2Face_[face1][1 + -facedonek] == i){
                facedone = face1;
            }
            else{
                cout << "Face " << i << " cell 2 not found." << endl;
            return;
            }
        }

        face2 = facedone;

        bool bigger = face1 <= face2;

        // Check here for order.
        min = face1 * bigger + face2 * !bigger;
        max = face1 * !bigger + face2 * bigger;
    
        meshData_->Face2Cell_[i][0] = min;
        meshData_->Face2Cell_[i][1] = max;
    }

    // Cell2Cell_

    for (unsigned int i = 0; i < meshData_->NCellsTotal_; i++){  //The cell we are checking
        for (unsigned int j = 0; j < meshData_->CellNfaces_[i]; j++){   //The cell's faces we are chking, we want the cell on the other side

            if (meshData_->Face2Cell_[i][0] != meshData_->Cell2Face_[i][j]){ 
                meshData_->Cell2Cell_[i][j] = meshData_->Face2Cell_[i][0];
               
             } 
             else { 
                meshData_->Cell2Cell_[i][j] = meshData_->Face2Cell_[i][1];
             } 
        }
    }




    /*
    TO DO :
        - Node2Cell_
    */


}

void MeshInitializer::deallocateMesh(){
    meshData_->Nodes_x_ = deallocate1Ddbl(meshData_->Nodes_x_);
    meshData_->Nodes_y_ = deallocate1Ddbl(meshData_->Nodes_y_);
    meshData_->CellNfaces_ = deallocate1Dint(meshData_->CellNfaces_);
    meshData_->Volume_ = deallocate1Ddbl(meshData_->Volume_);
    meshData_->Residu_ = deallocate1Ddbl(meshData_->Residu_);
    meshData_->rho_ = deallocate1Ddbl(meshData_->rho_);
    meshData_->u_ = deallocate1Ddbl(meshData_->u_);
    meshData_->v_ = deallocate1Ddbl(meshData_->v_);
    meshData_->p_ = deallocate1Ddbl(meshData_->p_);

    meshData_->Cell2Node_ = deallocate2Dint(meshData_->Cell2Node_, meshData_->NCellsTotal_);
    meshData_->Cell2Face_ = deallocate2Dint(meshData_->Cell2Face_, meshData_->NCellsTotal_);
    meshData_->Face2Node_ = deallocate2Dint(meshData_->Face2Node_, meshData_->NFaces_);
    meshData_->Face2Cell_ = deallocate2Dint(meshData_->Face2Cell_, meshData_->NFaces_);
    meshData_->Cell2Cell_ = deallocate2Dint(meshData_->Cell2Cell_, meshData_->NCellsTotal_);
    meshData_->Node2Cell_ = deallocate2Dint(meshData_->Node2Cell_, meshData_->NNodes_);

    meshData_->NCells_ = 0;
    meshData_->NCellsGhost_ = 0;
    meshData_->NCellsTotal_ = 0;
    meshData_->NFaces_ = 0;
    meshData_->NFacesGhost_ = 0;
    meshData_->NFacesTotal_ = 0;
    meshData_->NNodes_ = 0; 
}

