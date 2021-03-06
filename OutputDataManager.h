#ifndef DEF_OUTPUTDATAMANAGER_H
#define DEF_OUTPUTDATAMANAGER_H
//Standard lib.
#include <iostream>
#include <fstream>
#include <string>

//Projet files
#include "NSCData.h"
#include "Mesh_Data.h"


// This class allows to organize some particular data for future use.
// output file for tecplot using.
//
class OutputDataManager
{

    public:
        //Default Constructor
        OutputDataManager(NSCData *iNSCData, Mesh_Data *iMeshData);

        //Default Destructor
        virtual ~OutputDataManager();

        // This method allows to output a text file that will be input in TECPLOT to show the mesh
        void writeOutputTecplot();

        // Calculates rho_nodes, u_nodes, v_nodes, p_nodes
        void calculate_node_coefficients(); 

        // Outputs the content of meshData
        void writeMeshContent(); 

    private:
        NSCData     *nscData_;
        Mesh_Data   *meshData_;
};

#endif
