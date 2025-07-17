#pragma once

#include <mockcessor.h>

#include <boost/shared_ptr.hpp>

class BasarDateTimeMockCessor;

typedef boost::shared_ptr<BasarDateTimeMockCessor> BasarDateTimeMockCessorPtr;

class BasarDateTimeMockCessor : public basar::mock::MockCessor
{
    public:
        virtual ~BasarDateTimeMockCessor();

        BasarDateTimeMockCessor(const basar::VarString& accDefName);

    protected:
        virtual basar::db::aspect::ExecuteResultInfo doMockExecute(const basar::db::aspect::AccessMethodName& accessMethodName, const bool clearBeforeSelect = true, const bool flagExcept = true, const basar::db::aspect::DatabaseHintEnum dbHint = basar::db::aspect::NO_HINT) /*override*/;
};