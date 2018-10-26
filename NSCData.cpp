#include "NSCData.h"


NSCData::NSCData():
    ctrlfilename_(""),
    meshfilename_(""),
    title_(""),
    topo_(0),
    gamma_(0),
    alpha_(0),
    mach_(0),
    reynolds_(0),
    tinf_(0),
    cltarget_(0),
    dcl_(0),
	xref_(0),
    yref_(0),
    pi_(0),
    epsilon_(0),
    rhoInfini_(0),
    uInfini_(0),
    vInfini_(0),
    vis2_(0),
    vis4_(0),
    ressmo_(0),
    nitc_(0),
    niter_(0),
    itccfl_(0), 
    cfl_(0),
    dissip_(0),
    itertot_(0),
    rk_alfa_(nullptr),
    rk_beta_(nullptr),
    rk_(0),
    nstage_(0),
    nbiter_(0),
    rms0_(0)
{  
}

NSCData::~NSCData()
{
    //TBD.
}










