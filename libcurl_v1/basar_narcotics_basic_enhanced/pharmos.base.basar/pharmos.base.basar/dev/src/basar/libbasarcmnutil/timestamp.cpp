
#include "timestamp.h"
#include "libbasarcmnutil_exceptions.h"
#include "libbasarcmnutil_datetimeexception.h"

#include <sqlhdr.h>


//---------------------------------------------------------------------------------------------------------------------//
// namespace section
//---------------------------------------------------------------------------------------------------------------------//
namespace basar {
namespace cmnutil{
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//! \brief sole implemented class constructor, also private
//! \param what     requested initialization of internal buffer
//! \return object instance (duh)
//! \throw DateTimeException
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
Timestamp::Timestamp(populateInitialValuesFor what)
{
	::memset(this, 0, sizeof(Timestamp));

    if (DATETIME == what)
    {
	    m_Qual = TU_DTENCODE(TU_YEAR, TU_F3);

	    char         buf   [32];
	    char         fmtbuf[32];

	    ::sprintf(buf   , "%s%s", nullvalue(0), nullvalue(1) );
	    ::sprintf(fmtbuf, "%s%s", dt_format_date() ,dt_format_time());

	    if ( 0 > ::dtcvfmtasc(const_cast      <char    *>(buf   ),
		                      const_cast      <char    *>(fmtbuf),
						    reinterpret_cast  <dtime_t *>(this  )) )
	    {
		    VarString msg;
		    msg.format("::dtcvfmtasc(format=\"%s\", <%s>) failed", fmtbuf, buf);
		    ExceptInfo sInfo("basar.cmnutil.Timestamp.Timestamp()", msg, __FILE__, __LINE__);
		    throw DateTimeException(sInfo);
	    }
    }
    else if(TIMESPAN_DAYS == what)
    {
        //will do for now :)
        //DONE: use function return values as datetime above
        //DONE: use null value
        //DONE: check return code -> exception
        m_Qual = TU_IENCODE(9, TU_DAY, TU_F3);

	    char         buf   [32];
	    char         fmtbuf[32];

	    ::sprintf(buf   , "%s", NULLVAL_SQL_INTERVAL_SHORT );
	    ::sprintf(fmtbuf, "%s", ts_format_short() );

	    if ( 0 > ::incvfmtasc(const_cast      <char    *>(buf   ),
		                      const_cast      <char    *>(fmtbuf),
						    reinterpret_cast  <intrvl_t*>(this)) )
        {
		    VarString msg;
		    msg.format("::incvfmtasc(format=\"%s\", <%s>) failed", fmtbuf, buf);
		    ExceptInfo sInfo("basar.cmnutil.Timestamp.Timestamp()", msg, __FILE__, __LINE__);
		    throw DateTimeException(sInfo);

        }
    }
}

//----------------------------------------------------------------------------
}	// namespace cmnutil
}	// namespace basar