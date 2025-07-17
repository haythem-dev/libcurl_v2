#include "comboboxes.h"

comboboxes::comboboxes(QWidget *parent)
    : QDialog(parent)
{
	ui.setupUi(this);

//-------------------------------	
//MultiComboBox
	ui.comboBox_multi->setColumnCount(3);
	ui.comboBox_multi->setModelColumn(2);
	
	QString s1;
	QString s2;
	for( int i = 0; i < 3; ++i ){
		for( int h = 0; h < 3; ++h ){
			s1.setNum( i );
			s2.setNum( h );
			s1 += ", " + s2;
			ui.comboBox_multi->addItem( i, h, s1 );
		}
	}
	
	ui.comboBox_multi->setHorizontalHeaderItem(0,"Column1");
	ui.comboBox_multi->setHorizontalHeaderItem(1,"Column2");
	ui.comboBox_multi->setHorizontalHeaderItem(2,"Column3");
	
    ui.comboBox_multi->resizeColumnToContents(1);
	ui.comboBox_multi->resizeRowToContents(0);
	ui.comboBox_multi->setColumnWidth(0, 100);
	ui.comboBox_multi->setRowHeight(1, 100);
	//ui.comboBox_multi->hideRow(1);
	ui.comboBox_multi->setCurrentIndex(2);

	ui.comboBox_multi->setModelColumn(2);
	ui.comboBox_multi->setItemText(1, 1, "HALLO");

	
	std::list<basar::VarString> lst;
	lst.push_back("Col1");
	lst.push_back("Col2");
	lst.push_back("Col3");
	ui.comboBox_multi->insertRow(1,lst, ui.comboBox_multi->modelColumn());
		
	ui.comboBox_multi->setMinimumHeightList(100);
	ui.comboBox_multi->setMinimumWidthList(300);
	ui.comboBox_multi->setMinimumSizeList(300, 100);


//-------------------------------	
//QComboBox
	ui.comboBox_qt->addItem("Entry0");
	ui.comboBox_qt->addItem("Entry1");
	ui.comboBox_qt->addItem("Entry2");
	ui.comboBox_qt->addItem("Entry3");

//-------------------------------	
//BasarTableWidget
	QStringList lstCaptions;
	lstCaptions	<< tr("Col0")		<< tr("Col1")		
				<< tr("Col2")		<< tr("Col3");
	
	ui.tableWidget_basar->setHorizontalHeaderLabels( lstCaptions );
	ui.tableWidget_basar->hideVerticalHeader();

	ui.tableWidget_basar->setRowCount(3);
	ui.tableWidget_basar->setColumnCount(4);

	for( int col = 0; col < ui.tableWidget_basar->columnCount(); ++col ){
		if( 2 != col ){
			ui.tableWidget_basar->setEditableColumn( col, false );	
		}
	}

	ui.tableWidget_basar->setCellWidget ( 1,1, ui.comboBox_multi );

}

comboboxes::~comboboxes()
{

}

void comboboxes::on_btTest_clicked()
{
	//ui.comboBox_multi->showPopup();
	ui.comboBox_multi->removeRow(1);

}
