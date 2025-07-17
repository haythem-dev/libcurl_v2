#ifndef GUARD_ORDERSOVERVIEWUC_H
#define GUARD_ORDERSOVERVIEWUC_H
//-------------------------------------------------------------------------------------------------//
/*! \file
 *  \brief  main interface for use case class narcotics category
 *  \author Roland Kiefert
 *  \date   22.03.2006
 */
//-------------------------------------------------------------------------------------------------//
//includes
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
class OrderOverviewUC : public UseCaseController
{
public:
	/*! \brief The constructor for the use case narcotics category
		\n no-throw */
	OrderOverviewUC();
	/*! \brief The destructor for the use case narcotics category
		\n no-throw */
	~OrderOverviewUC();
	/*! \brief This method start the use case narcotics category
		\n no-throw */
	basar::appl::EventReturnType run();

	/*! \brief std method; has to be the first call in run() if events should be caught;
		\n no-throw */
	void registerEventHandlers( const bool forSave	//!< register = true / deregister = false
										);

	SYSTEM_EVENT_HANDLER_DECLARATION( LineCodeChanged )

	SYSTEM_EVENT_HANDLER_DECLARATION( SearchNarcoticsSuppliersForOrderOverView )
	/*! \brief	cancels an order (check whether possible, if so, remove contributionvoucher from bfarm buffer */
	SYSTEM_EVENT_HANDLER_DECLARATION( CancelFullOrder )

	/*! \brief	cancels one or more positions of an order (check whether possible, if so, remove contributionvoucher from bfarm buffer and generate a new one?*/
	SYSTEM_EVENT_HANDLER_DECLARATION( CancelBookings )

	/*! \brief This method processes the search of all bookings belonging to a special oder
		\n no-throw */
	SYSTEM_EVENT_HANDLER_DECLARATION( SearchOrderBooking )
	
	/*! \brief This method processes the search of the article and narcotic group specific data
		\n		belonging to a special order position
		\n		no-throw */
	SYSTEM_EVENT_HANDLER_DECLARATION( SearchArticleNarcoticDataForTransaction )

	/*! \brief This method processes the search of name and location for a changed customersupplierno
		\n		of a special order
		\n		no-throw */
	SYSTEM_EVENT_HANDLER_DECLARATION( SearchNameAndLocation )
	
	/*! \brief This method processes the search of narcotics supplier for corresponding combobox in subdialog
		\n		of a special order
		\n		no-throw */
	SYSTEM_EVENT_HANDLER_DECLARATION( SearchNarcoticsSupplier )

	/*! \brief	first it has to be made a decision to decide if it is about an insert or an update.
				With a update a deep copy is necessary for finding out the attribute values for logging.
				Afterwards the view connector values will be matched to the application specific booking 
				and the domain modul save methode is invoked. */
	SYSTEM_EVENT_HANDLER_DECLARATION( SaveBooking )
	
	/*! \brief	executes the saving of changes made to a whole order */
	SYSTEM_EVENT_HANDLER_DECLARATION( SaveOrderChanges )
	
	/*! \brief	Assigns the current narcotic booking of the view to the application single narcotic booking.
				Important for invocation of the use case method 'handleSaveBooking()'. There it will differentiate 
				if the current booking is a new or used one to decide the making of a deep copy (only in the case of an update). */
	SYSTEM_EVENT_HANDLER_DECLARATION( UsedBooking )

	/*! \brief	matches the current values to the current narcotic booking, delegates to the according method of the domain modul 
				(the narcotic booking is not deleted physically but will be set a deleted flag)				
				and will finally select the complete query result of the view connector 
				via the method 'handleSearchBookingCatalog()' to most current data 
				(perhaps the remaining data could be manipulated by another user). */
	SYSTEM_EVENT_HANDLER_DECLARATION( DeleteBooking )

	/*! \brief	evaluates if transactiontime has really changed and therefore button SaveOrderChanges has to be enabled */
	SYSTEM_EVENT_HANDLER_DECLARATION( CheckTimeChanges )

	/*! \find specific customer information. \n no-throw */
	SYSTEM_EVENT_HANDLER_DECLARATION( FindCustomer )

	/*! \brief branch specific data is searched. \n no-throw */
	SYSTEM_EVENT_HANDLER_DECLARATION( FindBranchData )

	SYSTEM_EVENT_HANDLER_DECLARATION( PrintDissolveReceipt )
	
private:
	/*! \brief	checks if changes to transactiontime or customersupplierno have really been applied. 
				\n no-throw. */
	bool checkChangesApplied( basar::I18nString fieldconstant );

	/*! \brief This method checks if customersupplier data is completely set and otherwise appends missing values
		\n no-throw */
	void checkCustomerSupplierData( basar::db::aspect::AccessorPropertyTable_YIterator currentBooking );

	/*! \brief This method defines the rollback workaround for saving bookings
		\n no-throw */
	void rollbackWorkaroundSaving();

	/*! \brief This method defines the rollback workaround for deleting bookings
		\n no-throw */
	void rollbackWorkaroundDeleting();

	bool ftpCancelOrder( basar::Int32 contributionVoucherNo );

	bool isCurrentBookingNotEqualToCurrentIterator( const basar::db::aspect::AccessorPropertyTable_YIterator currentBooking );

	OrderOverviewUC	( const OrderOverviewUC& ); // forbidden
	OrderOverviewUC& operator=( const OrderOverviewUC& ); // forbidden

	bool										m_DeepCopied;

	basar::db::aspect::AccessorPropertyTable_YIterator m_FirstPosYit;
};

} //namespace useCase
} //namespace narcotics

#endif //GUARD_ORDERSOVERVIEWUC_H