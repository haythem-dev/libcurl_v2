#pragma once

#include "mockcessor.h"
#include <libbasardbaspect.h>
#include <boost/shared_ptr.hpp>

class ConvertToStringMockCessor;

typedef boost::shared_ptr<ConvertToStringMockCessor> ConvertToStringMockCessorPtr;

class ConvertToStringMockCessor : public basar::mock::MockCessor
{
    public:

        virtual ~ConvertToStringMockCessor();

        ConvertToStringMockCessor(const basar::VarString& accDefName);

    protected:
        virtual basar::db::aspect::ExecuteResultInfo doMockExecute(const basar::db::aspect::AccessMethodName& accessMethodName, const bool clearBeforeSelect = true, const bool flagExcept = true, const basar::db::aspect::DatabaseHintEnum dbHint = basar::db::aspect::NO_HINT) /*override*/;
};