#ifndef EVD_DRAWVERTEX3D_CXX
#define EVD_DRAWVERTEX3D_CXX

#include "DrawVertex3D.h"

namespace evd {


DrawVertex3D::DrawVertex3D() {
  _name = "DrawVertex3D";
  _fout = 0;
}

bool DrawVertex3D::initialize() {

  return true;
}

bool DrawVertex3D::analyze(gallery::Event * ev) {

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





  // get a handle to the vertexes
  art::InputTag vertex_tag(_producer);
  auto const & vertexHandle
        = ev -> getValidHandle<std::vector <recob::Vertex> >(vertex_tag);


  // Clear out the data but reserve some space
  _data.clear();
  _data.reserve(vertexHandle -> size());


  // Populate the shower vector:
  for (auto & vtx : *vertexHandle) {
    Vertex3D temp;

    double xyz[3];
    vtx.XYZ(xyz);

    temp._x = xyz[0];
    temp._y = xyz[1];
    temp._z = xyz[2];

    _data.push_back(temp);
  }


  return true;
}

bool DrawVertex3D::finalize() {

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
  //   print(MSG::ERROR,__FUNCTION__,"Did not find an output file pointer!!! File not opened?");
  //
  return true;
}

DrawVertex3D::~DrawVertex3D() {}


} // evd

#endif
