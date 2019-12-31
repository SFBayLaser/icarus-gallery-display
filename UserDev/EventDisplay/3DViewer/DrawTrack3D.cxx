#ifndef EVD_DRAWTRACK3D_CXX
#define EVD_DRAWTRACK3D_CXX

#include "DrawTrack3D.h"

namespace evd {

DrawTrack3D::DrawTrack3D() {
  _name = "DrawTrack3D";
  _fout = 0;
}

bool DrawTrack3D::initialize() {

  // Resize data holder
  // if (_data.size() != geoService -> Nviews()) {
  //   _data.resize(geoService -> Nviews());
  // }
  return true;
}

bool DrawTrack3D::analyze(gallery::Event *ev) {

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

  // get a handle to the tracks

  // Clear out the data but reserve some space
  _data.clear();
  _data.reserve(trackHandle->size());

  // Populate the track vector:
  for (auto &track : *trackHandle) {
    _data.push_back(getTrack3d(track));
  }

  return true;
}

bool DrawTrack3D::finalize() {

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

DrawTrack3D::~DrawTrack3D() {}

Track3D DrawTrack3D::getTrack3d(const recob::Track &track) {
  Track3D result;
  result._track.reserve(track.NumberTrajectoryPoints());
  for (size_t i = 0; i < track.NumberTrajectoryPoints(); i++) {
    // project a point into 2D:
    try {
      if (track.HasValidPoint(i)) {
	continue;
	//auto loc = track.LocationAtPoint(i);
	//TVector3 xyz(loc.X(),loc.Y(),loc.Z());
        //auto point = geoHelper->Point_3Dto2D(xyz, plane);
        //result._track.push_back(track.LocationAtPoint(i));
      }
    } catch (...) {
      continue;
    }
  }

  return result;
}

} // evd

#endif
