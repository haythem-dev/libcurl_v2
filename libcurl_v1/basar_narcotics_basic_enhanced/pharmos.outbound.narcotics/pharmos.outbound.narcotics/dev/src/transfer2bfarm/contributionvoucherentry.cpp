//------------------------------------------------------------------------------
//includes
//------------------------------------------------------------------------------
#include "commonheader.h"
#include "contributionvoucherentry.h"

//------------------------------------------------------------------------------
//namespace
//------------------------------------------------------------------------------
namespace narcotics
{
namespace contributionvoucherentry
{

//------------------------------------------------------------------------------
basar::Int32 ContributionVoucherEntry::getPurchaseOrderNo()
{
	return m_purchaseOrderNo;
}

//------------------------------------------------------------------------------
basar::VarString ContributionVoucherEntry::getSubscriber()
{
	return m_subscriber;
}

//------------------------------------------------------------------------------
basar::Date ContributionVoucherEntry::getPrintedDate()
{
	return m_printedDate;
}

//------------------------------------------------------------------------------
basar::Int32 ContributionVoucherEntry::getCustomerSupplierNo()
{
	return m_customersupplierno;
}
	
//------------------------------------------------------------------------------
basar::Int32 ContributionVoucherEntry::getVoucherNo()
{
	return m_voucherNo;
}

//------------------------------------------------------------------------------
basar::Date ContributionVoucherEntry::getTransactionDate()
{
	return m_transactionDate;
}

//------------------------------------------------------------------------------
ContributionVoucherEntry::ContributionVoucherEntry(basar::Int32 voucherNo, 
												   basar::Int32 purchaseOrderNo, 
												   basar::VarString subscriber,
												   basar::Date printedDate, 
												   basar::Date transactiondate,
												   basar::Int32 customersupplierno):
													m_voucherNo(voucherNo),m_purchaseOrderNo(purchaseOrderNo),
														m_subscriber(subscriber),m_printedDate(printedDate),
														m_transactionDate(transactiondate),m_customersupplierno(customersupplierno)
{	
}

//------------------------------------------------------------------------------
ContributionVoucherEntry::~ContributionVoucherEntry(void)
{
}

} //namespace contributionvoucherentry
} //namespace narcotics