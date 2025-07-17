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
#include "acc_customernomapping.h"
#include "loggerpool.h"

//----------------------------------------------------------------------------
//namespace
//----------------------------------------------------------------------------
namespace acc_customernomapping
{

//---------------------------------------------------------------------------
BUILDER_DEFINITION(SelCustomerMapping)
	
	ENSURE_PROPERTIES_ARE_SET(SelCustomerMapping,"customersupplierno;branchno;")
	
	void SelCustomerMapping::buildSQLString()
	{
		resolve("SELECT customernoextern, switchaddressflag FROM customernomapping "
				"WHERE branchno=#branchno# AND customernointern=#customersupplierno#");

		BLOG_TRACE(LoggerPool::loggerDomModules, getSQLString());
	}

//---------------------------------------------------------------------------
BEGIN_ACCESSOR_DEFINITION("CustomerNoMapping")

	PROPERTY_DESCRIPTION_LIST("int32 customernoextern; int16 switchaddressflag;")
	
	ACCESS_METHOD("SelCustomerMapping")
		SQL_BUILDER_CREATE(SelCustomerMapping)
		SQL_BUILDER_PUSH_BACK(SelCustomerMapping)
		
END_ACCESSOR_DEFINITION
				
} //namespace acc_customernomapping