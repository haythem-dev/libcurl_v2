#ifndef GUARD_INCOMINGGOODSUC_H
#define GUARD_INCOMINGGOODSUC_H
//-------------------------------------------------------------------------------------------------//
/*! \file
 *  \brief  main interface for use case class incoming goods
 *  \author Roland Kiefert
 *  \date   14.02.2006
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
/*! \brief  Use Case class for use case register incoming goods.

	\n This is a final class.
	\n throws no-throw */
//-------------------------------------------------------------------------------------------------//
class IncomingGoodsUC : public UseCaseController
{
public:
	/*! \brief The constructor for the use case incoming goods
		\n no-throw */
	IncomingGoodsUC();
	/*! \brief The destructor for the use case incoming goods
		\n no-throw */
	~IncomingGoodsUC();

	/*! \brief This method start the use case incoming goods
		\n no-throw */
	basar::appl::EventReturnType run();

	/*! \brief std method; has to be the first call in run() if events should be caught;
		\n no-throw */
	void registerEventHandlers( const bool forSave	//!< register = true / deregister = false
										);


    SYSTEM_EVENT_HANDLER_DECLARATION( ReloadLastIncomingGoodsRequested )

    SYSTEM_EVENT_HANDLER_DECLARATION( AllUnprocessedIncomingGoodsRequested )

	/*! \brief This method processes the event search in the use case incoming goods
		\n no-throw */
	SYSTEM_EVENT_HANDLER_DECLARATION( SearchIncomingGoods )
	/*! \brief This method processes the event replace supplier in the use case incoming goods
		\n no-throw */
	SYSTEM_EVENT_HANDLER_DECLARATION( ReplaceSupplier )
	/*! \brief This method processes the event display undealed incoming goods (transactiontype b) in the use case incoming goods
		\n no-throw */
	SYSTEM_EVENT_HANDLER_DECLARATION( DisplayUndealedIncomingGoods )
	/*! \brief This method processes the event display undealed returned goods (transactiontype c) in the use case incoming goods
		\n no-throw */
	SYSTEM_EVENT_HANDLER_DECLARATION( DisplayUndealedReturnedGoods )
	/*! \brief This method processes the event refresh the incoming goods list in the use case incoming goods
		\n no-throw */
    SYSTEM_EVENT_HANDLER_DECLARATION( SaveIncomingGoodsList )
	/*! \brief This method processes the event registered incoming goods list in the use case incoming goods
		\n no-throw */
	SYSTEM_EVENT_HANDLER_DECLARATION( RegisteredIncomingGood )
	/*! \brief This method processes the event new booking in the use case incoming goods
		\n no-throw */
	SYSTEM_EVENT_HANDLER_DECLARATION( NewBookingIncomingGood )
	/*! \brief This method processes the event read article data in the use case incoming goods
		\n no-throw */
	SYSTEM_EVENT_HANDLER_DECLARATION( ReadArticleDataIncomingGood )
	/*! \brief This method processes the event read customer data in the use case incoming goods
		\n no-throw */
	SYSTEM_EVENT_HANDLER_DECLARATION( ReadCustomerDataIncomingGood )
	/*! \brief This method processes the event delete incoming goods in the use case incoming goods
		\n no-throw */
	SYSTEM_EVENT_HANDLER_DECLARATION( DeleteIncomingGoods )
	/*! \brief This method processes the event current iterator change in the use case incoming goods
		\n no-throw */
	SYSTEM_EVENT_HANDLER_DECLARATION( SetCurrentIteratorIncomingGoods )

private:
    void                                init();
    void                                shutdown();
    basar::appl::EventReturnStruct      findUnprocessedIncomingGoods();
    basar::appl::EventReturnStruct      findNarcoticsSupplier();
    basar::appl::EventReturnStruct      findNumberOfIncomingGoods();

    basar::appl::EventReturnStruct      findIncomingGoods();
    basar::appl::EventReturnStruct      preEvaluateSupplierReplacement();
    basar::appl::EventReturnStruct      postEvaluateSupplierReplacement();

	void newDisplayToSearch();
	/*! \brief method is needed to search for narcotic group specific data
		\n no-throw */
	basar::appl::EventReturnStruct findNarcoticGroupData();
	/*! \brief method is needed to match narcotic group specific data into getManager->m_currentBooking
		\n no-throw */
	void matchNarcoticGroupData();
	/*! \brief This method is used to encapsulate the real search for execute results
		\n no-throw */
	basar::appl::EventReturnStruct searchBookingsAndMatch();
	/*! \brief This method defines the rollback workaround for deleting incoming goods
		\n no-throw */
	void rollbackWorkaroundDeleting();
	/*! \brief This method defines the rollback workaround for saving bookings of incoming goods
	\n no-throw */
	void rollbackWorkaroundSaving();

	IncomingGoodsUC	( const IncomingGoodsUC& ); // forbidden
	IncomingGoodsUC& operator=( const IncomingGoodsUC& ); // forbidden

	enum EnumState
	{
		ONLY_QUANTITY_INPUT,	//!< only the quantity of the translation are set
		ALL_INPUT				//!< all fields can be set
	};

	EnumState	m_state;		//!< actual state of the use case
	// saves state of former user interaction
	basar::I18nString					m_ChangingState;
	// iterator based on an empty accessor used for searching article and customer specific data
	basar::db::aspect::AccessorPropertyTable_YIterator	m_SearchIt;
};

} //namespace useCase
} //namespace narcotics

#endif //GUARD_INCOMINGGOODSUC_H