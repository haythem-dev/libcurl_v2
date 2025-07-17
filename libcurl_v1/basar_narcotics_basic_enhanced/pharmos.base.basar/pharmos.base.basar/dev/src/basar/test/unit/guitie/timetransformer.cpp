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
using basar::Time;
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
using tie::TimeTransformer;

//------------------------------------------------------------------------------
SUITE(TransformTime)
{
	//--------------------------------------------------------------------------
	#define PROPTBLCOL					 "PropTblCol"

	//--------------------------------------------------------------------------
	static ConstString		GUITBLCOL     = "GuiTblCol";

	static ConstString		ACCDEF        = "TransformTimeAccDefName";
	static ConstString		ACCINST       = "TransformTimeAccInstName";
	static ConstString		DT_FORMAT     = "mm!hh#zzz$ss";
	static const Time       DT_VAL        = Time(Time::NULL_DATE, 25449987);
	static ConstString      DT_STR        = "54!02#987$49";
	static ConstString      DT_STR_DEF    = "02:54:49.987";

	//--------------------------------------------------------------------------
	BEGIN_ACCESSOR_DEFINITION(ACCDEF)
		PROPERTY_DESCRIPTION_LIST("string   dummy0;"
								  "int32    dummy1;"
								  "time   " PROPTBLCOL ";"
								  "int16    dummy2;")
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
	TimeTransformer tf (PROPTBLCOL, DT_FORMAT);

	accPropTblYIt.setTime(PROPTBLCOL, DT_VAL);
	tf.RightToLeft(guiPropTblYIt, accPropTblYIt);
	CHECK_EQUAL(DT_STR, guiPropTblYIt.getString(PROPTBLCOL).c_str());
}

//------------------------------------------------------------------------------
TEST_FIXTURE(FxTransform, Ctor_LR)
{
	TimeTransformer tf (PROPTBLCOL, DT_FORMAT);

	guiPropTblYIt.setString(PROPTBLCOL, DT_STR);
	tf.LeftToRight(guiPropTblYIt, accPropTblYIt);
	CHECK(DT_VAL == accPropTblYIt.getTime(PROPTBLCOL));
}

//------------------------------------------------------------------------------
TEST_FIXTURE(FxTransform, Ctor_RL_withoutFormat)
{
	TimeTransformer tf (PROPTBLCOL, "");

	accPropTblYIt.setTime(PROPTBLCOL, DT_VAL);
	tf.RightToLeft(guiPropTblYIt, accPropTblYIt);
	CHECK_EQUAL(DT_STR_DEF, guiPropTblYIt.getString(PROPTBLCOL).c_str());
}

//------------------------------------------------------------------------------
TEST_FIXTURE(FxTransform, Ctor_LR_withoutFormat)
{
	try
	{
		TimeTransformer tf (PROPTBLCOL, "");

		guiPropTblYIt.setString(PROPTBLCOL, DT_STR_DEF);
		tf.LeftToRight(guiPropTblYIt, accPropTblYIt);
		CHECK(DT_VAL.getTime()  == accPropTblYIt.getTime(PROPTBLCOL).getTime());
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
		TimeTransformer tf (PROPTBLCOL, GUITBLCOL, DT_FORMAT);

		accPropTblYIt.setTime(PROPTBLCOL, DT_VAL);
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
		TimeTransformer tf (PROPTBLCOL, GUITBLCOL, DT_FORMAT);

		guiPropTblYIt.setString(GUITBLCOL, DT_STR);
		tf.LeftToRight(guiPropTblYIt, accPropTblYIt);
		CHECK(DT_VAL == accPropTblYIt.getTime(PROPTBLCOL));
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
		TimeTransformer tf (PROPTBLCOL, GUITBLCOL, "");

		accPropTblYIt.setTime(PROPTBLCOL, DT_VAL);
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
		TimeTransformer tf (PROPTBLCOL, GUITBLCOL, "");

		guiPropTblYIt.setString(GUITBLCOL, DT_STR_DEF);
		tf.LeftToRight(guiPropTblYIt, accPropTblYIt);
		CHECK(DT_VAL.getTime() == accPropTblYIt.getTime(PROPTBLCOL).getTime());
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
