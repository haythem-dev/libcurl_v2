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

#include <QtWidgets/QRadioButton>

//------------------------------------------------------------------------------
using basar::ConstString;
using basar::I18nString;
using basar::FOR_CLEAN;

using basar::cmnutil::ParameterList;
using basar::cmnutil::ParamPair;

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
using tie::RadioButtonTransformer;

//------------------------------------------------------------------------------
SUITE(TransformRadioButton)
{
	//--------------------------------------------------------------------------
	#define DEF_PROPTBLCOL			 "PropTblCol"

	#define DEF_PROPTBLCOL_VAL0      "0"
	#define DEF_PROPTBLCOL_VAL1      "x"
	#define DEF_PROPTBLCOL_VAL2      "7"

	#define DEF_WIDGET0              "rbt0"
	#define DEF_WIDGET1              "rbt1"
	#define DEF_WIDGET2              "rbt2"
	#define DEF_WIDGET3              "lbElse"

	//--------------------------------------------------------------------------
	static ConstString PROPTBLCOL    = DEF_PROPTBLCOL;

	static ConstString ACCDEF        = "TransformRadioButtonAccDefName";
	static ConstString ACCINST       = "TransformRadioButtonAccInstName";

	static ConstString PARAMLIST     = DEF_PROPTBLCOL_VAL0 " = " DEF_WIDGET0 "; " 
		                               DEF_PROPTBLCOL_VAL1 " = " DEF_WIDGET1 "; " 
									   DEF_PROPTBLCOL_VAL2 " = " DEF_WIDGET2 ";";

	//--------------------------------------------------------------------------
	BEGIN_ACCESSOR_DEFINITION(ACCDEF)
		PROPERTY_DESCRIPTION_LIST("string dummy0;"
								  "int32  dummy1;"
								  "string " DEF_PROPTBLCOL ";"
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

			pRb[0] = new QRadioButton(pWidget);
			pRb[1] = new QRadioButton(pWidget);
			pRb[2] = new QRadioButton(pWidget);
			pRb[3] = new QRadioButton(pWidget);

			pRb[0]->setObjectName(DEF_WIDGET0);
			pRb[1]->setObjectName(DEF_WIDGET1);
			pRb[2]->setObjectName(DEF_WIDGET2);
			pRb[3]->setObjectName(DEF_WIDGET3);

			matcher		  = tie::Manager::getInstance().createMatcher(pWidget, accPropTbl);

			guiPropTbl    = matcher.getLeft();
			guiPropTblYIt = guiPropTbl.begin();
		}

		~FxTransform()
		{
			delete pWidget;
		}

		bool isChecked(int index) const
		{
			for (int i = 0; i < 4; ++i)
			{
				if (i == index)
				{
					if (!pRb[i]->isChecked())
						return false;
				}
				else
				{
					if (pRb[i]->isChecked())
						return false;
				}
			}

			return true;
		};

	protected:
		AccessorInstanceRef         accInst;
		AccessorPropertyTableRef	accPropTbl;
		GuiPropertyTableRef         guiPropTbl;
		MatcherRef                  matcher;  
		QWidget                   * pWidget;
		QRadioButton              * pRb[4];
	};

//------------------------------------------------------------------------------
TEST_FIXTURE(FxTransform, CtorParamList_RL)
{
	CHECK_EQUAL(3, (int)paramList    .size              ());
	CHECK_EQUAL(4, (int)guiPropTblYIt.getNumberOfColumns());

	try
	{
		RadioButtonTransformer tf (paramList, PROPTBLCOL); 

		accPropTblYIt.setString(PROPTBLCOL, DEF_PROPTBLCOL_VAL0);
		tf.RightToLeft(guiPropTblYIt, accPropTblYIt);
		CHECK_EQUAL(RadioButtonTransformer::s_STATEON , guiPropTblYIt.getString(DEF_WIDGET0).c_str());
		CHECK_EQUAL(RadioButtonTransformer::s_STATEOFF, guiPropTblYIt.getString(DEF_WIDGET1).c_str());
		CHECK_EQUAL(RadioButtonTransformer::s_STATEOFF, guiPropTblYIt.getString(DEF_WIDGET2).c_str());
		CHECK_EQUAL(RadioButtonTransformer::s_STATEOFF, guiPropTblYIt.getString(DEF_WIDGET3).c_str());
		CHECK(isChecked(0));

		accPropTblYIt.setString(PROPTBLCOL, DEF_PROPTBLCOL_VAL1);
		tf.RightToLeft(guiPropTblYIt, accPropTblYIt);
		CHECK_EQUAL(RadioButtonTransformer::s_STATEON , guiPropTblYIt.getString(DEF_WIDGET1).c_str());
		CHECK_EQUAL(RadioButtonTransformer::s_STATEOFF, guiPropTblYIt.getString(DEF_WIDGET0).c_str());
		CHECK_EQUAL(RadioButtonTransformer::s_STATEOFF, guiPropTblYIt.getString(DEF_WIDGET2).c_str());
		CHECK_EQUAL(RadioButtonTransformer::s_STATEOFF, guiPropTblYIt.getString(DEF_WIDGET3).c_str());
		CHECK(isChecked(1));

		accPropTblYIt.setString(PROPTBLCOL, DEF_PROPTBLCOL_VAL2);
		tf.RightToLeft(guiPropTblYIt, accPropTblYIt);
		CHECK_EQUAL(RadioButtonTransformer::s_STATEON , guiPropTblYIt.getString(DEF_WIDGET2).c_str());
		CHECK_EQUAL(RadioButtonTransformer::s_STATEOFF, guiPropTblYIt.getString(DEF_WIDGET0).c_str());
		CHECK_EQUAL(RadioButtonTransformer::s_STATEOFF, guiPropTblYIt.getString(DEF_WIDGET1).c_str());
		CHECK_EQUAL(RadioButtonTransformer::s_STATEOFF, guiPropTblYIt.getString(DEF_WIDGET3).c_str());
		CHECK(isChecked(2));
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
		RadioButtonTransformer tf (paramList, PROPTBLCOL); 

		guiPropTblYIt.setString(DEF_WIDGET0, RadioButtonTransformer::s_STATEON);
		guiPropTblYIt.setString(DEF_WIDGET1, RadioButtonTransformer::s_STATEOFF);
		guiPropTblYIt.setString(DEF_WIDGET2, RadioButtonTransformer::s_STATEOFF);
		CHECK(isChecked(0));
		tf.LeftToRight(guiPropTblYIt, accPropTblYIt);
		CHECK_EQUAL(DEF_PROPTBLCOL_VAL0, accPropTblYIt.getString(PROPTBLCOL).c_str());

		guiPropTblYIt.setString(DEF_WIDGET0, RadioButtonTransformer::s_STATEOFF);
		guiPropTblYIt.setString(DEF_WIDGET1, RadioButtonTransformer::s_STATEON);
		guiPropTblYIt.setString(DEF_WIDGET2, RadioButtonTransformer::s_STATEOFF);
		CHECK(isChecked(1));
		tf.LeftToRight(guiPropTblYIt, accPropTblYIt);
		CHECK_EQUAL(DEF_PROPTBLCOL_VAL1, accPropTblYIt.getString(PROPTBLCOL).c_str());

		guiPropTblYIt.setString(DEF_WIDGET0, RadioButtonTransformer::s_STATEOFF);
		guiPropTblYIt.setString(DEF_WIDGET1, RadioButtonTransformer::s_STATEOFF);
		guiPropTblYIt.setString(DEF_WIDGET2, RadioButtonTransformer::s_STATEON );
		CHECK(isChecked(2));
		tf.LeftToRight(guiPropTblYIt, accPropTblYIt);
		CHECK_EQUAL(DEF_PROPTBLCOL_VAL2, accPropTblYIt.getString(PROPTBLCOL).c_str());
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
		RadioButtonTransformer tf (ParamPair(PARAMLIST), PROPTBLCOL); 

		accPropTblYIt.setString(PROPTBLCOL, DEF_PROPTBLCOL_VAL0);
		tf.RightToLeft(guiPropTblYIt, accPropTblYIt);
		CHECK_EQUAL(RadioButtonTransformer::s_STATEON , guiPropTblYIt.getString(DEF_WIDGET0).c_str());
		CHECK_EQUAL(RadioButtonTransformer::s_STATEOFF, guiPropTblYIt.getString(DEF_WIDGET1).c_str());
		CHECK_EQUAL(RadioButtonTransformer::s_STATEOFF, guiPropTblYIt.getString(DEF_WIDGET2).c_str());
		CHECK_EQUAL(RadioButtonTransformer::s_STATEOFF, guiPropTblYIt.getString(DEF_WIDGET3).c_str());
		CHECK(isChecked(0));

		accPropTblYIt.setString(PROPTBLCOL, DEF_PROPTBLCOL_VAL1);
		tf.RightToLeft(guiPropTblYIt, accPropTblYIt);
		CHECK_EQUAL(RadioButtonTransformer::s_STATEON , guiPropTblYIt.getString(DEF_WIDGET1).c_str());
		CHECK_EQUAL(RadioButtonTransformer::s_STATEOFF, guiPropTblYIt.getString(DEF_WIDGET0).c_str());
		CHECK_EQUAL(RadioButtonTransformer::s_STATEOFF, guiPropTblYIt.getString(DEF_WIDGET2).c_str());
		CHECK_EQUAL(RadioButtonTransformer::s_STATEOFF, guiPropTblYIt.getString(DEF_WIDGET3).c_str());
		CHECK(isChecked(1));

		accPropTblYIt.setString(PROPTBLCOL, DEF_PROPTBLCOL_VAL2);
		tf.RightToLeft(guiPropTblYIt, accPropTblYIt);
		CHECK_EQUAL(RadioButtonTransformer::s_STATEON , guiPropTblYIt.getString(DEF_WIDGET2).c_str());
		CHECK_EQUAL(RadioButtonTransformer::s_STATEOFF, guiPropTblYIt.getString(DEF_WIDGET0).c_str());
		CHECK_EQUAL(RadioButtonTransformer::s_STATEOFF, guiPropTblYIt.getString(DEF_WIDGET1).c_str());
		CHECK_EQUAL(RadioButtonTransformer::s_STATEOFF, guiPropTblYIt.getString(DEF_WIDGET3).c_str());
		CHECK(isChecked(2));
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
		RadioButtonTransformer tf (ParamPair(PARAMLIST), PROPTBLCOL); 

		guiPropTblYIt.setString(DEF_WIDGET0, RadioButtonTransformer::s_STATEON);
		guiPropTblYIt.setString(DEF_WIDGET1, RadioButtonTransformer::s_STATEOFF);
		guiPropTblYIt.setString(DEF_WIDGET2, RadioButtonTransformer::s_STATEOFF);
		CHECK(isChecked(0));
		tf.LeftToRight(guiPropTblYIt, accPropTblYIt);
		CHECK_EQUAL(DEF_PROPTBLCOL_VAL0, accPropTblYIt.getString(PROPTBLCOL).c_str());

		guiPropTblYIt.setString(DEF_WIDGET0, RadioButtonTransformer::s_STATEOFF);
		guiPropTblYIt.setString(DEF_WIDGET1, RadioButtonTransformer::s_STATEON);
		guiPropTblYIt.setString(DEF_WIDGET2, RadioButtonTransformer::s_STATEOFF);
		CHECK(isChecked(1));
		tf.LeftToRight(guiPropTblYIt, accPropTblYIt);
		CHECK_EQUAL(DEF_PROPTBLCOL_VAL1, accPropTblYIt.getString(PROPTBLCOL).c_str());

		guiPropTblYIt.setString(DEF_WIDGET0, RadioButtonTransformer::s_STATEOFF);
		guiPropTblYIt.setString(DEF_WIDGET1, RadioButtonTransformer::s_STATEOFF);
		guiPropTblYIt.setString(DEF_WIDGET2, RadioButtonTransformer::s_STATEON );
		CHECK(isChecked(2));
		tf.LeftToRight(guiPropTblYIt, accPropTblYIt);
		CHECK_EQUAL(DEF_PROPTBLCOL_VAL2, accPropTblYIt.getString(PROPTBLCOL).c_str());
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
TEST_FIXTURE(FxTransform, CtorString_GuiDriven_LR)
{
	try
	{
		RadioButtonTransformer tf (ParamPair(PARAMLIST), PROPTBLCOL); 

		pRb[0]->toggle();
		CHECK_EQUAL(RadioButtonTransformer::s_STATEON , guiPropTblYIt.getString(DEF_WIDGET0).c_str());
		CHECK_EQUAL(RadioButtonTransformer::s_STATEOFF, guiPropTblYIt.getString(DEF_WIDGET1).c_str());
		CHECK_EQUAL(RadioButtonTransformer::s_STATEOFF, guiPropTblYIt.getString(DEF_WIDGET2).c_str());
		CHECK_EQUAL(RadioButtonTransformer::s_STATEOFF, guiPropTblYIt.getString(DEF_WIDGET3).c_str());
		CHECK(isChecked(0));
		tf.LeftToRight(guiPropTblYIt, accPropTblYIt);
		CHECK_EQUAL(DEF_PROPTBLCOL_VAL0, accPropTblYIt.getString(PROPTBLCOL).c_str());

		pRb[1]->toggle();
		CHECK_EQUAL(RadioButtonTransformer::s_STATEON , guiPropTblYIt.getString(DEF_WIDGET1).c_str());
		CHECK_EQUAL(RadioButtonTransformer::s_STATEOFF, guiPropTblYIt.getString(DEF_WIDGET0).c_str());
		CHECK_EQUAL(RadioButtonTransformer::s_STATEOFF, guiPropTblYIt.getString(DEF_WIDGET2).c_str());
		CHECK_EQUAL(RadioButtonTransformer::s_STATEOFF, guiPropTblYIt.getString(DEF_WIDGET3).c_str());
		CHECK(isChecked(1));
		tf.LeftToRight(guiPropTblYIt, accPropTblYIt);
		CHECK_EQUAL(DEF_PROPTBLCOL_VAL1, accPropTblYIt.getString(PROPTBLCOL).c_str());

		pRb[2]->toggle();
		CHECK_EQUAL(RadioButtonTransformer::s_STATEON , guiPropTblYIt.getString(DEF_WIDGET2).c_str());
		CHECK_EQUAL(RadioButtonTransformer::s_STATEOFF, guiPropTblYIt.getString(DEF_WIDGET1).c_str());
		CHECK_EQUAL(RadioButtonTransformer::s_STATEOFF, guiPropTblYIt.getString(DEF_WIDGET0).c_str());
		CHECK_EQUAL(RadioButtonTransformer::s_STATEOFF, guiPropTblYIt.getString(DEF_WIDGET3).c_str());
		CHECK(isChecked(2));
		tf.LeftToRight(guiPropTblYIt, accPropTblYIt);
		CHECK_EQUAL(DEF_PROPTBLCOL_VAL2, accPropTblYIt.getString(PROPTBLCOL).c_str());
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
