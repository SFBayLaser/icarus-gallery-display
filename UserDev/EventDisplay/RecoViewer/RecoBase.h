/**
 * \file RecoBase.h
 *
 * \ingroup RecoViewer
 *
 * \brief Class def header for a class RecoBase
 *
 * @author cadams
 */

/** \addtogroup RecoViewer

    @{*/
#ifndef RECOBASE_H
#define RECOBASE_H

#include "larcorealg/Geometry/GeometryCore.h"
#include "lardataalg/DetectorInfo/DetectorProperties.h"

#include <iostream>

struct _object;
typedef _object PyObject;

#ifndef __CLING__
#include "Python.h"
#define NPY_NO_DEPRECATED_API NPY_1_7_API_VERSION
#include "numpy/arrayobject.h"
#endif

/**
   \class RecoBase
   User defined class RecoBase ... these comments are used to generate
   doxygen documentation!
 */
namespace evd {

class PxPoint 
{
public:
    float  w; // wire distance in cm
    float  t; // time distance in cm (drift distance)
    size_t plane; // plane 0, 1, 2
  
    PxPoint() {
      Clear();
      //        std::cout<< "This is the default point ctor." << std::endl;
    }
  
    PxPoint(unsigned char pp, float ww, float tt) {
      plane = pp;
      w = ww;
      t = tt;
    }
  
    void Clear()
    {
      plane = 0;
      w     = 0;
      t     = 0;
    }
  
    ~PxPoint() {}
};

/// This is gonna be really really useful. Trust me. 
using Point2D = PxPoint;

template <class DATA_TYPE> class RecoBase 
{
public:

    /// Default constructor
    RecoBase(const geo::GeometryCore& geometry, const detinfo::DetectorProperties& detectorProperties);
  
    /// Default destructor
    virtual ~RecoBase() {}
  
    std::pair<float, float> getWireRange(size_t p);
    std::pair<float, float> getTimeRange(size_t p);
  
    void setProducer(std::string s);
  
    const std::vector<DATA_TYPE> & getDataByPlane(size_t p);

    // PyObject * getDataByPlane(size_t p);

    /**
     * @brief Convert a 3D point in the detector to 2D point on a plane
     * @details Conversion is done by setting the time to the X coordinated, minus any corrections from trigger offset and plane offset.
     * The wire coordinate is a combination of Y, Z coordinates that depends on the angle of the wires WRT vertical
     *
     * @param xyz Pointer to float of the point in 3D to project into 2D.  Caller is responsible for creating and deleting this array
     * @param plane The index of the plane to project into.  Ranges from 0 to Nplanes -1 (Nplanes available in Geometry::Nplanes() )
     *
     * @return Returns a Point2D of the 2D projection into the plane
     */
    Point2D Point_3Dto2D(float x, float y, float z, unsigned int plane) const;

    /**
     * @brief Take a line (start point and direction) and project that into 2D
     * @details This function will return the start point and direction of a line segment projected into 2D
     *  It works by using the Point_3Dto2D function to project the start point into 2D.  Then, it finds a second
     *  point along the line by using startPoint + direction3D.  If that point is not in the TPC, it halves the distance and
     *  tries that point.  Then, once it has two points in the TPC, it projects both into the plane, normalizes the direction,
     *  and returns them as PxPoints by reference
     *
     * @param startPoint3D TVector3 describing the 3D start point (in detector coordinates)
     * @param direction3D TVector3 describing the direction that the line emanates from the start point.  Need not be normalized
     * @param plane The plane in which the projection needs to be done.
     * @param startPoint2D Returned by reference: A Point2D representing the projection of the start point into the plane.
     * @param direction2D Returned by reference: A Point2D representing the projection of the direction into the plane, normalized.
     */
     float Slope_3Dto2D(float x, float y, float z, unsigned int plane) const ;    

protected:

    void _init_base();

    void Line_3Dto2D( const TVector3 & startPoint3D, const TVector3 & direction3D, unsigned int plane,
                      Point2D& startPoint2D, Point2D& direction2D) const;

    /**
     * @brief deterimine if a point is in the TPC
     * @details Checks this point against the geometry parameters
     *
     * @param pointIn3D The point to be evaluated, units in cm.
     * @return true if the point is in the TPC, false if otherwise
     */
    bool Point_isInTPC(const TVector3 & pointIn3D) const;

    const geo::GeometryCore&           _geoService;
    const detinfo::DetectorProperties& _detProp;

    std::string _producer;
  
    // Store the reco data to draw;
    std::vector <std::vector<DATA_TYPE>> _dataByPlane;
  
    // Store the bounding parameters of interest:
    // highest and lowest wire, highest and lowest time
    // Have to sort by plane still
  
    std::vector<std::pair<float, float>> _wireRange;
    std::vector<std::pair<float, float>> _timeRange;
};


template <class DATA_TYPE>
RecoBase<DATA_TYPE>::RecoBase(const geo::GeometryCore& geometry, const detinfo::DetectorProperties& detectorProperties) :
    _geoService(geometry),
    _detProp(detectorProperties)
{
    // Set up default values of the _wire and _time range
    _wireRange.resize(_geoService.Nplanes());
    _timeRange.resize(_geoService.Nplanes());
  
    for (size_t plane = 0; plane < _geoService.Nplanes(); plane++) 
    {
        _wireRange[plane].first  = 0;
        _wireRange[plane].second = _geoService.Nwires(plane);
        _timeRange[plane].first  = 0;
        _timeRange[plane].second = _detProp.NumberTimeSamples();
    }
}

template <class DATA_TYPE> void RecoBase <DATA_TYPE>::setProducer(std::string s) 
{
    _producer = s;
}

template <class DATA_TYPE> std::pair<float, float> RecoBase<DATA_TYPE>::getWireRange(size_t p) 
{
    std::pair<float, float> result(0.,0.);

    if (p >= _geoService.Nplanes() )
      std::cerr << "ERROR: Request for nonexistent plane " << p << std::endl;
    else {
        try {
          result = _wireRange[p];
        }
        catch (const std::exception& e) {
          std::cerr << e.what() << '\n';
        }
    }

    return result;
}


template <class DATA_TYPE> std::pair<float, float> RecoBase<DATA_TYPE>::getTimeRange(size_t p) 
{
  static std::pair<float, float> returnNull;
  if (p >= _geoService.Nplanes() ) {
    std::cerr << "ERROR: Request for nonexistent plane " << p << std::endl;
    return returnNull;
  }
  else {
    try {
      return _timeRange.at(p);
    }
    catch (const std::exception& e) {
      std::cerr << e.what() << '\n';
      return returnNull;
    }
  }
}

template <class DATA_TYPE> const std::vector<DATA_TYPE> & RecoBase<DATA_TYPE>::getDataByPlane(size_t p) 
{
  static std::vector<DATA_TYPE> returnNull;
  if (p >= _geoService.Nplanes() ) {
    std::cerr << "ERROR: Request for nonexistent plane " << p << std::endl;
    return returnNull;
  }
  else {
    try {
      return _dataByPlane.at(p);
    }
    catch (const std::exception& e) {
      std::cerr << e.what() << '\n';
      return returnNull;
    }
  }
}

template <class DATA_TYPE> Point2D
    RecoBase<DATA_TYPE>::Point_3Dto2D(float x, float y, float z, unsigned int plane) const 
{
    //initialize return value
    Point2D returnPoint;
  
    // Make a check on the plane:
    if (plane > _geoService.Nplanes()) {
        std::cerr << "ERROR: Can't project 3D point to unknown plane " << plane << std::endl;
        return returnPoint;
    }
  
    // Verify that the point is in the TPC before trying to project:
  
    // The wire position can be gotten with Geometry::NearestWire()
    // Convert result to centimeters as part of the unit convention
    // Previously used nearest wire functions, but they are
    // slightly inaccurate
    // If you want the nearest wire, use the nearest wire function!
    returnPoint.w = _geoService.WireCoordinate(y, z, plane, 0, 0) * _geoService.WirePitch(plane);
    // std::cout << "wire is " << returnPoint.w << " (cm)" << std::endl;
  
    // The time position is the X coordinate, corrected for
    // trigger offset and the offset of the plane
    // auto detp = DetProperties::GetME();
    returnPoint.t = x;
    // Add in the trigger offset:
    // (Trigger offset is time that the data is recorded
    // before the actual spill.
    // So, it moves the "0" farther away from the actual
    // time and is an addition)
    // returnPoint.t += detp -> TriggerOffset() * fTimeToCm;
    // std::cout << "trigger offset, plane " << plane
    //           << ": " << detp -> TriggerOffset() * fTimeToCm << std::endl;
    //
    //Get the origin point of this plane:
    // Double_t planeOrigin[3];
    // _geoService.PlaneOriginVtx(plane, planeOrigin);
    // Correct for the origin of the planes
    // X = 0 is at the very first wire plane, so the values
    // of the coords of the planes are either 0 or negative
    // because the planes are negative, the extra distance
    // beyond 0 needs to make the time coordinate larger
    // Therefore, subtract the offest (which is already
    // in centimeters)
    // returnPoint.t -= planeOrigin[0];
  
    // Set the plane of the Point2D:
    returnPoint.plane = plane;
  
    return returnPoint;
}

template <class DATA_TYPE> 
    void RecoBase<DATA_TYPE>::Line_3Dto2D( const TVector3& startPoint3D, const TVector3& direction3D, unsigned int plane,
                                           Point2D& startPoint2D, Point2D& direction2D) const
{
  // First step, project the start point into 2D
  startPoint2D = Point_3Dto2D(startPoint3D.X(), startPoint3D.Y(), startPoint3D.Z(), plane);
  //if (! Point_isInTPC( startPoint3D ) ) {
  //  std::cerr << "ERROR - GeometriaHelper::Line_3Dto2D: StartPoint3D must be in the TPC.\n";
  //  return;
  //}
  // Next, get a second point in 3D:
  float alpha = 10;
  TVector3 secondPoint3D = startPoint3D + alpha * direction3D;
  while ( ! Point_isInTPC(secondPoint3D) ) {
    alpha *= -0.75;
    secondPoint3D = startPoint3D + alpha * direction3D;
  }

  // std::cout << "3D line is (" << startPoint3D.X() << ", " << startPoint3D.Y()
  //           << ", " << startPoint3D.Z() << ") to ( " << secondPoint3D.X()
  //           << ", " << secondPoint3D.Y() << ", " << secondPoint3D.Z() << ")\n";

  // Project the second point into 2D:
  Point2D secondPoint2D = Point_3Dto2D(secondPoint3D.X(), secondPoint3D.Y(), secondPoint3D.Z(), plane);

  // std::cout << "2D line is (" << startPoint2D.w << ", " << startPoint2D.t
  //           << ") to (" << secondPoint2D.w << ", " << secondPoint2D.t << ")\n";

  // Now we have two points in 2D.  Get the direction by subtracting, and normalize
  TVector2 dir(secondPoint2D.w - startPoint2D.w, secondPoint2D.t - startPoint2D.t);
  if (dir.X() != 0.0 || dir.Y() != 0.0 )
    dir *= 1.0 / dir.Mod();
  direction2D.w = dir.X();
  direction2D.t = dir.Y();
  direction2D.plane = plane;

  return;
}

template <class DATA_TYPE> float RecoBase<DATA_TYPE>::Slope_3Dto2D(float x, float y, float z, unsigned int plane) const 
{
  // Do this by projecting the line:
  // Generate a start point right in the middle of the detector:
  TVector3 startPoint3D(0, 0, 0);
  TVector3 inputVector(x, y, z);
  startPoint3D[2] = 0.5 * _geoService.DetLength();
  Point2D p1, slope;
  Line_3Dto2D(startPoint3D, inputVector, plane, p1, slope);
  return slope.t / slope.w;
}

template <class DATA_TYPE> bool RecoBase<DATA_TYPE>::Point_isInTPC(const TVector3 & pointIn3D) const 
{
    float timeToCm = _detProp.SamplingRate() / 1000.0 * _detProp.DriftVelocity(_detProp.Efield(), _detProp.Temperature());

    // Check against the 3 coordinates:
    if (pointIn3D.X() > _geoService.DetHalfWidth() + _detProp.TriggerOffset() * timeToCm
        || pointIn3D.X() < - _geoService.DetHalfWidth() - _detProp.TriggerOffset() * timeToCm)
    {
      return false;
    }
    if (pointIn3D.Y() > _geoService.DetHalfHeight() || pointIn3D.Y() < - _geoService.DetHalfHeight() )
    {
      return false;
    }
    if (pointIn3D.Z() > _geoService.DetLength() || pointIn3D.Z() < 0.0)
    {
      return false;
    }
    return true;
}


} // evd

#endif
/** @} */ // end of doxygen group

