#include "MeshInitializer.h"
#include <string>
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
    unsigned int ncells = 16384;
    unsigned int nghosts = 128 + 128; // far field and airfoil
    unsigned int nfaces = 100000; // calculate this

    meshData_->Nodes_x_ = allocate1D<double>(npoints);
    meshData_->Nodes_y_ = allocate1D<double>(npoints);


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

