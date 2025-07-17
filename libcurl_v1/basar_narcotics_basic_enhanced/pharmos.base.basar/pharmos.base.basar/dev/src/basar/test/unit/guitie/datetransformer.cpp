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
using basar::Date;
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
using tie::DateTransformer;

//------------------------------------------------------------------------------
SUITE(TransformDate)
{
	//--------------------------------------------------------------------------
	#define PROPTBLCOL					 "PropTblCol"

	//--------------------------------------------------------------------------
	static ConstString		GUITBLCOL     = "GuiTblCol";

	static ConstString		ACCDEF        = "TransformDateAccDefName";
	static ConstString		ACCINST       = "TransformDateAccInstName";
	static ConstString		DATE_FORMAT   = "dd-yyyy+MM";
	static const Date       DATE_VAL      = Date(20090228);
	static ConstString      DATE_STR      = "28-2009+02";
	static ConstString      DATE_STR_DEF  = "28.02.2009";

	//--------------------------------------------------------------------------
	BEGIN_ACCESSOR_DEFINITION(ACCDEF)
		PROPERTY_DESCRIPTION_LIST("string dummy0;"
								  "int32  dummy1;"
								  "date  " PROPTBLCOL ";"
								  "int16  dummy2;")
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
	DateTransformer tf (PROPTBLCOL, DATE_FORMAT);

	accPropTblYIt.setDate(PROPTBLCOL, DATE_VAL);
	tf.RightToLeft(guiPropTblYIt, accPropTblYIt);
	CHECK_EQUAL(DATE_STR, guiPropTblYIt.getString(PROPTBLCOL).c_str());
}

//------------------------------------------------------------------------------
TEST_FIXTURE(FxTransform, Ctor_LR)
{
	DateTransformer tf (PROPTBLCOL, DATE_FORMAT);

	guiPropTblYIt.setString(PROPTBLCOL, DATE_STR);
	tf.LeftToRight(guiPropTblYIt, accPropTblYIt);
	CHECK(DATE_VAL == accPropTblYIt.getDate(PROPTBLCOL));
}

//------------------------------------------------------------------------------
TEST_FIXTURE(FxTransform, Ctor_RL_withoutFormat)
{
	DateTransformer tf (PROPTBLCOL, "");

	accPropTblYIt.setDate(PROPTBLCOL, DATE_VAL);
	tf.RightToLeft(guiPropTblYIt, accPropTblYIt);
	CHECK_EQUAL(DATE_STR_DEF, guiPropTblYIt.getString(PROPTBLCOL).c_str());
}

//------------------------------------------------------------------------------
TEST_FIXTURE(FxTransform, Ctor_LR_withoutFormat)
{
	try
	{
		DateTransformer tf (PROPTBLCOL, "");

		guiPropTblYIt.setString(PROPTBLCOL, DATE_STR_DEF);
		tf.LeftToRight(guiPropTblYIt, accPropTblYIt);
		CHECK(DATE_VAL == accPropTblYIt.getDate(PROPTBLCOL));
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
		DateTransformer tf (PROPTBLCOL, GUITBLCOL, DATE_FORMAT);

		accPropTblYIt.setDate(PROPTBLCOL, DATE_VAL);
		tf.RightToLeft(guiPropTblYIt, accPropTblYIt);
		CHECK_EQUAL(DATE_STR, guiPropTblYIt.getString(GUITBLCOL).c_str());
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
		DateTransformer tf (PROPTBLCOL, GUITBLCOL, DATE_FORMAT);

		guiPropTblYIt.setString(GUITBLCOL, DATE_STR);
		tf.LeftToRight(guiPropTblYIt, accPropTblYIt);
		CHECK(DATE_VAL == accPropTblYIt.getDate(PROPTBLCOL));
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
		DateTransformer tf (PROPTBLCOL, GUITBLCOL, "");

		accPropTblYIt.setDate(PROPTBLCOL, DATE_VAL);
		tf.RightToLeft(guiPropTblYIt, accPropTblYIt);
		CHECK_EQUAL(DATE_STR_DEF, guiPropTblYIt.getString(GUITBLCOL).c_str());
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
		DateTransformer tf (PROPTBLCOL, GUITBLCOL, "");

		guiPropTblYIt.setString(GUITBLCOL, DATE_STR_DEF);
		tf.LeftToRight(guiPropTblYIt, accPropTblYIt);
		CHECK(DATE_VAL == accPropTblYIt.getDate(PROPTBLCOL));
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
