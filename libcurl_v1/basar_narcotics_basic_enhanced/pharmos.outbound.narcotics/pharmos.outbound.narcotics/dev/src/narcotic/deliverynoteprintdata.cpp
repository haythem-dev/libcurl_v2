//----------------------------------------------------------------------------
//includes
//----------------------------------------------------------------------------
#include "commonheader.h"
#include "deliverynoteprintdata.h"

//----------------------------------------------------------------------------
//namespaces
//----------------------------------------------------------------------------
namespace narcotics
{

//----------------------------------------------------------------------------
DeliveryNotePrintData::DeliveryNotePrintData(	const basar::db::aspect::AccessorPropertyTableRef articleData,			//!< iterator containing transaction data
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
DeliveryNotePrintData::~DeliveryNotePrintData(void)
{
}

//----------------------------------------------------------------------------
basar::VarString DeliveryNotePrintData::getFormName() const
{
	return "BTMAbgabemeldung";
}

//----------------------------------------------------------------------------
basar::VarString DeliveryNotePrintData::getFormFileName() const
{
	return "Abgabemeldung.ps";
}

//----------------------------------------------------------------------------	
basar::VarString DeliveryNotePrintData::getTitleLine()
{
	return QApplication::translate("DeliveryNotePrintData", "Deliverynote").toLocal8Bit().data();
}

//----------------------------------------------------------------------------
bool DeliveryNotePrintData::printTour()
{
	return false;
}

//----------------------------------------------------------------------------
bool DeliveryNotePrintData::printDeliveryNoteNumber()
{
	return false;
}

} //namespace narcotics 