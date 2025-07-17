//----------------------------------------------------------------------------
/*!	\file
 *	\brief	Informix locale	environment	setting	and	restoring
 *	\author	Michael	Eichenlaub
 *	\date	25.03.2009
 */
//----------------------------------------------------------------------------

#ifndef	GUARD_SETENVINFX_H
#define	GUARD_SETENVINFX_H

//----------------------------------------------------------------------------
#include "libbasar_definitions.h"
#include "libbasarcmnutil_bstring.h"

//----------------------------------------------------------------------------
namespace basar	{
namespace db	{
namespace sql	{

//------------------------------------------------------------------------------
class DatabaseInfo;

//------------------------------------------------------------------------------
//!	\brief retrieves presently set Informix	environment	variables\n
//!		   retrieves database locale\n
//!		   sets	appropriate	Informix environment variables\n
//!		   restores	saved Informix environment variables if	dying
class SetEnvInfx
{
public:
	//!	\brief c'tor
			    SetEnvInfx  ();
	//!	\brief d'tor
			   ~SetEnvInfx  ();

	//!	\brief set appropriate Informix	environment
	static void	adjustEnv   (const DatabaseInfo & dbinfo	//!< database specific connection data
						    );
	//!	\brief clear environment (needed for some non-ESQL/C connections (e.g. Informix ODBC)
	static void	clear	    ();

private:
				 SetEnvInfx	(const SetEnvInfx &);	// forbidden
	SetEnvInfx & operator =	(const SetEnvInfx &);	// forbidden

	//!	set	Informix environment locale	(client	& server) variables
	static void	setEnvLocales   (ConstString client,    //!< client	variable to	set
							     ConstString db		    //!< db	variable to	set
							    );
	//!	set	Informix connection	timeout
	static void setEnvTimeout   (ConstString timeout,	//!< timeout variable to set
                                 ConstString retries	//!< retry variable	to set
						        );
    //! set environment \n throw DbExceptionInfx
    static void setEnviron      (char * const assign,   //!< static(!) buffer for environment assignment
                                 ConstString  var   ,   //!< environment variable to set
                                 ConstString  val       //!< environment value to set
                                );

	//!	save previously	set	Informix environment \n	no-throw
	void backup	();
	//!	restore	previously set Informix	environment	\n no-throw
	void restore();

	VarString m_prevClient;	    //!< saved environment variable	for	client locale
	VarString m_prevDb;		    //!< saved environment variable	for	db	   locale
	VarString m_prevTimeout;	//!< saved environment variable	for	timeout
	VarString m_prevRetry;	    //!< saved environment variable	for	retry
};

//----------------------------------------------------------------------------
}	// namespace sql
}	// namespace db
}	// namespace basar

//----------------------------------------------------------------------------
#endif	// GUARD
