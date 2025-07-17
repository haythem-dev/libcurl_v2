#ifndef NARCOTICSPROTOCOLVC_H
#define NARCOTICSPROTOCOLVC_H

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
#include "ui_narcoticsprotocolvc.h"
#pragma warning (pop)

//-------------------------------------------------------------------------------------------------//
// namespace
//-------------------------------------------------------------------------------------------------//
namespace narcotics 
{
namespace viewConn 
{

class NarcoticsProtocolVC : public QDialog, public Ui::narcoticsprotocolvcClass,
							public boost::enable_shared_from_this<NarcoticsProtocolVC>,
							public basar::gui::tie::IViewConnector
{
    Q_OBJECT

public:
    NarcoticsProtocolVC(QWidget *parent = 0);
    ~NarcoticsProtocolVC();

	void			init						( basar::db::aspect::AccessorPropertyTableRef protocolTable ,	//!< property table for Protocol
												  basar::db::aspect::AccessorPropertyTableRef catalogTable		//!< property table for TransactionCatalog
												);

	basar::gui::tie::WidgetReturnEnum	show	(); 
	void			hide						(); 
	bool			shutdown					(); 

	void			matchFromFirstSelection		( basar::db::aspect::AccessorPropertyTable_YIterator searchIt //!< iterator for the search parameter
												); 

	void			matchToProtocol				();
	void			matchToCatalog				();

	basar::db::aspect::AccessorPropertyTable_YIterator	getCatalogId();
	
private:
	void wireEvents();
	void initGuiCtrls();							

	basar::gui::tie::MatcherRef m_matCatalog;							//!< Matcher for TransactionCatalog
	basar::gui::tie::MatcherRef m_matProtocol;							//!< Matcher for Protocol

private slots:
	void onBtnClose_clicked();
	void onBtnPrint_clicked();
	void onBtnSearch_clicked();
	void onTwCatalog_itemSelectionChanged();
};

} //namespace viewConn
} //namespace narcotics

#endif //NARCOTICSPROTOCOLVC_H