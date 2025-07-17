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

#pragma warning (push)
#pragma warning (disable: 4481)
#include <QtWidgets/QGroupBox>
#include <QtWidgets/QComboBox>
#pragma warning (pop)

//------------------------------------------------------------------------------
using basar::ConstString;
using basar::VarString;
using basar::Int32;
using basar::FOR_CLEAN;
using basar::Exception;


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
using tie::CurrentListTransformer;

// expected behaviour for default matching prefilled comboboxes is selecting appropriate item,
// but it doesn't work without transformer 
// => test actual behaviour
#define ACTUAL_BEHAVIOUR

//------------------------------------------------------------------------------
SUITE(MatcherComboBox)
{
	//--------------------------------------------------------------------------
	#define DEF_COLKEY		"key"
	#define DEF_COLTEXT		"displaytest"
	#define DEF_COLTEXT2	"displaytest2"

	//--------------------------------------------------------------------------
	static ConstString ACCDEF         = "MatcherComboxBoxAccDefName" ;
	static ConstString ACCDEF2        = "MatcherComboxBoxAccDefName2" ;
	static ConstString ACCDEF_MAPPED  = "MatcherComboxBoxMappedAccDefName" ;
	static ConstString ACCINST        = "MatcherComboxBoxAccInstName";

	static ConstString ACCDEF_LOOKUP  = "LookupAccDefName" ;
	static ConstString ACCINST_LOOKUP = "LookupAccInstName";

	static Int32       LOOKUP_KEY0	 = 0;
	static Int32       LOOKUP_KEY1	 = 1;
	static Int32       LOOKUP_KEY2	 = 2;
	static Int32       LOOKUP_KEY3	 = 3;
	static Int32       LOOKUP_KEY4	 = 4;

	static Int32       LOOKUP2_KEY0	 = 10;
	static Int32       LOOKUP2_KEY1	 = 11;
	static Int32       LOOKUP2_KEY2	 = 12;
	static Int32       LOOKUP2_KEY3	 = 13;
	static Int32       LOOKUP2_KEY4	 = 14;

	static ConstString LOOKUP_NAME0	 = "LookupName0";
	static ConstString LOOKUP_NAME1	 = "LookupName1";
	static ConstString LOOKUP_NAME2	 = "LookupName2";
	static ConstString LOOKUP_NAME3	 = "LookupName3";
	static ConstString LOOKUP_NAME4	 = "LookupName4";

	static ConstString LOOKUP2_NAME0 = "Lookup2Name0";
	static ConstString LOOKUP2_NAME1 = "Lookup2Name1";
	static ConstString LOOKUP2_NAME2 = "Lookup2Name2";
	static ConstString LOOKUP2_NAME3 = "Lookup2Name3";
	static ConstString LOOKUP2_NAME4 = "Lookup2Name4";

	//--------------------------------------------------------------------------
	namespace nomapping
	{
		BEGIN_ACCESSOR_DEFINITION(ACCDEF)
			PROPERTY_DESCRIPTION_LIST("string   dummy0;"
									  "int32    dummy1;"
									  "string " DEF_COLTEXT ";"
									  "date     dummy2;")
		END_ACCESSOR_DEFINITION
	}

	namespace nomapping_diffcolnames
	{
		BEGIN_ACCESSOR_DEFINITION(ACCDEF2)
			PROPERTY_DESCRIPTION_LIST("string   dummy0;"
									  "int32    dummy1;"
									  "string " DEF_COLTEXT2 ";"
									  "date     dummy2;")
		END_ACCESSOR_DEFINITION
	}

	namespace mapping
	{
		BEGIN_ACCESSOR_DEFINITION(ACCDEF_MAPPED)
			PROPERTY_DESCRIPTION_LIST("string  dummy0;"
									  "int32   dummy1;"
									  "int32 " DEF_COLKEY ";"
									  "date    dummy2;")
		END_ACCESSOR_DEFINITION
	}

	namespace lookup
	{
		BEGIN_ACCESSOR_DEFINITION(ACCDEF_LOOKUP)
			PROPERTY_DESCRIPTION_LIST("int32  " DEF_COLKEY  ";"
									  "string " DEF_COLTEXT ";")
		END_ACCESSOR_DEFINITION
	}

	//--------------------------------------------------------------------------
	enum CaseEnum
	{
		CASE_NOMAPPING, 
		CASE_NOMAPPING_DIFFCOLNAMES,
		CASE_MAPPING
	};

	//--------------------------------------------------------------------------
	class FxMatcherBase
	{
	public:
		MatcherRef                      matcher; 
		MatcherRef                      matcherLookup; 

		AccessorPropertyTableRef	    accPropTbl;
		AccessorPropertyTable_YIterator accPropTblYIt;
		AccessorPropertyTableRef	    accPropTblLookup;

		GuiPropertyTableRef             guiPropTbl;
		GuiPropertyTable_YIterator      guiPropTblYIt;

		FxMatcherBase(CaseEnum eCase)
		{
			{
				accInstLookup = aspect::Manager::getInstance().createAccessorInstance(ACCINST_LOOKUP,
																					  ACCDEF_LOOKUP,
																					  ConnectionRef(), 
																					  FULL_CACHING);
				accPropTblLookup = accInstLookup.getPropertyTable();

				AccessorPropertyTable_YIterator yitLookup;
				yitLookup = accPropTblLookup.insert(FOR_CLEAN);
				yitLookup.setInt32 (DEF_COLKEY , LOOKUP_KEY0);
				yitLookup.setString(DEF_COLTEXT, LOOKUP_NAME0);

				yitLookup = accPropTblLookup.insert(FOR_CLEAN);
				yitLookup.setInt32 (DEF_COLKEY , LOOKUP_KEY1);
				yitLookup.setString(DEF_COLTEXT, LOOKUP_NAME1);

				yitLookup = accPropTblLookup.insert(FOR_CLEAN);
				yitLookup.setInt32 (DEF_COLKEY , LOOKUP_KEY2);
				yitLookup.setString(DEF_COLTEXT, LOOKUP_NAME2);

				yitLookup = accPropTblLookup.insert(FOR_CLEAN);
				yitLookup.setInt32 (DEF_COLKEY , LOOKUP_KEY3);
				yitLookup.setString(DEF_COLTEXT, LOOKUP_NAME3);

				yitLookup = accPropTblLookup.insert(FOR_CLEAN);
				yitLookup.setInt32 (DEF_COLKEY , LOOKUP_KEY4);
				yitLookup.setString(DEF_COLTEXT, LOOKUP_NAME4);
			}

			pFrame = new QGroupBox(0);
			pCb    = new QComboBox(pFrame);
			pCb->setAccessibleName(DEF_COLTEXT);

			matcherLookup = tie::Manager::getInstance().createMatcher(pFrame, accPropTblLookup);
			matcherLookup.RightToLeft();

			VarString definition;
			
			switch (eCase)
			{
			case CASE_MAPPING:
				definition = ACCDEF_MAPPED;
				break;

			case CASE_NOMAPPING_DIFFCOLNAMES:
				definition = ACCDEF2;
				break;

			case CASE_NOMAPPING:
			default:
				definition = ACCDEF;
				break;
			}

			accInst       = aspect::Manager::getInstance().createAccessorInstance(ACCINST,
																		          definition,
																				  ConnectionRef(), 
																				  FULL_CACHING);
			accPropTbl    = accInst.getPropertyTable();
			accPropTbl.insert(FOR_CLEAN);
			accPropTblYIt = accPropTbl.begin();

			matcher       = tie::Manager::getInstance().createMatcher(pFrame, accPropTbl);

			guiPropTbl    = matcher.getLeft();
			guiPropTblYIt = guiPropTbl.begin();
		}

		~FxMatcherBase()
		{
			delete pFrame;
		}

		void changeTblLookup()
		{
			AccessorPropertyTable_YIterator yit = accPropTblLookup.begin();
			CHECK(yit != accPropTblLookup.end());
			yit.setInt32 (DEF_COLKEY , LOOKUP2_KEY0);
			yit.setString(DEF_COLTEXT, LOOKUP2_NAME0);

			++yit;
			CHECK(yit != accPropTblLookup.end());
			yit.setInt32 (DEF_COLKEY , LOOKUP2_KEY1);
			yit.setString(DEF_COLTEXT, LOOKUP2_NAME1);

			++yit;
			CHECK(yit != accPropTblLookup.end());
			yit.setInt32 (DEF_COLKEY , LOOKUP2_KEY2);
			yit.setString(DEF_COLTEXT, LOOKUP2_NAME2);

			++yit;
			CHECK(yit != accPropTblLookup.end());
			yit.setInt32 (DEF_COLKEY , LOOKUP2_KEY3);
			yit.setString(DEF_COLTEXT, LOOKUP2_NAME3);

			++yit;
			CHECK(yit != accPropTblLookup.end());
			yit.setInt32 (DEF_COLKEY , LOOKUP2_KEY4);
			yit.setString(DEF_COLTEXT, LOOKUP2_NAME4);

			++yit;
			CHECK(yit == accPropTblLookup.end());
		}

	protected:
		AccessorInstanceRef         accInst;
		AccessorInstanceRef         accInstLookup;
		QGroupBox                 * pFrame; 
		QComboBox                 * pCb;
	};

	//--------------------------------------------------------------------------
	// no key-value-mapping DB col <-> GUI widgetname
	class FxMatcherNoMapping : public FxMatcherBase
	{
	public:
		FxMatcherNoMapping() : FxMatcherBase(CASE_NOMAPPING)
		{
		}
	};

	//--------------------------------------------------------------------------
	// no key-value-mapping DB col <-> GUI widgetname, different column/widget names
	class FxMatcherNoMappingDiffColNames : public FxMatcherBase
	{
	public:
		FxMatcherNoMappingDiffColNames() : FxMatcherBase(CASE_NOMAPPING_DIFFCOLNAMES)
		{
		}
	};

	//--------------------------------------------------------------------------
	// key-value-mapping DB col <-> GUI widgetname, different column/widget names
	class FxMatcherMapping : public FxMatcherBase
	{
	public:
		FxMatcherMapping() : FxMatcherBase(CASE_MAPPING)
		{
		}
	};

// -----------------------------------------------------------------------------
// test matcher for filling/refilling combobox
TEST_FIXTURE(FxMatcherNoMapping, RefillComboBox)
{
	CHECK_EQUAL(LOOKUP_NAME0, pCb->itemText(0).toLocal8Bit().constData());
	CHECK_EQUAL(LOOKUP_NAME1, pCb->itemText(1).toLocal8Bit().constData());
	CHECK_EQUAL(LOOKUP_NAME2, pCb->itemText(2).toLocal8Bit().constData());
	CHECK_EQUAL(LOOKUP_NAME3, pCb->itemText(3).toLocal8Bit().constData());
	CHECK_EQUAL(LOOKUP_NAME4, pCb->itemText(4).toLocal8Bit().constData());

	changeTblLookup();
	matcherLookup.RightToLeft();

	CHECK_EQUAL(LOOKUP2_NAME0, pCb->itemText(0).toLocal8Bit().constData());
	CHECK_EQUAL(LOOKUP2_NAME1, pCb->itemText(1).toLocal8Bit().constData());
	CHECK_EQUAL(LOOKUP2_NAME2, pCb->itemText(2).toLocal8Bit().constData());
	CHECK_EQUAL(LOOKUP2_NAME3, pCb->itemText(3).toLocal8Bit().constData());
	CHECK_EQUAL(LOOKUP2_NAME4, pCb->itemText(4).toLocal8Bit().constData());
}

// -----------------------------------------------------------------------------
// no key-value-mapping, DB columname = widget name
TEST_FIXTURE(FxMatcherNoMapping, MatcherCbB_RL)
{
#ifdef ACTUAL_BEHAVIOUR
	const int   CURRENT_INDEX[] = { 0, 0, 0, 0, 0 };
	ConstString ITEM_TEXT       = LOOKUP_NAME4;
#else
	const int   CURRENT_INDEX[] = { 0, 1, 2, 3, 4 };
	ConstString ITEM_TEXT       = LOOKUP_NAME0;
#endif 

	CHECK_EQUAL(1, (int)accPropTbl.size());
    CHECK_EQUAL(4, (int)accPropTbl.getPropertyDescriptionList().size());

    CHECK_EQUAL(5, (int)guiPropTbl.size());
    CHECK_EQUAL(2, (int)guiPropTbl.getPropertyDescriptionList().size());

	CHECK_EQUAL(LOOKUP_NAME0, pCb->itemText(0).toLocal8Bit().constData());
	CHECK_EQUAL(LOOKUP_NAME1, pCb->itemText(1).toLocal8Bit().constData());
	CHECK_EQUAL(LOOKUP_NAME2, pCb->itemText(2).toLocal8Bit().constData());
	CHECK_EQUAL(LOOKUP_NAME3, pCb->itemText(3).toLocal8Bit().constData());
	CHECK_EQUAL(LOOKUP_NAME4, pCb->itemText(4).toLocal8Bit().constData());


	try
	{
		accPropTblYIt.setString(DEF_COLTEXT , LOOKUP_NAME0);
		matcher.RightToLeft();
		CHECK_EQUAL(CURRENT_INDEX[0], pCb->currentIndex()); 
		CHECK_EQUAL(LOOKUP_NAME0, pCb->currentText().toLocal8Bit().constData());
		CHECK_EQUAL(LOOKUP_NAME0, guiPropTblYIt.getCurrentString(DEF_COLTEXT).c_str());

		accPropTblYIt.setString(DEF_COLTEXT , LOOKUP_NAME1);
		matcher.RightToLeft();		
		CHECK_EQUAL(CURRENT_INDEX[1], pCb->currentIndex()); 
		CHECK_EQUAL(LOOKUP_NAME1, pCb->currentText().toLocal8Bit().constData());
		CHECK_EQUAL(LOOKUP_NAME1, guiPropTblYIt.getCurrentString(DEF_COLTEXT).c_str());

		accPropTblYIt.setString(DEF_COLTEXT , LOOKUP_NAME2);
		matcher.RightToLeft();		
		CHECK_EQUAL(CURRENT_INDEX[2], pCb->currentIndex()); 
		CHECK_EQUAL(LOOKUP_NAME2, pCb->currentText().toLocal8Bit().constData());
		CHECK_EQUAL(LOOKUP_NAME2, guiPropTblYIt.getCurrentString(DEF_COLTEXT).c_str());

		accPropTblYIt.setString(DEF_COLTEXT , LOOKUP_NAME3);
		matcher.RightToLeft();		
		CHECK_EQUAL(CURRENT_INDEX[3], pCb->currentIndex()); 
		CHECK_EQUAL(LOOKUP_NAME3, pCb->currentText().toLocal8Bit().constData());
		CHECK_EQUAL(LOOKUP_NAME3, guiPropTblYIt.getCurrentString(DEF_COLTEXT).c_str());

		accPropTblYIt.setString(DEF_COLTEXT , LOOKUP_NAME4);
		matcher.RightToLeft();		
		CHECK_EQUAL(CURRENT_INDEX[4], pCb->currentIndex()); 
		CHECK_EQUAL(LOOKUP_NAME4, pCb->currentText().toLocal8Bit().constData());
		CHECK_EQUAL(LOOKUP_NAME4, guiPropTblYIt.getCurrentString(DEF_COLTEXT).c_str());
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

	CHECK_EQUAL(ITEM_TEXT   , pCb->itemText(0).toLocal8Bit().constData());
	CHECK_EQUAL(LOOKUP_NAME1, pCb->itemText(1).toLocal8Bit().constData());
	CHECK_EQUAL(LOOKUP_NAME2, pCb->itemText(2).toLocal8Bit().constData());
	CHECK_EQUAL(LOOKUP_NAME3, pCb->itemText(3).toLocal8Bit().constData());
	CHECK_EQUAL(LOOKUP_NAME4, pCb->itemText(4).toLocal8Bit().constData());
}

//------------------------------------------------------------------------------
// no key-value-mapping, DB columname = widget name
TEST_FIXTURE(FxMatcherNoMapping, MatcherCbB_LR)
{
#ifdef ACTUAL_BEHAVIOUR
	ConstString ITEM_TEXT[]  = 
	{ 
		LOOKUP_NAME0,
		LOOKUP_NAME0,
		LOOKUP_NAME0,
		LOOKUP_NAME0,
		LOOKUP_NAME0
	};
#else
	ConstString ITEM_TEXT[]  = 
	{ 
		LOOKUP_NAME0,
		LOOKUP_NAME1,
		LOOKUP_NAME2,
		LOOKUP_NAME3,
		LOOKUP_NAME4
	};
#endif 

	CHECK_EQUAL(1, (int)accPropTbl.size());
    CHECK_EQUAL(4, (int)accPropTbl.getPropertyDescriptionList().size());

    CHECK_EQUAL(5, (int)guiPropTbl.size());
    CHECK_EQUAL(2, (int)guiPropTbl.getPropertyDescriptionList().size());

	CHECK_EQUAL(LOOKUP_NAME0, pCb->itemText(0).toLocal8Bit().constData());
	CHECK_EQUAL(LOOKUP_NAME1, pCb->itemText(1).toLocal8Bit().constData());
	CHECK_EQUAL(LOOKUP_NAME2, pCb->itemText(2).toLocal8Bit().constData());
	CHECK_EQUAL(LOOKUP_NAME3, pCb->itemText(3).toLocal8Bit().constData());
	CHECK_EQUAL(LOOKUP_NAME4, pCb->itemText(4).toLocal8Bit().constData());

	try
	{
		guiPropTblYIt.setCurrentString(DEF_COLTEXT, LOOKUP_NAME0);
		CHECK_EQUAL(0, pCb->currentIndex()); 
		CHECK_EQUAL(LOOKUP_NAME0, pCb->currentText().toLocal8Bit().constData());
		matcher.LeftToRight();
		CHECK_EQUAL(ITEM_TEXT[0], accPropTblYIt.getString(DEF_COLTEXT));

		guiPropTblYIt.setCurrentString(DEF_COLTEXT, LOOKUP_NAME1);
		CHECK_EQUAL(1, pCb->currentIndex()); 
		CHECK_EQUAL(LOOKUP_NAME1, pCb->currentText().toLocal8Bit().constData());
		matcher.LeftToRight();
		CHECK_EQUAL(ITEM_TEXT[1], accPropTblYIt.getString(DEF_COLTEXT));

		guiPropTblYIt.setCurrentString(DEF_COLTEXT, LOOKUP_NAME2);
		CHECK_EQUAL(2, pCb->currentIndex()); 
		CHECK_EQUAL(LOOKUP_NAME2, pCb->currentText().toLocal8Bit().constData());
		matcher.LeftToRight();
		CHECK_EQUAL(ITEM_TEXT[2], accPropTblYIt.getString(DEF_COLTEXT));

		guiPropTblYIt.setCurrentString(DEF_COLTEXT, LOOKUP_NAME3);
		CHECK_EQUAL(3, pCb->currentIndex()); 
		CHECK_EQUAL(LOOKUP_NAME3, pCb->currentText().toLocal8Bit().constData());
		matcher.LeftToRight();
		CHECK_EQUAL(ITEM_TEXT[3], accPropTblYIt.getString(DEF_COLTEXT));

		guiPropTblYIt.setCurrentString(DEF_COLTEXT, LOOKUP_NAME4);
		CHECK_EQUAL(4, pCb->currentIndex()); 
		CHECK_EQUAL(LOOKUP_NAME4, pCb->currentText().toLocal8Bit().constData());
		matcher.LeftToRight();
		CHECK_EQUAL(ITEM_TEXT[4], accPropTblYIt.getString(DEF_COLTEXT));
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

	CHECK_EQUAL(LOOKUP_NAME0, pCb->itemText(0).toLocal8Bit().constData());
	CHECK_EQUAL(LOOKUP_NAME1, pCb->itemText(1).toLocal8Bit().constData());
	CHECK_EQUAL(LOOKUP_NAME2, pCb->itemText(2).toLocal8Bit().constData());
	CHECK_EQUAL(LOOKUP_NAME3, pCb->itemText(3).toLocal8Bit().constData());
	CHECK_EQUAL(LOOKUP_NAME4, pCb->itemText(4).toLocal8Bit().constData());
}

// -----------------------------------------------------------------------------
// no key-value-mapping, DB columname = widget name, transformer with name equality
TEST_FIXTURE(FxMatcherNoMapping, MatcherCbB_Transformed_RL)
{
#ifdef ACTUAL_BEHAVIOUR
	const int   CURRENT_INDEX[] = { 0, 0, 0, 0, 0 };
	ConstString ITEM_TEXT       = LOOKUP_NAME4;
#else
	const int   CURRENT_INDEX[] = { 0, 1, 2, 3, 4 };
	ConstString ITEM_TEXT       = LOOKUP_NAME0;
#endif 

	CHECK_EQUAL(1, (int)accPropTbl.size());
    CHECK_EQUAL(4, (int)accPropTbl.getPropertyDescriptionList().size());

    CHECK_EQUAL(5, (int)guiPropTbl.size());
    CHECK_EQUAL(2, (int)guiPropTbl.getPropertyDescriptionList().size());

	CHECK_EQUAL(LOOKUP_NAME0, pCb->itemText(0).toLocal8Bit().constData());
	CHECK_EQUAL(LOOKUP_NAME1, pCb->itemText(1).toLocal8Bit().constData());
	CHECK_EQUAL(LOOKUP_NAME2, pCb->itemText(2).toLocal8Bit().constData());
	CHECK_EQUAL(LOOKUP_NAME3, pCb->itemText(3).toLocal8Bit().constData());
	CHECK_EQUAL(LOOKUP_NAME4, pCb->itemText(4).toLocal8Bit().constData());

	try
	{
		CurrentListTransformer tf (DEF_COLTEXT, DEF_COLTEXT);
		matcher.push_back(tf);

		accPropTblYIt.setString(DEF_COLTEXT , LOOKUP_NAME0);
		matcher.RightToLeft();		
		CHECK_EQUAL(CURRENT_INDEX[0], pCb->currentIndex()); 
		CHECK_EQUAL(LOOKUP_NAME0, pCb->currentText().toLocal8Bit().constData());
		CHECK_EQUAL(LOOKUP_NAME0, guiPropTblYIt.getCurrentString(DEF_COLTEXT).c_str());

		accPropTblYIt.setString(DEF_COLTEXT , LOOKUP_NAME1);
		matcher.RightToLeft();		
		CHECK_EQUAL(CURRENT_INDEX[1], pCb->currentIndex()); 
		CHECK_EQUAL(LOOKUP_NAME1, pCb->currentText().toLocal8Bit().constData());
		CHECK_EQUAL(LOOKUP_NAME1, guiPropTblYIt.getCurrentString(DEF_COLTEXT).c_str());

		accPropTblYIt.setString(DEF_COLTEXT , LOOKUP_NAME2);
		matcher.RightToLeft();		
		CHECK_EQUAL(CURRENT_INDEX[2], pCb->currentIndex()); 
		CHECK_EQUAL(LOOKUP_NAME2, pCb->currentText().toLocal8Bit().constData());
		CHECK_EQUAL(LOOKUP_NAME2, guiPropTblYIt.getCurrentString(DEF_COLTEXT).c_str());

		accPropTblYIt.setString(DEF_COLTEXT , LOOKUP_NAME3);
		matcher.RightToLeft();		
		CHECK_EQUAL(CURRENT_INDEX[3], pCb->currentIndex()); 
		CHECK_EQUAL(LOOKUP_NAME3, pCb->currentText().toLocal8Bit().constData());
		CHECK_EQUAL(LOOKUP_NAME3, guiPropTblYIt.getCurrentString(DEF_COLTEXT).c_str());

		accPropTblYIt.setString(DEF_COLTEXT , LOOKUP_NAME4);
		matcher.RightToLeft();		
		CHECK_EQUAL(CURRENT_INDEX[4], pCb->currentIndex()); 
		CHECK_EQUAL(LOOKUP_NAME4, pCb->currentText().toLocal8Bit().constData());
		CHECK_EQUAL(LOOKUP_NAME4, guiPropTblYIt.getCurrentString(DEF_COLTEXT).c_str());
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

	CHECK_EQUAL(ITEM_TEXT   , pCb->itemText(0).toLocal8Bit().constData());
	CHECK_EQUAL(LOOKUP_NAME1, pCb->itemText(1).toLocal8Bit().constData());
	CHECK_EQUAL(LOOKUP_NAME2, pCb->itemText(2).toLocal8Bit().constData());
	CHECK_EQUAL(LOOKUP_NAME3, pCb->itemText(3).toLocal8Bit().constData());
	CHECK_EQUAL(LOOKUP_NAME4, pCb->itemText(4).toLocal8Bit().constData());
}

//------------------------------------------------------------------------------
// no key-value-mapping, DB columname = widget name, transformer with name equality
TEST_FIXTURE(FxMatcherNoMapping, MatcherCbB_Transformed_LR)
{
	CHECK_EQUAL(1, (int)accPropTbl.size());
    CHECK_EQUAL(4, (int)accPropTbl.getPropertyDescriptionList().size());

    CHECK_EQUAL(5, (int)guiPropTbl.size());
    CHECK_EQUAL(2, (int)guiPropTbl.getPropertyDescriptionList().size());

	CHECK_EQUAL(LOOKUP_NAME0, pCb->itemText(0).toLocal8Bit().constData());
	CHECK_EQUAL(LOOKUP_NAME1, pCb->itemText(1).toLocal8Bit().constData());
	CHECK_EQUAL(LOOKUP_NAME2, pCb->itemText(2).toLocal8Bit().constData());
	CHECK_EQUAL(LOOKUP_NAME3, pCb->itemText(3).toLocal8Bit().constData());
	CHECK_EQUAL(LOOKUP_NAME4, pCb->itemText(4).toLocal8Bit().constData());

	try
	{
		CurrentListTransformer tf (DEF_COLTEXT, DEF_COLTEXT);
		matcher.push_back(tf);

		guiPropTblYIt.setCurrentString(DEF_COLTEXT, LOOKUP_NAME0);
		CHECK_EQUAL(0, pCb->currentIndex()); 
		CHECK_EQUAL(LOOKUP_NAME0, pCb->currentText().toLocal8Bit().constData());
		matcher.LeftToRight();
		CHECK_EQUAL(LOOKUP_NAME0, accPropTblYIt.getString(DEF_COLTEXT));

		guiPropTblYIt.setCurrentString(DEF_COLTEXT, LOOKUP_NAME1);
		CHECK_EQUAL(1, pCb->currentIndex()); 
		CHECK_EQUAL(LOOKUP_NAME1, pCb->currentText().toLocal8Bit().constData());
		matcher.LeftToRight();
		CHECK_EQUAL(LOOKUP_NAME1, accPropTblYIt.getString(DEF_COLTEXT));

		guiPropTblYIt.setCurrentString(DEF_COLTEXT, LOOKUP_NAME2);
		CHECK_EQUAL(2, pCb->currentIndex()); 
		CHECK_EQUAL(LOOKUP_NAME2, pCb->currentText().toLocal8Bit().constData());
		matcher.LeftToRight();
		CHECK_EQUAL(LOOKUP_NAME2, accPropTblYIt.getString(DEF_COLTEXT));

		guiPropTblYIt.setCurrentString(DEF_COLTEXT, LOOKUP_NAME3);
		CHECK_EQUAL(3, pCb->currentIndex()); 
		CHECK_EQUAL(LOOKUP_NAME3, pCb->currentText().toLocal8Bit().constData());
		matcher.LeftToRight();
		CHECK_EQUAL(LOOKUP_NAME3, accPropTblYIt.getString(DEF_COLTEXT));

		guiPropTblYIt.setCurrentString(DEF_COLTEXT, LOOKUP_NAME4);
		CHECK_EQUAL(4, pCb->currentIndex()); 
		CHECK_EQUAL(LOOKUP_NAME4, pCb->currentText().toLocal8Bit().constData());
		matcher.LeftToRight();
		CHECK_EQUAL(LOOKUP_NAME4, accPropTblYIt.getString(DEF_COLTEXT));
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

	CHECK_EQUAL(LOOKUP_NAME0, pCb->itemText(0).toLocal8Bit().constData());
	CHECK_EQUAL(LOOKUP_NAME1, pCb->itemText(1).toLocal8Bit().constData());
	CHECK_EQUAL(LOOKUP_NAME2, pCb->itemText(2).toLocal8Bit().constData());
	CHECK_EQUAL(LOOKUP_NAME3, pCb->itemText(3).toLocal8Bit().constData());
	CHECK_EQUAL(LOOKUP_NAME4, pCb->itemText(4).toLocal8Bit().constData());
}

// -----------------------------------------------------------------------------
// no key-value-mapping, DB columname != widget name, transformer without name equality
TEST_FIXTURE(FxMatcherNoMappingDiffColNames, MatcherCbB_Transformed_RL_DiffColNames)
{
	CHECK_EQUAL(1, (int)accPropTbl.size());
    CHECK_EQUAL(4, (int)accPropTbl.getPropertyDescriptionList().size());

    CHECK_EQUAL(5, (int)guiPropTbl.size());
    CHECK_EQUAL(2, (int)guiPropTbl.getPropertyDescriptionList().size());

	CHECK_EQUAL(LOOKUP_NAME0, pCb->itemText(0).toLocal8Bit().constData());
	CHECK_EQUAL(LOOKUP_NAME1, pCb->itemText(1).toLocal8Bit().constData());
	CHECK_EQUAL(LOOKUP_NAME2, pCb->itemText(2).toLocal8Bit().constData());
	CHECK_EQUAL(LOOKUP_NAME3, pCb->itemText(3).toLocal8Bit().constData());
	CHECK_EQUAL(LOOKUP_NAME4, pCb->itemText(4).toLocal8Bit().constData());

	try
	{
		CurrentListTransformer tf (DEF_COLTEXT, DEF_COLTEXT2);
		matcher.push_back(tf);

		accPropTblYIt.setString(DEF_COLTEXT2, LOOKUP_NAME0);
		matcher.RightToLeft();		
		CHECK_EQUAL(0, pCb->currentIndex()); 
		CHECK_EQUAL(LOOKUP_NAME0, pCb->currentText().toLocal8Bit().constData());
		CHECK_EQUAL(LOOKUP_NAME0, guiPropTblYIt.getCurrentString(DEF_COLTEXT).c_str());

		accPropTblYIt.setString(DEF_COLTEXT2 , LOOKUP_NAME1);
		matcher.RightToLeft();		
		CHECK_EQUAL(1, pCb->currentIndex()); 
		CHECK_EQUAL(LOOKUP_NAME1, pCb->currentText().toLocal8Bit().constData());
		CHECK_EQUAL(LOOKUP_NAME1, guiPropTblYIt.getCurrentString(DEF_COLTEXT).c_str());

		accPropTblYIt.setString(DEF_COLTEXT2 , LOOKUP_NAME2);
		matcher.RightToLeft();		
		CHECK_EQUAL(2, pCb->currentIndex()); 
		CHECK_EQUAL(LOOKUP_NAME2, pCb->currentText().toLocal8Bit().constData());
		CHECK_EQUAL(LOOKUP_NAME2, guiPropTblYIt.getCurrentString(DEF_COLTEXT).c_str());

		accPropTblYIt.setString(DEF_COLTEXT2 , LOOKUP_NAME3);
		matcher.RightToLeft();		
		CHECK_EQUAL(3, pCb->currentIndex()); 
		CHECK_EQUAL(LOOKUP_NAME3, pCb->currentText().toLocal8Bit().constData());
		CHECK_EQUAL(LOOKUP_NAME3, guiPropTblYIt.getCurrentString(DEF_COLTEXT).c_str());

		accPropTblYIt.setString(DEF_COLTEXT2 , LOOKUP_NAME4);
		matcher.RightToLeft();		
		CHECK_EQUAL(4, pCb->currentIndex()); 
		CHECK_EQUAL(LOOKUP_NAME4, pCb->currentText().toLocal8Bit().constData());
		CHECK_EQUAL(LOOKUP_NAME4, guiPropTblYIt.getCurrentString(DEF_COLTEXT).c_str());
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

	CHECK_EQUAL(LOOKUP_NAME0, pCb->itemText(0).toLocal8Bit().constData());
	CHECK_EQUAL(LOOKUP_NAME1, pCb->itemText(1).toLocal8Bit().constData());
	CHECK_EQUAL(LOOKUP_NAME2, pCb->itemText(2).toLocal8Bit().constData());
	CHECK_EQUAL(LOOKUP_NAME3, pCb->itemText(3).toLocal8Bit().constData());
	CHECK_EQUAL(LOOKUP_NAME4, pCb->itemText(4).toLocal8Bit().constData());
}

//------------------------------------------------------------------------------
// no key-value-mapping, DB columname != widget name, transformer without name equality
TEST_FIXTURE(FxMatcherNoMappingDiffColNames, MatcherCbB_Transformed_LR_DiffColNames)
{
	CHECK_EQUAL(1, (int)accPropTbl.size());
    CHECK_EQUAL(4, (int)accPropTbl.getPropertyDescriptionList().size());

    CHECK_EQUAL(5, (int)guiPropTbl.size());
    CHECK_EQUAL(2, (int)guiPropTbl.getPropertyDescriptionList().size());

	CHECK_EQUAL(LOOKUP_NAME0, pCb->itemText(0).toLocal8Bit().constData());
	CHECK_EQUAL(LOOKUP_NAME1, pCb->itemText(1).toLocal8Bit().constData());
	CHECK_EQUAL(LOOKUP_NAME2, pCb->itemText(2).toLocal8Bit().constData());
	CHECK_EQUAL(LOOKUP_NAME3, pCb->itemText(3).toLocal8Bit().constData());
	CHECK_EQUAL(LOOKUP_NAME4, pCb->itemText(4).toLocal8Bit().constData());

	try
	{
		CurrentListTransformer tf (DEF_COLTEXT, DEF_COLTEXT2);
		matcher.push_back(tf);

		guiPropTblYIt.setCurrentString(DEF_COLTEXT, LOOKUP_NAME0);
		CHECK_EQUAL(0, pCb->currentIndex()); 
		CHECK_EQUAL(LOOKUP_NAME0, pCb->currentText().toLocal8Bit().constData());
		matcher.LeftToRight();
		CHECK_EQUAL(LOOKUP_NAME0, accPropTblYIt.getString(DEF_COLTEXT2));

		guiPropTblYIt.setCurrentString(DEF_COLTEXT, LOOKUP_NAME1);
		CHECK_EQUAL(1, pCb->currentIndex()); 
		CHECK_EQUAL(LOOKUP_NAME1, pCb->currentText().toLocal8Bit().constData());
		matcher.LeftToRight();
		CHECK_EQUAL(LOOKUP_NAME1, accPropTblYIt.getString(DEF_COLTEXT2));

		guiPropTblYIt.setCurrentString(DEF_COLTEXT, LOOKUP_NAME2);
		CHECK_EQUAL(2, pCb->currentIndex()); 
		CHECK_EQUAL(LOOKUP_NAME2, pCb->currentText().toLocal8Bit().constData());
		matcher.LeftToRight();
		CHECK_EQUAL(LOOKUP_NAME2, accPropTblYIt.getString(DEF_COLTEXT2));

		guiPropTblYIt.setCurrentString(DEF_COLTEXT, LOOKUP_NAME3);
		CHECK_EQUAL(3, pCb->currentIndex()); 
		CHECK_EQUAL(LOOKUP_NAME3, pCb->currentText().toLocal8Bit().constData());
		matcher.LeftToRight();
		CHECK_EQUAL(LOOKUP_NAME3, accPropTblYIt.getString(DEF_COLTEXT2));

		guiPropTblYIt.setCurrentString(DEF_COLTEXT, LOOKUP_NAME4);
		CHECK_EQUAL(4, pCb->currentIndex()); 
		CHECK_EQUAL(LOOKUP_NAME4, pCb->currentText().toLocal8Bit().constData());
		matcher.LeftToRight();
		CHECK_EQUAL(LOOKUP_NAME4, accPropTblYIt.getString(DEF_COLTEXT2));
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

	CHECK_EQUAL(LOOKUP_NAME0, pCb->itemText(0).toLocal8Bit().constData());
	CHECK_EQUAL(LOOKUP_NAME1, pCb->itemText(1).toLocal8Bit().constData());
	CHECK_EQUAL(LOOKUP_NAME2, pCb->itemText(2).toLocal8Bit().constData());
	CHECK_EQUAL(LOOKUP_NAME3, pCb->itemText(3).toLocal8Bit().constData());
	CHECK_EQUAL(LOOKUP_NAME4, pCb->itemText(4).toLocal8Bit().constData());
}

// -----------------------------------------------------------------------------
// key-value-mapping, DB columname != widget name, no transformer
TEST_FIXTURE(FxMatcherMapping, MatcherCbB_Mapped_RL)
{
	CHECK_EQUAL(1, (int)accPropTbl.size());
    CHECK_EQUAL(4, (int)accPropTbl.getPropertyDescriptionList().size());

    CHECK_EQUAL(5, (int)guiPropTbl.size());
    CHECK_EQUAL(2, (int)guiPropTbl.getPropertyDescriptionList().size());

	CHECK_EQUAL(LOOKUP_NAME0, pCb->itemText(0).toLocal8Bit().constData());
	CHECK_EQUAL(LOOKUP_NAME1, pCb->itemText(1).toLocal8Bit().constData());
	CHECK_EQUAL(LOOKUP_NAME2, pCb->itemText(2).toLocal8Bit().constData());
	CHECK_EQUAL(LOOKUP_NAME3, pCb->itemText(3).toLocal8Bit().constData());
	CHECK_EQUAL(LOOKUP_NAME4, pCb->itemText(4).toLocal8Bit().constData());

	try
	{
		accPropTblYIt.setInt32 (DEF_COLKEY , LOOKUP_KEY0);
		matcher.RightToLeft();		
		CHECK_EQUAL(0, pCb->currentIndex()); 
		CHECK_EQUAL(LOOKUP_NAME0, pCb->currentText().toLocal8Bit().constData());
		CHECK_EQUAL(LOOKUP_NAME0, guiPropTblYIt.getCurrentString(DEF_COLTEXT).c_str());

		accPropTblYIt.setInt32 (DEF_COLKEY , LOOKUP_KEY1);
		matcher.RightToLeft();		
		CHECK_EQUAL(0, pCb->currentIndex()); 
		CHECK_EQUAL(LOOKUP_NAME0, pCb->currentText().toLocal8Bit().constData());
		CHECK_EQUAL(LOOKUP_NAME0, guiPropTblYIt.getCurrentString(DEF_COLTEXT).c_str());

		accPropTblYIt.setInt32 (DEF_COLKEY , LOOKUP_KEY2);
		matcher.RightToLeft();		
		CHECK_EQUAL(0, pCb->currentIndex()); 
		CHECK_EQUAL(LOOKUP_NAME0, pCb->currentText().toLocal8Bit().constData());
		CHECK_EQUAL(LOOKUP_NAME0, guiPropTblYIt.getCurrentString(DEF_COLTEXT).c_str());

		accPropTblYIt.setInt32 (DEF_COLKEY , LOOKUP_KEY3);
		matcher.RightToLeft();		
		CHECK_EQUAL(0, pCb->currentIndex()); 
		CHECK_EQUAL(LOOKUP_NAME0, pCb->currentText().toLocal8Bit().constData());
		CHECK_EQUAL(LOOKUP_NAME0, guiPropTblYIt.getCurrentString(DEF_COLTEXT).c_str());

		accPropTblYIt.setInt32 (DEF_COLKEY , LOOKUP_KEY4);
		matcher.RightToLeft();		
		CHECK_EQUAL(0, pCb->currentIndex()); 
		CHECK_EQUAL(LOOKUP_NAME0, pCb->currentText().toLocal8Bit().constData());
		CHECK_EQUAL(LOOKUP_NAME0, guiPropTblYIt.getCurrentString(DEF_COLTEXT).c_str());
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

	CHECK_EQUAL(LOOKUP_NAME0, pCb->itemText(0).toLocal8Bit().constData());
	CHECK_EQUAL(LOOKUP_NAME1, pCb->itemText(1).toLocal8Bit().constData());
	CHECK_EQUAL(LOOKUP_NAME2, pCb->itemText(2).toLocal8Bit().constData());
	CHECK_EQUAL(LOOKUP_NAME3, pCb->itemText(3).toLocal8Bit().constData());
	CHECK_EQUAL(LOOKUP_NAME4, pCb->itemText(4).toLocal8Bit().constData());
}

//------------------------------------------------------------------------------
// key-value-mapping, DB columname != widget name, no transformer
TEST_FIXTURE(FxMatcherMapping, MatcherCbB_Mapped_LR)
{
	CHECK_EQUAL(1, (int)accPropTbl.size());
    CHECK_EQUAL(4, (int)accPropTbl.getPropertyDescriptionList().size());

    CHECK_EQUAL(5, (int)guiPropTbl.size());
    CHECK_EQUAL(2, (int)guiPropTbl.getPropertyDescriptionList().size());

	CHECK_EQUAL(LOOKUP_NAME0, pCb->itemText(0).toLocal8Bit().constData());
	CHECK_EQUAL(LOOKUP_NAME1, pCb->itemText(1).toLocal8Bit().constData());
	CHECK_EQUAL(LOOKUP_NAME2, pCb->itemText(2).toLocal8Bit().constData());
	CHECK_EQUAL(LOOKUP_NAME3, pCb->itemText(3).toLocal8Bit().constData());
	CHECK_EQUAL(LOOKUP_NAME4, pCb->itemText(4).toLocal8Bit().constData());

	try
	{
		guiPropTblYIt.setCurrentString(DEF_COLTEXT, LOOKUP_NAME0);
		CHECK_EQUAL(0, pCb->currentIndex()); 
		CHECK_EQUAL(LOOKUP_NAME0, pCb->currentText().toLocal8Bit().constData());
		matcher.LeftToRight();
		CHECK_THROW(accPropTblYIt.getInt32(DEF_COLKEY), Exception);

		guiPropTblYIt.setCurrentString(DEF_COLTEXT, LOOKUP_NAME1);
		CHECK_EQUAL(1, pCb->currentIndex()); 
		CHECK_EQUAL(LOOKUP_NAME1, pCb->currentText().toLocal8Bit().constData());
		matcher.LeftToRight();
		CHECK_THROW(accPropTblYIt.getInt32(DEF_COLKEY), Exception);

		guiPropTblYIt.setCurrentString(DEF_COLTEXT, LOOKUP_NAME2);
		CHECK_EQUAL(2, pCb->currentIndex()); 
		CHECK_EQUAL(LOOKUP_NAME2, pCb->currentText().toLocal8Bit().constData());
		matcher.LeftToRight();
		CHECK_THROW(accPropTblYIt.getInt32(DEF_COLKEY), Exception);

		guiPropTblYIt.setCurrentString(DEF_COLTEXT, LOOKUP_NAME3);
		CHECK_EQUAL(3, pCb->currentIndex()); 
		CHECK_EQUAL(LOOKUP_NAME3, pCb->currentText().toLocal8Bit().constData());
		matcher.LeftToRight();
		CHECK_THROW(accPropTblYIt.getInt32(DEF_COLKEY), Exception);

		guiPropTblYIt.setCurrentString(DEF_COLTEXT, LOOKUP_NAME4);
		CHECK_EQUAL(4, pCb->currentIndex()); 
		CHECK_EQUAL(LOOKUP_NAME4, pCb->currentText().toLocal8Bit().constData());
		matcher.LeftToRight();
		CHECK_THROW(accPropTblYIt.getInt32(DEF_COLKEY), Exception);
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

	CHECK_EQUAL(LOOKUP_NAME0, pCb->itemText(0).toLocal8Bit().constData());
	CHECK_EQUAL(LOOKUP_NAME1, pCb->itemText(1).toLocal8Bit().constData());
	CHECK_EQUAL(LOOKUP_NAME2, pCb->itemText(2).toLocal8Bit().constData());
	CHECK_EQUAL(LOOKUP_NAME3, pCb->itemText(3).toLocal8Bit().constData());
	CHECK_EQUAL(LOOKUP_NAME4, pCb->itemText(4).toLocal8Bit().constData());
}

// -----------------------------------------------------------------------------
// key-value-mapping, DB columname != widget name, mapped transformer without name equality
TEST_FIXTURE(FxMatcherMapping, MatcherCbB_Mapped_Transformed_RL)
{
	CHECK_EQUAL(1, (int)accPropTbl.size());
    CHECK_EQUAL(4, (int)accPropTbl.getPropertyDescriptionList().size());

    CHECK_EQUAL(5, (int)guiPropTbl.size());
    CHECK_EQUAL(2, (int)guiPropTbl.getPropertyDescriptionList().size());

	CHECK_EQUAL(LOOKUP_NAME0, pCb->itemText(0).toLocal8Bit().constData());
	CHECK_EQUAL(LOOKUP_NAME1, pCb->itemText(1).toLocal8Bit().constData());
	CHECK_EQUAL(LOOKUP_NAME2, pCb->itemText(2).toLocal8Bit().constData());
	CHECK_EQUAL(LOOKUP_NAME3, pCb->itemText(3).toLocal8Bit().constData());
	CHECK_EQUAL(LOOKUP_NAME4, pCb->itemText(4).toLocal8Bit().constData());

	try
	{
		CurrentListTransformer tf (DEF_COLTEXT, 
			                       DEF_COLKEY,
								   DEF_COLKEY,
								   DEF_COLTEXT,
								   accPropTblLookup); 
		matcher.push_back(tf);

		accPropTblYIt.setInt32 (DEF_COLKEY , LOOKUP_KEY0);
		matcher.RightToLeft();		
		CHECK_EQUAL(0, pCb->currentIndex()); 
		CHECK_EQUAL(LOOKUP_NAME0, pCb->currentText().toLocal8Bit().constData());
		CHECK_EQUAL(LOOKUP_NAME0, guiPropTblYIt.getCurrentString(DEF_COLTEXT).c_str());

		accPropTblYIt.setInt32 (DEF_COLKEY , LOOKUP_KEY1);
		matcher.RightToLeft();		
		CHECK_EQUAL(1, pCb->currentIndex()); 
		CHECK_EQUAL(LOOKUP_NAME1, pCb->currentText().toLocal8Bit().constData());
		CHECK_EQUAL(LOOKUP_NAME1, guiPropTblYIt.getCurrentString(DEF_COLTEXT).c_str());

		accPropTblYIt.setInt32 (DEF_COLKEY , LOOKUP_KEY2);
		matcher.RightToLeft();		
		CHECK_EQUAL(2, pCb->currentIndex()); 
		CHECK_EQUAL(LOOKUP_NAME2, pCb->currentText().toLocal8Bit().constData());
		CHECK_EQUAL(LOOKUP_NAME2, guiPropTblYIt.getCurrentString(DEF_COLTEXT).c_str());

		accPropTblYIt.setInt32 (DEF_COLKEY , LOOKUP_KEY3);
		matcher.RightToLeft();		
		CHECK_EQUAL(3, pCb->currentIndex()); 
		CHECK_EQUAL(LOOKUP_NAME3, pCb->currentText().toLocal8Bit().constData());
		CHECK_EQUAL(LOOKUP_NAME3, guiPropTblYIt.getCurrentString(DEF_COLTEXT).c_str());

		accPropTblYIt.setInt32 (DEF_COLKEY , LOOKUP_KEY4);
		matcher.RightToLeft();		
		CHECK_EQUAL(4, pCb->currentIndex()); 
		CHECK_EQUAL(LOOKUP_NAME4, pCb->currentText().toLocal8Bit().constData());
		CHECK_EQUAL(LOOKUP_NAME4, guiPropTblYIt.getCurrentString(DEF_COLTEXT).c_str());
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

	CHECK_EQUAL(LOOKUP_NAME0, pCb->itemText(0).toLocal8Bit().constData());
	CHECK_EQUAL(LOOKUP_NAME1, pCb->itemText(1).toLocal8Bit().constData());
	CHECK_EQUAL(LOOKUP_NAME2, pCb->itemText(2).toLocal8Bit().constData());
	CHECK_EQUAL(LOOKUP_NAME3, pCb->itemText(3).toLocal8Bit().constData());
	CHECK_EQUAL(LOOKUP_NAME4, pCb->itemText(4).toLocal8Bit().constData());
}

//------------------------------------------------------------------------------
// key-value-mapping, DB columname != widget name, mapped transformer without name equality
TEST_FIXTURE(FxMatcherMapping, MatcherCbB_Mapped_Transformed_LR)
{
	CHECK_EQUAL(1, (int)accPropTbl.size());
    CHECK_EQUAL(4, (int)accPropTbl.getPropertyDescriptionList().size());

    CHECK_EQUAL(5, (int)guiPropTbl.size());
    CHECK_EQUAL(2, (int)guiPropTbl.getPropertyDescriptionList().size());

	CHECK_EQUAL(LOOKUP_NAME0, pCb->itemText(0).toLocal8Bit().constData());
	CHECK_EQUAL(LOOKUP_NAME1, pCb->itemText(1).toLocal8Bit().constData());
	CHECK_EQUAL(LOOKUP_NAME2, pCb->itemText(2).toLocal8Bit().constData());
	CHECK_EQUAL(LOOKUP_NAME3, pCb->itemText(3).toLocal8Bit().constData());
	CHECK_EQUAL(LOOKUP_NAME4, pCb->itemText(4).toLocal8Bit().constData());

	try
	{
		CurrentListTransformer tf (DEF_COLTEXT, 
			                       DEF_COLKEY,
								   DEF_COLKEY,
								   DEF_COLTEXT,
								   accPropTblLookup); 
		matcher.push_back(tf);

		guiPropTblYIt.setCurrentString(DEF_COLTEXT, LOOKUP_NAME0);
		CHECK_EQUAL(0, pCb->currentIndex()); 
		CHECK_EQUAL(LOOKUP_NAME0, pCb->currentText().toLocal8Bit().constData());
		matcher.LeftToRight();
		CHECK_EQUAL(LOOKUP_KEY0, accPropTblYIt.getInt32(DEF_COLKEY));

		guiPropTblYIt.setCurrentString(DEF_COLTEXT, LOOKUP_NAME1);
		CHECK_EQUAL(1, pCb->currentIndex()); 
		CHECK_EQUAL(LOOKUP_NAME1, pCb->currentText().toLocal8Bit().constData());
		matcher.LeftToRight();
		CHECK_EQUAL(LOOKUP_KEY1, accPropTblYIt.getInt32(DEF_COLKEY));

		guiPropTblYIt.setCurrentString(DEF_COLTEXT, LOOKUP_NAME2);
		CHECK_EQUAL(2, pCb->currentIndex()); 
		CHECK_EQUAL(LOOKUP_NAME2, pCb->currentText().toLocal8Bit().constData());
		matcher.LeftToRight();
		CHECK_EQUAL(LOOKUP_KEY2, accPropTblYIt.getInt32(DEF_COLKEY));

		guiPropTblYIt.setCurrentString(DEF_COLTEXT, LOOKUP_NAME3);
		CHECK_EQUAL(3, pCb->currentIndex()); 
		CHECK_EQUAL(LOOKUP_NAME3, pCb->currentText().toLocal8Bit().constData());
		matcher.LeftToRight();
		CHECK_EQUAL(LOOKUP_KEY3, accPropTblYIt.getInt32(DEF_COLKEY));

		guiPropTblYIt.setCurrentString(DEF_COLTEXT, LOOKUP_NAME4);
		CHECK_EQUAL(4, pCb->currentIndex()); 
		CHECK_EQUAL(LOOKUP_NAME4, pCb->currentText().toLocal8Bit().constData());
		matcher.LeftToRight();
		CHECK_EQUAL(LOOKUP_KEY4, accPropTblYIt.getInt32(DEF_COLKEY));
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

	CHECK_EQUAL(LOOKUP_NAME0, pCb->itemText(0).toLocal8Bit().constData());
	CHECK_EQUAL(LOOKUP_NAME1, pCb->itemText(1).toLocal8Bit().constData());
	CHECK_EQUAL(LOOKUP_NAME2, pCb->itemText(2).toLocal8Bit().constData());
	CHECK_EQUAL(LOOKUP_NAME3, pCb->itemText(3).toLocal8Bit().constData());
	CHECK_EQUAL(LOOKUP_NAME4, pCb->itemText(4).toLocal8Bit().constData());
}

//------------------------------------------------------------------------------
}	// SUITE

