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

#pragma warning (push)
#pragma warning(disable: 4481)
#include <QtWidgets/QComboBox>
#pragma warning(pop)

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
using tie::CurrentListTransformer;

//------------------------------------------------------------------------------
SUITE(TransformCurrentList)
{
	//--------------------------------------------------------------------------
	#define DEF_PROPTBLCOL0			 "PropTblCol0"

	#define DEF_PROPTBLCOL_VAL0      "abcdefgh"

	#define DEF_CB_ITEM0			 "xyz01234"
	#define DEF_CB_ITEM1			 "ABCDEFGH"
	#define DEF_CB_ITEM2			 DEF_PROPTBLCOL_VAL0
	#define DEF_CB_ITEM3             "hddh ssiwiwi wkwkwk"

	#define DEF_WIDGET0              "widget0"

	//--------------------------------------------------------------------------
	static ConstString ACCDEF        = "TransformCurrentListAccDefName";
	static ConstString ACCINST       = "TransformCurrentListAccInstName";

	//--------------------------------------------------------------------------
	BEGIN_ACCESSOR_DEFINITION(ACCDEF)
		PROPERTY_DESCRIPTION_LIST("string dummy0;"
								  "int32  dummy1;"
								  "string " DEF_PROPTBLCOL0 ";"
								  "date   dummy2;")
	END_ACCESSOR_DEFINITION

	//--------------------------------------------------------------------------
	class FxTransform
	{
	public:
		AccessorPropertyTable_YIterator accPropTblYIt;
		GuiPropertyTable_YIterator      guiPropTblYIt;
		QComboBox                     * pCombo;

		FxTransform()
		{
			accInst       = aspect::Manager::getInstance().createAccessorInstance(ACCINST,
																		          ACCDEF,
																				  ConnectionRef(), 
																				  FULL_CACHING);
			accPropTbl    = accInst.getPropertyTable();
			accPropTbl.insert(FOR_CLEAN);
			accPropTblYIt = accPropTbl.begin();

			pWidget = new QWidget(0);

			pCombo  = new QComboBox(pWidget);
			pCombo->setObjectName(DEF_WIDGET0);
			pCombo->addItem(DEF_CB_ITEM0);
			pCombo->addItem(DEF_CB_ITEM1);
			pCombo->addItem(DEF_CB_ITEM2);
			pCombo->addItem(DEF_CB_ITEM3);

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
	};

	//--------------------------------------------------------------------------
	#define DEF_PROPTBLLOOKUPCOLKEY	 "PropTblLookupColKey"
	#define DEF_PROPTBLLOOKUPCOLNAME "PropTblLookupColName"

	//--------------------------------------------------------------------------
	static ConstString LOOKUP_KEY0	 = "LookupKey0";
	static ConstString LOOKUP_KEY1	 = "LookupKey1";
	static ConstString LOOKUP_KEY2	 = "LookupKey2";
	static ConstString LOOKUP_KEY3	 = "LookupKey3";

	static ConstString LOOKUP_NAME0	 = "LookupName0";
	static ConstString LOOKUP_NAME1	 = "LookupName1";
	static ConstString LOOKUP_NAME2	 = "LookupName2";
	static ConstString LOOKUP_NAME3	 = "LookupName3";

	//--------------------------------------------------------------------------
	static ConstString ACCDEFLOOKUP  = "TransformCurrentListAccDefLookupName";
	static ConstString ACCINSTLOOKUP = "TransformCurrentListAccInstLookupName";

	//--------------------------------------------------------------------------
	namespace lookup
	{
		BEGIN_ACCESSOR_DEFINITION(ACCDEFLOOKUP)
			PROPERTY_DESCRIPTION_LIST("string " DEF_PROPTBLLOOKUPCOLKEY  "; "
									  "string " DEF_PROPTBLLOOKUPCOLNAME ";")
		END_ACCESSOR_DEFINITION
	}

	//--------------------------------------------------------------------------
	class FxTransformWithKey : public FxTransform
	{
	public:
		AccessorPropertyTableRef	accPropTblLookup;

		FxTransformWithKey()
		{
			accInstLookup    = aspect::Manager::getInstance().createAccessorInstance(ACCINSTLOOKUP,
																		             ACCDEFLOOKUP,
																				     ConnectionRef(), 
																				     FULL_CACHING);
			accPropTblLookup = accInstLookup.getPropertyTable();

			AccessorPropertyTable_YIterator yitLookup;

			yitLookup = accPropTblLookup.insert(FOR_CLEAN);
			yitLookup.setString(DEF_PROPTBLLOOKUPCOLKEY , LOOKUP_KEY0 );
			yitLookup.setString(DEF_PROPTBLLOOKUPCOLNAME, LOOKUP_NAME0);

			yitLookup = accPropTblLookup.insert(FOR_CLEAN);
			yitLookup.setString(DEF_PROPTBLLOOKUPCOLKEY , LOOKUP_KEY1 );
			yitLookup.setString(DEF_PROPTBLLOOKUPCOLNAME, LOOKUP_NAME1);

			yitLookup = accPropTblLookup.insert(FOR_CLEAN);
			yitLookup.setString(DEF_PROPTBLLOOKUPCOLKEY , LOOKUP_KEY2 );
			yitLookup.setString(DEF_PROPTBLLOOKUPCOLNAME, LOOKUP_NAME2);

			yitLookup = accPropTblLookup.insert(FOR_CLEAN);
			yitLookup.setString(DEF_PROPTBLLOOKUPCOLKEY , LOOKUP_KEY3 );
			yitLookup.setString(DEF_PROPTBLLOOKUPCOLNAME, LOOKUP_NAME3);

			pCombo->setItemText(0, LOOKUP_NAME0);
			pCombo->setItemText(1, LOOKUP_NAME1);
			pCombo->setItemText(2, LOOKUP_NAME2);
			pCombo->setItemText(3, LOOKUP_NAME3);
		}

	protected:
		AccessorInstanceRef         accInstLookup;
	};

	//--------------------------------------------------------------------------
	#define DEF_PROPTBLLOOKUPCOLKEY_SAME	 DEF_PROPTBLCOL0
	#define DEF_PROPTBLLOOKUPCOLNAME_SAME    "PropTblLookupColName"

	//--------------------------------------------------------------------------
	static ConstString ACCDEFLOOKUP_SAME  = "TransformCurrentListAccDefLookupSameName";
	static ConstString ACCINSTLOOKUP_SAME = "TransformCurrentListAccInstLookupSameName";

	//--------------------------------------------------------------------------
	namespace lookupSame
	{
		BEGIN_ACCESSOR_DEFINITION(ACCDEFLOOKUP_SAME)
			PROPERTY_DESCRIPTION_LIST("string " DEF_PROPTBLLOOKUPCOLKEY_SAME  "; "
									  "string " DEF_PROPTBLLOOKUPCOLNAME      ";")
		END_ACCESSOR_DEFINITION
	}

	//--------------------------------------------------------------------------
	class FxTransformWithKeySame : public FxTransform
	{
	public:
		AccessorPropertyTableRef	accPropTblLookup;

		FxTransformWithKeySame()
		{
			accInstLookup    = aspect::Manager::getInstance().createAccessorInstance(ACCINSTLOOKUP_SAME,
																		             ACCDEFLOOKUP_SAME,
																				     ConnectionRef(), 
																				     FULL_CACHING);
			accPropTblLookup = accInstLookup.getPropertyTable();

			AccessorPropertyTable_YIterator yitLookup;

			yitLookup = accPropTblLookup.insert(FOR_CLEAN);
			yitLookup.setString(DEF_PROPTBLLOOKUPCOLKEY_SAME , LOOKUP_KEY0 );
			yitLookup.setString(DEF_PROPTBLLOOKUPCOLNAME_SAME, LOOKUP_NAME0);

			yitLookup = accPropTblLookup.insert(FOR_CLEAN);
			yitLookup.setString(DEF_PROPTBLLOOKUPCOLKEY_SAME , LOOKUP_KEY1 );
			yitLookup.setString(DEF_PROPTBLLOOKUPCOLNAME_SAME, LOOKUP_NAME1);

			yitLookup = accPropTblLookup.insert(FOR_CLEAN);
			yitLookup.setString(DEF_PROPTBLLOOKUPCOLKEY_SAME , LOOKUP_KEY2 );
			yitLookup.setString(DEF_PROPTBLLOOKUPCOLNAME_SAME, LOOKUP_NAME2);

			yitLookup = accPropTblLookup.insert(FOR_CLEAN);
			yitLookup.setString(DEF_PROPTBLLOOKUPCOLKEY_SAME , LOOKUP_KEY3 );
			yitLookup.setString(DEF_PROPTBLLOOKUPCOLNAME_SAME, LOOKUP_NAME3);

			pCombo->setItemText(0, LOOKUP_NAME0);
			pCombo->setItemText(1, LOOKUP_NAME1);
			pCombo->setItemText(2, LOOKUP_NAME2);
			pCombo->setItemText(3, LOOKUP_NAME3);
		}

	protected:
		AccessorInstanceRef         accInstLookup;
	};

//------------------------------------------------------------------------------
TEST_FIXTURE(FxTransform, Ctor_NoKey_RL)
{
	try
	{
		CurrentListTransformer tf (DEF_WIDGET0, DEF_PROPTBLCOL0); 

		accPropTblYIt.setString(DEF_PROPTBLCOL0, DEF_PROPTBLCOL_VAL0);

		tf.RightToLeft(guiPropTblYIt, accPropTblYIt);

		CHECK_EQUAL(2, pCombo->currentIndex()); 
		CHECK_EQUAL(DEF_PROPTBLCOL_VAL0, pCombo->currentText().toLocal8Bit().constData());
		CHECK_EQUAL(DEF_PROPTBLCOL_VAL0, guiPropTblYIt.getCurrentString(DEF_WIDGET0).c_str());

		CHECK_EQUAL(DEF_CB_ITEM0, pCombo->itemText(0).toLocal8Bit().constData());
		CHECK_EQUAL(DEF_CB_ITEM1, pCombo->itemText(1).toLocal8Bit().constData());
		CHECK_EQUAL(DEF_CB_ITEM2, pCombo->itemText(2).toLocal8Bit().constData());
		CHECK_EQUAL(DEF_CB_ITEM3, pCombo->itemText(3).toLocal8Bit().constData());
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
TEST_FIXTURE(FxTransform, Ctor_NoKey_LR)
{
	try
	{
		CurrentListTransformer tf (DEF_WIDGET0, DEF_PROPTBLCOL0); 

		guiPropTblYIt.setCurrentString(DEF_WIDGET0, DEF_PROPTBLCOL_VAL0);
		CHECK_EQUAL(2, pCombo->currentIndex()); 
		CHECK_EQUAL(DEF_PROPTBLCOL_VAL0, pCombo->currentText().toLocal8Bit().constData());

		tf.LeftToRight(guiPropTblYIt, accPropTblYIt);

		CHECK_EQUAL(DEF_PROPTBLCOL_VAL0, accPropTblYIt.getString(DEF_PROPTBLCOL0).c_str());

		CHECK_EQUAL(DEF_CB_ITEM0, pCombo->itemText(0).toLocal8Bit().constData());
		CHECK_EQUAL(DEF_CB_ITEM1, pCombo->itemText(1).toLocal8Bit().constData());
		CHECK_EQUAL(DEF_CB_ITEM2, pCombo->itemText(2).toLocal8Bit().constData());
		CHECK_EQUAL(DEF_CB_ITEM3, pCombo->itemText(3).toLocal8Bit().constData());
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
TEST_FIXTURE(FxTransformWithKey, Ctor_WithKey_RL)
{
	try
	{
		CurrentListTransformer tf (DEF_WIDGET0, 
			                       DEF_PROPTBLCOL0,
								   DEF_PROPTBLLOOKUPCOLKEY,
								   DEF_PROPTBLLOOKUPCOLNAME,
								   accPropTblLookup); 

		accPropTblYIt.setString(DEF_PROPTBLCOL0, LOOKUP_KEY2);

		tf.RightToLeft(guiPropTblYIt, accPropTblYIt);

		CHECK_EQUAL(2, pCombo->currentIndex()); 
		CHECK_EQUAL(LOOKUP_NAME2, pCombo->currentText().toLocal8Bit().constData());
		CHECK_EQUAL(LOOKUP_NAME2, guiPropTblYIt.getCurrentString(DEF_WIDGET0).c_str());

		CHECK_EQUAL(LOOKUP_NAME0, pCombo->itemText(0).toLocal8Bit().constData());
		CHECK_EQUAL(LOOKUP_NAME1, pCombo->itemText(1).toLocal8Bit().constData());
		CHECK_EQUAL(LOOKUP_NAME2, pCombo->itemText(2).toLocal8Bit().constData());
		CHECK_EQUAL(LOOKUP_NAME3, pCombo->itemText(3).toLocal8Bit().constData());
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
TEST_FIXTURE(FxTransformWithKey, Ctor_WithKey_LR)
{
	try
	{
		CurrentListTransformer tf (DEF_WIDGET0, 
			                       DEF_PROPTBLCOL0,
								   DEF_PROPTBLLOOKUPCOLKEY,
								   DEF_PROPTBLLOOKUPCOLNAME,
								   accPropTblLookup); 

		guiPropTblYIt.setCurrentString(DEF_WIDGET0, LOOKUP_NAME2);
		CHECK_EQUAL(2, pCombo->currentIndex()); 
		CHECK_EQUAL(LOOKUP_NAME2, pCombo->currentText().toLocal8Bit().constData());

		tf.LeftToRight(guiPropTblYIt, accPropTblYIt);

		CHECK_EQUAL(LOOKUP_KEY2, accPropTblYIt.getString(DEF_PROPTBLCOL0).c_str());

		CHECK_EQUAL(LOOKUP_NAME0, pCombo->itemText(0).toLocal8Bit().constData());
		CHECK_EQUAL(LOOKUP_NAME1, pCombo->itemText(1).toLocal8Bit().constData());
		CHECK_EQUAL(LOOKUP_NAME2, pCombo->itemText(2).toLocal8Bit().constData());
		CHECK_EQUAL(LOOKUP_NAME3, pCombo->itemText(3).toLocal8Bit().constData());
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
TEST_FIXTURE(FxTransformWithKeySame, Ctor_WithSameKey_RL)
{
	try
	{
		CurrentListTransformer tf (DEF_WIDGET0, 
			                       DEF_PROPTBLCOL0,
								   DEF_PROPTBLLOOKUPCOLKEY_SAME,
								   DEF_PROPTBLLOOKUPCOLNAME_SAME,
								   accPropTblLookup); 

		accPropTblYIt.setString(DEF_PROPTBLCOL0, LOOKUP_KEY2);

		tf.RightToLeft(guiPropTblYIt, accPropTblYIt);

		CHECK_EQUAL(2, pCombo->currentIndex()); 
		CHECK_EQUAL(LOOKUP_NAME2, pCombo->currentText().toLocal8Bit().constData());
		CHECK_EQUAL(LOOKUP_NAME2, guiPropTblYIt.getCurrentString(DEF_WIDGET0).c_str());

		CHECK_EQUAL(LOOKUP_NAME0, pCombo->itemText(0).toLocal8Bit().constData());
		CHECK_EQUAL(LOOKUP_NAME1, pCombo->itemText(1).toLocal8Bit().constData());
		CHECK_EQUAL(LOOKUP_NAME2, pCombo->itemText(2).toLocal8Bit().constData());
		CHECK_EQUAL(LOOKUP_NAME3, pCombo->itemText(3).toLocal8Bit().constData());
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
TEST_FIXTURE(FxTransformWithKeySame, Ctor_WithSameKey_LR)
{
	try
	{
		CurrentListTransformer tf (DEF_WIDGET0, 
			                       DEF_PROPTBLCOL0,
								   DEF_PROPTBLLOOKUPCOLKEY_SAME,
								   DEF_PROPTBLLOOKUPCOLNAME_SAME,
								   accPropTblLookup); 

		guiPropTblYIt.setCurrentString(DEF_WIDGET0, LOOKUP_NAME2);
		CHECK_EQUAL(2, pCombo->currentIndex()); 
		CHECK_EQUAL(LOOKUP_NAME2, pCombo->currentText().toLocal8Bit().constData());

		tf.LeftToRight(guiPropTblYIt, accPropTblYIt);

		CHECK_EQUAL(LOOKUP_KEY2, accPropTblYIt.getString(DEF_PROPTBLCOL0).c_str());

		CHECK_EQUAL(LOOKUP_NAME0, pCombo->itemText(0).toLocal8Bit().constData());
		CHECK_EQUAL(LOOKUP_NAME1, pCombo->itemText(1).toLocal8Bit().constData());
		CHECK_EQUAL(LOOKUP_NAME2, pCombo->itemText(2).toLocal8Bit().constData());
		CHECK_EQUAL(LOOKUP_NAME3, pCombo->itemText(3).toLocal8Bit().constData());
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
