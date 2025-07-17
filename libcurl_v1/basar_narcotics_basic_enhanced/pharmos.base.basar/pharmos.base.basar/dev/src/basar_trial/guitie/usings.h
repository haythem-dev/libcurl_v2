#include "libbasardbsql.h"
#include "libbasarproperty.h"
#include "libbasarcmnutil.h"
#include "libbasardbaspect.h"
#include "libbasarappl.h"
#include "libbasarguitie.h"

#pragma warning (push)
#pragma warning(disable: 4127 4244 4251 4311 4312 4512 4800)
#include "Qt/QMessageBox.h"
#pragma warning (pop)

using basar::cmnutil::Singleton;
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
using basar::db::sql::ConnectionRef;
using basar::VarString;
using basar::cmnutil::ParameterList;
using basar::gui::tie::MatcherRef;
using basar::gui::tie::GuiPropertyTable_YIterator;
using basar::property::PropertyDescriptionListRef;
using basar::property::PropertyDescriptionList_Iterator ;
using basar::gui::tie::Transformer;
using basar::Int32;

void test_viewconnptr();