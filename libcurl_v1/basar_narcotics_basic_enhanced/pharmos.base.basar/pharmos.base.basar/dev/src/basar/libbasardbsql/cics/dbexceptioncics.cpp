/*
 * @file dbexceptioncics.cpp
 * @brief ...
 * @author Anke Klink
 * @date 2007
 */

//------------------------------------------------------------------------------
#include "libbasardbsql_exceptions.h"

//------------------------------------------------------------------------------
namespace basar {
namespace db    {
namespace sql   {

// ------------------------------------------------------------------------------------------------
//! \return ExceptInfo containing error code
ExceptInfo DbExceptionCics::getExceptInfo(  BULong		      errorcode,
											const VarString & message  ,
											const VarString & file     ,
											const UInt32	  line     )
{
	VarString msg;
	msg.format("error (0x%.8lx): %s", errorcode, message.c_str());
	ExceptInfo info("basar.db.sql.cics", msg, file, line);

	return info;
}

// ------------------------------------------------------------------------------------------------
DbExceptionCics::DbExceptionCics(
    BULong		      errorcode,
									const VarString & message  ,
									const VarString & file     ,
									const UInt32	  line     )
  :	DbException(ExceptInfo(getExceptInfo(
                                errorcode, 
								                                          message, 
																		  file, 
																		  line)),
												 "DbExceptionCics", 
												 "")
{
}

// ------------------------------------------------------------------------------------------------
DbExceptionCicsUnavail::DbExceptionCicsUnavail(
    BULong		      errorcode,
    const VarString & message  ,
    const VarString & file     ,
    const UInt32	  line     )
  :	DbExceptionCics(errorcode, 
                    message, 
                    file, 
                    line)
{
}

// ------------------------------------------------------------------------------------------------
// namespaces
}
}
}

// ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~  ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~
