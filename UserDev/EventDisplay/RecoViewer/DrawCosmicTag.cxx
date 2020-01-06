#ifndef EVD_DRAWCOSMICTAG_CXX
#define EVD_DRAWCOSMICTAG_CXX

#include "DrawCosmicTag.h"

namespace evd {

CosmicTag DrawCosmicTag::getTrack2D(recob::Track track, unsigned int plane) {
    CosmicTag result;

    result._track.reserve(track.NumberTrajectoryPoints());
    for (unsigned int i = 0; i < track.NumberTrajectoryPoints(); i++) 
    {
        // project a point into 2D:
        try {
	         if (track.HasValidPoint(i)) {
	             auto loc = track.LocationAtPoint(i);
	             auto point = Point_3Dto2D(loc.X(), loc.Y(), loc.Z() , plane);
	             result._track.push_back(std::make_pair(point.w, point.t));
	         }
        } catch (...) {
	         continue;
        }
    }
    
    return result;
}

DrawCosmicTag::DrawCosmicTag(const geo::GeometryCore& geometry, const detinfo::DetectorProperties& detectorProperties) :
    RecoBase(geometry, detectorProperties)
{
  _name = "DrawCosmicTag";
  _fout = 0;
}

bool DrawCosmicTag::initialize() 
{
    // Resize data holder
    if (_dataByPlane.size() != _geoService.Nplanes()) {
        _dataByPlane.resize(_geoService.Nplanes());
    }
    return true;
}

bool DrawCosmicTag::analyze(gallery::Event *ev) 
{
  std::vector<anab::CosmicTag> ctags;
  std::vector<recob::Track>    tracks;

  ctags .clear();
  tracks.clear();

  // get handle to the association
  auto const& assoc_handle = ev->getValidHandle<art::Assns<recob::Track, anab::CosmicTag>>(_producer);
  
  if (assoc_handle->size() == 0)
    return true; // no cosmic tag

  for (auto &ass : *assoc_handle) {

    art::Ptr<recob::Track> t = ass.first;
    art::Ptr<anab::CosmicTag> ct = ass.second;
    
    ctags.emplace_back(*ct);
    tracks.emplace_back(*t);

  }

  // Clear out the data but reserve some space for the tracks
  for (unsigned int p = 0; p < _geoService.Nplanes(); p++) 
  {
    _dataByPlane[p].clear();
    _dataByPlane[p].reserve(tracks.size());
    _wireRange[p].first = 99999;
    _timeRange[p].first = 99999;
    _timeRange[p].second = -1.0;
    _wireRange[p].second = -1.0;
  }

  // Populate the track vector:
  for (auto &track : tracks) {
    for (unsigned int plane = 0; plane < _geoService.Nplanes(); plane++) {
      _dataByPlane[plane].push_back(getTrack2D(track, plane));
    }
  }


  return true;
}

bool DrawCosmicTag::finalize() {

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

DrawCosmicTag::~DrawCosmicTag() {}

} // larlite

#endif
