//----------------------------------------------------------------------------
/*! \file
 *  \brief  insertset rows informix implementation class
 *  \author Roland Kiefert
 *  \date   08.11.2005
 */
//----------------------------------------------------------------------------

#ifndef GUARD_INSERTSETINFX_H
#define GUARD_INSERTSETINFX_H

//----------------------------------------------------------------------------
#include <insertset.h>
#include "libbasarcmnutil_bstring.h"

//----------------------------------------------------------------------------
struct sqlda;

//----------------------------------------------------------------------------
namespace basar	{
namespace db	{
namespace sql	{

//----------------------------------------------------------------------------
//! \brief insertset rows informix implementation class 
//! \n     base class
//! 
//! When InsertsetInfx object is constructed, a private open() is called.\n
//! i.e.: cursor will immediately be opened;
//! Use this insertset for avoiding time penalties by calling put(), flow() 
//! because an opened cursor can be assumed (no need for opened cursor checking).
class InsertsetInfx : public Insertset
{
public:

	// --- method section ----------------------------------------------------

	//! create insertset implementation object								\n no-throw
	static InsertsetSharedPtr	create				(ConnectionSharedPtr pConn      ,	//!< shared pointer to connection implementation object
													 const VarString &   rStmtId    ,	//!< statement ID
													 const VarString &   rCursorId	,	//!< cursor ID: defined by statement
													 CursorTypeEnum		 eCursorType,   //!< cursor type
													 ::sqlda *	         pDescIn = 0	//!< sql descriptor area for input data -> parameterized values
													);
	
	//! close and destruct insertset implementation object					\n no-throw
	virtual					   ~InsertsetInfx		();

	//! release insertset resources											\n no-throw
	virtual void				close				();

	/*! \brief put row to insert set										\n no-throw */
	virtual void				insertRow			();

	//! return the number of rows which were inserted in database			\n no-throw
	virtual UInt32				flushRows			();

private:

	// --- method section ----------------------------------------------------

	//! construct implementation object by connection reference, declaring and opening cursor \n no-throw
								InsertsetInfx		(ConnectionSharedPtr pConn      ,	//!< shared pointer to connection implementation object
													 const VarString &   rStmtId    ,	//!< statement ID
													 const VarString &   rCursorId	,	//!< cursor ID: defined by statement
 													 CursorTypeEnum		 eCursorType,	//!< cursor type
													 ::sqlda *	         pDescIn 		//!< sql descriptor area for input data -> parameterized values
													 );

								InsertsetInfx		();								//!< forbidden
								InsertsetInfx		(const InsertsetInfx & rCopy);	//!< forbidden
	InsertsetInfx &    			operator =			(const InsertsetInfx & rCopy);	//!< forbidden

	//! declare cursor														\n no-throw
	void						declareCurs			(CursorTypeEnum eCursorType		//!< cursor type
													);
	//! free cursor															\n no-throw
	void						freeCurs			();

	//! open cursor															\n no-throw
	void						openCurs			();
	//! close cursor														\n no-throw
	void						closeCurs			();

	//! private insert row													\n no-throw
	void						privateInsertRow	();

	//! set connection to current											\n no-throw
	void						setCurrent			();


	// --- member section ----------------------------------------------------

	VarString					m_StmtId;			//!< application's unique statement ID  (i.e. statement object address)
	VarString					m_CursorId;			//!< application's unique insertset (= cursor) ID (i.e. object address)
	::sqlda *					m_pDescIn;			//!< sql descriptor area for input data -> parameterized values
};

//----------------------------------------------------------------------------
}	// namespace sql
}	// namespace db
}	// namespace basar

#endif	// GUARD
