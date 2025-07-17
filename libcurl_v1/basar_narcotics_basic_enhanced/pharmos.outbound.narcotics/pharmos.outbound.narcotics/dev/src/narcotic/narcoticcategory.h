//----------------------------------------------------------------------------
/*! \file
 *  \brief  Vieconnector interface for narcotics category
 *  \author Roland Kiefert
 *  \date   22.03.2006
 */
//----------------------------------------------------------------------------
#ifndef GUARD_NARCOTICCATEGORY_H
#define GUARD_NARCOTICCATEGORY_H

//-------------------------------------------------------------------------------------------------//
// includes
//-------------------------------------------------------------------------------------------------//
#include <boost/enable_shared_from_this.hpp>
#include <libbasardbaspect.h>
#include <libbasarappl.h>
#include <libbasarguitie.h>

#pragma warning (push)
#pragma warning(disable : 4127 4311 4312 4481 4512 4800 4244)
#include <QtWidgets/qdialog.h>
#include "ui_narcoticcategory.h"
#pragma warning (pop)

//-------------------------------------------------------------------------------------------------//
// namespace
//-------------------------------------------------------------------------------------------------//
namespace narcotics 
{
namespace viewConn 
{

class NarcoticCategoryVC :	public QDialog, public Ui::NarcoticCategoryClass,
							public boost::enable_shared_from_this<NarcoticCategoryVC>,
							public basar::gui::tie::IViewConnector
{
    Q_OBJECT

public:
    NarcoticCategoryVC							( QWidget *parent = 0 );
    ~NarcoticCategoryVC							();
	void			init						( basar::db::aspect::AccessorPropertyTableRef categorieTable ); 
	basar::gui::tie::WidgetReturnEnum	show	(); 
	void			hide						(); 
	bool			shutdown					(); 
	void			matchFromSelection			( basar::db::aspect::AccessorPropertyTable_YIterator searchIt );
	void			matchToCategory				();
	void			matchToGroup				( basar::I18nString groupvalue );

private slots:
	void onGroup_textChanged( const QString & );
	void onSearchname_textChanged( const QString & );
	void onBtnSearch_clicked();
	void onBtnClose_clicked();
	void buttonGroupStateChanged(); // controls enabling and disabling of Gui searchfields according to checkboxstates

private:
	void wireEvents();
	void initGuiCtrls(); 
	/*! \brief	Enables or disables the specific frame including its controls. \n  no-throw */
	void setDisabled_frameNarcoticGroup( bool disable );
	/*! \brief	Enables or disables the specific frame including its controls. \n  no-throw */
	void setDisabled_frameArticleData( bool disable );
	basar::gui::tie::MatcherRef m_matCatagory;
};

} //namespace viewConn
} //namespace narcotics

#endif //GUARD_NARCOTICCATEGORY_H