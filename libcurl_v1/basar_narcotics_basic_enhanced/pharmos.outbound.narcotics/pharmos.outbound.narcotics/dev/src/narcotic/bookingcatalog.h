#ifndef GUARD_BOOKINGCATALOG_H
#define GUARD_BOOKINGCATALOG_H

//-------------------------------------------------------------------------------------------------//
// include
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
class BookingCatalogUC : public UseCaseController
{
public:
	BookingCatalogUC();
	~BookingCatalogUC();
	
	//! \brief	starting of use case 'BookingCatalogUC'.
	virtual basar::appl::EventReturnType run();

	/*! \brief std method; has to be the first call in run() if events should be caught;
		\n no-throw */
	virtual void registerEventHandlers( const bool forSave	//!< register = true / deregister = false
										);
	
	/*! \brief	first search criteria will be organized for selecting stock location by article no, 
				afterwards all according narcotics booking (undeleted and processed) will be selected. 
				via the method 'handleSearchBookingCatalog()'. */
	SYSTEM_EVENT_HANDLER_DECLARATION( SearchBookingCatalog )
	
	/*! \brief	\n no-throw */
	SYSTEM_EVENT_HANDLER_DECLARATION( SearchArticleNarcoticData )
	
	/*! \brief	first search criteria will be organized for selecting all according narcotics booking (undeleted and processed). */
	SYSTEM_EVENT_HANDLER_DECLARATION( DisplayInternalBookings )

	/*! \brief	first search criteria will be organized for selecting all narcotics booking, that are incomplete (undeleted and processed). */
	SYSTEM_EVENT_HANDLER_DECLARATION( DisplayIncompeteBookings )
	
    /*! \brief	first search criteria will be organized for selecting all narcotics booking, that are double booked (undeleted and processed). */
	SYSTEM_EVENT_HANDLER_DECLARATION( SearchDoubleBookingsRequested )
	
	/*! \brief	cancels a position in an order (check whether possible, if so, remove contributionvoucher from bfarm buffer */
	SYSTEM_EVENT_HANDLER_DECLARATION( CancelBooking )

	/*! \brief	first it has to be made a decision to decide if it is about an insert or an update.
				With a update a deep copy is necessary for finding out the attribute values for logging.
				Afterwards the view connector values will be matched to the application specific booking 
				and the domain modul save methode is invoked. */
	SYSTEM_EVENT_HANDLER_DECLARATION( SaveBooking )
	
	/*! \brief	delegates to the domain modul to insert a new narcotic booking. 
				After that the empty values of the narcotic booking will be matched to the view connector to 
				establish a connection between gui-propertytable and accessor-propertytable. 
				The actual saving will be handled via another method. */
	SYSTEM_EVENT_HANDLER_DECLARATION( CreateBooking )
	
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
	
	/*! \brief	dedicated search for a supplier. */
	SYSTEM_EVENT_HANDLER_DECLARATION( SearchCustomerSupplierByBgaNo )
	
	/*! \brief	starts a new use case via which the deleted flag of narcotic bookings can be revoked. */
	SYSTEM_EVENT_HANDLER_DECLARATION( RebuildBooking )
	
	/*! \brief	print narcotics book
		\throw  no-throw */
	SYSTEM_EVENT_HANDLER_DECLARATION( PrintBookingCatalog )
	
	/*! \brief print delivery receipt. \n no-throw */
    SYSTEM_EVENT_HANDLER_DECLARATION( PrintDeliveryReceipt )

	/*! \brief print dissolve receipt. \n no-throw */
    SYSTEM_EVENT_HANDLER_DECLARATION( PrintDissolveReceipt )
	
	SYSTEM_EVENT_HANDLER_DECLARATION( SearchNarcoticsSupplier )
	
	/*! \brief branch specific data is searched. \n no-throw */
	SYSTEM_EVENT_HANDLER_DECLARATION( FindBranchData )

	/*! \find specific customer information. \n no-throw */
	SYSTEM_EVENT_HANDLER_DECLARATION( FindCustomer )

	SYSTEM_EVENT_HANDLER_DECLARATION( RegisterOrderForPrinting )
	
private:
	/*! \brief print method for dissolve receipt
		\n no-throw */
	void printDissolveReceipt( basar::db::aspect::AccessorPropertyTableRef prTabDissolveReceiptList,
							   basar::db::aspect::AccessorPropertyTable_YIterator yitBranch,
							   basar::db::aspect::AccessorPropertyTable_YIterator yitDate );

	/*! \brief This method checks if customersupplier data is completely set and otherwise appends missing values
		\n no-throw */
	void checkCustomerSupplierData( basar::db::aspect::AccessorPropertyTable_YIterator currentBooking );

	/*! \brief This method is used to match search criteria into Gui
		\n no-throw */
	void matchFromSearchCriteria();
	
	/*! \brief This method is used to match search criteria into search iterator
		\n no-throw */
	void matchToSearchCriteria();

	/*! \brief This method defines the rollback workaround for saving bookings
		\n no-throw */
	void rollbackWorkaroundSaving();

	/*! \brief This method defines the rollback workaround for deleting bookings
		\n no-throw */
	void rollbackWorkaroundDeleting();

	BookingCatalogUC( const BookingCatalogUC& ); // forbidden
	BookingCatalogUC& operator=( const BookingCatalogUC& ); // forbidden

	bool												m_DeepCopied;
	// saves state of former user interaction
	basar::I18nString									m_ChangingState;
};

} //namespace useCase
} //namespace narcotics

#endif // GUARD_BOOKINGCATALOG_H