#ifndef GUARD_GOODSTRANSACTION_H
#define GUARD_GOODSTRANSACTION_H

//-------------------------------------------------------------------------------------------------//
// includes
//-------------------------------------------------------------------------------------------------//
#include "usecasecontroller.h"

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
/*! \brief  class GoodsTransactionUC represents the use case class for displaying transaction data 
			(Deu: Bewegungsdaten).

    This is a final class.     
 */
class GoodsTransactionUC : public UseCaseController
{
public:
	/*! \brief	D'flt c'tor with a qt ctrls setup. \n  no-throw */
	GoodsTransactionUC();
	/*! \brief	D'tor with no (special) action. \n  no-throw */
	~GoodsTransactionUC();	
	
	/*! \brief	Initializes a dialog with a propertytable and shows it afterwards. \n  no-throw */
	basar::appl::EventReturnType run();
	
	/*! \brief std method; has to be the first call in run() if events should be caught;
		\n no-throw */
	void registerEventHandlers( const bool forSave	//!< register = true / deregister = false
										);

	/*! \brief	Matches the search criteria from the dialog to a search iterator, executes the search and 
				matches the hits to the dialog. \n  no-throw */
	SYSTEM_EVENT_HANDLER_DECLARATION( SearchGoodsTransaction )
	
	/*! \brief print list with articles to 1 customer or customers to 1 article. \n  no-throw */
	SYSTEM_EVENT_HANDLER_DECLARATION( PrintGoodsTransaction )

private:
	/*! \brief This method is used to match search criteria into Gui
		\n no-throw */
	void matchFromSearchCriteria();
	
	/*! \brief initializes a dialog with a propertytable and shows it afterwards. \n  no-throw */
	GoodsTransactionUC	( const GoodsTransactionUC& ); // forbidden
	
	/*! \brief initializes a dialog with a propertytable and shows it afterwards. \n  no-throw */
	GoodsTransactionUC& operator=( const GoodsTransactionUC& ); // forbidden
};

} //namespace useCase
} //namespace narcotics

#endif //GUARD_GOODSTRANSACTION_H