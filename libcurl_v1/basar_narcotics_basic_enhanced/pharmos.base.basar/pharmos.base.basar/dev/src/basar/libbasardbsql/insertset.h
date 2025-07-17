//----------------------------------------------------------------------------
/*! \file
 *  \brief  insertset implementation class
 *  \author Roland Kiefert
 *  \date   08.11.2005
 */
//----------------------------------------------------------------------------

#ifndef GUARD_INSERTSET_H
#define GUARD_INSERTSET_H

//----------------------------------------------------------------------------
#include "sqloperative.h"

//----------------------------------------------------------------------------
namespace basar	{
namespace db	{
namespace sql	{

//----------------------------------------------------------------------------
//! insertset implementation class \n base class
class Insertset : public SqlOperative
{
public:

	// --- method section - object state handling ----------------------------

	//! close and destruct insertset implementation object					\n no-throw
	virtual					   ~Insertset			();

	//! check if insertset is open (i.e. cursor opened)								\n no-throw
	bool						isOpen				() const;
	//! release insertset resources											\n no-throw
	virtual void				close				() = 0;


	// --- method section - row handling -------------------------------------

	//! return number of inserted rows										\n no-throw
	UInt32						getInsertedRows		() const;

	//! set a row to the cursor set											\n no-throw
	virtual void				insertRow			() = 0;

	//! return the number of rows which were inserted in database			\n no-throw
	virtual UInt32				flushRows			() = 0;


protected:

	// --- enum section ------------------------------------------------------

	enum StateEnum				//!< insertset state (important order!)
	{
		STATE_INVALID,			//!< object invalid				(cursor not yet declared/opened)
		STATE_OPENED			//!< object opened				(cursor declared & opened)
	};

	// --- method section ----------------------------------------------------

	//! construct implementation object by connection reference			\n no-throw
								Insertset			(ConnectionSharedPtr pConn			//!< shared pointer to connection implementation object
													);

	//! check connection state                        						\n no-throw
	virtual void				checkConnState		(const VarString & rWhat,	//!< text description 
													 const VarString & rFile,	//!< file where exception is thrown
													 UInt32			   line		//!< line of file where exception is thrown 
													) const;

	// --- member section ----------------------------------------------------

	StateEnum					m_eState;			//!< cursor state
	UInt32						m_InsertedRows;		//!< number of inserted rows
	ConnectionSharedPtr			m_pConn;			//!< shared pointer to connection implementation object



private:

	// --- method section ----------------------------------------------------

								Insertset			();							//!< forbidden
								Insertset			(const Insertset & rCopy);	//!< forbidden
	Insertset &    				operator =			(const Insertset & rCopy);	//!< forbidden
};

//----------------------------------------------------------------------------
}	// namespace sql
}	// namespace db
}	// namespace basar

#endif	// GUARD
