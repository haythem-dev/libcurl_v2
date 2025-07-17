//------------------------------------------------------------------------------
//includes
//------------------------------------------------------------------------------
#include "commonheader.h"
#include "bfarmservice.h"

//------------------------------------------------------------------------------
//namespaces
//------------------------------------------------------------------------------
namespace narcotics
{
namespace services
{

//------------------------------------------------------------------------------
void BfarmService::shutdown()
{
	m_bgaBranchRepo->shutdown();
	m_contributionvoucherentryrepos->shutdown();
	m_bgaFolderRepo->shutdown();
}

//------------------------------------------------------------------------------
BfarmService::BfarmService(	boost::shared_ptr<narcotics::bgabranch::BGABranchRepo> bgaBranchRepo,
							boost::shared_ptr<narcotics::contributionvoucherentry::ContributionVoucherEntryRepo> voucherRepo,
							boost::shared_ptr<narcotics::bgafolder::BGAFolderRepo> bgaFolderRepo,
							boost::shared_ptr<narcotics::datecalc::StartEndDateCalculator> dateCalc,
							narcotics::services::ServiceConfiguration config) : m_bgaBranchRepo(bgaBranchRepo),
																				m_contributionvoucherentryrepos(voucherRepo),		
																				m_bgaFolderRepo(bgaFolderRepo),
																				m_dateCalc(dateCalc),
																				m_config(config)
																					
{
}

//------------------------------------------------------------------------------
BfarmService::~BfarmService(void)
{
}

} //namespace services
} //namespace narcotics