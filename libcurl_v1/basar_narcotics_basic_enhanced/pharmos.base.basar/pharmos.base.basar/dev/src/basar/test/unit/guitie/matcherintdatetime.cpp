/* 
 * @file                                            
 * @author Michael Eichenlaub
 * @date   2010-04-22
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
using basar::Int32;
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

//------------------------------------------------------------------------------
SUITE(MatcherIntDateTime)
{
	// -------------------------------------------------------------------------
	#define DEF_PROPTBLCOL0				"PropTblCol0"
	#define DEF_PROPTBLCOL1				"PropTblCol1"

	//--------------------------------------------------------------------------
	static ConstString PROPTBLCOL0   = DEF_PROPTBLCOL0;
	static ConstString PROPTBLCOL1   = DEF_PROPTBLCOL1;
	static ConstString GUITBLCOL0    = "GuiTblCol0";
	static ConstString GUITBLCOL1    = "GuiTblCol1";

	static ConstString ACCDEF        = "GuiTieTest_MatcherIntDateTimeAccDefName" ;
	static ConstString ACCINST       = "GuiTieTest_MatcherIntDateTimeAccInstName";

	static const Int32 INTDT         = 20100329;
	static ConstString INTDT_STR     = "29.03.2010";

	static const Int32 INTTM         = 11259345;
	static ConstString INTTM_STR     = "01:12:59.345";

	//--------------------------------------------------------------------------
	BEGIN_ACCESSOR_DEFINITION(ACCDEF)
		PROPERTY_DESCRIPTION_LIST("string    dummy0;"
								  "int32     dummy1;"
								  "intdate " DEF_PROPTBLCOL0 ";"
								  "inttime " DEF_PROPTBLCOL1 ";"
								  "date   dummy2;")
	END_ACCESSOR_DEFINITION

	//--------------------------------------------------------------------------
	class FxMatcherBase
	{
	public:
		MatcherRef                      matcher; 

		AccessorPropertyTableRef	    accPropTbl;
		AccessorPropertyTable_YIterator accPropTblYIt;

		GuiPropertyTableRef             guiPropTbl;
		GuiPropertyTable_YIterator      guiPropTblYIt;

		FxMatcherBase()
		{
			accInst       = aspect::Manager::getInstance().createAccessorInstance(ACCINST,
																		          ACCDEF,
																				  ConnectionRef(), 
																				  FULL_CACHING);
			accPropTbl    = accInst.getPropertyTable();
			accPropTbl.insert(FOR_CLEAN);
			accPropTblYIt = accPropTbl.begin();

			pWidget       = new QWidget(0);
			pLabel[0]     = new QLabel(pWidget);
			pLabel[1]     = new QLabel(pWidget);
		}

		~FxMatcherBase()
		{
			delete pWidget;
		}

	protected:
		AccessorInstanceRef         accInst;
		QWidget                   * pWidget; 
		QLabel                    * pLabel[2];
	};

	//--------------------------------------------------------------------------
	class FxMatcher : public FxMatcherBase
	{
	public:

		FxMatcher()
		{
			pLabel[0]->setObjectName(PROPTBLCOL0);
			pLabel[1]->setObjectName(PROPTBLCOL1);

			matcher		  = tie::Manager::getInstance().createMatcher(pWidget, accPropTbl);

			guiPropTbl    = matcher.getLeft();
			guiPropTblYIt = guiPropTbl.begin();
		}
	};

	//--------------------------------------------------------------------------
	class FxMatcherGuiCol : public FxMatcherBase
	{
	public:

		FxMatcherGuiCol()
		{
			pLabel[0]->setObjectName(GUITBLCOL0);
			pLabel[1]->setObjectName(GUITBLCOL1);

			matcher		  = tie::Manager::getInstance().createMatcher(pWidget, accPropTbl);

			guiPropTbl    = matcher.getLeft();
			guiPropTblYIt = guiPropTbl.begin();

			std::list<VarString> cols,
								 labels;

			cols  .push_back(PROPTBLCOL0);
			cols  .push_back(PROPTBLCOL1);
			labels.push_back(GUITBLCOL0);
			labels.push_back(GUITBLCOL1);

			matcher.transformProperties(labels, cols);
		}
	};


// -----------------------------------------------------------------------------
TEST_FIXTURE(FxMatcher, MatcherIntDateTime_RL)
{
	CHECK_EQUAL(1, (int)accPropTbl.size());
    CHECK_EQUAL(5, (int)accPropTbl.getPropertyDescriptionList().size());

    CHECK_EQUAL(1, (int)guiPropTbl.size());
    CHECK_EQUAL(2, (int)guiPropTbl.getPropertyDescriptionList().size());

	try
	{
		accPropTblYIt.setInt32(PROPTBLCOL0, INTDT);
		accPropTblYIt.setInt32(PROPTBLCOL1, INTTM);
		matcher.RightToLeft();		
		CHECK_EQUAL(INTDT_STR, guiPropTblYIt.getString(PROPTBLCOL0).c_str());
		CHECK_EQUAL(INTTM_STR, guiPropTblYIt.getString(PROPTBLCOL1).c_str());
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
TEST_FIXTURE(FxMatcher, MatcherIntDateTime_LR)
{
	CHECK_EQUAL(1, (int)accPropTbl.size());
    CHECK_EQUAL(5, (int)accPropTbl.getPropertyDescriptionList().size());

    CHECK_EQUAL(1, (int)guiPropTbl.size());
    CHECK_EQUAL(2, (int)guiPropTbl.getPropertyDescriptionList().size());

	try
	{
		guiPropTblYIt.setString(PROPTBLCOL0, INTDT_STR);
		guiPropTblYIt.setString(PROPTBLCOL1, INTTM_STR);
		matcher.LeftToRight();
		CHECK_EQUAL(INTDT, accPropTblYIt.getInt32(PROPTBLCOL0));
		CHECK_EQUAL(INTTM, accPropTblYIt.getInt32(PROPTBLCOL1));
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

// -----------------------------------------------------------------------------
TEST_FIXTURE(FxMatcherGuiCol, MatcherIntDateTimeGuiCol_RL)
{
	CHECK_EQUAL(1, (int)accPropTbl.size());
    CHECK_EQUAL(5, (int)accPropTbl.getPropertyDescriptionList().size());

    CHECK_EQUAL(1, (int)guiPropTbl.size());
    CHECK_EQUAL(2, (int)guiPropTbl.getPropertyDescriptionList().size());

	try
	{
		accPropTblYIt.setInt32(PROPTBLCOL0, INTDT);
		accPropTblYIt.setInt32(PROPTBLCOL1, INTTM);
		matcher.RightToLeft();
		CHECK_EQUAL(INTDT_STR, guiPropTblYIt.getString(GUITBLCOL0).c_str());
		CHECK_EQUAL(INTTM_STR, guiPropTblYIt.getString(GUITBLCOL1).c_str());
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
TEST_FIXTURE(FxMatcherGuiCol, MatcherIntDateTimeGuiCol_LR)
{
	CHECK_EQUAL(1, (int)accPropTbl.size());
    CHECK_EQUAL(5, (int)accPropTbl.getPropertyDescriptionList().size());

    CHECK_EQUAL(1, (int)guiPropTbl.size());
    CHECK_EQUAL(2, (int)guiPropTbl.getPropertyDescriptionList().size());

	try
	{
		guiPropTblYIt.setString(GUITBLCOL0, INTDT_STR);
		guiPropTblYIt.setString(GUITBLCOL1, INTTM_STR);
		matcher.LeftToRight();
		CHECK_EQUAL(INTDT, accPropTblYIt.getInt32(PROPTBLCOL0));
		CHECK_EQUAL(INTTM, accPropTblYIt.getInt32(PROPTBLCOL1));
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

