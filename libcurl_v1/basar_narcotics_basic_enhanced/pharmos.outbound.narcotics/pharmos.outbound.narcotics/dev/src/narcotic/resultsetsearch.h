//------------------------------------------------------------------------------
/*! \file
 *  \brief  class for finding resultset to given name
 *  \author Benjamin Endlicher
 *  \date   12.04.2010
 */
//------------------------------------------------------------------------------
#ifndef GUARD_NARCOTICS_RESULTSETSEARCH_H
#define GUARD_NARCOTICS_RESULTSETSEARCH_H

//------------------------------------------------------------------------------
//includes
//------------------------------------------------------------------------------
#include "usecasecontroller.h"
#include "resultsetdefines.h"

// to instantiate members of domain modules
#include "narcoticsmodule.h"
#include "narcoticsgroupmodule.h"
#include "customermodule.h"

//------------------------------------------------------------------------------
//namespaces
//------------------------------------------------------------------------------
namespace narcotics	
{
namespace useCase 
{

//-------------------------------------------------------------------------------------------------//
// class declaration
//-------------------------------------------------------------------------------------------------//
/*! \brief  class ResultsetSearch represents the use case class for conducting resultset search.

    This is a final class.     
 */
class ResultsetSearch : public UseCaseController
{
public:
	/*! \brief	D'flt c'tor. \n  no-throw */
	ResultsetSearch();
	
	/*! \brief	D'tor with no (special) action. \n  no-throw */
	~ResultsetSearch();	

		/*! \brief	Initializes a dialog with a propertytable and shows it afterwards. \n  no-throw */
	basar::appl::EventReturnType run();
	
	/*! \brief std method; has to be the first call in run() if events should be caught;
		\n no-throw */
	void registerEventHandlers( const bool forSave	//!< register = true / deregister = false
										);
	/*! \brief This method is used to encapsulate the search for results corresponding to given searchcriteria
		\n no-throw */
	resultsetdefines::searchResultHits getSearchResults( basar::I18nString searchcriteria );

	/*! \brief This method is used to match corresponding number for given name into search iterator
		\n no-throw */
	void matchNumberForName( basar::I18nString searchcriteria, bool detailDialogShown );

	/*! \brief This method gets current selected entry from detail dialog 
		\n no-throw */
	basar::appl::EventReturnStruct getCurrentSelection( basar::I18nString searchcriteria );

	/*! \brief This method is used to show detail dialog
		\n no-throw */
	basar::gui::tie::WidgetReturnEnum showDetailDialog( basar::db::aspect::AccessorPropertyTableRef resultTable,
							basar::ConstString property1,
							basar::ConstString property2,
							basar::I18nString headerlabel1, 
							basar::I18nString headerlabel2 );

	/*! \brief This method finds corresponding number to given name
		\n no-throw */
	basar::appl::EventReturnStruct findNoByName( basar::I18nString searchCriteria );

	/*! \brief This method evaluates given search criteria to decide further course of action
		\n no-throw */
	basar::I18nString differentiateSearchCriteria();

	/*! \brief This method is used to find resultset according to given search criteria
		\n no-throw */
	basar::appl::EventReturnStruct findResultset();

	/*! \brief This method is used to provide calling UCC the value of m_searchCriteria
		\n no-throw */
	basar::I18nString getSearchCriteria();

	/*! \brief This method is used to reset the value of m_searchCriteria
		\n no-throw */
	void resetSearchCriteria();

	/*! \brief This method is used to set additional search criteria for bookings search (bookings are processed)
		\n no-throw */
	void setAdditionalSearchCriteriaProcessed();

	/*! \brief This method is used to set additional search criteria for bookings search (bookings aren't processed)
		\n no-throw */
	void setAdditionalSearchCriteriaNotProcessed();

	/*! \brief This method is used to check if matching from search criteria into Gui is allowed
		\n no-throw */
	bool matchingFromSearchCriteriaAllowed();

private:
	/*! \brief initializes a dialog with a propertytable and shows it afterwards. \n  no-throw */
	ResultsetSearch( const ResultsetSearch& ); // forbidden
	
	/*! \brief initializes a dialog with a propertytable and shows it afterwards. \n  no-throw */
	ResultsetSearch& operator=( const ResultsetSearch& ); // forbidden

	// supporting function as FullCaching might be needed in UseCaseController, shouldn't be activated
	// because of matters of reuse
	void forwardCaching( basar::db::aspect::AccessorPropertyTableRef table );

	basar::I18nString			m_searchCriteria;
	basar::Int32				m_resultCounter;
	bool						m_DetailDialogCancelled;
};

} //namespace useCase
} //namespace narcotics

#endif	//GUARD_NARCOTICS_RESULTSETSEARCH_H