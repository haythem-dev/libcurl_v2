#include "libbasardbsql.h"
#include "libbasarproperty.h"
#include "libbasardbaspect.h"


//#include "libbasarcmnutil_pattern.h"

#include "sqlstringbuilderimpl.h"
#include "libbasardbaspect_sqlstringbuilder.h"
#include "libbasardbaspect_accessmethod.h"
#include "libbasardbaspect_accessor.h"
#include "libbasardbaspect_accessorpropertytable.h"
#include "libbasardbaspect_manager.h"

#include "libbasarcmnutil.h"
#include "libbasardbaspect.h"
#include "libbasardbaspect_connectionref.h"
#include "libbasarproperty_propertytablesnapshotref.h"
#include "libbasardbaspect_exceptions.h"

using std::ostringstream;
using std::cout;
using std::cin;
using std::endl;

using basar::VarString;

using basar::cmnutil::Singleton;
using basar::cmnutil::ParameterList;

using basar::db::aspect::PropertyDescriptionListRef;
using basar::db::aspect::AccessorPropertyTable_XIterator;
using basar::db::aspect::PropertyTableSnapshotRef;

using basar::db::aspect::ConnectionRef;
using basar::db::aspect::AccessorInstanceRef;
using basar::db::aspect::CachingPolicyEnum;
using basar::db::aspect::SQLQueryStringBuilder;
using basar::db::aspect::SQLStringBuilder;
using basar::db::aspect::SQLStringBuilderImpl;
using basar::db::aspect::SQLStringBuilderRef;
using basar::db::aspect::SQLWriteStringBuilder;
using basar::db::aspect::AccessorDefinitionRef;
using basar::db::aspect::AccessorInstanceRef;
using basar::db::aspect::AccessorDefinitionListRef;
using basar::db::aspect::AccessorDefinitionList_Iterator;
using basar::db::aspect::AccessorPropertyTable_YIterator;
//using basar::db::aspect::AccessorInstanceListRef;
using basar::db::aspect::AccessMethodRef;
using basar::db::aspect::AccessMethodListRef;
using basar::db::aspect::Manager;


using basar::db::aspect::FULL_CACHING;
using basar::db::aspect::ON_DEMAND_CACHING;
using basar::db::aspect::SINGLE_ROW_CACHING;
using basar::db::aspect::NO_CACHING;

