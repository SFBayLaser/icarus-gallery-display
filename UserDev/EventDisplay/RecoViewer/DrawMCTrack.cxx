#ifndef EVD_DRAWMCTRACK_CXX
#define EVD_DRAWMCTRACK_CXX

#include "DrawMCTrack.h"

namespace evd {

MCTrack2D DrawMCTrack::getMCTrack2D(simb::MCParticle track, unsigned int plane) {
  MCTrack2D result;
  result._track.reserve(track.NumberTrajectoryPoints());
  auto vtxtrk = track.Position(0);
  if (plane == 2) {
    std::cout << "Particle with PDG " << track.PdgCode() << std::endl;
    std::cout << "\t with " << track.NumberTrajectoryPoints() << " points and vertex @ " 
	      << "[ " << vtxtrk.X() << ", " << vtxtrk.Y() << ", " << vtxtrk.Z() << " ]" << std::endl;
  }
  for (unsigned int i = 0; i < track.NumberTrajectoryPoints(); i++) {
    // project a point into 2D:
    try {
      auto pointtrk = track.Position(i);
      auto point = Point_3Dto2D(pointtrk.X(), pointtrk.Y(), pointtrk.Z(), plane);
      result._track.push_back(std::make_pair(point.w, point.t));
    } catch (...) {
      continue;
    }
  }

  //result._origin = track.Origin();

  return result;
}

DrawMCTrack::DrawMCTrack(const geo::GeometryCore& geometry, const detinfo::DetectorProperties& detectorProperties) :
    RecoBase(geometry, detectorProperties)
{
  _name = "DrawMCTrack";
  _fout = 0;
}

bool DrawMCTrack::initialize() {

  // Resize data holder
  if (_dataByPlane.size() != _geoService.Nplanes()) {
    _dataByPlane.resize(_geoService.Nplanes());
  }
  return true;
}

bool DrawMCTrack::analyze(gallery::Event *ev) 
{
  //
  // Do your event-by-event analysis here. This function is called for
  // each event in the loop. You have "storage" pointer which contains
  // event-wise data. To see what is available, check the "Manual.pdf":
  //
  // http://microboone-docdb.fnal.gov:8080/cgi-bin/ShowDocument?docid=3183
  //
  // Or you can refer to Base/DataFormatConstants.hh for available data type
  // enum values. Here is one example of getting PMT waveform collection.
  //
  // event_fifo* my_pmtfifo_v = (event_fifo*)(storage->get_data(DATA::PMFIFO));
  //
  // if( event_fifo )
  //
  //   std::cout << "Event ID: " << my_pmtfifo_v->event_id() << std::endl;
  //

  // get a handle to the tracks
  art::InputTag tracks_tag(_producer);
  auto const &trackHandle = ev->getValidHandle<std::vector<simb::MCParticle>>(tracks_tag);

  //std::cout << "there are " << trackHandle->size() << " mctracks" << std::endl;

  // Clear out the data but reserve some space for the tracks
  for (unsigned int p = 0; p < _geoService.Nplanes(); p++) 
  {
      _dataByPlane[p].clear();
      _dataByPlane[p].reserve(trackHandle->size());
      _wireRange[p].first = 99999;
      _timeRange[p].first = 99999;
      _timeRange[p].second = -1.0;
      _wireRange[p].second = -1.0;
  }

  // Populate the track vector:
  for (auto &track : *trackHandle) {

    // pass some minimum threshold in momentum
    if ( (track.NumberTrajectoryPoints() < 5) && (fabs(track.PdgCode() == 11) ) ) continue;

    for (unsigned int plane = 0; plane < _geoService.Nplanes(); plane++) 
    {
      if ( (fabs(track.PdgCode()) == 13) || (fabs(track.PdgCode()) == 2212) || (fabs(track.PdgCode()) == 211) || (fabs(track.PdgCode()) == 11) )
	       _dataByPlane[plane].push_back(getMCTrack2D(track, plane));
    }
  }

  return true;
}

bool DrawMCTrack::finalize() {

  // This function is called at the end of event loop.
  // Do all variable finalization you wish to do here.
  // If you need, you can store your ROOT class instance in the output
  // file. You have an access to the output file through "_fout" pointer.
  //
  // Say you made a histogram pointer h1 to store. You can do this:
  //
  // if(_fout) { _fout->cd(); h1->Write(); }
  //
  // else
  //   print(MSG::ERROR,__FUNCTION__,"Did not find an output file pointer!!!
  //   File not opened?");
  //
  return true;
}

DrawMCTrack::~DrawMCTrack() {}

} // larlite

#endif
