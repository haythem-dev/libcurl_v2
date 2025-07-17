//----------------------------------------------------------------------------
/*! \file
 *  \brief  string builder definition for branch accessor
 *  \author Bjoern BIschof
 *  \date   29.06.2006
 */
//----------------------------------------------------------------------------//

//----------------------------------------------------------------------------//
// includes
//----------------------------------------------------------------------------//
#include "commonheader.h"
#include "acc_filiale.h"
#include "loggerpool.h"

//----------------------------------------------------------------------------//
// namespace section
//----------------------------------------------------------------------------//
namespace acc_branch
{

//----------------------------------------------------------------------------//
// accessor
//----------------------------------------------------------------------------//
BEGIN_ACCESSOR_DEFINITION( "Branch" )
	PROPERTY_DESCRIPTION_LIST(	"int16 branchno; string name; string location_short; string location_long; "
								"string street; string postcode; string postofficebox; string postcode_postofficebox;  "
								"string location; string federal_state; string area_code; string telno; "
								"string faxno; int32 bgano; string address;" )	
	ACCESS_METHOD( "SelBranch" )
		SQL_BUILDER_CREATE( SelBranch )
		SQL_BUILDER_PUSH_BACK( SelBranch )
	ACCESS_METHOD( "SelBranchAddress" )
		SQL_BUILDER_CREATE( SelBranchAddress )
		SQL_BUILDER_PUSH_BACK( SelBranchAddress )
		
END_ACCESSOR_DEFINITION

//---------------------------------------------------------------------------
BUILDER_DEFINITION( SelBranch )
	
	ENSURE_PROPERTIES_ARE_SET( SelBranch, "branchno;" )
	
	SQL_PATTERN( SelBranch,	"SELECT filialnr as branchno, name, standort_kurz as location_short, standort_lang as location_long, "
							"		strasse as street, postleitzahl as postcode, postfach as postofficebox,     "
							"		plz_postfach as postcode_postofficebox, ort as location, bundesland as federal_state, "
							"		vorwahl as area_code, telnr as telno, faxnr as faxno, bganr_ek as bgano "
							"FROM	filiale "
							"WHERE	filialnr = #branchno#" )

//---------------------------------------------------------------------------
BUILDER_DEFINITION( SelBranchAddress )
	
	ENSURE_PROPERTIES_ARE_SET( SelBranchAddress, "branchno;" )

	void SelBranchAddress::buildSQLString()
	{
		resolve("SELECT bganr_ek as bgano, 'PHOENIX Pharmahandel GmbH & CoKG ' || TRIM(strasse) || ' ' || TRIM(postleitzahl) || ' ' || TRIM(ort) as address "
				"FROM filiale "
				"WHERE filialnr=#branchno# " );
		BLOG_TRACE( narcotics::LoggerPool::loggerDomModules, getSQLString() );
	}

} //namespace acc_branch