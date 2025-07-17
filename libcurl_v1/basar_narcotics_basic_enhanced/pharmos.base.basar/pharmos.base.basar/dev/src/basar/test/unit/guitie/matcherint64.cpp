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

#include <QtWidgets/QLabel>

SUITE(Matcher_Int64)
{

// -----------------------------------------------------------------------------
#define PROPTBLCOL				 "PropTblCol"

    //--------------------------------------------------------------------------
    static basar::ConstString GUITBLCOL     = "GuiTblCol";

    static basar::ConstString ACCDEF        = "GuiTieTest_Matcher_Int64AccDefName" ;
    static basar::ConstString ACCINST       = "GuiTieTest_Matcher_Int64AccInstName";

    //--------------------------------------------------------------------------
    BEGIN_ACCESSOR_DEFINITION(ACCDEF)
    PROPERTY_DESCRIPTION_LIST("string dummy0;"
                              "int32  dummy1;"
                              "int64 " PROPTBLCOL ";"
                              "date   dummy2;")
    END_ACCESSOR_DEFINITION

    //--------------------------------------------------------------------------
    class FxMatcher
    {
        public:
            basar::gui::tie::MatcherRef                      matcher;

            basar::db::aspect::AccessorPropertyTableRef	    accPropTbl;
            basar::db::aspect::AccessorPropertyTable_YIterator accPropTblYIt;

            basar::gui::tie::GuiPropertyTableRef             guiPropTbl;
            basar::gui::tie::GuiPropertyTable_YIterator      guiPropTblYIt;

            FxMatcher()
            {
                accInst       = basar::db::aspect::Manager::getInstance().createAccessorInstance(ACCINST,
                                ACCDEF,
                                basar::db::aspect::ConnectionRef(),
                                basar::db::aspect::FULL_CACHING);
                accPropTbl    = accInst.getPropertyTable();
                accPropTbl.insert(basar::FOR_CLEAN);
                accPropTblYIt = accPropTbl.begin();

                pLabel       = new QLabel(0);

                pLabel->setObjectName(GUITBLCOL);

                matcher		  = basar::gui::tie::Manager::getInstance().createMatcher(pLabel, accPropTbl);

                //configure matcher
                {
                    std::list<basar::VarString> headerLabels;
                    headerLabels.push_back(GUITBLCOL);

                    std::list<basar::VarString> origAttrs;
                    origAttrs.push_back(PROPTBLCOL);

                    matcher.transformProperties(headerLabels, origAttrs);
                }

                guiPropTbl    = matcher.getLeft();
                guiPropTblYIt = guiPropTbl.begin();
            }

            ~FxMatcher()
            {
                delete pLabel;
            }

        protected:
            basar::db::aspect::AccessorInstanceRef         accInst;
            QLabel                   * pLabel;
    };

//--------------------------------------------------------------------------

    TEST_FIXTURE(FxMatcher, Matcher_RL)
    {
        const basar::Int64 expectedInt64 = -12147483647LL;

        basar::VarString expectedInt64String;
        expectedInt64String.lltos(expectedInt64);

        accPropTblYIt.setInt64(PROPTBLCOL, expectedInt64);
        matcher.RightToLeft();
        CHECK_EQUAL(expectedInt64String, guiPropTblYIt.getString(GUITBLCOL));
    }

//--------------------------------------------------------------------------

    TEST_FIXTURE(FxMatcher, Matcher_LR)
    {
        const basar::Int64 expectedInt64 = -12147483647LL;

        basar::VarString expectedInt64String;
        expectedInt64String.lltos(expectedInt64);

        pLabel->setText(QString::number(expectedInt64));

        matcher.LeftToRight();

        CHECK_EQUAL(expectedInt64, accPropTblYIt.getInt64(PROPTBLCOL));
    }

}