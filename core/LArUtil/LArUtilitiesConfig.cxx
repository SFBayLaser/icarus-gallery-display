#ifndef GALLERY_FMWK_LARUTILCONFIG_CXX
#define GALLERY_FMWK_LARUTILCONFIG_CXX

#include "LArUtilitiesConfig.h"

namespace larutil {

  galleryfmwk::geo::DetId_t LArUtilitiesConfig::_detector = galleryfmwk::geo::kMicroBooNE;


  bool LArUtilitiesConfig::SetDetector(galleryfmwk::geo::DetId_t type)
  {
    if(_detector == type) return true;
    bool status = true;
    switch(type) {

    case galleryfmwk::geo::kArgoNeuT: 
    case galleryfmwk::geo::kMicroBooNE:
      _detector = type;
      break;
    case galleryfmwk::geo::kICARUS:
      _detector = type;
      break;
    case galleryfmwk::geo::kBo:
    case galleryfmwk::geo::kLBNE10kt:
    case galleryfmwk::geo::kJP250L:
    case galleryfmwk::geo::kLBNE35t:
    case galleryfmwk::geo::kLBNE34kt:
    case galleryfmwk::geo::kCSU40L:
    case galleryfmwk::geo::kLArIAT:
    case galleryfmwk::geo::kDetIdMax:
      galleryfmwk::Message::send(__FUNCTION__,
			      Form("Detector type: %d not supported!",type));
      status = false;
    }
    return status;
  }

}

#endif
