//-------------------------------------------------------------------------------------------------//
// includes
//-------------------------------------------------------------------------------------------------//
#include "commonheader.h"
#include "narcoticstransformer.h"
#include "na01incominggoodsoverviewvc.h"
#include "loggerpool.h"
#include "qvalidatoritemdelegate.h"

//-------------------------------------------------------------------------------------------------//
// macro definition section
//-------------------------------------------------------------------------------------------------//
#define INSTALL_BUSY_CURSOR                 QCursor cursor = this->cursor();\
                                            setCursor( Qt::BusyCursor );
#define REINSTALL_PREVIOUS_CURSOR( cursor ) setCursor( cursor );

//-------------------------------------------------------------------------------------------------//
// namespaces
//-------------------------------------------------------------------------------------------------//
namespace narcotics
{
namespace viewConn
{

#define qtyColumn 5

using basar::VarString;
using basar::Int32;
using basar::db::aspect::AccessorPropertyTableRef;
using basar::db::aspect::AccessorPropertyTable_YIterator;
using basar::gui::tie::PropertyNameTransformer;


//-------------------------------------------------------------------------------------------------//
IncomingGoodsOverviewVC::IncomingGoodsOverviewVC( QWidget *parent )
: QDialog( parent )
{
    setupUi( this );
    connectEvents();
    bCellPressed = false;
}

//-------------------------------------------------------------------------------------------------//
IncomingGoodsOverviewVC::~IncomingGoodsOverviewVC()
{
}

//-------------------------------------------------------------------------------------------------//
void IncomingGoodsOverviewVC::connectEvents()
{
    QObject::connect( chkArticleName,	        SIGNAL( stateChanged( int )									),
                      this,                     SLOT  ( activateSearchForArticleName()						));
	QObject::connect( chkSupplierName,	        SIGNAL( stateChanged( int )									),
                      this,                     SLOT  ( activateSearchForSupplierName()						));
	QObject::connect( optOrder,				    SIGNAL( toggled( bool )										),
                      this,                     SLOT  ( buttonGroupStateChanged()							));
	QObject::connect( optSupplier,			    SIGNAL( toggled( bool )										),
                      this,						SLOT  ( buttonGroupStateChanged()							));
	QObject::connect( btnRecordItem,			SIGNAL( clicked()											),
                      this,						SLOT  ( onBtnRecordItem_clicked()							));
	QObject::connect( btnShowIncomingGoods,		SIGNAL( clicked()											),
                      this,						SLOT  ( onBtnShowIncomingGoods_clicked()					));
	QObject::connect( btnShowReturnGoods,		SIGNAL( clicked()											),
                      this,						SLOT  ( onBtnShowReturnGoods_clicked()						));
	QObject::connect( btnShowAllIncomingGoods,	SIGNAL( clicked()											),
                      this,						SLOT  ( onBtnShowAllIncomingGoods_clicked()					));
	QObject::connect( tblIncomingGoods,			SIGNAL( itemSelectionChanged()								),
                      this,						SLOT  ( onTblIncomingGoods_itemSelectionChanged()			));
    
    QObject::connect( tblIncomingGoods,			SIGNAL( cellPressed( int, int )						),
                      this,						SLOT  ( onTblIncomingGoods_cellPressed( int, int )	)); //sk
    QObject::connect( tblIncomingGoods,			SIGNAL( itemActivated( QTableWidgetItem* )					),
                      this,						SLOT  ( onTblIncomingGoods_itemActivated( QTableWidgetItem* )	)); //sk
	QObject::connect( tblIncomingGoods,			SIGNAL( cellDoubleClicked( int, int )						),
                      this,						SLOT  ( onTblIncomingGoods_cellDoubleClicked( int, int )	));
	QObject::connect( tblIncomingGoods,			SIGNAL( itemChanged( QTableWidgetItem* )					),
                      this,						SLOT  ( onTblIncomingGoods_itemChanged( QTableWidgetItem* )	));
	QObject::connect(btnProcessAllVisibleIncomingGoods,SIGNAL(clicked()										),
					  this,						SLOT  (onBtnProcessAllVisibleIncomingGoods_clicked()		));
	QObject::connect( btnSearch,				SIGNAL( clicked()											),
                      this,						SLOT  ( onBtnSearch_clicked()								));
	QObject::connect( btnIncomingGoodsRefresh,	SIGNAL( clicked()											),
                      this,						SLOT  ( onBtnIncomingGoodsRefresh_clicked()					));
	QObject::connect( btnIncomingGoodsDelete,	SIGNAL( clicked()											),
                      this,						SLOT  ( onBtnIncomingGoodsDelete_clicked()					));
	QObject::connect( btnUpdateSupplier,		SIGNAL( clicked()											),
                      this,						SLOT  ( onBtnUpdateSupplier_clicked()						));
	QObject::connect( orderNumber,				SIGNAL( textChanged( QString )								),
                      this,						SLOT  ( onOrderNumber_textChanged( QString )				));
	QObject::connect( articleNumber,			SIGNAL( textChanged( QString )								),
                      this,						SLOT  ( onArticleNumber_textChanged( QString )				));
	QObject::connect( articleName,				SIGNAL( textChanged( QString )								),
                      this,						SLOT  ( onArticleName_textChanged( QString )				));
	QObject::connect( supplierNo,				SIGNAL( textChanged( QString )								),
                      this,						SLOT  ( onSupplierNo_textChanged( QString )					));
	QObject::connect( supplierName,				SIGNAL( textChanged( QString )								),
                      this,						SLOT  ( onSupplierName_textChanged( QString )				));

    //return pressed for serach fields
    //order first
    QObject::connect( orderNumber,		SIGNAL( returnPressed() ), this, SLOT( onBtnSearch_clicked() ) );
    QObject::connect( articleNumber,	SIGNAL( returnPressed() ), this, SLOT( onBtnSearch_clicked() ) );
    QObject::connect( articleName,		SIGNAL( returnPressed() ), this, SLOT( onBtnSearch_clicked() ) );

    //supplier second
    QObject::connect( supplierNo,		SIGNAL( returnPressed() ), this, SLOT( onBtnSearch_clicked() ) );
    QObject::connect( supplierName,		SIGNAL( returnPressed() ), this, SLOT( onBtnSearch_clicked() ) );
}

//-------------------------------------------------------------------------------------------------//
void IncomingGoodsOverviewVC::init( AccessorPropertyTableRef incomingGoodsTable,
								    AccessorPropertyTableRef supplierTable,
								    AccessorPropertyTableRef noIncomingGoodsTable,
								    AccessorPropertyTableRef noReturnGoodsTable   )
{
    basar::ConstString fun = "IncomingGoodsOverviewVC::init()";
    BLOG_TRACE_METHOD( LoggerPool::loggerViewConn, fun );

	m_IsFirstCall = true;
	initGuiCtrls();

	m_matSupplier = basar::gui::tie::Manager::getInstance().createMatcher( cboSupplier, supplierTable );
	if( m_OriginalAttributeNames.empty() )
	{
		m_OriginalAttributeNames.push_back( "narcoticssupplierno"	);
		m_OriginalAttributeNames.push_back( "suppliername"			);
	}

	m_matSupplier.transformProperties( cboSupplier->horizontalHeaderLabels(), m_OriginalAttributeNames );

	m_matNoReturnGoods	= basar::gui::tie::Manager::getInstance().createMatcher( grpNumberOpenGoods, noReturnGoodsTable );
    m_matNoIncomingGoods= basar::gui::tie::Manager::getInstance().createMatcher( grpNumberOpenGoods, noIncomingGoodsTable );

    m_matIncomingGoods	= basar::gui::tie::Manager::getInstance().createMatcher( gBox_incomingGoods, incomingGoodsTable );
    VarString Parameter;
	Parameter.format(	"purchaseorderno=%s; articlecode=%s; articlename=%s; pharmaform=%s; packageunit=%s; qtytransaction=%s; "
						"transactiontype=%s; customersupplierno=%s; customersuppliername=%s; customersupplierstreet=%s; customersuppliercip=%s; "
                        "customersupplierlocation=%s; transactiondate=%s; transactiontime=%s;",
						tr("Order-No.").toLocal8Bit().data(), tr("ArticleNo.").toLocal8Bit().data(), tr("ArticleName").toLocal8Bit().data(),
						tr("PharmaForm").toLocal8Bit().data(), tr("PackageUnit").toLocal8Bit().data(), tr("Quantity").toLocal8Bit().data(),
						tr("TT").toLocal8Bit().data(), tr("BGA-No.").toLocal8Bit().data(), tr("Name").toLocal8Bit().data(),
                        tr("Street").toLocal8Bit().data(), tr("CIP").toLocal8Bit().data(), tr("Location").toLocal8Bit().data(),
						tr("Date").toLocal8Bit().data(), tr("Time").toLocal8Bit().data());
	PropertyNameTransformer tableTransformer( Parameter.c_str() );
	m_matIncomingGoods.push_back( tableTransformer );
	//QuantityTransformer quantityTransformer;
	//m_matIncomingGoods.push_back( quantityTransformer );
	VariableDateTransformer dateTransformer( constants::TRANSACTIONDATE, tr("Date").toLocal8Bit().constData() );
	m_matIncomingGoods.push_back( dateTransformer );
	VariableTimeTransformer timeTransformer( constants::TRANSACTIONTIME, tr("Time").toLocal8Bit().constData() );
	m_matIncomingGoods.push_back( timeTransformer );
    /*
	QObject::connect(tblIncomingGoods->itemDelegate(), SIGNAL(closeEditor(QWidget*, QAbstractItemDelegate::EndEditHint)),
		this, SLOT(onTblIncomingGoods_closeEditor(QWidget*)));
    */ //srk
    
}

//-------------------------------------------------------------------------------------------------//
void IncomingGoodsOverviewVC::buttonGroupStateChanged()
{
    activateOrderSearchCriteria   (  optOrder->isChecked() );
    activateSupplierSearchCriteria( !optOrder->isChecked() );

    activateIncomingGoodsDisplay( false );
    activateNumberOfGoodsDisplay( false );
}

//-------------------------------------------------------------------------------------------------//
void IncomingGoodsOverviewVC::activateOrderNumberCriterion( const bool active )
{
    orderNumber->clear();
    orderNumber->setEnabled( active );
    if( active ){
		orderNumber->setFocus();
    }
}

//-------------------------------------------------------------------------------------------------//
void IncomingGoodsOverviewVC::activateArticleNumberCriterion( const bool active )
{
    articleNumber->clear();
    articleNumber->setEnabled( active );
}

//-------------------------------------------------------------------------------------------------//
void IncomingGoodsOverviewVC::activateArticleNameCriterion( const bool active )
{
    articleName->clear();
    articleName->setEnabled( active );
    if( active ){
		articleName->setFocus();
    }
}

//-------------------------------------------------------------------------------------------------//
void IncomingGoodsOverviewVC::activateOrderSearchCriteria( const bool active )
{
    grpOrderSearchCriteria->setEnabled( active );
    chkArticleName->setChecked( false );
    chkArticleName->setEnabled( active );

    activateOrderNumberCriterion( active );
    activateArticleNumberCriterion( active );
    activateArticleNameCriterion( !active );
}

///////////////////////////////////////////////////////////////////////////////////////////////////
/*! \return	void
	\param	is not used here
	\throw	no-throw */
///////////////////////////////////////////////////////////////////////////////////////////////////
void IncomingGoodsOverviewVC::activateSearchForArticleName()
{
    activateArticleNumberCriterion( !chkArticleName->isChecked() );
    activateArticleNameCriterion  (  chkArticleName->isChecked() );
}

//-------------------------------------------------------------------------------------------------//
void IncomingGoodsOverviewVC::activateSupplierSearchCriteria( const bool active )
{
    grpSupplierSearchCriteria->setEnabled( active );
    chkSupplierName->setChecked( false );
    chkSupplierName->setEnabled( active );

    activateSupplierNumberCriterion( active );
    activateSupplierNameCriterion( !active );

    btnSearch->setEnabled( false );
}

//-------------------------------------------------------------------------------------------------//
void IncomingGoodsOverviewVC::activateSupplierNumberCriterion( const bool active )
{
    supplierNo->clear();
	supplierNo->setEnabled( active );
	if( active ){
	    supplierNo->setFocus();
    }
}

//-------------------------------------------------------------------------------------------------//
void IncomingGoodsOverviewVC::activateSupplierNameCriterion( const bool active )
{
	supplierName->clear();
	supplierName->setEnabled( active );
    if( active ){
		supplierName->setFocus();
    }
}

//-------------------------------------------------------------------------------------------------//
void IncomingGoodsOverviewVC::activateSearchForSupplierName()
{
    activateSupplierNumberCriterion( !chkSupplierName->isChecked() );
    activateSupplierNameCriterion  (  chkSupplierName->isChecked() );
}

//-------------------------------------------------------------------------------------------------//
void IncomingGoodsOverviewVC::activateIncomingGoodsDisplay( const bool active )
{
    grpIncomingGoodsDisplay->setEnabled( active );
    tblIncomingGoods->setEnabled       ( active );
    btnIncomingGoodsRefresh->setEnabled( active );
    btnIncomingGoodsDelete->setEnabled ( active );

    if( false == active ){
        tblIncomingGoods->clearContext();
    }
    activateSupplierDisplay( false );
}

//-------------------------------------------------------------------------------------------------//
void IncomingGoodsOverviewVC::switchSupplierReplacement( const bool active )
{
    activateSupplierDisplay( active );
}

//-------------------------------------------------------------------------------------------------//
void IncomingGoodsOverviewVC::activateSupplierDisplay( const bool active )
{
    grpSupplierDisplay->setEnabled( active );
    cboSupplier->setEnabled( active );
    btnUpdateSupplier->setEnabled( active );
}

//-------------------------------------------------------------------------------------------------//
void IncomingGoodsOverviewVC::initSupplierDisplay()
{
    cboSupplier->setColumnCount         ( 2             );
	cboSupplier->setHorizontalHeaderItem( 0, tr("BGA")  );
	cboSupplier->setHorizontalHeaderItem( 1, tr("Name") );
}

//-------------------------------------------------------------------------------------------------//
void IncomingGoodsOverviewVC::initIncomingGoodsDisplay()
{
    QStringList list;
	list	<< tr("Order-No.") << tr("ArticleNo.") << tr("ArticleName") << tr("PharmaForm") << tr("PackageUnit")
			<< tr("Quantity") << tr("TT") << tr("BGA-No.") << tr("Name") << tr("Street") << tr("CIP") << tr("Location") << tr("Date") << tr("Time");
	tblIncomingGoods->setHorizontalHeaderLabels( list );
	tblIncomingGoods->hideVerticalHeader();
	for( int i = 0; i < tblIncomingGoods->columnCount(); i++ )
	{
		if( i == qtyColumn ){
            tblIncomingGoods->setEditableColumn( i, true );
            tblIncomingGoods->autoFillBackground();
		}
		else{
            tblIncomingGoods->setEditableColumn( i, false );
		}
	}
	tblIncomingGoods->resizeColumnsToContents();

	// set column validator
	QValidatorItemDelegate* delegate = new QValidatorItemDelegate( this );
	delegate->setColumnValidator( qtyColumn, new QRegExpValidator( QRegExp("[0-9]{0,4}"), this ) );
	tblIncomingGoods->setItemDelegate( delegate );
	tblIncomingGoods->setEditTriggers(QAbstractItemView::AllEditTriggers);

    initSupplierDisplay();
}

//-------------------------------------------------------------------------------------------------//
void IncomingGoodsOverviewVC::initSearchCriteria()
{
    initSearchOptions();

	optOrder->adjustSize();
	optSupplier->adjustSize();

    if( 0 == orderNumber->validator() ){
		orderNumber->setValidator( new QRegExpValidator( QRegExp("[0-9]{0,7}"), this ) );
	}

	if( 0 == articleNumber->validator() ){
		articleNumber->setValidator( new QRegExpValidator( QRegExp("[0-9]{0,8}"), this ) );
	}

    if( 0 == supplierNo->validator() ){
		supplierNo->setValidator( new QRegExpValidator( QRegExp("[0-9]{0,7}"), this ) );
	}
}

//-------------------------------------------------------------------------------------------------//
void IncomingGoodsOverviewVC::initGuiCtrls()
{
    initSearchCriteria();
	optOrder->blockSignals(true);
	optOrder->setChecked(true);
    optOrder->blockSignals(false);
	activateOrderSearchCriteria( true );
	activateSupplierSearchCriteria( false );

    initIncomingGoodsDisplay();
    activateIncomingGoodsDisplay( false );

    activateNumberOfGoodsDisplay( false );

    // dialog specific ctrls
    btnRecordItem->setEnabled( true );
    btnClose->setEnabled( true );
}

//-------------------------------------------------------------------------------------------------//
void IncomingGoodsOverviewVC::activateNumberOfGoodsDisplay( const bool active )
{
    grpNumberOpenGoods->setEnabled( true );

    lblOpenIncomingGoods->setEnabled( active );
    lblOpenReturnGoods->setEnabled( active );

    if( false == active )
	{
        incominggoods->clear();
	    returngoods->clear();
    }

    btnShowIncomingGoods->setEnabled( active );
    btnShowReturnGoods->setEnabled( active );
    btnShowAllIncomingGoods->setEnabled( true );
}

//-------------------------------------------------------------------------------------------------//
void IncomingGoodsOverviewVC::onBtnSearch_clicked()
{
    basar::ConstString fun = "IncomingGoodsOverviewVC::onBtnSearch_clicked()";
    BLOG_TRACE_METHOD( LoggerPool::loggerViewConn, fun );

    using namespace basar::appl;
    INSTALL_BUSY_CURSOR

	EventReturnStruct result = basar::appl::SystemEventManager::getInstance().fire( "SearchIncomingGoods" );
	if( false == result.isHandlerOK() ){
		clearWidget();
	}
    REINSTALL_PREVIOUS_CURSOR( cursor )
    PROCESS_EVENT_RETURN( result, this )

    selectFirstIncomingGood();
}

//-------------------------------------------------------------------------------------------------//
void IncomingGoodsOverviewVC::onBtnShowIncomingGoods_clicked()
{
	basar::ConstString fun = "IncomingGoodsOverviewVC::onBtnShowIncomingGoods_clicked()";
    BLOG_TRACE_METHOD( LoggerPool::loggerViewConn, fun );

	INSTALL_BUSY_CURSOR

    basar::appl::EventReturnType result = basar::appl::SystemEventManager::getInstance().fire( "DisplayUndealedIncomingGoods" );
	if( false == result.isHandlerOK() ){
		clearWidget();
	}

    REINSTALL_PREVIOUS_CURSOR( cursor )
    PROCESS_EVENT_RETURN( result, this )
}

//-------------------------------------------------------------------------------------------------//
void IncomingGoodsOverviewVC::onBtnShowReturnGoods_clicked()
{
	basar::ConstString fun = "IncomingGoodsOverviewVC::onBtnShowReturnGoods_clicked()";
    BLOG_TRACE_METHOD( LoggerPool::loggerViewConn, fun );

	INSTALL_BUSY_CURSOR

    btnUpdateSupplier->setDisabled( true );
    basar::appl::EventReturnType result = basar::appl::SystemEventManager::getInstance().fire( "DisplayUndealedReturnedGoods" );
	if( false == result.isHandlerOK() ){
		clearWidget();
	}

    REINSTALL_PREVIOUS_CURSOR( cursor )
    PROCESS_EVENT_RETURN( result, this )
}


//----------Process All Button---------------------------------------------------------------------------------------//
void IncomingGoodsOverviewVC::onBtnProcessAllVisibleIncomingGoods_clicked()
{
	basar::ConstString fun = "IncomingGoodsOverviewVC::onBtnProcessAllIncomingGoods_clicked()";
	BLOG_TRACE_METHOD(LoggerPool::loggerViewConn, fun);

	INSTALL_BUSY_CURSOR

		int rowCount = tblIncomingGoods->rowCount();
        int Currentrow = 0; //srk

		while(--rowCount >= 0 && isIncomingGoodDisplayed())
		{
            QTableWidgetItem *item1(tblIncomingGoods->item(Currentrow,0)); //srk added
            if(item1) //srk
            {//srk
                tblIncomingGoods->selectRow(Currentrow); //srk selectRow(0);
			    basar::appl::EventReturnStruct result = basar::appl::SystemEventManager::getInstance().fire("SaveIncomingGoodsList");
                if (!result.isOK())
			    {
                    if(result.message == "OUTDATED_DATE")
                    {
                        QString str1 = "Bitte korrigiere Datum fur Article No. " + tblIncomingGoods->item( tblIncomingGoods->currentRow(), 1 )->text();
                        str1 += " Datum " + tblIncomingGoods->item( tblIncomingGoods->currentRow(), 10 )->text() + " ist ";
                        str1 +=  result.message.c_str();
                        basar::VarString s1(str1.toStdString());
                        basar::gui::tie::infoMsgBox(this,s1.c_str() , tr("Narcotics").toLocal8Bit().data());
                    }
                    else if(result.message == "ARTICLE_LOCKED")
                    {
                        QString str2 = "Error Article No. " + tblIncomingGoods->item( tblIncomingGoods->currentRow(), 1 )->text() + " ist ";
                        str2 +=  result.message.c_str();
                        basar::VarString s2(str2.toStdString());
                        basar::gui::tie::infoMsgBox(this,s2.c_str() , tr("Narcotics").toLocal8Bit().data());
                    }
                    else
                    {
                        //QString str3 = "Fur Article No. " + tblIncomingGoods->item( tblIncomingGoods->currentRow(), 1 )->text() + " korrigiere die Error ";
						QString str3 = "Es wurde keine Aenderung fur PZN: " + tblIncomingGoods->item(tblIncomingGoods->currentRow(), 1)->text() + " vorgenommen ";
                        str3 +=  result.message.c_str();
                        basar::VarString s3(str3.toStdString());
                        basar::gui::tie::infoMsgBox(this,s3.c_str() , tr("Narcotics").toLocal8Bit().data());
                    }
                    break;
                }
            } //sk
            else
            {
               break; //There are no more items to process
            }

		}

	REINSTALL_PREVIOUS_CURSOR(cursor)
}

//-------------------------------------------------------------------------------------------------//
void IncomingGoodsOverviewVC::initSearchOptions()
{
	basar::ConstString fun = "IncomingGoodsOverviewVC::initSearchOptions()";
    BLOG_TRACE_METHOD( LoggerPool::loggerViewConn, fun );

    optDummy->setVisible( false );
    optDummy->setChecked( true  );
    optOrder->setChecked( false );
    optSupplier->setChecked( false );
}

//-------------------------------------------------------------------------------------------------//
void IncomingGoodsOverviewVC::onBtnShowAllIncomingGoods_clicked()
{
	basar::ConstString fun = "IncomingGoodsOverviewVC::onBtnShowAllIncomingGoods_clicked()";
    BLOG_TRACE_METHOD( LoggerPool::loggerViewConn, fun );

	INSTALL_BUSY_CURSOR

    initSearchOptions();

    activateOrderSearchCriteria( false );
    activateSupplierSearchCriteria( false );

    activateIncomingGoodsDisplay( false );
    activateNumberOfGoodsDisplay( false );

    using namespace basar::appl;
    EventReturnStruct result = SystemEventManager::getInstance().fire( "AllUnprocessedIncomingGoodsRequested" );

    REINSTALL_PREVIOUS_CURSOR( cursor )
    PROCESS_EVENT_RETURN( result, this )
}

//-------------------------------------------------------------------------------------------------//
void IncomingGoodsOverviewVC::onBtnIncomingGoodsDelete_clicked()
{
    basar::ConstString fun = "IncomingGoodsOverviewVC::onBtnIncomingGoodsDelete_clicked()";
    BLOG_TRACE_METHOD( LoggerPool::loggerViewConn, fun );

	if( basar::gui::tie::questionMsgBox( this,
		tr("Do you really want to delete the incoming delivery?").toLocal8Bit().constData(),
		tr("Narcotics").toLocal8Bit().constData() ) == basar::gui::tie::ButtonYes )
	{
		INSTALL_BUSY_CURSOR
		basar::appl::EventReturnStruct result = basar::appl::SystemEventManager::getInstance().fire( "DeleteIncomingGoods" );

		REINSTALL_PREVIOUS_CURSOR( cursor )
		PROCESS_EVENT_RETURN( result, this )
	}
}

//-------------------------------------------------------------------------------------------------//
void IncomingGoodsOverviewVC::onBtnIncomingGoodsRefresh_clicked()
{
	basar::ConstString fun = "IncomingGoodsOverviewVC::onBtnIncomingGoodsRefresh_clicked()";
	BLOG_TRACE_METHOD( LoggerPool::loggerViewConn, fun );

	INSTALL_BUSY_CURSOR
    //bAktBtnClicked = true; //srk
	basar::appl::EventReturnStruct result = basar::appl::SystemEventManager::getInstance().fire( "ReloadLastIncomingGoodsRequested" );
    //bAktBtnClicked = false; //srk
	REINSTALL_PREVIOUS_CURSOR( cursor )
	PROCESS_EVENT_RETURN( result, this )
}

//-------------------------------------------------------------------------------------------------//
void IncomingGoodsOverviewVC::onBtnRecordItem_clicked()
{
	basar::ConstString fun = "IncomingGoodsOverviewVC::onBtnRecordItem_clicked()";
    BLOG_TRACE_METHOD( LoggerPool::loggerViewConn, fun );

    {
        INSTALL_BUSY_CURSOR

        basar::appl::SystemEventManager::getInstance().fire( "RegisteredIncomingGood" );
	    m_ManualDialog->init( m_matSupplier.getRight() );

	    basar::appl::SystemEventManager::getInstance().fire( "NewBookingIncomingGood" );

        REINSTALL_PREVIOUS_CURSOR( cursor )
	    m_ManualDialog->show();
    }

    {
        INSTALL_BUSY_CURSOR
	    basar::appl::SystemEventManager::getInstance().fire( "RegisteredIncomingGood" );
        REINSTALL_PREVIOUS_CURSOR( cursor )
    }
}

//-------------------------------------------------------------------------------------------------//
const bool IncomingGoodsOverviewVC::isOneIncomingGoodItemSelected()
{
    basar::ConstString fun = "IncomingGoodsOverviewVC::isOneIncomingGoodItemSelected()";
    BLOG_TRACE_METHOD( LoggerPool::loggerViewConn, fun );

    return !getCurrentIncomingGood().isNull();
}

//-------------------------------------------------------------------------------------------------//
void IncomingGoodsOverviewVC::onTblIncomingGoods_itemChanged( QTableWidgetItem* p )
{
    basar::ConstString fun = "IncomingGoodsOverviewVC::onTblIncomingGoods_itemChanged()";
    BLOG_TRACE_METHOD( LoggerPool::loggerViewConn, fun );

    const bool selected = isOneIncomingGoodItemSelected();
    btnIncomingGoodsDelete->setEnabled( selected );
    bCellPressed = false;
    if( false == selected )
	{
		tblIncomingGoods->blockSignals( true );
		p->setText("");
		tblIncomingGoods->blockSignals( false );
		return;
	}

    if( tblIncomingGoods->column(p) == qtyColumn )
	{
		INSTALL_BUSY_CURSOR
		if( tblIncomingGoods->item( tblIncomingGoods->row(p), tblIncomingGoods->column(p) )->text().toInt() != 0 )
		{
			basar::appl::EventReturnStruct result =  basar::appl::SystemEventManager::getInstance().fire( "SaveIncomingGoodsList" );
			if (!result.isOK()) //srk Added
			{
				if(result.message == "OUTDATED_DATE")
				{
					QString str1 = "Bitte korrigiere Datum fur Article No. " + tblIncomingGoods->item( tblIncomingGoods->currentRow(), 1 )->text();
					str1 += " Datum " + tblIncomingGoods->item( tblIncomingGoods->currentRow(), 10 )->text() + " ist ";
					str1 +=  result.message.c_str();
					basar::VarString s1(str1.toStdString());
					basar::gui::tie::infoMsgBox(this,s1.c_str() , tr("Narcotics").toLocal8Bit().data());
				}
				else if(result.message == "ARTICLE_LOCKED")
				{
					QString str2 = "Error Article No. " + tblIncomingGoods->item( tblIncomingGoods->currentRow(), 1 )->text() + " ist ";
					str2 +=  result.message.c_str();
					basar::VarString s2(str2.toStdString());
					basar::gui::tie::infoMsgBox(this,s2.c_str() , tr("Narcotics").toLocal8Bit().data());
				}
				else
				{
					//QString str3 = "Fur Article No. " + tblIncomingGoods->item( tblIncomingGoods->currentRow(), 1 )->text() + " korrigiere die Error ";
					QString str3 = "Es wurde keine Aenderung fur PZN: " + tblIncomingGoods->item(tblIncomingGoods->currentRow(), 1)->text() + " vorgenommen ";
					str3 +=  result.message.c_str();
					basar::VarString s3(str3.toStdString());
					basar::gui::tie::infoMsgBox(this,s3.c_str() , tr("Narcotics").toLocal8Bit().data());
				}
			}
		}
            //selectFirstIncomingGood(); //srk commented
		REINSTALL_PREVIOUS_CURSOR( cursor )
	}
    
}

//-------------------------------------------------------------------------------------------------//
/*
void IncomingGoodsOverviewVC::onTblIncomingGoods_closeEditor(QWidget*)
{
    QTableWidgetItem *p = tblIncomingGoods->currentItem();
    int col = tblIncomingGoods->column(p);
   if( tblIncomingGoods->column(p) == qtyColumn )
   {
       QString str1 = "Do You Want to Save PZN  " + tblIncomingGoods->item( tblIncomingGoods->currentRow(), 1 )->text();
       basar::VarString s1(str1.toStdString());
       basar::gui::tie::MessageBoxButtonEnum answer = basar::gui::tie::questionMsgBox( 0, s1.c_str() );
       if( answer == basar::gui::tie::ButtonYes )
       {
            onTblIncomingGoods_itemChanged(tblIncomingGoods->currentItem());
            if(col == qtyColumn)
            tblIncomingGoods->selectColumn(-1);
       }
   }
}
*/

void IncomingGoodsOverviewVC::onTblIncomingGoods_itemActivated( QTableWidgetItem* p)
{
    
    basar::ConstString fun = "IncomingGoodsOverviewVC::onTblIncomingGoods_itemActivated()";
    BLOG_TRACE_METHOD( LoggerPool::loggerViewConn, fun );
   if( tblIncomingGoods->item( tblIncomingGoods->currentRow(), 1 ) != nullptr )
   {
       if( tblIncomingGoods->column(p) == qtyColumn )
       {
           onTblIncomingGoods_itemChanged(tblIncomingGoods->currentItem());
       }
   }
}

//-------------------------------------------------------------------------------------------------//

void IncomingGoodsOverviewVC::onTblIncomingGoods_cellPressed( int , int col )
{
	basar::ConstString fun = "IncomingGoodsOverviewVC::onTblIncomingGoods_cellPressed()";
    BLOG_TRACE_METHOD( LoggerPool::loggerViewConn, fun );

    if(col == 5)
    {
       if( tblIncomingGoods->item( tblIncomingGoods->currentRow(), 1 ) != nullptr )
       {
           QString str1 = tr("Do You Want to Save PZN  ").toLocal8Bit().constData() + tblIncomingGoods->item( tblIncomingGoods->currentRow(), 1 )->text();
           basar::VarString s1(str1.toStdString());
           basar::gui::tie::MessageBoxButtonEnum answer = basar::gui::tie::questionMsgBox( 0, s1.c_str() );
           if( answer == basar::gui::tie::ButtonYes )
           {
                onTblIncomingGoods_itemChanged(tblIncomingGoods->currentItem());
           }
       }
   }
}

//-------------------------------------------------------------------------------------------------//
void IncomingGoodsOverviewVC::onTblIncomingGoods_cellDoubleClicked( int, int )
{
	basar::ConstString fun = "IncomingGoodsOverviewVC::onTblIncomingGoods_cellDoubleClicked()";
    BLOG_TRACE_METHOD( LoggerPool::loggerViewConn, fun );

	if( isIncomingGoodDisplayed() && isIncomingGoodSelected() )
	{
		{
			INSTALL_BUSY_CURSOR

			basar::appl::SystemEventManager::getInstance().fire( "RegisteredIncomingGood" );
			m_ManualDialog->init( m_matSupplier.getRight() );
			basar::appl::SystemEventManager::getInstance().fire( "SetCurrentIteratorIncomingGoods" );
			matchToNewBooking( getCurrentIncomingGood() );

			REINSTALL_PREVIOUS_CURSOR( cursor )
			m_ManualDialog->show();
		}

		{
			INSTALL_BUSY_CURSOR
			basar::appl::SystemEventManager::getInstance().fire( "RegisteredIncomingGood" );
			REINSTALL_PREVIOUS_CURSOR( cursor )
		}
		// reinitialize widget after changing entries
		basar::appl::EventReturnStruct result = basar::appl::SystemEventManager::getInstance().fire( "SearchIncomingGoods" );
		if( false == result.isHandlerOK() ){
			clearWidget();
		}
		PROCESS_EVENT_RETURN( result, this )
	}
}

//-------------------------------------------------------------------------------------------------//
void IncomingGoodsOverviewVC::onBtnUpdateSupplier_clicked()
{
	basar::ConstString fun = "IncomingGoodsOverviewVC::on_btnUpdateSupplier_clicked()";
    BLOG_TRACE_METHOD( LoggerPool::loggerViewConn, fun );

	if( basar::gui::tie::questionMsgBox( this,
		tr("Do you really want to replace the supplier?").toLocal8Bit().constData(),
		tr("Narcotics").toLocal8Bit().constData()) == basar::gui::tie::ButtonYes )
	{
		INSTALL_BUSY_CURSOR
		basar::appl::SystemEventManager::getInstance().fire( "ReplaceSupplier" );
	    REINSTALL_PREVIOUS_CURSOR( cursor )
	}
}

///////////////////////////////////////////////////////////////////////////////////////////////////
/*! \return	void
	\param	is not used here
	\throw	no-throw */
///////////////////////////////////////////////////////////////////////////////////////////////////
void IncomingGoodsOverviewVC::clearWidget()
{
    basar::ConstString fun = "IncomingGoodsOverviewVC::clearWidget()";
    BLOG_TRACE_METHOD( LoggerPool::loggerViewConn, fun );

    tblIncomingGoods->clearContents();
	tblIncomingGoods->setDisabled( true );
}

//-------------------------------------------------------------------------------------------------//
basar::gui::tie::WidgetReturnEnum IncomingGoodsOverviewVC::show()
{
	if( LoggerPool::loggerViewConn.isEnabledFor(log4cplus::TRACE_LOG_LEVEL) )
	{
		basar::ConstString msg = "show of IncomingGoodsOverviewVC.";
		LoggerPool::loggerViewConn.forcedLog(log4cplus::TRACE_LOG_LEVEL, msg, __FILE__, __LINE__);
	}

	m_IsFirstCall = false;

	return basar::gui::tie::getWidgetReturnType( exec() );
}

//-------------------------------------------------------------------------------------------------//
void IncomingGoodsOverviewVC::hide()
{
	if( LoggerPool::loggerViewConn.isEnabledFor(log4cplus::TRACE_LOG_LEVEL) )
	{
		basar::ConstString msg = "hide of IncomingGoodsOverviewVC.";
		LoggerPool::loggerViewConn.forcedLog(log4cplus::TRACE_LOG_LEVEL, msg, __FILE__, __LINE__);
	}

	return QWidget::hide();
}

//-------------------------------------------------------------------------------------------------//
bool IncomingGoodsOverviewVC::shutdown()
{
    basar::ConstString fun = "IncomingGoodsOverviewVC::shutdown()";
    BLOG_TRACE_METHOD( LoggerPool::loggerViewConn, fun );

    m_ManualDialog->shutdown();
	m_ManualDialog.reset();

	m_matNoReturnGoods.reset();
	m_matNoReturnGoods.reset();
	m_matIncomingGoods.reset();
	m_matSupplier.reset();

    return close();
}

//-------------------------------------------------------------------------------------------------//
void IncomingGoodsOverviewVC::matchToSelection( AccessorPropertyTable_YIterator searchIterator )
{
	basar::ConstString fun = "IncomingGoodsOverviewVC::matchToSelection()";
    BLOG_TRACE_METHOD( LoggerPool::loggerViewConn, fun );

    if( !orderNumber->text().isEmpty() ){
		searchIterator.setString( constants::PURCHASEORDERNO,orderNumber->text().toLocal8Bit().constData() );
    }

	// frameOrderSearch
    if( !articleNumber->text().isEmpty() )
	{
		basar::I18nString articleCode = articleNumber->text().toLocal8Bit().constData();
		searchIterator.setString( constants::ARTICLECODE, removeLeadingZeros(articleCode) );
    }

	if( !articleName->text().isEmpty() )
	{
		basar::I18nString searchname = articleName->text().toLocal8Bit().constData();
		searchname.upper();
		searchIterator.setString( constants::ARTICLENAME, searchname );
	}

	// frameSupplierSearch
    if( !supplierNo->text().isEmpty() )
	{
		searchIterator.setString( constants::CUSTOMERSUPPLIERNO, supplierNo->text().toLocal8Bit().constData() );
		searchIterator.setString( constants::SUPPLIERNO,		 supplierNo->text().toLocal8Bit().constData() );
    }

	if( !supplierName->text().isEmpty() )
	{
		basar::I18nString searchname = supplierName->text().toLocal8Bit().constData();
		searchname.upper();
		searchIterator.setString( constants::SUPPLIERNAME, searchname );
	}
}

//-------------------------------------------------------------------------------------------------//
void IncomingGoodsOverviewVC::matchFromIncomingGoods()
{
    basar::ConstString fun = "IncomingGoodsOverviewVC::matchFromIncomingGoods()";
    BLOG_TRACE_METHOD( LoggerPool::loggerViewConn, fun );

    tblIncomingGoods->blockSignals( true );

	tblIncomingGoods->clearContext();
    const basar::Int32 hits = m_matIncomingGoods.RightToLeft( 0, tblIncomingGoods->rowCount() );
	tblIncomingGoods->resizeColumnsToContents();
    activateIncomingGoodsDisplay( (hits == 0 ? false : true) );
    if( 0 < hits )
	{
        //if(bAktBtnClicked == false)
        {
            //selectFirstIncomingGood(); //srk commented
        }
        btnIncomingGoodsDelete->setEnabled( true );
    }
	else // nichts zu den Suchkriterien gefunden
	{
		if (false == articleName->text().isEmpty())
		{
			articleName->selectAll();
			articleName->setFocus();
		}
		else if (false == articleNumber->text().isEmpty())
		{
			articleNumber->selectAll();
			articleNumber->setFocus();
		}
		else
		{
			orderNumber->selectAll();
			orderNumber->setFocus();
		}
	}
    tblIncomingGoods->blockSignals( false );
}

//-------------------------------------------------------------------------------------------------//
void IncomingGoodsOverviewVC::selectFirstIncomingGood()
{
    BLOG_TRACE_METHOD( LoggerPool::loggerViewConn, "IncomingGoodsOverviewVC::selectFirstIncomingGood()" );

    if( false == isIncomingGoodDisplayed() ){
        return;
    }
    if( true == isIncomingGoodSelected() ){
        return;
    }
	if ( true == m_IsFirstCall )
		return;
    
    tblIncomingGoods->selectRow( 0 );
    tblIncomingGoods->setFocus();
    tblIncomingGoods->setCurrentCell( 0, qtyColumn );
    
}

//-------------------------------------------------------------------------------------------------//
void IncomingGoodsOverviewVC::matchToIncomingGoods()
{
	basar::ConstString fun = "IncomingGoodsOverviewVC::matchToIncomingGoods()";
    BLOG_TRACE_METHOD( LoggerPool::loggerViewConn, fun );

	m_matIncomingGoods.LeftToRight();
}

//-------------------------------------------------------------------------------------------------//
const bool IncomingGoodsOverviewVC::isIncomingGoodDisplayed()
{
    basar::ConstString fun = "IncomingGoodsOverviewVC::isIncomingGoodDisplayed()";
    BLOG_TRACE_METHOD( LoggerPool::loggerViewConn, fun );

    if( NULL == tblIncomingGoods->item( 0, 0 ) ){
        return false;
    }
    return true;
}

//-------------------------------------------------------------------------------------------------//
const bool IncomingGoodsOverviewVC::isIncomingGoodSelected()
{
    basar::ConstString fun = "IncomingGoodsOverviewVC::isIncomingGoodSelected()";
    BLOG_TRACE_METHOD( LoggerPool::loggerViewConn, fun );

    basar::db::aspect::AccessorPropertyTable_YIterator yit = m_matIncomingGoods.getCurrentRight();

    if( yit.isNull() || yit.isEnd() ){
        return false;
    }

    return true;
}

//-------------------------------------------------------------------------------------------------//
AccessorPropertyTable_YIterator IncomingGoodsOverviewVC::getCurrentIncomingGood()
{
	basar::ConstString fun = "IncomingGoodsOverviewVC::getCurrentIncomingGood()";
    BLOG_TRACE_METHOD( LoggerPool::loggerViewConn, fun );

    AccessorPropertyTable_YIterator yitCurrentIncomingGood;

    if( isIncomingGoodDisplayed() && isIncomingGoodSelected() ){
        yitCurrentIncomingGood = m_matIncomingGoods.getCurrentRight();
    }
    return yitCurrentIncomingGood;
}

//-------------------------------------------------------------------------------------------------//
void IncomingGoodsOverviewVC::getQtyFromCurrentRow(Int32 & currentQty)
{
	if( LoggerPool::loggerViewConn.isEnabledFor(log4cplus::TRACE_LOG_LEVEL) )
	{
		basar::ConstString msg = "getQtyFromCurrentRow of IncomingGoodsOverviewVC.";
		LoggerPool::loggerViewConn.forcedLog(log4cplus::TRACE_LOG_LEVEL, msg, __FILE__, __LINE__);
	}

	currentQty = tblIncomingGoods->item( tblIncomingGoods->currentRow(), qtyColumn )->text().toInt();
}

//-------------------------------------------------------------------------------------------------//
void IncomingGoodsOverviewVC::setQtyFromCurrentRow(Int32 currentQty)
{
	if( LoggerPool::loggerViewConn.isEnabledFor(log4cplus::TRACE_LOG_LEVEL) )
	{
		basar::ConstString msg = "setQtyFromCurrentRow of IncomingGoodsOverviewVC.";
		LoggerPool::loggerViewConn.forcedLog(log4cplus::TRACE_LOG_LEVEL, msg, __FILE__, __LINE__);
	}

	QTableWidgetItem* myItem;
	myItem = tblIncomingGoods->item( tblIncomingGoods->currentRow(), qtyColumn );
	QString help;
	help.setNum( currentQty );
	tblIncomingGoods->blockSignals( true );
	myItem->setText( help );
	tblIncomingGoods->blockSignals( false );
}

//-------------------------------------------------------------------------------------------------//
void IncomingGoodsOverviewVC::matchFromSupplier()
{
	basar::ConstString fun = "IncomingGoodsOverviewVC::matchFromSupplier()";
    BLOG_TRACE_METHOD( LoggerPool::loggerViewConn, fun );

	m_matSupplier.RightToLeft();
	cboSupplier->resizeColumnsToContents();
}

///////////////////////////////////////////////////////////////////////////////////////////////////
/*! \return void
	\throw	no-throw */
///////////////////////////////////////////////////////////////////////////////////////////////////
void IncomingGoodsOverviewVC::matchToArticleNo( basar::I18nString articleNoValue )
{
	if( LoggerPool::loggerViewConn.isEnabledFor(log4cplus::TRACE_LOG_LEVEL) )
	{
		basar::ConstString msg = "matchToArticleNo of IncomingGoodsOverviewVC.";
		LoggerPool::loggerViewConn.forcedLog(log4cplus::TRACE_LOG_LEVEL, msg, __FILE__, __LINE__);
	}

    // disable chkArticleName in order to clear it and enable articleNumber which can therefore now be filled with
    // passed articleNoValue.
	chkArticleName->setChecked( false );
	articleNumber->setText( QString::fromLocal8Bit(articleNoValue.c_str()) );
}

///////////////////////////////////////////////////////////////////////////////////////////////////
/*! \return void
	\throw	no-throw */
///////////////////////////////////////////////////////////////////////////////////////////////////
void IncomingGoodsOverviewVC::matchToSupplierNo( basar::I18nString supplierNoValue )
{
	if( LoggerPool::loggerViewConn.isEnabledFor(log4cplus::TRACE_LOG_LEVEL) )
	{
		basar::ConstString msg = "matchToSupplierNo of IncomingGoodsOverviewVC.";
		LoggerPool::loggerViewConn.forcedLog(log4cplus::TRACE_LOG_LEVEL, msg, __FILE__, __LINE__);
	}

    // disable chkSupplierName in order to clear it and enable supplierNo which can therefore now be filled with
    // passed supplierNoValue.
	chkSupplierName->setChecked( false );
	supplierNo->setText( QString::fromLocal8Bit(supplierNoValue.c_str()) );
}

//-------------------------------------------------------------------------------------------------//
const bool IncomingGoodsOverviewVC::isAlternativeSupplierDisplayed()
{
    basar::ConstString fun = "IncomingGoodsOverviewVC::isAlternativeSupplierDisplayed()";
    BLOG_TRACE_METHOD( LoggerPool::loggerViewConn, fun );

    if( -1 == cboSupplier->currentIndex() ){
        return false;
    }
    return true;
}

//-------------------------------------------------------------------------------------------------//
const bool IncomingGoodsOverviewVC::isAlternativeSupplierSelected()
{
    basar::ConstString fun = "IncomingGoodsOverviewVC::isAlternativeSupplierSelected()";
    BLOG_TRACE_METHOD( LoggerPool::loggerViewConn, fun );

    basar::db::aspect::AccessorPropertyTable_YIterator yitSupplier = m_matSupplier.getCurrentRight();
    if( yitSupplier.isNull() || yitSupplier.isEnd() ){
        return false;
    }

    return true;
}

//-------------------------------------------------------------------------------------------------//
AccessorPropertyTable_YIterator IncomingGoodsOverviewVC::getCurrentAlternativeSupplier()
{
    basar::ConstString fun = "IncomingGoodsOverviewVC::getCurrentAlternativeSupplier()";
    BLOG_TRACE_METHOD( LoggerPool::loggerViewConn, fun );

    AccessorPropertyTable_YIterator yitSupplier;

    if( isAlternativeSupplierDisplayed() && isAlternativeSupplierSelected() ){
        yitSupplier = m_matSupplier.getCurrentRight();
    }

    return yitSupplier;
}

//-------------------------------------------------------------------------------------------------//
void IncomingGoodsOverviewVC::matchFromNoIncomingGoods()
{
    basar::ConstString fun = "IncomingGoodsOverviewVC::matchFromNoIncomingGoods()";
    BLOG_TRACE_METHOD( LoggerPool::loggerViewConn, fun );

	const Int32 hits = m_matNoIncomingGoods.RightToLeft();
	m_matNoReturnGoods.RightToLeft();
    activateNumberOfGoodsDisplay( 0 < hits ? true : false );
}

//-------------------------------------------------------------------------------------------------//
void IncomingGoodsOverviewVC::matchToNewBooking( AccessorPropertyTable_YIterator newBooking )
{
	if( LoggerPool::loggerViewConn.isEnabledFor(log4cplus::TRACE_LOG_LEVEL) )
	{
		basar::ConstString msg = "matchToNewBooking of IncomingGoodsOverviewVC.";
		LoggerPool::loggerViewConn.forcedLog(log4cplus::TRACE_LOG_LEVEL, msg, __FILE__, __LINE__);
	}

	m_ManualDialog->matchToBooking( newBooking );
}

//-------------------------------------------------------------------------------------------------//
void IncomingGoodsOverviewVC::matchFromNewBooking( AccessorPropertyTable_YIterator newBooking )
{
	if( LoggerPool::loggerViewConn.isEnabledFor(log4cplus::TRACE_LOG_LEVEL) )
	{
		basar::ConstString msg = "matchFromNewBooking of IncomingGoodsOverviewVC.";
		LoggerPool::loggerViewConn.forcedLog(log4cplus::TRACE_LOG_LEVEL, msg, __FILE__, __LINE__);
	}

	m_ManualDialog->matchFromBooking( newBooking );
}

///////////////////////////////////////////////////////////////////////////////////////////////////
//!	\retval	true if one of the search criteria is set (orderno, articleno, articlename, supplierno or suppliername)
//!	\retval false otherwise
//!	\throw	no-throw */
///////////////////////////////////////////////////////////////////////////////////////////////////
const bool IncomingGoodsOverviewVC::isSearchAllowed()
{
    basar::ConstString fun = "IncomingGoodsOverviewVC::isSearchAllowed()";
    BLOG_TRACE_METHOD( LoggerPool::loggerViewConn, fun );

	return ( !orderNumber->text().isEmpty() ) || ( !articleNumber->text().isEmpty() ) ||
		   ( !articleName->text().isEmpty() ) || ( !supplierNo->text().isEmpty() ) || ( !supplierName->text().isEmpty() );
}

///////////////////////////////////////////////////////////////////////////////////////////////////
/*! \return	void
	\param	is not used here
	\throw	no-throw */
///////////////////////////////////////////////////////////////////////////////////////////////////
void IncomingGoodsOverviewVC::evaluateSearchExecution()
{
    basar::ConstString fun = "IncomingGoodsOverviewVC::evaluateSearchExecution()";
    BLOG_TRACE_METHOD( LoggerPool::loggerViewConn, fun );

    btnSearch->setEnabled( isSearchAllowed() );
    activateIncomingGoodsDisplay( false );
    activateNumberOfGoodsDisplay( false );
}

///////////////////////////////////////////////////////////////////////////////////////////////////
/*! \return	void
	\throw	no-throw */
///////////////////////////////////////////////////////////////////////////////////////////////////
void IncomingGoodsOverviewVC::onTblIncomingGoods_itemSelectionChanged()
{
	basar::ConstString fun = "IncomingGoodsOverviewVC::on_tblIncomingGoods_itemSelectionChanged()";
    BLOG_TRACE_METHOD( LoggerPool::loggerViewConn, fun );

	btnIncomingGoodsDelete->setDisabled( !isIncomingGoodDisplayed() || !isIncomingGoodSelected() );
}

///////////////////////////////////////////////////////////////////////////////////////////////////
/*! \return	void
	\param	is not used here
	\throw	no-throw */
///////////////////////////////////////////////////////////////////////////////////////////////////
void IncomingGoodsOverviewVC::onOrderNumber_textChanged( const QString& )
{
    basar::ConstString fun = "IncomingGoodsOverviewVC::onOrderNumber_textChanged()";
    BLOG_TRACE_METHOD( LoggerPool::loggerViewConn, fun );

    evaluateSearchExecution();
}

///////////////////////////////////////////////////////////////////////////////////////////////////
/*! \return	void
	\param	is not used here
	\throw	no-throw */
///////////////////////////////////////////////////////////////////////////////////////////////////
void IncomingGoodsOverviewVC::onArticleNumber_textChanged( const QString& )
{
    basar::ConstString fun = "IncomingGoodsOverviewVC::on_articleNumber_textChanged()";
    BLOG_TRACE_METHOD( LoggerPool::loggerViewConn, fun );

    evaluateSearchExecution();
}

///////////////////////////////////////////////////////////////////////////////////////////////////
/*! \return	void
	\param	is not used here
	\throw	no-throw */
///////////////////////////////////////////////////////////////////////////////////////////////////
void IncomingGoodsOverviewVC::onArticleName_textChanged( const QString& )
{
    basar::ConstString fun = "IncomingGoodsOverviewVC::on_articleName_textChanged()";
    BLOG_TRACE_METHOD( LoggerPool::loggerViewConn, fun );

    evaluateSearchExecution();
}

///////////////////////////////////////////////////////////////////////////////////////////////////
/*! \return	void
	\param	is not used here
	\throw	no-throw */
///////////////////////////////////////////////////////////////////////////////////////////////////
void IncomingGoodsOverviewVC::onSupplierNo_textChanged( const QString& )
{
    basar::ConstString fun = "IncomingGoodsOverviewVC::on_supplierNo_textChanged()";
    BLOG_TRACE_METHOD( LoggerPool::loggerViewConn, fun );

    evaluateSearchExecution();
}

///////////////////////////////////////////////////////////////////////////////////////////////////
/*! \return	void
	\param	is not used here
	\throw	no-throw */
///////////////////////////////////////////////////////////////////////////////////////////////////
void IncomingGoodsOverviewVC::onSupplierName_textChanged( const QString& )
{
    basar::ConstString fun = "IncomingGoodsOverviewVC::on_supplierName_textChanged()";
    BLOG_TRACE_METHOD( LoggerPool::loggerViewConn, fun );

    evaluateSearchExecution();
}

///////////////////////////////////////////////////////////////////////////////////////////////////
/*! \return	void
	\param	is not used here
	\throw	no-throw */
///////////////////////////////////////////////////////////////////////////////////////////////////
void IncomingGoodsOverviewVC::switchNextBookingsCreation( bool enable )
{
    basar::ConstString fun = "IncomingGoodsOverviewVC::switchNextBookingsCreation()";
    BLOG_TRACE_METHOD( LoggerPool::loggerViewConn, fun );

    m_ManualDialog->enableButtonStateNext( enable );
}

} //namespace viewConn
} //namespace narcotics