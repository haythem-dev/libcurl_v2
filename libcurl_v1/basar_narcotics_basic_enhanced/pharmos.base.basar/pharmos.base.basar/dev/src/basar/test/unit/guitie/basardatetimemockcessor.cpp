#include "libbasarguitie.h"
#include "basardatetimemockcessor.h"

BasarDateTimeMockCessor::~BasarDateTimeMockCessor()
{
}

BasarDateTimeMockCessor::BasarDateTimeMockCessor(const basar::VarString & accDefName) : basar::mock::MockCessor(accDefName)
{
}

basar::db::aspect::ExecuteResultInfo BasarDateTimeMockCessor::doMockExecute(const basar::db::aspect::AccessMethodName & /*accessMethodName*/, const bool /*clearBeforeSelect*/, const bool /*flagExcept*/, const basar::db::aspect::DatabaseHintEnum /*dbHint*/)
{
    fillProptabWithString("01.01.2000|01.01.2000 01:02:03.000|01.01.2000 01:02:03.000|01:02:03|31.08.2019 23:59:59.000|31.12.9999 23:46:57.123|");
    return basar::db::aspect::ExecuteResultInfo();
}
