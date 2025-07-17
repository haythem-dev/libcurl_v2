#ifndef GUARD_CONTRIBUTIONVOUCHERHELPER
#define GUARD_CONTRIBUTIONVOUCHERHELPER

//---------------------------------------------------------------------------
//namespace section
//---------------------------------------------------------------------------
namespace infrastructure
{
namespace contributionVoucher
{

//---------------------------------------------------------------------------
class ContributionVoucherHelper
{
public:
	basar::VarString buildFileName( const basar::VarString& baseFileName ) const;

	ContributionVoucherHelper( const basar::Int16& branchno );
	~ContributionVoucherHelper();

private:
	basar::Int16 m_Branchno;
};

} //namespace contributionVoucher
} //namespace infrastructure

#endif //GUARD_CONTRIBUTIONVOUCHERHELPER