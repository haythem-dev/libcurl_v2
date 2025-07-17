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

#include <QtWidgets/QLabel>

//------------------------------------------------------------------------------
using basar::ConstString;
using basar::TimeSpan;
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
using tie::TimeSpanTransformer;

//------------------------------------------------------------------------------
SUITE(TransformTimeSpan)
{

	//--------------------------------------------------------------------------
	#define PROPTBLCOL					 "PropTblCol"

	//--------------------------------------------------------------------------
	static ConstString		GUITBLCOL     = "GuiTblCol";

	static ConstString		ACCDEF        = "TransformTimeSpanAccDefName";
	static ConstString		ACCINST       = "TransformTimeSpanAccInstName";
	static ConstString		TS_FORMAT     = "%d#%H!%M+%S%F3";
	static const TimeSpan   TS_VAL        = TimeSpan("42 21:10:50.123");
	static ConstString      TS_STR        = "42#21!10+50.123";
	static ConstString      TS_STR_DEF    = "42 21:10:50.123";

	//--------------------------------------------------------------------------
	BEGIN_ACCESSOR_DEFINITION(ACCDEF)
		PROPERTY_DESCRIPTION_LIST("string     dummy0;"
								  "int32      dummy1;"
								  "timespan " PROPTBLCOL ";"
								  "int16      dummy2;")
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

			pWidget       = new QLabel(0);
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
		QWidget                   * pWidget; 
	};

	//--------------------------------------------------------------------------
	class FxTransform : public FxTransformBase
	{
	public:

		FxTransform()
		{
			pWidget->setObjectName(PROPTBLCOL);

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
			pWidget->setObjectName(GUITBLCOL);

			matcher		  = tie::Manager::getInstance().createMatcher(pWidget, accPropTbl);

			guiPropTbl    = matcher.getLeft();
			guiPropTblYIt = guiPropTbl.begin();
		}
	};

//------------------------------------------------------------------------------
TEST_FIXTURE(FxTransform, Ctor_RL)
{
	TimeSpanTransformer tf (PROPTBLCOL, TS_FORMAT);

	accPropTblYIt.setTimeSpan(PROPTBLCOL, TS_VAL);
	tf.RightToLeft(guiPropTblYIt, accPropTblYIt);
	CHECK_EQUAL(TS_STR, guiPropTblYIt.getString(PROPTBLCOL).c_str());
}

//------------------------------------------------------------------------------
TEST_FIXTURE(FxTransform, Ctor_LR)
{
	TimeSpanTransformer tf (PROPTBLCOL, TS_FORMAT);

	guiPropTblYIt.setString(PROPTBLCOL, TS_STR);
	tf.LeftToRight(guiPropTblYIt, accPropTblYIt);
	CHECK(TS_VAL == accPropTblYIt.getTimeSpan(PROPTBLCOL));
}

//------------------------------------------------------------------------------
TEST_FIXTURE(FxTransform, Ctor_RL_withoutFormat)
{
	TimeSpanTransformer tf (PROPTBLCOL, "");

	accPropTblYIt.setTimeSpan(PROPTBLCOL, TS_VAL);
	tf.RightToLeft(guiPropTblYIt, accPropTblYIt);
	CHECK_EQUAL(TS_STR_DEF, guiPropTblYIt.getString(PROPTBLCOL).c_str());
}

//------------------------------------------------------------------------------
TEST_FIXTURE(FxTransform, Ctor_LR_withoutFormat)
{
	try
	{
		TimeSpanTransformer tf (PROPTBLCOL, "");

		guiPropTblYIt.setString(PROPTBLCOL, TS_STR_DEF);
		tf.LeftToRight(guiPropTblYIt, accPropTblYIt);
		CHECK(TS_VAL == accPropTblYIt.getTimeSpan(PROPTBLCOL));
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
		TimeSpanTransformer tf (PROPTBLCOL, GUITBLCOL, TS_FORMAT);

		accPropTblYIt.setTimeSpan(PROPTBLCOL, TS_VAL);
		tf.RightToLeft(guiPropTblYIt, accPropTblYIt);
		CHECK_EQUAL(TS_STR, guiPropTblYIt.getString(GUITBLCOL).c_str());
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
		TimeSpanTransformer tf (PROPTBLCOL, GUITBLCOL, TS_FORMAT);

		guiPropTblYIt.setString(GUITBLCOL, TS_STR);
		tf.LeftToRight(guiPropTblYIt, accPropTblYIt);
		CHECK(TS_VAL == accPropTblYIt.getTimeSpan(PROPTBLCOL));
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
		TimeSpanTransformer tf (PROPTBLCOL, GUITBLCOL, "");

		accPropTblYIt.setTimeSpan(PROPTBLCOL, TS_VAL);
		tf.RightToLeft(guiPropTblYIt, accPropTblYIt);
		CHECK_EQUAL(TS_STR_DEF, guiPropTblYIt.getString(GUITBLCOL).c_str());
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
		TimeSpanTransformer tf (PROPTBLCOL, GUITBLCOL, "");

		guiPropTblYIt.setString(GUITBLCOL, TS_STR_DEF);
		tf.LeftToRight(guiPropTblYIt, accPropTblYIt);
		CHECK(TS_VAL == accPropTblYIt.getTimeSpan(PROPTBLCOL));
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
