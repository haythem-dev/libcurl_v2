#pragma once

#include <libbasardbaspect_iaccessor.h>
#include <libbasardbaspect_accessor.h>

namespace basar
{
    namespace mock
    {
        enum CheckLengthEnum
        {
            CHECK_LENGTH,
            DONT_CHECK_LENGTH
        };

        class MockCessor : public basar::db::aspect::IAccessor
        {
            public:
                MockCessor(const basar::VarString& accessorDefinitionName);

                //force class to be abstract
                virtual ~MockCessor() = 0;

                // Inherited via IAccessor - final member functions(?)
                //maybe template method is overkill here... but i'd like the flexibility of doing stuff before or after the user code (doMock-member functions) runs/ran
                virtual const basar::db::aspect::AccessorPropertyTableRef getPropertyTable() const /*override*/;
                virtual basar::db::aspect::AccessorPropertyTableRef getPropertyTable() /*override*/;
                virtual basar::db::aspect::ExecuteResultInfo execute(const basar::db::aspect::AccessMethodName& accessMethodName, bool clearBeforeSelect = true, bool flagExcept = true, basar::db::aspect::DatabaseHintEnum dbHint = basar::db::aspect::NO_HINT) /*override*/;
                virtual basar::db::aspect::ExecuteResultInfo execute(const basar::db::aspect::AccessMethodName& accessMethodName, basar::db::aspect::AccessorPropertyTable_YIterator& yit, bool clearBeforeSelect = true, bool flagExcept = true, basar::db::aspect::DatabaseHintEnum dbHint = basar::db::aspect::NO_HINT) /*override*/;
                virtual basar::db::aspect::ExecuteResultInfo execute(const basar::db::aspect::AccessMethodName& accessMethodName, basar::db::aspect::AccessorPropertyTable_YIterator& yitBegin, basar::db::aspect::AccessorPropertyTable_YIterator& yitEnd, bool clearBeforeSelect = true, bool flagExcept = true, basar::db::aspect::DatabaseHintEnum dbHint = basar::db::aspect::NO_HINT) /*override*/;
                virtual std::pair<bool, ::basar::Decimal> executeAggregate(const basar::db::aspect::AccessMethodName& accessMethodName) /*override*/;
                virtual std::pair<bool, ::basar::Decimal> executeAggregate(const basar::db::aspect::AccessMethodName& accessMethodName, basar::db::aspect::AccessorPropertyTable_YIterator yit) /*override*/;
                virtual basar::Int64 getLastInsertedSerial() /*override*/;
                virtual basar::Int64 getLastInsertedSerial(basar::Int32 index) /*override*/;
                virtual basar::Int32 getNumberOfLastInsertedSerials() /*override*/;
                virtual bool hasLastInsertedSerial() /*override*/;
                virtual void clearLastInsertedSerials() /*override*/;
                virtual basar::db::aspect::IAccessorSharedPtr createInstance(const basar::db::aspect::ConnectionRef connRef, const basar::db::aspect::CachingPolicyEnum eCachePolicy = basar::db::aspect::ON_DEMAND_CACHING) /*override*/;
                virtual basar::property::PropertyDescriptionListRef getPropertyDescriptionList() /*override*/;
                virtual basar::db::aspect::AccessMethodRef createAccessMethod(const basar::db::aspect::AccessMethodName& Name) /*override*/;
                virtual const basar::db::aspect::AccessMethodRef getAccessMethod(const basar::db::aspect::AccessMethodName& Name) const /*override*/;
                virtual const basar::db::aspect::AccessMethodListRef getAccessMethodList() const /*override*/;
                virtual void setDefaultParameter(cmnutil::ParameterList paramList, const basar::db::aspect::AccessMethodName& rName) /*override*/;
                virtual void setDefaultParameter(const VarString& rDefParams, const basar::db::aspect::AccessMethodName& rName) /*override*/;
                virtual void setDefaultParameter(ConstString defParams, const basar::db::aspect::AccessMethodName& rName) /*override*/;
                virtual cmnutil::ParameterList getDefaultParameter(const basar::db::aspect::AccessMethodName& rName) const /*override*/;
                virtual basar::db::aspect::CollAccessorMethodRef::iterator begin() /*override*/;
                virtual basar::db::aspect::CollAccessorMethodRef::iterator end() /*override*/;
                virtual basar::db::aspect::AccessMethodListSize size() const /*override*/;
                virtual bool empty() const /*override*/;
                virtual const basar::db::aspect::IAccessorSharedPtr getThis() const /*override*/;
                virtual const VarString& getAccessorName() const /*override*/;
                virtual void setAccessorName(const VarString name) /*override*/;
                virtual void switchCreateInternalSnapshot(bool toCreate) /*override*/;
                virtual const property::PropertyTableSnapshotRef createSnapshot() const /*override*/;
                virtual void resetToSnapshot(property::PropertyTableSnapshotRef snapShot) /*override*/;
                virtual void clearInternalSnapshot() /*override*/;
                virtual void beginTransaction() /*override*/;
                virtual void commitTransaction() /*override*/;
                virtual void rollbackTransaction() /*override*/;
                virtual bool isInTransaction() const /*override*/;
                virtual const basar::db::aspect::ConnectionRef getConnection() const /*override*/;
                virtual void setAccessorExecuteListener(const basar::db::aspect::AccessorExecuteListener& proto) /*override*/;
                virtual void removeAccessorExecuteListener() /*override*/;
                virtual basar::db::aspect::AccessorExecuteListenerRef getAccessorExecuteListener() const /*override*/;

            protected:

                basar::db::aspect::AccessorInstanceRef          getAccessor() const;
                void                                            initAccessor() const;

                //auto fill method - use basar defines for formats
                void fillProptabWithString(const basar::VarString& data, CheckLengthEnum checkLength = CHECK_LENGTH, const basar::VarString delimiter = "|", const basar::VarString& dateFormat = "dd.mm.yyyy", const basar::VarString& dateTimeFormat = "%d.%m.%Y %H:%M:%S.%F3");

                //template methods
                virtual const basar::db::aspect::AccessorPropertyTableRef doMockgetPropertyTable() const;
                virtual basar::db::aspect::AccessorPropertyTableRef doMockgetPropertyTable();
                virtual basar::db::aspect::ExecuteResultInfo doMockExecute(const basar::db::aspect::AccessMethodName& accessMethodName, const bool clearBeforeSelect = true, const bool flagExcept = true, const basar::db::aspect::DatabaseHintEnum dbHint = basar::db::aspect::NO_HINT);
                //the execute below is the most commonly used one. if unsure which to override, override this
                virtual basar::db::aspect::ExecuteResultInfo doMockExecute(const basar::db::aspect::AccessMethodName& accessMethodName, basar::db::aspect::AccessorPropertyTable_YIterator yit, const bool clearBeforeSelect = true, const bool flagExcept = true, const basar::db::aspect::DatabaseHintEnum dbHint = basar::db::aspect::NO_HINT);
                virtual basar::db::aspect::ExecuteResultInfo doMockExecute(const basar::db::aspect::AccessMethodName& accessMethodName, basar::db::aspect::AccessorPropertyTable_YIterator yitBegin, basar::db::aspect::AccessorPropertyTable_YIterator yitEnd, const bool clearBeforeSelect = true, const bool flagExcept = true, const basar::db::aspect::DatabaseHintEnum dbHint = basar::db::aspect::NO_HINT);
                virtual std::pair<bool, ::basar::Decimal> doMockExecuteAggregate(const basar::db::aspect::AccessMethodName& accessMethodName);
                virtual std::pair<bool, ::basar::Decimal> doMockExecuteAggregate(const basar::db::aspect::AccessMethodName& accessMethodName, basar::db::aspect::AccessorPropertyTable_YIterator yit);
                virtual basar::Int64 doMockGetLastInsertedSerial();
                virtual basar::Int64 doMockGetLastInsertedSerial(basar::Int32 index);
                virtual basar::Int32 doMockGetNumberOfLastInsertedSerials();
                virtual bool doMockHasLastInsertedSerial();
                virtual void doMockClearLastInsertedSerials();
                virtual basar::db::aspect::IAccessorSharedPtr doMockCreateInstance(const basar::db::aspect::ConnectionRef connRef, const basar::db::aspect::CachingPolicyEnum eCachePolicy = basar::db::aspect::ON_DEMAND_CACHING) /*override*/;
                virtual basar::property::PropertyDescriptionListRef doMockGetPropertyDescriptionList() /*override*/;
                virtual basar::db::aspect::AccessMethodRef doMockCreateAccessMethod(const basar::db::aspect::AccessMethodName& Name) /*override*/;
                virtual const basar::db::aspect::AccessMethodRef doMockGetAccessMethod(const basar::db::aspect::AccessMethodName& Name) const /*override*/;
                virtual const basar::db::aspect::AccessMethodListRef doMockGetAccessMethodList() const /*override*/;
                virtual void doMockSetDefaultParameter(cmnutil::ParameterList paramList, const basar::db::aspect::AccessMethodName& rName) /*override*/;
                virtual void doMockSetDefaultParameter(const VarString& rDefParams, const basar::db::aspect::AccessMethodName& rName) /*override*/;
                virtual void doMockSetDefaultParameter(ConstString defParams, const basar::db::aspect::AccessMethodName& rName) /*override*/;
                virtual cmnutil::ParameterList doMockGetDefaultParameter(const basar::db::aspect::AccessMethodName& rName) const /*override*/;
                virtual basar::db::aspect::CollAccessorMethodRef::iterator doMockBegin() /*override*/;
                virtual basar::db::aspect::CollAccessorMethodRef::iterator doMockEnd() /*override*/;
                virtual basar::db::aspect::AccessMethodListSize doMockSize() const /*override*/;
                virtual bool doMockEmpty() const /*override*/;
                virtual const basar::db::aspect::IAccessorSharedPtr doMockGetThis() const /*override*/;
                virtual const VarString& doMockGetAccessorName() const /*override*/;
                virtual void doMockSetAccessorName(const VarString name) /*override*/;
                virtual void doMockSwitchCreateInternalSnapshot(bool toCreate) /*override*/;
                virtual const property::PropertyTableSnapshotRef doMockCreateSnapshot() const /*override*/;
                virtual void doMockResetToSnapshot(property::PropertyTableSnapshotRef snapShot) /*override*/;
                virtual void doMockClearInternalSnapshot() /*override*/;
                virtual void doMockBeginTransaction() /*override*/;
                virtual void doMockCommitTransaction() /*override*/;
                virtual void doMockRollbackTransaction() /*override*/;
                virtual bool doMockIsInTransaction() const /*override*/;
                virtual const basar::db::aspect::ConnectionRef doMockGetConnection() const /*override*/;
                virtual void doMockSetAccessorExecuteListener(const basar::db::aspect::AccessorExecuteListener& proto) /*override*/;
                virtual void doMockRemoveAccessorExecuteListener() /*override*/;
                virtual basar::db::aspect::AccessorExecuteListenerRef doMockGetAccessorExecuteListener() const /*override*/;
            private:
                //funcs
                MockCessor(const MockCessor&) /* = delete */;
                MockCessor& operator=(const MockCessor&) /* = delete */;

                basar::VarString getUniqueAccDefName(const basar::VarString& name) const;

                basar::VarString getUniqueAccInstName(const basar::VarString& name) const;

                //vars
                mutable basar::db::aspect::AccessorInstanceRef m_Accessor;

                basar::VarString m_AccessorDefinitionName;
                basar::VarString m_AccessorInstanceName;

                basar::VarString m_AccessorName;

        };
    }
}
