/**
 * \file DrawSeed3D.h
 *
 * \ingroup RecoViewer
 *
 * \brief Class def header for a class DrawSeed3D
 *
 * @author cadams
 */

/** \addtogroup RecoViewer

    @{*/
#ifndef EVD_DRAWSEED3D_H
#define EVD_DRAWSEED3D_H

#include <iostream>
#include "Analysis/anabase.h"
#include "lardataobj/RecoBase/Seed.h"

#include "RecoBase3D.h"
/**
   \class DrawSeed3D
   User defined class DrawSeed3D ... these comments are used to generate
   doxygen documentation!
 */

// typedef std::vector< std::pair<float,float> > evd::Track2d;

namespace evd {

class Seed3D {

friend class DrawSeed3D;

public:
    TVector3 point(){return _point;}
    TVector3 direction(){return _direction;}
    float length(){return _length;}

private:
    TVector3 _point;
    TVector3 _direction;
    float _length;
};


class DrawSeed3D : public galleryfmwk::anabase, public RecoBase3D<Seed3D> {

public:

    /// Default constructor
    DrawSeed3D();

    /// Default destructor
    ~DrawSeed3D();

    /** IMPLEMENT in DrawCluster.cc!
        Initialization method to be called before the analysis event loop.
    */
    virtual bool initialize();

    /** IMPLEMENT in DrawCluster.cc!
        Analyze a data event-by-event
    */
    virtual bool analyze(gallery::Event * event);

    /** IMPLEMENT in DrawCluster.cc!
        Finalize method to be called after all events processed.
    */
    virtual bool finalize();


private:


};

} // evd

#endif
/** @} */ // end of doxygen group

