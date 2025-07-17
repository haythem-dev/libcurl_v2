//----------------------------------------------------------------------------
/*! \file
 *  \brief  string builder declaration for supplier-accessor
 *  \author Frank Attmannspacher
 *  \date   15.05.2013
 */
//----------------------------------------------------------------------------
//include section
//----------------------------------------------------------------------------
#include "commonheader.h"
#include "acc_supplier.h"
#include "loggerpool.h"

//----------------------------------------------------------------------------
//namespace section
//----------------------------------------------------------------------------
namespace acc_supplier
{

//---------------------------------------------------------------------------
BUILDER_DEFINITION(SelSupplier)
	
ENSURE_PROPERTIES_ARE_SET(SelSupplier,"customerno;")
	
void SelSupplier::buildSQLString()
{
	resolve("SELECT bga_nr as customerno, besla_name as customername, ort as customerlocation, "
			"strasse AS street, plz_strasse AS postcode, telefon as telno, vorwahl as quickdialing "
			"FROM   pusupplier "
			"WHERE  besla_nr = liefer_besla_nr "
			"AND    bga_nr = #customerno# ");
		
	BLOG_TRACE( narcotics::LoggerPool::loggerDomModules, getSQLString() );
}

//---------------------------------------------------------------------------
BEGIN_ACCESSOR_DEFINITION("Supplier")

	PROPERTY_DESCRIPTION_LIST("int16 branchno; int32 customerno; string customername; string customerlocation; "
								"string telno; string quickdialing; string postcode; string street; string customeraddress;")
	
	ACCESS_METHOD("SelSupplier")
		SQL_BUILDER_CREATE(SelSupplier)
		SQL_BUILDER_PUSH_BACK(SelSupplier)
		
END_ACCESSOR_DEFINITION
				
} //namespace acc_supplier