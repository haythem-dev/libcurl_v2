//----------------------------------------------------------------------------
/*! \file
 *  \brief  resultset multiple rows Informix implementation class
 *  \author Michael Eichenlaub
 *  \date   21.08.2005
 */
//----------------------------------------------------------------------------

#ifndef GUARD_RESULTSETINFXMULTI_H
#define GUARD_RESULTSETINFXMULTI_H

//----------------------------------------------------------------------------
#include "resultsetinfx.h"

//----------------------------------------------------------------------------
namespace basar	{
namespace db	{
namespace sql	{

//----------------------------------------------------------------------------
//! \brief resultset multiple rows Informix implementation class 
//! \n     base class
//! 
//! When ResultsetInfxMulti object is constructed, a private open() is called.\n
//! i.e.: cursor will immediately be opened; so statements with "order by" clause 
//! can last some time. But this results in avoiding time penalties at calling next(), 
//! because an opened cursor can be assumed (no need for opened cursor checking).
class ResultsetInfxMulti : public ResultsetInfx
{
public:

	// --- method section ----------------------------------------------------

	//! create resultset implementation object								\n no-throw
	static ResultsetSharedPtr	create				(ConnectionSharedPtr pConn		,	//!< shared pointer to connection implementation object
													 const VarString &   rStmtId	,	//!< statement ID
													 const VarString &   rCursorId	,	//!< cursor ID: defined by statement
													 CursorTypeEnum		 eCursorType,	//!< cursor type
													 ::sqlda *	         pDescIn = 0	//!< sql descriptor area for input data -> parameterized values
													);
	
	//! close and destruct resultset implementation object					\n no-throw
	virtual					   ~ResultsetInfxMulti	();

	//! delete/update on the current row											\n no-throw
	virtual Int32				execCurrentRow		(const VarString & rSql //!< statement for delete/update
													);

	//! move to the next row in the result set								\n no-throw
	virtual bool				next				();
	//! release resultset resources											\n no-throw
	virtual void				close				();
	//! reopen previously opened resultset									\n no-throw
	virtual void				reopen				();


private:

	// --- method section ----------------------------------------------------

	//! construct implementation object by connection reference, declare and open cursor \n no-throw
								ResultsetInfxMulti	(ConnectionSharedPtr pConn      ,	//!< shared pointer to connection implementation object
													 const VarString &   rStmtId    ,	//!< statement ID
													 const VarString &   rCursorId	,	//!< cursor ID: defined by statement
													 CursorTypeEnum		 eCursorType,	//!< cursor type
													 ::sqlda *	         pDescIn		//!< sql descriptor area for input data -> parameterized values
													);


								ResultsetInfxMulti	();									//!< forbidden
								ResultsetInfxMulti	(const ResultsetInfxMulti & rCopy);	//!< forbidden
	ResultsetInfxMulti &    	operator =			(const ResultsetInfxMulti & rCopy);	//!< forbidden


	//! private open cursor													\n no-throw
	void						privateOpenCurs		();

	//! declare cursor														\n no-throw
	void						declareCurs			();
	//! free cursor															\n no-throw
	void						freeCurs			();
	//! open cursor															\n no-throw
	void						openCurs			();
	//! close cursor														\n no-throw
	void						closeCurs			();

	// --- member section ----------------------------------------------------

	VarString					m_CursorId;			//!< application's unique resultset (= cursor) ID (i.e. object address)
	CursorTypeEnum	            m_eCursorType;		//!< cursor type: behaviour for transaction (needed for reopen())
	::sqlda *					m_pDescIn;			//!< sql descriptor area for input data -> parameterized values
};

//----------------------------------------------------------------------------
}	// namespace sql
}	// namespace db
}	// namespace basar

#endif	// GUARD
