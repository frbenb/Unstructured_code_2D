#ifndef DEF_FLUXCOMPUTER_H
#define DEF_FLUXCOMPUTER_H

//Standard library
#include <iostream>
#include <math.h>

//Project files
#include <Mesh_Data.h>


//This class calculates fluxes cell centered

class FluxComputer
{
    public:
    //Default Constructor
    FluxComputer();

    //Default Destructor
    ~FluxComputer();

    //This method calculates .......
    void eflux();

    //This method calculates .......
    void dflux();

    //This method calculates .......
    void roeScheme();

    private:
}




#endif