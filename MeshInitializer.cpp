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

    unsigned int* variables = new unsigned int[3]; // no other idea

    prepass(meshFilename, variables);

    // pre-pass should give the following results
    unsigned int npoints = variables[0];
    unsigned int ncells = variables[1];
    unsigned int nghosts = variables[2]; // far field and airfoil
    unsigned int ncellstot = ncells + nghosts;
    //calculate nfaces to allocate these vectors. 

    delete [] variables;

    // Nodes, Cells allocation
    meshData_->Nodes_x_ = allocate1Ddbl(npoints);
    meshData_->Nodes_y_ = allocate1Ddbl(npoints);

    meshData_->Cell2Node_ = allocate1Dintstar(ncellstot);
    meshData_->CellNfaces_ = allocate1Dint(ncellstot);
    meshData_->GhostType_ = allocate1Dint(nghosts);

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

    //Closing the mesh file
    meshfile.close();

    meshData_->NCellsTotal_ = meshData_->NCellsGhost_ + meshData_->NCells_;

    if (meshData_->NCellsGhost_ != nghosts){
        cout << "Pre-pass and full read nCellsGhost differ." << endl;
        return;
    }

    //Counting the thotal number of faces
    meshData_->NFaces_ = (nFaces_double + meshData_->NCellsGhost_)/2;


    //Initialize memory for normal of faces
    meshData_->normal_x_ = allocate1Ddbl(meshData_->NFaces_);
    meshData_->normal_y_ = allocate1Ddbl(meshData_->NFaces_);

    //Initialize memory for CenterFaces coordinates vector
    meshData_->FaceCenter_x_ = allocate1Ddbl(meshData_->NFaces_);
    meshData_->FaceCenter_y_ = allocate1Ddbl(meshData_->NFaces_);

    //Closing the mesh file
    meshfile.close();


    // NFaces allocation

    meshData_->Face2Node_ = allocate2Dint(meshData_->NFaces_, 2);
    meshData_->Face2Cell_ = allocate2Dint(meshData_->NFaces_, 2);

    unsigned int nFacesDone = 0;
    unsigned int node1, node2, min, max, found;

    for (unsigned int i = 0; i < meshData_->NCellsTotal_; i++){
        for (unsigned int j = 0; j < meshData_->CellNfaces_[i]; j++){
            found = meshData_->NFaces_ + 1; // Just an impossible value. was -1 and generated warnings.
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

            if (found == meshData_->NFaces_ + 1){
                meshData_->Face2Node_[nFacesDone][0] = min;
                meshData_->Face2Node_[nFacesDone][1] = max;
                found = nFacesDone;
                meshData_->Face2Cell_[nFacesDone][0] = i;
                nFacesDone++;  
            }
            else{
                meshData_->Face2Cell_[found][1] = i;
            }

            meshData_->Cell2Face_[i][j] = found;
        }
    }

    // Cell2Cell_ 

    for (unsigned int i = 0; i < meshData_->NCellsTotal_; i++){  //The cell we are checking
        for (unsigned int j = 0; j < meshData_->CellNfaces_[i]; j++){   //The cell's faces we are chking, we want the cell on the other side

            unsigned int cell0 = meshData_->Face2Cell_[meshData_->Cell2Face_[i][j]][0];
            unsigned int cell1 = meshData_->Face2Cell_[meshData_->Cell2Face_[i][j]][1]; 

            bool factor = cell1 == i;

            meshData_->Cell2Cell_[i][j] = factor * cell0 + !factor * cell1; 
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
    meshData_->GhostType_ = deallocate1Dint(meshData_->GhostType_);
    meshData_->rho_ = deallocate1Ddbl(meshData_->rho_);
    meshData_->u_ = deallocate1Ddbl(meshData_->u_);
    meshData_->v_ = deallocate1Ddbl(meshData_->v_);
    meshData_->p_ = deallocate1Ddbl(meshData_->p_);
    meshData_->normal_x_ = deallocate1Ddbl(meshData_->normal_x_);
    meshData_->normal_y_ = deallocate1Ddbl(meshData_->normal_y_);
    meshData_->FaceCenter_x_ = deallocate1Ddbl(meshData_->FaceCenter_x_);
    meshData_->FaceCenter_y_ = deallocate1Ddbl(meshData_->FaceCenter_y_);

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
    meshData_->NNodes_ = 0; 
}


void MeshInitializer::metric()
{
    // Calculate Cell center.

    // Calculate face center.
    calculateFaceCenter();

    // Calculate face area.

    //Calculate normal on faces.
    calculateNormal();
}

void MeshInitializer::mesh4halos()
{
    
}


void MeshInitializer::calculateCellCenter()
{

}

void MeshInitializer::calculateFaceCenter()
{

    int unsigned nbFaces = 0;
    unsigned int nodeID[2];

    double node1_x = 0;
    double node1_y = 0;

    double node2_x = 0;
    double node2_y = 0;

    double node_at_center_coord[2]; // Index 0 stands for x value and 1 for y value.
  
    for(unsigned int i(0); i < meshData_->NFaces_;i++)
    {
       
        nodeID[0] = meshData_->Face2Node_[i][0];
        nodeID[1] = meshData_->Face2Node_[i][1];

        //Get coordinates x and y of each node
        node1_x = meshData_->Nodes_x_[nodeID[0]];
        node1_y = meshData_->Nodes_y_[nodeID[0]];
        node2_x = meshData_->Nodes_x_[nodeID[1]];
        node2_y = meshData_->Nodes_y_[nodeID[1]];

        //Calculate average on y coordinates
        meshData_->FaceCenter_x_[i] = (node1_x + node2_x)/2;
        meshData_->FaceCenter_y_[i] = (node1_y + node2_y)/2;

    }

}

void MeshInitializer::calculateNormal()
{
    unsigned int nodeID[2];

    double node1_x; // Contains values of x for nodeID[0]
    double node1_y; // Contains values of x for nodeID[0]

    double node2_x;// Contains value of x for nodeID[1]
    double node2_y;// Contains value of x for nodeID[1]

    double vector_nodes[2];
    double vector_normal[2];
    double vector_center_cells[2];

    unsigned int leftCellID;
    unsigned int rightCellID;

    double leftCellCoord[2]; // Coord x and y
    double rightCellCoord[2]; // Coord x and y

    double dot_product = 0;
    int sign_orientation = 0;

    double normal_vector_length = 0;

    for(int i(0);i < meshData_->NFaces_;i++)
    {
        //1.2 Get the 2 nodes ID in 1 face.
        nodeID[0] = meshData_->Face2Node_[i][0];
        nodeID[1] = meshData_->Face2Node_[i][1];

        //1.3 Get coordinateds x and y for each nodeID
        node1_x = meshData_->Nodes_x_[nodeID[0]];
        node1_y = meshData_->Nodes_x_[nodeID[0]];
        node2_x = meshData_->Nodes_y_[nodeID[1]];
        node2_y = meshData_->Nodes_y_[nodeID[1]];

        //1.4 Get vector connecting the two nodes.
        vector_nodes[0] = node2_x - node1_x;
        vector_nodes[1] = node2_y - node1_y;

        //1.5 Get a normal vector of vector_nodes
        vector_normal[0] = vector_nodes[1];
        vector_normal[1] = (-1)*vector_nodes[0];

        //1.6 Find coordinates of center of cells on left and right
        leftCellID = meshData_->Face2Cell_[i][0];
        rightCellID = meshData_->Face2Cell_[i][1];

        //Get Coordinates of cells at centers (To do here) with ID defined previously
        rightCellCoord[0] = 0; // Coord. x of right cell
        rightCellCoord[1] = 0; // Coord. y of right cell
        leftCellCoord[0] = 0; // Coord. x
        leftCellCoord[1] = 0; // Coord. y

        //Calculate vector between centers of cells right + left
        vector_center_cells[0] = rightCellCoord[0] - leftCellCoord[0];
        vector_center_cells[1] = rightCellCoord[1] - leftCellCoord[1];

        //Dot product between Normal and center-vector to get sign
        dot_product = vector_center_cells[0]*vector_normal[0] + vector_center_cells[1]*vector_normal[1];
        sign_orientation = dot_product / std::abs(dot_product); // Get -1 or 1.

        //Final normal Result of the face[j] in  cell[i] and final mapping.
        normal_vector_length = std::sqrt(vector_normal[0]*vector_normal[0] + vector_normal[1]*vector_normal[1]);
        meshData_->normal_x_[i] = (sign_orientation * vector_normal[0]) / normal_vector_length;
        meshData_->normal_y_[i] = (sign_orientation * vector_normal[1]) / normal_vector_length;
    }
}

void MeshInitializer::calculateCellsArea()
{

}


void MeshInitializer::prepass(string& meshFilename, unsigned int* variables){
    //unsigned int variables[3]; // 0 is npoints, 1 is ncells, 2 is nghost

    unsigned int npoints;
    unsigned int ncells;
    unsigned int nghosts = 0; 

    double dbldummy0, dbldummy1;
    unsigned int intdummy;
    
    string token;

    /*unsigned int npoints = 16512;
    unsigned int ncells = 16384;
    unsigned int nghosts = 128 + 128; */

    ifstream meshfile;
    meshfile.open(meshFilename);

    //Checking if the file is open or not
    if(!meshfile.is_open())
    {
        cout << "Error: " << meshFilename << " could not open for pre-pass." << endl;
        return;
    }
    else
    {
        cout << meshFilename << " is open for pre-pass." << endl;
    }

    unsigned int ndim;
    unsigned int shape;
    unsigned int cellnshape;
    meshfile >> token >> ndim;

    meshfile >> token >> npoints;

    for (unsigned int i = 0; i < npoints; i++){
        meshfile >> dbldummy0 >> dbldummy1;
    }

    meshfile >> token >> ncells;

    for (unsigned int i=0; i < ncells; i++)
    {
        meshfile >> shape;
        cellnshape = 0;

        switch (shape)
        {
            //Quadrilaterals
            case 9:
                cellnshape = 4; //4 faces for a quadrilateral
                break;

            //Triangles
            case 5:
                cellnshape = 3; //3 faces for a triangle
                break;
            
            //Lines
            case 3:
                cellnshape = 2; //2 faces for a line
                break;
        }

        for (unsigned int j = 0; j < cellnshape; j++){
            meshfile >> intdummy;
        }
    }

    unsigned int nboundarytypes;
    meshfile >> token >> nboundarytypes; //Here, token = "NMARK="

    unsigned int nelements;

    for (unsigned int i=0; i<nboundarytypes; i++)
    {
        meshfile >> token; //Here, token = "MARKER_TAG="
        meshfile >> token; // boundary type

        meshfile >> token >> nelements; //Here, token = "MARKER_ELEMS="
        for (unsigned int j=0; j<nelements; j++)
        {
            meshfile >> shape;
            if (shape != 3){
                cout << "Boundaries can only be line elements." << endl;
                return;
            }

            meshfile >> intdummy;
            meshfile >> intdummy;
        }

        //For every bc, adding the number of elem. to the number of ghost faces (every element is a line in 2D)
        nghosts += nelements; 
    }

    meshfile.close();

    variables[0] = npoints;
    variables[1] = ncells;
    variables[2] = nghosts;
    return;
}


