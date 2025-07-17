//----------------------------------------------------------------------------
/*! \file
 *  \brief  string builder definition for supplier-accessor
 *  \author Roland Kiefert
 *  \date   17.02.2006
 */
//----------------------------------------------------------------------------
//includes
//----------------------------------------------------------------------------
#include "commonheader.h"
#include "acc_numberincominggoods.h"

//----------------------------------------------------------------------------
//namespaces
//----------------------------------------------------------------------------
namespace acc_numberincominggoods
{

//---------------------------------------------------------------------------
BUILDER_DEFINITION(SelIncominggoods)
	
	ENSURE_PROPERTIES_ARE_SET(SelIncominggoods,"branchno;")

SQL_PATTERN(SelIncominggoods,
			"SELECT CAST(count(*) AS INTEGER) AS incominggoods  "
			"FROM narctransactioncatalog "
            "WHERE branchno=#branchno# "
			"AND transactiontype = 'b' "
			"AND processedflag = '0' "
			"AND deletedflag='0' ")

//---------------------------------------------------------------------------
BEGIN_ACCESSOR_DEFINITION("NumberIncomingGoods")

	PROPERTY_DESCRIPTION_LIST("int32 incominggoods;")
	
	ACCESS_METHOD("SelIncominggoods")
		SQL_BUILDER_CREATE(SelIncominggoods)
		SQL_BUILDER_PUSH_BACK(SelIncominggoods)

END_ACCESSOR_DEFINITION
				
} //namespace acc_numberincominggoods