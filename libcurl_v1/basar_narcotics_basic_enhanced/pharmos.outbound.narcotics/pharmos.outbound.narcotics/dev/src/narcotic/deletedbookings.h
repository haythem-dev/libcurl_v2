#ifndef GUARD_DELETEDBOOKINGS_H
#define GUARD_DELETEDBOOKINGS_H
//-------------------------------------------------------------------------------------------------//
/*! \file
 *  \brief  main interface for use case class deleted bookings
 *  \author Roland Kiefert
 *  \date   28.03.2006
 */
//-------------------------------------------------------------------------------------------------//
// includes
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
/*! \brief  Use Case class for use case deleted bookings.

	\n This is a final class.  
	\n throws no-throw */
//-------------------------------------------------------------------------------------------------//
class DeletedBookingsUC : public UseCaseController
{
public:
	/*! \brief The constructor for the use case incoming goods
		\n no-throw */
	DeletedBookingsUC();
	
	/*! \brief The destructor for the use case incoming goods
		\n no-throw */
	~DeletedBookingsUC();

	/*! \brief This method start the use case incoming goods
		\n no-throw */
	basar::appl::EventReturnType run();

	/*! \brief std method; has to be the first call in run() if events should be caught;
		\n no-throw */
	void registerEventHandlers( const bool forSave	//!< register = true / deregister = false
										);

	/*! \brief This method processes the event search in the use case incoming goods
		\n no-throw */
	SYSTEM_EVENT_HANDLER_DECLARATION( SearchDeletedBookings )
	
	/*! \brief This method processes the event restore the deleted booking in the use case incoming goods
		\n no-throw */
	SYSTEM_EVENT_HANDLER_DECLARATION( RestoreDeletedBookings )

	/*! \brief This method defines the rollback workaround for restoring bookings
		\n no-throw */
	void rollbackWorkaroundRestoring( basar::Int32 position );
	
private:
	DeletedBookingsUC	( const DeletedBookingsUC& ); // forbidden
	DeletedBookingsUC& operator=( const DeletedBookingsUC& ); // forbidden
};

} //namespace useCase
} //namespace narcotics

#endif //GUARD_DELETEDBOOKINGS_H