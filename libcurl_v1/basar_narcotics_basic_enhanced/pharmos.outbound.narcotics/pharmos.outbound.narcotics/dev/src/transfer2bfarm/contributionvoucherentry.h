#ifndef CONTRIBUTIONVOUCHERNETRY_GUARD
#define CONTRIBUTIONVOUCHERNETRY_GUARD

//------------------------------------------------------------------------------
//namespace
//------------------------------------------------------------------------------

namespace narcotics {
namespace contributionvoucherentry {

//------------------------------------------------------------------------------
class ContributionVoucherEntry
{

public:
	basar::Int32 getVoucherNo();

	basar::Int32 getPurchaseOrderNo();

	basar::VarString getSubscriber();

	basar::Date getPrintedDate();

	basar::Date getTransactionDate();

	basar::Int32 getCustomerSupplierNo();

	ContributionVoucherEntry(basar::Int32 voucherNo, basar::Int32 purchaseOrderNo, basar::VarString subscriber,basar::Date printedDate, basar::Date transactiondate, basar::Int32 customersupplierno );

	virtual ~ContributionVoucherEntry(void);

private:
	basar::Int32 m_voucherNo;
	basar::Int32 m_purchaseOrderNo;
	basar::VarString m_subscriber;
	basar::Date m_printedDate;
	basar::Date m_transactionDate;
	basar::Int32 m_customersupplierno;

};

} //namespace contributionvoucherentry
} //namespace narcotics

#endif //CONTRIBUTIONVOUCHERNETRY_GUARD