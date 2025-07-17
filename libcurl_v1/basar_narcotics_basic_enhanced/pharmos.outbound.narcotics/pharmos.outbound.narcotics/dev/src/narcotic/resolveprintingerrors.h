//------------------------------------------------------------------------------
/*! \file
 *  \brief  class for finding resultset to given name
 *  \author Marco Köppendörfer
 *  \date   06.09.2012
 */
//------------------------------------------------------------------------------

#ifndef GUARD_NARCOTICS_RESOLVEPRINTINGERRORS_H
#define GUARD_NARCOTICS_RESOLVEPRINTINGERRORS_H

//------------------------------------------------------------------------------
//includes
//------------------------------------------------------------------------------
#include "usecasecontroller.h"
#include "resolveprintingerrorsvc.h"
#include "orderdetailsvc.h"

// to instantiate members of domain modules
#include "narcoticsmodule.h"
#include "narcoticsgroupmodule.h"
#include "customermodule.h"

//dommods
#include "contributionvouchermodule.h"

//serivce
#include "servicelocatorptr.h"

//------------------------------------------------------------------------------
//namespaces
//------------------------------------------------------------------------------
namespace narcotics	
{
namespace useCase
{

//-------------------------------------------------------------------------------------------------//
// class declaration
//-------------------------------------------------------------------------------------------------//
/*! \brief  class ResultsetSearch represents the use case class for conducting resultset search.

    This is a final class.     
 */
class ResolvePrintingErrors : public UseCaseController
{
public:
	/*! \brief	D'flt c'tor. \n  no-throw */
	ResolvePrintingErrors( basar::Int16 branchNo );
	
	/*! \brief	D'tor with no (special) action. \n  no-throw */
	~ResolvePrintingErrors();	

		/*! \brief	Initializes a dialog with a propertytable and shows it afterwards. \n  throws missinginjectionexception */
	basar::appl::EventReturnType run();
	
    void init();

	/*! \brief std method; has to be the first call in run() if events should be caught;
		\n no-throw */
	void registerEventHandlers( const bool forSave	//!< register = true / deregister = false
										);

	/*! \brief This method is used to match corresponding number for given name into search iterator
		\n no-throw */
	void matchNumberForName( basar::I18nString searchcriteria, bool detailDialogShown );

	/*! \brief This method gets current selected entry from detail dialog 
		\n no-throw */
	basar::appl::EventReturnStruct getCurrentSelection( basar::I18nString searchcriteria );

	SYSTEM_EVENT_HANDLER_DECLARATION( ResolvePrintingErrors )

    SYSTEM_EVENT_HANDLER_DECLARATION( DisplayOrderDetailsRequested )

    void injectContributionVoucherDM( boost::shared_ptr<domMod::ContributionVoucherDM> contributionVoucherDM );
    
    void injectNarcoticsDM( boost::shared_ptr<domMod::NarcoticsDM> narcoticsDM );
    
    void injectSerivceLocator( infrastructure::ServiceLocatorPtr serviceLocator );

	void injectParentWindow			( QWidget * mw );
    

private:
	/*! \brief initializes a dialog with a propertytable and shows it afterwards. \n  no-throw */
	ResolvePrintingErrors( const ResolvePrintingErrors& ); // forbidden

	/*! \brief initializes a dialog with a propertytable and shows it afterwards. \n  no-throw */
	ResolvePrintingErrors& operator=	  ( const ResolvePrintingErrors& ); // forbidden

    /*! \brief checks if all nessecary injections took place. \n  throws missinginjectionexception */
    void checkInjections();

    /*! \brief fills tableWidget with bookings marked as In_Print*/
    void populateTableWithBookingsInPrint();
    void setContributionVoucherNoAsPrinted( basar::Int32 contributionVoucherNo, basar::Int32 transactionDate, basar::Int32 purchaseOrderNo, basar::Int32 customerSupplierNo, constants::ResolvePrintingErrorsTasks tasks );
    bool isInPrintBookingAlreadyTransfered( basar::db::aspect::AccessorPropertyTable_YIterator selectedBooking );

    const basar::cmnutil::Date getLastSundaysDate();

    //member
	basar::Int16													m_BranchNo;

    basar::gui::tie::ViewConnPtr<viewConn::ResolvePrintingErrorsVC> m_ResolvePrintingErrorsVC;
    basar::gui::tie::ViewConnPtr<viewConn::OrderDetailsVC>			m_OrderDetailsVC;

    boost::shared_ptr<domMod::ContributionVoucherDM>				m_ContributionVoucherDM;
    boost::shared_ptr<domMod::NarcoticsDM>							m_NarcoticsDM;

    infrastructure::ServiceLocatorPtr								m_ServiceLocatorPtr;
};

} //namespace useCase
} //namespace narcotics

#endif //GUARD_NARCOTICS_RESOLVEPRINTINGERRORS_H