#ifndef CROSSCHECKUC_GUARD
#define CROSSCHECKUC_GUARD
//------------------------------------------------------------------------------
//includes
//------------------------------------------------------------------------------
#include "usecasecontroler.h"
#include "crosschecksvc.h"

//------------------------------------------------------------------------------
//namespace
//------------------------------------------------------------------------------
namespace narcotics
{
namespace usecase
{

//------------------------------------------------------------------------------
class CrossCheckContributionVouchersUC : public UseCaseController
{
public:

	CrossCheckContributionVouchersUC(boost::shared_ptr<narcotics::services::CrossCheckSVC> crossCheckSVC);
	virtual ~CrossCheckContributionVouchersUC(void);

	virtual basar::appl::EventReturnType run();

private:
	boost::shared_ptr<narcotics::services::CrossCheckSVC> m_crossCheckSVC;

};

} //namespace usecase
} //namespace narcotics

#endif //CROSSCHECKUC_GUARD