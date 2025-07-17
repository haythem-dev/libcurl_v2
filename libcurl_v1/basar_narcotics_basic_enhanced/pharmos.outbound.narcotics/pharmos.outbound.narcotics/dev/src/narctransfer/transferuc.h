//---------------------------------------------------------------------------//
/*! \file 
 *  \brief interface for use case TransferUC
 *  \author 
 *  \date 31.08.2009
 *  \version 00.00.00.01 first version
 */ //-----------------------------------------------------------------------//
#ifndef GUARD_TRANSFERUC_H
#define GUARD_TRANSFERUC_H

//---------------------------------------------------------------------------//
// include section
//---------------------------------------------------------------------------//
#include "usecasecontroller.h"
#include "narctransfer_definitions.h"

//---------------------------------------------------------------------------//
// namespace section
//---------------------------------------------------------------------------//
namespace narcotics 
{
namespace useCase 
{

//---------------------------------------------------------------------------------------------------------//
// struct ControlBreak
//---------------------------------------------------------------------------------------------------------//
struct ControlBreak
{
	ControlBreak()
	{
		init();
	}
	void init()
	{
		m_ArticleNo			= 0;
		m_BranchNo			= 0;
		m_TransactionTime	= 0;
		m_TransactionDate	= 0;
	}
	basar::Int32 m_ArticleNo;
	basar::Int16 m_BranchNo;
	basar::Int32 m_TransactionTime;
	basar::Int32 m_TransactionDate;
};

//---------------------------------------------------------------------------------------------------------//
// struct ControlBreak
//---------------------------------------------------------------------------------------------------------//
struct NarcoticBooking
{
	NarcoticBooking()
	{
		catalogid   = 0;
        qty			= 0;
		branchNo	= 0;
		articleNo	= 0;
		bookingDate = 0;
		bookingTime = 0;
	}
	
	basar::Int32	catalogid;
    basar::Int32	qty;
	basar::Int16	branchNo;
	basar::Int32	articleNo;
	basar::Int32	bookingDate;
	basar::Int32	bookingTime;
};

//---------------------------------------------------------------------------------------------------------//
// class Counter
//---------------------------------------------------------------------------------------------------------//
class Counter
{
public:
	//! c'tor			\n no-throw
	Counter		()	
	:	m_Group             (0),
		m_TransferredGroups (0),
		m_TransferredRows   (0),
		m_MergeUpdatedRows  (0),
        m_UpdatedRows       (0),
		m_LockedGroups      (0),
		m_RolledbackGroups  (0)
	{};

	basar::UInt32	m_Group;				//!< number of groups
	basar::UInt32	m_TransferredGroups;	//!< number of transferred groups
	basar::UInt32	m_TransferredRows;		//!< number of transferred rows
	basar::UInt32	m_MergeUpdatedRows;		//!< number of merge rows (qty transaction)
	basar::BULong	m_UpdatedRows;			//!< number of updated rows (qty in stock)
	basar::UInt32	m_LockedGroups;			//!< number of locked groups (by other)
	basar::UInt32	m_RolledbackGroups;		//!< number of rolledback groups

private:
				Counter		(const Counter & rCopy);	//!< forbidden
	Counter & 	operator =	(const Counter & rCopy);	//!< forbidden
};

//---------------------------------------------------------------------------//
/*! \brief TransferUC use case class for  ... 
		   \n This is a final class. \n throws no-throw
 */
//---------------------------------------------------------------------------//
class TransferUC : public UseCaseController
{
public:
	/*! \brief std-constructor \n \throw no-throw */
	TransferUC();
	
	/*! \brief std-destructor \n \throw no-throw */
	~TransferUC();

	virtual void registerEventHandlers(const bool forSave	//!< register = true / deregister = false
										);

	virtual basar::appl::EventReturnType run(basar::appl::IEventSource& rSource,
		basar::db::aspect::AccessorPropertyTable_YIterator yitFromEvSrc);

	SYSTEM_EVENT_HANDLER_DECLARATION(TransferNarcTransactions)

private:
	/*! \brief copy constructor \n \throw no-throw */
	TransferUC(const TransferUC& r);
	/*! \brief assign operator \n \throw no-throw */
	TransferUC& operator = (const TransferUC& r);

	bool		isTransactionTypeValid( const basar::VarString& transactionType);
	bool		isIncomingTransactionType(const basar::VarString& transactionType);
	ProcessStateEnum	processBookings( basar::db::aspect::AccessorPropertyTable_YIterator yit);
	bool		fetchCustomerSupplierAndNarcGroupData(basar::db::aspect::AccessorPropertyTable_YIterator yit);
	bool		isControlBreak( basar::db::aspect::AccessorPropertyTable_YIterator yit);
	void		fetchNextControlBreak( basar::db::aspect::AccessorPropertyTable_YIterator& yit);
	void		assignControlBreak( basar::db::aspect::AccessorPropertyTable_YIterator yit);
	void		setCatalogID2NarcoticBooking( basar::db::aspect::AccessorPropertyTable_YIterator yit);
	void		createUniqueTransactionTime(basar::db::aspect::AccessorPropertyTable_YIterator yit);
	int			checkTransactionAndUnlock( bool& inTransaction, Counter& cnt );
	std::map<basar::Int32, NarcoticBooking>::iterator findUnprocessedPurchaseOrderPos( basar::db::aspect::AccessorPropertyTable_YIterator& yit );

	basar::appl::EventReturnStruct processTransfersByBranch(basar::db::aspect::AccessorPropertyTable_YIterator yit);

	void		RollbackHandle(basar::UInt32& rolledbackGroups, bool& inTransaction, 
					basar::db::aspect::AccessorPropertyTable_YIterator& yit, 
					basar::VarString msg);

	ControlBreak							m_ControlBreak;
	std::map<basar::Int32, NarcoticBooking>	m_PurchaseOrderMap;

};

} //namespace useCase
} //namespace narcotics

#endif //GUARD_TRANSFERUC_H