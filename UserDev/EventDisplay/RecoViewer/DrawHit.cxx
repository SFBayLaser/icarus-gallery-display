#ifndef EVD_DRAWHIT_CXX
#define EVD_DRAWHIT_CXX

#include "DrawHit.h"

namespace evd {


DrawHit::DrawHit(const geo::GeometryCore& geometry, const detinfo::DetectorProperties& detectorProperties) :
    RecoBase(geometry,detectorProperties)
{
    _name = "DrawHit";
    _fout = 0;
}

bool DrawHit::initialize() 
{
    std::cout << "***** Initializing DrawHit *****" << std::endl;
    // // Resize data holder to accommodate planes and wires:
    if (_dataByPlane.size() != _geoService.Nplanes()) 
    {
        _dataByPlane.resize(_geoService.Nplanes());
        _maxCharge.resize(_geoService.Nplanes());
    }
    return true;
}

bool DrawHit::analyze(gallery::Event* ev) 
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
  
    // get a handle to the hits
  
    art::InputTag hits_tag(_producer);
    auto const & hitHandle = ev -> getValidHandle<std::vector <recob::Hit> >(hits_tag);

    std::cout << "***** recovering data for hits, input: " << hits_tag << ", size: " << hitHandle->size() << std::endl;
  
    // Clear out the hit data but reserve some space for the hits
    for (unsigned int p = 0; p < _geoService.Nplanes(); p ++) 
    {
        _dataByPlane[p].clear();
        _dataByPlane[p].reserve(hitHandle->size());
        _maxCharge[p] = 0.0;
        _wireRange[p].first  = 99999;
        _timeRange[p].first  = 99999;
        _timeRange[p].second = -1.0;
        _wireRange[p].second = -1.0;
    }

    for (auto & hit : *hitHandle) 
    {
        unsigned int plane = hit.WireID().Plane;
        _dataByPlane[plane].emplace_back(
            Hit2D(hit.WireID().Wire,
                  hit.PeakTime(),
                  hit.Integral(),
                  hit.RMS(),
                  hit.StartTick(),
                  hit.PeakTime(),
                  hit.EndTick(),
                  hit.PeakAmplitude(),
                  plane
                 ));
        if (_dataByPlane[plane].back()._charge > _maxCharge[plane])
          _maxCharge[plane] = _dataByPlane[plane].back()._charge;
        // Check the auto range values:
        if (_dataByPlane[plane].back().wire() < _wireRange[plane].first) {
          _wireRange[plane].first = _dataByPlane[plane].back().wire();
        }
        if (_dataByPlane[plane].back().wire() > _wireRange[plane].second) {
          _wireRange[plane].second = _dataByPlane[plane].back().wire();
        }
        if (_dataByPlane[plane].back().time() < _timeRange[plane].first) {
          _timeRange[plane].first = _dataByPlane[plane].back().time();
        }
        if (_dataByPlane[plane].back().time() > _timeRange[plane].second) {
          _timeRange[plane].second = _dataByPlane[plane].back().time();
        }
        // wireByPlane -> at(view).push_back(hit.WireID().Wire);
        // hitStartByPlane -> at(view).push_back(hit.PeakTime() - hit.RMS());
        // hitEndByPlane -> at(view).push_back(hit.PeakTime() + hit.RMS());
    }

  return true;
}

float DrawHit::maxCharge(size_t p) 
{
    if (p >= _geoService.Nplanes() ) 
    {
        std::cerr << "ERROR: Request for nonexistent plane " << p << std::endl;
        return 1.0;
    }
    else 
    {
        try {
            return _maxCharge[p];
        }
        catch (const std::exception& e) {
            std::cerr << e.what() << '\n';
            return 1.0;
        }
    }
}

std::vector<Hit2D> DrawHit::getHitsOnWirePlane(size_t wire, size_t plane) 
{
    std::vector<Hit2D> result;

    if (plane >= _geoService.Nplanes() ) 
    {
        std::cerr << "ERROR: Request for nonexistent plane " << plane << std::endl;
        return result;
    }
    else 
    {
        for (auto & hit : _dataByPlane.at(plane)) {
            if (hit.wire() == wire)
                result.emplace_back(hit);
        }
    }

  return result;
}


bool DrawHit::finalize() {

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




} // larlite

#endif
