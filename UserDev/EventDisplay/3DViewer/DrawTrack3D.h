/**
 * \file DrawTrack3D.h
 *
 * \ingroup RecoViewer
 *
 * \brief Class def header for a class DrawTrack3D
 *
 * @author cadams
 */

/** \addtogroup RecoViewer

    @{*/
#ifndef EVD_DRAWTRACK3D_H
#define EVD_DRAWTRACK3D_H

#include <iostream>
#include "Analysis/anabase.h"
#include "lardataobj/RecoBase/Track.h"

#include "RecoBase3D.h"
/**
   \class DrawTrack3D
   User defined class DrawTrack3D ... these comments are used to generate
   doxygen documentation!
 */


namespace evd {

class Track3D {

public:
  friend class DrawTrack3D;

  const std::vector<TVector3> &track() { return _track; }
  // const std::vector<std::vector<float> > & direction() {return _track;}
  TVector3 start_point() { return _start_point; }
  TVector3 end_point() { return _end_point; }


protected:
    std::vector<TVector3 > _track;
    TVector3 _start_point;
    TVector3 _end_point;
};


class DrawTrack3D : public galleryfmwk::anabase, public RecoBase3D<Track3D> {

public:

    /// Default constructor
    DrawTrack3D();

    /// Default destructor
    ~DrawTrack3D();

    /** IMPLEMENT in DrawCluster.cc!
        Initialization method to be called before the analysis event loop.
    */
    virtual bool initialize();

    /** IMPLEMENT in DrawCluster.cc!
        Analyze a data event-by-event
    */
    virtual bool analyze(gallery::Event * ev);

    /** IMPLEMENT in DrawCluster.cc!
        Finalize method to be called after all events processed.
    */
    virtual bool finalize();


private:

    Track3D getTrack3d(const recob::Track & track);

};

} // evd

#endif
/** @} */ // end of doxygen group

