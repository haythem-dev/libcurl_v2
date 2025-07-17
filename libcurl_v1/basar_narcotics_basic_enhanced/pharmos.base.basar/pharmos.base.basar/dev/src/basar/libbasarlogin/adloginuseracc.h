#ifndef GUARD_ACC_ADLOGINUSER_H
#define GUARD_ACC_ADLOGINUSER_H

//----------------------------------------------------------------------------
#include "libbasardbaspect_macros.h"

//----------------------------------------------------------------------------
//! namespace for user-accessor
namespace acc_adLoginUser
{

//---------------------------------------------------------------------------
//! class for selecting user id from ppuser \n final class
BEGIN_QUERY_BUILDER_DECLARATION(SelUserID)
END_BUILDER_DECLARATION

//---------------------------------------------------------------------------
//! class for selecting user data from ppuser \n final class
BEGIN_QUERY_BUILDER_DECLARATION(SelUserData)
END_BUILDER_DECLARATION

//---------------------------------------------------------------------------
//! class for selecting number of entries in pppasswordprevious for one user \n final class
BEGIN_AGGREGATE_QUERY_BUILDER_DECLARATION(SelNumOfPrevPw)
END_BUILDER_DECLARATION

//---------------------------------------------------------------------------
//! class for selecting earliest date from pppasswordprevious for one user \n final class
BEGIN_AGGREGATE_QUERY_BUILDER_DECLARATION(SelMinDateInPwHistory)
END_BUILDER_DECLARATION

//---------------------------------------------------------------------------
//! class for writing new entry to pppasswordprevious \n final class
BEGIN_WRITE_BUILDER_DECLARATION(InsertPrevPw)
END_BUILDER_DECLARATION

//---------------------------------------------------------------------------
//! class for deleting oldest entry from pppasswordprevious for one user \n final class
BEGIN_WRITE_BUILDER_DECLARATION(DeletePrevPw)
END_BUILDER_DECLARATION

//---------------------------------------------------------------------------
//! class for saving user data to ppuser \n final class
BEGIN_WRITE_BUILDER_DECLARATION(SaveUserData)
END_BUILDER_DECLARATION


//----------------------------------------------------------------------------
} // acc_user

//----------------------------------------------------------------------------
#endif	// GUARD
