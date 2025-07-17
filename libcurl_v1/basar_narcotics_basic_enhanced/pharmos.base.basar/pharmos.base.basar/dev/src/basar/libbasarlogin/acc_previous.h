//----------------------------------------------------------------------------
/*! \file
 *  \brief  string builder declaration for previous-accessor
 *  \author Thomas Hörath
 *  \date   15.12.2005
 */
//----------------------------------------------------------------------------

#ifndef GUARD_ACC_PREVIOUS_H
#define GUARD_ACC_PREVIOUS_H

//----------------------------------------------------------------------------
#include "libbasardbaspect_macros.h"

//----------------------------------------------------------------------------
//! namespace for previous-accessor
namespace acc_previous
{

//---------------------------------------------------------------------------
//! class for selecting previous data from pploginprevious \n final class
BEGIN_QUERY_BUILDER_DECLARATION(SelPrevious)
END_BUILDER_DECLARATION

//---------------------------------------------------------------------------
//! class for saving previous data to pploginprevious  \n final class
class SavePrevious: public basar::db::aspect::SQLWriteStringBuilder
{
public:
	/*! create new SQLWriteStringBuilder */
	virtual const boost::shared_ptr<basar::db::aspect::SQLStringBuilder> create() const;
protected:
	/*! check if statement is ready for execution */
	virtual bool isExecutable() const;
	/*! resolve sql-statement */
	virtual void buildSQLString();
private:
	static basar::ConstString s_SqlStmt;		//!< sql-string 
	static basar::ConstString s_SqlStmt_Upd;	//!< update of ppPrevious
	static basar::ConstString s_SqlStmt_Ins;	//!< insert to ppPrevious
};

//----------------------------------------------------------------------------
} // acc_previous

//----------------------------------------------------------------------------
#endif	// GUARD
