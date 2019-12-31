#ifndef EVD_DRAWTRACK_CXX
#define EVD_DRAWTRACK_CXX

#include "DrawTrack.h"

namespace evd {

Track2D DrawTrack::getTrack2D(recob::Track track, unsigned int plane) {
  Track2D result;
  auto geoHelper = larutil::GeometriaHelper::GetME();
  result._track.reserve(track.NumberTrajectoryPoints());
  for (unsigned int i = 0; i < track.NumberTrajectoryPoints(); i++) {
    // project a point into 2D:
    try {
      if (track.HasValidPoint(i)) {
	auto loc = track.LocationAtPoint(i);
	TVector3 xyz(loc.X(),loc.Y(),loc.Z());
        auto point = geoHelper->Point_3Dto2D(xyz, plane);
        result._track.push_back(std::make_pair(point.w, point.t));
      }
    } catch (...) {
      continue;
    }
  }

  return result;
}

DrawTrack::DrawTrack() {
  _name = "DrawTrack";
  _fout = 0;
}

bool DrawTrack::initialize() {

  // Resize data holder
  if (_dataByPlane.size() != geoService->Nviews()) {
    _dataByPlane.resize(geoService->Nviews());
  }
  return true;
}

bool DrawTrack::analyze(gallery::Event *ev) {

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
  auto const &trackHandle =
      ev->getValidHandle<std::vector<recob::Track>>(tracks_tag);

  // Clear out the data but reserve some space for the tracks
  for (unsigned int p = 0; p < geoService->Nviews(); p++) {
    _dataByPlane.at(p).clear();
    _dataByPlane.at(p).reserve(trackHandle->size());
    _wireRange.at(p).first = 99999;
    _timeRange.at(p).first = 99999;
    _timeRange.at(p).second = -1.0;
    _wireRange.at(p).second = -1.0;
  }

  // Populate the track vector:
  for (auto &track : *trackHandle) {
    for (unsigned int view = 0; view < geoService->Nviews(); view++) {
      _dataByPlane.at(view).push_back(getTrack2D(track, view));
    }
  }

  return true;
}

bool DrawTrack::finalize() {

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

DrawTrack::~DrawTrack() {}

} // larlite

#endif
