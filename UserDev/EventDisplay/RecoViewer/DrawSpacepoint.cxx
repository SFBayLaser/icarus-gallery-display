#ifndef EVD_DRAWSPACEPOINT_CXX
#define EVD_DRAWSPACEPOINT_CXX

#include "DrawSpacepoint.h"

namespace evd {


DrawSpacepoint::DrawSpacepoint(const geo::GeometryCore& geometry, const detinfo::DetectorProperties& detectorProperties) :
    RecoBase(geometry, detectorProperties)
{
    _name = "DrawSpacepoint";
    _fout = 0;
}

bool DrawSpacepoint::initialize() 
{
    if (_dataByPlane.size() != _geoService.Nplanes()) {
        _dataByPlane.resize(_geoService.Nplanes());
    }
    return true;
}

bool DrawSpacepoint::analyze(gallery::Event * ev) 
{
    // get a handle to the tracks
    art::InputTag sps_tag(_producer);
    auto const & spacepointHandle = ev -> getValidHandle<std::vector <recob::SpacePoint> >(sps_tag);
  
    // Clear out the data but reserve some space
    for (unsigned int p = 0; p < _geoService.Nplanes(); p ++) 
    {
      _dataByPlane[p].clear();
      _dataByPlane[p].reserve(spacepointHandle -> size());
      _wireRange[p].first  = 99999;
      _timeRange[p].first  = 99999;
      _timeRange[p].second = -1.0;
      _wireRange[p].second = -1.0;
    }
  
    evd::Point2D point;
    float        timeToCm = _detProp.SamplingRate() / 1000.0 * _detProp.DriftVelocity(_detProp.Efield(), _detProp.Temperature());
  
    // Populate the spacepoint vector:
    for (auto & spt : *spacepointHandle) 
    {
      // A spacepoint is a 3D object.  So take it and project it into each plane:
      for (unsigned int p = 0; p < _geoService.Nplanes(); p ++) 
      {
        try {
          point = Point_3Dto2D(spt.XYZ()[0],spt.XYZ()[1],spt.XYZ()[2],p);
        }
        catch (const std::exception& e) {
          std::cerr << e.what() << '\n';
        }
        _dataByPlane[p].push_back(point);
  
        // Determine if this hit should change the view range:
        if (point.w / _geoService.WirePitch() > _wireRange[p].second)
          _wireRange[p].second = point.w / _geoService.WirePitch();
        if (point.w / _geoService.WirePitch() < _wireRange[p].first)
          _wireRange[p].first = point.w / _geoService.WirePitch();
        if (point.t / timeToCm > _timeRange[p].second)
          _timeRange[p].second = point.t / timeToCm;
        if (point.t / timeToCm < _timeRange[p].first)
          _timeRange[p].first = point.t / timeToCm;
      }
    }


  return true;
}

bool DrawSpacepoint::finalize() {

  return true;
}

DrawSpacepoint::~DrawSpacepoint() {}


} // evd

#endif
