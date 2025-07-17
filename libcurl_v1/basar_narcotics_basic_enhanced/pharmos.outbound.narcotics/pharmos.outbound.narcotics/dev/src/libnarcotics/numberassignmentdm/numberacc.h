//----------------------------------------------------------------------------
/*! \file
 *  \brief  string builder declarations for number-accessor
 *  \author 
 *  \date   
 */
//----------------------------------------------------------------------------

#ifndef GUARD_NUMBERACC_H
#define GUARD_NUMBERACC_H

//----------------------------------------------------------------------------
//! namespace for number-accessor (each accessor has to have it´s own namespace!)
namespace acc_number
{

//---------------------------------------------------------------------------
//! class for selecting internal purchasing order number \n final class
BEGIN_QUERY_BUILDER_DECLARATION(SelectNoForContext)
	static basar::ConstString s_SqlStmt_SelectNoForContext;	//!< select - stmt to find the number for the branch and number typ.
END_BUILDER_DECLARATION

//---------------------------------------------------------------------------
//! class for updating internal purchasing order number \n final class
BEGIN_WRITE_BUILDER_DECLARATION(UpdateNoForContext)
	static basar::ConstString s_SqlStmt_UpdateNoForContext;	//!< update - stmt to update the number for a branch and numbertype.
END_BUILDER_DECLARATION

} //namespace acc_number

#endif	//GUARD_NUMBERACC_H