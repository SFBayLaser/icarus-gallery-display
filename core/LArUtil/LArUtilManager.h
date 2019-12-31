/**
 * \file LArUtilManager.h
 *
 * \ingroup LArUtil
 * 
 * \brief Class def header for a class LArUtilManager
 *
 * @author kazuhiro
 */

/** \addtogroup LArUtil

    @{*/
#ifndef LARUTILMANAGER_H
#define LARUTILMANAGER_H

#include <iostream>
#include "Geometria.h"
#include "LArProp.h"
#include "DetProperties.h"
#include "GeometriaHelper.h"

#include "Base/messenger.h"

namespace larutil {
  /**
     \class LArUtilManager
     User defined class LArUtilManager ... these comments are used to generate
     doxygen documentation!
  */
  class LArUtilManager  {
    
  private:
    
    /// Default constructor
    LArUtilManager(){};
    
    /// Default destructor
    virtual ~LArUtilManager(){};

    /// Method to execute reconfigure for utilitites
    static bool ReconfigureUtilities();
    
  public:

    /// Method to reconfigure utiities for the provided detector type
    static bool Reconfigure(galleryfmwk::geo::DetId_t type);
    
  };
}

#endif
/** @} */ // end of doxygen group 

