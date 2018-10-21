#ifndef DEF_NSCDATA_H
#define DEF_NSCDATA_H


//Standard lib.
#include <iostream>


// This class allows to store the data according to NSC structure presented in the
// NSCode. Is called by the solvers and initializer through multiple processes.
class NSCData
{

    public:
        //Default Constructor
        NSCData();

        //Default Destructor
        virtual ~NSCData();

        //Data.
        std::string ctrlfilename_;
        std::string meshfilename_;
        std::string title_;
        int topo_;

        double gamma_;
        double alpha_;
        double mach_;
        double reynolds_;
        double tinf_;

        /* flow & geometry properties */
	    double xref_;
        double yref_; //from "input file"
        double cmac_;

        double pi_;
        double epsilon_;

        double rhoInfini_;
        double uInfini_;
        double vInfini_;
        double pInfini_;

        double vis2_;
        double vis4_;
        double ressmo_;

        unsigned int nitc_;
        unsigned int niter_;
        double* itccfl_; //iterate timestep
        double cfl_;

        unsigned int dissip_;
        unsigned int itertot_;
        
        double* rk_alfa_;
        double* rk_beta_;
        
        /* constants from "input file" */
        unsigned int nbiter_;
    
        /*convergence*/
        double rms0_;


};


#endif




