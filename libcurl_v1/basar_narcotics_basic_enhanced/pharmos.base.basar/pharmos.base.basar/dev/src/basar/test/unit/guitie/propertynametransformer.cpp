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
using basar::I18nString;
using basar::FOR_CLEAN;

using basar::cmnutil::ParameterList;

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
using tie::PropertyNameTransformer;

//------------------------------------------------------------------------------
SUITE(TransformPropertyName)
{
	//--------------------------------------------------------------------------
	#define DEF_PROPTBLCOL0			 "PropTblCol0"
	#define DEF_PROPTBLCOL1			 "PropTblCol1"

	#define DEF_PROPTBLCOL_VAL0      "abcdefgh"
	#define DEF_PROPTBLCOL_VAL1      "TUVWXYZ"

	#define DEF_WIDGET0              "widget0"
	#define DEF_WIDGET1              "widget1"

	//--------------------------------------------------------------------------
	static ConstString ACCDEF        = "TransformPropertyNameAccDefName";
	static ConstString ACCINST       = "TransformPropertyNameAccInstName";

	static ConstString PARAMLIST     = DEF_PROPTBLCOL0 " = " DEF_WIDGET0 "; " 
		                               DEF_PROPTBLCOL1 " = " DEF_WIDGET1 "; ";

	//--------------------------------------------------------------------------
	BEGIN_ACCESSOR_DEFINITION(ACCDEF)
		PROPERTY_DESCRIPTION_LIST("string dummy0;"
								  "int32  dummy1;"
								  "string " DEF_PROPTBLCOL0 ";"
		                          "string " DEF_PROPTBLCOL1 ";"
								  "date   dummy2;")
	END_ACCESSOR_DEFINITION

	//--------------------------------------------------------------------------
	class FxTransform
	{
	public:
		AccessorPropertyTable_YIterator accPropTblYIt;
		GuiPropertyTable_YIterator      guiPropTblYIt;
		ParameterList                   paramList;

		FxTransform() : paramList(PARAMLIST)
		{
			accInst       = aspect::Manager::getInstance().createAccessorInstance(ACCINST,
																		          ACCDEF,
																				  ConnectionRef(), 
																				  FULL_CACHING);
			accPropTbl    = accInst.getPropertyTable();
			accPropTbl.insert(FOR_CLEAN);
			accPropTblYIt = accPropTbl.begin();

			pWidget   = new QWidget(0);

			pLabel[0] = new QLabel(pWidget);
			pLabel[1] = new QLabel(pWidget);

			pLabel[0]->setObjectName(DEF_WIDGET0);
			pLabel[1]->setObjectName(DEF_WIDGET1);

			matcher		  = tie::Manager::getInstance().createMatcher(pWidget, accPropTbl);

			guiPropTbl    = matcher.getLeft();
			guiPropTblYIt = guiPropTbl.begin();
		}

		~FxTransform()
		{
			delete pWidget;
		}

	protected:
		AccessorInstanceRef         accInst;
		AccessorPropertyTableRef	accPropTbl;
		GuiPropertyTableRef         guiPropTbl;
		MatcherRef                  matcher;  
		QWidget                   * pWidget;
		QLabel                    * pLabel[2];
	};

//------------------------------------------------------------------------------
TEST_FIXTURE(FxTransform, CtorParamList_RL)
{
	try
	{
		PropertyNameTransformer tf (paramList); 

		accPropTblYIt.setString(DEF_PROPTBLCOL0, DEF_PROPTBLCOL_VAL0);
		accPropTblYIt.setString(DEF_PROPTBLCOL1, DEF_PROPTBLCOL_VAL1);

		tf.RightToLeft(guiPropTblYIt, accPropTblYIt);

		CHECK_EQUAL(DEF_PROPTBLCOL_VAL0, guiPropTblYIt.getString(DEF_WIDGET0).c_str());
		CHECK_EQUAL(DEF_PROPTBLCOL_VAL1, guiPropTblYIt.getString(DEF_WIDGET1).c_str());
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
TEST_FIXTURE(FxTransform, CtorParamList_LR)
{
	try
	{
		PropertyNameTransformer tf (paramList); 

		guiPropTblYIt.setString(DEF_WIDGET0, DEF_PROPTBLCOL_VAL0);
		guiPropTblYIt.setString(DEF_WIDGET1, DEF_PROPTBLCOL_VAL1);

		tf.LeftToRight(guiPropTblYIt, accPropTblYIt);

		CHECK_EQUAL(DEF_PROPTBLCOL_VAL0, accPropTblYIt.getString(DEF_PROPTBLCOL0).c_str());
		CHECK_EQUAL(DEF_PROPTBLCOL_VAL1, accPropTblYIt.getString(DEF_PROPTBLCOL1).c_str());
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
TEST_FIXTURE(FxTransform, CtorString_RL)
{
	try
	{
		PropertyNameTransformer tf(PARAMLIST);

		accPropTblYIt.setString(DEF_PROPTBLCOL0, DEF_PROPTBLCOL_VAL0);
		accPropTblYIt.setString(DEF_PROPTBLCOL1, DEF_PROPTBLCOL_VAL1);

		tf.RightToLeft(guiPropTblYIt, accPropTblYIt);

		CHECK_EQUAL(DEF_PROPTBLCOL_VAL0 , guiPropTblYIt.getString(DEF_WIDGET0).c_str());
		CHECK_EQUAL(DEF_PROPTBLCOL_VAL1, guiPropTblYIt.getString(DEF_WIDGET1).c_str());
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
TEST_FIXTURE(FxTransform, CtorString_LR)
{
	try
	{
		PropertyNameTransformer tf (PARAMLIST); 

		guiPropTblYIt.setString(DEF_WIDGET0, DEF_PROPTBLCOL_VAL0);
		guiPropTblYIt.setString(DEF_WIDGET1, DEF_PROPTBLCOL_VAL1);

		tf.LeftToRight(guiPropTblYIt, accPropTblYIt);

		CHECK_EQUAL(DEF_PROPTBLCOL_VAL0, accPropTblYIt.getString(DEF_PROPTBLCOL0).c_str());
		CHECK_EQUAL(DEF_PROPTBLCOL_VAL1, accPropTblYIt.getString(DEF_PROPTBLCOL1).c_str());
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
