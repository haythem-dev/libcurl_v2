//-------------------------------------------------------------------------------------------------//
/*! \file 
 *  \brief  Accessor for producing an empty search It
 *  \author Benjamin Endlicher
 *  \date   24.08.2010
 */
//-------------------------------------------------------------------------------------------------//

// following include directive must be the first include !
// this cpp file must be compiled for all(!) configurations with settings:
//	   /Yu"cmnaccessor.h" /Fp$(IntDir)/cmnaccessor.pch

//-------------------------------------------------------------------------------------------------//
// include section
//-------------------------------------------------------------------------------------------------//
#include "commonheader.h"
#include "acc_search.h"

//-------------------------------------------------------------------------------------------------//
// namespace section
//-------------------------------------------------------------------------------------------------//
namespace acc_search
{

//------------------------------------------------------------------------------------------------------------//
// Accessor Definition ACC_SEARCH
//------------------------------------------------------------------------------------------------------------//
BEGIN_ACCESSOR_DEFINITION( "Search" )		

	PROPERTY_DESCRIPTION_LIST(	"int32 catalogid; int16 branchno; int32 articleno; string articlecode; string articlename; "
								"string pharmaform; string packageunit;" 
                                "int32 transactiondate; int32 transactiontime; "
								"int32 customersupplierno; int32 customerno; string customersuppliername; string customersupplierlocation; "
								"string customersupplierstreet; string customersuppliercip; "
		                        "string transactiontype; int32 purchaseorderno; int32 qtytransaction; int32 qtyinstock; "
								"string deletedflag; string processedflag; string storagelocation; int32 balance; "
								"decimal countofbookings; string narcunit; string narcgroup; string narcname; decimal narcpotency; decimal narcfactor; "
								"int32 contributionvoucherno; int16 acknowledgementstatus; string tourid; " )

END_ACCESSOR_DEFINITION

} //namespace acc_search