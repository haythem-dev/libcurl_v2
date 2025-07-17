#ifndef GUARD_NARCOTICSAPPL_H
#define GUARD_NARCOTICSAPPL_H
//-------------------------------------------------------------------------------------------------//
/*! \file
 *  \brief  main interface for narcotics component
 *  \author Roland Kiefert
 *  \date   14.02.2006
 */
//-------------------------------------------------------------------------------------------------//

//-------------------------------------------------------------------------------------------------//
// includes 
//-------------------------------------------------------------------------------------------------//
#include "connection.h"
// dialog
#include "na01incominggoodsoverviewvc.h"
//#include "incominggoodsoverview.h"
#include "narcoticbook.h"
#include "narcoticscategory.h"
#include "comparebookingsvc.h"
#include "narcoticsprotocolvc.h"
#include "deletedbookingvc.h"
#include "halfyearlyreportvc.h"
#include "transactiondatavc.h"
#include "customermappingvc.h"
#include "differenceinstockvc.h"
#include "inputdatevc.h"
#include "orderoverviewvc.h"
#include "loginvc.h"
#include "resultsetoverview.h"

// use case
#include "incominggoodsuc.h"
//#include "incominggoods.h"
#include "bookingcatalog.h"
#include "narcoticcategory.h"
#include "comparebookings.h"
#include "narcoticsprotocol.h"
#include "deletedbookings.h"
#include "halfyearlyreportuc.h"
#include "goodstransaction.h"
#include "diffqtyinstockuc.h"
#include "reportprint.h"
#include "orderoverviewuc.h"
#include "resultsetsearch.h"
#include "customermappinguc.h"
#include "checkprinterconnectionuc.h"

// domain modules
#include "narcoticsmodule.h"
#include "narcoticsgroupmodule.h"
#include "logmodule.h"
#include "openordermodule.h"
#include "originalmodule.h"
#include "articlemodule.h"
#include "customermodule.h"
#include "suppliermodule.h"
#include "branchmodule.h"
#include "contributionvouchermodule.h"
#include "searchmodule.h"
#include "orderdm.h"

// services
#include "prerequisiteschecker.h"
#include "servicelocatorptr.h"
#include "servicelocator.h"
#include "ftpservice.h"

#include <lockmanager/branchlockmanager.h>
#include "printthread.h"

#pragma warning (push) // to avoid warnings from QT always put QT-includes between push - pop
#pragma warning(disable: 4127 4244 4311 4312 4481 4512 4800)
#include <QtCore/QTranslator>
#pragma warning (pop)
//-------------------------------------------------------------------------------------------------//
using basar::gui::tie::ViewConnPtr;

//-------------------------------------------------------------------------------------------------//
// namespaces
//-------------------------------------------------------------------------------------------------//
namespace narcotics
{
	class PrintThread;
//-------------------------------------------------------------------------------------------------//
// class declaration
//-------------------------------------------------------------------------------------------------//
/*! \brief  One and only instance for using narcotics functions.

	\n This is a final class.  
	\n throws no-throw */
//-------------------------------------------------------------------------------------------------//
class NarcoticsApplBase
{
	//---------------------------------------------------------//
	// friends
	//---------------------------------------------------------//
	friend class basar::cmnutil::Singleton<NarcoticsApplBase>; //!< singleton as friend for construction!
	friend class viewConn::CreateProcessNarcoticsBookingVC;	//!< view conn class needs private access to current YIterators

	friend class useCase::IncomingGoodsUC;		 //!< use case class needs private access to current YIterators
	friend class useCase::BookingCatalogUC;		 //!< use case class needs private access to current YIterators
	friend class useCase::NarcoticsCategoryUC;	 //!< use case class needs private access to current YIterators
	friend class useCase::CompareBookingsUC;	 //!< use case class needs private access to current YIterators
	friend class useCase::NarcoticsProtocolUC;	 //!< use case class needs private access to current YIterators
	friend class useCase::DeletedBookingsUC;	 //!< use case class needs private access to current YIterators
	friend class useCase::HalfYearlyReportUC;	 //!< use case class needs private access to current YIterators
	friend class useCase::GoodsTransactionUC;	 //!< use case class needs private access to current YIterators
	friend class useCase::CustomerMappingUC;	 //!< use case class needs private access to current YIterators
	friend class useCase::DiffQtyInStockUC;		 //!< use case class needs private access to current YIterators
	friend class useCase::ReportPrintUC;		 //!< use case class needs private access to current YIterators
	friend class useCase::OrderOverviewUC;		 //!< use case class needs private access to current YIterators
	friend class useCase::ResultsetSearch;		 //!< use case class needs private access to current YIterators

public:
	//---------------------------------------------------------//
	// typedef
	//---------------------------------------------------------//
	typedef boost::shared_ptr< NarcoticsApplBase >    NarcoticsApplSharedPtr;	//!< shared pointer to NarcoticsAppl
	
	boost::shared_ptr< domMod::OrderDM > getOrderDM();
	//---------------------------------------------------------//
	// static
	//---------------------------------------------------------//
	/*! \brief The method creates a NarcoticsAppl. \n no-throw */
	static NarcoticsApplSharedPtr create();

	/*! \brief destructor \n no-throw */
	virtual ~NarcoticsApplBase();

	PrintThread* getPrintThread();

	boost::shared_ptr< domMod::LogDM > getLogDM();

	//############################################################################################################//
	// dialogs
	//############################################################################################################//
	/*! \brief initialize narcotics applctrl \n no-throw */
	bool init( bool checkDifferenceQtyInStock = true );
	
	/*! \brief This method show the Dialogs for incoming goods at work about the usecase \n no-throw */
	void startIncomingGoods(QWidget * mw);

	/*! \brief show the dialog nacotics book \n no-throw */
	void startNarcoticsBook(QWidget * mw);

	/*! \brief show the dialog narcotics category \n no-throw */
	void startNarcoticsCategory(QWidget * mw);

	/*! \brief show the dialog compare narcotics bookings with original bookings \n no-throw */
	void startCompareNarcoticsOriginal(QWidget * mw);

	/*! \brief show the dialog narcotics protocol \n no-throw */
	void startNarcoticsProtocol(QWidget * mw);

	/*! \brief show the dialog for the half yearly notification \n no-throw */
	void startHalfYearlyNotification(QWidget * mw);

	/*! \brief show the dialog for the difference between original and nacotics data \n no-throw */
	void startDiffQtyInStock(QWidget * mw);
	
	/*! \brief show the dialog for the difference between original and nacotics data \n no-throw */
	void startTransactionData(QWidget * mw);

	void startCustomerMapping(QWidget * mw);

	/*! \brief show print preview window for stock list
		\n	   no-throw */
	void startStockList(QWidget * mw);

    void startStockListQuick(QWidget * mw);
	/*! \brief show print preview window for inventory count list
		\n	   no-throw */
	void startCountList(QWidget * mw);
	/*! \brief show print preview window for destruction voucher of current date
		\n	   no-throw */
	void startDissolveReceipt(QWidget * mw);
	
    /*! \brief This method show the Dialogs for incoming goods at work about the usecase 
		\n	   no-throw */
	void startOrderOverview(QWidget * mw);

	 /*! \brief This method shows the contribution voucher dialog
		\n	   no-throw */
	void startContributionVoucher(QWidget * mw);

    /*! \brief This method shows the contribution voucher scan dialog
	\n	   no-throw */
    void startContributionVoucherScan(QWidget * mw);

	/*! \brief opens login dialog to change active user during session
		\n	   no-throw */
	bool startLogin(QWidget * mw);

	/*! \brief start printing contribution vouchers
		\n	   no-throw */
	void startPrint();

	/*! \brief start printing contribution vouchers
		\n	   no-throw */
	void stopElectronicContributionVoucherPrint();

    void startResolvePrintingErrors(QWidget * mw);

	void startCheckPrinterConnection();

	/*! \brief shows whether the proper parameter in uadm is set for enabling electronic transfer to bfarm (0,1 are possible values)
		\n	   no-throw */
	bool isNewNarcoticsProcess( basar::VarString branchNoTxt );

	const bool isNewNarcoticsProcess() const;

	/*! \brief shows whether the application has accquired the lock for printing in this branch
		\n	   no-throw */
	const bool hasLockForPrinting() const;

	void releaseAllPrintLocks();

	void releaseTransferLocks();

	/*! \brief get name of printer selected for current branch
		\n	   no-throw */
	const basar::I18nString & getPrinterName() const;

	/*! \brief catch fire thrown by loginvc	\n no-throw */
	SYSTEM_EVENT_HANDLER_DECLARATION( Relogin )
	
	/*! \brief checks whether the prerequisites for starting the electronic print process are fullfilled (signatures)	\n no-throw */
	bool areElectronicDeliveryNotePrerequisitesFullfilled();

private:
	NarcoticsApplBase				();
	NarcoticsApplBase				( const NarcoticsApplBase & rCopy );	//!< forbidden copy constructor
	NarcoticsApplBase& operator =	( const NarcoticsApplBase & rCopy );	//!< forbidden assignment operator

	/*! \brief std method; has to be the first call in run() if events should be caught;
		\n no-throw */
	virtual void registerEventHandlers( const bool forSave	//!< register = true / deregister = false
										);
	
	/*! \brief Returns the default text shown in the case of an basar exception. \n no-throw */
    basar::VarString            getExceptionDefaultText( const basar::VarString& txt );
	
	void						shutdown();

	/*! \brief Function checks if connection is still active in order to see if shutdown-method has already been called. \n no-throw */
	bool						isActive();

	/*! \brief check if user has given right after login \n no-throw */
	bool						checkUserRight( basar::ConstString rightName //!< name of right to check
												);

	/*! \brief checks the prerequisites that have to be fullfilled for starting the application \n no-throw */
	bool						arePrerequisitesFullfilled();

    /*! \brief  method to fill initialization parameters for printhread (needed for not using I18nStrings in Printthread)*/
    void                        fillPrintThreadInitParams( PrintThreadInitParams& p );
	
	bool loadTranslators();
	
	//---------------------------------------------------------//
	// Translators
	//---------------------------------------------------------//
	QTranslator m_trGuiQt;
	QTranslator m_trQt;
	QTranslator m_trNarcotic;

	//---------------------------------------------------------//
	// members
	//---------------------------------------------------------//
	basar::db::aspect::AccessorPropertyTable_YIterator	m_currentBooking;		//!< iterator to single narcotics booking

	boost::shared_ptr< narcotic::useCase::services::PrerequisitesCheckerSV > m_PrerequisitesCheckerSV;

	boost::shared_ptr<libnarcotics::lockmanager::BranchLockManager> m_LockManager;		//!< class for locking business objects

	//---------------------------------------------------------//
	// DomainModules
	//---------------------------------------------------------//
	boost::shared_ptr< domMod::NarcoticsDM >			m_NarcoticsDM;			//!< domain module class for narcotics
	domMod::NarcoticsDM									m_DeletedDM;			//!< domain module class for narcotics deleted bookings
	boost::shared_ptr< domMod::NarcoticsGroupDM >		m_NarcoticsGroupDM;		//!< domain module class for narcotics group
	boost::shared_ptr< domMod::LogDM >					m_LogDM;				//!< domain module class for log
	domMod::OpenOrderDM									m_OpenOrderDM;			//!< domain module class for open Order
	domMod::OriginalDM									m_OriginalDM;			//!< domain module class for narcotics
	domMod::ArticleDM									m_ArticleDM;			//!< domain module class for article
	boost::shared_ptr< domMod::CustomerDM >				m_CustomerDM;			//!< domain module class for customer
	boost::shared_ptr< domMod::SupplierDM >				m_SupplierDM;			//!< domain module class for supplier
	boost::shared_ptr< domMod::BranchDM >				m_BranchDM;				//!< domain module class for branch
	boost::shared_ptr< domMod::ContributionVoucherDM >	m_ContributionVoucherDM;//!< domain module class for contribution vouchers
	boost::shared_ptr< domMod::SearchDM >				m_SearchDM;				//!< domain module class for searching
	boost::shared_ptr< domMod::OrderDM >				m_OrderDM;

	//---------------------------------------------------------//
	// UseCases
	//---------------------------------------------------------//
	useCase::IncomingGoodsUC							m_IncomingGoodsUC;		//!< use case for incoming goods
	useCase::BookingCatalogUC							m_BookingCatalogUC;		//!< use case for narcotics book 
	useCase::NarcoticsCategoryUC						m_NarcoticsCategoryUC;	//!< use case for narcotics category
	useCase::CompareBookingsUC							m_CompareBookingsUC;	//!< use case for compare bookings between orinal catalog and narcotics catalog
	useCase::NarcoticsProtocolUC						m_NarcoticsProtocolUC;	//!< use case for protocol
	useCase::DeletedBookingsUC							m_DeletedBookingsUC;	//!< use case for deleted bookings
	useCase::HalfYearlyReportUC							m_HalfYearlyReportUC;	//!< use case for half yearly report
	useCase::GoodsTransactionUC							m_GoodsTransactionUC;	//!< use case for transaction data
	useCase::CustomerMappingUC							m_CustomerMappingUC;	//!< use case for customer mapping
	useCase::DiffQtyInStockUC							m_DiffQtyInStockUC;		//!< use case for difference quantity in stock
	useCase::ReportPrintUC								m_ReportPrintUC;		//!< use case for printing different reports
	useCase::OrderOverviewUC							m_OrderOverviewUC;		//!< use case for order overview
	useCase::ResultsetSearch							m_ResultsetSearch;      //!< use case contains methods for dealing with resultsetoverviewvc

	//---------------------------------------------------------//
	// ViewConnectors
	//---------------------------------------------------------//
	ViewConnPtr< viewConn::IncomingGoodsOverviewVC >		m_IncomingGoodsOverviewVC;			//!< view connector for incoming goods
	ViewConnPtr< viewConn::NarcoticBookVC >					m_NarcoticBookVC;					//!< view connector for narcotics book
	ViewConnPtr< viewConn::NarcoticCategoryVC >				m_NarcoticCategoryVC;				//!< view connector for narcotics category
	ViewConnPtr< viewConn::CompareBookingsVC >				m_CompareBookingsVC;				//!< view connector for compare bookings between orinal catalog and narcotics catalog
	ViewConnPtr< viewConn::NarcoticsProtocolVC >			m_NarcoticsProtocolVC;				//!< view connector for protocol
	ViewConnPtr< viewConn::DeletedBookingVC >				m_DeletedBookingsVC;				//!< view connector for deleted bookings
	ViewConnPtr< viewConn::HalfYearlyReportVC >				m_HalfYearlyReportVC;				//!< view connector for half yearly report
	ViewConnPtr< viewConn::TransactionDataVC >				m_TransactionDataVC;				//!< view connector for transaction data
	ViewConnPtr< viewConn::CustomerMappingVC >				m_CustomerMappingVC;				//!< view connector for customer mappings
	ViewConnPtr< viewConn::DifferenceInStockVC >			m_DiffQtyInStockVC;					//!< view connector for difference quantity in stock
	ViewConnPtr< viewConn::InputDateVc >					m_InputDateVc;						//!< view connector for input date
	ViewConnPtr< viewConn::OrderOverviewVC >				m_OrderOverviewVC;					//!< view connector for order overview
	ViewConnPtr< viewConn::LoginVC >						m_LoginVC;							//!< view connector for changing active user during a session
	ViewConnPtr< viewConn::ResultSetOverviewVC >			m_ResultSetOverviewVC;				//!< view connector for detailed dialog for search with name as searchcriteria

	//---------------------------------------------------------//
	// Connection
	//---------------------------------------------------------//
	Connection									m_Connection;					//!< connection for the dll narcotics
	bool										m_Initialized;					//!< member defined in order to declare state of initialization
	bool										m_Relogin;						//!< member defined in order to declare relogin state
	bool										m_ElectronicContributionVoucher;//<! NEW_NARCOTIC_PROCESS Elektronischer Abgabebeleg aktiviert
	bool										m_HasPrintLock;					//!< NEW_NARCOTIC_PROCESS print lock accquired

	//---------------------------------------------------------//
	// default printer
	//---------------------------------------------------------//
	basar::I18nString									m_PrinterName;
	PrintThread*                                        m_PrintThread;

	//---------------------------------------------------------//
	// services
	//---------------------------------------------------------//
    infrastructure::ServiceLocatorPtr        m_ServiceLocator;

    infrastructure::FtpParameters            getFtpParameters();
};

//-------------------------------------------------------------------------------------------------//
class NarcoticsAppl : public basar::cmnutil::Singleton<NarcoticsApplBase>
{
private:
	NarcoticsAppl();
	NarcoticsAppl( const NarcoticsAppl& r );
	NarcoticsAppl& operator = ( const NarcoticsAppl& r );
};

//-------------------------------------------------------------------------------------------------//

} //namespace narcotics

//-------------------------------------------------------------------------------------------------//

#endif //GUARD_NARCOTICSAPPL_H

//-------------------------------------------------------------------------------------------------//
