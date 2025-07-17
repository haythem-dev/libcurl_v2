//----------------------------------------------------------------------------
/*! \file
 *  \brief  resultset single row Informix implementation class
 *  \author Michael Eichenlaub
 *  \date   20.08.2005
 */
//----------------------------------------------------------------------------

#ifndef GUARD_RESULTSETINFXSINGLE_H
#define GUARD_RESULTSETINFXSINGLE_H

//----------------------------------------------------------------------------
#include "resultsetinfx.h"

//----------------------------------------------------------------------------
namespace basar	{
namespace db	{
namespace sql	{

//----------------------------------------------------------------------------
//! resultset single row Informix implementation class \n base class
class ResultsetInfxSingle : public ResultsetInfx
{
public:

	// --- method section ----------------------------------------------------

	//! create resultset implementation object								\n no-throw
	static ResultsetSharedPtr	create				(ConnectionSharedPtr pConn  ,		//!< shared pointer to connection implementation object
													 const VarString &   rStmtId,		//!< statement ID
													 ::sqlda *	         pDescIn = 0	//!< sql descriptor area for input data -> parameterized values
													);
	
	//! close and destruct resultset implementation object					\n no-throw
	virtual					   ~ResultsetInfxSingle	();

	//! move to the next row in the result set								\n no-throw
	virtual bool				next				();
	//! release resultset resources											\n no-throw
	virtual void				close				();
	//! reopen previously opened resultset									\n no-throw
	virtual void				reopen				();


private:

	// --- method section ----------------------------------------------------

	//! constructs implementation object by connection reference			\n no-throw
								ResultsetInfxSingle	(ConnectionSharedPtr pConn  ,	//!< shared pointer to connection implementation object
													 const VarString &   rStmtId,	//!< statement ID
													 ::sqlda *	         pDescIn	//!< sql descriptor area for input data -> parameterized values
													);

								ResultsetInfxSingle	();										//!< forbidden
								ResultsetInfxSingle	(const ResultsetInfxSingle & rCopy);	//!< forbidden
	ResultsetInfxSingle &    	operator =			(const ResultsetInfxSingle & rCopy);	//!< forbidden

	//! private execute													\n no-throw
	void						privateExec			();

	::sqlda *					m_pDescIn;			//!< sql descriptor area for input data -> parameterized values
};

//----------------------------------------------------------------------------
}	// namespace sql
}	// namespace db
}	// namespace basar

#endif	// GUARD
