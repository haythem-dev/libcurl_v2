//-------------------------------------------------------------------------------------------------------//
/*! \file
 *  \brief  string builder definition for user-accessor + registration of user-accessor
 *  \author Roland Kiefert
 *  \date   17.02.2006
 */
//-------------------------------------------------------------------------------------------------------//

//-------------------------------------------------------------------------------------------------------//
// includes
//-------------------------------------------------------------------------------------------------------//
#include "commonheader.h"
#include "acc_transfer.h"
#include "loggerpool.h"
#include "narctransfer_definitions.h"

//-------------------------------------------------------------------------------------------------------//
// namespace section
//-------------------------------------------------------------------------------------------------------//
namespace acc_transfer
{

//-------------------------------------------------------------------------------------------------------//
//
//-------------------------------------------------------------------------------------------------------//
BUILDER_DEFINITION( SelTransfer )


	ENSURE_PROPERTIES_ARE_SET( SelTransfer,
		lit::narcotics::BRANCHNO.toPropertyString() +
		lit::narcotics::COMPLETEDFLAG.toPropertyString()
	)

	void SelTransfer::buildSQLString()
	{
		resolve("SELECT transferid, branchno, articleno, transactiondate, transactiontime, "
					"DECODE(transactiontype, 'b', "
						"(NVL((SELECT bga_nr FROM pusupplier l WHERE customersupplierno = l.besla_nr "
						" ), 0)), customersupplierno) AS customersupplierno, "
					"DECODE(transactiontype, 'a', "
						"CASE purchaseorderno WHEN 0 THEN "
							"NVL((select kdauftragnrfiliale from kdauftrag where filialnr=branchno and kdauftragnr=orderno ), 0) " // BNR = 0, dann aus kdauftrag holen
							"ELSE purchaseorderno END, purchaseorderno) as purchaseorderno, "
					"transactiontype, qtytransaction, qtyinstock, tourid, warehouseorderno "
				"FROM narctransactiontransfer nt1 "
				"WHERE not exists(select transferid from narctransactiontransfer nt2 where nt2.completedflag < #completedflag# "
				"and nt1.purchaseorderno = nt2.purchaseorderno and nt1.transactiondate=nt2.transactiondate "
				"and nt1.branchno = nt2.branchno and nt1.customersupplierno = nt2.customersupplierno) "
				"AND branchno=#branchno# "
				"ORDER BY branchno, articleno, transactiondate, transactiontime");
		BLOG_TRACE(LoggerPool::loggerDomModules, getSQLString());
	}


//-------------------------------------------------------------------------------------------------------//
//
//-------------------------------------------------------------------------------------------------------//
BUILDER_DEFINITION( DelTransfer )
	ENSURE_PROPERTIES_ARE_SET( DelTransfer,
		lit::narcotics::TRANSFERID.toPropertyString()
	)

	void DelTransfer::buildSQLString()
	{
		resolve("DELETE FROM narctransactiontransfer WHERE transferid=#transferid#");
		BLOG_TRACE(LoggerPool::loggerDomModules, getSQLString());
	}

//-------------------------------------------------------------------------------------------------------//
//
//-------------------------------------------------------------------------------------------------------//
BUILDER_DEFINITION( SelBranchesToTransfer )
	DONT_CHECK_PROPERTIES( SelBranchesToTransfer )

	void SelBranchesToTransfer::buildSQLString()
	{
		resolve("SELECT distinct branchno FROM narctransactiontransfer");
		BLOG_TRACE(LoggerPool::loggerDomModules, getSQLString());
	}

//-------------------------------------------------------------------------------------------------------//
// ACCESSOR DEFINITION
//-------------------------------------------------------------------------------------------------------//
BEGIN_ACCESSOR_DEFINITION( "Transfer" )

	BEGIN_PROPERTY_DESCRIPTION_LIST
		PROPERTY_DESCRIPTION_LIST_ADD(lit::narcotics::CATALOGID)
		PROPERTY_DESCRIPTION_LIST_ADD(lit::narcotics::TRANSFERID)
		PROPERTY_DESCRIPTION_LIST_ADD(lit::narcotics::BRANCHNO)
		PROPERTY_DESCRIPTION_LIST_ADD(lit::narcotics::ARTICLENO)
		PROPERTY_DESCRIPTION_LIST_ADD(lit::narcotics::TRANSACTIONDATE)
		PROPERTY_DESCRIPTION_LIST_ADD(lit::narcotics::TRANSACTIONTIME)
		PROPERTY_DESCRIPTION_LIST_ADD(lit::narcotics::CUSTOMERSUPPLIERNO)
		PROPERTY_DESCRIPTION_LIST_ADD(lit::narcotics::CUSTOMERSUPPLIERNAME)
        PROPERTY_DESCRIPTION_LIST_ADD(lit::narcotics::CUSTOMERSUPPLIERSTREET)
		PROPERTY_DESCRIPTION_LIST_ADD(lit::narcotics::CUSTOMERSUPPLIERCITY)
		PROPERTY_DESCRIPTION_LIST_ADD(lit::narcotics::CUSTOMERSUPPLIERCIP)
		PROPERTY_DESCRIPTION_LIST_ADD(lit::narcotics::TRANSACTIONTYPE)
		PROPERTY_DESCRIPTION_LIST_ADD(lit::narcotics::PURCHASEORDERNO)
		PROPERTY_DESCRIPTION_LIST_ADD(lit::narcotics::QTYTRANSACTION)
		PROPERTY_DESCRIPTION_LIST_ADD(lit::narcotics::QTYINSTOCK)
		PROPERTY_DESCRIPTION_LIST_ADD(lit::narcotics::PROCESSEDFLAG)
		PROPERTY_DESCRIPTION_LIST_ADD(lit::narcotics::NARCGROUP)
		PROPERTY_DESCRIPTION_LIST_ADD(lit::narcotics::NARCNAME)
		PROPERTY_DESCRIPTION_LIST_ADD(lit::narcotics::CONCENTRATION)
		PROPERTY_DESCRIPTION_LIST_ADD(lit::narcotics::PACKAGEUNIT)
		PROPERTY_DESCRIPTION_LIST_ADD(lit::narcotics::NARCFACTOR)
		PROPERTY_DESCRIPTION_LIST_ADD(lit::narcotics::CONTRIBUTIONVOUCHERNO)
		PROPERTY_DESCRIPTION_LIST_ADD(lit::narcotics::ACKNOWLEDGEMENTSTATUS)
		PROPERTY_DESCRIPTION_LIST_ADD(lit::narcotics::TOURID)
		PROPERTY_DESCRIPTION_LIST_ADD(lit::narcotics::WAREHOUSEORDERNO)
	END_PROPERTY_DESCRIPTION_LIST

	//----------------------------------------------------------//
	// SELECTS
	//----------------------------------------------------------//
	ACCESS_METHOD( "SelTransfer" )
		SQL_BUILDER_CREATE( SelTransfer )
		SQL_BUILDER_PUSH_BACK( SelTransfer )

	ACCESS_METHOD( "DelTransfer" )
		SQL_BUILDER_CREATE( DelTransfer )
		SQL_BUILDER_PUSH_BACK( DelTransfer )

	ACCESS_METHOD( "SelBranchesToTransfer" )
		SQL_BUILDER_CREATE( SelBranchesToTransfer )
		SQL_BUILDER_PUSH_BACK( SelBranchesToTransfer )

END_ACCESSOR_DEFINITION

} //namespace acc_transfer