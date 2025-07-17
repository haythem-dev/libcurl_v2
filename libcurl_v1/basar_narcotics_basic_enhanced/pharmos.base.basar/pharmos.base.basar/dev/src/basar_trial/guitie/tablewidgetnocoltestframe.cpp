
#include "tablewidgetnocoltestframe.h"
#include "buttablepropertydescriptionvectors.h"
#include "testconnection.h"

using basar_trial::guitie::TestConnection;

TableWidgetNoColTestFrame::TableWidgetNoColTestFrame(QWidget *parent)
: QFrame(parent)
{
    ui.setupUi(this);

    initTableWidget(TABLE_WIDGET_NOCOLNAMES, *(ui.tableWidget));
    initTableWidget(TABLE_WIDGET_OK, *(ui.tableWidget_3));
    initTableWidget(TABLE_WIDGET_NOCOLS, *(ui.tableWidget_2));

}

TableWidgetNoColTestFrame::~TableWidgetNoColTestFrame()
{
    m_accessor[TABLE_WIDGET_NOCOLS].getPropertyTable().clear();
    m_accessor[TABLE_WIDGET_NOCOLNAMES].getPropertyTable().clear();
    m_accessor[TABLE_WIDGET_OK].getPropertyTable().clear();
}


void TableWidgetNoColTestFrame::initTableWidget(Which which, BasarTableWidget & tablewidget)
{  
    try 
    {
        basar::db::aspect::ConnectionRef connection2 = TestConnection::getBasarTestConnection();
        AccessorInstanceRef accessorOne = basar::db::aspect::Manager::getInstance().createAccessorInstance("test","Acc_StringSelect", connection2, basar::db::aspect::FULL_CACHING, true);

        tablewidget.setRowCount(10);
        if (TABLE_WIDGET_NOCOLS == which)
        {
            tablewidget.setColumnCount(0);
        }
        else
        {
            tablewidget.setColumnCount(
                static_cast<int>(propertyDescriptionVectorButTableDescriptionAllStrings.m_vector.size()));
        }

        if (TABLE_WIDGET_OK == which)
        {
            QStringList list;
            std::vector<basar::property::PropertyDescription>::const_iterator iter;
            for (iter = propertyDescriptionVectorButTableDescriptionAllStrings.m_vector.begin(); iter != propertyDescriptionVectorButTableDescriptionAllStrings.m_vector.end(); ++iter)
            {
                list << iter->getName().c_str();
            }
            tablewidget.setHorizontalHeaderLabels(list);
        }

        MatcherRef matcherOne = 
            basar::gui::tie::Manager::getInstance().createMatcher(&(tablewidget), accessorOne.getPropertyTable());

        m_accessor[which] = accessorOne;
        m_matcher[which] = matcherOne;

    }
    catch (const basar::Exception& except)
    {
        except.what();
    }

}

void TableWidgetNoColTestFrame::select(Which i)
{
    select(m_matcher[i],m_accessor[i]);
}

void TableWidgetNoColTestFrame::select(MatcherRef & m, AccessorInstanceRef & a)
{  
    a.execute("AccMethod_StringSelect");
    m.RightToLeft(0,0);
}

void TableWidgetNoColTestFrame::on_pushButton_clicked()
{
    select(TABLE_WIDGET_NOCOLNAMES);
}


void TableWidgetNoColTestFrame::on_pushButton_2_clicked()
{
    select(TABLE_WIDGET_NOCOLS);
}

void TableWidgetNoColTestFrame::on_pushButton_All_clicked()
{ 
    select(TABLE_WIDGET_OK);
    select(TABLE_WIDGET_NOCOLNAMES);
    select(TABLE_WIDGET_NOCOLS);
}

void TableWidgetNoColTestFrame::on_pushButton_3_clicked()
{
  select(TABLE_WIDGET_OK);
}