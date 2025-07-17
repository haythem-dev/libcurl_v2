//------------------------------------------------------------------------------
//includes
//------------------------------------------------------------------------------
#include "commonheader.h"
#include "definitions.h"
#include "bgabranchrepo.h"
#include "bgabranch.h"
#include "bgafolder.h"
#include "infxconnection.h"
#include "exbgabranchrepo.h"
#include "convert.h"
#include "loggerpool.h"

//------------------------------------------------------------------------------
//namespace
//------------------------------------------------------------------------------
namespace narcotics
{
namespace bgabranch
{

//------------------------------------------------------------------------------
void BGABranchRepo::shutdownAccessors()
{
	BLOG_TRACE_METHOD(LoggerPool::getLoggerTransferBfarm(), "BGABranchRepo::shutdownAccessors()");

	m_searchAcc.reset();
	m_bgaBranchAcc1.reset();
}

//------------------------------------------------------------------------------
void BGABranchRepo::shutdown()
{
	shutdownAccessors();
}

//------------------------------------------------------------------------------
BGABranchPtr BGABranchRepo::getPartialBranch(bgafolder::BGAFolderPtr folder)
{
	BLOG_TRACE_METHOD(LoggerPool::getLoggerTransferBfarm(), "BGABranchRepo::getPartialBranch(boost::shared_ptr<narcotics::bgafolder::BGAFolder> folder)");

	using namespace basar::db::aspect;
	
	m_searchAcc.getPropertyTable().clear();

	AccessorPropertyTable_YIterator yit = m_searchAcc.getPropertyTable().insert(basar::FOR_UNKNOWN);

	yit.setInt32(narcotics::constants::BGANO,folder->getBGANo());

	//throws exception in case of error
	m_bgaBranchAcc1.execute("amSelectBranchData",yit);

	AccessorPropertyTable_YIterator yitBranch;

	BGABranchPtr branchPtr(new BGABranch());
	
	yitBranch = m_bgaBranchAcc1.getPropertyTable().begin();

	if(!yitBranch.isContainedAndSet(narcotics::constants::BRANCHNO))
	{
		throw narcotics::exceptions::ExBGABranchRepo( basar::ExceptInfo("getBGABranchForBGAFolder(boost::shared_ptr<narcotics::bgafolder::BGAFolder> folder)",
				"no branchspecific data found!!", __FILE__, __LINE__) ); 	
	}

	basar::Int16 branchno = yitBranch.getInt16(narcotics::constants::BRANCHNO);
	branchPtr->setBranchno(branchno);
	return branchPtr;
}

//------------------------------------------------------------------------------
BGABranchPtr BGABranchRepo::setStartDate(BGABranchPtr branch)
{
	BLOG_TRACE_METHOD(LoggerPool::getLoggerTransferBfarm(), "BGABranchRepo::setStartDate(BGABranchPtr branch)");
	
	basar::VarString branchno = utils::Convert::toVarString(branch->getBranchno());
	basar::I18nString strDate;

	if (basar::login::Manager::getInstance().getParaValue(branchno, constants::UADM_STARTDATE, strDate) == false)
	{
		basar::login::Manager::getInstance().getParaValue(constants::UADM_STARTDATE, strDate); // check for global start date
	}
	if (strDate.empty())
	{
		throw narcotics::exceptions::ExBGABranchRepo( basar::ExceptInfo("BGABranchRepo::setStartDate()",
				"no start date found", __FILE__, __LINE__) ); 	
	}
	basar::Date startDate(utils::Convert::toBasarInt32(strDate));
	branch->setElectronicVoucherStartDate(startDate);
	return branch;
}

//------------------------------------------------------------------------------
BGABranchPtr BGABranchRepo::setBGANo(BGABranchPtr branch, basar::Int32 bgaNo)
{	
	branch->setBGANo(bgaNo);
	return branch;
}

//------------------------------------------------------------------------------
bgafolder::BGAFolderPtr BGABranchRepo::setBGABranchForBGAFolder(bgafolder::BGAFolderPtr folder)
{
	BLOG_TRACE_METHOD(LoggerPool::getLoggerTransferBfarm(), "BGABranchRepo::setBGABranchForBGAFolder(boost::shared_ptr<narcotics::bgafolder::BGAFolder> folder)");

	BGABranchPtr branchPtr = getPartialBranch(folder);
    
	std::map<basar::Int16,BGABranchPtr>::iterator it;
	
    it=m_foundBranches.find(branchPtr->getBranchno()); 

	//branch already in map
	if(it != m_foundBranches.end())
	{
		folder->setBGABranch(it->second);
	}
	else
	{
		setStartDate(branchPtr);
		setBGANo(branchPtr,folder->getBGANo());
	}

	folder->setBGABranch(branchPtr);

	return folder;
}

//------------------------------------------------------------------------------
void BGABranchRepo::createAccessors()
{
	BLOG_TRACE_METHOD(LoggerPool::getLoggerTransferBfarm(), "BGABranchRepo::createAccessors()");

	m_bgaBranchAcc1 = basar::db::aspect::Manager::getInstance().createAccessorInstance(
		"AccBGABranchDB1", "AccBGABranch", m_connection1.getCurrentConnection());

	m_searchAcc = basar::db::aspect::Manager::getInstance().createAccessorInstance(
		"AccBGABranchSearch", "AccBGABranch", m_connection1.getCurrentConnection());
}

//------------------------------------------------------------------------------
BGABranchRepo::BGABranchRepo(narcotics::connection::InfxConnection con1) :
	m_connection1(con1)
{
	createAccessors();
}

//------------------------------------------------------------------------------
BGABranchRepo::~BGABranchRepo()
{
}

} //namespace bgabranch
} //namespace narcotics
