#include "libbasarguitie.h"
#include "matcherstringonelinestringsreversemockcessor.h"

MatcherStringOneLineStringsReverseMockCessor::~MatcherStringOneLineStringsReverseMockCessor()
{
}

MatcherStringOneLineStringsReverseMockCessor::MatcherStringOneLineStringsReverseMockCessor(const basar::VarString & accDefName) : basar::mock::MockCessor(accDefName)
{
}

basar::db::aspect::ExecuteResultInfo MatcherStringOneLineStringsReverseMockCessor::doMockExecute(const basar::db::aspect::AccessMethodName & /*accessMethodName*/, const bool /*clearBeforeSelect*/, const bool /*flagExcept*/, const basar::db::aspect::DatabaseHintEnum /*dbHint*/)
{
    //as the pdl is the driving definition how many values get inserted (no space in prop tab, no insert), longer strings are fine as long DONT_CHECK_LENGTH is set
    fillProptabWithString("0COLCH21   0COLCH21   0COLCH21   0COLCH21|0COLCH21   0COLCH21|0COLCH21|COLCH21||COLCH21|1|", basar::mock::DONT_CHECK_LENGTH);

    return basar::db::aspect::ExecuteResultInfo();
}
