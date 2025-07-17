#include "libbasarguitie.h"
#include "matcherstringsmorelinestwostringsmockcessor.h"

MatcherStringsMoreLinesTwoStringsMockCessor::~MatcherStringsMoreLinesTwoStringsMockCessor()
{
}

MatcherStringsMoreLinesTwoStringsMockCessor::MatcherStringsMoreLinesTwoStringsMockCessor(const basar::VarString & accDefName) : basar::mock::MockCessor(accDefName)
{
}

basar::db::aspect::ExecuteResultInfo MatcherStringsMoreLinesTwoStringsMockCessor::doMockExecute(const basar::db::aspect::AccessMethodName & /*accessMethodName*/, const bool /*clearBeforeSelect*/, const bool /*flagExcept*/, const basar::db::aspect::DatabaseHintEnum /*dbHint*/)
{
    //as the pdl is the driving definition how many values get inserted (no space in prop tab, no insert), longer strings are fine as long DONT_CHECK_LENGTH is set
    fillProptabWithString("1|COLCH21||COLCH21|0COLCH21|0COLCH21   0COLCH21|0COLCH21   0COLCH21   0COLCH21   0COLCH21|", basar::mock::DONT_CHECK_LENGTH);
    fillProptabWithString("2|COLCH22|#|\\#|\\\\ \\\\ \\\\ \\ #####|'|1COLCH22   1COLCH22   1COLCH22   1COLCH22|", basar::mock::DONT_CHECK_LENGTH);
    fillProptabWithString("3|COLCH23|2|' ' '|2COLCH23|2COLCH23   2COLCH23|2COLCH23   2COLCH23   2COLCH23   2COLCH23|", basar::mock::DONT_CHECK_LENGTH);
    fillProptabWithString("4|COLCH24|3|COLCH24|3COLCH24|3COLCH24   3COLCH24|3COLCH24   3COLCH24   3COLCH24   3COLCH24|", basar::mock::DONT_CHECK_LENGTH);
    fillProptabWithString("5|COLCH25|4|COLCH25|4COLCH25|4COLCH25   4COLCH25|4COLCH25   4COLCH25   4COLCH25   4COLCH25|", basar::mock::DONT_CHECK_LENGTH);
    fillProptabWithString("6|COLCH26|5|COLCH26|5COLCH26|5COLCH26   5COLCH26|5COLCH26   5COLCH26   5COLCH26   5COLCH26|", basar::mock::DONT_CHECK_LENGTH);

    return basar::db::aspect::ExecuteResultInfo();
}
