
#include "tablewidgetmultmatchtestframe.h"
#include "buttablepropertydescriptionvectors.h"
#include "testconnection.h"

using basar_trial::guitie::TestConnection;

TableWidgetMultMatchTestFrame::TableWidgetMultMatchTestFrame(QWidget *parent)
: QFrame(parent)
{
    ui.setupUi(this);

    try 
    {
        basar::db::aspect::ConnectionRef connection2 = TestConnection::getBasarTestConnection();
        m_accessorOne = basar::db::aspect::Manager::getInstance().createAccessorInstance("test","Acc_StringSelect", connection2, basar::db::aspect::FULL_CACHING, true);
        m_accessorReverse = basar::db::aspect::Manager::getInstance().createAccessorInstance("test2","Acc_StringSelectReverse", connection2, basar::db::aspect::FULL_CACHING, true);

        ui.tableWidget->setRowCount(10);
        ui.tableWidget->setColumnCount(
            static_cast<int>(propertyDescriptionVectorButTableDescriptionAllStrings.m_vector.size()));

        ui.tableWidget_2->setRowCount(10);
        ui.tableWidget_2->setColumnCount(
            static_cast<int>(propertyDescriptionVectorButTableDescriptionAllStringsReverse.m_vector.size()));

        setHorizontalHeader( *(ui.tableWidget),propertyDescriptionVectorButTableDescriptionAllStrings);
        setHorizontalHeader( *(ui.tableWidget_2),propertyDescriptionVectorButTableDescriptionAllStringsReverse);

        m_matcherOne = 
            basar::gui::tie::Manager::getInstance().createMatcher(ui.tableWidget, m_accessorOne.getPropertyTable());

        m_matcherReverse = 
            basar::gui::tie::Manager::getInstance().createMatcher(ui.tableWidget_2, m_accessorReverse.getPropertyTable());

        ui.frameMatcherSelect1->setMatcher(m_matcherOne, m_matcherReverse, true);
        ui.frameMatcherSelect2->setMatcher(m_matcherOne, m_matcherReverse, false);

    }
    catch (const basar::Exception& except)
    {
        except.what();
    }
}

TableWidgetMultMatchTestFrame::~TableWidgetMultMatchTestFrame()
{
    m_accessorOne.getPropertyTable().clear();
    m_accessorReverse.getPropertyTable().clear();
}

void TableWidgetMultMatchTestFrame::setHorizontalHeader( BasarTableWidget & tableWidget, const PropertyDescriptionVector & vec)
{
    QStringList list;
    std::vector<basar::property::PropertyDescription>::const_iterator iter;
    for (iter = vec.m_vector.begin(); iter != vec.m_vector.end(); ++iter)
    {
        list << iter->getName().c_str();
    }
    tableWidget.setHorizontalHeaderLabels(list);
}

void  TableWidgetMultMatchTestFrame::select(MatcherRef & matcher,  AccessorInstanceRef & acessor)
{
    acessor.execute("AccMethod_StringSelect");
    matcher.RightToLeft(0,0);
}

void TableWidgetMultMatchTestFrame::showNonsence( BasarTableWidget & tableWidget, const QString & text)
{
    for (int r=0; r<tableWidget.rowCount(); ++r)
    {
        QString rowstring;
        rowstring.setNum(r);
        rowstring = text + "(r=" + rowstring + ",c=";
        for (int c=0; c<tableWidget.rowCount(); ++c)
        {
            QString colstring;
            colstring.setNum(c);
            QTableWidgetItem * item = tableWidget.item (r,c);
            if (item != 0)
            {
                item->setText(rowstring + colstring + ")");
            }
        }
    }
}
void TableWidgetMultMatchTestFrame::on_button_Select_clicked()
{
    select(m_matcherOne, m_accessorOne);
    select(m_matcherReverse, m_accessorReverse);

}

void TableWidgetMultMatchTestFrame::on_button_shownonsence_clicked()
{
    showNonsence( *(ui.tableWidget  ), QString("A"));
    showNonsence( *(ui.tableWidget_2), QString("B"));
}

void TableWidgetMultMatchTestFrame::on_pushButton_clicked()
{
    on_button_Select_clicked();
    ui.frameMatcherSelect1->on_pushButton_RL21_clicked();
}

void TableWidgetMultMatchTestFrame::on_pushButton_2_clicked()
{
    on_button_Select_clicked();
    ui.frameMatcherSelect1->on_pushButton_LR21_clicked();
    ui.frameMatcherSelect1->on_pushButton_RL_clicked();
}