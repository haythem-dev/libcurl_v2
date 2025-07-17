//----------------------------------------------------------------------------
/*! \file
 *  \brief  all global statusline for a main window
 *  \author Roland Kiefert
 *  \date   09.12.2005
 */
//----------------------------------------------------------------------------

#include "libbasarguiqt_manager.h"
#include "aboutdlg.h"

#include "libbasarlogin_manager.h"

/*! \ingroup DIFF_WIN_UNIX */ 
#ifdef _WIN32
#define VC_EXTRALEAN			//!< less Visual C headers
#define WIN32_LEAN_AND_MEAN		//!< less Windows headers
#include <windows.h>
#include <shellapi.h>
#endif

//----------------------------------------------------------------------------
namespace basar {
namespace gui {
namespace qt {

//----------------------------------------------------------------------------
/*!	\throw no-throw */
AboutDlg::AboutDlg(QWidget *parent)
	: QDialog(parent, Qt::WindowTitleHint )
{
	ui.setupUi(this);
	ui.branch->setText(QString::fromLocal8Bit(login::Manager::getInstance().getAreaName().c_str()));
	ui.user  ->setText(QString::fromLocal8Bit(login::Manager::getInstance().getUserName().c_str()));

	ui.product  ->setText(QString::fromLocal8Bit(gui::qt::Manager::getInstance().getDef().programDescription.c_str()));
	ui.copyright->setText(QString::fromLocal8Bit(gui::qt::Manager::getInstance().getDef().copyright.c_str()));
	ui.version  ->setText(QString::fromLocal8Bit(gui::qt::Manager::getInstance().getDef().version.c_str()));

	ui.database   ->setText(QString::fromLocal8Bit(gui::qt::Manager::getInstance().getInit().database.c_str()));

    m_DocuLink = gui::qt::Manager::getInstance().getInit().dokulink;

    //if an extra link text is given by caller, use this as button label
    if (!gui::qt::Manager::getInstance().getInit().dokulinktext.empty())
    {
        ui.bt_doculink->setText(QString::fromLocal8Bit(gui::qt::Manager::getInstance().getInit().dokulinktext.c_str()));
    }
    else
    {
        //use link as button label
        ui.bt_doculink->setText(QString::fromLocal8Bit(m_DocuLink.c_str()));
    }

	ui.bt_doculink->adjustSize();

	QPalette palette = ui.bt_doculink->palette();
	palette.setColor(QPalette::ButtonText, Qt::blue);
	ui.bt_doculink->setPalette(palette);

	QFont font = ui.bt_doculink->font();
	font.setUnderline(true);
	ui.bt_doculink->setFont(font);

	ui.server->setText(QString::fromLocal8Bit(gui::qt::Manager::getInstance().getInit().host.c_str()));

	QStringList headerList;
	headerList << tr("Name") << tr("Version");
	ui.tw_dll->setHorizontalHeaderLabels( headerList );

	basar::DllStruct   dll;
	QTableWidgetItem * item;
	Int32              row = 0;
	bool               ok  = gui::qt::Manager::getInstance().getFirstDll(dll);
	
	while (ok)
	{
		ui.tw_dll->setRowCount(row+1);
		item = new QTableWidgetItem( QString::fromLocal8Bit(dll.name.c_str()) );
		item->setFlags(Qt::ItemIsEnabled);
		ui.tw_dll->setItem( row, 0, item );
		item = new QTableWidgetItem( QString::fromLocal8Bit(dll.pkgVersion.c_str()) );
		item->setFlags(Qt::ItemIsEnabled);
		ui.tw_dll->setItem( row, 1, item );
		row++;
		ok = gui::qt::Manager::getInstance().getNextDll(dll);
	}

	ui.tw_dll->resizeColumnsToContents();
	ui.tw_dll->resizeRowsToContents();

    QObject::connect(ui.bt_doculink, SIGNAL(clicked()), this, SLOT(link()));
	ui.bt_cancel->setFocus();
}

//----------------------------------------------------------------------------
/*!	\throw no-throw */
AboutDlg::~AboutDlg()
{
}

//----------------------------------------------------------------------------
/*!	\throw no-throw */
void AboutDlg::link()
{
	ShellExecute(NULL,								// parent window handle
		         "open",							// operation
                 m_DocuLink.c_str(),	// executed file
				 NULL,								// parameters
				 NULL,								// default directory
				 SW_SHOWNORMAL);					// flag how app is display when opened

}

//----------------------------------------------------------------------------
}	// namespace qt
}	// namespace gui
}	// namespace bassar
