//----------------------------------------------------------------------------
/*! \file
 *  \brief  string builder definition for central article accessor
 *  \author Roland Kiefert
 *  \date   03.04.2006
 */
//----------------------------------------------------------------------------
// includes
//-------------------------------------------------------------------------------------------------//
#include "commonheader.h"
#include "acc_customersupplier.h"
#include "loggerpool.h"

//-------------------------------------------------------------------------------------------------//
// namespaces
//-------------------------------------------------------------------------------------------------//
namespace acc_customersupplier
{

//---------------------------------------------------------------------------
BUILDER_DEFINITION(SelSupplier)
	
	ENSURE_PROPERTIES_ARE_SET(SelSupplier,"customersupplierno;")
	
	void SelSupplier::buildSQLString()
	{
		//da sich die addresse des lieferanten von der des bga nummer inhabers unterscheiden kann, wird hier die übergebene bganr verwendet
		//aber die zum lieferanten passende addresse gesucht. es sind nicht für alle lieferanten die bganrn gepflegt!!!

        //obiges ist nicht ganz richtig
        //es existieren bestelllieferanten und lieferanten. bestelllieferanten sind diejenigen, bei denen bestellt wird
        //lieferanten diejenigen die liefern. speziell bei der BTM-abgabe beauftragen die bestelllieferanten
        //spezialisierte lieferanten mit den BTM-lieferungen. bei BTM-lieferungen ist allerdings die bga-nr
        //des lieferanten (der die ware liefert) entscheident und nicht die des bestelllieferanten (der schreibt uns nur eine rechnung)
        //in der pusupplier-tabelle sind sowohl bestelllieferanten wie auch lieferanten enthalten
        //im falle eines bestelllieferanten muss für BTMs auch ein lieferant mit bga-nr vorhanden sein
        //um nun zu einer lieferanten bga-nr die zugehörigen daten aus der datenbank zu bekommen
        //ist es hinreichend denjenigen datensatz zu selektieren, bei dem die bga-nr mit der übergebenen übereinstimmt (logisch, oder?)
        //und bei dem die besla_nr (bestelllieferanten nummer, primärschlüssel) mit der liefer_besla_nr (lieferantennummer, genauer der lieferant, der für einen bestelllieferanten ware liefert - 
        //wir erinnern uns, bestelllieferanten "liefern" nur die rechnung) übereinstimmt
        //im falle von gleichheit der besla und liefer_besla ist also der bestelllieferant sein eigener lieferant
        //ob bei den spezialisierten BTM-lieferanten ebenfalls bestellt werden kann, ist in diesem fall irrelevant (wobei ich nicht davon ausgehe, da diese lieferanten keine hersteller sind)

		resolve("SELECT bga_nr as customersupplierno, besla_name as customersuppliername, ort as customersupplierlocation, "
				"strasse AS street, plz_strasse AS postcode "
				"FROM   pusupplier "
				"WHERE  besla_nr = liefer_besla_nr "
				"AND    bga_nr = #customersupplierno# ");
		BLOG_TRACE(narcotics::LoggerPool::loggerDomModules, getSQLString());
	}

//---------------------------------------------------------------------------
    BUILDER_DEFINITION(SelSupplierForPrint)

    ENSURE_PROPERTIES_ARE_SET(SelSupplierForPrint, "customersupplierno;purchaseorderno;transactiondate;branchno;")

    void SelSupplierForPrint::buildSQLString()
    {
        resolve("SELECT first 1 n.customersupplierno, n.customersuppliername, n.customersuppliercity as customersupplierlocation, "
				"case when n.customersupplierstreet = '' then b.strasse else n.customersupplierstreet end AS street, "
				"case when n.customersuppliercip = '' then b.plz_strasse else n.customersuppliercip end as postcode "
				"FROM   pusupplier b, narctransactioncatalog n "
				"WHERE  b.besla_nr = "
                "       (SELECT liefer_besla_nr "
				"        FROM pusupplier "
                "        group by liefer_besla_nr, bga_nr having bga_nr = #customersupplierno#) and b.bga_nr = n.customersupplierno and n.customersupplierno = #customersupplierno# "
                "and n.purchaseorderno = #purchaseorderno# and n.transactiondate = #transactiondate# and n.branchno = #branchno#");
        
        BLOG_TRACE(narcotics::LoggerPool::loggerDomModules, getSQLString());
    }

//---------------------------------------------------------------------------
	BUILDER_DEFINITION(SelCustomer)
	
	ENSURE_PROPERTIES_ARE_SET(SelCustomer,"customersupplierno; branchno;")
	
	void SelCustomer::buildSQLString()
	{
		resolve("SELECT idfnr as customersupplierno, nameapo as customersuppliername, TRIM(ort) as customersupplierlocation, "
				"strasse as street, postleitzahl as postcode "
				"FROM   kunde "
				"WHERE  kundennr = #customersupplierno# "
				"AND    vertriebszentrumnr = #branchno# ");
				
        BLOG_TRACE(narcotics::LoggerPool::loggerDomModules, getSQLString());

	}

//---------------------------------------------------------------------------
    BUILDER_DEFINITION(SelCustomerForPrint)

    ENSURE_PROPERTIES_ARE_SET(SelCustomerForPrint, "customersupplierno; branchno; purchaseorderno; transactiondate")

    void SelCustomerForPrint::buildSQLString()
    {
        resolve ("SELECT first 1 n.customersupplierno, n.customersuppliername, n.customersuppliercity as customersupplierlocation, "
            "case when n.customersupplierstreet = '' then k.strasse else n.customersupplierstreet end as street, "
			"case when n.customersuppliercip = '' then k.postleitzahl else n.customersuppliercip end as postcode "
            "FROM kunde k, narctransactioncatalog n "
            "WHERE k.kundennr = n.customersupplierno and k.kundennr = #customersupplierno# AND k.vertriebszentrumnr = #branchno# and n.branchno = #branchno# "
			"and n.purchaseorderno = #purchaseorderno# and n.transactiondate = #transactiondate# and deletedflag = '0' and acknowledgementstatus = 6 ");

        BLOG_TRACE(narcotics::LoggerPool::loggerDomModules, getSQLString());
    }

//---------------------------------------------------------------------------
BEGIN_ACCESSOR_DEFINITION("CustomerSupplier")

	PROPERTY_DESCRIPTION_LIST(
		"int32 customersupplierno; "
		"string customersuppliername; "
		"string customersupplierlocation; "
		"string postcode; "
		"string street;"
		)
	
	ACCESS_METHOD("SelSupplier")
		SQL_BUILDER_CREATE(SelSupplier)
        SQL_BUILDER_PUSH_BACK(SelSupplier)

	ACCESS_METHOD("SelSupplierForPrint")
		SQL_BUILDER_CREATE(SelSupplierForPrint)
        SQL_BUILDER_PUSH_BACK(SelSupplierForPrint)

   	ACCESS_METHOD("SelCustomerForPrint")
		SQL_BUILDER_CREATE(SelCustomerForPrint)
		SQL_BUILDER_PUSH_BACK(SelCustomerForPrint)


	ACCESS_METHOD("SelCustomer")
		SQL_BUILDER_CREATE(SelCustomer)
		SQL_BUILDER_PUSH_BACK(SelCustomer)
		
END_ACCESSOR_DEFINITION
				
} //namespace acc_customersupplier