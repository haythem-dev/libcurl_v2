//---------------------------------------------------------------------------
//include section
//---------------------------------------------------------------------------
#include "commonheader.h"
#include "contributionvoucherhelper.h"

//---------------------------------------------------------------------------
//namespace section
//---------------------------------------------------------------------------
namespace infrastructure
{
namespace contributionVoucher
{
	//---------------------------------------------------------------------------
	basar::VarString ContributionVoucherHelper::buildFileName( const basar::VarString& baseFileName ) const
	{
		std::stringstream ss;

		ss << m_Branchno;

		basar::VarString branchnoTxt = ss.str().c_str();

		basar::VarString newFileName = branchnoTxt;

		newFileName.append("_").append(baseFileName);

		return newFileName;	
	}

	//---------------------------------------------------------------------------
	ContributionVoucherHelper::ContributionVoucherHelper( const basar::Int16& branchno )
	{
		m_Branchno = branchno;
	}

	//---------------------------------------------------------------------------
	ContributionVoucherHelper::~ContributionVoucherHelper()
	{
	}

} //namespace contributionVoucher
} //namespace infrastructure