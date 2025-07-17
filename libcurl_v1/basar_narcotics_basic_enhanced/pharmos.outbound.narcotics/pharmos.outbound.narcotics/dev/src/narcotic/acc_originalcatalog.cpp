//----------------------------------------------------------------------------
/*! \file
 *  \brief  string builder definition for OrgCatalog-accessordef & registration
 *  \author Roland Kiefert
 *  \date   23.03.2006
 */
//----------------------------------------------------------------------------
//includes
//----------------------------------------------------------------------------
#include "commonheader.h"
#include "acc_originalcatalog.h"

//----------------------------------------------------------------------------
//namespace
//----------------------------------------------------------------------------
namespace acc_originalcatalog
{

//---------------------------------------------------------------------------
BUILDER_DEFINITION(SelCat)
	
	ENSURE_PROPERTIES_ARE_SET(SelCat,"branchno;")
	
	void SelCat::buildSQLString()
	{
		resolve("SELECT originalid, branchno, articleno, artikel_name AS articlename, darreichform AS pharmaform, "
			    "einheit AS packageunit, transactiondate, transactiontime, customersupplierno, transactiontype, "
				"purchaseorderno, qtytransaction, qtyinstock "
				"FROM narctransactionoriginal, artikelzentral "
				"WHERE branchno=#branchno# AND articleno=artikelzentral.artikel_nr ");
		if( getPropertyList().contains(constants::ARTICLENO) && !(getPropertyList().getState(constants::ARTICLENO) == basar::SS_UNSET) )
		{
			basar::VarString res;
			res.format("AND articleno = #%s# ", constants::ARTICLENO); //normal AND "ARTICLENO" = #%s#
			resolve(res);
		}

		if( getPropertyList().contains(constants::PURCHASEORDERNO) && 
			!(getPropertyList().getState(constants::PURCHASEORDERNO) == basar::SS_UNSET) )
		{
			basar::VarString res;
			res.format("AND purchaseorderno = #%s# ", constants::PURCHASEORDERNO); //normal AND "ORDERNO" = #%s#
			resolve(res);
		}

		if( getPropertyList().contains(constants::TODATE) &&
			getPropertyList().contains(constants::FROMDATE)&&
			!(getPropertyList().getState(constants::TODATE) == basar::SS_UNSET || getPropertyList().getState(constants::FROMDATE) == basar::SS_UNSET) )
		{
			basar::VarString res;
			res.format("AND transactiondate BETWEEN #%s# AND #%s#", constants::FROMDATE, constants::TODATE);
			resolve(res);
		}
		else 
		{
			if( getPropertyList().contains(constants::TODATE) && !(getPropertyList().getState(constants::TODATE) == basar::SS_UNSET) )
			{
				basar::VarString res;
				res.format("AND transactiondate <= #%s# ", constants::TODATE);
				resolve(res);
			}
			else if( getPropertyList().contains(constants::FROMDATE) && !(getPropertyList().getState(constants::FROMDATE) == basar::SS_UNSET) )
			{
				basar::VarString res;
				res.format("AND transactiondate >= #%s# ", constants::FROMDATE);
				resolve(res);
			}
		}
		if( getPropertyList().contains(constants::TRANSACTIONTYPE) && !(getPropertyList().getState(constants::TRANSACTIONTYPE) == basar::SS_UNSET) )
		{
			basar::VarString res;
			res.format("AND transactiontype = '#%s#' ", constants::TRANSACTIONTYPE); //eigentlich AND "TRANSACTIONTYPE" = ...
			resolve(res);
		}
		literal("ORDER BY transactiondate desc, transactiontime desc ");
	}

//---------------------------------------------------------------------------
BEGIN_ACCESSOR_DEFINITION("OriginalCatalog")

	PROPERTY_DESCRIPTION_LIST("int32 originalid; int16 branchno; int32 articleno; string articlename; "
							  "string pharmaform; string packageunit; int32 transactiondate; int32 transactiontime; "
							  "int32 customersupplierno; string transactiontype; "
							  "int32 purchaseorderno; int32 qtytransaction; int32 qtyinstock;")
	
	ACCESS_METHOD("SelCatalog")
		SQL_BUILDER_CREATE(SelCat)
		SQL_BUILDER_PUSH_BACK(SelCat)

END_ACCESSOR_DEFINITION
				
} //namespace acc_originalcatalog