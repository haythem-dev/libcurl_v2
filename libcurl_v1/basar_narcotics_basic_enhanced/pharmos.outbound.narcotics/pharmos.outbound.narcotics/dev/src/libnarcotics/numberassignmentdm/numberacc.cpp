//----------------------------------------------------------------------------
/*! \file
 *  \brief  string builder definition for number-accessor + registration of number-accessor
 *  \author 
 *  \date   
 */
//-------------------------------------------------------------------------------------------------------------------------//
// include section
//-------------------------------------------------------------------------------------------------------------------------//
#include "commonheader.h"
#include "numberacc.h"
#include "numberassignmentdm_definitions.h"
#include "loggerpool/loggerpool.h"

//-------------------------------------------------------------------------------------------------------------------------//
// namespace section
//-------------------------------------------------------------------------------------------------------------------------//
namespace acc_number
{

//---------------------------------------------------------------------------
BUILDER_DEFINITION(SelectNoForContext)

	ENSURE_PROPERTIES_ARE_SET(SelectNoForContext,lit::pdm_numberassignment::BRANCH_NO.toPropertyString() + 
												lit::pdm_numberassignment::NUMBERTYPE.toPropertyString() + 
												lit::pdm_numberassignment::TABLENAME.toPropertyString())

   void SelectNoForContext::buildSQLString()
   {
     basar::VarString sqlStatement;
     sqlStatement.append("SELECT ");
     sqlStatement.append("'" + lit::pdm_numberassignment::TABLENAME.toSQLReplacementString() + "' AS "         + lit::pdm_numberassignment::TABLENAME.getName()+ ", ");
     sqlStatement.append("filialnr AS "         + lit::pdm_numberassignment::BRANCH_NO.getName()+ ", ");
     sqlStatement.append("nr AS "               + lit::pdm_numberassignment::NUMBER.getName()+ ", ");
     sqlStatement.append("nr_art AS "           + lit::pdm_numberassignment::NUMBERTYPE.getName()+ ", ");
     sqlStatement.append("nr_maximum AS "       + lit::pdm_numberassignment::NUMBER_MAXIMUM.getName()+ ", ");
     sqlStatement.append("nr_start AS "         + lit::pdm_numberassignment::NUMBER_START.getName()+ ", ");
     sqlStatement.append("ueberlauf_aktion AS " + lit::pdm_numberassignment::OVERFLOW_ACTION.getName()+ " ");
     sqlStatement.append("FROM "                + lit::pdm_numberassignment::TABLENAME.toSQLReplacementString()+ " ");
     sqlStatement.append("WHERE nr_art = '"     + lit::pdm_numberassignment::NUMBERTYPE.toSQLReplacementString() + "' ");
     sqlStatement.append("AND filialnr = "      + lit::pdm_numberassignment::BRANCH_NO.toSQLReplacementString() + " ");
     sqlStatement.append("FOR UPDATE");
     resolve(sqlStatement);
     BLOG_TRACE( narcotics::LoggerPool::loggerDomModules, getSQLString() );
   }
//--- (only for documenting purposes !) ---
	//! \ingroup SQLSTMT
	//! select-statement to find the number for the branch and number typ.
	basar::ConstString 	SelectNoForContext::s_SqlStmt_SelectNoForContext = 
    "SELECT '#tablename#' AS tablename, "
           "filialnr AS branchno, "
           "nr AS number, "
           "nr_art AS numbertype, "
           "nr_maximum AS numbermaximum, "
           "nr_start AS numberstart, "
           "ueberlauf_aktion AS overflowaction "
    "FROM #tablename# "
    "WHERE nr_art = '#numbertype#' "
    "AND filialnr = #branchno# "
    "FOR UPDATE";

//---------------------------------------------------------------------------
BUILDER_DEFINITION(UpdateNoForContext)

ENSURE_PROPERTIES_ARE_SET(UpdateNoForContext,lit::pdm_numberassignment::BRANCH_NO.toPropertyString() + 
												lit::pdm_numberassignment::NUMBERTYPE.toPropertyString() + 
												lit::pdm_numberassignment::TABLENAME.toPropertyString())

void UpdateNoForContext::buildSQLString()
{
  basar::VarString sqlStatement;
  sqlStatement.append("UPDATE " + lit::pdm_numberassignment::TABLENAME.toSQLReplacementString() + " ");
  sqlStatement.append("SET nr=" + lit::pdm_numberassignment::NUMBER.toSQLReplacementString() + " ");
  sqlStatement.append("WHERE filialnr = " + lit::pdm_numberassignment::BRANCH_NO.toSQLReplacementString() + " ");
  sqlStatement.append("AND ");
  sqlStatement.append("nr_art = '" + lit::pdm_numberassignment::NUMBERTYPE.toSQLReplacementString() + "' ");
  resolve(sqlStatement); 
  BLOG_TRACE( narcotics::LoggerPool::loggerDomModules, getSQLString() );
}
//--- (only for documenting purposes !) ---
	//! \ingroup SQLSTMT
	//! update-statement to update the number for a branch and numbertype.
basar::ConstString 	UpdateNoForContext::s_SqlStmt_UpdateNoForContext = 
  "UPDATE #tablename# "
  "SET nr=#number# "
  "WHERE filialnr=#branchno# "
  "AND nr_art='#numbertype#'";

//---------------------------------------------------------------------------
BEGIN_ACCESSOR_DEFINITION("AccNumber")

	BEGIN_PROPERTY_DESCRIPTION_LIST
		PROPERTY_DESCRIPTION_LIST_ADD(lit::pdm_numberassignment::BRANCH_NO);
		PROPERTY_DESCRIPTION_LIST_ADD(lit::pdm_numberassignment::NUMBER);
		PROPERTY_DESCRIPTION_LIST_ADD(lit::pdm_numberassignment::NUMBERTYPE);
		PROPERTY_DESCRIPTION_LIST_ADD(lit::pdm_numberassignment::NUMBER_START);
		PROPERTY_DESCRIPTION_LIST_ADD(lit::pdm_numberassignment::NUMBER_MAXIMUM);
		PROPERTY_DESCRIPTION_LIST_ADD(lit::pdm_numberassignment::OVERFLOW_ACTION);
		PROPERTY_DESCRIPTION_LIST_ADD(lit::pdm_numberassignment::TABLENAME);
	END_PROPERTY_DESCRIPTION_LIST
	
	ACCESS_METHOD("amSelectNoForContext")
		SQL_BUILDER_CREATE(SelectNoForContext)
		SQL_BUILDER_PUSH_BACK(SelectNoForContext)

	ACCESS_METHOD("amUpdateNoForContext")
		SQL_BUILDER_CREATE(UpdateNoForContext)
		SQL_BUILDER_PUSH_BACK(UpdateNoForContext)

END_ACCESSOR_DEFINITION
				
} //namespace acc_number