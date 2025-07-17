//----------------------------------------------------------------------------
/*! \file
 *  \brief  SQL diagnostics area's info implementation class
 *  \author Michael Eichenlaub
 *  \date   11.08.2005
 */
//----------------------------------------------------------------------------

#ifndef GUARD_SQLDIAGNOSTICS_H
#define GUARD_SQLDIAGNOSTICS_H

//------------------------------------------------------------------------------
#include <deque>
#include "libbasar_definitions.h"
#include "libbasarcmnutil_bstring.h"

//----------------------------------------------------------------------------
namespace basar	{
namespace db	{
namespace sql	{

//----------------------------------------------------------------------------
//! SQL diagnostics area's info implementation class \n base class
class SqlDiagnostics
{
public:
	//! destructs implementation object			\n no-throw
	virtual					~SqlDiagnostics	();	
	
	//! write all exceptions in caller string			\n no-throw
	void					getAllExcepts	(VarString & outString	//!< string where exceptions are written to
											) const;	

	//! moves to specific exception						\n OutOfRangeIndexException
	void					absolute		(Int32 exceptIndex	//!< exception index (0, ..., n-1) where to move to
											) const;

	Int32					getCountExcepts	() const;	//!< get number of current SQL exceptions			\n no-throw

	const VarString &		getDbServer		() const;	//!< get name of database server					\n no-throw
	const VarString &		getConnId		() const;	//!< get ID of connection							\n no-throw

	const VarString &		getSqlState		() const;	//!< get SQLSTATE value								\n no-throw
	BLong					getSqlCode		() const;	//!< get DB specific exception code SQLCODE			\n no-throw
	const VarString &		getMsgTxt  		() const;	//!< get message text to describe exception			\n no-throw


protected:
							SqlDiagnostics	();								//!< initialize \n no-throw	
							SqlDiagnostics	(const SqlDiagnostics & rCopy);	//!< forbidden
	SqlDiagnostics &		operator =		(const SqlDiagnostics & rCopy);	//!< forbidden

	//! requests total exceptions number and all Informix SQL exceptions \n no-throw
	virtual void			init			() = 0;


	Int32											m_CountExcepts;			//!< number of current SQL exceptions
	VarString										m_DbServer;				//!< database server name
	VarString										m_ConnId;				//!< connection ID

	
	struct DiagnosticInfo				//! diagnostics info
	{
		BLong			sqlCode;		//!< DB-specific exception code SQLCODE
		VarString		sqlState;		//!< SQLSTATE value
		VarString		msgTxt;			//!< message text to describe exception 
	};

	typedef std::deque <DiagnosticInfo>				CollDiagnosticInfo;		//!< collection definition with DiagnosticsInfo

	CollDiagnosticInfo								m_CollDiagInfo;			//!< collection with DiagnosticsInfo
	mutable CollDiagnosticInfo::const_iterator		m_ItDiagInfo;			//!< iterator for current DiagnosticsInfo
};

//----------------------------------------------------------------------------
}	// namespace sql
}	// namespace db
}	// namespace basar

#endif	// GUARD
