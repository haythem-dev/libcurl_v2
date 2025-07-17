#ifndef GUARD_CUSTOMERMAPPINGUC_H
#define GUARD_CUSTOMERMAPPINGUC_H

//-------------------------------------------------------------------------------------------------//
// includes
//-------------------------------------------------------------------------------------------------//
#include "usecasecontroller.h"
#include "customermappingdetailsvc.h"

//-------------------------------------------------------------------------------------------------//
// namespaces
//-------------------------------------------------------------------------------------------------//
namespace narcotics
{
namespace useCase
{

//-------------------------------------------------------------------------------------------------//
// class declaration
//-------------------------------------------------------------------------------------------------//
/*! \brief  class CustomerMappingUC represents the use case class for displaying / maintaining customer mappings

    This is a final class.     
 */
class CustomerMappingUC : public UseCaseController
{
public:
	/*! \brief	D'flt c'tor with a qt ctrls setup. \n  no-throw */
	CustomerMappingUC();
	/*! \brief	D'tor with no (special) action. \n  no-throw */
	~CustomerMappingUC();	
	
	/*! \brief	Initializes a dialog with a propertytable and shows it afterwards. \n  no-throw */
	basar::appl::EventReturnType run();
	
	/*! \brief std method; has to be the first call in run() if events should be caught;
		\n no-throw */
	void registerEventHandlers( const bool forSave	//!< register = true / deregister = false
										);

	/*! \brief	Matches the search criteria from the dialog to a search iterator, executes the search and 
				matches the hits to the dialog. \n  no-throw */
	SYSTEM_EVENT_HANDLER_DECLARATION( SearchCustomerMappings )

	SYSTEM_EVENT_HANDLER_DECLARATION( DeleteCustomerMapping )

	SYSTEM_EVENT_HANDLER_DECLARATION( EditCustomerMapping )

	SYSTEM_EVENT_HANDLER_DECLARATION( CreateCustomerMapping )

	SYSTEM_EVENT_HANDLER_DECLARATION( SaveCustomerMapping )
	
	SYSTEM_EVENT_HANDLER_DECLARATION( CheckCustomerExtern )
	SYSTEM_EVENT_HANDLER_DECLARATION( CheckCustomerIntern )

private:
	/*! \brief This method is used to match search criteria into Gui
		\n no-throw */
	void matchFromSearchCriteria();
	
	/*! \brief initializes a dialog with a propertytable and shows it afterwards. \n  no-throw */
	CustomerMappingUC	( const CustomerMappingUC& ); // forbidden
	
	/*! \brief initializes a dialog with a propertytable and shows it afterwards. \n  no-throw */
	CustomerMappingUC& operator=( const CustomerMappingUC& ); // forbidden

	basar::gui::tie::ViewConnPtr<viewConn::CustomerMappingDetailsVC>			m_CustomerMappingDetailsVC;
};

} //namespace useCase
} //namespace narcotics

#endif //GUARD_CUSTOMERMAPPINGUC_H