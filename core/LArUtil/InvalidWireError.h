/**
 * \file InvalidWireError.h
 *
 * \ingroup LArUtil
 * 
 * \brief Class def header for exception class for InvalidWireError
 *
 * @author kazuhiro
 */

/** \addtogroup LArUtil

    @{*/
#ifndef GALLERY_FMWK_INVALIDWIREEXCEPTION_H
#define GALLERY_FMWK_INVALIDWIREEXCEPTION_H

#include <iostream>
//#include <exception>
#include "cetlib_except/exception.h"

namespace larutil {
  /**
     \class InvalidWireError
  */
  class InvalidWireError : public cet::exception{

  public:

    InvalidWireError(std::string msg="") : cet::exception(msg)
    {
      better_wire_number = 0;
      _msg = "\033[93m";
      _msg += msg;
      _msg += "\033[00m";
    }

    virtual ~InvalidWireError() throw(){};
    virtual const char* what() const throw() 
    { return _msg.c_str(); }

    unsigned int better_wire_number;

  private:

    std::string _msg;
  };

}
#endif
/** @} */ // end of doxygen group 

