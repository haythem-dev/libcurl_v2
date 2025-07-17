/* 
 * @file   matching SQL type and QLabel                                           
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
using basar::VarString;
using basar::Int32;
using basar::DateTime;
using basar::Date;
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

//------------------------------------------------------------------------------
SUITE(MatcherSqlDateTime)
{
	// -------------------------------------------------------------------------
	#define DEF_PROPTBLCOL0				"PropTblCol0"
	#define DEF_PROPTBLCOL1				"PropTblCol1"
	#define DEF_PROPTBLCOL2				"PropTblCol2"

	//--------------------------------------------------------------------------
	static ConstString PROPTBLCOL0   = DEF_PROPTBLCOL0;
	static ConstString PROPTBLCOL1   = DEF_PROPTBLCOL1;
	static ConstString PROPTBLCOL2   = DEF_PROPTBLCOL2;
	static ConstString GUITBLCOL0    = "GuiTblCol0";
	static ConstString GUITBLCOL1    = "GuiTblCol1";
	static ConstString GUITBLCOL2    = "GuiTblCol2";

	static ConstString ACCDEF        = "GuiTieTest_MatcherSqlDateTimeAccDefName" ;
	static ConstString ACCINST       = "GuiTieTest_MatcherSqlDateTimeAccInstName";

	static const DateTime SQLDT      = DateTime(20100329, 11259345);
	static ConstString    SQLDT_STR  = "29.03.2010 01:12:59.345";

	static const Date     SQLD       = Date(20090228);
	static ConstString    SQLD_STR   = "28.02.2009";

	static const Time     SQLT       = Time(Time::NULL_DATE, 25449987);
	static ConstString    SQLT_STR   = "02:54:49.987";

	//--------------------------------------------------------------------------
	BEGIN_ACCESSOR_DEFINITION(ACCDEF)
		PROPERTY_DESCRIPTION_LIST("string     dummy0;"
								  "int32      dummy1;"
								  "datetime " DEF_PROPTBLCOL0 ";"
								  "date     " DEF_PROPTBLCOL1 ";"
								  "time     " DEF_PROPTBLCOL2 ";"
								  "int16      dummy2;")
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
			pLabel[2]     = new QLabel(pWidget);
		}

		~FxMatcherBase()
		{
			delete pWidget;
		}

	protected:
		AccessorInstanceRef         accInst;
		QWidget                   * pWidget; 
		QLabel                    * pLabel[3];
	};

	//--------------------------------------------------------------------------
	class FxMatcher : public FxMatcherBase
	{
	public:

		FxMatcher()
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
	class FxMatcherGuiCol : public FxMatcherBase
	{
	public:

		FxMatcherGuiCol()
		{
			pLabel[0]->setObjectName(GUITBLCOL0);
			pLabel[1]->setObjectName(GUITBLCOL1);
			pLabel[2]->setObjectName(GUITBLCOL2);

			matcher		  = tie::Manager::getInstance().createMatcher(pWidget, accPropTbl);

			guiPropTbl    = matcher.getLeft();
			guiPropTblYIt = guiPropTbl.begin();

			std::list<VarString> cols,
								 labels;

			cols  .push_back(PROPTBLCOL0);
			cols  .push_back(PROPTBLCOL1);
			cols  .push_back(PROPTBLCOL2);
			labels.push_back(GUITBLCOL0);
			labels.push_back(GUITBLCOL1);
			labels.push_back(GUITBLCOL2);

			matcher.transformProperties(labels, cols);
		}
	};


// -----------------------------------------------------------------------------
TEST_FIXTURE(FxMatcher, MatcherSqlDateTime_RL)
{
	CHECK_EQUAL(1, (int)accPropTbl.size());
    CHECK_EQUAL(6, (int)accPropTbl.getPropertyDescriptionList().size());

    CHECK_EQUAL(1, (int)guiPropTbl.size());
    CHECK_EQUAL(3, (int)guiPropTbl.getPropertyDescriptionList().size());

	try
	{
		accPropTblYIt.setDateTime(PROPTBLCOL0, SQLDT);
		accPropTblYIt.setDate    (PROPTBLCOL1, SQLD );
		accPropTblYIt.setTime    (PROPTBLCOL2, SQLT );
		matcher.RightToLeft();		
		CHECK_EQUAL(SQLDT_STR, guiPropTblYIt.getString(PROPTBLCOL0).c_str());
		CHECK_EQUAL(SQLD_STR , guiPropTblYIt.getString(PROPTBLCOL1).c_str());
		CHECK_EQUAL(SQLT_STR , guiPropTblYIt.getString(PROPTBLCOL2).c_str());
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
TEST_FIXTURE(FxMatcher, MatcherSqlDateTime_LR)
{
	CHECK_EQUAL(1, (int)accPropTbl.size());
    CHECK_EQUAL(6, (int)accPropTbl.getPropertyDescriptionList().size());

    CHECK_EQUAL(1, (int)guiPropTbl.size());
    CHECK_EQUAL(3, (int)guiPropTbl.getPropertyDescriptionList().size());

	try
	{
		guiPropTblYIt.setString(PROPTBLCOL0, SQLDT_STR);
		guiPropTblYIt.setString(PROPTBLCOL1, SQLD_STR );
		guiPropTblYIt.setString(PROPTBLCOL2, SQLT_STR );
		matcher.LeftToRight();
		CHECK(SQLDT          == accPropTblYIt.getDateTime(PROPTBLCOL0));
		CHECK(SQLD           == accPropTblYIt.getDate    (PROPTBLCOL1));
		CHECK(SQLT.getTime() == accPropTblYIt.getTime    (PROPTBLCOL2).getTime());
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
TEST_FIXTURE(FxMatcherGuiCol, MatcherSqlDateTimeGuiCol_RL)
{
	CHECK_EQUAL(1, (int)accPropTbl.size());
    CHECK_EQUAL(6, (int)accPropTbl.getPropertyDescriptionList().size());

    CHECK_EQUAL(1, (int)guiPropTbl.size());
    CHECK_EQUAL(3, (int)guiPropTbl.getPropertyDescriptionList().size());

	try
	{
		accPropTblYIt.setDateTime(PROPTBLCOL0, SQLDT);
		accPropTblYIt.setDate    (PROPTBLCOL1, SQLD );
		accPropTblYIt.setTime    (PROPTBLCOL2, SQLT );
		matcher.RightToLeft();
		CHECK_EQUAL(SQLDT_STR, guiPropTblYIt.getString(GUITBLCOL0).c_str());
		CHECK_EQUAL(SQLD_STR , guiPropTblYIt.getString(GUITBLCOL1).c_str());
		CHECK_EQUAL(SQLT_STR , guiPropTblYIt.getString(GUITBLCOL2).c_str());
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
TEST_FIXTURE(FxMatcherGuiCol, MatcherSqlDateTimeGuiCol_LR)
{
	CHECK_EQUAL(1, (int)accPropTbl.size());
    CHECK_EQUAL(6, (int)accPropTbl.getPropertyDescriptionList().size());

    CHECK_EQUAL(1, (int)guiPropTbl.size());
    CHECK_EQUAL(3, (int)guiPropTbl.getPropertyDescriptionList().size());

	try
	{
		guiPropTblYIt.setString(GUITBLCOL0, SQLDT_STR);
		guiPropTblYIt.setString(GUITBLCOL1, SQLD_STR );
		guiPropTblYIt.setString(GUITBLCOL2, SQLT_STR );
		matcher.LeftToRight();
		CHECK(SQLDT          == accPropTblYIt.getDateTime(PROPTBLCOL0));
		CHECK(SQLD           == accPropTblYIt.getDate    (PROPTBLCOL1));
		CHECK(SQLT.getTime() == accPropTblYIt.getTime    (PROPTBLCOL2).getTime());
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

