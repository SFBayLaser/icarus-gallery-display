//
// cint script to generate libraries
// Declaire namespace & classes you defined
// #pragma statement: order matters! Google it ;)
//

#ifdef __CLING__
#pragma link off all globals;
#pragma link off all classes;
#pragma link off all functions;

#pragma link C++ namespace larservices;
#pragma link C++ namespace geo;

#pragma link C++ nestedclass;

#pragma link C++ class geo::GeometryCore+;

#pragma link C++ class larservices::GeometrySvc+;
// //ADD_NEW_CLASS ... do not change this line
#endif








