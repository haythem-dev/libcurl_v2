#pragma once

#include "mockcessor.h"
#include <libbasardbaspect.h>
#include <boost/shared_ptr.hpp>

class MatcherStringsMoreLinesTwoStringsMockCessor;

typedef boost::shared_ptr<MatcherStringsMoreLinesTwoStringsMockCessor> MatcherStringsMoreLinesTwoStringsMockCessorPtr;


class MatcherStringsMoreLinesTwoStringsMockCessor : public basar::mock::MockCessor
{
public:
    virtual ~MatcherStringsMoreLinesTwoStringsMockCessor();

    MatcherStringsMoreLinesTwoStringsMockCessor(const basar::VarString& accDefName);

protected:
    virtual basar::db::aspect::ExecuteResultInfo doMockExecute(const basar::db::aspect::AccessMethodName& accessMethodName, const bool clearBeforeSelect = true, const bool flagExcept = true, const basar::db::aspect::DatabaseHintEnum dbHint = basar::db::aspect::NO_HINT) /*override*/;
};