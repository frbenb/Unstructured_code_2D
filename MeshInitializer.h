#ifndef DEF_MESHINITIALIZER_H
#define DEF_MESHINITIALIZER_H

//Standard lib.
#include <string>
#include <math.h>

//Projet files
#include "NSCData.h"
#include "Mesh_Data.h"

// This class allows to
// Takes the roles of the following functions:
// topo4grids()
// mesh4grids()
// mesh4halos()
// metric()

class MeshInitializer
{

    public:

        //Default Constructor
        MeshInitializer(NSCData *iNSCData, Mesh_Data *iMeshData);

        //Default Destructor
        virtual ~MeshInitializer();

        void initializeMesh(string& meshFilename);

        //This method allows to define some properties beonging to faces and 
        //cells (normals direction, areas)
        void metric();

        void mesh4halos(); // Make private

        void calculateCellCenter(); // Make private

        void calculateFaceCenter(); // Make private

        void calculateNormal(); // Make private

        void calculateCellsArea(); // Make private

    private:

        NSCData     *nscData_;
        Mesh_Data   *meshData_;

        void deallocateMesh();
        unsigned int* prepass(string& meshFilename);
        unsigned int read_su2(string meshFilename, unsigned int npoints, unsigned int ncells, unsigned int nghosts);
        void fill_face_arrays();
        void fill_cell2cell();
        void fill_node2cell();
};


#endif
