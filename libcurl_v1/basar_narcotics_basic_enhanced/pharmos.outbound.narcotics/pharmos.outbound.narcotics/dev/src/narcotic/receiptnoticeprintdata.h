#ifndef RECEIPTNOTICEPRINTDATA_H_GUARD
#define RECEIPTNOTICEPRINTDATA_H_GUARD
//----------------------------------------------------------------------------
//includes
//----------------------------------------------------------------------------
#include "printdatabase.h"

//----------------------------------------------------------------------------
//namespaces
//----------------------------------------------------------------------------
namespace narcotics
{

class ReceiptNoticePrintData :
	public PrintDataBase
{
public:
	ReceiptNoticePrintData(	const basar::db::aspect::AccessorPropertyTableRef articleData,			//!< iterator containing transaction data
							const basar::db::aspect::AccessorPropertyTable_YIterator branchData,	//!< iterator containing branch data
							const basar::db::aspect::AccessorPropertyTable_YIterator customerData,	//!< iterator containing customer data
							const basar::db::aspect::AccessorPropertyTable_YIterator signaturelnk	//!< iterator containing link to digital user signature
							);
    ReceiptNoticePrintData(	const basar::db::aspect::AccessorPropertyTableRef articleData,			//!< iterator containing transaction data
						const basar::db::aspect::AccessorPropertyTable_YIterator branchData,	//!< iterator containing branch data
						const basar::db::aspect::AccessorPropertyTable_YIterator customerData,	//!< iterator containing customer data
						const basar::db::aspect::AccessorPropertyTable_YIterator signaturelnk,	//!< iterator containing link to digital user signature
						basar::Int32 beginOfOrder,												//!< int declaring first booking of an order
						basar::Int32 endOfOrder 	);

    void reprintDeliveryNote( basar::db::aspect::AccessorPropertyTable_YIterator noacknowledgementIt );

	virtual ~ReceiptNoticePrintData(void);

protected:
	basar::VarString getFormName() const;
	basar::VarString getFormFileName() const;
	basar::VarString getTitleLine();

	bool printTour();
	bool printDeliveryNoteNumber();
};

} //namespace narcotics

#endif //RECEIPTNOTICEPRINTDATA_H_GUARD