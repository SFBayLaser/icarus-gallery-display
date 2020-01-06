/**
 * \file Geometria.h
 *
 * \ingroup LArUtil
 *
 * \brief Class def header for a class Geometria
 *
 * @author kazuhiro
 */

/** \addtogroup LArUtil

    @{*/
#ifndef GALLERY_FMWK_GEOMETRYSVC_H
#define GALLERY_FMWK_GEOMETRYSVC_H

#include "larcorealg/Geometry/GeometryCore.h"
#include <iostream>

namespace larservices {
/**
   \class Geometria
*/
class GeometrySvc  
{

public:

    /// Constructor
    GeometrySvc(const geo::GeometryCore& geometryCore) : _geometryCore(geometryCore) {}

    /// Default destructor
    virtual ~GeometrySvc() {};

    void printGeometry() const;

    const geo::GeometryCore& geo() {return _geometryCore;}


private:
    const geo::GeometryCore& _geometryCore;

};
}

#endif
/** @} */ // end of doxygen group

