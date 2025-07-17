/* 
 * @file                                            
 * @author Michael Eichenlaub
 * @date   2010-05-03
 */ 

#include <UnitTest++/UnitTest++.h>
#include "libbasarguitie.h"

#include "libbasarproperty_propertydescriptionlistref.h"
#include "libbasardbaspect_accessor.h"
#include "libbasardbaspect_connectionref.h"
#include "libbasardbaspect_manager.h"
#include "libbasardbaspect_macros.h"
#include "libbasardbaspect_accessmethod.h"
#include "libbasardbaspect_accessorpropertytable_yiterator.h"

#include "libbasarqtwidget.h"

//------------------------------------------------------------------------------
using basar::ConstString;
using basar::VarString;
using basar::DateTime;
using basar::FOR_CLEAN;

using namespace basar::db;
using aspect::ConnectionRef;
using aspect::FULL_CACHING;
using aspect::AccessorInstanceRef;
using aspect::AccessorPropertyTable_YIterator;
using aspect::AccessorPropertyTableRef;

using namespace basar::gui;

using tie::GuiPropertyTableRef;
using tie::GuiPropertyTable_YIterator;
using tie::MatcherRef;
using tie::DateTimeTransformer;

//------------------------------------------------------------------------------
SUITE(TransformDateTime)
{

	//--------------------------------------------------------------------------
	#define PROPTBLCOL					 "PropTblCol"
	#define PROPDMYCOL0				     "PropDmyCol0"
	#define PROPDMYCOL1					 "PropDmyCol1"
	#define PROPDMYCOL2					 "PropDmyCol2"

	//--------------------------------------------------------------------------
	static ConstString		GUITBLCOL     = "GuiTblCol";
	static ConstString		GUIDMYCOL0    = "GuiDmyCol0";
	static ConstString		GUIDMYCOL1    = "GuiDmyCol1";
	static ConstString		GUIDMYCOL2    = "GuiDmyCol2";

	static ConstString		ACCDEF        = "TransformDateTimeAccDefName";
	static ConstString		ACCINST       = "TransformDateTimeAccInstName";
	static ConstString		DT_FORMAT     = "hh#dd-mm!yyyy+ss$MM*zzz";
	static const DateTime   DT_VAL        = DateTime(20100329, 11259345);
	static ConstString      DT_STR        = "01#29-12!2010+59$03*345";
	static ConstString      DT_STR_DEF    = "29.03.2010 01:12:59.345";

	//--------------------------------------------------------------------------
	BEGIN_ACCESSOR_DEFINITION(ACCDEF)
		PROPERTY_DESCRIPTION_LIST("string   "PROPDMYCOL0";"
								  "int32    "PROPDMYCOL1";"
								  "datetime "PROPTBLCOL ";"
								  "int16    "PROPDMYCOL2";")
	END_ACCESSOR_DEFINITION

	//--------------------------------------------------------------------------
	class FxTransformBase
	{
	public:
		AccessorPropertyTable_YIterator accPropTblYIt;
		GuiPropertyTable_YIterator      guiPropTblYIt;

		FxTransformBase()
		{
			accInst       = aspect::Manager::getInstance().createAccessorInstance(ACCINST,
																		          ACCDEF,
																				  ConnectionRef(), 
																				  FULL_CACHING);
			accPropTbl    = accInst.getPropertyTable();
			accPropTbl.insert(FOR_CLEAN);
			accPropTblYIt = accPropTbl.begin();

			pWidget = new BasarTableWidget(1, 4);
			pWidget->setHorizontalHeaderItem(0, new QTableWidgetItem());
			pWidget->setHorizontalHeaderItem(1, new QTableWidgetItem());
			pWidget->setHorizontalHeaderItem(2, new QTableWidgetItem());
			pWidget->setHorizontalHeaderItem(3, new QTableWidgetItem());

			pWidget->setItem(0, 0, new QTableWidgetItem());
			pWidget->setItem(0, 1, new QTableWidgetItem());
			pWidget->setItem(0, 2, new QTableWidgetItem());
			pWidget->setItem(0, 3, new QTableWidgetItem());
		}

		~FxTransformBase()
		{
			delete pWidget;
		}

	protected:
		AccessorInstanceRef         accInst;
		AccessorPropertyTableRef	accPropTbl;
		GuiPropertyTableRef         guiPropTbl;
		MatcherRef                  matcher;  
		BasarTableWidget          * pWidget; 
	};

	//--------------------------------------------------------------------------
	class FxTransform : public FxTransformBase
	{
	public:

		FxTransform()
		{
			QStringList propCols;

			propCols.push_back(PROPTBLCOL);
			propCols.push_back(PROPDMYCOL0);
			propCols.push_back(PROPDMYCOL1);
			propCols.push_back(PROPDMYCOL2);
			pWidget->setHorizontalHeaderLabels(propCols);

			matcher		  = tie::Manager::getInstance().createMatcher(pWidget, accPropTbl);

			guiPropTbl    = matcher.getLeft();
			guiPropTblYIt = guiPropTbl.begin();
		}
	};

	//--------------------------------------------------------------------------
	class FxTransformGuiCol : public FxTransformBase
	{
	public:

		FxTransformGuiCol()
		{
			std::list<VarString> attribs;
			std::list<VarString> labels;
			QStringList          headers;

			attribs.push_back(PROPDMYCOL0);
			attribs.push_back(PROPDMYCOL1);
			attribs.push_back(PROPDMYCOL2);

			labels.push_back(GUIDMYCOL0);
			labels.push_back(GUIDMYCOL1);
			labels.push_back(GUIDMYCOL2);

			headers.push_back(GUITBLCOL);
			headers.push_back(GUIDMYCOL0);
			headers.push_back(GUIDMYCOL1);
			headers.push_back(GUIDMYCOL2);

			pWidget->setHorizontalHeaderLabels(headers);

			matcher = tie::Manager::getInstance().createMatcher(pWidget, accPropTbl);
			matcher.transformProperties(labels, attribs);

			guiPropTbl    = matcher.getLeft();
			guiPropTblYIt = guiPropTbl.begin();
		}
	};

//------------------------------------------------------------------------------
TEST_FIXTURE(FxTransform, Ctor_RL)
{
	DateTimeTransformer tf (PROPTBLCOL, DT_FORMAT);

	accPropTblYIt.setDateTime(PROPTBLCOL, DT_VAL);
	tf.RightToLeft(guiPropTblYIt, accPropTblYIt);
	CHECK_EQUAL(DT_STR, guiPropTblYIt.getString(PROPTBLCOL).c_str());
}

//------------------------------------------------------------------------------
TEST_FIXTURE(FxTransform, Ctor_LR)
{
	DateTimeTransformer tf (PROPTBLCOL, DT_FORMAT);

	guiPropTblYIt.setString(PROPTBLCOL, DT_STR);
	tf.LeftToRight(guiPropTblYIt, accPropTblYIt);
	CHECK(DT_VAL == accPropTblYIt.getDateTime(PROPTBLCOL));
}

//------------------------------------------------------------------------------
TEST_FIXTURE(FxTransform, Ctor_RL_withoutFormat)
{
	DateTimeTransformer tf (PROPTBLCOL, "");

	accPropTblYIt.setDateTime(PROPTBLCOL, DT_VAL);
	tf.RightToLeft(guiPropTblYIt, accPropTblYIt);
	CHECK_EQUAL(DT_STR_DEF, guiPropTblYIt.getString(PROPTBLCOL).c_str());
}

//------------------------------------------------------------------------------
TEST_FIXTURE(FxTransform, Ctor_LR_withoutFormat)
{
	try
	{
		DateTimeTransformer tf (PROPTBLCOL, "");

		guiPropTblYIt.setString(PROPTBLCOL, DT_STR_DEF);
		tf.LeftToRight(guiPropTblYIt, accPropTblYIt);
		CHECK(DT_VAL == accPropTblYIt.getDateTime(PROPTBLCOL));
	}
	catch (basar::Exception & ex)
    {
        std::cout << ex.what() << std::endl;
        const bool ExceptionOccured = false;
        CHECK(ExceptionOccured);
    }
    catch (...)
    {
        const bool ExceptionOccured = false;
        CHECK(ExceptionOccured);
        throw;
    }
}

//------------------------------------------------------------------------------
TEST_FIXTURE(FxTransformGuiCol, Ctor_GuiCol_RL)
{
	try
	{
		DateTimeTransformer tf (PROPTBLCOL, GUITBLCOL, DT_FORMAT);

		accPropTblYIt.setDateTime(PROPTBLCOL, DT_VAL);
		tf.RightToLeft(guiPropTblYIt, accPropTblYIt);
		CHECK_EQUAL(DT_STR, guiPropTblYIt.getString(GUITBLCOL).c_str());
	}
	catch (basar::Exception & ex)
    {
        std::cout << ex.what() << std::endl;
        const bool ExceptionOccured = false;
        CHECK(ExceptionOccured);
    }
    catch (...)
    {
        const bool ExceptionOccured = false;
        CHECK(ExceptionOccured);
        throw;
    }
}

//------------------------------------------------------------------------------
TEST_FIXTURE(FxTransformGuiCol, Ctor_GuiCol_LR)
{
	try
	{
		DateTimeTransformer tf (PROPTBLCOL, GUITBLCOL, DT_FORMAT);

		guiPropTblYIt.setString(GUITBLCOL, DT_STR);
		tf.LeftToRight(guiPropTblYIt, accPropTblYIt);
		CHECK(DT_VAL == accPropTblYIt.getDateTime(PROPTBLCOL));
	}
	catch (basar::Exception & ex)
    {
        std::cout << ex.what() << std::endl;
        const bool ExceptionOccured = false;
        CHECK(ExceptionOccured);
    }
    catch (...)
    {
        const bool ExceptionOccured = false;
        CHECK(ExceptionOccured);
        throw;
    }
}

//------------------------------------------------------------------------------
TEST_FIXTURE(FxTransformGuiCol, Ctor_GuiCol_RL_Matcher)
{
	try
	{
		DateTimeTransformer tf (PROPTBLCOL, GUITBLCOL, DT_FORMAT);
		matcher.push_back(tf);

		accPropTblYIt.setDateTime(PROPTBLCOL, DT_VAL);
		matcher.RightToLeft();

		CHECK_EQUAL(DT_STR, matcher.getLeft().begin().getString(GUITBLCOL).c_str());
	}
	catch (basar::Exception & ex)
    {
        std::cout << ex.what() << std::endl;
        const bool ExceptionOccured = false;
        CHECK(ExceptionOccured);
    }
    catch (...)
    {
        const bool ExceptionOccured = false;
        CHECK(ExceptionOccured);
        throw;
    }
}

//------------------------------------------------------------------------------
TEST_FIXTURE(FxTransformGuiCol, Ctor_GuiCol_LR_Matcher)
{
	try
	{
		DateTimeTransformer tf (PROPTBLCOL, GUITBLCOL, DT_FORMAT);
		matcher.push_back(tf);

		QTableWidgetItem * pItem = pWidget->item(0, 0);
		CHECK(pItem != NULL);
		pItem->setText( DT_STR);
		matcher.LeftToRight();

		CHECK(DT_VAL == accPropTblYIt.getDateTime(PROPTBLCOL));
	}
	catch (basar::Exception & ex)
    {
        std::cout << ex.what() << std::endl;
        const bool ExceptionOccured = false;
        CHECK(ExceptionOccured);
    }
    catch (...)
    {
        const bool ExceptionOccured = false;
        CHECK(ExceptionOccured);
        throw;
    }
}

//------------------------------------------------------------------------------
TEST_FIXTURE(FxTransformGuiCol, Ctor_GuiCol_RL_withoutFormat)
{
	try
	{
		DateTimeTransformer tf (PROPTBLCOL, GUITBLCOL, "");

		accPropTblYIt.setDateTime(PROPTBLCOL, DT_VAL);
		tf.RightToLeft(guiPropTblYIt, accPropTblYIt);
		CHECK_EQUAL(DT_STR_DEF, guiPropTblYIt.getString(GUITBLCOL).c_str());
	}
	catch (basar::Exception & ex)
    {
        std::cout << ex.what() << std::endl;
        const bool ExceptionOccured = false;
        CHECK(ExceptionOccured);
    }
    catch (...)
    {
        const bool ExceptionOccured = false;
        CHECK(ExceptionOccured);
        throw;
    }
}

//------------------------------------------------------------------------------
TEST_FIXTURE(FxTransformGuiCol, Ctor_GuiCol_LR_withoutFormat)
{
	try
	{
		DateTimeTransformer tf (PROPTBLCOL, GUITBLCOL, "");

		guiPropTblYIt.setString(GUITBLCOL, DT_STR_DEF);
		tf.LeftToRight(guiPropTblYIt, accPropTblYIt);
		CHECK(DT_VAL == accPropTblYIt.getDateTime(PROPTBLCOL));
	}
	catch (basar::Exception & ex)
    {
        std::cout << ex.what() << std::endl;
        const bool ExceptionOccured = false;
        CHECK(ExceptionOccured);
    }
    catch (...)
    {
        const bool ExceptionOccured = false;
        CHECK(ExceptionOccured);
        throw;
    }
}

//------------------------------------------------------------------------------
}	// SUITE
