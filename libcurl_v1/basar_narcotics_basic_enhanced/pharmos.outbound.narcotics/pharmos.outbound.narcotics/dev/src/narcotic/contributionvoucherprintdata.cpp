//----------------------------------------------------------------------------
//includes
//----------------------------------------------------------------------------
#include "commonheader.h"
#include "printdatabase.h"
#include "contributionvoucherprintdata.h"
#include "contributionvoucherhelper.h"

//----------------------------------------------------------------------------
//namespaces
//----------------------------------------------------------------------------
namespace narcotics
{

//----------------------------------------------------------------------------
ContributionVoucherPrintData::ContributionVoucherPrintData( const basar::db::aspect::AccessorPropertyTableRef articleData,			//!< iterator containing transaction data
						const basar::db::aspect::AccessorPropertyTable_YIterator branchData,	//!< iterator containing branch data
						const basar::db::aspect::AccessorPropertyTable_YIterator customerData,	//!< iterator containing customer data
						const basar::db::aspect::AccessorPropertyTable_YIterator signaturelnk,	//!< iterator containing link to digital user signature
						basar::Int32 beginOfOrder,												//!< int declaring first booking of an order
						basar::Int32 endOfOrder,
						basar::Int16 branchno ) 
: PrintDataBase( articleData, branchData, customerData, signaturelnk, beginOfOrder, endOfOrder )
{
	m_Branchno = branchno;
}

//----------------------------------------------------------------------------
ContributionVoucherPrintData::~ContributionVoucherPrintData()
{
}

//----------------------------------------------------------------------------
basar::VarString ContributionVoucherPrintData::getFormName() const
{
	return "BTMAbgabemeldung";
}

//----------------------------------------------------------------------------
basar::VarString ContributionVoucherPrintData::getFormFileName() const
{
	infrastructure::contributionVoucher::ContributionVoucherHelper contributionVoucherHelper( m_Branchno );

	return contributionVoucherHelper.buildFileName( constants::CONTRIBUTIONVOUCHERBASEFILENAME );		
}
	
//----------------------------------------------------------------------------
basar::VarString ContributionVoucherPrintData::getTitleLine()
{
	return QApplication::translate("ContributionVoucherPrintData", "Contributionvoucher").toLocal8Bit().data();
}

//----------------------------------------------------------------------------
bool ContributionVoucherPrintData::printTour()
{
	return false;
}

//----------------------------------------------------------------------------
bool ContributionVoucherPrintData::printDeliveryNoteNumber()
{
	return false;
}

} //namespace narcotics