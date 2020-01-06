#ifndef EVD_DRAWRAWDIGIT_CXX
#define EVD_DRAWRAWDIGIT_CXX

#include "DrawRawDigit.h"

//#include "TTree.h"
//#include "TGraph.h"

//#include "canvas/Persistency/Common/FindMany.h"
//#include "canvas/Utilities/InputTag.h"
#include "gallery/Event.h"

#include "lardataobj/RawData/RawDigit.h"

namespace evd {

DrawRawDigit::DrawRawDigit(const geo::GeometryCore& geometry, const detinfo::DetectorProperties& detectorProperties) : 
  RawBase(geometry, detectorProperties)
{
    _name     = "DrawRawDigit";
    _producer = "rawDigitFilterTPC2"; //"daq";

    // And whether or not to correct the data:
    _correct_data = false;
} 

void DrawRawDigit::setPadding(size_t padding, size_t plane) 
{
    if (_padding_by_plane.size() > plane) _padding_by_plane[plane] = padding;
}

bool DrawRawDigit::initialize() 
{
    //
    // This function is called in the beggining of event loop
    // Do all variable initialization you wish to do here.
    // If you have a histogram to fill in the event loop, for example,
    // here is a good place to create one on the heap (i.e. "new TH1D").
    //
    //
    _padding_by_plane.resize(geoService.Nplanes());
    for (unsigned int p = 0; p < geoService.Nplanes(); p++) 
    {
        setXDimension(geoService.Nwires(p), p);
        setYDimension(detProp.ReadOutWindowSize(), p);
    }

    initDataHolder();
  
    //if (larutil::LArUtilitiesConfig::Detector() == galleryfmwk::geo::kMicroBooNE) {
    //  _noise_filter.init();
    //}
  
    return true;
}

bool DrawRawDigit::analyze(gallery::Event *ev) 
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
  
    // This is an event viewer.  In particular, this handles raw wire signal
    // drawing.
    // So, obviously, first thing to do is to get the wires.
  
    _producer = "rawDigitFilterTPC2";
  
    art::InputTag wires_tag(_producer);
  
    std::cout << "   wires_tag: " << wires_tag << std::endl;
  
    auto const &raw_digits =
        ev->getValidHandle<std::vector<raw::RawDigit>>(wires_tag);
  
    std::cout << "   --> Returned from look up of data, size: " << raw_digits->size() << std::endl;

    // if the tick-length set is different from what is actually stored in the ADC
    // vector -> fix.
    if (raw_digits->size() > 0) 
    {
        for (size_t pl = 0; pl < geoService.Nplanes(); pl++) 
        {
            if (_y_dimensions[pl] < raw_digits->front().ADCs().size()) 
            {
                _y_dimensions[pl] = raw_digits->front().ADCs().size();
            }
        }
    }

    _planeData.clear();

    size_t n_ticks = raw_digits->front().ADCs().size();

    //if (larutil::LArUtilitiesConfig::Detector() == galleryfmwk::geo::kMicroBooNE) {
    //  _noise_filter.set_n_time_ticks(n_ticks);
    //}
    initDataHolder();

    // If the output data holder is not the same size as RawDigit length,
    // it messes up the noise filter.  Easist thing to do here is to
    // temporarily store the data in it's native format, then copy
    // the data over to the final output.

    std::vector<std::vector<float>> temp_data_holder(geoService.Nplanes());

    for (size_t i_plane = 0; i_plane < geoService.Nplanes(); i_plane++) 
        temp_data_holder[i_plane].resize(n_ticks * _x_dimensions[i_plane]);


    for (auto const &rawdigit : *raw_digits) 
    {
        unsigned int ch  = rawdigit.Channel();
        float        ped = rawdigit.GetPedestal();
    
        std::vector<geo::WireID> widVec = geoService.ChannelToWire(ch);
        unsigned int wire = widVec[0].Wire;
        unsigned int plane = widVec[0].Plane;
    
        if (wire > geoService.Nwires(plane)) continue;
    
        int offset = wire * n_ticks;

        std::vector<float>&          planeRawDigitVec = temp_data_holder[plane];
        std::vector<float>::iterator startItr         = planeRawDigitVec.begin() + offset;

        // Copy with pedestal subtraction
        for(const auto& adcVal : rawdigit.ADCs())
            *startItr++ = adcVal - ped;
   }


    //if (larutil::LArUtilitiesConfig::Detector() == galleryfmwk::geo::kMicroBooNE) {
    //  _noise_filter.set_data(&_temp_data_holder);
    //  if (_correct_data && ev->eventAuxiliary().isRealData()) {
    //    _noise_filter.clean_data();
    //  } else {
    //    _noise_filter.pedestal_subtract_only();
    //  }
    //}
  
    // In some cases, raw digits are truncated and the padding is needed.
    // In other cases, raw digits are not truncated and no padding is needed,
    // even in a truncate file.  What a mess.
    // Hack: wipe out the padding if it's clearly not needed:
  
    std::vector<int> _temp_padding_by_plane(_padding_by_plane.size(), 0);
  
    for (size_t i_plane = 0; i_plane < geoService.Nviews(); i_plane++) 
    {
        if (n_ticks + _padding_by_plane[i_plane] > _y_dimensions[i_plane]) 
            _temp_padding_by_plane[i_plane] = 0;
        else                                                                
            _temp_padding_by_plane[i_plane] = _padding_by_plane[i_plane];
    }

    // Now, copy the data from the temp storage to the output storage:
  
    for (size_t i_plane = 0; i_plane < geoService.Nplanes(); i_plane++) 
    {
        size_t n_wires = temp_data_holder[i_plane].size() / n_ticks;
  
        std::vector<float>& planeTempData = temp_data_holder[i_plane];
        std::vector<float>& planeData     = _planeData[i_plane];
  
        for (size_t i_wire = 0; i_wire < n_wires; i_wire++) 
        {
            size_t offset_raw   = i_wire * n_ticks;
            size_t offset_final = i_wire * _y_dimensions[i_plane] + _temp_padding_by_plane[i_plane];

            std::vector<float>::iterator tempDataItr  = planeTempData.begin() + offset_raw;
            std::vector<float>::iterator planeDataItr = planeData.begin() + offset_final;

            std::copy(tempDataItr,tempDataItr+n_ticks,planeDataItr);
        }
    }

    return true;
}

bool DrawRawDigit::finalize() 
{
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

}
#endif
