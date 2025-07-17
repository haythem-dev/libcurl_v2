/* 
 * @file                                            
 * @author Michael Eichenlaub
 * @date   2010-04-20
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
using basar::VarString;
using basar::I18nString;
using basar::Int32;
using basar::Int64;
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
using tie::IntTransformer;

//------------------------------------------------------------------------------
SUITE(TransformInteger)
{
	//--------------------------------------------------------------------------
	#define DEF_PROPTBLCOL0			 "PropTblCol0"
	#define DEF_PROPTBLCOL1			 "PropTblCol1"
    #define DEF_PROPTBLCOL2          "PropTblCol2"

	//--------------------------------------------------------------------------
	static ConstString PROPTBLCOL0   = DEF_PROPTBLCOL0;
	static ConstString PROPTBLCOL1   = DEF_PROPTBLCOL1;
    static ConstString PROPTBLCOL2   = DEF_PROPTBLCOL2;

	static ConstString GUITBLCOL0     = "GuiTblCol0";
	static ConstString GUITBLCOL1     = "GuiTblCol1";
    static ConstString GUITBLCOL2     = "GuiTblCol2";

	static ConstString ACCDEF        = "TransformIntAccDefName";
	static ConstString ACCINST       = "TransformIntAccInstName";

	//--------------------------------------------------------------------------
	BEGIN_ACCESSOR_DEFINITION(ACCDEF)
		PROPERTY_DESCRIPTION_LIST("string dummy0;"
								  "int32  dummy1;"
								  "int32  " DEF_PROPTBLCOL0 ";"
								  "int16  " DEF_PROPTBLCOL1 ";"
                                  "int64  " DEF_PROPTBLCOL2 ";"
								  "date   dummy2;")
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

			pWidget    = new QWidget(0);
			pLabel[0]  = new QLabel(pWidget);
			pLabel[1]  = new QLabel(pWidget);
            pLabel[2]  = new QLabel(pWidget);
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
		QLabel                    * pLabel[3];
	};

	//--------------------------------------------------------------------------
	class FxTransform : public FxTransformBase
	{
	public:

		FxTransform()
		{
			pLabel[0]->setObjectName(PROPTBLCOL0);
			pLabel[1]->setObjectName(PROPTBLCOL1);
            pLabel[2]->setObjectName(PROPTBLCOL2);

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
			pLabel[0]->setObjectName(GUITBLCOL0);
			pLabel[1]->setObjectName(GUITBLCOL1);
            pLabel[2]->setObjectName(GUITBLCOL2);

			matcher		  = tie::Manager::getInstance().createMatcher(pWidget, accPropTbl);

			guiPropTbl    = matcher.getLeft();
			guiPropTblYIt = guiPropTbl.begin();
		}
	};

	static const Int32 INT_LEN  =         7;

	static const Int32 INT0     =       123;
	static const Int32 INT1     =   1234567;
	static const Int32 INT2     = 123456789;

    static const Int64 INT3     = 123456789012LL;
    static const Int64 INT4     = 1234567890123456LL;
    static const Int64 INT5     = 123456789012345678LL;

	static ConstString INT_STR0 = "0000123";
	static ConstString INT_STR1 = "1234567";
	static ConstString INT_STR2 = "123456789";

    static ConstString INT_STR3 = "123456789012";
    static ConstString INT_STR4 = "1234567890123456";
    static ConstString INT_STR5 = "123456789012345678";

//------------------------------------------------------------------------------
TEST_FIXTURE(FxTransform, CtorInt32_RL)
{
	try
	{
		IntTransformer tf (PROPTBLCOL0, INT_LEN);

		accPropTblYIt.setInt32(PROPTBLCOL0, INT0);
		tf.RightToLeft(guiPropTblYIt, accPropTblYIt);
		CHECK_EQUAL(INT_STR0, guiPropTblYIt.getString(PROPTBLCOL0).c_str());

		accPropTblYIt.setInt32(PROPTBLCOL0, INT1);
		tf.RightToLeft(guiPropTblYIt, accPropTblYIt);
		CHECK_EQUAL(INT_STR1, guiPropTblYIt.getString(PROPTBLCOL0).c_str());

		accPropTblYIt.setInt32(PROPTBLCOL0, INT2);
		tf.RightToLeft(guiPropTblYIt, accPropTblYIt);
		CHECK_EQUAL(INT_STR2, guiPropTblYIt.getString(PROPTBLCOL0).c_str());
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
TEST_FIXTURE(FxTransform, CtorInt32_LR)
{
	try
	{
		IntTransformer tf (PROPTBLCOL0, INT_LEN);

		guiPropTblYIt.setString(PROPTBLCOL0, INT_STR0);
		tf.LeftToRight(guiPropTblYIt, accPropTblYIt);
		CHECK(INT0 == accPropTblYIt.getInt32(PROPTBLCOL0));

		guiPropTblYIt.setString(PROPTBLCOL0, INT_STR1);
		tf.LeftToRight(guiPropTblYIt, accPropTblYIt);
		CHECK(INT1 == accPropTblYIt.getInt32(PROPTBLCOL0));

		guiPropTblYIt.setString(PROPTBLCOL0, INT_STR2);
		tf.LeftToRight(guiPropTblYIt, accPropTblYIt);
		CHECK(INT2 == accPropTblYIt.getInt32(PROPTBLCOL0));
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
TEST_FIXTURE(FxTransform, CtorInt64_RL)
{
	try
    {
		IntTransformer tf (PROPTBLCOL2, INT_LEN);

		accPropTblYIt.setInt64(PROPTBLCOL2, INT3);
		tf.RightToLeft(guiPropTblYIt, accPropTblYIt);
		CHECK_EQUAL(INT_STR3, guiPropTblYIt.getString(PROPTBLCOL2).c_str());

		accPropTblYIt.setInt64(PROPTBLCOL2, INT4);
		tf.RightToLeft(guiPropTblYIt, accPropTblYIt);
		CHECK_EQUAL(INT_STR4, guiPropTblYIt.getString(PROPTBLCOL2).c_str());

		accPropTblYIt.setInt64(PROPTBLCOL2, INT5);
		tf.RightToLeft(guiPropTblYIt, accPropTblYIt);
		CHECK_EQUAL(INT_STR5, guiPropTblYIt.getString(PROPTBLCOL2).c_str());
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
TEST_FIXTURE(FxTransform, CtorInt64_LR)
{
	try
	{
		IntTransformer tf (PROPTBLCOL2, INT_LEN);

		guiPropTblYIt.setString(PROPTBLCOL2, INT_STR3);
		tf.LeftToRight(guiPropTblYIt, accPropTblYIt);
		CHECK(INT3 == accPropTblYIt.getInt64(PROPTBLCOL2));

		guiPropTblYIt.setString(PROPTBLCOL2, INT_STR4);
		tf.LeftToRight(guiPropTblYIt, accPropTblYIt);
		CHECK(INT4 == accPropTblYIt.getInt64(PROPTBLCOL2));

		guiPropTblYIt.setString(PROPTBLCOL2, INT_STR5);
		tf.LeftToRight(guiPropTblYIt, accPropTblYIt);
		CHECK(INT5 == accPropTblYIt.getInt64(PROPTBLCOL2));
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
TEST_FIXTURE(FxTransform, CtorInt16_RL)
{
	try
	{
		IntTransformer tf (PROPTBLCOL1, INT_LEN);

		accPropTblYIt.setInt16(PROPTBLCOL1, INT0);
		tf.RightToLeft(guiPropTblYIt, accPropTblYIt);
		CHECK_EQUAL(INT_STR0, guiPropTblYIt.getString(PROPTBLCOL1).c_str());
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
TEST_FIXTURE(FxTransform, CtorInt16_LR)
{
	try
	{
		IntTransformer tf (PROPTBLCOL1, INT_LEN);

		guiPropTblYIt.setString(PROPTBLCOL1, INT_STR0);
		tf.LeftToRight(guiPropTblYIt, accPropTblYIt);
		CHECK(INT0 == accPropTblYIt.getInt16(PROPTBLCOL1));
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
		IntTransformer tf (PROPTBLCOL0, GUITBLCOL0, INT_LEN);

		accPropTblYIt.setInt32(PROPTBLCOL0, INT0);
		tf.RightToLeft(guiPropTblYIt, accPropTblYIt);
		CHECK_EQUAL(INT_STR0, guiPropTblYIt.getString(GUITBLCOL0).c_str());

		accPropTblYIt.setInt32(PROPTBLCOL0, INT1);
		tf.RightToLeft(guiPropTblYIt, accPropTblYIt);
		CHECK_EQUAL(INT_STR1, guiPropTblYIt.getString(GUITBLCOL0).c_str());

		accPropTblYIt.setInt32(PROPTBLCOL0, INT2);
		tf.RightToLeft(guiPropTblYIt, accPropTblYIt);
		CHECK_EQUAL(INT_STR2, guiPropTblYIt.getString(GUITBLCOL0).c_str());
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
		IntTransformer tf (PROPTBLCOL0, GUITBLCOL0, INT_LEN);

		guiPropTblYIt.setString(GUITBLCOL0, INT_STR0);
		tf.LeftToRight(guiPropTblYIt, accPropTblYIt);
		CHECK(INT0 == accPropTblYIt.getInt32(PROPTBLCOL0));

		guiPropTblYIt.setString(GUITBLCOL0, INT_STR1);
		tf.LeftToRight(guiPropTblYIt, accPropTblYIt);
		CHECK(INT1 == accPropTblYIt.getInt32(PROPTBLCOL0));

		guiPropTblYIt.setString(GUITBLCOL0, INT_STR2);
		tf.LeftToRight(guiPropTblYIt, accPropTblYIt);
		CHECK(INT2 == accPropTblYIt.getInt32(PROPTBLCOL0));
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
TEST_FIXTURE(FxTransformGuiCol, Ctor_GuiCol_Int64_RL)
{
	try
	{
		IntTransformer tf (PROPTBLCOL2, GUITBLCOL2, INT_LEN);

		accPropTblYIt.setInt64(PROPTBLCOL2, INT3);
		tf.RightToLeft(guiPropTblYIt, accPropTblYIt);
		CHECK_EQUAL(INT_STR3, guiPropTblYIt.getString(GUITBLCOL2).c_str());

		accPropTblYIt.setInt64(PROPTBLCOL2, INT4);
		tf.RightToLeft(guiPropTblYIt, accPropTblYIt);
		CHECK_EQUAL(INT_STR4, guiPropTblYIt.getString(GUITBLCOL2).c_str());

		accPropTblYIt.setInt64(PROPTBLCOL2, INT5);
		tf.RightToLeft(guiPropTblYIt, accPropTblYIt);
		CHECK_EQUAL(INT_STR5, guiPropTblYIt.getString(GUITBLCOL2).c_str());
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
TEST_FIXTURE(FxTransformGuiCol, Ctor_GuiCol_Int64_LR)
{
	try
	{
		IntTransformer tf (PROPTBLCOL2, GUITBLCOL2, INT_LEN);

		guiPropTblYIt.setString(GUITBLCOL2, INT_STR3);
		tf.LeftToRight(guiPropTblYIt, accPropTblYIt);
		CHECK(INT3 == accPropTblYIt.getInt64(PROPTBLCOL2));

		guiPropTblYIt.setString(GUITBLCOL2, INT_STR4);
		tf.LeftToRight(guiPropTblYIt, accPropTblYIt);
		CHECK(INT4 == accPropTblYIt.getInt64(PROPTBLCOL2));

		guiPropTblYIt.setString(GUITBLCOL2, INT_STR5);
		tf.LeftToRight(guiPropTblYIt, accPropTblYIt);
		CHECK(INT5 == accPropTblYIt.getInt64(PROPTBLCOL2));
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
