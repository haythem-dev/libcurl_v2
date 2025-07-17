/*
* @file                                            
* @author Anke Klink                      
* @date 2008                            
*/    

// -----------------------------------------------------------------------------
#include <UnitTest++/UnitTest++.h>
#include "libbasarguitie.h"

#include "libbasarproperty_propertytype.h"
#include "libbasarproperty_propertydescriptionlistref.h"
#include "libbasardbaspect_accessor.h"
#include "libbasardbaspect_connectionref.h"
#include "libbasardbaspect_manager.h"
#include "libbasardbaspect_macros.h"
#include "libbasardbaspect_accessmethod.h"
#include "libbasardbaspect_accessorpropertytable_yiterator.h"

#include "selectstringsmockcessor.h"

#include "testconnection.h"
#include "accessornames.h"

#include <boost/make_shared.hpp>

#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>

#pragma warning(push)
#pragma warning( disable: 4481 4512 )
#include <QtWidgets/QTableWidget>
#include <QtWidgets/QHeaderView>

#pragma warning(pop)

// -----------------------------------------------------------------------------
using basar::ConstString;
using basar::VarString;
using basar::Int32;
using basar::FOR_CLEAN;

using namespace basar::db;

using aspect::ConnectionRef;
using aspect::AccessorInstanceRef;
using aspect::AccessorPropertyTableRef;
using aspect::AccessorPropertyTable_YIterator;
using aspect::FULL_CACHING;

using namespace basar::gui;

using tie::MatcherRef;
using tie::GuiPropertyTableRef;
using tie::GuiPropertyTable_YIterator;
using tie::CheckBoxTransformer;

using basar::test::unit::guitie::TestConnection;



//------------------------------------------------------------------------------
SUITE(GuiTieTest_Matcher)
{

// -----------------------------------------------------------------------------
TEST(Matcher_Creation)
{
    try
    {    
        MatcherRef                                  matcher;
        basar::property::PropertyDescriptionListRef propertydescriptionlist;
        QWidget *                                   widget = 0;

        SelectStringsMockCessor selectStringsMockCessor(ACCESSORNAME_SELECT_STRINGS);

        try
        {
            // matcher with 0-Widget:
            {
				matcher = tie::Manager::getInstance().createMatcher(widget, selectStringsMockCessor.getPropertyTable());
                CHECK(!matcher.isNull());

                CHECK(0 == matcher.getLeft().size());
                CHECK(0 == matcher.getLeft().getPropertyDescriptionList().size());
            }

            // test with widget (no proptab entry) and subwidgets (proptab entry)
            {
                widget = new QWidget(0);

                // matcher with empty gui-proptab list:
				matcher = tie::Manager::getInstance().createMatcher(widget, selectStringsMockCessor.getPropertyTable());

                CHECK(!matcher.isNull());
                CHECK(0 == matcher.getLeft().getPropertyDescriptionList().size());

                // name the widget, it does not count as a prop
                VarString widgetobjectname = "widgetname";
                widget->setObjectName(widgetobjectname.c_str());

				matcher = tie::Manager::getInstance().createMatcher(widget, selectStringsMockCessor.getPropertyTable());
                propertydescriptionlist = matcher.getLeft().getPropertyDescriptionList();
                CHECK(0 == propertydescriptionlist.size());
                CHECK(!propertydescriptionlist.contains(widgetobjectname));

                // add a label:
                QLabel * lab = new QLabel(widget);
                VarString lableobjectname = "lablename";
                lab->setObjectName (lableobjectname.c_str());

				matcher = tie::Manager::getInstance().createMatcher(widget, selectStringsMockCessor.getPropertyTable());
                propertydescriptionlist = matcher.getLeft().getPropertyDescriptionList();

                CHECK(1 == propertydescriptionlist.size());
                CHECK(!propertydescriptionlist.contains(widgetobjectname));
                CHECK(propertydescriptionlist.contains(lableobjectname));
                CHECK(0 == propertydescriptionlist.getIndex(lableobjectname));

				delete widget;
            }

            // test lable-widget (proptab entry) and subwidgets (proptab entry) equal name
            {  
                widget = new QLabel(0);
                // matcher with empty gui-proptab list:
				matcher = tie::Manager::getInstance().createMatcher(widget, selectStringsMockCessor.getPropertyTable());

                CHECK(!matcher.isNull());
                propertydescriptionlist = matcher.getLeft().getPropertyDescriptionList();
                CHECK(1 == propertydescriptionlist.size());
                CHECK(propertydescriptionlist.contains(""));

                new QLabel(widget);
				matcher = tie::Manager::getInstance().createMatcher(widget, selectStringsMockCessor.getPropertyTable());
                propertydescriptionlist = matcher.getLeft().getPropertyDescriptionList();
                CHECK(1 == propertydescriptionlist.size());
                CHECK(propertydescriptionlist.contains(""));

				delete widget;

            }

            // test lable-widget (proptab entry) and subwidgets (proptab entry) equal name
            {
                VarString labelobjectname1 = "lablename1";
                VarString labelobjectname2 = "lablename2";
                widget = new QLabel(0);

                widget->setObjectName(labelobjectname1.c_str());
				matcher = tie::Manager::getInstance().createMatcher(widget, selectStringsMockCessor.getPropertyTable());
                propertydescriptionlist = matcher.getLeft().getPropertyDescriptionList();
                CHECK(1 == propertydescriptionlist.size());
                CHECK(propertydescriptionlist.contains(labelobjectname1));

                // add a label:
                QLabel * l1 = new QLabel(widget);
                l1->setObjectName (labelobjectname2.c_str());

				matcher = tie::Manager::getInstance().createMatcher(widget, selectStringsMockCessor.getPropertyTable());
                propertydescriptionlist = matcher.getLeft().getPropertyDescriptionList();

                CHECK(2 == propertydescriptionlist.size());
                CHECK(propertydescriptionlist.contains(labelobjectname1));
                CHECK(propertydescriptionlist.contains(labelobjectname2));
                CHECK(0 == propertydescriptionlist.getIndex(labelobjectname1));
                CHECK(1 == propertydescriptionlist.getIndex(labelobjectname2));

				delete widget;
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
            const bool ExceptionOccured2 = false;
            CHECK(ExceptionOccured2);
            throw;
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
        const bool ExceptionOccured2 = false;
        CHECK(ExceptionOccured2);
        throw;
    }
}

// -----------------------------------------------------------------------------
	#define PROPTBLCOL				 "PropTblCol"

	//--------------------------------------------------------------------------
	static ConstString GUITBLCOL     = "GuiTblCol";
	static ConstString DBCHECKED     = "checked";
	static ConstString DBUNCHECKED   = "unchecked";
	static ConstString DBWHATEVER    = "xyz";

	static ConstString ACCDEF        = "GuiTieTest_MatcherAccDefName" ;
	static ConstString ACCINST       = "GuiTieTest_MatcherAccInstName";

	//--------------------------------------------------------------------------
	BEGIN_ACCESSOR_DEFINITION(ACCDEF)
		PROPERTY_DESCRIPTION_LIST("string dummy0;"
								  "int32  dummy1;"
								  "string " PROPTBLCOL ";"
								  "date   dummy2;")
	END_ACCESSOR_DEFINITION

	//--------------------------------------------------------------------------
	class FxMatcher
	{
	public:
		MatcherRef                      matcher; 

		AccessorPropertyTableRef	    accPropTbl;
		AccessorPropertyTable_YIterator accPropTblYIt;

		GuiPropertyTableRef             guiPropTbl;
		GuiPropertyTable_YIterator      guiPropTblYIt;

		FxMatcher()
		{
            selectStringsMockCessor = boost::make_shared<SelectStringsMockCessor>(ACCDEF);

            accPropTbl = selectStringsMockCessor->getPropertyTable();

			accPropTbl.insert(FOR_CLEAN);
			accPropTblYIt = accPropTbl.begin();

			pWidget       = new QLabel(0);

			pWidget->setObjectName(GUITBLCOL);

			matcher		  = tie::Manager::getInstance().createMatcher(pWidget, accPropTbl);

			guiPropTbl    = matcher.getLeft();
			guiPropTblYIt = guiPropTbl.begin();

			CheckBoxTransformer tf (GUITBLCOL, 
									DBCHECKED, DBUNCHECKED,
									CheckBoxTransformer::EVAL_STATE_ON, 
									PROPTBLCOL);
			matcher.push_back(tf);
		}

		~FxMatcher()
		{
			delete pWidget;
		}

	protected:
        SelectStringsMockCessorPtr selectStringsMockCessor;
		QWidget                   * pWidget; 
	};

// -----------------------------------------------------------------------------
TEST_FIXTURE(FxMatcher, MatcherWithTransformer_RL)
{
	CHECK_EQUAL(1, (int)accPropTbl.size());
    CHECK_EQUAL(4, (int)accPropTbl.getPropertyDescriptionList().size());

    CHECK_EQUAL(1, (int)guiPropTbl.size());
    CHECK_EQUAL(1, (int)guiPropTbl.getPropertyDescriptionList().size());

	accPropTblYIt.setString(PROPTBLCOL, DBCHECKED);
	matcher.RightToLeft();
	CHECK_EQUAL(CheckBoxTransformer::s_STATEON, guiPropTblYIt.getString(GUITBLCOL).c_str());

	accPropTblYIt.setString(PROPTBLCOL, DBUNCHECKED);
	matcher.RightToLeft();
	CHECK_EQUAL(CheckBoxTransformer::s_STATEOFF, guiPropTblYIt.getString(GUITBLCOL).c_str());

	accPropTblYIt.setString(PROPTBLCOL, DBWHATEVER);
	matcher.RightToLeft();
	CHECK_EQUAL(CheckBoxTransformer::s_STATEON, guiPropTblYIt.getString(GUITBLCOL).c_str());
}

//------------------------------------------------------------------------------
TEST_FIXTURE(FxMatcher, MatcherWithTransformer_LR)
{
	CHECK_EQUAL(1, (int)accPropTbl.size());
    CHECK_EQUAL(4, (int)accPropTbl.getPropertyDescriptionList().size());

    CHECK_EQUAL(1, (int)guiPropTbl.size());
    CHECK_EQUAL(1, (int)guiPropTbl.getPropertyDescriptionList().size());

	guiPropTblYIt.setString(GUITBLCOL, CheckBoxTransformer::s_STATEON);
	matcher.LeftToRight();
	CHECK_EQUAL(DBCHECKED, accPropTblYIt.getString(PROPTBLCOL).c_str());

	guiPropTblYIt.setString(GUITBLCOL, CheckBoxTransformer::s_STATEOFF);
	matcher.LeftToRight();
	CHECK_EQUAL(DBUNCHECKED, accPropTblYIt.getString(PROPTBLCOL).c_str());
}

// -----------------------------------------------------------------------------
class FxMatcherTable
{
	public:
		MatcherRef                      matcher; 

        const basar::VarString str1;
        const basar::VarString str2;
        const basar::VarString str3;
        const basar::VarString str4;

		AccessorPropertyTableRef	    accPropTbl;
		AccessorPropertyTable_YIterator accPropTblYit;

		GuiPropertyTableRef             guiPropTbl;
		GuiPropertyTable_YIterator      guiPropTblYit;

		FxMatcherTable() : str1("mystr1"), str2("mystr2"), str3("mystr3"), str4("mystr4")
		{
            selectStringsMockCessor = boost::make_shared<SelectStringsMockCessor>(ACCDEF);

			accPropTbl    = selectStringsMockCessor->getPropertyTable();

			accPropTblYit = accPropTbl.insert(FOR_CLEAN);

            accPropTblYit.setString( PROPTBLCOL, str1 );

            accPropTblYit = accPropTbl.insert(FOR_CLEAN);

            accPropTblYit.setString( PROPTBLCOL, str2 );

            accPropTblYit = accPropTbl.insert(FOR_CLEAN);

            accPropTblYit.setString( PROPTBLCOL, str3 );

            accPropTblYit = accPropTbl.insert(FOR_CLEAN);

            accPropTblYit.setString( PROPTBLCOL, str4 );

            pWidget       = new QTableWidget(0);

            pWidget->setRowCount( 4 );
            pWidget->setColumnCount( 1 );
            QTableWidgetItem* h = new QTableWidgetItem();
            h->setText( "string" );
            pWidget->setHorizontalHeaderItem(0, h);

            for( int i = 0; i < 4; ++i )
            {
                pWidget->setItem( i, 0, new QTableWidgetItem() );
            }

            std::list<basar::VarString> headerLabels;
            headerLabels.push_back("string");

            std::list<basar::VarString> origAttrs;
            origAttrs.push_back(PROPTBLCOL);

			matcher		  = tie::Manager::getInstance().createMatcher(pWidget, accPropTbl);

            matcher.setMatchingPolicy( tie::DISREGARD_DELETED | tie::SKIP );

            matcher.transformProperties( headerLabels, origAttrs );

			guiPropTbl    = matcher.getLeft();
			guiPropTblYit = guiPropTbl.begin();
		}

		~FxMatcherTable()
		{
			delete pWidget;
		}

	protected:
        SelectStringsMockCessorPtr selectStringsMockCessor;
		QTableWidget              * pWidget; 
    private:
        FxMatcherTable( const FxMatcherTable& );
        FxMatcherTable& operator=( const FxMatcherTable& );
};

// -----------------------------------------------------------------------------
TEST_FIXTURE(FxMatcherTable, MatcherDisregardDeleted_MatchFromGui_FirstAccEntry)
{
    //mark first dataset as deleted
    accPropTbl.begin().markForDelete();

    CHECK_EQUAL( basar::SS_DELETE, accPropTbl.begin().getState().getStates() );

    matcher.RightToLeft();

    CHECK_EQUAL( str2.c_str(), pWidget->item(0, 0)->text().toLocal8Bit().constData() );
    CHECK_EQUAL( str3.c_str(), pWidget->item(1, 0)->text().toLocal8Bit().constData() );
    CHECK_EQUAL( str4.c_str(), pWidget->item(2, 0)->text().toLocal8Bit().constData() );

    matcher.LeftToRight();

    accPropTblYit = accPropTbl.begin();

    CHECK_EQUAL( str1, accPropTblYit.getString( PROPTBLCOL ) );
    ++accPropTblYit;
    CHECK_EQUAL( str2, accPropTblYit.getString( PROPTBLCOL ) );
    ++accPropTblYit;
    CHECK_EQUAL( str3, accPropTblYit.getString( PROPTBLCOL ) );
    ++accPropTblYit;
    CHECK_EQUAL( str4, accPropTblYit.getString( PROPTBLCOL ) );

}

// -----------------------------------------------------------------------------
TEST_FIXTURE(FxMatcherTable, MatcherDisregardDeleted_MatchFromGui_LastAccEntry)
{
    //mark last dataset as deleted
    accPropTbl.at(3).markForDelete();

    CHECK_EQUAL( basar::SS_DELETE, accPropTbl.at(3).getState().getStates() );

    matcher.RightToLeft();

    CHECK_EQUAL( str1.c_str(), pWidget->item(0, 0)->text().toLocal8Bit().constData() );
    CHECK_EQUAL( str2.c_str(), pWidget->item(1, 0)->text().toLocal8Bit().constData() );
    CHECK_EQUAL( str3.c_str(), pWidget->item(2, 0)->text().toLocal8Bit().constData() );

    matcher.LeftToRight();

    accPropTblYit = accPropTbl.begin();

    CHECK_EQUAL( str1, accPropTblYit.getString( PROPTBLCOL ) );
    ++accPropTblYit;
    CHECK_EQUAL( str2, accPropTblYit.getString( PROPTBLCOL ) );
    ++accPropTblYit;
    CHECK_EQUAL( str3, accPropTblYit.getString( PROPTBLCOL ) );
    ++accPropTblYit;
    CHECK_EQUAL( str4, accPropTblYit.getString( PROPTBLCOL ) );

}

// -----------------------------------------------------------------------------

	static const Int32 PROPTBLVAL     = 1235;
	static ConstString PROPTBLVAL_STR = "1235";

	static ConstString ACCDEF2        = "GuiTieTest_Matcher2AccDefName" ;
	static ConstString ACCINST2       = "GuiTieTest_Matcher2AccInstName";

	static ConstString ACCDEF3        = "GuiTieTest_Matcher3AccDefName" ;
	static ConstString ACCINST3       = "GuiTieTest_Matcher3AccInstName";

	namespace MatcherUnset
	{
		BEGIN_ACCESSOR_DEFINITION(ACCDEF2)
			PROPERTY_DESCRIPTION_LIST("string dummy0;"
									  "int32  dummy1;"
									  "int32 " PROPTBLCOL ";"
									  "date   dummy2;")
		END_ACCESSOR_DEFINITION
	}

	namespace MatcherUnset2
	{
		BEGIN_ACCESSOR_DEFINITION(ACCDEF3)
			PROPERTY_DESCRIPTION_LIST("int32 " PROPTBLCOL ";")
		END_ACCESSOR_DEFINITION
	}

	//--------------------------------------------------------------------------
	class FxMatcherUnset
	{
	public:
		MatcherRef                      matcher; 

		AccessorPropertyTableRef	    accPropTbl;
		AccessorPropertyTable_YIterator accPropTblYIt;

		GuiPropertyTableRef             guiPropTbl;
		GuiPropertyTable_YIterator      guiPropTblYIt;

		FxMatcherUnset()
		{
            selectStringsMockCessor = boost::make_shared<SelectStringsMockCessor>(ACCDEF2);

			accPropTbl    = selectStringsMockCessor->getPropertyTable();
			accPropTbl.insert(FOR_CLEAN);
			accPropTblYIt = accPropTbl.begin();
			accPropTblYIt.setInt32(PROPTBLCOL, PROPTBLVAL);

			pWidget       = new QLineEdit(0);
			pWidget->setObjectName(PROPTBLCOL);

			matcher		  = tie::Manager::getInstance().createMatcher(pWidget, accPropTbl);

			guiPropTbl    = matcher.getLeft();
			guiPropTblYIt = guiPropTbl.begin();

			matcher.RightToLeft();
			CHECK_EQUAL(PROPTBLVAL_STR, guiPropTblYIt.getString(PROPTBLCOL).c_str());
			CHECK_EQUAL(PROPTBLVAL_STR, pWidget->text().toLocal8Bit().constData());
		}

		~FxMatcherUnset()
		{
			delete pWidget;
		}

	protected:
        SelectStringsMockCessorPtr selectStringsMockCessor;

		QLineEdit                 * pWidget; 
	};


// -----------------------------------------------------------------------------
TEST_FIXTURE(FxMatcherUnset, MatcherUnsetProperty_RL)
{
	AccessorInstanceRef             locAccInst;
	AccessorPropertyTableRef        locAccPropTbl;
	AccessorPropertyTable_YIterator locAccPropTblYIt;
	GuiPropertyTableRef             locGuiPropTbl;
	GuiPropertyTable_YIterator      locGuiPropTblYIt;

	MatcherRef						locMatch; 

	locAccInst = aspect::Manager::getInstance().createAccessorInstance(ACCINST3,
																	   ACCDEF3,
																	   ConnectionRef(), 
																	   FULL_CACHING);
	locAccPropTbl = locAccInst.getPropertyTable();
	locAccPropTbl.insert(FOR_CLEAN);

	locAccPropTblYIt = locAccPropTbl.begin();

	locAccPropTblYIt.setInt32        (PROPTBLCOL, 0);
	locAccPropTblYIt.setPropertyState(PROPTBLCOL, basar::SS_UNSET);

	locMatch	     = tie::Manager::getInstance().createMatcher(pWidget, locAccPropTbl);

	locGuiPropTbl    = locMatch.getLeft();
	locGuiPropTblYIt = locGuiPropTbl.begin();

	locMatch.RightToLeft();

	CHECK_EQUAL(PROPTBLVAL_STR, guiPropTblYIt.getString(PROPTBLCOL).c_str());
	CHECK_EQUAL(PROPTBLVAL_STR, pWidget->text().toLocal8Bit().constData());
}

// -----------------------------------------------------------------------------
}	// SUITE
