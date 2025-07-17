/* 
 * @file                                            
 * @author Michael Eichenlaub
 * @date   2010-04-09
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
using basar::Int32;
using basar::Decimal;
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
using tie::DecimalTransformer;

//------------------------------------------------------------------------------
SUITE(TransformDecimal)
{
	//--------------------------------------------------------------------------
	#define PROPTBLCOL					 "PropTblCol"

	//--------------------------------------------------------------------------
	static ConstString		GUITBLCOL     = "GuiTblCol";

	static ConstString		ACCDEF        = "TransformDecimalAccDefName";
	static ConstString		ACCINST       = "TransformDecimalAccInstName";
	static const Int32		DECPLACES     = 2;
	static const Decimal    DEC_RL        = Decimal("123,4567");
	static const Decimal    DEC_LR        = Decimal("123,46");
	static ConstString      DEC_STR       = "123,46";

	//--------------------------------------------------------------------------
	BEGIN_ACCESSOR_DEFINITION(ACCDEF)
		PROPERTY_DESCRIPTION_LIST("string dummy0;"
								  "int32  dummy1;"
								  "decimal " PROPTBLCOL ";"
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
	DecimalTransformer tf (PROPTBLCOL, DECPLACES);

	accPropTblYIt.setDecimal(PROPTBLCOL, DEC_RL);
	tf.RightToLeft(guiPropTblYIt, accPropTblYIt);
	CHECK_EQUAL(DEC_STR, guiPropTblYIt.getString(PROPTBLCOL).c_str());
}

//------------------------------------------------------------------------------
TEST_FIXTURE(FxTransform, Ctor_LR)
{
	DecimalTransformer tf (PROPTBLCOL, DECPLACES);

	guiPropTblYIt.setString(PROPTBLCOL, DEC_STR);
	tf.LeftToRight(guiPropTblYIt, accPropTblYIt);
	CHECK(DEC_LR == accPropTblYIt.getDecimal(PROPTBLCOL));
}

//------------------------------------------------------------------------------
TEST_FIXTURE(FxTransformGuiCol, Ctor_GuiCol_RL)
{
	try
	{
		DecimalTransformer tf (PROPTBLCOL, GUITBLCOL, DECPLACES);

		accPropTblYIt.setDecimal(PROPTBLCOL, DEC_RL);
		tf.RightToLeft(guiPropTblYIt, accPropTblYIt);
		CHECK_EQUAL(DEC_STR, guiPropTblYIt.getString(GUITBLCOL).c_str());
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
		DecimalTransformer tf (PROPTBLCOL, GUITBLCOL, DECPLACES);

		guiPropTblYIt.setString(GUITBLCOL, DEC_STR);
		tf.LeftToRight(guiPropTblYIt, accPropTblYIt);
		CHECK(DEC_LR == accPropTblYIt.getDecimal(PROPTBLCOL));
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
