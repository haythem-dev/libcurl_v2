//----------------------------------------------------------------------------
/*! \file
 *  \brief  global definitions for PDM number assignment
 *  \author Thomas Hoerath
 *  \date   23.01.2008  
 */
//-------------------------------------------------------------------------------------------------------------------------//
// include section
//-------------------------------------------------------------------------------------------------------------------------//

#ifndef GUARD_NUMBERASSIGNMENTDM_DEFINITIONS_H
#define GUARD_NUMBERASSIGNMENTDM_DEFINITIONS_H

#include <libbasarcmnutil_parameterlist.h>
#include <libbasarproperty_propertydescription.h>

//----------------------------------------------------------------------------
namespace pdm_numberassignment
{
	const basar::VarString accountPropTable("AccAccountsInstance");			//!< standard name for the prob table account
	
	const basar::BULong ONE_ROW_AFFECTED = 1;	  //!< constant for affectedRows == 1

	basar::ConstString NARCNUMBER = "narcnumber";
	basar::ConstString OUTPUT = "OUTPUT";
	basar::ConstString DELIVERYNOTECOPYFOLDERNO = "DNCFNO";

  enum OverflowActionMode
  {
    OVERFLOW_ACTION_NONE    = 0, //!< no overflow action defined
    OVERFLOW_ACTION_RESTART = 1  //!< on overflow, start on numberstart
  };
}
	
//----------------------------------------------------------------------------
//!  \brief Namespace containing the definitions made by all domain modules.
namespace lit
{
 
/*!	\brief Namespace containing all necessary defintions for PDM number assignment<BR>
           (fieldnames used for matching / accessing properties + defined values) */
namespace pdm_numberassignment
{
	// property names in alphabetical order
	CONST_PROPERTY_DESCRIPTION(BRANCH_NO,	"branchno", basar::INT16)		//!< field name in property table 
	CONST_PROPERTY_DESCRIPTION(NUMBER,		"number", basar::INT32)		//!< field name in property table 
	CONST_PROPERTY_DESCRIPTION(NUMBERTYPE,	"numbertype", basar::STRING)		//!< field name in property table 
	CONST_PROPERTY_DESCRIPTION(NUMBER_START,	"numberstart", basar::INT32)		//!< field name in property table 
	CONST_PROPERTY_DESCRIPTION(NUMBER_MAXIMUM,	"numbermaximum", basar::INT32)		//!< field name in property table 
	CONST_PROPERTY_DESCRIPTION(OVERFLOW_ACTION,	"overflowaction", basar::INT16)		//!< field name in property table 
	CONST_PROPERTY_DESCRIPTION(TABLENAME,	"tablename", basar::STRING)		//!< field name in property table 
		
} //namespace pdm_numberassignment

} //namespace lit

#endif	//GUARD_NUMBERASSIGNMENTDM_DEFINITIONS_H
