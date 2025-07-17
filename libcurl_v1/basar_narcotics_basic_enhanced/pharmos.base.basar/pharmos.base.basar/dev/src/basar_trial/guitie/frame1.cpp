#include "frame1.h"
#include "testguitie.h"
#include "dlg2.h"

#include <windows.h>

void MyTransformer::RightToLeft(basar::gui::tie::GuiPropertyTable_YIterator              guiIt,
								const basar::db::aspect::AccessorPropertyTable_YIterator accIt
					 		    )
{
	guiIt.setString("bezeichnung",accIt.getString("artikel_name"));
}

void MyTransformer::LeftToRight (const basar::gui::tie::GuiPropertyTable_YIterator  guiIt,
								 basar::db::aspect::AccessorPropertyTable_YIterator accIt
							    )
{
	accIt.setString("artikel_name",guiIt.getString("bezeichnung"));
}

boost::shared_ptr<Transformer> MyTransformer::create()
{
	return boost::shared_ptr<Transformer>(new MyTransformer(*this));
}


Frame1::Frame1(QWidget *parent)
    : QFrame(parent)
{
	ui.setupUi(this);

	//----------------------------------------------------------------------------------------------//
	QStringList lstCaptions;
	lstCaptions	<< tr("Datum")		<< tr("Uhrzeit")		<< tr("Bga-Nr.")	<< tr("name");		
				//<< tr("Satzart")	<< tr("Bestell-Nr.")	<< tr("Menge")		<< tr("Bestand");

//	int iSize = lstCaptions.size();
	QString s;
	
	ui.tableWidget_bb->setColumnCount( 4 );
	ui.tableWidget_bb->setRowCount( 5 );
	ui.tableWidget_bb->setHorizontalHeaderLabels( lstCaptions );
	ui.tableWidget_bb->hideVerticalHeader();

	/*QString lbl;
	QStringList r = ui.tableWidget_bb->horizontalHeaderLabels();
	for( int z = 0; z < r.size(); ++z ){
		ui.tableWidget_bb->setItem( 0, z, new QTableWidgetItem(r.at(z)) );
	}*/

	//----------------------------------------------------------------------------------------------//


	ui.comboBox->setColumnCount(3);
	QString s1;
	QString s2;
	for( int i = 0; i < 10; ++i ){
		for( int h = 0; h < 3; ++h ){
			s1.setNum( i );
			s2.setNum( h );
			s1 += ", " + s2;
			ui.comboBox->addItem( i, h, s1 );
		}
	}
	ui.comboBox->setHorizontalHeaderItem(0,"Column1");
	ui.comboBox->setHorizontalHeaderItem(1,"Column2");
	ui.comboBox->setHorizontalHeaderItem(2,"Column3");

	ui.comboBox->setMaxVisibleItems(1);

	QStringList list;
	list << "bezeichnung" << "filialnr" << "bestand";
	ui.tableWidget->setColumnCount( 3 );
	ui.tableWidget->setRowCount( 3 );
	ui.tableWidget->setHorizontalHeaderLabels( list );
	m_AI = basar::db::aspect::Manager::getInstance().getAccessorInstance("AccessorArticle_Inst");

	AccessorPropertyTable_YIterator yIt;
	m_AI.setDefaultParameter("artikel_nr=398008;");
	m_AI.execute("ReadArticle");
	for (yIt = m_AI.getPropertyTable().begin(); yIt != m_AI.getPropertyTable().end(); yIt++);
	m_MR = basar::gui::tie::Manager::getInstance().createMatcher(ui.frame_4, m_AI.getPropertyTable());
	MyTransformer mT;
	m_MR.push_back(mT);

	basar::gui::tie::CheckBoxTransformer trafoChBox2State("internesteuerung", "0", "1");
	m_MR.push_back(trafoChBox2State);
	basar::gui::tie::RadioButtonTransformer RadioButtonFrame( 
		basar::gui::tie::Transformer::ParamList( "E=frameRadioButton1; D=frameRadioButton2;" ), "abc_artikel");
	m_MR.push_back(RadioButtonFrame);

	//basar::gui::tie::CheckBoxTransformer trafoChBox3State("bestand", "2", "4", "0");
	//m_MR.push_back(trafoChBox3State);

	ui.pushButton_2->setEnabled(false);
}


Frame1::~Frame1()
{

}

void Frame1::on_selectPushButton_clicked()
{
	m_MR.RightToLeft(0,ui.tableWidget->rowCount());
}

void Frame1::on_pushButton_clicked()
{
	m_MR.LeftToRight();
	if (m_MR.getRight().begin(basar::SS_UPDATE) != m_MR.getRight().end())
		ui.pushButton_2->setEnabled(true);
	else
		ui.pushButton_2->setEnabled(false);
}

void Frame1::on_pushButton_2_clicked()
{
	size_t i = m_AI.execute("SaveArticle", m_MR.getRight().begin(basar::SS_UPDATE), m_MR.getRight().end()).m_affectedRows;
	if( i )
	{
		if (m_MR.getRight().begin(basar::SS_UPDATE) != m_MR.getRight().end())
			ui.pushButton_2->setEnabled(true);
		else
			ui.pushButton_2->setEnabled(false);
	}
	
}
///////////////////////////////////////////////////////////////////////////////////
// children einlesen
///////////////////////////////////////////////////////////////////////////////////
void Frame1::on_childrenEinlesenPushButton_clicked()
{
	//QString row = ui.frameLineEdit->text();
	//GuiPropertyTable_YIterator yit = m_MR.getLeft().begin();
	//++yit;
	/*PropertyDescriptionList_Iterator beg
		= m_MR.getLeft().getPropertyDescriptionList().begin();
	PropertyDescriptionList_Iterator end
		= m_MR.getLeft().getPropertyDescriptionList().begin();

	for( ; it ! end; ++it ){
		
	
	}*/
	//return;

	//ui.frame->setVisible( false );

	ui.childrenListWidget->clear();
	ui.childrenCountLineEdit->setText( "0" );

	QList<QWidget*> allCtrlsDialog;
	QWidget* pw;
	if( ui.childrenDlgRadioButton->isChecked() ){
		pw = this;
	} else if( ui.childrenFrameRadioButton->isChecked() ){
		pw = ui.frame;
	} else {
		ui.childrenListWidget->addItem( 
			"Welcher Frame (QWidget) ist einzulesen!" );
		return;
	}
	allCtrlsDialog = pw->findChildren<QWidget*>();

	ui.childrenCountLineEdit->setText( QString::number(allCtrlsDialog.count()) );
	//QList<QLineEdit*> allLineEdits		= this->findChildren<QLineEdit*>();
	//QList<QPushButton*> allPushButtons	= this->findChildren<QPushButton*>();
	
	QString all;
	foreach( pw, allCtrlsDialog ){
		const QMetaObject* mo = pw->metaObject();
		all = pw->objectName() + ": \t" + mo->className();
		//all = all + "(Adresse: " + QString::number(int(*pw)) + ")";
		ui.childrenListWidget->addItem( all );
    }
	
}
///////////////////////////////////////////////////////////////////////////////////
// 
///////////////////////////////////////////////////////////////////////////////////
void Frame1::on_qTableFuellenPushButton_3_clicked()
{
	QString s;
	QTableWidgetItem* item;
	for( int i = 0; i < ui.tableWidget->columnCount(); ++i ){
		for( int h = 0; h < ui.tableWidget->rowCount(); ++h ){
			s.setNum( i );
			s.append(", ");
			s.append( QString::number(h) );
			item = new QTableWidgetItem( s );
			ui.tableWidget->setItem ( i, h, item );
			ui.tableWidget->resizeRowToContents( h );
		}
		ui.tableWidget->resizeColumnToContents( i );
	}
}
///////////////////////////////////////////////////////////////////////////////////
// 
///////////////////////////////////////////////////////////////////////////////////
void Frame1::on_qTableClearenPushButton_3_2_clicked()
{
	ui.tableWidget->clear();
	//ui.tableWidget->resizeColumnToContents
}
///////////////////////////////////////////////////////////////////////////////////
// 
///////////////////////////////////////////////////////////////////////////////////
void Frame1::on_openPEPushButton_clicked()
{
	ui.tableWidget->openPersistentEditor( ui.tableWidget->item(0,0) );
}
///////////////////////////////////////////////////////////////////////////////////
// 
///////////////////////////////////////////////////////////////////////////////////
void Frame1::on_schliessenPushButton_clicked()
{
	this->close();
}

void Frame1::on_neuerDialogPushButton_2_clicked()
{
	char str[128];
	sprintf(str, "onclick: thread id %ld\n", GetCurrentThreadId());
	OutputDebugString(str);

	dlg2 obj( this );
	obj.exec();
}

void Frame1::on_checkBox_stateChanged(int)
{
	ui._bestand->clear();
	switch( ui.checkBox->checkState() ){
		case Qt::Unchecked: 
				ui.tableWidget->setEnabled( false);
				break;
		case Qt::PartiallyChecked:	
				break;
		case Qt::Checked:			
				ui.tableWidget->setEnabled( true );
				break;
		default:	return;
					break;
	}
}

void Frame1::on_tableWidget_clicked(const QModelIndex & rIndex )
{
	int iRow = rIndex.row();
	int iCol = rIndex.column();

	QTableWidgetItem* item = ui.tableWidget->currentItem();

	if (0 == item)
		return;

	ui._bestand->clear();
	ui._bestand->addItem( QString::number(iRow) );
	ui._bestand->addItem( QString::number(iCol) );
	ui._bestand->addItem( item->text() );
	item->setText( "bin geclicked worden!" );
	QColor c;
	c.red();
	item->setTextColor( c );
}

void Frame1::on_erasePushButton_clicked()
{
	QString row = ui.frameLineEdit->text();
	GuiPropertyTable_YIterator yit = m_MR.getLeft().begin();
	//++yit;
	m_MR.getLeft().erase( yit );
}