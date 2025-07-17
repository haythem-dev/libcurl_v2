#ifndef GUARD_CONTRIBUTIONVOUCHER_H
#define GUARD_CONTRIBUTIONVOUCHER_H
//-----------------------------------------------------------------------------------------------//
/*! \file
 *  \brief  shows missing acknowledgements, prints contribution vouchers and creates delivery note copies
 *  \author Benjamin Endlicher
 *  \date   14.09.2010
 */
//-------------------------------------------------------------------------------------------------//
//includes
//-------------------------------------------------------------------------------------------------//
#include "usecasecontroller.h"
#include "contributionvoucherftptransfer.h"

//viewconnectors
#include "contributionvouchervc.h"

//services
#include "servicelocatorptr.h"

//dommods
#include "narcoticsmodule.h"
#include "customermodule.h"
#include "branchmodule.h"
#include "searchmodule.h"
#include "contributionvouchermodule.h"

//-------------------------------------------------------------------------------------------------//
// forward declaration
//-------------------------------------------------------------------------------------------------//
namespace narcotics {
	namespace viewConn {
		class NA18DeliveryNoteCopyVC;
	}
}
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
/*! \brief  Use Case class for dealing with acknowledgements and delivery note copies.

	\n This is a final class.  
	\n throws no-throw */
//-------------------------------------------------------------------------------------------------//
class ContributionVoucherUC : public UseCaseController
{
public:
	/*! \brief The constructor for the use case incoming goods
		\n no-throw */
	ContributionVoucherUC();
	
	/*! \brief The destructor for the use case incoming goods
		\n no-throw */
	~ContributionVoucherUC();

	/*! \brief methods starts usecase contributionvoucher
		\n no-throw */
	basar::appl::EventReturnType run();

	void injectParentWindow			( QWidget * mw );
    void injectBranchDM				( boost::shared_ptr<domMod::BranchDM> branchDM								);
    void injectCustomerDM			( boost::shared_ptr<domMod::CustomerDM> customerDM							);
    void injectContributionVoucherDM( boost::shared_ptr<domMod::ContributionVoucherDM> contributionVoucherDM	);
    void injectNarcoticsDM			( boost::shared_ptr<domMod::NarcoticsDM> narcoticsDM						);
    void injectSearchDM				( boost::shared_ptr<domMod::SearchDM> searchDM								);
    void injectServiceLocator		( infrastructure::ServiceLocatorPtr sp										);

	/*! \brief std method; has to be the first call in run() if events should be caught;
		\n no-throw */
	void registerEventHandlers( const bool forSave	//!< register = true / deregister = false
										);
	/*! \brief print delivery note copy for selected order
		\n no-throw */
	SYSTEM_EVENT_HANDLER_DECLARATION( PrintDeliveryNoteCopy );

	/*! \brief update acknowledgementstatus of bookings belonging to selected order as ACKNOWLEDGEMENT_RECEIVED
		\n no-throw */
	SYSTEM_EVENT_HANDLER_DECLARATION( MarkAsReceived );

	/*! \brief update acknowledgementstatus of bookings belonging to selected order as ACKNOWLEDGEMENT_PRINTED
		\n no-throw */
	SYSTEM_EVENT_HANDLER_DECLARATION( MarkAsPrinted );
	
	/*! \brief open empty deliverynote with systemwide default application
		\n no-throw */
	SYSTEM_EVENT_HANDLER_DECLARATION( OpenEmptyDeliveryNote );

	/*! \brief open empty contributionvoucher and deliverynote copy  with systemwide default application
		\n no-throw */
	SYSTEM_EVENT_HANDLER_DECLARATION( OpenEmptyContributionVoucherDelCopy );

	/*! \brief reprint the selected deliverynote
		\n no-throw */
	SYSTEM_EVENT_HANDLER_DECLARATION( ReprintDeliveryNote );

	/*! \brief search an order for given details 
		\n no-throw */
	SYSTEM_EVENT_HANDLER_DECLARATION( SearchAcknowledgements );

	/*! \brief refresh table widget changed acknowledgements
		\n no-throw */
	SYSTEM_EVENT_HANDLER_DECLARATION( RefreshAcknowledgement );

    SYSTEM_EVENT_HANDLER_DECLARATION( PrintAcknowledgementList );
	
private:
	ContributionVoucherUC	( const ContributionVoucherUC& ); // forbidden
	ContributionVoucherUC& operator=				( const ContributionVoucherUC& ); // forbidden

	void reprintDeliveryNote(	basar::db::aspect::AccessorPropertyTableRef articledata, 
								basar::db::aspect::AccessorPropertyTable_YIterator yitBranch, 
								basar::db::aspect::AccessorPropertyTable_YIterator yitCustomer, 
								basar::db::aspect::AccessorPropertyTable_YIterator yitLink, 
								basar::Int32 beginOrder, 
								basar::Int32 endOrder	);

    void checkInjections();

	/*! \brief method to handle real print call
		\n no-throw */
	void transferDeliveryNoteCopy( basar::db::aspect::AccessorPropertyTableRef articleData, 
					 basar::db::aspect::AccessorPropertyTable_YIterator yitBranch, 
					 basar::db::aspect::AccessorPropertyTable_YIterator yitCustomer, 
					 basar::Int32 startPosition, 
					 basar::Int32 endPosition );

	///*! \brief method to assign order bookings to a form for printing
	//	\n no-throw */
	void printDeliveryNoteCopy(	basar::db::aspect::AccessorPropertyTableRef articleData, 
								basar::db::aspect::AccessorPropertyTable_YIterator yitBranch,
								basar::db::aspect::AccessorPropertyTable_YIterator yitCustomer );

	basar::VarString m_ChangedAcknowledgementLink;
	basar::VarString m_PrintFolderPath;

    basar::gui::tie::ViewConnPtr<viewConn::ContributionVoucherVC>	m_ContributionVoucherVC;
    basar::gui::tie::ViewConnPtr<viewConn::NA18DeliveryNoteCopyVC>	m_NA18DeliveryNoteCopyVC;

    //DMs
    boost::shared_ptr<domMod::CustomerDM>				m_CustomerDM;			//!< domain module class for customer
	boost::shared_ptr<domMod::BranchDM>					m_BranchDM;				//!< domain module class for branch
	boost::shared_ptr<domMod::ContributionVoucherDM>	m_ContributionVoucherDM;//!< domain module class for contribution vouchers
	boost::shared_ptr<domMod::SearchDM>					m_SearchDM;				//!< domain module class for searching
    boost::shared_ptr<domMod::NarcoticsDM>			    m_NarcoticsDM;			//!< domain module class for narcotics

    //Service Locator
    infrastructure::ServiceLocatorPtr m_ServiceLocatorPtr;
};

} //namespace useCase
} //namespace narcotics

#endif //GUARD_CONTRIBUTIONVOUCHER_H