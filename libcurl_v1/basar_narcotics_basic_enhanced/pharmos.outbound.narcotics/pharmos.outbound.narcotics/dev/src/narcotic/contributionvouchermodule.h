//-------------------------------------------------------------------------------------------------//
/*! \file 
 *  \brief  Domain module for handling contribution voucher entries
 *  \author Benjamin Endlicher
 *  \date   03.05.2010
 */
//-------------------------------------------------------------------------------------------------//
#ifndef GUARD_CONTRIBUTIONVOUCHERMODULE_H
#define GUARD_CONTRIBUTIONVOUCHERMODULE_H

//-------------------------------------------------------------------------------------------------//
// include section
//-------------------------------------------------------------------------------------------------//
#include "accessor_definitions.h"
#include "logmodule.h"
#include "constants.h"

//-------------------------------------------------------------------------------------------------//
// namespace section
//-------------------------------------------------------------------------------------------------//
namespace domMod
{
  //-------------------------------------------------------------------------------------------------//
  // class declaration
  //-------------------------------------------------------------------------------------------------//
  /*!\brief Domain module for handling contribution voucher entries */
	class ContributionVoucherDM
	{
	public:
		/*!\brief Default constructor.
		\n no-throw */
		ContributionVoucherDM();

		/*!\brief Default destructor.
		\n no-throw */
		virtual ~ContributionVoucherDM();

		//! \brief call this function to initialize accessor instances \n no-throw
		void init( const basar::Int16 branchNo,
				   const basar::db::aspect::ConnectionRef activeConn );

        //! \brief call this function to clear all accessor instances and the contents of property tables \n no-throw 
	    void shutdown();

        void injectLogDM( boost::shared_ptr<domMod::LogDM> logDM );

		//! \brief call this function to find bookings related to a special order  \n no-throw 
		//basar::db::aspect::AccessorPropertyTableRef findOrderPositionsAcknowledgement( basar::db::aspect::AccessorPropertyTable_YIterator acknowledgement );

		//! \brief call this function to find bookings related to a order  \n no-throw 
		//basar::db::aspect::AccessorPropertyTableRef findOrderPositions( basar::Int32 purchaseorderno, basar::Int32 transactiondate );

		//! \brief call this function to find bookings related to a needed delivery note copy  \n no-throw 
		basar::db::aspect::AccessorPropertyTableRef findOrderPositionsDelivery( basar::db::aspect::AccessorPropertyTable_YIterator deliveryNote );

		//! \brief call this function to find bookings related to a given order  \n no-throw 
		basar::db::aspect::AccessorPropertyTableRef findOrderPositionsRegister( basar::db::aspect::AccessorPropertyTable_YIterator orderIt );

		//! \brief call this function to find all acknowledgements by criteria of given searchIt  \n no-throw 
		basar::db::aspect::AccessorPropertyTableRef findAcknowledgements( basar::db::aspect::AccessorPropertyTable_YIterator searchIt );
		
        //basar::db::aspect::AccessorPropertyTableRef	getNewInsertedBooking( basar::db::aspect::AccessorPropertyTable_YIterator searchIt ); 

        basar::db::aspect::AccessorPropertyTableRef	getNewInsertedOrderAndSetToInPrint( basar::db::aspect::AccessorPropertyTable_YIterator searchIt );

        basar::db::aspect::AccessorPropertyTableRef getAllNewInsertedOrders( basar::db::aspect::AccessorPropertyTable_YIterator searchIt);

		//! \brief call this function in order to update acknowledgementstatus in narctransactioncatalog  \n no-throw 
		basar::db::aspect::ExecuteResultInfo setAcknowledgementstatus( basar::db::aspect::AccessorPropertyTableRef newState );
        
        basar::db::aspect::ExecuteResultInfo setAcknowledgementstatus( basar::db::aspect::AccessorPropertyTable_YIterator newStateYit );

        constants::ContributionVoucherScanResults markContributionVoucherAsReceived( basar::VarString scannedContributionVoucherNo );

		//! \brief call this function in order to update the acknowledgementstatus of a single order in narctransactioncatalog  \n no-throw 
		basar::db::aspect::ExecuteResultInfo setAcknowledgementstatusOfOrder( basar::db::aspect::AccessorPropertyTable_YIterator yitNewState );
		
		basar::db::aspect::AccessorPropertyTable_YIterator ContributionVoucherDM::findPersonalDataByID( basar::db::aspect::AccessorPropertyTable_YIterator yit );
		basar::db::aspect::ExecuteResultInfo	setContributionVoucherNo( basar::db::aspect::AccessorPropertyTable_YIterator voucherNo );

		//! \brief call this function to get contribution voucher property table \n no-throw 
		basar::db::aspect::AccessorPropertyTableRef getAccContributionVoucherPropTab();

		//! \brief call this function to get manual contribution voucher property table \n no-throw 
		basar::db::aspect::AccessorPropertyTableRef getAccManualContributionVoucherPropTab();
		
        void resetNotPrintedBookings( basar::db::aspect::AccessorPropertyTable_YIterator resetNotPrintedBookingsIt );

        bool areBookingsInPrint( basar::db::aspect::AccessorPropertyTable_YIterator checkForInPrintBookings );

        bool isContributionVoucherStillInPrint( basar::db::aspect::AccessorPropertyTable_YIterator checkForInPrintVoucher );

        bool checkUserHasPrintLock( basar::Int16 branchNo, const basar::VarString& userName );

        basar::db::aspect::AccessorPropertyTableRef findBookingsInPrint( basar::Int16 branchNo );

		//! \brief call this function to clear the content of contribution voucher property table \n no-throw 
	    void clearContributionVoucherPropTab();

		//! \brief call this function to clear the content of manual contribution voucher property table \n no-throw 
	    void clearManualContributionVoucherPropTab();

		//! \brief call this function to get delivery note copy property table \n no-throw 
		basar::db::aspect::AccessorPropertyTableRef getAccDeliveryNoteCopyPropTab();

		//! \brief call this function to clear the content of delivery note copy property table \n no-throw 
	    void clearDeliveryNoteCopyPropTab();

		//! \brief call this function to get no acknowledgement property table \n no-throw 
		basar::db::aspect::AccessorPropertyTableRef getAccNoAcknowledgementPropTab();

		//! \brief call this function to clear the content of no acknowledgement property table \n no-throw 
	    void clearNoAcknowledgementPropTab();

		//! \brief call this function to get order property table \n no-throw 
		basar::db::aspect::AccessorPropertyTableRef getAccOrderPropTab();

		//! \brief call this function to clear the content of order specific property table \n no-throw 
	    void clearOrderPropTab();

		//! \brief call this function to get search property table \n no-throw 
		basar::db::aspect::AccessorPropertyTableRef getAccSearchPropTab();

		//! \brief call this function to get print search property table \n no-throw 
		basar::db::aspect::AccessorPropertyTableRef getAccPrintSearchPropTab();

		//! \brief call this function to clear the content of search property table \n no-throw 
	    void clearSearchPropTab();

		//! \brief call this function to get property table new bookings \n no-throw 
		basar::db::aspect::AccessorPropertyTableRef getAccNewBookingsPropTab();

		//! \brief call this function to clear the content of property table containing new bookings \n no-throw 
	    void clearNewBookingsPropTab();

		//! \brief call this function to get property table for acknowledgement links \n no-throw 
		basar::db::aspect::AccessorPropertyTableRef getAccDocumentPropTab();

		//! \brief call this function to clear the content of property table link to acknowledgement \n no-throw 
	    void clearDocumentPropTab();

	private:
		/*! \brief copy constructor
		\n no-throw */
		ContributionVoucherDM( const ContributionVoucherDM& ContributionVoucherDM );
		
		/*! \brief assignment operator
		\n no-throw */
		ContributionVoucherDM& operator= ( const ContributionVoucherDM& ContributionVoucherDM );

        void splitScannedVoucherNo( const basar::VarString& scannedVoucherNo, basar::Int32& voucherTransactionDate, basar::Int32& voucherNo );

        bool hasContributionVoucherAlreadyBeenReceived( basar::VarString scannedContributionVoucherNo );

		basar::db::aspect::AccessorInstanceRef getContributionVoucherACC();

		basar::db::aspect::AccessorInstanceRef getManualContributionVoucherACC();

		basar::db::aspect::AccessorInstanceRef getDeliveryNoteCopyACC();

		basar::db::aspect::AccessorInstanceRef getNoAcknowledgementACC();

		basar::db::aspect::AccessorInstanceRef getOrderACC();
		
		basar::db::aspect::AccessorInstanceRef getSearchACC();

		basar::db::aspect::AccessorInstanceRef getPrintSearchACC();
		basar::db::aspect::AccessorInstanceRef getCustomerSupplierACC();

		basar::db::aspect::AccessorInstanceRef getNewBookingsACC();

		basar::db::aspect::ConnectionRef m_ConnectionRef;
		
		basar::Int16					 m_BranchNo;	//!< branch number 
		basar::Int32					 m_Initialized;	//!< Flag/Counter for the number of initializations, only the counts the number of domain modules

        boost::shared_ptr<domMod::LogDM> m_LogDM;

		//-------------------------------------------------------------------------------------
		// accessor instances ( created in init() ) which needs an active connection
		//-------------------------------------------------------------------------------------
        CREATE_ACCESSOR_INSTANCE_DECL( ContributionVoucher )
		CREATE_ACCESSOR_INSTANCE_DECL( ManualContributionVoucher )
		CREATE_ACCESSOR_INSTANCE_DECL( NoAcknowledgement )
		CREATE_ACCESSOR_INSTANCE_DECL( DeliveryNoteCopy )
		CREATE_ACCESSOR_INSTANCE_DECL( Order )
		CREATE_ACCESSOR_INSTANCE_DECL( Search )
		CREATE_ACCESSOR_INSTANCE_DECL( PrintSearch )
		CREATE_ACCESSOR_INSTANCE_DECL( NewBookings )
		CREATE_ACCESSOR_INSTANCE_DECL( CustomerSupplier )
	};

} //namespace domMod

#endif //GUARD_CONTRIBUTIONVOUCHERMODULE_H