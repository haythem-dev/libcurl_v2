#ifndef COLDMETADATA_GUARD
#define COLDMETADATA_GUARD

//------------------------------------------------------------------------------
//includes
//------------------------------------------------------------------------------
#include "bgabranchptr.h"
#include "metadata.h"
#include "contributionvoucherentryptr.h"
#include "contributionvoucherfileptr.h"

//------------------------------------------------------------------------------
//namespace
//------------------------------------------------------------------------------
namespace narcotics
{
namespace archiving
{

//------------------------------------------------------------------------------
struct MetaDataEntry
{
	basar::VarString FilePath;
	basar::Int32 ContributionVoucherNo;
	basar::Date PrintedDate;
	basar::Date TransactionDate;
	basar::Int32 PurchaseOrderNo;
	basar::VarString Subscriber;
	basar::Int32 CustomerSupplierNo;
	basar::VarString FileType;
	basar::VarString kofaxPath;
};

//------------------------------------------------------------------------------
class ColdMetaData : public MetaData
{
public:
	ColdMetaData(basar::Int16 branchno);
	virtual ~ColdMetaData();

	virtual void	addFileEntry(	narcotics::contributionvoucherentry::ContributionVoucherEntryPtr dbEntry, 
								narcotics::contributionvoucherfile::ContributionVoucherFilePtr file,
								basar::VarString kofaxPath);
	virtual QString	toString();

protected:
	std::vector<MetaDataEntry>	m_entries;
	basar::Int16				m_branchno;
};

} //namespace archiving
} //namespace narcotics

#endif //COLDMETADATA_GUARD