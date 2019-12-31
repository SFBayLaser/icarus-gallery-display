/**
 * \file DrawShower3D.h
 *
 * \ingroup RecoViewer
 *
 * \brief Class def header for a class DrawShower3D
 *
 * @author cadams
 */

/** \addtogroup RecoViewer

    @{*/
#ifndef EVD_DRAWSHOWER3D_H
#define EVD_DRAWSHOWER3D_H

#include <iostream>
#include "Analysis/anabase.h"
#include "lardataobj/RecoBase/Shower.h"

#include "RecoBase3D.h"
/**
   \class DrawShower3D
   User defined class DrawShower3D ... these comments are used to generate
   doxygen documentation!
 */

// typedef std::vector< std::pair<float,float> > evd::Track2d;

namespace evd {

class Shower3D {

friend class DrawShower3D;

public:
    // std::vector<TVector3 > _track;
    // const std::vector<TVector3 > & shower() {return _track;}
    // const std::vector<std::vector<float> > & direction() {return _track;}
    TVector3 start_point(){return _start_point;}
    TVector3 direction(){return _direction;}
    float length(){return _length;}
    float opening_angle(){return _opening_angle;}

protected:
    TVector3 _start_point;
    TVector3 _direction;
    float _length;
    float _opening_angle;
};


class DrawShower3D : public galleryfmwk::anabase, public RecoBase3D<Shower3D> {

public:

    /// Default constructor
    DrawShower3D();

    /// Default destructor
    ~DrawShower3D();

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

    Shower3D getShower3d(const recob::Shower &  shower);

};

} // evd

#endif
/** @} */ // end of doxygen group

