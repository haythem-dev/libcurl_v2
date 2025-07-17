#include "checkboxtest.h"
#include "testconnection.h"

using basar_trial::guitie::TestConnection;

BUILDER_DEFINITION(CheckBoxSelect2)
DONT_CHECK_PROPERTIES(CheckBoxSelect2)
SQL_PATTERN(CheckBoxSelect2,
"SELECT colvch4 as colvch4a , colvch3 as colvch3a FROM but_detail WHERE colser = 9")


BUILDER_DEFINITION(CheckBoxSelect1)
DONT_CHECK_PROPERTIES(CheckBoxSelect1)
SQL_PATTERN(CheckBoxSelect1,
"SELECT   colvch3 as colvch3b FROM but_detail WHERE colser = 9")

BUILDER_DEFINITION(CheckBoxSelect3)
DONT_CHECK_PROPERTIES(CheckBoxSelect3)
SQL_PATTERN(CheckBoxSelect3,
"SELECT   colvch3 as colvch3c FROM but_detail WHERE colser = 9")

namespace accessor_CheckBoxSelect1 
{
    BEGIN_ACCESSOR_DEFINITION("Acc_CheckBoxSelect1")
    PROPERTY_DESCRIPTION_LIST("string colvch3b;")
    ACCESS_METHOD("AccMethod_CheckBoxSelect1")
    SQL_BUILDER_CREATE(CheckBoxSelect1)
    SQL_BUILDER_PUSH_BACK(CheckBoxSelect1)
    END_ACCESSOR_DEFINITION
}

namespace accessor_CheckBoxSelect3
{
    BEGIN_ACCESSOR_DEFINITION("Acc_CheckBoxSelect3")
    PROPERTY_DESCRIPTION_LIST("string colvch3c;")
    ACCESS_METHOD("AccMethod_CheckBoxSelect3")
    SQL_BUILDER_CREATE(CheckBoxSelect3)
    SQL_BUILDER_PUSH_BACK(CheckBoxSelect3)
    END_ACCESSOR_DEFINITION
}

namespace accessor_CheckBoxSelect2 
{
    BEGIN_ACCESSOR_DEFINITION("Acc_CheckBoxSelect2")
    PROPERTY_DESCRIPTION_LIST("string colvch4a; string colvch3a; ")

    ACCESS_METHOD("AccMethod_CheckBoxSelect2")
    SQL_BUILDER_CREATE(CheckBoxSelect2)
    SQL_BUILDER_PUSH_BACK(CheckBoxSelect2)
    END_ACCESSOR_DEFINITION
}

CheckBoxTest::CheckBoxTest(QWidget *parent)
: QFrame(parent)
{
    ui.setupUi(this);

  
    try 
    {
        basar::db::aspect::ConnectionRef connection2 = TestConnection::getBasarTestConnection();
        m_accessorCB1 = basar::db::aspect::Manager::getInstance().createAccessorInstance("Acc_CheckBoxSelect_Inst1","Acc_CheckBoxSelect1", connection2, basar::db::aspect::FULL_CACHING, true);
        m_accessorCB2 = basar::db::aspect::Manager::getInstance().createAccessorInstance("Acc_CheckBoxSelect_Inst2","Acc_CheckBoxSelect2", connection2, basar::db::aspect::FULL_CACHING, true);
        m_accessorCB3 = basar::db::aspect::Manager::getInstance().createAccessorInstance("Acc_CheckBoxSelect_Inst3","Acc_CheckBoxSelect3", connection2, basar::db::aspect::FULL_CACHING, true);

        m_matcherCB1 = basar::gui::tie::Manager::getInstance().createMatcher(ui.groupbox_checkbox1, m_accessorCB1.getPropertyTable());
        m_matcherCB2 = basar::gui::tie::Manager::getInstance().createMatcher(ui.groupbox_checkbox2, m_accessorCB2.getPropertyTable());
        m_matcherCB3 = basar::gui::tie::Manager::getInstance().createMatcher(ui.colvch3c          , m_accessorCB3.getPropertyTable());


    }
    catch (const basar::Exception& except)
    {
        except.what();
    }

}

CheckBoxTest::~CheckBoxTest()
{

}

void CheckBoxTest::on_pushButton_14_clicked()
{
    m_accessorCB1.execute("AccMethod_CheckBoxSelect1");
    m_matcherCB1.RightToLeft(0,0);
}

void CheckBoxTest::on_pushButton_15_clicked()
{
    m_accessorCB3.execute("AccMethod_CheckBoxSelect3");
    m_matcherCB3.RightToLeft(0,0);
}

void CheckBoxTest::on_pushButton_12_clicked()
{
    m_accessorCB2.execute("AccMethod_CheckBoxSelect2");
    m_matcherCB2.RightToLeft(0,0);
}

void CheckBoxTest::on_pushButton_13_clicked()
{
    ui.colvch3a->setChecked(true);
    ui.colvch3b->setChecked(true);
    ui.colvch4a->setChecked(true);

}