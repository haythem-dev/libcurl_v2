//----------------------------------------------------------------------------
/*! \file
 *  \brief  Viewconnector defines for detail dialog resultsetoverview
 *  \author Roland Kiefert
 *  \date   22.03.2006
 */
//----------------------------------------------------------------------------
#ifndef GUARD_RESULTSETDEFINES_H
#define GUARD_RESULTSETDEFINES_H

namespace resultsetdefines
{
// defines activating search criteria
basar::ConstString DIRECTSEARCH							= "directSearch";	
basar::ConstString SEARCH_FOR_ARTICLENO					= "searchForArticleNo";	
basar::ConstString SEARCH_FOR_CUSTOMERNO				= "searchForCustomerNo";	
basar::ConstString SEARCH_FOR_SUPPLIERNO				= "searchForSupplierNo";	
basar::ConstString SEARCH_FOR_ARTICLENO_AND_CUSTOMERNO	= "searchForArticleNoAndSearchForCustomerNo";	
basar::ConstString SEARCH_FOR_ARTICLENO_AND_SUPPLIERNO	= "searchForArticleNoAndSearchForSupplierNo";
basar::ConstString SEARCH_FOR_NARCOTICGROUP				= "searchForNarcoticGroup";
basar::ConstString NO_SEARCH_CRITERIA_SET				= "noSearchCriteriaSet";

// defines quantity of results in detail dialog
enum searchResultHits
{
	NO_HIT											= 0,
	ONE_HIT											= 1,
	MORE_THAN_ONE_HIT								= 2
};
} // namespace
#endif // GUARD_RESULTSETDEFINES_H