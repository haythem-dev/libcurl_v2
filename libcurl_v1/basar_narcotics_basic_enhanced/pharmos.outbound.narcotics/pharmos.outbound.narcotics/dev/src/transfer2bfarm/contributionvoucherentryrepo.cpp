//------------------------------------------------------------------------------
//includes
//------------------------------------------------------------------------------
#include "commonheader.h"
#include "contributionvoucherentryrepo.h"
#include "definitions.h"
#include "bgabranch.h"
#include "convert.h"
#include "exvoucherentryrepo.h"
#include "contributionvoucherentry.h"
#include "contributionvoucherentryptr.h"
#include "loggerpool.h"

//------------------------------------------------------------------------------
//namespace
//------------------------------------------------------------------------------
namespace narcotics
{
namespace contributionvoucherentry
{

//------------------------------------------------------------------------------
void ContributionVoucherEntryRepo::shutdown()
{
	BLOG_TRACE( LoggerPool::getLoggerTransferBfarm(), "ContributionVoucherEntryRepo::shutdown()" );  
	shutdownAccessors();
}

//------------------------------------------------------------------------------
void ContributionVoucherEntryRepo::shutdownAccessors()
{
	BLOG_TRACE( LoggerPool::getLoggerTransferBfarm(), "ContributionVoucherEntryRepo::shutdownAccessors()" );  

	m_searchAcc.reset();
	m_entriesDB1.reset();
}

//------------------------------------------------------------------------------
void ContributionVoucherEntryRepo::createAccessors()
{
	BLOG_TRACE( LoggerPool::getLoggerTransferBfarm(), "ContributionVoucherEntryRepo::createAccessors()" );  

	m_searchAcc = basar::db::aspect::Manager::getInstance().createAccessorInstance(
		"AccVoucherSearch", "AccVoucherEntries", m_con1.getCurrentConnection()); 

	m_entriesDB1 = basar::db::aspect::Manager::getInstance().createAccessorInstance(
		"AccVoucherEntries1", "AccVoucherEntries", m_con1.getCurrentConnection(),
		basar::db::aspect::SINGLE_ROW_CACHING);
}

//------------------------------------------------------------------------------
std::map<basar::Int32, ContributionVoucherEntryPtr>* ContributionVoucherEntryRepo::getContributionVoucherEntriesForBranchAndDate(bgabranch::BGABranchPtr branch, datecalc::DateInfo dates)
{
	BLOG_TRACE( LoggerPool::getLoggerTransferBfarm(), "ContributionVoucherEntryRepo::getContributionVoucherEntriesForBranchAndDate(bgabranch::BGABranchPtr branch, datecalc::DateInfo dates)" );

	using namespace basar::db::aspect;
	using narcotics::contributionvoucherentry::ContributionVoucherEntry;

	std::map<basar::Int32, ContributionVoucherEntryPtr> *foundEntries = new std::map<basar::Int32, ContributionVoucherEntryPtr>();

	AccessorPropertyTableRef searchPropTab = m_searchAcc.getPropertyTable();

	searchPropTab.clear();
	
	searchPropTab.insert(basar::FOR_UNKNOWN);
	
	AccessorPropertyTable_YIterator searchYit = searchPropTab.begin();

	searchYit.setInt32(narcotics::constants::PROCESSSTARTDATE, dates.getStartDate());

	searchYit.setInt32(narcotics::constants::PROCESSENDDATE, dates.getEndDate());

	searchYit.setInt16(narcotics::constants::BRANCHNO, branch->getBranchno());

	m_entriesDB1.execute("amSelectContributionVoucherEntries", searchYit);

	AccessorPropertyTableRef resultPropTab = m_entriesDB1.getPropertyTable();

	if(!resultPropTab.isNull() && !resultPropTab.empty())
	{
		AccessorPropertyTable_YIterator yit = resultPropTab.begin();

		while(!yit.isEnd())
		{			
			basar::Int32 voucherNo = yit.getInt32(narcotics::constants::VOUCHERNO);
			basar::Int32 purchaseOrderNo = yit.getInt32(narcotics::constants::PURCHASEORDERNO);
			basar::VarString subscriber = yit.getString(narcotics::constants::SUBSCRIBER);
			basar::Int32 printedDate = yit.getInt32(narcotics::constants::PRINTEDDATE);
			basar::Int32 transactionDate = yit.getInt32(narcotics::constants::TRANSACTIONDATE);
			basar::Int32 customersupplierno = yit.getInt32(narcotics::constants::CUSTOMERSUPPLIERNO);

			basar::Date convertedPrintedDate(printedDate);

			if(printedDate == 0)
			{
				convertedPrintedDate.setInvalid();
			}

			basar::Date date(transactionDate);

			if(!date.isValid())
			{
				basar::VarString msg = "invalid date: ";
				msg.append(utils::Convert::toVarString(transactionDate).c_str());				

				throw narcotics::exceptions::ExVoucherEntryRepo(basar::ExceptInfo("BGAFolder::BGAFolder",msg,__FILE__,__LINE__)); 
			}

			contributionvoucherentry::ContributionVoucherEntryPtr voucherEntry(new ContributionVoucherEntry(voucherNo,purchaseOrderNo,subscriber,convertedPrintedDate,transactionDate, customersupplierno));

			(*foundEntries)[voucherEntry->getVoucherNo()]= voucherEntry;

			yit++;
		}
	}

	return foundEntries;
}
//------------------------------------------------------------------------------
std::map<basar::Int32, ContributionVoucherEntryPtr>* ContributionVoucherEntryRepo::getDeliveryNoteCopyEntriesForBranchAndDate(bgabranch::BGABranchPtr branch, datecalc::DateInfo dates)
{
	BLOG_TRACE( LoggerPool::getLoggerTransferBfarm(), "ContributionVoucherEntryRepo::getDeliveryNoteCopyEntriesForBranchAndDate(bgabranch::BGABranchPtr branch, datecalc::DateInfo dates)" );

	using namespace basar::db::aspect;
	using narcotics::contributionvoucherentry::ContributionVoucherEntry;

	std::map<basar::Int32, ContributionVoucherEntryPtr> *foundEntries = new std::map<basar::Int32, ContributionVoucherEntryPtr>();

	AccessorPropertyTableRef searchPropTab = m_searchAcc.getPropertyTable();

	searchPropTab.clear();
	
	searchPropTab.insert(basar::FOR_UNKNOWN);
	
	AccessorPropertyTable_YIterator searchYit = searchPropTab.begin();

	searchYit.setInt32(narcotics::constants::PROCESSSTARTDATE, dates.getStartDate());

	searchYit.setInt32(narcotics::constants::PROCESSENDDATE, dates.getEndDate());

	searchYit.setInt16(narcotics::constants::BRANCHNO, branch->getBranchno());

	m_entriesDB1.execute("amSelectDeliveryNoteCopyEntries", searchYit);

	AccessorPropertyTableRef resultPropTab = m_entriesDB1.getPropertyTable();

	if(!resultPropTab.isNull() && !resultPropTab.empty())
	{
		AccessorPropertyTable_YIterator yit = resultPropTab.begin();

		while(!yit.isEnd())
		{			
			basar::Int32 voucherNo = yit.getInt32(narcotics::constants::VOUCHERNO);
			basar::Int32 purchaseOrderNo = yit.getInt32(narcotics::constants::PURCHASEORDERNO);
			basar::VarString subscriber = yit.getString(narcotics::constants::SUBSCRIBER);
			basar::Int32 printedDate = yit.getInt32(narcotics::constants::PRINTEDDATE);
			basar::Int32 transactionDate = yit.getInt32(narcotics::constants::TRANSACTIONDATE);
			basar::Int32 customersupplierno = yit.getInt32(narcotics::constants::CUSTOMERSUPPLIERNO);

			basar::Date convertedPrintedDate(printedDate);

			if(printedDate == 0)
			{
				convertedPrintedDate.setInvalid();
			}

			basar::Date date(transactionDate);

			if(!date.isValid())
			{
				basar::VarString msg = "invalid date: ";
				msg.append(utils::Convert::toVarString(transactionDate).c_str());				

				throw narcotics::exceptions::ExVoucherEntryRepo(basar::ExceptInfo("BGAFolder::BGAFolder",msg,__FILE__,__LINE__)); 
			}

			contributionvoucherentry::ContributionVoucherEntryPtr voucherEntry(new ContributionVoucherEntry(voucherNo,purchaseOrderNo,subscriber,convertedPrintedDate,transactionDate, customersupplierno));

			(*foundEntries)[voucherEntry->getVoucherNo()]= voucherEntry;

			yit++;
		}
	}

	return foundEntries;
}

//------------------------------------------------------------------------------
ContributionVoucherEntryRepo::ContributionVoucherEntryRepo(narcotics::connection::InfxConnection con1) :
	m_con1(con1)
{
	createAccessors();
}

//------------------------------------------------------------------------------
ContributionVoucherEntryRepo::~ContributionVoucherEntryRepo()
{
}

//------------------------------------------------------------------------------

} //namespace contributionvoucherentry
} //namespace narcotics