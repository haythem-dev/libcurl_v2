//----------------------------------------------------------------------------
/*! \file
 *  \brief  string builder definition for central article accessor
 *  \author Roland Kiefert
 *  \date   03.04.2006
 */
//-------------------------------------------------------------------------------------------------//
// includes
//-------------------------------------------------------------------------------------------------//
#include "commonheader.h"
#include "acc_customer.h"
#include "loggerpool.h"

//-------------------------------------------------------------------------------------------------//
// namespaces
//-------------------------------------------------------------------------------------------------//
namespace acc_customer
{

//---------------------------------------------------------------------------
BUILDER_DEFINITION(SelCustomer)
	
ENSURE_PROPERTIES_ARE_SET(SelCustomer,"customerno; branchno;")
	
void SelCustomer::buildSQLString()
{
	resolve(
		"SELECT idfnr as customerno, nameapo as customername, ort as customerlocation, "
			"telnr as telno, telkurzwahl as quickdialing, postleitzahl as postcode, strasse as street "
		"FROM kunde "
		"WHERE filialnr=#branchno# AND idfnr=#customerno#");
	BLOG_TRACE( narcotics::LoggerPool::loggerDomModules, getSQLString() );
}

//---------------------------------------------------------------------------
    BUILDER_DEFINITION(SelCustomerForReprint)

    ENSURE_PROPERTIES_ARE_SET(SelCustomerForReprint, "customersupplierno; branchno; purchaseorderno; transactiondate;")

    void SelCustomerForReprint::buildSQLString()
    {
        resolve ("SELECT first 1 n.customersupplierno as customerno, n.customersuppliername as customername, n.customersuppliercity as customerlocation, "
            "case when n.customersupplierstreet = '' then k.strasse else n.customersupplierstreet end as street, "
			"case when n.customersuppliercip = '' then k.postleitzahl else n.customersuppliercip end  as postcode, k.telnr as telno, k.telkurzwahl as quickdialing "
            "FROM kunde k, narctransactioncatalog n "
            "WHERE k.kundennr = n.customersupplierno and k.kundennr = #customersupplierno# AND k.vertriebszentrumnr = #branchno# and n.branchno = #branchno# "
			"and n.purchaseorderno = #purchaseorderno# and n.transactiondate = #transactiondate# and deletedflag = '0'");

        BLOG_TRACE(narcotics::LoggerPool::loggerDomModules, getSQLString());
    }

//---------------------------------------------------------------------------
BUILDER_DEFINITION(SelCustomerByName)
	
ENSURE_PROPERTIES_ARE_SET(SelCustomerByName,"customername; branchno;")
	
void SelCustomerByName::buildSQLString()
{
	resolve(
		"SELECT k.idfnr as customerno, TRIM(k.nameapo) || ' ' || TRIM(k.postleitzahl) || ' ' || TRIM(k.ort) AS customername "
		"FROM kunde k ");
	
	if( !isContainedAndSet(constants::SHOW_LOCKED_CUST) || getPropertyList().getString(constants::SHOW_LOCKED_CUST) == "0" )
	{
		resolve(", kundeschalter s WHERE k.filialnr=#branchno# and k.filialnr=s.vertriebszentrumnr and k.idfnr=s.idfnr and s.skdkundeaegesperrt=0 ");
	}
	else
	{
		resolve("WHERE k.filialnr=#branchno# ");
	}

	if( isContainedAndSet(constants::CUSTOMERNAME) )
	{
		resolve("AND k.nameapo LIKE '#customername#%' ");
	}
	literal("ORDER BY customername");
	BLOG_TRACE( narcotics::LoggerPool::loggerDomModules, getSQLString() );
}

//---------------------------------------------------------------------------
BUILDER_DEFINITION(SelCustomerMapping)
	
ENSURE_PROPERTIES_ARE_SET(SelCustomerMapping,"branchno;")
	
void SelCustomerMapping::buildSQLString()
{
	resolve(
		"select  customernoextern, e.nameapo as name_extern, e.strasse as street_extern, e.postleitzahl as cip_extern, TRIM(e.ort) as location_extern, "
				"customernointern, i.nameapo as name_intern, i.strasse as street_intern, i.postleitzahl as cip_intern, TRIM(i.ort) as location_intern, "
				"switchaddressflag from customernomapping m, kunde i, kunde e "
				"where m.branchno = #branchno# "
					"and m.branchno = i.vertriebszentrumnr "
					"and m.branchno = e.vertriebszentrumnr "
					"and m.customernoextern = e.idfnr "
					"and m.customernointern = i.idfnr ");
	
	if( isContainedAndSet(constants::CUSTOMERSUPPLIERNO) )
	{
		resolve(" and (customernoextern = #customersupplierno# or customernointern = #customersupplierno#) ");
	}

	literal(" ORDER BY name_extern ");
	
	BLOG_TRACE( narcotics::LoggerPool::loggerDomModules, getSQLString() );
}

//---------------------------------------------------------------------------
BUILDER_DEFINITION(DelCustomerMapping)
	
ENSURE_PROPERTIES_ARE_SET(DelCustomerMapping,"branchno;customernoextern;customernointern;")
	
void DelCustomerMapping::buildSQLString()
{
	resolve(
		"DELETE from customernomapping where branchno = #branchno# "
		"and customernoextern = #customernoextern# "
		"and customernointern = #customernointern# ");
	
	BLOG_TRACE( narcotics::LoggerPool::loggerDomModules, getSQLString() );
}

//---------------------------------------------------------------------------
BUILDER_DEFINITION(InsCustomerMapping)
	
	ENSURE_PROPERTIES_ARE_SET_AND_ONE_IN_STATE(InsCustomerMapping,"branchno;customernoextern;customernointern;switchaddressflag;", basar::SS_INSERT)
	
void InsCustomerMapping::buildSQLString()
{
	resolve(
		"INSERT INTO customernomapping (branchno, customernoextern, customernointern, switchaddressflag) "
		"VALUES (#branchno#, #customernoextern#, #customernointern#, #switchaddressflag#) ");
	
	BLOG_TRACE( narcotics::LoggerPool::loggerDomModules, getSQLString() );
}

//---------------------------------------------------------------------------
BUILDER_DEFINITION(UpdCustomerMapping)
	
ENSURE_PROPERTIES_ARE_SET_AND_ONE_IN_STATE(UpdCustomerMapping,"branchno;customernoextern;customernointern;switchaddressflag;", basar::SS_UPDATE)
	
void UpdCustomerMapping::buildSQLString()
{
	resolve(
		"UPDATE customernomapping SET switchaddressflag = #switchaddressflag# where branchno = #branchno# "
		"and customernoextern = #customernoextern# "
		"and customernointern = #customernointern# ");
	
	BLOG_TRACE( narcotics::LoggerPool::loggerDomModules, getSQLString() );
}


//---------------------------------------------------------------------------
BEGIN_ACCESSOR_DEFINITION("Customer")

	PROPERTY_DESCRIPTION_LIST("int16 branchno; int32 customerno; string customername; string customerlocation; "
								"string telno; string quickdialing; string postcode; string street; string customeraddress; "
								
								"int32 customernoextern; string name_extern; string street_extern; string cip_extern; string location_extern; "
								"int32 customernointern; string name_intern; string street_intern; string cip_intern; string location_intern; "
								"int16 switchaddressflag; string printed_adress;" )
	
	ACCESS_METHOD("SelCustomer")
		SQL_BUILDER_CREATE(SelCustomer)
		SQL_BUILDER_PUSH_BACK(SelCustomer)

	ACCESS_METHOD("SelCustomerForReprint")
		SQL_BUILDER_CREATE(SelCustomerForReprint)
		SQL_BUILDER_PUSH_BACK(SelCustomerForReprint)		

	ACCESS_METHOD("SelCustomerByName")
		SQL_BUILDER_CREATE(SelCustomerByName)
		SQL_BUILDER_PUSH_BACK(SelCustomerByName)

	ACCESS_METHOD("SelCustomerMapping")
		SQL_BUILDER_CREATE(SelCustomerMapping)
		SQL_BUILDER_PUSH_BACK(SelCustomerMapping)

	ACCESS_METHOD("DelCustomerMapping")
		SQL_BUILDER_CREATE(DelCustomerMapping)
		SQL_BUILDER_PUSH_BACK(DelCustomerMapping)	

	ACCESS_METHOD("SaveCustomerMapping")
		SQL_BUILDER_CREATE(InsCustomerMapping)
		SQL_BUILDER_PUSH_BACK(InsCustomerMapping)
		SQL_BUILDER_CREATE(UpdCustomerMapping)
		SQL_BUILDER_PUSH_BACK(UpdCustomerMapping)
		
END_ACCESSOR_DEFINITION
				
} //namespace acc_customer