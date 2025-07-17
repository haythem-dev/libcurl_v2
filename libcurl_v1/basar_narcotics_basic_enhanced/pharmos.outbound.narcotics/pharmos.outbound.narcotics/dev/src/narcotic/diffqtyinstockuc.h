#ifndef GUARD_QTYINSTOCKUC_H
#define GUARD_QTYINSTOCKUC_H
//-------------------------------------------------------------------------------------------------//
/*! \file
 *  \brief  main interface for use case class difference quantity in stock
 *  \author Roland Kiefert
 *  \date   26.04.2006
 */
//-------------------------------------------------------------------------------------------------//
// includes
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
/*! \brief  Use Case class for use case difference quantity in stock.

	\n This is a final class.  
	\n throws no-throw */
//-------------------------------------------------------------------------------------------------//
class DiffQtyInStockUC : public UseCaseController
{
public:
	/*! \brief The constructor for the use case difference quantity in stock.\n no-throw */
	DiffQtyInStockUC();
	/*! \brief The destructor for the use case difference quantity in stock. \n no-throw */
	~DiffQtyInStockUC();
	
	/*! \brief This method starts the use case difference quantity in stock. \n no-throw */
	basar::appl::EventReturnType	run(); 

	/*! \brief std method; has to be the first call in run() if events should be caught;
		\n no-throw */
	void registerEventHandlers( const bool forSave	//!< register = true / deregister = false
										);

	/*! \brief	. \n no-throw */
	SYSTEM_EVENT_HANDLER_DECLARATION( UpdateDiffQtyInStock )

	/*! \brief	This method shows the view connector (dialog) displaying the narcotic bookings 
				with divergence in qty in stock in comparison to the PHARMOS original bookings. \n no-throw */
	SYSTEM_EVENT_HANDLER_DECLARATION( ShowComparingBookings )
	

private:
	DiffQtyInStockUC	( const DiffQtyInStockUC& ); // forbidden
	DiffQtyInStockUC& operator=( const DiffQtyInStockUC& ); // forbidden
};

} //namespace useCase
} //namespace narcotics

#endif //GUARD_QTYINSTOCKUC_H