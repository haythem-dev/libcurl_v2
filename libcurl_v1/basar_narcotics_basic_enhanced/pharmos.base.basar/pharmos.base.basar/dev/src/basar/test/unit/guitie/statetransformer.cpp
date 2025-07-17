/* 
 * @file                                            
 * @author Michael Eichenlaub
 * @date   2010-04-19
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
using basar::FOR_CLEAN;

using basar::cmnutil::ParameterList;
using basar::cmnutil::ParamPair;
using basar::cmnutil::ParamName;

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
using tie::Transformer;
using tie::StateTransformer;

//------------------------------------------------------------------------------
SUITE(TransformState)
{
	//--------------------------------------------------------------------------
	#define DEF_PROPTBLCOL0			 "PropTblCol0"
	#define DEF_PROPTBLCOL1			 "PropTblCol1"

	//--------------------------------------------------------------------------
	static ConstString PROPTBLCOL0   = DEF_PROPTBLCOL0;
	static ConstString PROPTBLCOL1   = DEF_PROPTBLCOL1;

	static ConstString GUITBLCOL0     = "GuiTblCol0";
	static ConstString GUITBLCOL1     = "GuiTblCol1";

	static ConstString ACCDEF        = "TransformStateAccDefName";
	static ConstString ACCINST       = "TransformStateAccInstName";

	//--------------------------------------------------------------------------
	BEGIN_ACCESSOR_DEFINITION(ACCDEF)
		PROPERTY_DESCRIPTION_LIST("string dummy0;"
								  "int32  dummy1;"
								  "string " DEF_PROPTBLCOL0 ";"
								  "int32  " DEF_PROPTBLCOL1 ";"
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
		QLabel                    * pLabel[2];
	};

	//--------------------------------------------------------------------------
	class FxTransform : public FxTransformBase
	{
	public:

		FxTransform()
		{
			pLabel[0]->setObjectName(PROPTBLCOL0);
			pLabel[1]->setObjectName(PROPTBLCOL1);

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

			matcher		  = tie::Manager::getInstance().createMatcher(pWidget, accPropTbl);

			guiPropTbl    = matcher.getLeft();
			guiPropTblYIt = guiPropTbl.begin();
		}
	};

//------------------------------------------------------------------------------
TEST_FIXTURE(FxTransform, CtorParamListMap_RL)
{
	try
	{
		ConstString DEF_ACCVAL = "x";
		ConstString DEF_GUIVAL = "UV XYZ";

		ConstString ACCVAL0 = "a";
		ConstString GUIVAL0 = "ABC";

		Transformer::ParamList pl;
		pl.push_back(ACCVAL0, GUIVAL0);

		StateTransformer tf (pl, 
			                 PROPTBLCOL0, 
							 DEF_ACCVAL, 
							 DEF_GUIVAL); 

		accPropTblYIt.setString(PROPTBLCOL0, ACCVAL0);
		tf.RightToLeft(guiPropTblYIt, accPropTblYIt);
		CHECK_EQUAL(GUIVAL0, guiPropTblYIt.getString(PROPTBLCOL0).c_str());

		accPropTblYIt.setString(PROPTBLCOL0, "else");
		tf.RightToLeft(guiPropTblYIt, accPropTblYIt);
		CHECK_EQUAL(DEF_GUIVAL, guiPropTblYIt.getString(PROPTBLCOL0).c_str());
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
TEST_FIXTURE(FxTransform, CtorParamListMap_LR)
{
	try
	{
		ConstString DEF_ACCVAL = "x";
		ConstString DEF_GUIVAL = "UV XYZ";

		ConstString ACCVAL0 = "a";
		ConstString GUIVAL0 = "ABC";

		Transformer::ParamList pl;
		pl.push_back(ACCVAL0, GUIVAL0);

		StateTransformer tf (pl, 
			                 PROPTBLCOL0, 
							 DEF_ACCVAL, 
							 DEF_GUIVAL); 

		guiPropTblYIt.setString(PROPTBLCOL0, GUIVAL0);
		tf.LeftToRight(guiPropTblYIt, accPropTblYIt);
		CHECK_EQUAL(ACCVAL0, accPropTblYIt.getString(PROPTBLCOL0).c_str());

		guiPropTblYIt.setString(PROPTBLCOL0, "else");
		tf.LeftToRight(guiPropTblYIt, accPropTblYIt);
		CHECK_EQUAL(DEF_ACCVAL, accPropTblYIt.getString(PROPTBLCOL0).c_str());
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
TEST_FIXTURE(FxTransform, CtorStringMap_IntCol_RL)
{
	try
	{
		ConstString DEF_ACCVAL = "0";
		ConstString DEF_GUIVAL = "";

		const Int32 ACCVAL0 =  1;
		ConstString GUIVAL0 = "X";

		StateTransformer tf (ParamPair("1 = X;"), 
			                 PROPTBLCOL1, 
							 DEF_ACCVAL, 
							 DEF_GUIVAL); 

		accPropTblYIt.setInt32(PROPTBLCOL1, ACCVAL0);
		tf.RightToLeft(guiPropTblYIt, accPropTblYIt);
		CHECK_EQUAL(GUIVAL0, guiPropTblYIt.getString(PROPTBLCOL1).c_str());

		accPropTblYIt.setInt32(PROPTBLCOL1, -1);
		tf.RightToLeft(guiPropTblYIt, accPropTblYIt);
		CHECK_EQUAL(DEF_GUIVAL, guiPropTblYIt.getString(PROPTBLCOL1).c_str());
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
TEST_FIXTURE(FxTransform, CtorStringMap_IntCol_LR)
{
	try
	{
		ConstString DEF_ACCVAL = "0";
		ConstString DEF_GUIVAL = "";

		const Int32 ACCVAL0 =  1;
		ConstString GUIVAL0 = "X";

		StateTransformer tf (ParamPair("1 = X;"), 
			                 PROPTBLCOL1, 
							 DEF_ACCVAL, 
							 DEF_GUIVAL); 

		guiPropTblYIt.setString(PROPTBLCOL1, GUIVAL0);
		tf.LeftToRight(guiPropTblYIt, accPropTblYIt);
		CHECK_EQUAL(ACCVAL0, accPropTblYIt.getInt32(PROPTBLCOL1));

		guiPropTblYIt.setString(PROPTBLCOL1, "else");
		tf.LeftToRight(guiPropTblYIt, accPropTblYIt);
		CHECK_EQUAL(VarString(DEF_ACCVAL).stoi(), accPropTblYIt.getInt32(PROPTBLCOL1));
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
TEST_FIXTURE(FxTransformGuiCol, CtorParamListMap_RL_GuiCol)
{
	try
	{
		ConstString DEF_ACCVAL = "x";
		ConstString DEF_GUIVAL = "UV XYZ";

		ConstString ACCVAL0 = "a";
		ConstString GUIVAL0 = "ABC";

		Transformer::ParamList pl;
		pl.push_back(ACCVAL0, GUIVAL0);

		StateTransformer tf (pl, 
			                 PROPTBLCOL0, 
			                 GUITBLCOL0, 
							 DEF_ACCVAL, 
							 DEF_GUIVAL); 

		accPropTblYIt.setString(PROPTBLCOL0, ACCVAL0);
		tf.RightToLeft(guiPropTblYIt, accPropTblYIt);
		CHECK_EQUAL(GUIVAL0, guiPropTblYIt.getString(GUITBLCOL0).c_str());

		accPropTblYIt.setString(PROPTBLCOL0, "else");
		tf.RightToLeft(guiPropTblYIt, accPropTblYIt);
		CHECK_EQUAL(DEF_GUIVAL, guiPropTblYIt.getString(GUITBLCOL0).c_str());
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
TEST_FIXTURE(FxTransformGuiCol, CtorParamListMap_LR_GuiCol)
{
	try
	{
		ConstString DEF_ACCVAL = "x";
		ConstString DEF_GUIVAL = "UV XYZ";

		ConstString ACCVAL0 = "a";
		ConstString GUIVAL0 = "ABC";

		Transformer::ParamList pl;
		pl.push_back(ACCVAL0, GUIVAL0);

		StateTransformer tf (pl, 
			                 PROPTBLCOL0, 
			                 GUITBLCOL0, 
							 DEF_ACCVAL, 
							 DEF_GUIVAL); 

		guiPropTblYIt.setString(GUITBLCOL0, GUIVAL0);
		tf.LeftToRight(guiPropTblYIt, accPropTblYIt);
		CHECK_EQUAL(ACCVAL0, accPropTblYIt.getString(PROPTBLCOL0).c_str());

		guiPropTblYIt.setString(GUITBLCOL0, "else");
		tf.LeftToRight(guiPropTblYIt, accPropTblYIt);
		CHECK_EQUAL(DEF_ACCVAL, accPropTblYIt.getString(PROPTBLCOL0).c_str());
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
TEST_FIXTURE(FxTransformGuiCol, CtorStringMap_IntCol_RL_GuiCol)
{
	try
	{
		ConstString DEF_ACCVAL = "0";
		ConstString DEF_GUIVAL = "";

		const Int32 ACCVAL0 =  1;
		ConstString GUIVAL0 = "X";

		StateTransformer tf (ParamPair("1 = X;"), 
			                 PROPTBLCOL1, 
			                 GUITBLCOL1, 
							 DEF_ACCVAL, 
							 DEF_GUIVAL); 

		accPropTblYIt.setInt32(PROPTBLCOL1, ACCVAL0);
		tf.RightToLeft(guiPropTblYIt, accPropTblYIt);
		CHECK_EQUAL(GUIVAL0, guiPropTblYIt.getString(GUITBLCOL1).c_str());

		accPropTblYIt.setInt32(PROPTBLCOL1, -1);
		tf.RightToLeft(guiPropTblYIt, accPropTblYIt);
		CHECK_EQUAL(DEF_GUIVAL, guiPropTblYIt.getString(GUITBLCOL1).c_str());
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
TEST_FIXTURE(FxTransformGuiCol, CtorStringMap_IntCol_LR_GuiCol)
{
	try
	{
		ConstString DEF_ACCVAL = "0";
		ConstString DEF_GUIVAL = "";

		const Int32 ACCVAL0 =  1;
		ConstString GUIVAL0 = "X";

		StateTransformer tf (ParamPair("1 = X;"), 
			                 PROPTBLCOL1, 
			                 GUITBLCOL1, 
							 DEF_ACCVAL, 
							 DEF_GUIVAL); 

		guiPropTblYIt.setString(GUITBLCOL1, GUIVAL0);
		tf.LeftToRight(guiPropTblYIt, accPropTblYIt);
		CHECK_EQUAL(ACCVAL0, accPropTblYIt.getInt32(PROPTBLCOL1));

		guiPropTblYIt.setString(GUITBLCOL1, "else");
		tf.LeftToRight(guiPropTblYIt, accPropTblYIt);
		CHECK_EQUAL(VarString(DEF_ACCVAL).stoi(), accPropTblYIt.getInt32(PROPTBLCOL1));
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
TEST_FIXTURE(FxTransformGuiCol, CtorParamListMap_RL_GuiCol_MultiMapping)
{
	try
	{
		ConstString DEF_ACCVAL = "!";
		ConstString DEF_GUIVAL = "ERROR";

		ConstString ACCVAL0 = "0";
		ConstString GUIVAL0 = "not defined";
		ConstString ACCVAL1 = "1";
		ConstString GUIVAL1 = "defined";
		ConstString ACCVAL2 = "2";
		ConstString GUIVAL2 = "X";
		ConstString ACCVAL3 = "";
		ConstString GUIVAL3 = "";

		Transformer::ParamList pl;
		pl.push_back(ACCVAL0, GUIVAL0);
		pl.push_back(ACCVAL1, GUIVAL1);
		pl.push_back(ACCVAL2, GUIVAL2);
		pl.push_back(ACCVAL3, GUIVAL3);

		StateTransformer tf (pl, 
			                 PROPTBLCOL0, 
			                 GUITBLCOL0, 
							 DEF_ACCVAL, 
							 DEF_GUIVAL); 

		accPropTblYIt.setString(PROPTBLCOL0, ACCVAL0);
		tf.RightToLeft(guiPropTblYIt, accPropTblYIt);
		CHECK_EQUAL(GUIVAL0, guiPropTblYIt.getString(GUITBLCOL0).c_str());

		accPropTblYIt.setString(PROPTBLCOL0, ACCVAL1);
		tf.RightToLeft(guiPropTblYIt, accPropTblYIt);
		CHECK_EQUAL(GUIVAL1, guiPropTblYIt.getString(GUITBLCOL0).c_str());

		accPropTblYIt.setString(PROPTBLCOL0, ACCVAL2);
		tf.RightToLeft(guiPropTblYIt, accPropTblYIt);
		CHECK_EQUAL(GUIVAL2, guiPropTblYIt.getString(GUITBLCOL0).c_str());

		accPropTblYIt.setString(PROPTBLCOL0, ACCVAL3);
		tf.RightToLeft(guiPropTblYIt, accPropTblYIt);
		CHECK_EQUAL(GUIVAL3, guiPropTblYIt.getString(GUITBLCOL0).c_str());

		accPropTblYIt.setString(PROPTBLCOL0, "else");
		tf.RightToLeft(guiPropTblYIt, accPropTblYIt);
		CHECK_EQUAL(DEF_GUIVAL, guiPropTblYIt.getString(GUITBLCOL0).c_str());
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
TEST_FIXTURE(FxTransformGuiCol, CtorParamListMap_LR_GuiCol_MultiMapping)
{
	try
	{
		ConstString DEF_ACCVAL = "!";
		ConstString DEF_GUIVAL = "ERROR";

		ConstString ACCVAL0 = "0";
		ConstString GUIVAL0 = "not defined";
		ConstString ACCVAL1 = "1";
		ConstString GUIVAL1 = "defined";
		ConstString ACCVAL2 = "2";
		ConstString GUIVAL2 = "X";
		ConstString ACCVAL3 = "";
		ConstString GUIVAL3 = "";

		Transformer::ParamList pl;
		pl.push_back(ACCVAL0, GUIVAL0);
		pl.push_back(ACCVAL1, GUIVAL1);
		pl.push_back(ACCVAL2, GUIVAL2);
		pl.push_back(ACCVAL3, GUIVAL3);

		StateTransformer tf (pl, 
			                 PROPTBLCOL0, 
			                 GUITBLCOL0, 
							 DEF_ACCVAL, 
							 DEF_GUIVAL); 

		guiPropTblYIt.setString(GUITBLCOL0, GUIVAL0);
		tf.LeftToRight(guiPropTblYIt, accPropTblYIt);
		CHECK_EQUAL(ACCVAL0, accPropTblYIt.getString(PROPTBLCOL0).c_str());

		guiPropTblYIt.setString(GUITBLCOL0, GUIVAL1);
		tf.LeftToRight(guiPropTblYIt, accPropTblYIt);
		CHECK_EQUAL(ACCVAL1, accPropTblYIt.getString(PROPTBLCOL0).c_str());

		guiPropTblYIt.setString(GUITBLCOL0, GUIVAL2);
		tf.LeftToRight(guiPropTblYIt, accPropTblYIt);
		CHECK_EQUAL(ACCVAL2, accPropTblYIt.getString(PROPTBLCOL0).c_str());

		guiPropTblYIt.setString(GUITBLCOL0, GUIVAL3);
		tf.LeftToRight(guiPropTblYIt, accPropTblYIt);
		CHECK_EQUAL(ACCVAL3, accPropTblYIt.getString(PROPTBLCOL0).c_str());

		guiPropTblYIt.setString(GUITBLCOL0, "else");
		tf.LeftToRight(guiPropTblYIt, accPropTblYIt);
		CHECK_EQUAL(DEF_ACCVAL, accPropTblYIt.getString(PROPTBLCOL0).c_str());
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
