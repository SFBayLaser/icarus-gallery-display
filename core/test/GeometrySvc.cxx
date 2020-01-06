#ifndef GALLERY_FMWK_GEOMETRYSVC_CXX
#define GALLERY_FMWK_GEOMETRYSVC_CXX

#include "GeometrySvc.h"

namespace larservices {

void GeometrySvc::printGeometry() const
{
  _geometryCore.Print(std::cout);
}

}

#endif
