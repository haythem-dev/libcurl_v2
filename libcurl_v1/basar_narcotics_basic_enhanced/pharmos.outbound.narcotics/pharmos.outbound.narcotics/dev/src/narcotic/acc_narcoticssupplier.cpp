//------------------------------------------------------------------------------------------------//
/*! \file
 *  \brief  string builder definition for supplier-accessor
 *  \author Roland Kiefert
 *  \date   17.02.2006
 */
//------------------------------------------------------------------------------------------------//

//------------------------------------------------------------------------------------------------//
// includes
//------------------------------------------------------------------------------------------------/
#include "commonheader.h"
#include "acc_narcoticssupplier.h"
#include "loggerpool.h"

//------------------------------------------------------------------------------------------------/
// namespace section
//------------------------------------------------------------------------------------------------/
namespace acc_narcoticssupplier 
{
	//------------------------------------------------------------------------------------------------/
	//				"FROM liefer l, pusupplier b, fliefer f "
	//---------------------------------------------------------------------------
	BUILDER_DEFINITION(SelSupplier)
		
	ENSURE_PROPERTIES_ARE_SET(SelSupplier,"branchno;")

	SQL_PATTERN(SelSupplier,
					"SELECT distinct b2.bga_nr AS narcoticssupplierno, b1.besla_name AS suppliername, b1.ort AS supplierlocation, b1.strasse AS street, b1.plz_strasse as postcode "
					"FROM pusupplier b1, pusupplier b2 "
					"WHERE b2.lieferspezial='O' AND "
					"b1.besla_nr = b2.liefer_besla_nr "
					"AND b2.bga_nr != 0 AND b2.liefer_besla_nr != 0 "
					"order by 2 ") //besla_name
  
	//------------------------------------------------------------------------------------------------/
	//				"FROM liefer l, pusupplier b, fliefer f "
	//---------------------------------------------------------------------------
	BUILDER_DEFINITION(SelPhoenixSupplier)
		
	ENSURE_PROPERTIES_ARE_SET(SelPhoenixSupplier,"branchno;")

	SQL_PATTERN(SelPhoenixSupplier,
					"SELECT distinct b2.bga_nr AS narcoticssupplierno, b1.besla_name || ' ' || b1.ort AS suppliername, b1.ort AS supplierlocation, b1.plz_strasse as postcode "
					"FROM pusupplier b1, pusupplier b2  "
					"WHERE b2.lieferspezial!='O' AND "
					"b1.besla_nr = b2.liefer_besla_nr "
					"AND b2.bga_nr != 0 AND b2.liefer_besla_nr != 0 "
					"AND b1.besla_name like '%PHOENIX%' "
					"order by 2 ")

	//------------------------------------------------------------------------------------------------/
	// builder SelSupplierByBgaNo
	//------------------------------------------------------------------------------------------------/
	BUILDER_DEFINITION( SelSupplierByBgaNo )
		
	ENSURE_PROPERTIES_ARE_SET( SelSupplierByBgaNo, "narcoticssupplierno;" )

	// aliase?
	SQL_PATTERN(SelSupplierByBgaNo,
					"SELECT bga_nr AS narcoticssupplierno, besla_name AS suppliername, strasse AS street, "
						"plz_strasse AS postcode, ort AS supplierlocation "
					"FROM pusupplier "
					"WHERE besla_nr = liefer_besla_nr "
                    "AND bga_nr = #narcoticssupplierno# ")

	//------------------------------------------------------------------------------------------------/
	// builder SelSupplierByName
	//------------------------------------------------------------------------------------------------/
	BUILDER_DEFINITION( SelSupplierByName )
		
	ENSURE_PROPERTIES_ARE_SET( SelSupplierByName, "suppliername;" )

	// aliase?
	void SelSupplierByName::buildSQLString()
	{
		resolve( "SELECT DISTINCT b1.bga_nr AS narcoticssupplierno, TRIM(b1.besla_name) || ' ' || TRIM(b1.plz_strasse) || ' ' || TRIM(b1.ort) AS suppliername "
				 "FROM pusupplier b1, pusupplier b2  "
				 "WHERE b1.besla_nr = b2.liefer_besla_nr "
				 "AND b2.lieferspezial = 'O' "
				 "AND b2.bga_nr != 0 AND b2.liefer_besla_nr != 0 " );
		if( getPropertyList().contains( constants::SUPPLIERNAME ) && 
			!(getPropertyList().getState( constants::SUPPLIERNAME ) == basar::SS_UNSET) )
		{
			basar::VarString res;
			res.format( "AND b1.besla_mc LIKE '#%s#%c' ", constants::SUPPLIERNAME, '%');
			resolve( res );
		}
		literal("ORDER BY suppliername ");
		BLOG_TRACE( narcotics::LoggerPool::loggerDomModules, getSQLString() );
	}

	//------------------------------------------------------------------------------------------------/
	// accessor definition
	//------------------------------------------------------------------------------------------------/
	BEGIN_ACCESSOR_DEFINITION("NarcoticsSupplier")

		PROPERTY_DESCRIPTION_LIST("int32 narcoticssupplierno; string suppliername; string supplierlocation; "
									"string postcode; string street;")
		
		ACCESS_METHOD("SelSupplier")
			SQL_BUILDER_CREATE(SelSupplier)
			SQL_BUILDER_PUSH_BACK(SelSupplier)

		ACCESS_METHOD("SelPhoenixSupplier")
			SQL_BUILDER_CREATE(SelPhoenixSupplier)
			SQL_BUILDER_PUSH_BACK(SelPhoenixSupplier)

		ACCESS_METHOD("SelSupplierByBgaNo")
			SQL_BUILDER_CREATE(SelSupplierByBgaNo)
			SQL_BUILDER_PUSH_BACK(SelSupplierByBgaNo)

		ACCESS_METHOD("SelSupplierByName")
			SQL_BUILDER_CREATE(SelSupplierByName)
			SQL_BUILDER_PUSH_BACK(SelSupplierByName)

	END_ACCESSOR_DEFINITION
	//------------------------------------------------------------------------------------------------/
	// end accessor definition
	//------------------------------------------------------------------------------------------------/

} //namespace acc_narcoticssupplier