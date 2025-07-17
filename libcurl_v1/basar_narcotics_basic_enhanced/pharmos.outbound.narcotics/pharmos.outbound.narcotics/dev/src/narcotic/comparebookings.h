#ifndef GUARD_COMPAREBOOKINGS_H
#define GUARD_COMPAREBOOKINGS_H
//-------------------------------------------------------------------------------------------------//
/*! \file
 *  \brief  main interface for use case class incoming goods
 *  \author Roland Kiefert
 *  \date   24.02.2006
 */
//-------------------------------------------------------------------------------------------------//
//includes
//-------------------------------------------------------------------------------------------------//
#include "usecasecontroller.h"
#include "contributionvouchervc.h"

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
/*! \brief  Use Case class for use case compare original and narcotics bookings.

	\n This is a final class.  
	\n throws no-throw */
//-------------------------------------------------------------------------------------------------//
class CompareBookingsUC : public UseCaseController
{
public:
	/*! \brief The constructor for the use case compare bookings
		\n no-throw */
	CompareBookingsUC();
	/*! \brief The destructor for the use case compare bookings
		\n no-throw */
	~CompareBookingsUC();

	/*! \brief This method start the use case compare bookings
		\n no-throw */
	basar::appl::EventReturnType run(); 

	/*! \brief std method; has to be the first call in run() if events should be caught;
		\n no-throw */
	void registerEventHandlers( const bool forSave	//!< register = true / deregister = false
										);

	/*! \brief This method processes the event search in the use case compare bookings
		\n no-throw */
	SYSTEM_EVENT_HANDLER_DECLARATION( SearchCompareBookings )
	/*! \brief This method processes the event rebuild in the use case compare bookings
		\n no-throw */
	SYSTEM_EVENT_HANDLER_DECLARATION( RebuildNarcoticsBooking )
	/*! \brief This method processes the event delete in the use case compare bookings
		\n no-throw */
	SYSTEM_EVENT_HANDLER_DECLARATION( DeleteNarcoticsBooking )

	SYSTEM_EVENT_HANDLER_DECLARATION( SearchArticleNoWithDiffQtyInStock )

public:
	/*! \brief This method processes . \n no-throw */
	bool m_invokeContextDiffQtyInStock;

private:

};

} //namespace useCase
} //namespace narcotics
#endif //GUARD_COMPAREBOOKINGS_H