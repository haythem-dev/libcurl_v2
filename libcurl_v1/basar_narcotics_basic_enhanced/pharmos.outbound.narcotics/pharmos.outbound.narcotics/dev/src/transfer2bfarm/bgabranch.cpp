//------------------------------------------------------------------------------
//includes
//------------------------------------------------------------------------------
#include "commonheader.h"
#include "definitions.h"
#include "bgabranch.h"
#include "bgafolder.h"
#include "loggerpool.h"

//------------------------------------------------------------------------------
//namespace
//------------------------------------------------------------------------------
namespace narcotics
{
namespace bgabranch
{

//------------------------------------------------------------------------------
const basar::cmnutil::Date BGABranch::getElectronicVoucherStartDate()
{
	BLOG_TRACE( LoggerPool::getLoggerTransferBfarm(), "BGABranch::getElectronicVoucherStartDate()" );
	return m_startDate;
}

//------------------------------------------------------------------------------
basar::Int16 BGABranch::getBranchno()
{
	BLOG_TRACE( LoggerPool::getLoggerTransferBfarm(), " BGABranch::getBranchno()" );
	return m_branchno;
}

//------------------------------------------------------------------------------
basar::Int32 BGABranch::getBGANo()
{
	BLOG_TRACE( LoggerPool::getLoggerTransferBfarm(), " BGABranch::getBGANo()" );
	return m_bgaNo;
}

//------------------------------------------------------------------------------
void BGABranch::setElectronicVoucherStartDate(const basar::cmnutil::Date& electronicVoucherstartDate)
{
	m_startDate = electronicVoucherstartDate;
}

//------------------------------------------------------------------------------
void BGABranch::setBranchno(const basar::Int16 branchno)
{
	m_branchno=branchno;
}

void BGABranch::setBGANo(const basar::Int32 bgaNo)
{
	m_bgaNo=bgaNo;
}

//------------------------------------------------------------------------------
//usually check values in constructor and throw exception if invalid->no parameterlist
BGABranch::BGABranch()
{   
}

//------------------------------------------------------------------------------
BGABranch::~BGABranch(void)
{
}

} //namespace usecase
} //namespace narcotics
