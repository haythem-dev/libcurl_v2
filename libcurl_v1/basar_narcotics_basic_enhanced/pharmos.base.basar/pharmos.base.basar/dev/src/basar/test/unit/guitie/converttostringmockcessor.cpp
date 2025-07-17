#include "libbasarguitie.h"
#include "converttostringmockcessor.h"

ConvertToStringMockCessor::~ConvertToStringMockCessor()
{
}

ConvertToStringMockCessor::ConvertToStringMockCessor(const basar::VarString & accDefName) : basar::mock::MockCessor(accDefName)
{
}

basar::db::aspect::ExecuteResultInfo ConvertToStringMockCessor::doMockExecute(const basar::db::aspect::AccessMethodName & /*accessMethodName*/, const bool /*clearBeforeSelect*/, const bool /*flagExcept*/, const basar::db::aspect::DatabaseHintEnum /*dbHint*/)
{

    fillProptabWithString("1|100001|2001|300001.34568|4001.98755|500001.61|600001.32|1|COLCH21||COLCH21|0COLCH21|0COLCH21   0COLCH21|0COLCH21   0COLCH21   0COLCH21   0COLCH21|9997-04-05|8764-05-30 20:59:56.000|8764-05-30 05:27:58.990|||1|", basar::mock::DONT_CHECK_LENGTH, "|", "yyyy-mm-dd", "%Y-%m-%d %H:%M:%S.%F3");
    fillProptabWithString("2|100002|2002|300002.34568|4002.98755|500002.61|600002.32|2|COLCH22|#|\\#|\\\\ \\\\ \\\\ \\ #####|'|1COLCH22   1COLCH22   1COLCH22   1COLCH22|9994-07-10|8763-05-29 17:59:53.000|8763-05-28 10:55:58.980|||2|", basar::mock::DONT_CHECK_LENGTH, "|", "yyyy-mm-dd", "%Y-%m-%d %H:%M:%S.%F3");
    fillProptabWithString("3|100003|2003|300003.34568|4003.98755|500003.61|600003.32|3|COLCH23|2|' ' '|2COLCH23|2COLCH23   2COLCH23|2COLCH23   2COLCH23   2COLCH23   2COLCH23|9991-10-14|8762-05-28 14:59:50.000|8762-05-26 16:23:58.970|||3|", basar::mock::DONT_CHECK_LENGTH, "|", "yyyy-mm-dd", "%Y-%m-%d %H:%M:%S.%F3");
    fillProptabWithString("4|100004|2004|300004.34568|4004.98755|500004.61|600004.32|4|COLCH24|3|COLCH24|3COLCH24|3COLCH24   3COLCH24|3COLCH24   3COLCH24   3COLCH24   3COLCH24|9989-01-17|8761-05-27 11:59:47.000|8761-05-24 21:51:58.960|||4|", basar::mock::DONT_CHECK_LENGTH, "|", "yyyy-mm-dd", "%Y-%m-%d %H:%M:%S.%F3");
    fillProptabWithString("5|100005|2005|300005.34568|4005.98755|500005.61|600005.32|5|COLCH25|4|COLCH25|4COLCH25|4COLCH25   4COLCH25|4COLCH25   4COLCH25   4COLCH25   4COLCH25|9986-04-23|8760-05-26 08:59:44.000|8760-05-23 03:19:58.950|||5|", basar::mock::DONT_CHECK_LENGTH, "|", "yyyy-mm-dd", "%Y-%m-%d %H:%M:%S.%F3");
    fillProptabWithString("6|100006|2006|300006.34568|4006.98755|500006.61|600006.32|6|COLCH26|5|COLCH26|5COLCH26|5COLCH26   5COLCH26|5COLCH26   5COLCH26   5COLCH26   5COLCH26|9983-07-28|8759-05-25 05:59:41.000|8759-05-21 08:47:58.940|||6|", basar::mock::DONT_CHECK_LENGTH, "|", "yyyy-mm-dd", "%Y-%m-%d %H:%M:%S.%F3");

    return basar::db::aspect::ExecuteResultInfo();
}
