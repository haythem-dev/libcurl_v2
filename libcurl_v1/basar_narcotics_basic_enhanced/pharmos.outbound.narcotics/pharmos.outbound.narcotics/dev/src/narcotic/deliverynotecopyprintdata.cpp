//----------------------------------------------------------------------------
//includes
//----------------------------------------------------------------------------
#include "commonheader.h"
#include "deliverynotecopyprintdata.h"

//----------------------------------------------------------------------------
//namespaces
//----------------------------------------------------------------------------
namespace narcotics {

//----------------------------------------------------------------------------
DeliveryNoteCopyPrintData::DeliveryNoteCopyPrintData(	const basar::db::aspect::AccessorPropertyTableRef articleData,			//!< iterator containing transaction data
														const basar::db::aspect::AccessorPropertyTable_YIterator branchData,	//!< iterator containing branch data
														const basar::db::aspect::AccessorPropertyTable_YIterator customerData,	//!< iterator containing customer data
														const basar::db::aspect::AccessorPropertyTable_YIterator signaturelnk,	//!< iterator containing link to digital user signature
														basar::Int32 beginOfOrder,												//!< int declaring first booking of an order
														basar::Int32 endOfOrder 												//!< int declaring last booking of an order)
													)	: PrintDataBase(articleData,
																		branchData,
																		customerData,
																		signaturelnk,
																		beginOfOrder,
																		endOfOrder)
{
}

//----------------------------------------------------------------------------
DeliveryNoteCopyPrintData::~DeliveryNoteCopyPrintData(void)
{
}

//----------------------------------------------------------------------------
basar::VarString DeliveryNoteCopyPrintData::getFormName() const
{
	return "Lieferschein_Doppel";
}

//----------------------------------------------------------------------------
basar::VarString DeliveryNoteCopyPrintData::getFormFileName() const
{
	return "Lieferscheindoppel.ps";
}

//----------------------------------------------------------------------------	
basar::VarString DeliveryNoteCopyPrintData::getTitleLine()
{
	return QApplication::translate("DeliveryNoteCopyPrintData", "Deliverynotecopy").toLocal8Bit().data();
}

//----------------------------------------------------------------------------
bool DeliveryNoteCopyPrintData::printTour()
{
	return false;
}

//----------------------------------------------------------------------------
bool DeliveryNoteCopyPrintData::printDeliveryNoteNumber()
{
	return false;
}

} //namespace narcotics