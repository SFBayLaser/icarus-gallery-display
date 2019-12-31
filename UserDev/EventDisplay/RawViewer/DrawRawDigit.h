/**
 * \file DrawRawDigit.h
 *
 * \ingroup EventViewer
 *
 * \brief Class def header for a class DrawRawDigit
 *
 * @author cadams
 */

/** \addtogroup EventViewer

    @{*/

#ifndef EVD_DRAWRAWDIGIT_H
#define EVD_DRAWRAWDIGIT_H

#include "Analysis/anabase.h"
#include "LArUtil/Geometria.h"
#include "RawBase.h"
#include "UbooneNoiseFilter/UbooneNoiseFilter.h"

#include "TTree.h"
#include "TGraph.h"

#include "canvas/Persistency/Common/FindMany.h"
#include "canvas/Utilities/InputTag.h"
#include "gallery/Event.h"

#include "lardataobj/RawData/RawDigit.h"




struct _object;
typedef _object PyObject;


#ifndef __CINT__
#include "Python.h"
#include "numpy/arrayobject.h"
#endif


namespace evd {
/**
   \class DrawRawDigit
   User custom analysis class made by SHELL_USER_NAME
 */
class DrawRawDigit : public galleryfmwk::anabase, public RawBase {

public:

    /// Default constructor
    DrawRawDigit();

    /// Default destructor
    virtual ~DrawRawDigit() {}

    /** IMPLEMENT in DrawRawDigit.cc!
        Initialization method to be called before the analysis event loop.
    */
    virtual bool initialize();

    /** IMPLEMENT in DrawRawDigit.cc!
        Analyze a data event-by-event
    */
    virtual bool analyze(gallery::Event * event);

    /** IMPLEMENT in DrawRawDigit.cc!
        Finalize method to be called after all events processed.
    */
    virtual bool finalize();

    void SetCorrectData(bool _doit = true) {_correct_data = _doit;}
    void setPadding(size_t padding, size_t plane);


private:

    // Store whether or not to correct the data
    bool _correct_data;

    ub_noise_filter::UbooneNoiseFilter _noise_filter;
    std::vector<size_t> _padding_by_plane;

};
}
#endif

//**************************************************************************
//
// For Analysis framework documentation, read Manual.pdf here:
//
// http://microboone-docdb.fnal.gov:8080/cgi-bin/ShowDocument?docid=3183
//
//**************************************************************************

/** @} */ // end of doxygen group
