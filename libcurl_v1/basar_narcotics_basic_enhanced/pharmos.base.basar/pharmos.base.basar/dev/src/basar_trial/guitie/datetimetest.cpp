#include "datetimetest.h"
#include "testconnection.h"

using basar_trial::guitie::TestConnection;

//------------------------------------------------------------------------------
//------------------- Accessor and StringBuilder Definitions -------------------
//------------------------------------------------------------------------------


BUILDER_DEFINITION(DateTimeSelect)
DONT_CHECK_PROPERTIES(DateTimeSelect)
SQL_PATTERN(DateTimeSelect,
"SELECT  coldate, coldt1 , coltime1 FROM but_detail WHERE colser = 9")

BUILDER_DEFINITION(DateTimeWriteDate)
//ENSURE_PROPERTIES_ARE_SET_AND_ONE_IN_STATE(DateTimeWrite,"coldate;", "UPDATE")
ENSURE_PROPERTIES_ARE_SET(DateTimeWriteDate,"coldate;coldt1;coltime1;")
SQL_PATTERN(DateTimeWriteDate, 
"UPDATE but_detail SET coldate=date('#coldate#') ,coldt1=datetime(#coldt1#) year to fraction , coltime1=datetime(#coltime1#) hour to fraction WHERE colser = 9")



namespace accessor_DateTimeSelect 
{
    BEGIN_ACCESSOR_DEFINITION("Acc_DateTimeSelect")
        PROPERTY_DESCRIPTION_LIST("date coldate; datetime coldt1; time coltime1;")
        ACCESS_METHOD("AccMethod_DateTimeWriteDate")
        SQL_BUILDER_CREATE(DateTimeWriteDate)
        SQL_BUILDER_PUSH_BACK(DateTimeWriteDate)
        ACCESS_METHOD("AccMethod_DateTimeSelect")
        SQL_BUILDER_CREATE(DateTimeSelect)
        SQL_BUILDER_PUSH_BACK(DateTimeSelect)
        END_ACCESSOR_DEFINITION
}


DateTimeTest::DateTimeTest(QWidget *parent)
: QFrame(parent)
{
    ui.setupUi(this);
	
  
    try 
    {
        basar::db::aspect::ConnectionRef connection2 = TestConnection::getBasarTestConnection();
        m_accessor = basar::db::aspect::Manager::getInstance().createAccessorInstance("Acc_DateTimeSelect_Inst","Acc_DateTimeSelect", connection2, basar::db::aspect::FULL_CACHING, true);

        m_matcher = 
            basar::gui::tie::Manager::getInstance().createMatcher(ui.dateWidget, m_accessor.getPropertyTable());
    }
    catch (const basar::Exception& ex)
    {
        QString exstring(tr("Exception: "));
        exstring += ex.what().c_str();
        QMessageBox::warning (this, tr("warning"), exstring, QMessageBox::Ok , 0 ) ;
    }
ui.coldate->setLocale (QLocale(QLocale::C,QLocale::UnitedStates)) ;

}

DateTimeTest::~DateTimeTest()
{

}


void DateTimeTest::on_pushButton_9_clicked()
{
    m_accessor.execute("AccMethod_DateTimeSelect");
    m_matcher.RightToLeft(0,0);
}

void DateTimeTest::on_pushButton_10_clicked()
{ 
    m_matcher.LeftToRight(0,0);
    AccessorPropertyTable_YIterator yit = m_matcher.getRight().begin();
    m_accessor.execute("AccMethod_DateTimeWriteDate", yit);

}


void DateTimeTest::on_pushButton_11_clicked()
{
    m_matcher.LeftToRight(0,0);
    AccessorPropertyTable_YIterator yit = m_matcher.getRight().begin();
    basar::Date date;
    basar::DateTime datetime;
    date.setInvalid();
    datetime.setInvalid();
    yit.setDateTime("coldt1", datetime); 
    yit.setDate("coldate", date);
    yit.setTime("coltime1", datetime);
    m_accessor.execute("AccMethod_DateTimeWriteDate", yit);

    m_accessor.execute("AccMethod_DateTimeSelect");
    m_matcher.RightToLeft(0,0);

}
