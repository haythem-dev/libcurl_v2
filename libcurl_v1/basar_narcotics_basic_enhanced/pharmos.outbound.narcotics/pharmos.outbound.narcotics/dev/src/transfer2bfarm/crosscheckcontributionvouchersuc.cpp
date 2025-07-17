//------------------------------------------------------------------------------
//includes
//------------------------------------------------------------------------------
#include "commonheader.h"
#include "crosscheckcontributionvouchersuc.h"
#include "loggerpool.h"

//------------------------------------------------------------------------------
//namespaces
//------------------------------------------------------------------------------

namespace narcotics {
namespace usecase {

//------------------------------------------------------------------------------

basar::appl::EventReturnType CrossCheckContributionVouchersUC::run()
{		
	BLOG_TRACE_METHOD(LoggerPool::getLoggerTransferBfarm(), "CrossCheckContributionVouchersUC::run()");

	m_crossCheckSVC->crossCheckFoldersAndCleanUp();
	m_crossCheckSVC->shutdown();
	return basar::appl::EventReturnStruct();
}

//------------------------------------------------------------------------------

CrossCheckContributionVouchersUC::CrossCheckContributionVouchersUC(boost::shared_ptr<narcotics::services::CrossCheckSVC> crossCheckSVC)
		:m_crossCheckSVC(crossCheckSVC)
{
}

//------------------------------------------------------------------------------

CrossCheckContributionVouchersUC::~CrossCheckContributionVouchersUC(void)
{
}

//------------------------------------------------------------------------------

} //namespace usecase
} //namespace narcotics

//------------------------------------------------------------------------------
