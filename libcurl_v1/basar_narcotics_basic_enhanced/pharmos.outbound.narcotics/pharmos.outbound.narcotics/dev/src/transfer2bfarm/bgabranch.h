#ifndef BGABRANCH_GUARD
#define BGABRANCH_GUARD

//------------------------------------------------------------------------------
//includes
//------------------------------------------------------------------------------

//#include "definitions.h"
#include "bgafolderptr.h"

//------------------------------------------------------------------------------
//namespace
//------------------------------------------------------------------------------
namespace narcotics
{
	namespace bgafolder
	{
		class BGAFolder;		
	}
}

namespace narcotics
{
namespace bgabranch
{

//------------------------------------------------------------------------------
class BGABranch
{
public:
	const basar::cmnutil::Date getElectronicVoucherStartDate();
	basar::Int16 getBranchno();
	basar::Int32 getBGANo();

	void setElectronicVoucherStartDate(const basar::cmnutil::Date& electronicVoucherstartDate);
	void setBranchno(const basar::Int16 branchno);
	void setBGANo(const basar::Int32 bgaNo);

	BGABranch();

	virtual ~BGABranch(void);

private:
	QList<bgafolder::BGAFolderPtr> m_bgaFolders;
	
	basar::cmnutil::Date m_startDate;
	basar::Int16 m_branchno;
	basar::Int32 m_bgaNo;
};

} //namespace bgabranch
} //namespace narcotics

#endif //BGABRANCH_GUARD
