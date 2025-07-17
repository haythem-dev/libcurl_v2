#ifndef GUARD_NARCOTICSCATEGORY_H
#define GUARD_NARCOTICSCATEGORY_H
//-------------------------------------------------------------------------------------------------//
/*! \file
 *  \brief  main interface for use case class narcotics category
 *  \author Roland Kiefert
 *  \date   22.03.2006
 */
//-------------------------------------------------------------------------------------------------//

#include "usecasecontroller.h"

//-------------------------------------------------------------------------------------------------//
// namespace
//-------------------------------------------------------------------------------------------------//
namespace narcotics 
{
namespace useCase 
{

//-------------------------------------------------------------------------------------------------//
// class declaration
//-------------------------------------------------------------------------------------------------//
/*! \brief  Use Case class for use case narcotics category.

	\n This is a final class.  
	\n throws no-throw */
//-------------------------------------------------------------------------------------------------//
class NarcoticsCategoryUC : public UseCaseController
{
public:
	/*! \brief The constructor for the use case narcotics category
		\n no-throw */
	NarcoticsCategoryUC();
	/*! \brief The destructor for the use case narcotics category
		\n no-throw */
	~NarcoticsCategoryUC();
	/*! \brief This method start the use case narcotics category
		\n no-throw */
	basar::appl::EventReturnType run();

	/*! \brief std method; has to be the first call in run() if events should be caught;
		\n no-throw */
	void registerEventHandlers( const bool forSave	//!< register = true / deregister = false
										);

	/*! \brief This method processes the event search in the use case narcotics category by searching for parameter group
		\n no-throw */
	SYSTEM_EVENT_HANDLER_DECLARATION( SearchNarcoticsCategory )
	
private:
	NarcoticsCategoryUC	( const NarcoticsCategoryUC& ); // forbidden
	NarcoticsCategoryUC& operator=( const NarcoticsCategoryUC& ); // forbidden

	/*! \brief This method is used to match search criteria into search iterator
		\n no-throw */
	void matchToSearchCriteria();

	/*! \brief This method is used to match search criteria into Gui
		\n no-throw */
	void matchFromSearchCriteria();

	/*! \brief This method is used to match search results into main dialog
		\n no-throw */
	void matchFromCategory();

	/*! \brief This method is used to encapsulate the real search for execute results
		\n no-throw */
	basar::appl::EventReturnStruct searchCategory(); 
};

} //namespace useCase
} //namespace narcotics

#endif //GUARD_NARCOTICSCATEGORY_H