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

    private:

        NSCData     *nscData_;
        Mesh_Data   *meshData_;
};

#endif
