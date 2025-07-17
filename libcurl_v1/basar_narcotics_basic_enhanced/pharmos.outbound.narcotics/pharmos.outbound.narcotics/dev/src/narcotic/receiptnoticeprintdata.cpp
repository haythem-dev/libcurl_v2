//----------------------------------------------------------------------------
//includes
//----------------------------------------------------------------------------
#include "commonheader.h"
#include "receiptnoticeprintdata.h"

//----------------------------------------------------------------------------
//namespaces
//----------------------------------------------------------------------------
namespace narcotics
{

	//----------------------------------------------------------------------------
	ReceiptNoticePrintData::ReceiptNoticePrintData(	const basar::db::aspect::AccessorPropertyTableRef articleData,			//!< iterator containing transaction data
													const basar::db::aspect::AccessorPropertyTable_YIterator branchData,	//!< iterator containing branch data
													const basar::db::aspect::AccessorPropertyTable_YIterator customerData,	//!< iterator containing customer data
													const basar::db::aspect::AccessorPropertyTable_YIterator signaturelnk,	//!< iterator containing link to digital user signature
													basar::Int32 beginOfOrder,												//!< int declaring first booking of an order
													basar::Int32 endOfOrder 	) 
														: PrintDataBase(articleData,
																		branchData, 
																		customerData, 
																		signaturelnk,
																		beginOfOrder,
																		endOfOrder)
	{
	}

	//----------------------------------------------------------------------------
	ReceiptNoticePrintData::ReceiptNoticePrintData(	const basar::db::aspect::AccessorPropertyTableRef articleData,			//!< iterator containing transaction data
								const basar::db::aspect::AccessorPropertyTable_YIterator branchData,	//!< iterator containing branch data
								const basar::db::aspect::AccessorPropertyTable_YIterator customerData,	//!< iterator containing customer data
								const basar::db::aspect::AccessorPropertyTable_YIterator signaturelnk	//!< iterator containing link to digital user signature
								) 
	: PrintDataBase( articleData, branchData, customerData, signaturelnk, 0, static_cast<unsigned int>( articleData.size() ) )
	{
		
	}

	//----------------------------------------------------------------------------
	ReceiptNoticePrintData::~ReceiptNoticePrintData( void )
	{
	}

	//----------------------------------------------------------------------------
	basar::VarString ReceiptNoticePrintData::getFormName() const
	{
		return "BTMEmpfangsbest";
	}

	//----------------------------------------------------------------------------
	basar::VarString ReceiptNoticePrintData::getFormFileName() const
	{
		return "BTMEmpfangsbest";
	}

	//----------------------------------------------------------------------------		
	basar::VarString ReceiptNoticePrintData::getTitleLine()
	{
		return QApplication::translate("ReceiptNoticePrintData", "Receiptnotice").toLocal8Bit().data();
	}

	//----------------------------------------------------------------------------
	bool ReceiptNoticePrintData::printTour()
	{
		return true;
	}

	//----------------------------------------------------------------------------
	bool ReceiptNoticePrintData::printDeliveryNoteNumber()
	{
		return true;
	}

} //namespace narcotics