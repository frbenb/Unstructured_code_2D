#ifndef DEF_MESHINITIALIZER_H
#define DEF_MESHINITIALIZER_H

//Standard lib.
#include <string>

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

        void mesh4halos();


    private:

        NSCData     *nscData_;
        Mesh_Data   *meshData_;

        void deallocateMesh();
        unsigned int* prepass(string& meshFilename);

};


#endif
