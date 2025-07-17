#include "mockcessor.h"

#include <libbasardbaspect.h>
#include <libbasarproperty.h>

#include <sstream>

namespace basar
{
    namespace mock
    {

        MockCessor::MockCessor(const basar::VarString& accessorDefinitionName) : m_AccessorName("Mockcessor")
        {
            using namespace basar::db::aspect;
            //here we're basically creating a copy of an existing accessor in regards to its property table
            //get acc definition
            AccessorDefinitionRef accDefOrig = Manager::getInstance().getAccessorDefinition(accessorDefinitionName);
            m_AccessorDefinitionName = getUniqueAccDefName("InMemoryAcc");

            AccessorDefinitionRef accDef = Manager::getInstance().createAccessorDefinition(m_AccessorDefinitionName);

            //get original property description list
            basar::property::PropertyDescriptionListRef origPdl = accDefOrig.getPropertyDescriptionList();

            basar::property::PropertyDescriptionListRef targetPdl = accDef.getPropertyDescriptionList();

            for (basar::property::PropertyDescriptionList_Iterator it = origPdl.begin(); it != origPdl.end(); ++it)
            {
                targetPdl.push_back(*it);
            }
        }

        MockCessor::~MockCessor()
        {
            using namespace basar::db::aspect;

            if (false == m_Accessor.isNull())
            {
                Manager::getInstance().releaseAccessorInstance(m_Accessor.getAccessorName());
                m_Accessor.reset();
            }

            Manager::getInstance().releaseAccessorDefinition(m_AccessorDefinitionName);
        }

        const basar::db::aspect::AccessorPropertyTableRef MockCessor::getPropertyTable() const
        {
            return doMockgetPropertyTable();
        }

        basar::db::aspect::AccessorPropertyTableRef MockCessor::getPropertyTable()
        {
            return doMockgetPropertyTable();
        }

        basar::db::aspect::ExecuteResultInfo MockCessor::execute(const basar::db::aspect::AccessMethodName& accessMethodName, bool clearBeforeSelect /*= true*/, bool flagExcept /*= true*/, basar::db::aspect::DatabaseHintEnum dbHint /*= basar::db::aspect::NO_HINT*/)
        {
            return doMockExecute(accessMethodName, clearBeforeSelect, flagExcept, dbHint);
        }

        basar::db::aspect::ExecuteResultInfo MockCessor::execute(const basar::db::aspect::AccessMethodName& accessMethodName, basar::db::aspect::AccessorPropertyTable_YIterator& yit, bool clearBeforeSelect /*= true*/, bool flagExcept /*= true*/, basar::db::aspect::DatabaseHintEnum dbHint /*= basar::db::aspect::NO_HINT*/)
        {
            return doMockExecute(accessMethodName, yit, clearBeforeSelect, flagExcept, dbHint);
        }

        basar::db::aspect::ExecuteResultInfo MockCessor::execute(const basar::db::aspect::AccessMethodName& accessMethodName, basar::db::aspect::AccessorPropertyTable_YIterator& yitBegin, basar::db::aspect::AccessorPropertyTable_YIterator& yitEnd, bool clearBeforeSelect /*= true*/, bool flagExcept /*= true*/, basar::db::aspect::DatabaseHintEnum dbHint /*= basar::db::aspect::NO_HINT*/)
        {
            return doMockExecute(accessMethodName, yitBegin, yitEnd, clearBeforeSelect, flagExcept, dbHint);
        }

        std::pair<bool, ::basar::Decimal> MockCessor::executeAggregate(const basar::db::aspect::AccessMethodName& accessMethodName)
        {
            return doMockExecuteAggregate(accessMethodName);
        }

        std::pair<bool, ::basar::Decimal> MockCessor::executeAggregate(const basar::db::aspect::AccessMethodName& accessMethodName, basar::db::aspect::AccessorPropertyTable_YIterator yit)
        {
            return doMockExecuteAggregate(accessMethodName, yit);
        }

        basar::Int64 MockCessor::getLastInsertedSerial()
        {
            return doMockGetLastInsertedSerial();
        }

        basar::Int64 MockCessor::getLastInsertedSerial(const basar::Int32 index)
        {
            return doMockGetLastInsertedSerial(index);
        }

        basar::Int32 MockCessor::getNumberOfLastInsertedSerials()
        {
            return doMockGetNumberOfLastInsertedSerials();
        }

        bool MockCessor::hasLastInsertedSerial()
        {
            return doMockHasLastInsertedSerial();
        }

        void MockCessor::clearLastInsertedSerials()
        {
            return doMockClearLastInsertedSerials();
        }

        basar::db::aspect::IAccessorSharedPtr MockCessor::createInstance(const basar::db::aspect::ConnectionRef connRef, const basar::db::aspect::CachingPolicyEnum eCachePolicy)
        {
            return doMockCreateInstance(connRef, eCachePolicy);
        }

        basar::property::PropertyDescriptionListRef MockCessor::getPropertyDescriptionList()
        {
            return doMockGetPropertyDescriptionList();
        }

        basar::db::aspect::AccessMethodRef MockCessor::createAccessMethod(const basar::db::aspect::AccessMethodName& Name)
        {
            return doMockCreateAccessMethod(Name);
        }

        const basar::db::aspect::AccessMethodRef MockCessor::getAccessMethod(const basar::db::aspect::AccessMethodName& Name) const
        {
            return doMockGetAccessMethod(Name);
        }

        const basar::db::aspect::AccessMethodListRef MockCessor::getAccessMethodList() const
        {
            return doMockGetAccessMethodList();
        }

        void MockCessor::setDefaultParameter(cmnutil::ParameterList paramList, const basar::db::aspect::AccessMethodName& rName)
        {
            doMockSetDefaultParameter(paramList, rName);
        }

        void MockCessor::setDefaultParameter(const VarString& rDefParams, const basar::db::aspect::AccessMethodName& rName)
        {
            doMockSetDefaultParameter(rDefParams, rName);
        }

        void MockCessor::setDefaultParameter(ConstString defParams, const basar::db::aspect::AccessMethodName& rName)
        {
            doMockSetDefaultParameter(defParams, rName);
        }

        cmnutil::ParameterList MockCessor::getDefaultParameter(const basar::db::aspect::AccessMethodName& rName) const
        {
            return doMockGetDefaultParameter(rName);
        }

        basar::db::aspect::CollAccessorMethodRef::iterator MockCessor::begin()
        {
            return doMockBegin();
        }

        basar::db::aspect::CollAccessorMethodRef::iterator MockCessor::end()
        {
            return doMockEnd();
        }

        basar::db::aspect::AccessMethodListSize MockCessor::size() const
        {
            return doMockSize();
        }

        bool MockCessor::empty() const
        {
            return doMockEmpty();
        }

        const basar::db::aspect::IAccessorSharedPtr MockCessor::getThis() const
        {
            return doMockGetThis();
        }

        const VarString& MockCessor::getAccessorName() const
        {
            return doMockGetAccessorName();
        }

        void MockCessor::setAccessorName(const VarString name)
        {
            doMockSetAccessorName(name);
        }

        void MockCessor::switchCreateInternalSnapshot(bool toCreate)
        {
            doMockSwitchCreateInternalSnapshot(toCreate);
        }

        const property::PropertyTableSnapshotRef MockCessor::createSnapshot() const
        {
            return doMockCreateSnapshot();
        }

        void MockCessor::resetToSnapshot(property::PropertyTableSnapshotRef snapShot)
        {
            doMockResetToSnapshot(snapShot);
        }

        void MockCessor::clearInternalSnapshot()
        {
            doMockClearInternalSnapshot();
        }

        void MockCessor::beginTransaction()
        {
            doMockBeginTransaction();
        }

        void MockCessor::commitTransaction()
        {
            doMockCommitTransaction();
        }

        void MockCessor::rollbackTransaction()
        {
            doMockRollbackTransaction();
        }

        bool MockCessor::isInTransaction() const
        {
            return doMockIsInTransaction();
        }

        const basar::db::aspect::ConnectionRef MockCessor::getConnection() const
        {
            return doMockGetConnection();
        }

        void MockCessor::setAccessorExecuteListener(const basar::db::aspect::AccessorExecuteListener& proto)
        {
            doMockSetAccessorExecuteListener(proto);
        }

        void MockCessor::removeAccessorExecuteListener()
        {
            doMockRemoveAccessorExecuteListener();
        }

        basar::db::aspect::AccessorExecuteListenerRef MockCessor::getAccessorExecuteListener() const
        {
            return doMockGetAccessorExecuteListener();
        }

        basar::db::aspect::AccessorInstanceRef MockCessor::getAccessor() const
        {
            if (true == m_Accessor.isNull())
            {
                initAccessor();
            }

            return m_Accessor;
        }

        void MockCessor::initAccessor() const
        {
            using namespace basar::db::aspect;

            basar::VarString instanceName = getUniqueAccInstName(m_AccessorDefinitionName + "_Instance");

            m_Accessor = Manager::getInstance().createAccessorInstance(instanceName, m_AccessorDefinitionName, ConnectionRef(), FULL_CACHING, true);
        }

        void MockCessor::fillProptabWithString(const basar::VarString& data, CheckLengthEnum checkLength, const basar::VarString delimiter, const basar::VarString& dateFormat, const basar::VarString& dateTimeFormat)
        {
            //poor man's symbolic constants :(
            static const bool acceptNoDelimiter = false;
            static const bool remoteEmptyString = false;

            const basar::cmnutil::CollBString stringSplit = data.tokenizeAndTrim(delimiter, basar::cmnutil::BString::FIRST_END, acceptNoDelimiter, remoteEmptyString);

            basar::db::aspect::AccessorInstanceRef acc = getAccessor();

            basar::property::PropertyDescriptionListRef pdl = acc.getPropertyDescriptionList();

            basar::db::aspect::AccessorPropertyTable_YIterator yit = acc.getPropertyTable().insert(basar::FOR_CLEAN);

            if (checkLength == CHECK_LENGTH && pdl.size() != stringSplit.size())
            {
                throw basar::cmnutil::BasarRuntimeException(basar::ExceptInfo("MockCessor::fillProptabWithString", "Property description list size does not match fields in string", __FILE__, __LINE__));
            }

            //todo: define proper default values in case of empty string

            int index = 0;
            const int splitSize = int(stringSplit.size());

            for (basar::property::PropertyDescriptionList_Iterator it = pdl.begin(); it != pdl.end() && index < splitSize; ++it)
            {
                switch (it->second.getType())
                {
                case basar::STRING:
                    yit.setString(it->first, stringSplit.at(index));
                    break;

                case basar::INTDATE:
                case basar::INTTIME:
                case basar::INT32:
                {
                    if (!stringSplit.at(index).empty())
                    {
                        yit.setInt32(it->first, stringSplit.at(index).stoi());
                    }
                }
                break;

                case basar::INT16:
                {
                    if (!stringSplit.at(index).empty())
                    {
                        yit.setInt16(it->first, static_cast<basar::Int16> (stringSplit.at(index).stoi()));
                    }
                }
                break;

                case basar::DATE:
                {
                    if (!stringSplit.at(index).empty())
                    {
                        basar::Date d;
                        d.fromString(dateFormat.c_str(), stringSplit.at(index));
                        yit.setDate(it->first, d);
                    }
                }
                break;

                case basar::FLOAT32:
                {
                    if (!stringSplit.at(index).empty())
                    {
                        yit.setFloat32(it->first, static_cast<basar::Float32> (stringSplit.at(index).stof()));
                    }
                }
                break;

                case basar::FLOAT64:
                {
                    if (!stringSplit.at(index).empty())
                    {
                        yit.setFloat64(it->first, stringSplit.at(index).stof());
                    }
                }
                break;

                case basar::DECIMAL:
                {
                    if (!stringSplit.at(index).empty())
                    {
                        basar::Decimal d;
                        d.fromNonLocString(stringSplit.at(index));
                        yit.setDecimal(it->first, d);
                    }
                }
                break;

                case basar::DATETIME:
                {
                    if (!stringSplit.at(index).empty())
                    {
                        basar::DateTime d;
                        d.fromString(dateTimeFormat.c_str(), stringSplit.at(index));
                        yit.setDateTime(it->first, d);
                    }
                }
                break;

                case basar::TIME:
                {
                    if (!stringSplit.at(index).empty())
                    {
                        basar::DateTime d;

                        const basar::VarString& timeStr = stringSplit.at(index);

                        const int timelen = int(timeStr.length());

                        if (8 == timelen)
                        {
                            d.fromStrTimeShort(timeStr);
                        }
                        else
                        {
                            d.fromStrTime(timeStr);
                        }

                        yit.setTime(it->first, d);
                    }
                }
                break;

                case basar::TIMESPAN:
                {
                    if (!stringSplit.at(index).empty())
                    {
                        basar::TimeSpan t;

                        t.fromNormalizedString(stringSplit.at(index));

                        yit.setTimeSpan(it->first, t);
                    }
                }
                break;

                default:
                    throw basar::cmnutil::BasarRuntimeException(basar::ExceptInfo("MockCessor::fillProptabWithString", "unknown basar type encountered", __FILE__, __LINE__));
                    break;
                }

                ++index;

            }

        }

        const basar::db::aspect::AccessorPropertyTableRef MockCessor::doMockgetPropertyTable() const
        {
            return getAccessor().getPropertyTable();
        }

        basar::db::aspect::AccessorPropertyTableRef MockCessor::doMockgetPropertyTable()
        {
            return getAccessor().getPropertyTable();
        }

        basar::db::aspect::ExecuteResultInfo MockCessor::doMockExecute(const basar::db::aspect::AccessMethodName&, const bool /*clearBeforeSelect*/, const bool /*flagExcept*/, const basar::db::aspect::DatabaseHintEnum /*dbHint*/)
        {
            return basar::db::aspect::ExecuteResultInfo();
        }

        basar::db::aspect::ExecuteResultInfo MockCessor::doMockExecute(const basar::db::aspect::AccessMethodName&, basar::db::aspect::AccessorPropertyTable_YIterator, const bool /*clearBeforeSelect*/, const bool /*flagExcept*/, const basar::db::aspect::DatabaseHintEnum /*dbHint*/)
        {
            return basar::db::aspect::ExecuteResultInfo();
        }

        basar::db::aspect::ExecuteResultInfo MockCessor::doMockExecute(const basar::db::aspect::AccessMethodName&, basar::db::aspect::AccessorPropertyTable_YIterator /*yitBegin*/, basar::db::aspect::AccessorPropertyTable_YIterator /*yitEnd*/, const bool /*clearBeforeSelect*/, const bool /*flagExcept*/, const basar::db::aspect::DatabaseHintEnum /*dbHint*/)
        {
            return basar::db::aspect::ExecuteResultInfo();
        }

        basar::Int64 MockCessor::doMockGetLastInsertedSerial()
        {
            return -1;
        }

        basar::Int64 MockCessor::doMockGetLastInsertedSerial(const basar::Int32 /*index*/)
        {
            return -1;
        }

        basar::Int32 MockCessor::doMockGetNumberOfLastInsertedSerials()
        {
            return -1;
        }

        bool MockCessor::doMockHasLastInsertedSerial()
        {
            return false;
        }

        void MockCessor::doMockClearLastInsertedSerials()
        {
            //nothing to do
        }

        basar::db::aspect::IAccessorSharedPtr MockCessor::doMockCreateInstance(const basar::db::aspect::ConnectionRef /*connRef*/, const basar::db::aspect::CachingPolicyEnum /*eCachePolicy*/)
        {
            return basar::db::aspect::IAccessorSharedPtr();
        }

        basar::property::PropertyDescriptionListRef MockCessor::doMockGetPropertyDescriptionList()
        {
            return getAccessor().getPropertyDescriptionList();
        }

        basar::db::aspect::AccessMethodRef MockCessor::doMockCreateAccessMethod(const basar::db::aspect::AccessMethodName& /*Name*/)
        {
            return basar::db::aspect::AccessMethodRef();
        }

        const basar::db::aspect::AccessMethodRef MockCessor::doMockGetAccessMethod(const basar::db::aspect::AccessMethodName& /*Name*/) const
        {
            return basar::db::aspect::AccessMethodRef();
        }

        const basar::db::aspect::AccessMethodListRef MockCessor::doMockGetAccessMethodList() const
        {
            return getAccessor().getAccessMethodList();
        }

        void MockCessor::doMockSetDefaultParameter(cmnutil::ParameterList /*paramList*/, const basar::db::aspect::AccessMethodName& /*rName*/)
        {
            // do nothing
        }

        void MockCessor::doMockSetDefaultParameter(const VarString& /*rDefParams*/, const basar::db::aspect::AccessMethodName& /*rName*/)
        {
            // do nothing
        }

        void MockCessor::doMockSetDefaultParameter(ConstString /*defParams*/, const basar::db::aspect::AccessMethodName& /*rName*/)
        {
            // do nothing
        }

        cmnutil::ParameterList MockCessor::doMockGetDefaultParameter(const basar::db::aspect::AccessMethodName& /*rName*/) const
        {
            return cmnutil::ParameterList();
        }

        basar::db::aspect::CollAccessorMethodRef::iterator MockCessor::doMockBegin()
        {
            return basar::db::aspect::CollAccessorMethodRef::iterator();
        }

        basar::db::aspect::CollAccessorMethodRef::iterator MockCessor::doMockEnd()
        {
            return basar::db::aspect::CollAccessorMethodRef::iterator();
        }

        basar::db::aspect::AccessMethodListSize MockCessor::doMockSize() const
        {
            return basar::db::aspect::AccessMethodListSize();
        }

        bool MockCessor::doMockEmpty() const
        {
            return false;
        }

        const basar::db::aspect::IAccessorSharedPtr MockCessor::doMockGetThis() const
        {
            return basar::db::aspect::IAccessorSharedPtr();
        }

        const VarString& MockCessor::doMockGetAccessorName() const
        {
            return m_AccessorName;
        }

        void MockCessor::doMockSetAccessorName(const VarString name)
        {
            m_AccessorName = name;
        }

        void MockCessor::doMockSwitchCreateInternalSnapshot(bool /*toCreate*/)
        {
        }

        const property::PropertyTableSnapshotRef MockCessor::doMockCreateSnapshot() const
        {
            return property::PropertyTableSnapshotRef();
        }

        void MockCessor::doMockResetToSnapshot(property::PropertyTableSnapshotRef /*snapShot*/)
        {
        }

        void MockCessor::doMockClearInternalSnapshot()
        {
        }

        void MockCessor::doMockBeginTransaction()
        {
        }

        void MockCessor::doMockCommitTransaction()
        {
        }

        void MockCessor::doMockRollbackTransaction()
        {
        }

        bool MockCessor::doMockIsInTransaction() const
        {
            return false;
        }

        const basar::db::aspect::ConnectionRef MockCessor::doMockGetConnection() const
        {
            return basar::db::aspect::ConnectionRef();
        }

        void MockCessor::doMockSetAccessorExecuteListener(const basar::db::aspect::AccessorExecuteListener& /*proto*/)
        {
            // do nothing
        }

        void MockCessor::doMockRemoveAccessorExecuteListener()
        {
        }

        basar::db::aspect::AccessorExecuteListenerRef MockCessor::doMockGetAccessorExecuteListener() const
        {
            return basar::db::aspect::AccessorExecuteListenerRef();
        }

        std::pair<bool, ::basar::Decimal> MockCessor::doMockExecuteAggregate(const basar::db::aspect::AccessMethodName& /*accessMethodName*/, basar::db::aspect::AccessorPropertyTable_YIterator /*yit*/)
        {
            return std::pair<bool, ::basar::Decimal>();
        }

        std::pair<bool, ::basar::Decimal> MockCessor::doMockExecuteAggregate(const basar::db::aspect::AccessMethodName& /*accessMethodName*/)
        {
            return std::pair<bool, ::basar::Decimal>();
        }

        basar::VarString MockCessor::getUniqueAccDefName(const basar::VarString& name) const
        {
            basar::Int32 index = 1;

            basar::VarString accName;

            do
            {
                std::ostringstream os;

                os << name << '_' << index;

                accName.assign(os.str());

                ++index;
            }
            while (true == basar::db::aspect::Manager::getInstance().hasAccessorDefinition(accName));

            return accName;
        }

        basar::VarString MockCessor::getUniqueAccInstName(const basar::VarString& name) const
        {
            basar::Int32 index = 1;

            basar::VarString accName;

            do
            {
                std::ostringstream os;

                os << name << '_' << index;

                accName.assign(os.str());

                ++index;
            }
            while (true == basar::db::aspect::Manager::getInstance().hasAccessorInstance(accName));

            return accName;
        }
    }
}