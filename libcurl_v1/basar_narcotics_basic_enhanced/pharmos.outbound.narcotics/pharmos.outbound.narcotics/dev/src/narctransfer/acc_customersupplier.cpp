//----------------------------------------------------------------------------
/*! \file
 *  \brief  string builder definition for central article accessor
 *  \author Roland Kiefert
 *  \date   03.04.2006
 */
//----------------------------------------------------------------------------
//includes
//----------------------------------------------------------------------------
#include "commonheader.h"
#include "acc_customersupplier.h"
#include "narctransfer_definitions.h"
#include "loggerpool.h"

//----------------------------------------------------------------------------
//namespace
//----------------------------------------------------------------------------
namespace acc_customersupplier
{

//---------------------------------------------------------------------------
BUILDER_DEFINITION(SelCustomer)
	
	ENSURE_PROPERTIES_ARE_SET(SelCustomer,
		lit::narcotics::CUSTOMERSUPPLIERNO.toPropertyString() +
		lit::narcotics::BRANCHNO.toPropertyString()
	)
	
	void SelCustomer::buildSQLString()
	{
		resolve(
			"SELECT "
				"vertriebszentrumnr AS branchno, "
				"idfnr AS customersupplierno, "
				"nameapo AS customersuppliername, "
                "strasse AS customersupplierstreet, "
				"TRIM(ort) AS customersuppliercity, "
				"postleitzahl AS customersuppliercip "
			"FROM kunde "
			"WHERE "
				"kundennr=#customersupplierno# AND "
				"vertriebszentrumnr=#branchno#"
		);
		BLOG_TRACE(LoggerPool::loggerDomModules, getSQLString());
	}

//---------------------------------------------------------------------------
BUILDER_DEFINITION(SelSupplier)
	
	ENSURE_PROPERTIES_ARE_SET(SelSupplier,
		lit::narcotics::CUSTOMERSUPPLIERNO.toPropertyString()
	)
	
	void SelSupplier::buildSQLString()
	{
		resolve(
			"SELECT "
				"bga_nr AS customersupplierno, "
				"besla_name AS customersuppliername, "
                "strasse AS customersupplierstreet, "
				"ort AS customersuppliercity, "
				"plz_strasse AS customersuppliercip " 
			"FROM "
				"pusupplier "
			"WHERE "
				"besla_nr=(SELECT DISTINCT liefer_besla_nr FROM pusupplier WHERE bga_nr = #customersupplierno#)"
		);
		BLOG_TRACE(LoggerPool::loggerDomModules, getSQLString());
	}

//---------------------------------------------------------------------------
BEGIN_ACCESSOR_DEFINITION("CustomerSupplier")

	BEGIN_PROPERTY_DESCRIPTION_LIST
		PROPERTY_DESCRIPTION_LIST_ADD(lit::narcotics::BRANCHNO)
		PROPERTY_DESCRIPTION_LIST_ADD(lit::narcotics::CUSTOMERSUPPLIERNO)
		PROPERTY_DESCRIPTION_LIST_ADD(lit::narcotics::CUSTOMERSUPPLIERNAME)
        PROPERTY_DESCRIPTION_LIST_ADD(lit::narcotics::CUSTOMERSUPPLIERSTREET)
		PROPERTY_DESCRIPTION_LIST_ADD(lit::narcotics::CUSTOMERSUPPLIERCITY)
		PROPERTY_DESCRIPTION_LIST_ADD(lit::narcotics::CUSTOMERSUPPLIERCIP)
	END_PROPERTY_DESCRIPTION_LIST

	ACCESS_METHOD("SelCustomer")
		SQL_BUILDER_CREATE(SelCustomer)
		SQL_BUILDER_PUSH_BACK(SelCustomer)

	ACCESS_METHOD("SelSupplier")
		SQL_BUILDER_CREATE(SelSupplier)
		SQL_BUILDER_PUSH_BACK(SelSupplier)
		
END_ACCESSOR_DEFINITION
				
} //namespace acc_customersupplier