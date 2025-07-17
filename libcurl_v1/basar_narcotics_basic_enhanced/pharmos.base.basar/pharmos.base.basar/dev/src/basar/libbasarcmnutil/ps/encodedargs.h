//----------------------------------------------------------------------------
/*! \file
 *  \brief  encoded arguments for ghostscript
 *  \date   13.07.2015
 */
//----------------------------------------------------------------------------

#ifndef GUARD_ENCODEDARGS_H
#define GUARD_ENCODEDARGS_H

//----------------------------------------------------------------------------
#include "collargs.h"
#include "../libbasarcmnutil_i18nstring.h"

#ifdef _WIN32
#include <memory>
#endif

#ifdef _AIX
#include <boost/scoped_array.hpp>
#endif
//----------------------------------------------------------------------------
namespace basar		{
namespace cmnutil	{

//----------------------------------------------------------------------------
class EncodedArgs
{
public:
    LIBBASARCMNUTIL_API EncodedArgs(const CollArgs & rawargs);

    //! \brief get pointer array of encoded arguments
    char ** get() const;

private:
    //! \brief convert WinCP/ISO coded string to UTF
    static I18nString conv2Utf      (const I18nString & native //!< WinCP/ISO coded string
                                    );
    //! \brief find WINCP/ISO code page regarding basar locale
    static UInt32     getCodepageId ();

    //! \brief find WINCP/ISO code page name regarding basar locale
    static basar::ConstBuffer getCodepageName();
    
    void              setEncodedArgs(const CollArgs & rawargs);
    void              setPtrArray   ();


    //! encoded argument container
    CollArgs                                               m_encodedArgs;
    //! encoded argument pointer array
#ifdef _WIN32
    std::unique_ptr<CollArgs::value_type::value_type * []> m_PtrArray;
#endif
    
#ifdef _AIX
    boost::scoped_array<CollArgs::value_type::value_type *> m_PtrArray;
#endif
};

//----------------------------------------------------------------------------
}	// namespace cmnutil
}	// namespace basar

//----------------------------------------------------------------------------
#endif	// GUARD
