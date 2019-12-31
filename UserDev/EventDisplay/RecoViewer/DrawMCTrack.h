/**
 * \file DrawMCTrack.h
 *
 * \ingroup RecoViewer
 *
 * \brief Class def header for a class DrawMCTrack
 *
 * @author cadams, mdeltutt
 */

/** \addtogroup RecoViewer

    @{*/
#ifndef EVD_DRAWMCTRACK_H
#define EVD_DRAWMCTRACK_H

#include "Analysis/anabase.h"
#include "lardataobj/MCBase/MCStep.h"
#include "lardataobj/MCBase/MCTrack.h"
//#include "lardataobj/SimulationBase/MCParticle.h"
#include <iostream>

#include "DrawTrack.h"
#include "RecoBase.h"
/**
   \class DrawMCTrack
   User defined class DrawTrack ... these comments are used to generate
   doxygen documentation!
 */

namespace evd {

class MCTrack2D : public Track2D {

  friend class DrawMCTrack;

public:
  const int &origin() { return _origin; }

protected:
  int _origin; // mc origin type: 0=unknown, 1=beam neutrino, 2=cosmic,
               // 3=supernova neutrino, 4=single particle
};

class DrawMCTrack : public galleryfmwk::anabase, public RecoBase<MCTrack2D> {

public:
  /// Default constructor
  DrawMCTrack();

  /// Default destructor
  ~DrawMCTrack();

  virtual bool initialize();

  virtual bool analyze(gallery::Event *event);

  virtual bool finalize();

private:
  MCTrack2D getMCTrack2D(simb::MCParticle track, unsigned int plane);
};

} // evd

#endif
/** @} */ // end of doxygen group
