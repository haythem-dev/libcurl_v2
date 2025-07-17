#include "libbasarguitie.h"
#include "matcherstringsmorelinesstringsreversemockcessor.h"

MatcherStringsMoreLinesStringsReverseMockCessor::~MatcherStringsMoreLinesStringsReverseMockCessor()
{
}

MatcherStringsMoreLinesStringsReverseMockCessor::MatcherStringsMoreLinesStringsReverseMockCessor(const basar::VarString & accDefName) : basar::mock::MockCessor(accDefName)
{
}

basar::db::aspect::ExecuteResultInfo MatcherStringsMoreLinesStringsReverseMockCessor::doMockExecute(const basar::db::aspect::AccessMethodName & /*accessMethodName*/, const bool /*clearBeforeSelect*/, const bool /*flagExcept*/, const basar::db::aspect::DatabaseHintEnum /*dbHint*/)
{
    //as the pdl is the driving definition how many values get inserted (no space in prop tab, no insert), longer strings are fine as long DONT_CHECK_LENGTH is set
    fillProptabWithString("0COLCH21   0COLCH21   0COLCH21   0COLCH21|0COLCH21   0COLCH21|0COLCH21|COLCH21||COLCH21|1|", basar::mock::DONT_CHECK_LENGTH);
    fillProptabWithString("1COLCH22   1COLCH22   1COLCH22   1COLCH22|'|\\\\ \\\\ \\\\ \\ #####|\\#|#|COLCH22|2|", basar::mock::DONT_CHECK_LENGTH);
    fillProptabWithString("2COLCH23   2COLCH23   2COLCH23   2COLCH23|2COLCH23   2COLCH23|2COLCH23|' ' '|2|COLCH23|3|", basar::mock::DONT_CHECK_LENGTH);
    fillProptabWithString("3COLCH24   3COLCH24   3COLCH24   3COLCH24|3COLCH24   3COLCH24|3COLCH24|COLCH24|3|COLCH24|4|", basar::mock::DONT_CHECK_LENGTH);
    fillProptabWithString("4COLCH25   4COLCH25   4COLCH25   4COLCH25|4COLCH25   4COLCH25|4COLCH25|COLCH25|4|COLCH25|5|", basar::mock::DONT_CHECK_LENGTH);
    fillProptabWithString("5COLCH26   5COLCH26   5COLCH26   5COLCH26|5COLCH26   5COLCH26|5COLCH26|COLCH26|5|COLCH26|6|", basar::mock::DONT_CHECK_LENGTH);

    return basar::db::aspect::ExecuteResultInfo();
}
