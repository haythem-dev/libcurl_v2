#include "libbasarguitie.h"
#include "matcherstringonelinemockcessor.h"

MatcherStringOneLineMockCessor::~MatcherStringOneLineMockCessor()
{
}

MatcherStringOneLineMockCessor::MatcherStringOneLineMockCessor(const basar::VarString & accDefName) : basar::mock::MockCessor(accDefName)
{
}

basar::db::aspect::ExecuteResultInfo MatcherStringOneLineMockCessor::doMockExecute(const basar::db::aspect::AccessMethodName & /*accessMethodName*/, const bool /*clearBeforeSelect*/, const bool /*flagExcept*/, const basar::db::aspect::DatabaseHintEnum /*dbHint*/)
{
    fillProptabWithString("1|COLCH21||COLCH21|0COLCH21|0COLCH21   0COLCH21|0COLCH21   0COLCH21   0COLCH21   0COLCH21|", basar::mock::DONT_CHECK_LENGTH);
    return basar::db::aspect::ExecuteResultInfo();
}
