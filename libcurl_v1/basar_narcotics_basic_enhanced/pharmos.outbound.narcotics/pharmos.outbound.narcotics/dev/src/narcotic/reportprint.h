//----------------------------------------------------------------------------
/*! \file
 *  \brief  use case for printing reports
 *  \author Thomas Hörath
 *  \date   17.05.2006
 */
//----------------------------------------------------------------------------
#ifndef GUARD_REPORTPRINT_H
#define GUARD_REPORTPRINT_H

//---------------------------------------------------------------------------
//includes
//---------------------------------------------------------------------------
#include "usecasecontroller.h"

//---------------------------------------------------------------------------
//namespaces
//---------------------------------------------------------------------------
namespace narcotics
{

namespace useCase
{

//---------------------------------------------------------------------------
class ReportPrintUC : public UseCaseController
{
public:
	ReportPrintUC();
	~ReportPrintUC();
	
	virtual basar::appl::EventReturnType run();

	/*! \brief std method; has to be the first call in run() if events should be caught;
		\n no-throw */
	virtual void registerEventHandlers( const bool forSave	//!< register = true / deregister = false
										);

	/*! \brief show print preview of inventory count list
		\n no-throw */
	SYSTEM_EVENT_HANDLER_DECLARATION( PrintCountList )

    SYSTEM_EVENT_HANDLER_DECLARATION( PrintStockListQuick )
	
	/*! \brief show print preview of inventory stock list
		\n no-throw */
	SYSTEM_EVENT_HANDLER_DECLARATION( PrintStockList )
	
	/*! \brief show print preview of destruction voucher
		\n no-throw */
	SYSTEM_EVENT_HANDLER_DECLARATION( PrintDissolveReceipt )

private:
	/*! \brief reset pointer to print data / print preview
		\n no-throw */
	void init();

	/*! \brief print method for dissolve receipt
		\n no-throw */
	void printDissolveReceipt( basar::db::aspect::AccessorPropertyTableRef prTabDissolveReceiptList,
							   basar::db::aspect::AccessorPropertyTable_YIterator yitBranch,
							   basar::db::aspect::AccessorPropertyTable_YIterator yitDate );
};

} //namespace useCase
} //namespace narcotics

#endif //GUARD_REPORTPRINT_H