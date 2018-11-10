#ifndef DEF_FLUXCOMPUTER_H
#define DEF_FLUXCOMPUTER_H

//Standard library
#include <iostream>
#include <math.h>

//Project files
#include <NSCData.h>
#include <Mesh_Data.h>


//This class calculates fluxes cell centered
class FluxComputer
{
    public:
        //Default Constructor
        FluxComputer();

        //Default Destructor
        ~FluxComputer();

        //This method calculates fluxes at each face
        void calculateConvectiveFluxes(); //eflux

        //This method stabilizes the fluxes calculation (based on Roe's Scheme)
        void calculateArtificialDissipRoe(); //dflux (first order)

       
        void roeScheme();

    private:
        NSCData     *nscData_;
        Mesh_Data   *meshData_;
    
};

#endif