#ifndef EVD_DRAWVERTEX_CXX
#define EVD_DRAWVERTEX_CXX

#include "DrawVertex.h"

namespace evd {

DrawVertex::DrawVertex(const geo::GeometryCore& geometry, const detinfo::DetectorProperties& detectorProperties) :
    RecoBase(geometry,detectorProperties)
{
    _name = "DrawVertex";
    _fout = 0;
}

bool DrawVertex::initialize() 
{
  //
  // This function is called in the beginning of event loop
  // Do all variable initialization you wish to do here.
  // If you have a histogram to fill in the event loop, for example,
  // here is a good place to create one on the heap (i.e. "new TH1D").
  //
  if (_dataByPlane.size() != _geoService.Nplanes()) {
    _dataByPlane.resize(_geoService.Nplanes());
  }
  return true;

}

bool DrawVertex::analyze(gallery::Event * ev) 
{
  art::InputTag vertex_tag(_producer);
  auto const & vertexHandle = ev -> getValidHandle<std::vector <recob::Vertex> >(vertex_tag);

  // Clear out the hit data but reserve some space for the hits
  for (unsigned int plane = 0; plane < _geoService.Nplanes(); plane ++) 
  {
    _dataByPlane[plane].clear();
    _dataByPlane[plane].reserve(vertexHandle -> size());
    _wireRange[plane].first  = 99999;
    _timeRange[plane].first  = 99999;
    _timeRange[plane].second = -1.0;
    _wireRange[plane].second = -1.0;
  }


  // Loop over the vertices and fill the necessary vectors.
  Vertex2D point;
  double * xyz = new double[3];

  for (auto & vertex : * vertexHandle) 
  {
    // A vertex is a 3D object.  So take it and project it into each plane:
    for (unsigned int plane = 0; plane < _geoService.Nplanes(); plane++) 
    {
      vertex.XYZ(xyz);

      try {
        point = Point_3Dto2D(xyz[0], xyz[1], xyz[2], plane);
      }
      catch (const std::exception& e) {
        std::cerr << e.what() << '\n';
      }
      _dataByPlane[plane].push_back(point);


      // Determine if this hit should change the view range:
      if (point.w > _wireRange[plane].second)
        _wireRange[plane].second = point.w;
      if (point.w < _wireRange[plane].first)
        _wireRange[plane].first = point.w;
      if (point.t > _timeRange[plane].second)
        _timeRange[plane].second = point.t;
      if (point.t < _timeRange[plane].first)
        _timeRange[plane].first = point.t;
    }
  }


  return true;
}

bool DrawVertex::finalize() {

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
  //

  return true;
}

}
#endif
