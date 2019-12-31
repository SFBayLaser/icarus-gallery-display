//
// cint script to generate libraries
// Declaire namespace & classes you defined
// #pragma statement: order matters! Google it ;)
//

#ifdef __CLING__
#pragma link off all globals;
#pragma link off all classes;
#pragma link off all functions;

#pragma link C++ namespace larutil;
#pragma link C++ namespace galleryfmwk;
#pragma link C++ namespace galleryfmwk::geo;

#pragma link C++ nestedclass;

#pragma link C++ class larutil::LArUtilException+;
#pragma link C++ class larutil::InvalidWireError+;

#pragma link C++ class std::vector<std::vector<Float_t> >+;
#pragma link C++ class std::vector<std::vector<std::vector<Double_t > > >+;
#pragma link C++ class larutil::LArUtilBase+;
#pragma link C++ class larutil::Geometria+;
#pragma link C++ class larutil::LArProp+;
#pragma link C++ class larutil::DetProperties+;
#pragma link C++ class larutil::TimeService+;
#pragma link C++ class larutil::ElecClock+;
#pragma link C++ class larutil::PxPoint+;
#pragma link C++ class larutil::PxLine+;
#pragma link C++ class larutil::PxHit+;

#pragma link C++ class larutil::LArUtilitiesConfig+;
#pragma link C++ class larutil::LArUtilManager+;
#pragma link C++ class larutil::GeometriaHelper+;
// //ADD_NEW_CLASS ... do not change this line
#endif








