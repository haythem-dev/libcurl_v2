//-------------------------------------------------------------------------------------------------//
/*! \file
 *  \brief  string builder definition for user-accessor + registration of user-accessor
 *  \author Roland Kiefert
 *  \date   17.02.2006
 */
//-------------------------------------------------------------------------------------------------//

//-------------------------------------------------------------------------------------------------//
// includes
//-------------------------------------------------------------------------------------------------//
#include "commonheader.h"
#include "acc_narcgroup.h"
#include "narctransfer_definitions.h"
#include "loggerpool.h"

//-------------------------------------------------------------------------------------------------//
// namespaces
//-------------------------------------------------------------------------------------------------//
namespace acc_narcgroup
{

//-------------------------------------------------------------------------------------------------//
// builder definition
//-------------------------------------------------------------------------------------------------//
BUILDER_DEFINITION(SelGroupByArticleNo)

	ENSURE_PROPERTIES_ARE_SET(SelGroupByArticleNo,"articleno;")

	void SelGroupByArticleNo::buildSQLString()
	{
		resolve("SELECT btm_gruppe AS narcgroup, "
						"btm_bez AS narcname, "
						"btm_gehalt AS concentration, "
						"RTRIM(LTRIM(btm_einheit)) AS packageunit, "
						"btm_faktor AS narcfactor, "
						"tradegroup.artikel_nr AS articleno "
				"FROM narcgroup, tradegroup, artikelzentral z "
				"WHERE tradegroup.art='6' AND "
						"tradegroup.warengruppe=btm_gruppe AND "
						"tradegroup.artikel_nr=#articleno# AND "
						"tradegroup.artikel_nr=z.artikel_nr ");
		BLOG_TRACE(LoggerPool::loggerDomModules, getSQLString());
	}

//-------------------------------------------------------------------------------------------------//
// accessor definition
//-------------------------------------------------------------------------------------------------//
BEGIN_ACCESSOR_DEFINITION("NarcoticsGroup")

	BEGIN_PROPERTY_DESCRIPTION_LIST
		PROPERTY_DESCRIPTION_LIST_ADD(lit::narcotics::NARCGROUP)
		PROPERTY_DESCRIPTION_LIST_ADD(lit::narcotics::NARCNAME)
		PROPERTY_DESCRIPTION_LIST_ADD(lit::narcotics::CONCENTRATION)
		PROPERTY_DESCRIPTION_LIST_ADD(lit::narcotics::PACKAGEUNIT)
		PROPERTY_DESCRIPTION_LIST_ADD(lit::narcotics::NARCFACTOR)
		PROPERTY_DESCRIPTION_LIST_ADD(lit::narcotics::ARTICLENO)
	END_PROPERTY_DESCRIPTION_LIST

	ACCESS_METHOD("FindGroupByArticleNo")
		SQL_BUILDER_CREATE(SelGroupByArticleNo)
		SQL_BUILDER_PUSH_BACK(SelGroupByArticleNo)

END_ACCESSOR_DEFINITION

} //namespace acc_narcgroup