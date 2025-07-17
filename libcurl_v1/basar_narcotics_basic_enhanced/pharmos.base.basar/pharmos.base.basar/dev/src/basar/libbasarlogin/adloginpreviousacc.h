#ifndef GUARD_ACC_ADLOGINPREVIOUS_H
#define GUARD_ACC_ADLOGINPREVIOUS_H

//----------------------------------------------------------------------------
#include "libbasardbaspect_macros.h"

//----------------------------------------------------------------------------
//! namespace for previous-accessor
namespace acc_adloginprevious
{

//---------------------------------------------------------------------------
//! class for selecting previous data from pploginprevious \n final class
BEGIN_QUERY_BUILDER_DECLARATION(SelPrevious)
END_BUILDER_DECLARATION

//---------------------------------------------------------------------------
//! class for deleting previous login info from currentuserlogin \n final class
BEGIN_WRITE_BUILDER_DECLARATION(DeletePreviousLogin)
END_BUILDER_DECLARATION

BEGIN_WRITE_BUILDER_DECLARATION(DeletePreviousDefaults)
END_BUILDER_DECLARATION

//---------------------------------------------------------------------------
//! class for saving current user login info to currentuserlogin \n final class
BEGIN_WRITE_BUILDER_DECLARATION(SaveCurrentLogin)
END_BUILDER_DECLARATION

BEGIN_WRITE_BUILDER_DECLARATION(SaveDefaults)
END_BUILDER_DECLARATION
//---------------------------------------------------------------------------
//! class for saving previous data to pploginprevious  \n final class
class SavePrevious: public basar::db::aspect::SQLWriteStringBuilder
{
public:
	/*! create new SQLWriteStringBuilder */
    //! \return shared_ptr to StringBuilder-instance
	virtual const boost::shared_ptr<basar::db::aspect::SQLStringBuilder> create() const;
protected:
	/*! check if statement is ready for execution */
    //! \return bool indicating if statement is executeable (= true )
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
