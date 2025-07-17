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
#include "acc_numberreturngoods.h"


//----------------------------------------------------------------------------
//namespaces
//----------------------------------------------------------------------------
namespace acc_numberreturngoods {

//---------------------------------------------------------------------------
BUILDER_DEFINITION(SelReturngoods)
	
	ENSURE_PROPERTIES_ARE_SET(SelReturngoods,"branchno;")

SQL_PATTERN(SelReturngoods,
			"SELECT CAST(count(*) AS INTEGER) AS returngoods  "
			"FROM narctransactioncatalog "
            "WHERE branchno=#branchno# "
			"AND transactiontype = 'c' "
			"AND processedflag = '0' "
			"AND deletedflag='0' ")

//---------------------------------------------------------------------------
BEGIN_ACCESSOR_DEFINITION("NumberReturnGoods")

	PROPERTY_DESCRIPTION_LIST("int32 returngoods;")
	
	ACCESS_METHOD("SelReturngoods")
		SQL_BUILDER_CREATE(SelReturngoods)
		SQL_BUILDER_PUSH_BACK(SelReturngoods)

END_ACCESSOR_DEFINITION
				
} //namespace acc_numberreturngoods