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
#include <QtWidgets/QCheckBox>
#include <QtWidgets/QGroupBox>

#ifdef _WIN32
#pragma warning(disable: 4834)
#endif

//------------------------------------------------------------------------------
using basar::ConstString;
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
using tie::CheckBoxTransformer;

//------------------------------------------------------------------------------
SUITE(TransformCheckBox)
{
	//--------------------------------------------------------------------------
	#define PROPTBLCOL				 "PropTblCol"
	#define PROPTBLCOL2				 "PropTblCol2"

	//--------------------------------------------------------------------------
	static ConstString GUITBLCOL     = "GuiTblCol";
	static ConstString DBCHECKED     = "checked";
	static ConstString DBPARTCHECKED = "partially checked";
	static ConstString DBUNCHECKED   = "unchecked";
	static ConstString DBWHATEVER    = "xyz";

	static ConstString ACCDEF        = "TransformCheckBoxAccDefName";
	static ConstString ACCINST       = "TransformCheckBoxAccInstName";

	//--------------------------------------------------------------------------
	BEGIN_ACCESSOR_DEFINITION(ACCDEF)
		PROPERTY_DESCRIPTION_LIST("string dummy0;"
								  "int32  dummy1;"
								  "string " PROPTBLCOL  ";"
								  "string " PROPTBLCOL2 ";"
								  "date   dummy2;")
	END_ACCESSOR_DEFINITION

	//--------------------------------------------------------------------------
	class FxTransformBase
	{
	public:
		AccessorPropertyTable_YIterator accPropTblYIt;
		GuiPropertyTable_YIterator      guiPropTblYIt;

		FxTransformBase() : pWidget  (new QWidget  (0)),
		                    pCheckBox(new QCheckBox(pWidget)),
			                pGroupBox(new QGroupBox(pWidget))
		{
			pGroupBox->setCheckable(true);

			accInst       = aspect::Manager::getInstance().createAccessorInstance(ACCINST,
																		          ACCDEF,
																				  ConnectionRef(), 
																				  FULL_CACHING);
			accPropTbl    = accInst.getPropertyTable();
			accPropTbl.insert(FOR_CLEAN);
			accPropTblYIt = accPropTbl.begin();
		}

		~FxTransformBase()
		{
			delete pWidget;
		}

	protected:
		QWidget                   * pWidget;
		QCheckBox                 * pCheckBox;
		QGroupBox                 * pGroupBox;

		AccessorInstanceRef         accInst;
		AccessorPropertyTableRef	accPropTbl;
		GuiPropertyTableRef         guiPropTbl;
		MatcherRef                  matcher;  
	};

	//--------------------------------------------------------------------------
	class FxCheckBox : public FxTransformBase
	{
	public:

		FxCheckBox()
		{
			pCheckBox->setObjectName(PROPTBLCOL);

			matcher		  = tie::Manager::getInstance().createMatcher(pCheckBox, accPropTbl);

			guiPropTbl    = matcher.getLeft();
			guiPropTblYIt = guiPropTbl.begin();
		}
	};

	//--------------------------------------------------------------------------
	class FxCheckBoxGuiCol : public FxTransformBase
	{
	public:

		FxCheckBoxGuiCol()
		{
			pCheckBox->setObjectName(GUITBLCOL);

			matcher		  = tie::Manager::getInstance().createMatcher(pCheckBox, accPropTbl);

			guiPropTbl    = matcher.getLeft();
			guiPropTblYIt = guiPropTbl.begin();
		}
	};

	//--------------------------------------------------------------------------
	class FxGroupBox : public FxTransformBase
	{
	public:

		FxGroupBox()
		{
			pGroupBox->setObjectName(PROPTBLCOL);

			matcher		  = tie::Manager::getInstance().createMatcher(pGroupBox, accPropTbl);

			guiPropTbl    = matcher.getLeft();
			guiPropTblYIt = guiPropTbl.begin();
		}
	};

	//--------------------------------------------------------------------------
	class FxGroupBoxWithLabel : public FxTransformBase
	{
	public:

		FxGroupBoxWithLabel() : pLabel(new QLabel(pGroupBox))
		{
			pGroupBox->setObjectName(PROPTBLCOL);

			pLabel   ->setObjectName(LABELNAME);
			pLabel   ->setText      (LABELTEXT);

			matcher		  = tie::Manager::getInstance().createMatcher(pGroupBox, accPropTbl);

			guiPropTbl    = matcher.getLeft();
			guiPropTblYIt = guiPropTbl.begin();
		}

		~FxGroupBoxWithLabel()
		{
			// CHECK_EQUAL(LABELNAME, pLabel->objectName().toLocal8Bit().constData());  C4297 function assumed not to throw an exception but does
			// CHECK_EQUAL(LABELTEXT, pLabel->text      ().toLocal8Bit().constData());  C4297 function assumed not to throw an exception but does
		}

	protected:
		static ConstString LABELTEXT;
		static ConstString LABELNAME;

		QLabel           * pLabel;
	};

	ConstString FxGroupBoxWithLabel::LABELTEXT = "labeltext";
	ConstString FxGroupBoxWithLabel::LABELNAME = "labelname";

	//--------------------------------------------------------------------------
	class FxGroupBoxWithCheckBoxes : public FxTransformBase
	{
	public:

		FxGroupBoxWithCheckBoxes() 
		{
			pGroupBox->setObjectName(PROPTBLCOL);

			pCheckBox[0] = new QCheckBox(pGroupBox);
			pCheckBox[1] = new QCheckBox(pGroupBox);
			pCheckBox[2] = new QCheckBox(pGroupBox);

			pCheckBox[0]->setObjectName(CHECKBOXNAME[0]);
			pCheckBox[1]->setObjectName(CHECKBOXNAME[1]);
			pCheckBox[2]->setObjectName(CHECKBOXNAME[2]);

			pCheckBox[0]->setCheckState(STATE[0]);
			pCheckBox[1]->setCheckState(STATE[1]);
			pCheckBox[2]->setCheckState(STATE[2]);

			matcher		  = tie::Manager::getInstance().createMatcher(pGroupBox, accPropTbl);

			guiPropTbl    = matcher.getLeft();
			guiPropTblYIt = guiPropTbl.begin();
		}

		~FxGroupBoxWithCheckBoxes()
		{
			// CHECK_EQUAL(CHECKBOXNAME[0], pCheckBox[0]->objectName().toLocal8Bit().constData()); C4297 function assumed not to throw an exception but does
			// CHECK_EQUAL(CHECKBOXNAME[1], pCheckBox[1]->objectName().toLocal8Bit().constData()); C4297 function assumed not to throw an exception but does
			// CHECK_EQUAL(CHECKBOXNAME[2], pCheckBox[2]->objectName().toLocal8Bit().constData()); C4297 function assumed not to throw an exception but does

			// CHECK_EQUAL(STATE[0], pCheckBox[0]->checkState());                                  C4297 function assumed not to throw an exception but does
			// CHECK_EQUAL(STATE[1], pCheckBox[1]->checkState());                                  C4297 function assumed not to throw an exception but does
			// CHECK_EQUAL(STATE[2], pCheckBox[2]->checkState());                                  C4297 function assumed not to throw an exception but does
		}

	protected:
		static const Qt::CheckState STATE       [3];
		static ConstString          CHECKBOXNAME[3];

		QCheckBox          * pCheckBox[3];
	};

	const Qt::CheckState FxGroupBoxWithCheckBoxes::STATE[] = 
	{ 
		Qt::Checked, 
		Qt::Unchecked, 
		Qt::Checked 
	};
	ConstString FxGroupBoxWithCheckBoxes::CHECKBOXNAME[] = 
	{ 
		"CheckBox0", 
		"CheckBox1", 
		"CheckBox2" 
	};

	//--------------------------------------------------------------------------
	class FxGroupBoxGuiCol : public FxTransformBase
	{
	public:

		FxGroupBoxGuiCol()
		{
			pGroupBox->setObjectName(GUITBLCOL);

			matcher		  = tie::Manager::getInstance().createMatcher(pGroupBox, accPropTbl);

			guiPropTbl    = matcher.getLeft();
			guiPropTblYIt = guiPropTbl.begin();
		}
	};

//------------------------------------------------------------------------------
TEST_FIXTURE(FxCheckBox, MatchCheckBox_without_Transformer_RL)
{
	try
	{
		accPropTblYIt.setString(PROPTBLCOL, DBCHECKED);
		matcher.RightToLeft();
		CHECK_EQUAL(CheckBoxTransformer::s_STATEOFF, guiPropTblYIt.getString(PROPTBLCOL).c_str());
		CHECK_EQUAL(Qt::Unchecked, pCheckBox->checkState());
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
TEST_FIXTURE(FxCheckBox, MatchCheckBox_without_Transformer_LR)
{
	try
	{
		guiPropTblYIt.setString(PROPTBLCOL, CheckBoxTransformer::s_STATEON);
		CHECK_EQUAL(Qt::Checked, pCheckBox->checkState());
		matcher.LeftToRight();
		CHECK_THROW(accPropTblYIt.getString(PROPTBLCOL).c_str(), basar::Exception);

		pCheckBox->setCheckState(Qt::Checked);
		CHECK_EQUAL(CheckBoxTransformer::s_STATEON, guiPropTblYIt.getString(PROPTBLCOL).c_str());
		CHECK_EQUAL(Qt::Checked, pCheckBox->checkState());
		matcher.LeftToRight();
		CHECK_THROW(accPropTblYIt.getString(PROPTBLCOL).c_str(), basar::Exception);
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
TEST_FIXTURE(FxCheckBox, MatchCheckBoxTwoState_EvalOn_RL_Unset_Default)
{
	CheckBoxTransformer tf (PROPTBLCOL, 
							DBCHECKED, DBUNCHECKED, 
							CheckBoxTransformer::EVAL_STATE_ON);
	matcher.push_back(tf);

	accPropTblYIt.setPropertyState(PROPTBLCOL, basar::SS_UNSET);
	matcher.RightToLeft();
	CHECK_EQUAL(CheckBoxTransformer::s_STATEOFF, guiPropTblYIt.getString(PROPTBLCOL).c_str());
	CHECK_EQUAL(Qt::Unchecked, pCheckBox->checkState());
}

//------------------------------------------------------------------------------
TEST_FIXTURE(FxCheckBox, MatchCheckBoxTwoState_EvalOn_RL_Unset)
{
	CheckBoxTransformer tf (PROPTBLCOL, 
							DBCHECKED, DBUNCHECKED, 
							CheckBoxTransformer::EVAL_STATE_ON);

	matcher.push_back(tf);

	guiPropTblYIt.setString(PROPTBLCOL, CheckBoxTransformer::s_STATEON);
	CHECK_EQUAL(CheckBoxTransformer::s_STATEON, guiPropTblYIt.getString(PROPTBLCOL).c_str());
	CHECK_EQUAL(Qt::Checked, pCheckBox->checkState());

	accPropTblYIt.setPropertyState(PROPTBLCOL, basar::SS_UNSET);
	matcher.RightToLeft();
	CHECK_EQUAL(CheckBoxTransformer::s_STATEON, guiPropTblYIt.getString(PROPTBLCOL).c_str());
	CHECK_EQUAL(Qt::Checked, pCheckBox->checkState());
}

//------------------------------------------------------------------------------
TEST_FIXTURE(FxCheckBox, MatchCheckBoxTwoState_EvalOn_GuiDriven_RL_Unset)
{
	CheckBoxTransformer tf (PROPTBLCOL, 
							DBCHECKED, DBUNCHECKED, 
							CheckBoxTransformer::EVAL_STATE_ON);

	matcher.push_back(tf);

	pCheckBox->setCheckState(Qt::Checked);
	CHECK_EQUAL(CheckBoxTransformer::s_STATEON, guiPropTblYIt.getString(PROPTBLCOL).c_str());
	CHECK_EQUAL(Qt::Checked, pCheckBox->checkState());

	accPropTblYIt.setPropertyState(PROPTBLCOL, basar::SS_UNSET);
	matcher.RightToLeft();
	CHECK_EQUAL(CheckBoxTransformer::s_STATEON, guiPropTblYIt.getString(PROPTBLCOL).c_str());
	CHECK_EQUAL(Qt::Checked, pCheckBox->checkState());
}

//------------------------------------------------------------------------------
TEST_FIXTURE(FxCheckBox, CheckBoxTwoState_EvalOn_RL_Unset_Default)
{
	CheckBoxTransformer tf (PROPTBLCOL, 
							DBCHECKED, DBUNCHECKED, 
							CheckBoxTransformer::EVAL_STATE_ON);

	accPropTblYIt.setPropertyState(PROPTBLCOL, basar::SS_UNSET);
	tf.RightToLeft(guiPropTblYIt, accPropTblYIt);
	CHECK_EQUAL(CheckBoxTransformer::s_STATEOFF, guiPropTblYIt.getString(PROPTBLCOL).c_str());
	CHECK_EQUAL(Qt::Unchecked, pCheckBox->checkState());
}

//------------------------------------------------------------------------------
TEST_FIXTURE(FxCheckBox, CheckBoxTwoState_EvalOn_RL_Unset)
{
	CheckBoxTransformer tf (PROPTBLCOL, 
							DBCHECKED, DBUNCHECKED, 
							CheckBoxTransformer::EVAL_STATE_ON);

	guiPropTblYIt.setString(PROPTBLCOL, CheckBoxTransformer::s_STATEON);
	CHECK_EQUAL(CheckBoxTransformer::s_STATEON, guiPropTblYIt.getString(PROPTBLCOL).c_str());
	CHECK_EQUAL(Qt::Checked, pCheckBox->checkState());

	accPropTblYIt.setPropertyState(PROPTBLCOL, basar::SS_UNSET);
	tf.RightToLeft(guiPropTblYIt, accPropTblYIt);
	CHECK_EQUAL(CheckBoxTransformer::s_STATEON, guiPropTblYIt.getString(PROPTBLCOL).c_str());
	CHECK_EQUAL(Qt::Checked, pCheckBox->checkState());
}

//------------------------------------------------------------------------------
TEST_FIXTURE(FxCheckBox, CheckBoxTwoState_EvalOn_GuiDriven_RL_Unset)
{
	CheckBoxTransformer tf (PROPTBLCOL, 
							DBCHECKED, DBUNCHECKED, 
							CheckBoxTransformer::EVAL_STATE_ON);

	pCheckBox->setCheckState(Qt::Checked);
	CHECK_EQUAL(CheckBoxTransformer::s_STATEON, guiPropTblYIt.getString(PROPTBLCOL).c_str());
	CHECK_EQUAL(Qt::Checked, pCheckBox->checkState());

	accPropTblYIt.setPropertyState(PROPTBLCOL, basar::SS_UNSET);
	tf.RightToLeft(guiPropTblYIt, accPropTblYIt);
	CHECK_EQUAL(CheckBoxTransformer::s_STATEON, guiPropTblYIt.getString(PROPTBLCOL).c_str());
	CHECK_EQUAL(Qt::Checked, pCheckBox->checkState());
}

//------------------------------------------------------------------------------
TEST_FIXTURE(FxCheckBox, CheckBoxTwoState_EvalOn_RL)
{
	CheckBoxTransformer tf (PROPTBLCOL, 
							DBCHECKED, DBUNCHECKED, 
							CheckBoxTransformer::EVAL_STATE_ON);

	accPropTblYIt.setString(PROPTBLCOL, DBCHECKED);
	tf.RightToLeft(guiPropTblYIt, accPropTblYIt);
	CHECK_EQUAL(CheckBoxTransformer::s_STATEON, guiPropTblYIt.getString(PROPTBLCOL).c_str());
	CHECK_EQUAL(Qt::Checked, pCheckBox->checkState());

	accPropTblYIt.setString(PROPTBLCOL, DBUNCHECKED);
	tf.RightToLeft(guiPropTblYIt, accPropTblYIt);
	CHECK_EQUAL(CheckBoxTransformer::s_STATEOFF, guiPropTblYIt.getString(PROPTBLCOL).c_str());
	CHECK_EQUAL(Qt::Unchecked, pCheckBox->checkState());

	accPropTblYIt.setString(PROPTBLCOL, DBWHATEVER);
	tf.RightToLeft(guiPropTblYIt, accPropTblYIt);
	CHECK_EQUAL(CheckBoxTransformer::s_STATEON, guiPropTblYIt.getString(PROPTBLCOL).c_str());
	CHECK_EQUAL(Qt::Checked, pCheckBox->checkState());
}

//------------------------------------------------------------------------------
TEST_FIXTURE(FxCheckBox, CheckBoxTwoState_EvalOn_LR)
{
	CheckBoxTransformer tf (PROPTBLCOL, 
							DBCHECKED, DBUNCHECKED, 
							CheckBoxTransformer::EVAL_STATE_ON);

	guiPropTblYIt.setString(PROPTBLCOL, CheckBoxTransformer::s_STATEON);
	CHECK_EQUAL(Qt::Checked, pCheckBox->checkState());
	tf.LeftToRight(guiPropTblYIt, accPropTblYIt);
	CHECK_EQUAL(DBCHECKED, accPropTblYIt.getString(PROPTBLCOL).c_str());

	guiPropTblYIt.setString(PROPTBLCOL, CheckBoxTransformer::s_STATEOFF);
	CHECK_EQUAL(Qt::Unchecked, pCheckBox->checkState());
	tf.LeftToRight(guiPropTblYIt, accPropTblYIt);
	CHECK_EQUAL(DBUNCHECKED, accPropTblYIt.getString(PROPTBLCOL).c_str());
}

//------------------------------------------------------------------------------
TEST_FIXTURE(FxCheckBox, CheckBoxTwoState_EvalOn_GuiDriven_LR)
{
	CheckBoxTransformer tf (PROPTBLCOL, 
							DBCHECKED, DBUNCHECKED, 
							CheckBoxTransformer::EVAL_STATE_ON);

	pCheckBox->setCheckState(Qt::Checked);
	CHECK_EQUAL(CheckBoxTransformer::s_STATEON, guiPropTblYIt.getString(PROPTBLCOL).c_str());
	CHECK_EQUAL(Qt::Checked, pCheckBox->checkState());
	tf.LeftToRight(guiPropTblYIt, accPropTblYIt);
	CHECK_EQUAL(DBCHECKED, accPropTblYIt.getString(PROPTBLCOL).c_str());

	pCheckBox->setCheckState(Qt::Unchecked);
	CHECK_EQUAL(CheckBoxTransformer::s_STATEOFF, guiPropTblYIt.getString(PROPTBLCOL).c_str());
	CHECK_EQUAL(Qt::Unchecked, pCheckBox->checkState());
	tf.LeftToRight(guiPropTblYIt, accPropTblYIt);
	CHECK_EQUAL(DBUNCHECKED, accPropTblYIt.getString(PROPTBLCOL).c_str());
}

//------------------------------------------------------------------------------
TEST_FIXTURE(FxCheckBox, CheckBoxTwoState_EvalOff_RL)
{
	CheckBoxTransformer tf (PROPTBLCOL, 
							DBCHECKED, DBUNCHECKED, 
							CheckBoxTransformer::EVAL_STATE_OFF);

	accPropTblYIt.setString(PROPTBLCOL, DBCHECKED);
	tf.RightToLeft(guiPropTblYIt, accPropTblYIt);
	CHECK_EQUAL(CheckBoxTransformer::s_STATEON, guiPropTblYIt.getString(PROPTBLCOL).c_str());
	CHECK_EQUAL(Qt::Checked, pCheckBox->checkState());

	accPropTblYIt.setString(PROPTBLCOL, DBUNCHECKED);
	tf.RightToLeft(guiPropTblYIt, accPropTblYIt);
	CHECK_EQUAL(CheckBoxTransformer::s_STATEOFF, guiPropTblYIt.getString(PROPTBLCOL).c_str());
	CHECK_EQUAL(Qt::Unchecked, pCheckBox->checkState());

	accPropTblYIt.setString(PROPTBLCOL, DBWHATEVER);
	tf.RightToLeft(guiPropTblYIt, accPropTblYIt);
	CHECK_EQUAL(CheckBoxTransformer::s_STATEOFF, guiPropTblYIt.getString(PROPTBLCOL).c_str());
	CHECK_EQUAL(Qt::Unchecked, pCheckBox->checkState());
}

//------------------------------------------------------------------------------
TEST_FIXTURE(FxCheckBox, CheckBoxTwoState_EvalOff_LR)
{
	CheckBoxTransformer tf (PROPTBLCOL, 
							DBCHECKED, DBUNCHECKED, 
							CheckBoxTransformer::EVAL_STATE_OFF);

	guiPropTblYIt.setString(PROPTBLCOL, CheckBoxTransformer::s_STATEON);
	CHECK_EQUAL(Qt::Checked, pCheckBox->checkState());
	tf.LeftToRight(guiPropTblYIt, accPropTblYIt);
	CHECK_EQUAL(DBCHECKED, accPropTblYIt.getString(PROPTBLCOL).c_str());

	guiPropTblYIt.setString(PROPTBLCOL, CheckBoxTransformer::s_STATEOFF);
	CHECK_EQUAL(Qt::Unchecked, pCheckBox->checkState());
	tf.LeftToRight(guiPropTblYIt, accPropTblYIt);
	CHECK_EQUAL(DBUNCHECKED, accPropTblYIt.getString(PROPTBLCOL).c_str());
}

//------------------------------------------------------------------------------
TEST_FIXTURE(FxCheckBoxGuiCol, CheckBoxTwoState_GuiTbl_EvalOn_RL)
{
	CheckBoxTransformer tf (GUITBLCOL, 
							DBCHECKED, DBUNCHECKED, 
							CheckBoxTransformer::EVAL_STATE_ON, 
							PROPTBLCOL);

	accPropTblYIt.setString(PROPTBLCOL, DBCHECKED);
	tf.RightToLeft(guiPropTblYIt, accPropTblYIt);
	CHECK_EQUAL(CheckBoxTransformer::s_STATEON, guiPropTblYIt.getString(GUITBLCOL).c_str());
	CHECK_EQUAL(Qt::Checked, pCheckBox->checkState());

	accPropTblYIt.setString(PROPTBLCOL, DBUNCHECKED);
	tf.RightToLeft(guiPropTblYIt, accPropTblYIt);
	CHECK_EQUAL(CheckBoxTransformer::s_STATEOFF, guiPropTblYIt.getString(GUITBLCOL).c_str());
	CHECK_EQUAL(Qt::Unchecked, pCheckBox->checkState());

	accPropTblYIt.setString(PROPTBLCOL, DBWHATEVER);
	tf.RightToLeft(guiPropTblYIt, accPropTblYIt);
	CHECK_EQUAL(CheckBoxTransformer::s_STATEON, guiPropTblYIt.getString(GUITBLCOL).c_str());
	CHECK_EQUAL(Qt::Checked, pCheckBox->checkState());
}

//------------------------------------------------------------------------------
TEST_FIXTURE(FxCheckBoxGuiCol, CheckBoxTwoState_GuiTbl_EvalOn_LR)
{
	CheckBoxTransformer tf (GUITBLCOL, 
							DBCHECKED, DBUNCHECKED, 
							CheckBoxTransformer::EVAL_STATE_ON, 
							PROPTBLCOL);

	guiPropTblYIt.setString(GUITBLCOL, CheckBoxTransformer::s_STATEON);
	CHECK_EQUAL(Qt::Checked, pCheckBox->checkState());
	tf.LeftToRight(guiPropTblYIt, accPropTblYIt);
	CHECK_EQUAL(DBCHECKED, accPropTblYIt.getString(PROPTBLCOL).c_str());

	guiPropTblYIt.setString(GUITBLCOL, CheckBoxTransformer::s_STATEOFF);
	CHECK_EQUAL(Qt::Unchecked, pCheckBox->checkState());
	tf.LeftToRight(guiPropTblYIt, accPropTblYIt);
	CHECK_EQUAL(DBUNCHECKED, accPropTblYIt.getString(PROPTBLCOL).c_str());
}

//------------------------------------------------------------------------------
TEST_FIXTURE(FxCheckBoxGuiCol, CheckBoxTwoState_GuiTbl_EvalOff_RL)
{
	CheckBoxTransformer tf (GUITBLCOL, 
							DBCHECKED, DBUNCHECKED, 
							CheckBoxTransformer::EVAL_STATE_OFF, 
							PROPTBLCOL);

	accPropTblYIt.setString(PROPTBLCOL, DBCHECKED);
	tf.RightToLeft(guiPropTblYIt, accPropTblYIt);
	CHECK_EQUAL(CheckBoxTransformer::s_STATEON, guiPropTblYIt.getString(GUITBLCOL).c_str());
	CHECK_EQUAL(Qt::Checked, pCheckBox->checkState());

	accPropTblYIt.setString(PROPTBLCOL, DBUNCHECKED);
	tf.RightToLeft(guiPropTblYIt, accPropTblYIt);
	CHECK_EQUAL(CheckBoxTransformer::s_STATEOFF, guiPropTblYIt.getString(GUITBLCOL).c_str());
	CHECK_EQUAL(Qt::Unchecked, pCheckBox->checkState());

	accPropTblYIt.setString(PROPTBLCOL, DBWHATEVER);
	tf.RightToLeft(guiPropTblYIt, accPropTblYIt);
	CHECK_EQUAL(CheckBoxTransformer::s_STATEOFF, guiPropTblYIt.getString(GUITBLCOL).c_str());
	CHECK_EQUAL(Qt::Unchecked, pCheckBox->checkState());
}

//------------------------------------------------------------------------------
TEST_FIXTURE(FxCheckBoxGuiCol, CheckBoxTwoState_GuiTbl_EvalOff_LR)
{
	CheckBoxTransformer tf (GUITBLCOL, 
							DBCHECKED, DBUNCHECKED, 
							CheckBoxTransformer::EVAL_STATE_OFF, 
							PROPTBLCOL);

	guiPropTblYIt.setString(GUITBLCOL, CheckBoxTransformer::s_STATEON);
	CHECK_EQUAL(Qt::Checked, pCheckBox->checkState());
	tf.LeftToRight(guiPropTblYIt, accPropTblYIt);
	CHECK_EQUAL(DBCHECKED, accPropTblYIt.getString(PROPTBLCOL).c_str());

	guiPropTblYIt.setString(GUITBLCOL, CheckBoxTransformer::s_STATEOFF);
	CHECK_EQUAL(Qt::Unchecked, pCheckBox->checkState());
	tf.LeftToRight(guiPropTblYIt, accPropTblYIt);
	CHECK_EQUAL(DBUNCHECKED, accPropTblYIt.getString(PROPTBLCOL).c_str());
}

//------------------------------------------------------------------------------
TEST_FIXTURE(FxCheckBox, CheckBoxTriState_EvalOn_RL)
{
	CheckBoxTransformer tf (PROPTBLCOL, 
							DBCHECKED, DBPARTCHECKED, DBUNCHECKED, 
							CheckBoxTransformer::EVAL_STATE_ON);

	accPropTblYIt.setString(PROPTBLCOL, DBCHECKED);
	tf.RightToLeft(guiPropTblYIt, accPropTblYIt);
	CHECK_EQUAL(CheckBoxTransformer::s_STATEON, guiPropTblYIt.getString(PROPTBLCOL).c_str());
	CHECK_EQUAL(Qt::Checked, pCheckBox->checkState());

	accPropTblYIt.setString(PROPTBLCOL, DBUNCHECKED);
	tf.RightToLeft(guiPropTblYIt, accPropTblYIt);
	CHECK_EQUAL(CheckBoxTransformer::s_STATEOFF, guiPropTblYIt.getString(PROPTBLCOL).c_str());
	CHECK_EQUAL(Qt::Unchecked, pCheckBox->checkState());

	accPropTblYIt.setString(PROPTBLCOL, DBPARTCHECKED);
	tf.RightToLeft(guiPropTblYIt, accPropTblYIt);
	CHECK_EQUAL(CheckBoxTransformer::s_STATEPART, guiPropTblYIt.getString(PROPTBLCOL).c_str());
	CHECK_EQUAL(Qt::PartiallyChecked, pCheckBox->checkState());

	accPropTblYIt.setString(PROPTBLCOL, DBWHATEVER);
	tf.RightToLeft(guiPropTblYIt, accPropTblYIt);
	CHECK_EQUAL(CheckBoxTransformer::s_STATEON, guiPropTblYIt.getString(PROPTBLCOL).c_str());
	CHECK_EQUAL(Qt::Checked, pCheckBox->checkState());
}

//------------------------------------------------------------------------------
TEST_FIXTURE(FxCheckBox, CheckBoxTriState_EvalOn_LR)
{
	CheckBoxTransformer tf (PROPTBLCOL, 
							DBCHECKED, DBPARTCHECKED, DBUNCHECKED, 
							CheckBoxTransformer::EVAL_STATE_ON);

	guiPropTblYIt.setString(PROPTBLCOL, CheckBoxTransformer::s_STATEON);
	CHECK_EQUAL(Qt::Checked, pCheckBox->checkState());
	tf.LeftToRight(guiPropTblYIt, accPropTblYIt);
	CHECK_EQUAL(DBCHECKED, accPropTblYIt.getString(PROPTBLCOL).c_str());

	guiPropTblYIt.setString(PROPTBLCOL, CheckBoxTransformer::s_STATEOFF);
	CHECK_EQUAL(Qt::Unchecked, pCheckBox->checkState());
	tf.LeftToRight(guiPropTblYIt, accPropTblYIt);
	CHECK_EQUAL(DBUNCHECKED, accPropTblYIt.getString(PROPTBLCOL).c_str());

	guiPropTblYIt.setString(PROPTBLCOL, CheckBoxTransformer::s_STATEPART);
	CHECK_EQUAL(Qt::PartiallyChecked, pCheckBox->checkState());
	tf.LeftToRight(guiPropTblYIt, accPropTblYIt);
	CHECK_EQUAL(DBPARTCHECKED, accPropTblYIt.getString(PROPTBLCOL).c_str());
}

//------------------------------------------------------------------------------
TEST_FIXTURE(FxCheckBox, CheckBoxTriState_EvalOn_GuiDriven_LR)
{
	CheckBoxTransformer tf (PROPTBLCOL, 
							DBCHECKED, DBPARTCHECKED, DBUNCHECKED, 
							CheckBoxTransformer::EVAL_STATE_ON);

	pCheckBox->setCheckState(Qt::Checked);
	CHECK_EQUAL(CheckBoxTransformer::s_STATEON, guiPropTblYIt.getString(PROPTBLCOL).c_str());
	CHECK_EQUAL(Qt::Checked, pCheckBox->checkState());
	tf.LeftToRight(guiPropTblYIt, accPropTblYIt);
	CHECK_EQUAL(DBCHECKED, accPropTblYIt.getString(PROPTBLCOL).c_str());

	pCheckBox->setCheckState(Qt::Unchecked);
	CHECK_EQUAL(CheckBoxTransformer::s_STATEOFF, guiPropTblYIt.getString(PROPTBLCOL).c_str());
	CHECK_EQUAL(Qt::Unchecked, pCheckBox->checkState());
	tf.LeftToRight(guiPropTblYIt, accPropTblYIt);
	CHECK_EQUAL(DBUNCHECKED, accPropTblYIt.getString(PROPTBLCOL).c_str());

	pCheckBox->setCheckState(Qt::PartiallyChecked);
	CHECK_EQUAL(CheckBoxTransformer::s_STATEPART, guiPropTblYIt.getString(PROPTBLCOL).c_str());
	CHECK_EQUAL(Qt::PartiallyChecked, pCheckBox->checkState());
	tf.LeftToRight(guiPropTblYIt, accPropTblYIt);
	CHECK_EQUAL(DBPARTCHECKED, accPropTblYIt.getString(PROPTBLCOL).c_str());
}

//------------------------------------------------------------------------------
TEST_FIXTURE(FxCheckBox, CheckBoxTriState_EvalOff_RL)
{
	CheckBoxTransformer tf (PROPTBLCOL, 
							DBCHECKED, DBPARTCHECKED, DBUNCHECKED, 
							CheckBoxTransformer::EVAL_STATE_OFF);

	accPropTblYIt.setString(PROPTBLCOL, DBCHECKED);
	tf.RightToLeft(guiPropTblYIt, accPropTblYIt);
	CHECK_EQUAL(CheckBoxTransformer::s_STATEON, guiPropTblYIt.getString(PROPTBLCOL).c_str());
	CHECK_EQUAL(Qt::Checked, pCheckBox->checkState());

	accPropTblYIt.setString(PROPTBLCOL, DBUNCHECKED);
	tf.RightToLeft(guiPropTblYIt, accPropTblYIt);
	CHECK_EQUAL(CheckBoxTransformer::s_STATEOFF, guiPropTblYIt.getString(PROPTBLCOL).c_str());
	CHECK_EQUAL(Qt::Unchecked, pCheckBox->checkState());

	accPropTblYIt.setString(PROPTBLCOL, DBPARTCHECKED);
	tf.RightToLeft(guiPropTblYIt, accPropTblYIt);
	CHECK_EQUAL(CheckBoxTransformer::s_STATEPART, guiPropTblYIt.getString(PROPTBLCOL).c_str());
	CHECK_EQUAL(Qt::PartiallyChecked, pCheckBox->checkState());

	accPropTblYIt.setString(PROPTBLCOL, DBWHATEVER);
	tf.RightToLeft(guiPropTblYIt, accPropTblYIt);
	CHECK_EQUAL(CheckBoxTransformer::s_STATEOFF, guiPropTblYIt.getString(PROPTBLCOL).c_str());
	CHECK_EQUAL(Qt::Unchecked, pCheckBox->checkState());
}

//------------------------------------------------------------------------------
TEST_FIXTURE(FxCheckBox, CheckBoxTriState_EvalOff_LR)
{
	CheckBoxTransformer tf (PROPTBLCOL, 
							DBCHECKED, DBPARTCHECKED, DBUNCHECKED, 
							CheckBoxTransformer::EVAL_STATE_OFF);

	guiPropTblYIt.setString(PROPTBLCOL, CheckBoxTransformer::s_STATEON);
	CHECK_EQUAL(Qt::Checked, pCheckBox->checkState());
	tf.LeftToRight(guiPropTblYIt, accPropTblYIt);
	CHECK_EQUAL(DBCHECKED, accPropTblYIt.getString(PROPTBLCOL).c_str());

	guiPropTblYIt.setString(PROPTBLCOL, CheckBoxTransformer::s_STATEOFF);
	CHECK_EQUAL(Qt::Unchecked, pCheckBox->checkState());
	tf.LeftToRight(guiPropTblYIt, accPropTblYIt);
	CHECK_EQUAL(DBUNCHECKED, accPropTblYIt.getString(PROPTBLCOL).c_str());

	guiPropTblYIt.setString(PROPTBLCOL, CheckBoxTransformer::s_STATEPART);
	CHECK_EQUAL(Qt::PartiallyChecked, pCheckBox->checkState());
	tf.LeftToRight(guiPropTblYIt, accPropTblYIt);
	CHECK_EQUAL(DBPARTCHECKED, accPropTblYIt.getString(PROPTBLCOL).c_str());
}

//------------------------------------------------------------------------------
TEST_FIXTURE(FxCheckBoxGuiCol, CheckBoxTriState_GuiTbl_EvalOn_RL)
{
	CheckBoxTransformer tf (GUITBLCOL, 
							DBCHECKED, DBPARTCHECKED, DBUNCHECKED, 
							CheckBoxTransformer::EVAL_STATE_ON, 
							PROPTBLCOL);

	accPropTblYIt.setString(PROPTBLCOL, DBCHECKED);
	tf.RightToLeft(guiPropTblYIt, accPropTblYIt);
	CHECK_EQUAL(CheckBoxTransformer::s_STATEON, guiPropTblYIt.getString(GUITBLCOL).c_str());
	CHECK_EQUAL(Qt::Checked, pCheckBox->checkState());

	accPropTblYIt.setString(PROPTBLCOL, DBUNCHECKED);
	tf.RightToLeft(guiPropTblYIt, accPropTblYIt);
	CHECK_EQUAL(CheckBoxTransformer::s_STATEOFF, guiPropTblYIt.getString(GUITBLCOL).c_str());
	CHECK_EQUAL(Qt::Unchecked, pCheckBox->checkState());

	accPropTblYIt.setString(PROPTBLCOL, DBPARTCHECKED);
	tf.RightToLeft(guiPropTblYIt, accPropTblYIt);
	CHECK_EQUAL(CheckBoxTransformer::s_STATEPART, guiPropTblYIt.getString(GUITBLCOL).c_str());
	CHECK_EQUAL(Qt::PartiallyChecked, pCheckBox->checkState());

	accPropTblYIt.setString(PROPTBLCOL, DBWHATEVER);
	tf.RightToLeft(guiPropTblYIt, accPropTblYIt);
	CHECK_EQUAL(CheckBoxTransformer::s_STATEON, guiPropTblYIt.getString(GUITBLCOL).c_str());
}

//------------------------------------------------------------------------------
TEST_FIXTURE(FxCheckBoxGuiCol, CheckBoxTriState_GuiTbl_EvalOn_LR)
{
	CheckBoxTransformer tf (GUITBLCOL, 
							DBCHECKED, DBPARTCHECKED, DBUNCHECKED, 
							CheckBoxTransformer::EVAL_STATE_ON, 
							PROPTBLCOL);

	guiPropTblYIt.setString(GUITBLCOL, CheckBoxTransformer::s_STATEON);
	CHECK_EQUAL(Qt::Checked, pCheckBox->checkState());
	tf.LeftToRight(guiPropTblYIt, accPropTblYIt);
	CHECK_EQUAL(DBCHECKED, accPropTblYIt.getString(PROPTBLCOL).c_str());

	guiPropTblYIt.setString(GUITBLCOL, CheckBoxTransformer::s_STATEOFF);
	CHECK_EQUAL(Qt::Unchecked, pCheckBox->checkState());
	tf.LeftToRight(guiPropTblYIt, accPropTblYIt);
	CHECK_EQUAL(DBUNCHECKED, accPropTblYIt.getString(PROPTBLCOL).c_str());

	guiPropTblYIt.setString(GUITBLCOL, CheckBoxTransformer::s_STATEPART);
	CHECK_EQUAL(Qt::PartiallyChecked, pCheckBox->checkState());
	tf.LeftToRight(guiPropTblYIt, accPropTblYIt);
	CHECK_EQUAL(DBPARTCHECKED, accPropTblYIt.getString(PROPTBLCOL).c_str());
}

//------------------------------------------------------------------------------
TEST_FIXTURE(FxCheckBoxGuiCol, CheckBoxTriState_GuiTbl_EvalOff_RL)
{
	CheckBoxTransformer tf (GUITBLCOL, 
							DBCHECKED, DBPARTCHECKED, DBUNCHECKED, 
							CheckBoxTransformer::EVAL_STATE_OFF, 
							PROPTBLCOL);

	accPropTblYIt.setString(PROPTBLCOL, DBCHECKED);
	tf.RightToLeft(guiPropTblYIt, accPropTblYIt);
	CHECK_EQUAL(CheckBoxTransformer::s_STATEON, guiPropTblYIt.getString(GUITBLCOL).c_str());
	CHECK_EQUAL(Qt::Checked, pCheckBox->checkState());

	accPropTblYIt.setString(PROPTBLCOL, DBUNCHECKED);
	tf.RightToLeft(guiPropTblYIt, accPropTblYIt);
	CHECK_EQUAL(CheckBoxTransformer::s_STATEOFF, guiPropTblYIt.getString(GUITBLCOL).c_str());
	CHECK_EQUAL(Qt::Unchecked, pCheckBox->checkState());

	accPropTblYIt.setString(PROPTBLCOL, DBPARTCHECKED);
	tf.RightToLeft(guiPropTblYIt, accPropTblYIt);
	CHECK_EQUAL(CheckBoxTransformer::s_STATEPART, guiPropTblYIt.getString(GUITBLCOL).c_str());
	CHECK_EQUAL(Qt::PartiallyChecked, pCheckBox->checkState());

	accPropTblYIt.setString(PROPTBLCOL, DBWHATEVER);
	tf.RightToLeft(guiPropTblYIt, accPropTblYIt);
	CHECK_EQUAL(CheckBoxTransformer::s_STATEOFF, guiPropTblYIt.getString(GUITBLCOL).c_str());
	CHECK_EQUAL(Qt::Unchecked, pCheckBox->checkState());
}

//------------------------------------------------------------------------------
TEST_FIXTURE(FxCheckBoxGuiCol, CheckBoxTriState_GuiTbl_EvalOff_LR)
{
	CheckBoxTransformer tf (GUITBLCOL, 
							DBCHECKED, DBPARTCHECKED, DBUNCHECKED, 
							CheckBoxTransformer::EVAL_STATE_OFF, 
							PROPTBLCOL);

	guiPropTblYIt.setString(GUITBLCOL, CheckBoxTransformer::s_STATEON);
	CHECK_EQUAL(Qt::Checked, pCheckBox->checkState());
	tf.LeftToRight(guiPropTblYIt, accPropTblYIt);
	CHECK_EQUAL(DBCHECKED, accPropTblYIt.getString(PROPTBLCOL).c_str());

	guiPropTblYIt.setString(GUITBLCOL, CheckBoxTransformer::s_STATEOFF);
	CHECK_EQUAL(Qt::Unchecked, pCheckBox->checkState());
	tf.LeftToRight(guiPropTblYIt, accPropTblYIt);
	CHECK_EQUAL(DBUNCHECKED, accPropTblYIt.getString(PROPTBLCOL).c_str());

	guiPropTblYIt.setString(GUITBLCOL, CheckBoxTransformer::s_STATEPART);
	CHECK_EQUAL(Qt::PartiallyChecked, pCheckBox->checkState());
	tf.LeftToRight(guiPropTblYIt, accPropTblYIt);
	CHECK_EQUAL(DBPARTCHECKED, accPropTblYIt.getString(PROPTBLCOL).c_str());
}

//------------------------------------------------------------------------------
TEST_FIXTURE(FxGroupBox, MatchGroupBox_without_Transformer_RL)
{
	try
	{
		accPropTblYIt.setString(PROPTBLCOL, DBCHECKED);
		matcher.RightToLeft();
		CHECK_EQUAL(CheckBoxTransformer::s_STATEOFF, guiPropTblYIt.getString(PROPTBLCOL).c_str());
		CHECK(!pGroupBox->isChecked());
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
TEST_FIXTURE(FxGroupBox, MatchGroupBox_without_Transformer_LR)
{
	try
	{
		guiPropTblYIt.setString(PROPTBLCOL, CheckBoxTransformer::s_STATEON);
		CHECK(pGroupBox->isChecked());
		matcher.LeftToRight();
		CHECK_THROW(accPropTblYIt.getString(PROPTBLCOL).c_str(), basar::Exception);

		pGroupBox->setChecked(true);
		CHECK_EQUAL(CheckBoxTransformer::s_STATEON, guiPropTblYIt.getString(PROPTBLCOL).c_str());
		CHECK(pGroupBox->isChecked());
		matcher.LeftToRight();
		CHECK_THROW(accPropTblYIt.getString(PROPTBLCOL).c_str(), basar::Exception);
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
TEST_FIXTURE(FxGroupBox, GroupBoxTwoState_EvalOn_RL)
{
	CheckBoxTransformer tf (PROPTBLCOL, 
							DBCHECKED, DBUNCHECKED, 
							CheckBoxTransformer::EVAL_STATE_ON);

	accPropTblYIt.setString(PROPTBLCOL, DBCHECKED);
	tf.RightToLeft(guiPropTblYIt, accPropTblYIt);
	CHECK_EQUAL(CheckBoxTransformer::s_STATEON, guiPropTblYIt.getString(PROPTBLCOL).c_str());
	CHECK(pGroupBox->isChecked());

	accPropTblYIt.setString(PROPTBLCOL, DBUNCHECKED);
	tf.RightToLeft(guiPropTblYIt, accPropTblYIt);
	CHECK_EQUAL(CheckBoxTransformer::s_STATEOFF, guiPropTblYIt.getString(PROPTBLCOL).c_str());
	CHECK(!pGroupBox->isChecked());

	accPropTblYIt.setString(PROPTBLCOL, DBWHATEVER);
	tf.RightToLeft(guiPropTblYIt, accPropTblYIt);
	CHECK_EQUAL(CheckBoxTransformer::s_STATEON, guiPropTblYIt.getString(PROPTBLCOL).c_str());
	CHECK(pGroupBox->isChecked());
}

//------------------------------------------------------------------------------
TEST_FIXTURE(FxGroupBox, GroupBoxTwoState_EvalOn_LR)
{
	CheckBoxTransformer tf (PROPTBLCOL, 
							DBCHECKED, DBUNCHECKED, 
							CheckBoxTransformer::EVAL_STATE_ON);

	guiPropTblYIt.setString(PROPTBLCOL, CheckBoxTransformer::s_STATEON);
	CHECK(pGroupBox->isChecked());
	tf.LeftToRight(guiPropTblYIt, accPropTblYIt);
	CHECK_EQUAL(DBCHECKED, accPropTblYIt.getString(PROPTBLCOL).c_str());

	guiPropTblYIt.setString(PROPTBLCOL, CheckBoxTransformer::s_STATEOFF);
	CHECK(!pGroupBox->isChecked());
	tf.LeftToRight(guiPropTblYIt, accPropTblYIt);
	CHECK_EQUAL(DBUNCHECKED, accPropTblYIt.getString(PROPTBLCOL).c_str());

	pGroupBox->setChecked(true);
	CHECK_EQUAL(CheckBoxTransformer::s_STATEON, guiPropTblYIt.getString(PROPTBLCOL).c_str());
	CHECK(pGroupBox->isChecked());
	tf.LeftToRight(guiPropTblYIt, accPropTblYIt);
	CHECK_EQUAL(DBCHECKED, accPropTblYIt.getString(PROPTBLCOL).c_str());
}

//------------------------------------------------------------------------------
TEST_FIXTURE(FxGroupBox, GroupBoxTwoState_EvalOff_RL)
{
	CheckBoxTransformer tf (PROPTBLCOL, 
							DBCHECKED, DBUNCHECKED, 
							CheckBoxTransformer::EVAL_STATE_OFF);

	accPropTblYIt.setString(PROPTBLCOL, DBCHECKED);
	tf.RightToLeft(guiPropTblYIt, accPropTblYIt);
	CHECK_EQUAL(CheckBoxTransformer::s_STATEON, guiPropTblYIt.getString(PROPTBLCOL).c_str());
	CHECK(pGroupBox->isChecked());

	accPropTblYIt.setString(PROPTBLCOL, DBUNCHECKED);
	tf.RightToLeft(guiPropTblYIt, accPropTblYIt);
	CHECK_EQUAL(CheckBoxTransformer::s_STATEOFF, guiPropTblYIt.getString(PROPTBLCOL).c_str());
	CHECK(!pGroupBox->isChecked());

	accPropTblYIt.setString(PROPTBLCOL, DBWHATEVER);
	tf.RightToLeft(guiPropTblYIt, accPropTblYIt);
	CHECK_EQUAL(CheckBoxTransformer::s_STATEOFF, guiPropTblYIt.getString(PROPTBLCOL).c_str());
	CHECK(!pGroupBox->isChecked());
}

//------------------------------------------------------------------------------
TEST_FIXTURE(FxGroupBox, GroupBoxTwoState_EvalOff_LR)
{
	CheckBoxTransformer tf (PROPTBLCOL, 
							DBCHECKED, DBUNCHECKED, 
							CheckBoxTransformer::EVAL_STATE_OFF);

	guiPropTblYIt.setString(PROPTBLCOL, CheckBoxTransformer::s_STATEON);
	CHECK(pGroupBox->isChecked());
	tf.LeftToRight(guiPropTblYIt, accPropTblYIt);
	CHECK_EQUAL(DBCHECKED, accPropTblYIt.getString(PROPTBLCOL).c_str());

	guiPropTblYIt.setString(PROPTBLCOL, CheckBoxTransformer::s_STATEOFF);
	CHECK(!pGroupBox->isChecked());
	tf.LeftToRight(guiPropTblYIt, accPropTblYIt);
	CHECK_EQUAL(DBUNCHECKED, accPropTblYIt.getString(PROPTBLCOL).c_str());

	pGroupBox->setChecked(true);
	CHECK_EQUAL(CheckBoxTransformer::s_STATEON, guiPropTblYIt.getString(PROPTBLCOL).c_str());
	CHECK(pGroupBox->isChecked());
	tf.LeftToRight(guiPropTblYIt, accPropTblYIt);
	CHECK_EQUAL(DBCHECKED, accPropTblYIt.getString(PROPTBLCOL).c_str());
}

//------------------------------------------------------------------------------
TEST_FIXTURE(FxGroupBoxGuiCol, GroupBoxTwoState_GuiTbl_EvalOn_RL)
{
	CheckBoxTransformer tf (GUITBLCOL, 
							DBCHECKED, DBUNCHECKED, 
							CheckBoxTransformer::EVAL_STATE_ON, 
							PROPTBLCOL);

	accPropTblYIt.setString(PROPTBLCOL, DBCHECKED);
	tf.RightToLeft(guiPropTblYIt, accPropTblYIt);
	CHECK_EQUAL(CheckBoxTransformer::s_STATEON, guiPropTblYIt.getString(GUITBLCOL).c_str());
	CHECK(pGroupBox->isChecked());

	accPropTblYIt.setString(PROPTBLCOL, DBUNCHECKED);
	tf.RightToLeft(guiPropTblYIt, accPropTblYIt);
	CHECK_EQUAL(CheckBoxTransformer::s_STATEOFF, guiPropTblYIt.getString(GUITBLCOL).c_str());
	CHECK(!pGroupBox->isChecked());

	accPropTblYIt.setString(PROPTBLCOL, DBWHATEVER);
	tf.RightToLeft(guiPropTblYIt, accPropTblYIt);
	CHECK_EQUAL(CheckBoxTransformer::s_STATEON, guiPropTblYIt.getString(GUITBLCOL).c_str());
	CHECK(pGroupBox->isChecked());
}

//------------------------------------------------------------------------------
TEST_FIXTURE(FxGroupBoxGuiCol, GroupBoxTwoState_GuiTbl_EvalOn_LR)
{
	CheckBoxTransformer tf (GUITBLCOL, 
							DBCHECKED, DBUNCHECKED, 
							CheckBoxTransformer::EVAL_STATE_ON, 
							PROPTBLCOL);

	guiPropTblYIt.setString(GUITBLCOL, CheckBoxTransformer::s_STATEON);
	CHECK(pGroupBox->isChecked());
	tf.LeftToRight(guiPropTblYIt, accPropTblYIt);
	CHECK_EQUAL(DBCHECKED, accPropTblYIt.getString(PROPTBLCOL).c_str());

	guiPropTblYIt.setString(GUITBLCOL, CheckBoxTransformer::s_STATEOFF);
	CHECK(!pGroupBox->isChecked());
	tf.LeftToRight(guiPropTblYIt, accPropTblYIt);
	CHECK_EQUAL(DBUNCHECKED, accPropTblYIt.getString(PROPTBLCOL).c_str());

	pGroupBox->setChecked(true);
	CHECK_EQUAL(CheckBoxTransformer::s_STATEON, guiPropTblYIt.getString(GUITBLCOL).c_str());
	CHECK(pGroupBox->isChecked());
	tf.LeftToRight(guiPropTblYIt, accPropTblYIt);
	CHECK_EQUAL(DBCHECKED, accPropTblYIt.getString(PROPTBLCOL).c_str());
}

//------------------------------------------------------------------------------
TEST_FIXTURE(FxGroupBoxWithLabel, GroupBoxWithLabel_RL)
{
	CheckBoxTransformer tf (PROPTBLCOL, 
							DBCHECKED, DBUNCHECKED, 
							CheckBoxTransformer::EVAL_STATE_ON);

	accPropTblYIt.setString(PROPTBLCOL, DBCHECKED);
	tf.RightToLeft(guiPropTblYIt, accPropTblYIt);
	CHECK_EQUAL(CheckBoxTransformer::s_STATEON, guiPropTblYIt.getString(PROPTBLCOL).c_str());
	CHECK(pGroupBox->isChecked());

	accPropTblYIt.setString(PROPTBLCOL, DBUNCHECKED);
	tf.RightToLeft(guiPropTblYIt, accPropTblYIt);
	CHECK_EQUAL(CheckBoxTransformer::s_STATEOFF, guiPropTblYIt.getString(PROPTBLCOL).c_str());
	CHECK(!pGroupBox->isChecked());

	accPropTblYIt.setString(PROPTBLCOL, DBWHATEVER);
	tf.RightToLeft(guiPropTblYIt, accPropTblYIt);
	CHECK_EQUAL(CheckBoxTransformer::s_STATEON, guiPropTblYIt.getString(PROPTBLCOL).c_str());
	CHECK(pGroupBox->isChecked());
}

//------------------------------------------------------------------------------
TEST_FIXTURE(FxGroupBoxWithLabel, GroupBoxWithLabel_LR)
{
	CheckBoxTransformer tf (PROPTBLCOL, 
							DBCHECKED, DBUNCHECKED, 
							CheckBoxTransformer::EVAL_STATE_ON);

	guiPropTblYIt.setString(PROPTBLCOL, CheckBoxTransformer::s_STATEON);
	CHECK(pGroupBox->isChecked());
	tf.LeftToRight(guiPropTblYIt, accPropTblYIt);
	CHECK_EQUAL(DBCHECKED, accPropTblYIt.getString(PROPTBLCOL).c_str());

	guiPropTblYIt.setString(PROPTBLCOL, CheckBoxTransformer::s_STATEOFF);
	CHECK(!pGroupBox->isChecked());
	tf.LeftToRight(guiPropTblYIt, accPropTblYIt);
	CHECK_EQUAL(DBUNCHECKED, accPropTblYIt.getString(PROPTBLCOL).c_str());

	pGroupBox->setChecked(true);
	CHECK_EQUAL(CheckBoxTransformer::s_STATEON, guiPropTblYIt.getString(PROPTBLCOL).c_str());
	CHECK(pGroupBox->isChecked());
	tf.LeftToRight(guiPropTblYIt, accPropTblYIt);
	CHECK_EQUAL(DBCHECKED, accPropTblYIt.getString(PROPTBLCOL).c_str());
}

//------------------------------------------------------------------------------
TEST_FIXTURE(FxGroupBoxWithCheckBoxes, GroupBoxWithCheckBoxes_RL)
{
	CheckBoxTransformer tf (PROPTBLCOL, 
							DBCHECKED, DBUNCHECKED, 
							CheckBoxTransformer::EVAL_STATE_ON);

	accPropTblYIt.setString(PROPTBLCOL, DBCHECKED);
	tf.RightToLeft(guiPropTblYIt, accPropTblYIt);
	CHECK_EQUAL(CheckBoxTransformer::s_STATEON, guiPropTblYIt.getString(PROPTBLCOL).c_str());
	CHECK(pGroupBox->isChecked());

	accPropTblYIt.setString(PROPTBLCOL, DBUNCHECKED);
	tf.RightToLeft(guiPropTblYIt, accPropTblYIt);
	CHECK_EQUAL(CheckBoxTransformer::s_STATEOFF, guiPropTblYIt.getString(PROPTBLCOL).c_str());
	CHECK(!pGroupBox->isChecked());

	accPropTblYIt.setString(PROPTBLCOL, DBWHATEVER);
	tf.RightToLeft(guiPropTblYIt, accPropTblYIt);
	CHECK_EQUAL(CheckBoxTransformer::s_STATEON, guiPropTblYIt.getString(PROPTBLCOL).c_str());
	CHECK(pGroupBox->isChecked());
}

//------------------------------------------------------------------------------
TEST_FIXTURE(FxGroupBoxWithCheckBoxes, GroupBoxWithCheckBoxes_LR)
{
	CheckBoxTransformer tf (PROPTBLCOL, 
							DBCHECKED, DBUNCHECKED, 
							CheckBoxTransformer::EVAL_STATE_ON);

	guiPropTblYIt.setString(PROPTBLCOL, CheckBoxTransformer::s_STATEON);
	CHECK(pGroupBox->isChecked());
	tf.LeftToRight(guiPropTblYIt, accPropTblYIt);
	CHECK_EQUAL(DBCHECKED, accPropTblYIt.getString(PROPTBLCOL).c_str());

	guiPropTblYIt.setString(PROPTBLCOL, CheckBoxTransformer::s_STATEOFF);
	CHECK(!pGroupBox->isChecked());
	tf.LeftToRight(guiPropTblYIt, accPropTblYIt);
	CHECK_EQUAL(DBUNCHECKED, accPropTblYIt.getString(PROPTBLCOL).c_str());

	pGroupBox->setChecked(true);
	CHECK_EQUAL(CheckBoxTransformer::s_STATEON, guiPropTblYIt.getString(PROPTBLCOL).c_str());
	CHECK(pGroupBox->isChecked());
	tf.LeftToRight(guiPropTblYIt, accPropTblYIt);
	CHECK_EQUAL(DBCHECKED, accPropTblYIt.getString(PROPTBLCOL).c_str());
}

//------------------------------------------------------------------------------
TEST_FIXTURE(FxGroupBoxWithCheckBoxes, GroupBoxWithTransformedCheckBox_RL)
{
	ConstString CB_NAME = CHECKBOXNAME[2];
	QCheckBox * pCB     = pCheckBox   [2];

	try
	{
		CheckBoxTransformer tfCB(CB_NAME, 
								 DBCHECKED, DBUNCHECKED, 
								 CheckBoxTransformer::EVAL_STATE_ON,
								 PROPTBLCOL);

		CheckBoxTransformer tfGB(PROPTBLCOL, 
								 DBCHECKED, DBUNCHECKED, 
								 CheckBoxTransformer::EVAL_STATE_ON);

		matcher.push_back(tfCB);
		matcher.push_back(tfGB);

		accPropTblYIt.setString(PROPTBLCOL, DBCHECKED);
		matcher.RightToLeft();
		CHECK_EQUAL(CheckBoxTransformer::s_STATEON, guiPropTblYIt.getString(PROPTBLCOL).c_str());
		CHECK(pGroupBox->isChecked());
		CHECK_EQUAL(CheckBoxTransformer::s_STATEON, guiPropTblYIt.getString(CB_NAME).c_str());
		CHECK_EQUAL(Qt::Checked, pCB->checkState());

		accPropTblYIt.setString(PROPTBLCOL, DBUNCHECKED);
		matcher.RightToLeft();
		CHECK_EQUAL(CheckBoxTransformer::s_STATEOFF, guiPropTblYIt.getString(PROPTBLCOL).c_str());
		CHECK(!pGroupBox->isChecked());
		CHECK_EQUAL(CheckBoxTransformer::s_STATEOFF, guiPropTblYIt.getString(CB_NAME).c_str());
		CHECK_EQUAL(Qt::Unchecked, pCB->checkState());

		accPropTblYIt.setString(PROPTBLCOL, DBWHATEVER);
		matcher.RightToLeft();
		CHECK_EQUAL(CheckBoxTransformer::s_STATEON, guiPropTblYIt.getString(PROPTBLCOL).c_str());
		CHECK(pGroupBox->isChecked());
		CHECK_EQUAL(CheckBoxTransformer::s_STATEON, guiPropTblYIt.getString(CB_NAME).c_str());
		CHECK_EQUAL(Qt::Checked, pCB->checkState());
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
TEST_FIXTURE(FxGroupBoxWithCheckBoxes, GroupBoxWithTransformedCheckBox_LR)
{
	ConstString CB_NAME = CHECKBOXNAME[2];
	QCheckBox * pCB     = pCheckBox   [2];

	try
	{
		CheckBoxTransformer tfCB(CB_NAME, 
								 DBCHECKED, DBUNCHECKED, 
								 CheckBoxTransformer::EVAL_STATE_ON,
								 PROPTBLCOL2);

		CheckBoxTransformer tfGB(PROPTBLCOL, 
								 DBCHECKED, DBUNCHECKED, 
								 CheckBoxTransformer::EVAL_STATE_ON);

		matcher.push_back(tfCB);
		matcher.push_back(tfGB);

		{
			guiPropTblYIt.setString(PROPTBLCOL, CheckBoxTransformer::s_STATEON);
			CHECK(pGroupBox->isChecked());
			matcher.LeftToRight();
			CHECK_EQUAL(DBCHECKED, accPropTblYIt.getString(PROPTBLCOL).c_str());

			guiPropTblYIt.setString(PROPTBLCOL, CheckBoxTransformer::s_STATEOFF);
			CHECK(!pGroupBox->isChecked());
			matcher.LeftToRight();
			CHECK_EQUAL(DBUNCHECKED, accPropTblYIt.getString(PROPTBLCOL).c_str());

			pGroupBox->setChecked(true);
			CHECK_EQUAL(CheckBoxTransformer::s_STATEON, guiPropTblYIt.getString(PROPTBLCOL).c_str());
			CHECK(pGroupBox->isChecked());
			matcher.LeftToRight();
			CHECK_EQUAL(DBCHECKED, accPropTblYIt.getString(PROPTBLCOL).c_str());
		}

		{
			guiPropTblYIt.setString(CB_NAME, CheckBoxTransformer::s_STATEOFF);
			CHECK_EQUAL(Qt::Unchecked, pCB->checkState());
			matcher.LeftToRight();
			CHECK_EQUAL(DBUNCHECKED, accPropTblYIt.getString(PROPTBLCOL2).c_str());

			guiPropTblYIt.setString(CB_NAME, CheckBoxTransformer::s_STATEON);
			CHECK_EQUAL(Qt::Checked, pCB->checkState());
			matcher.LeftToRight();
			CHECK_EQUAL(DBCHECKED, accPropTblYIt.getString(PROPTBLCOL2).c_str());

			pCB->setCheckState(Qt::Unchecked);
			CHECK_EQUAL(CheckBoxTransformer::s_STATEOFF, guiPropTblYIt.getString(CB_NAME).c_str());
			CHECK_EQUAL(Qt::Unchecked, pCB->checkState());
			matcher.LeftToRight();
			CHECK_EQUAL(DBUNCHECKED, accPropTblYIt.getString(PROPTBLCOL2).c_str());
		}
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

	pCB->setCheckState(Qt::Checked);	// ~FxGroupBoxWithCheckBoxes() check
}

//------------------------------------------------------------------------------
}	// SUITE
