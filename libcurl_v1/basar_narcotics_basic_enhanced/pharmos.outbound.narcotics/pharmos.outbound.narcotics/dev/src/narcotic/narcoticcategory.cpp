//-------------------------------------------------------------------------------------------------//
// includes
//-------------------------------------------------------------------------------------------------//
#include "commonheader.h"
#include "narcoticstransformer.h"
#include "narcoticcategory.h"
#include "loggerpool.h"

//-------------------------------------------------------------------------------------------------//
// namespaces
//-------------------------------------------------------------------------------------------------//
namespace narcotics {
namespace viewConn {

using basar::VarString;
using basar::db::aspect::AccessorPropertyTableRef;

/////////////////////////////////////////////////////////////////////////////////////////////////////
// 
/////////////////////////////////////////////////////////////////////////////////////////////////////
NarcoticCategoryVC::NarcoticCategoryVC( QWidget *parent )
: QDialog( parent )
{
	setupUi(this);
	wireEvents();
}

/////////////////////////////////////////////////////////////////////////////////////////////////////
// 
/////////////////////////////////////////////////////////////////////////////////////////////////////
NarcoticCategoryVC::~NarcoticCategoryVC()
{
}

/////////////////////////////////////////////////////////////////////////////////////////////////////
// 
/////////////////////////////////////////////////////////////////////////////////////////////////////
void NarcoticCategoryVC::wireEvents()
{
	QObject::connect(	group,		SIGNAL( textChanged( QString )				),	
						this,		SLOT  (	onGroup_textChanged( QString )		));	
	QObject::connect(	searchname,	SIGNAL( textChanged( QString )				),	
						this,		SLOT  (	onSearchname_textChanged( QString )	));	
	QObject::connect(	btnSearch,	SIGNAL( clicked()							),	
						this,		SLOT  (	onBtnSearch_clicked()				));	
	QObject::connect(	btnClose,	SIGNAL( clicked()							),	
						this,		SLOT  (	onBtnClose_clicked()				));	
	QObject::connect(	optGroup,	SIGNAL( toggled( bool )						),	
						this,		SLOT  (	buttonGroupStateChanged()			));	
	QObject::connect(	optName,	SIGNAL( toggled( bool )						),	
						this,		SLOT  (	buttonGroupStateChanged()			));	
}

/////////////////////////////////////////////////////////////////////////////////////////////////////
// 
/////////////////////////////////////////////////////////////////////////////////////////////////////
void NarcoticCategoryVC::onBtnSearch_clicked()
{
	BLOG_TRACE_METHOD(LoggerPool::loggerViewConn, "NarcoticCategoryVC::onBtnSearch_clicked()");
	
	setCursor( Qt::WaitCursor );
	// important: always clear table widget before starting a new search
	setDisabled_frameNarcoticGroup( true );
	setDisabled_frameArticleData( true );

	basar::appl::EventReturnStruct ret = basar::appl::SystemEventManager::getInstance().fire( "SearchNarcoticsCategory" );

	setCursor( Qt::ArrowCursor );

	if( ret.ret == basar::appl::HANDLER_INFO ){
		basar::gui::tie::infoMsgBox( this, ret.message.c_str(), tr("Narcotics").toLocal8Bit().constData() );
	}
}

///////////////////////////////////////////////////////////////////////////////////////////////////
// 
///////////////////////////////////////////////////////////////////////////////////////////////////
void NarcoticCategoryVC::onBtnClose_clicked()
{
	BLOG_TRACE_METHOD(LoggerPool::loggerViewConn, "NarcoticCategoryVC::onBtnClose_clicked()");

	shutdown();
}

///////////////////////////////////////////////////////////////////////////////////////////////////
/*! \param	text completely passed content of the QTextEdit field 'group'.
	\throw	no-throw */  
///////////////////////////////////////////////////////////////////////////////////////////////////
void NarcoticCategoryVC::onGroup_textChanged( const QString& text )
{
	BLOG_TRACE_METHOD(LoggerPool::loggerViewConn, "NarcoticCategoryVC::onGroup_textChanged()");

	btnSearch->setDisabled( text.isEmpty() );
}

///////////////////////////////////////////////////////////////////////////////////////////////////
/*! \param	text completely passed content of the QTextEdit field 'searchname'.
	\throw	no-throw */  
///////////////////////////////////////////////////////////////////////////////////////////////////
void NarcoticCategoryVC::onSearchname_textChanged( const QString& text )
{
	BLOG_TRACE_METHOD(LoggerPool::loggerViewConn, "NarcoticCategoryVC::onSearchname_textChanged()");

	btnSearch->setDisabled( text.isEmpty() );
}

///////////////////////////////////////////////////////////////////////////////////////////////////
// 
///////////////////////////////////////////////////////////////////////////////////////////////////
void NarcoticCategoryVC::init( AccessorPropertyTableRef categorieTable )
{
	BLOG_TRACE_METHOD(LoggerPool::loggerViewConn, "NarcoticCategoryVC::init()");

	initGuiCtrls();
	m_matCatagory	= basar::gui::tie::Manager::getInstance().createMatcher( frame_category, categorieTable );
	std::list<VarString> attribute;
	attribute.push_back( constants::ARTICLECODE );
	attribute.push_back( "articlename"			);
	attribute.push_back( "articlepharmaform"	);
	attribute.push_back( "articlepackageunit"	);
	m_matCatagory.transformProperties( tw_article->horizontalHeaderLabels(), attribute );
}	

///////////////////////////////////////////////////////////////////////////////////////////////////
// 
///////////////////////////////////////////////////////////////////////////////////////////////////
void NarcoticCategoryVC::initGuiCtrls()
{
	QStringList list;
	list << tr("ArticleNo") << tr("ArticleName") << tr("PharmaForm") << tr("PackageUnit"); 
	tw_article->setHorizontalHeaderLabels( list );
	tw_article->hideVerticalHeader();
	for ( int i = 0; i < tw_article->columnCount(); i++ )
	{
		tw_article->setEditableColumn( i, false );
	}
	tw_article->resizeColumnsToContents();
	btnSearch->setDisabled( true );
	
	group->setText("");
	group->setDisabled( true );
	group->setValidator( new QRegExpValidator(QRegExp("[0-9]{0,5}"), this) );
	
	searchname->setText("");
	searchname->setDisabled( true );
	
	setDisabled_frameNarcoticGroup( true );
	setDisabled_frameArticleData( true );
}

/////////////////////////////////////////////////////////////////////////////////////////////////////
// 
/////////////////////////////////////////////////////////////////////////////////////////////////////
basar::gui::tie::WidgetReturnEnum NarcoticCategoryVC::show()
{
	BLOG_TRACE_METHOD(LoggerPool::loggerViewConn, "NarcoticCategoryVC::show()");

	return basar::gui::tie::getWidgetReturnType( exec() );
}

/////////////////////////////////////////////////////////////////////////////////////////////////////
// 
/////////////////////////////////////////////////////////////////////////////////////////////////////
void NarcoticCategoryVC::hide()
{
	BLOG_TRACE_METHOD(LoggerPool::loggerViewConn, "NarcoticCategoryVC::hide()");

	return QWidget::hide();
}

/////////////////////////////////////////////////////////////////////////////////////////////////////
// 
/////////////////////////////////////////////////////////////////////////////////////////////////////
bool NarcoticCategoryVC::shutdown()
{
	BLOG_TRACE_METHOD(LoggerPool::loggerViewConn, "NarcoticCategoryVC::shutdown()");

	m_matCatagory.reset();
	return close();
}

/////////////////////////////////////////////////////////////////////////////////////////////////////
// 
/////////////////////////////////////////////////////////////////////////////////////////////////////
void NarcoticCategoryVC::matchFromSelection( basar::db::aspect::AccessorPropertyTable_YIterator searchIt )  
{
	BLOG_TRACE_METHOD(LoggerPool::loggerViewConn, "NarcoticCategoryVC::matchFromSelection()");

	if( !group->text().isEmpty() ){
		searchIt.setString( constants::GROUP, group->text().toLocal8Bit().constData() );
	}
	if( !searchname->text().isEmpty() )
	{
		basar::I18nString nam = searchname->text().toLocal8Bit().constData();
		nam.upper();
		searchIt.setString( constants::NARCOTICNAME, nam );
	}
}

/////////////////////////////////////////////////////////////////////////////////////////////////////
// 
/////////////////////////////////////////////////////////////////////////////////////////////////////
void NarcoticCategoryVC::matchToCategory()
{
	BLOG_TRACE_METHOD(LoggerPool::loggerViewConn, "NarcoticCategoryVC::matchToCategory()");

	setDisabled_frameNarcoticGroup( false );
	setDisabled_frameArticleData( false );
	m_matCatagory.RightToLeft( 0, tw_article->rowCount() );
	tw_article->resizeColumnsToContents();
}

/////////////////////////////////////////////////////////////////////////////////////////////////////
// 
/////////////////////////////////////////////////////////////////////////////////////////////////////
void NarcoticCategoryVC::matchToGroup( basar::I18nString groupvalue )
{
	BLOG_TRACE_METHOD(LoggerPool::loggerViewConn, "NarcoticCategoryVC::matchToGroup()");

// enable group checkbox to set value for field group.
	optName->setChecked( false );
	optGroup->setChecked( true );
	group->setText( QString::fromLocal8Bit(groupvalue.c_str()) );
}

/////////////////////////////////////////////////////////////////////////////////////////////////////
// 
/////////////////////////////////////////////////////////////////////////////////////////////////////
void NarcoticCategoryVC::setDisabled_frameNarcoticGroup( bool disable )
{
	name->setDisabled( disable );
	packageunit->setDisabled( disable );
	concentration->setDisabled( disable );

	if( disable )
	{
		name->setText( "" );
		packageunit->setText( "" );
		concentration->setText( "" );
	}
}

/////////////////////////////////////////////////////////////////////////////////////////////////////
// 
/////////////////////////////////////////////////////////////////////////////////////////////////////
void NarcoticCategoryVC::setDisabled_frameArticleData( bool disable )
{
	tw_article->setDisabled( disable );

	if( disable ){
		tw_article->clearContext();
	}
}

/////////////////////////////////////////////////////////////////////////////////////////////////////
// 
/////////////////////////////////////////////////////////////////////////////////////////////////////
void NarcoticCategoryVC::buttonGroupStateChanged()
{
	BLOG_TRACE_METHOD(LoggerPool::loggerViewConn, "NarcoticCategoryVC::buttonGroupStateChanged()");

	if( optName->isChecked() )
	{
		searchname->setDisabled( false );
		searchname->setFocus();
	}
	else
	{
		searchname->setDisabled( true );
		searchname->setText( "" );
	}

	if( optGroup->isChecked() )
	{
		group->setDisabled( false );
		group->setFocus();
	}
	else
	{
		group->setDisabled( true );
		group->setText( "" );
	}
}

/////////////////////////////////////////////////////////////////////////////////////////////////////
// 
/////////////////////////////////////////////////////////////////////////////////////////////////////

} //namespace viewConn
} //namespace narcotics