#ifndef LARUTILMANAGER_CXX
#define LARUTILMANAGER_CXX

#include "LArUtilManager.h"

namespace larutil {

bool LArUtilManager::Reconfigure(galleryfmwk::geo::DetId_t type)
{

  if (type == LArUtilitiesConfig::Detector()) return true;

  bool status = LArUtilitiesConfig::SetDetector(type);

  if (!status) return status;

  return ReconfigureUtilities();

}

bool LArUtilManager::ReconfigureUtilities()
{
  bool status = true;

  // Geometry
  galleryfmwk::Message::send(__FUNCTION__, "Reconfiguring Geometry");
  Geometria* geom = (Geometria*)(Geometria::GetME(false));
  geom->SetFileName(Form("%s/LArUtil/dat/%s",
                         getenv("GALLERY_FMWK_COREDIR"),
                         kUTIL_DATA_FILENAME[LArUtilitiesConfig::Detector()].c_str()));
  geom->SetTreeName(kTREENAME_GEOMETRY);
  status = status && geom->LoadData(true);

  // LArProp
  galleryfmwk::Message::send(__FUNCTION__, "Reconfiguring LArProp");
  LArProp* larp = (LArProp*)(LArProp::GetME(false));
  larp->SetFileName(Form("%s/LArUtil/dat/%s",
                         getenv("GALLERY_FMWK_COREDIR"),
                         kUTIL_DATA_FILENAME[LArUtilitiesConfig::Detector()].c_str()));
  larp->SetTreeName(kTREENAME_LARPROPERTIES);
  status = status && larp->LoadData(true);

  // DetProperties
  galleryfmwk::Message::send(__FUNCTION__, "Reconfiguring DetProperties");
  DetProperties* detp = (DetProperties*)(DetProperties::GetME(false));
  detp->SetFileName(Form("%s/LArUtil/dat/%s",
                         getenv("GALLERY_FMWK_COREDIR"),
                         kUTIL_DATA_FILENAME[LArUtilitiesConfig::Detector()].c_str()));
  detp->SetTreeName(kTREENAME_DETECTORPROPERTIES);
  status = status && detp->LoadData(true);

  if (status) {

    // GeometriaHelper
    galleryfmwk::Message::send(__FUNCTION__, "Reconfiguring GeometriaHelper...");
    GeometriaHelper* gser = (GeometriaHelper*)(GeometriaHelper::GetME());
    gser->Reconfigure();

    // GeometriaHelper
    galleryfmwk::Message::send( __FUNCTION__, "Reconfiguring GeometriaHelper...");
    GeometriaHelper* ghelp = (GeometriaHelper*)(GeometriaHelper::GetME());
    ghelp->Reconfigure();

  }

  return status;
}

}

#endif
