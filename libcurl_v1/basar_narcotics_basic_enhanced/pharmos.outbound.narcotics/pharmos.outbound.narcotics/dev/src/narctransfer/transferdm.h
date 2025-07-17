//------------------------------------------------------------------------------------------------//
/*! \file
 *  \brief  main interface for domain class narcotics
 *  \author Roland Kiefert
 *  \date   14.02.2006
 */
//------------------------------------------------------------------------------------------------//
#ifndef GUARD_TRANSFERDM_H
#define GUARD_TRANSFERDM_H
//-------------------------------------------------------------------------------------------------//
// includes
//-------------------------------------------------------------------------------------------------//

namespace libnarcotics {
	namespace lockmanager {
		class LockManager;
	}
}

//------------------------------------------------------------------------------------------------//
// namespaces section
//------------------------------------------------------------------------------------------------//
namespace narcotics 
{
namespace domMod 
{

//------------------------------------------------------------------------------------------------//
/*! \brief  domain class for narcotics module.

	\n This is a final class.  
	\n throws no-throw */
//------------------------------------------------------------------------------------------------//
// class TransferDM
//------------------------------------------------------------------------------------------------//
class TransferDM
{
public:
	TransferDM();

	virtual ~TransferDM();

	/*! \brief	this method initializes the module narcoticsModule. 
				Creates several accessor instance member variables. \n no-throw */
	void													init(/*const basar::Int16 branchNo,*/ const basar::db::aspect::ConnectionRef activeConn );

	//! \brief	resets all AccessorInstanceRef members. \n no-throw
	void													shutdown();

	//-------------------------------------------------------------------------------------------------------------------------//
	//	find - methods
	//-------------------------------------------------------------------------------------------------------------------------//
	//! \brief one or more narcotics bookings are loaded depending on the search pattern. \n no-throw
	basar::db::aspect::AccessorPropertyTableRef findAllTransferBookings(basar::db::aspect::AccessorPropertyTable_YIterator yit);

	basar::db::aspect::AccessorPropertyTableRef findAllBranchesToTransfer();
		
	//! \brief  \n no-throw
	basar::db::aspect::AccessorPropertyTable_YIterator	findCustomerMapping(basar::db::aspect::AccessorPropertyTable_YIterator yit);

	//-------------------------------------------------------------------------------------------------------------------------//
	//	method for preparing bookings from narctransfer for printing in narcotics
	//-------------------------------------------------------------------------------------------------------------------------//
	const std::vector<basar::Int16>						prepareBookingsForPrint();
	//-------------------------------------------------------------------------------------------------------------------------//
	//	save - methods (updates, inserts)
	//-------------------------------------------------------------------------------------------------------------------------//
	basar::db::aspect::ExecuteResultInfo				saveOriginal( basar::db::aspect::AccessorPropertyTable_YIterator yit );

	//-------------------------------------------------------------------------------------------------------------------------//
	//	save - methods (updates, inserts)
	//-------------------------------------------------------------------------------------------------------------------------//
	basar::db::aspect::ExecuteResultInfo				saveCatBookings( basar::db::aspect::AccessorPropertyTable_YIterator yit, bool SaveFlag );

	//-------------------------------------------------------------------------------------------------------------------------//
	//	
	//-------------------------------------------------------------------------------------------------------------------------//
	basar::db::aspect::ExecuteResultInfo				deleteTransferEntry( basar::db::aspect::AccessorPropertyTable_YIterator yit );
	//-------------------------------------------------------------------------------------------------------------------------//
	//	
	//-------------------------------------------------------------------------------------------------------------------------//
	basar::db::aspect::AccessorPropertyTable_YIterator	findCustomerSupplierByID( basar::db::aspect::AccessorPropertyTable_YIterator yit );

	//-------------------------------------------------------------------------------------------------------------------------//
	//	
	//-------------------------------------------------------------------------------------------------------------------------//
	basar::db::aspect::AccessorPropertyTable_YIterator	findNarcoticsGroupByArticleno( basar::db::aspect::AccessorPropertyTable_YIterator yit );

    //-------------------------------------------------------------------------------------------------------------------------//
	//	
	//-------------------------------------------------------------------------------------------------------------------------//
	basar::db::aspect::AccessorPropertyTable_YIterator	findUnprocessedPurchaseOrderPos( basar::db::aspect::AccessorPropertyTable_YIterator yit );

	//-------------------------------------------------------------------------------------------------------------------------//
	//	save - methods (updates, inserts)
	//-------------------------------------------------------------------------------------------------------------------------//
	basar::db::aspect::ExecuteResultInfo				makeInventoryUpdate(	basar::Int32 articleNo, basar::Int16 branchNo, 
																				basar::Int32 transactionDate, basar::Int32 transactionTime );
	//-------------------------------------------------------------------------------------------------------------------------//
	//	save - methods (updates, inserts)
	//-------------------------------------------------------------------------------------------------------------------------//
	const basar::db::aspect::ExecuteResultInfo			saveBookingsToPrint(	const basar::db::aspect::AccessorPropertyTable_YIterator yit	);
	//-------------------------------------------------------------------------------------------------------------------------//
	bool												checkUniqueTimestampByBranchArticle(	const basar::db::aspect::AccessorPropertyTable_YIterator yit	);
	//-------------------------------------------------------------------------------------------------------------------------//
	//	save - methods (updates, inserts)
	//-------------------------------------------------------------------------------------------------------------------------//
	void TransactionBegin();
	//-------------------------------------------------------------------------------------------------------------------------//
	//	save - methods (updates, inserts)
	//-------------------------------------------------------------------------------------------------------------------------//
	void TransactionCommit();
	//-------------------------------------------------------------------------------------------------------------------------//
	//	save - methods (updates, inserts)
	//-------------------------------------------------------------------------------------------------------------------------//
	void TransactionRollback();

	/*! \brief	With the given article no, the branch no and the username (both member variables - set by init()-method) 
				an insert is made in the table 'narclock'. 
				No transaction is used here as transactions are used in the invoking methods. \n no-throw */
	const basar::db::aspect::ExecuteResultInfo			lock( const basar::Int32 articleNo, const basar::Int16 branchNo );

	/*! \brief	To unlock the article with the passed article no, the concerning dataset in the table 'narclock' will be erased. 
				Considering the branchNo (member variable).
				No transaction is used here as transactions are used in the invoking methods. \n no-throw */
	const basar::db::aspect::ExecuteResultInfo			unlock( const basar::Int32 articleNo, const basar::Int16 branchNo );

	const basar::db::aspect::ExecuteResultInfo			unlockAllArticleLocksByUser();

private:
	bool		isNewNarcoticsProcess(basar::db::aspect::AccessorPropertyTable_YIterator yit);

	basar::db::aspect::AccessorInstanceRef				m_TransferACC;			//!< instance of accessor for selecting all row of halfyearyreport
	basar::db::aspect::AccessorInstanceRef				m_BranchesToTransferACC;
	basar::db::aspect::AccessorInstanceRef				m_CatalogACC;			//!< instance of accessor for selecting all row of halfyearyreport
	basar::db::aspect::AccessorInstanceRef				m_CustomerSupplierACC;	//!< instance of accessor for selecting all row of halfyearyreport
	basar::db::aspect::AccessorInstanceRef				m_CustomerMappingACC;	//!< instance of accessor for selecting the externe customer number
	basar::db::aspect::AccessorInstanceRef				m_NarcGroupACC;			//!< instance of accessor for selecting the group parameter for a Article 
	basar::Int32										m_Initialized;				//!< two meanings: a) to find out if the module is initialized at all and b) the method 'shutdown' is only run completly through if the module is not referenced any more.
	bool												m_inTransaction;
	boost::shared_ptr<libnarcotics::lockmanager::LockManager>	m_LockManager;
};

} // namespace domMod
} // namespace narcotics

#endif // GUARD_TRANSFERDM_H
