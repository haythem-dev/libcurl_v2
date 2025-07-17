#include "tablewidgetabsoluteheadertestframe1.h" 
#include "buttablepropertydescriptionvectors.h"
#include "testconnection.h"

#include "libbasarqtwidget_integersignalemitter.h"

using basar_trial::guitie::TestConnection;


TableWidgetAbsoluteHeaderTestFrame1::TableWidgetAbsoluteHeaderTestFrame1(QWidget *parent)
: QFrame(parent)
{
    ui.setupUi(this);
    
    m_maxrows = 10;

    try 
    {
        basar::db::aspect::ConnectionRef connection2 = TestConnection::getBasarTestConnection();
        m_accessor = basar::db::aspect::Manager::getInstance().createAccessorInstance("test","Acc_StringSelect", connection2, basar::db::aspect::FULL_CACHING, true);
        ui.tableWidget->setRowCount(3);
        ui.tableWidget->setColumnCount(
            static_cast<int>(propertyDescriptionVectorButTableDescriptionAllStrings.m_vector.size()));

        QStringList list;
        std::vector<basar::property::PropertyDescription>::const_iterator iter;
        for (iter = propertyDescriptionVectorButTableDescriptionAllStrings.m_vector.begin(); iter != propertyDescriptionVectorButTableDescriptionAllStrings.m_vector.end(); ++iter)
        {
            list << iter->getName().c_str();
        }
        ui.tableWidget->setHorizontalHeaderLabels(list);
    }
    catch (const basar::Exception& except)
    {
        except.what();
    }

}

TableWidgetAbsoluteHeaderTestFrame1::~TableWidgetAbsoluteHeaderTestFrame1()
{
    m_accessor.getPropertyTable().clear();
}


void TableWidgetAbsoluteHeaderTestFrame1::on_pb_tableWidget_clicked()
{
    if (m_matcher.isNull())
    {
        m_matcher = 
            basar::gui::tie::Manager::getInstance().createMatcher(ui.tableWidget, m_accessor.getPropertyTable());
		// test:
		//m_matcher.getLeft().setMemoryPolicy(basar::gui::tie::WITH_MEMORY);
    }
    m_accessor.execute("AccMethod_StringSelect");
    m_matcher.RightToLeft(0,ui.tableWidget->rowCount());
}



void TableWidgetAbsoluteHeaderTestFrame1::on_pb_rows_clicked()
{
    int n = ui.tableWidget->rowCount();
    if (n < m_maxrows)
    {
        ui.tableWidget->setRowCount(n+1);
    }
    else
    {
        ui.tableWidget->setRowCount(1);
    }

}

void TableWidgetAbsoluteHeaderTestFrame1::on_pushButton_2_clicked()
{
    ui.tableWidget->setAbsoluteVerticalHeaders(true);
}

void TableWidgetAbsoluteHeaderTestFrame1::on_pushButton_3_clicked()
{
    ui.tableWidget->setAbsoluteVerticalHeaders(false);
}

void TableWidgetAbsoluteHeaderTestFrame1::on_pushButton_clicked()
{
    QStringList list;
    QString num;
    for (int i=1; i<9; i+=2)
    {
        num.setNum(i);
        list << num;
        list << "";
    }
    ui.tableWidget->setShotcutsToSelectRows(list);
}

void TableWidgetAbsoluteHeaderTestFrame1::on_button_useABC_clicked()
{
    ui.tableWidget->setABCVerticalHeaderPolicy();
}

void TableWidgetAbsoluteHeaderTestFrame1::on_button_useABC_2_clicked()
{
    ui.tableWidget->setABCVerticalHeaderPolicy(Qt::WindowShortcut);
}

void TableWidgetAbsoluteHeaderTestFrame1::on_pushButton_4_clicked()
{
    QStringList list;
    QString num;
    QString text = "LABEL_";
    for (int i=1; i<9; i++)
    {
        num.setNum(i);
        list << (text+num);
    }
  ui.tableWidget->setVerticalHeaderLabels(list);
}

void TableWidgetAbsoluteHeaderTestFrame1::on_pushButton_5_clicked()
{
    /*
 QStringList list ;
    list <<
        "Shift+A"<< "Shift+B"<< "Shift+C"<< "Shift+D"<< "Shift+E"<< "Shift+F"<< "Shift+G"<< 
        "Shift+H"<< "Shift+I"<< "Shift+J"<< "Shift+K"<< "Shift+L"<< "Shift+M"<< "Shift+N"<< 
        "Shift+O"<< "Shift+P"<< "Shift+Q"<< "Shift+R"<< "Shift+S"<< "Shift+T"<< "Shift+U"<<
        "Shift+V"<< "Shift+W"<< "Shift+X"<< "Shift+Y"<< "Shift+Z" ;
  ui.tableWidget->setShotcutsToSelectRows(list, Qt::WindowShortcut);
  */
    
       QShortcut * s1 = new QShortcut(QKeySequence("Shift+B"),this);
       QShortcut * s  = new QShortcut(QKeySequence("B"),this);
        s1->setContext(Qt::WindowShortcut);
        s->setContext(Qt::WindowShortcut);
        connect(s1, SIGNAL(activated ()), s, SIGNAL(activated ()));
        IntegerSignalEmitter * e = new IntegerSignalEmitter(1, this);
        connect(s, SIGNAL(activated ()), e, SLOT(click()));
        connect(s, SIGNAL(activatedAmbiguously  ()), e, SLOT(click()));
        connect(e, SIGNAL(clicked(int)), this, SLOT(selectRow ( int ) ));
       
    /*  {
        QShortcut * s = new QShortcut(QKeySequence("Shift+1"),this);
        s->setContext( Qt::WindowShortcut);
        IntegerSignalEmitter * e = new IntegerSignalEmitter(1, this);
        connect(s, SIGNAL(activated ()), e, SLOT(click()));
        connect(s, SIGNAL(activatedAmbiguously  ()), e, SLOT(click()));
        connect(e, SIGNAL(clicked(int)), ui.tableWidget, SLOT(selectRow ( int ) ));
}
    {
        QShortcut * s = new QShortcut(QKeySequence(Qt::SHIFT,Qt::Key_2),this);
        s->setContext( Qt::WindowShortcut);
        IntegerSignalEmitter * e = new IntegerSignalEmitter(2, this);
        connect(s, SIGNAL(activated ()), e, SLOT(click()));
        connect(s, SIGNAL(activatedAmbiguously  ()), e, SLOT(click()));
        connect(e, SIGNAL(clicked(int)), ui.tableWidget, SLOT(selectRow ( int ) ));
    }*/
   
  QStringList list2 ;
    list2 <<
        "Shift+a"<< "Shift+b"<< "Shift+c"<< "Shift+d"<< "Shift+e"<< "Shift+f"<< "Shift+g"<< 
        "Shift+h"<< "Shift+i"<< "Shift+j"<< "Shift+k"<< "Shift+l"<< "Shift+m"<< "Shift+n"<< 
        "Shift+o"<< "Shift+p"<< "Shift+q"<< "Shift+r"<< "Shift+s"<< "Shift+t"<< "Shift+u"<<
        "Shift+V"<< "Shift+W"<< "Shift+X"<< "Shift+Y"<< "Shift+Z" ;
  ui.tableWidget->setShotcutsToSelectRows(list2, Qt::WindowShortcut);

}