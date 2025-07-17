#ifndef CONTRIBUTIONVOUCHERPRINTDATA_H_GUARD
#define CONTRIBUTIONVOUCHERPRINTDATA_H_GUARD
//----------------------------------------------------------------------------
//includes
//----------------------------------------------------------------------------
#include "printdatabase.h"

//----------------------------------------------------------------------------
//namespaces
//----------------------------------------------------------------------------
namespace narcotics
{

class ContributionVoucherPrintData :
	public PrintDataBase
{
public:
	ContributionVoucherPrintData( const basar::db::aspect::AccessorPropertyTableRef articleData,	//!< iterator containing transaction data
						const basar::db::aspect::AccessorPropertyTable_YIterator branchData,	//!< iterator containing branch data
						const basar::db::aspect::AccessorPropertyTable_YIterator customerData,	//!< iterator containing customer data
						const basar::db::aspect::AccessorPropertyTable_YIterator signaturelnk,	//!< iterator containing link to digital user signature
						basar::Int32 beginOfOrder,												//!< int declaring first booking of an order
						basar::Int32 endOfOrder,
						basar::Int16 branchno );
						
	virtual ~ContributionVoucherPrintData( void );
	
protected:
	
	basar::VarString getFormName() const;
	basar::VarString getFormFileName() const;
	basar::VarString getTitleLine();

	bool printTour();
	bool printDeliveryNoteNumber();

private:
	basar::Int16 m_Branchno;
};

} //namespace narcotics

#endif //CONTRIBUTIONVOUCHERPRINTDATA_H_GUARD