#pragma once

#include "mockcessor.h"
#include <libbasardbaspect.h>
#include <boost/shared_ptr.hpp>

class MatcherStringsMoreLinesTwoStringsReverseMockCessor;

typedef boost::shared_ptr<MatcherStringsMoreLinesTwoStringsReverseMockCessor> MatcherStringsMoreLinesTwoStringsReverseMockCessorPtr;


class MatcherStringsMoreLinesTwoStringsReverseMockCessor : public basar::mock::MockCessor
{
public:
    virtual ~MatcherStringsMoreLinesTwoStringsReverseMockCessor();

    MatcherStringsMoreLinesTwoStringsReverseMockCessor(const basar::VarString& accDefName);

protected:
    virtual basar::db::aspect::ExecuteResultInfo doMockExecute(const basar::db::aspect::AccessMethodName& accessMethodName, const bool clearBeforeSelect = true, const bool flagExcept = true, const basar::db::aspect::DatabaseHintEnum dbHint = basar::db::aspect::NO_HINT) /*override*/;
};