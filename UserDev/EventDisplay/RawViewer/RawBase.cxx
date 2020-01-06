#ifndef RAWBASE_CXX
#define RAWBASE_CXX

#include "RawBase.h"

namespace evd {

RawBase::RawBase(const geo::GeometryCore& geometry, const detinfo::DetectorProperties& detectorProperties) :
  geoService(geometry),
  detProp(detectorProperties)
{
    _import_array();
}

RawBase::~RawBase() {}


const std::vector<float> & RawBase::getDataByPlane(unsigned int p) const 
{
    static std::vector<float> returnNull;
    if (p >= geoService.Nplanes()) 
    {
        std::cerr << "ERROR: Request for nonexistant plane " << p << std::endl;
        return returnNull;
    }
    else 
    {
        try {
            return _planeData.at(p);
        }
        catch ( ... ) {
            std::cerr << "WARNING:  REQUEST FOR PLANE FOR WHICH THERE IS NOT WIRE DATA.\n";
            return returnNull;
        }
    }
}

bool RawBase::fileExists(std::string s)
{
    struct stat buffer;   
    return (stat (s.c_str(), &buffer) == 0); 
}

PyObject* RawBase::getArrayByPlane(unsigned int p) 
{
    std::cout << "Recovering plane data for plane: " << p << std::endl;

    PyObject* result = nullptr;

    if (p >= geoService.Nplanes()) 
        std::cerr << "ERROR: Request for nonexistant plane " << p << std::endl;
    else 
    {
        // Convert the wire data to numpy arrays:
        int n_dim = 2;
        long int dims[2];
        dims[0] = _x_dimensions[p];
        dims[1] = _y_dimensions[p];
        int data_type = NPY_FLOAT; //PyArray_FLOAT;
  
        std::cout << "--> returning PyObject with n_dim: " << n_dim << ", dims: " << dims[0] << "/" << dims[1] << ", data_type: " << data_type << ", size data: " << _planeData[p].size() << std::endl;
        std::cout << "    size of planedata for plane: " << p << " is " << _planeData[p].size() << ", value at plane data: " << _planeData[p].data() << std::endl;
  
        //PyArrayObject* result = PyArray_FromDimsAndData(n_dim, dims, data_type, (char*)_planeData[p].data() );
        result = PyArray_SimpleNewFromData(n_dim, dims, NPY_FLOAT, _planeData[p].data());
  
        std::cout << "Did I even return?" << std::endl;
        std::cout << "    >> created PyObject, pointer: " << result << std::endl;
    }

  return result;
}


void RawBase::setXDimension(unsigned int x_dim, unsigned int plane) 
{
    if (_x_dimensions.size() < plane + 1) _x_dimensions.resize(plane + 1);
    
    _x_dimensions.at(plane) = x_dim;

    return;
}
void RawBase::setYDimension(unsigned int y_dim, unsigned int plane) 
{
    if (_y_dimensions.size() < plane + 1) _y_dimensions.resize(plane + 1);

    std::cout << "***** Setting y dim for plane " << plane << " to " << y_dim << std::endl;
    _y_dimensions.at(plane) = y_dim;

    return;
}

void RawBase::setPedestal(float pedestal, unsigned int plane) 
{
    if (_pedestals.size() < plane + 1) _pedestals.resize(plane + 1);
  
    _pedestals.at(plane) = pedestal;

    return;
}


void RawBase::initDataHolder() 
{
    _planeData.resize(_x_dimensions.size());

    for (size_t i = 0; i < _x_dimensions.size(); i ++ ) 
    {
      _planeData.at(i).resize(_x_dimensions.at(i) * _y_dimensions.at(i));
    }
    return;
}


} // evd


#endif
