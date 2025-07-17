#include "testguitie.h"

#include <libbasarcmnutil.h>
#include <libbasarappl.h>
#include <libbasarguitie.h>

#pragma warning (push)
#pragma warning(disable: 4311 4312 4127 4251 4512)
#include <Qt/qdialog.h>
#include <Qt/qwidget.h>
#pragma warning (pop)


//------------------------------------------------------------------------------
class ViewConn1 : public QDialog
{
public:

	ViewConn1(QWidget *parent)
		: QDialog(parent)
	{ // hier wird wirlich konstruiert, d.h. Qt reserviert erst jetzt Speicherplatz
	}

	~ViewConn1()
	{
	}

	// typische Methoden zur Illustration

	void init()
	{
	}

	void show()
	{
	}

	void shutdown()
	{
	}
};

//------------------------------------------------------------------------------
class ViewConn2 : public QDialog
{
public:

	ViewConn2(QWidget *parent)
		: QDialog(parent)
	{ // hier wird wirlich konstruiert, d.h. Qt reserviert erst jetzt Speicherplatz
	}

	~ViewConn2()
	{
	}

	// typische Methoden zur Illustration

	void init()
	{
	}

	void show()
	{
	}

	void shutdown()
	{
	}
};

//------------------------------------------------------------------------------
// Diese Zeile steht in der Applikation
	basar::gui::tie::ViewConnPtr<ViewConn1> m_VC1;
	basar::gui::tie::ViewConnPtr<ViewConn2> m_VC2;


//------------------------------------------------------------------------------
void f(basar::gui::tie::ViewConnPtr<ViewConn1> vcptr)
{

	if (vcptr == m_VC1) // Manchmal muss
	// der UseCaseController vergleichen ...
	{
		bool b;
		b = true;
	}

// Die folgende Zeile geht nicht durch den Compiler,
// da der globale operator==() schon erkennt, dass
// die Typen unvergleichbar sind.

/*
	if (vcptr == m_VC2) // Manchmal muss
	// der UseCaseController vergleichen ...
	{
	}

*/


}

//------------------------------------------------------------------------------
void handleIrgendwas(basar::appl::IEventSource& ev)
{
	//static_cast<basar::gui::tie::ViewConnPtr<ViewConn1>&>(ev)->show();
	
	if (ev == m_VC1) // Manchmal muss
	// der UseCaseController vergleichen ...
	{
		bool b;
		b = true;
	}

	if (ev == basar::appl::NullEventSource::getInstance()) // Manchmal muss
	// der UseCaseController vergleichen ...
	{
		bool b;
		b = true;
	}
	if (m_VC1 == ev) // Manchmal muss
	// der UseCaseController vergleichen ...
	{
		bool b;
		b = true;
	}

	if (m_VC2 == ev) // Manchmal muss
	// der UseCaseController vergleichen ...
	{
		bool b;
		b = true;
	}

	// Manchmal benutzt der Programmierer von UCCs
	// Hilfsfunktionen, die einen konkreten ViewConPtr
	// als Parameter erwarten
	// Die Syntax zur Umwandlung von der EventSource
	// muss er beherrschen (Bsp. im APM!)

	if (ev == m_VC1)
	{
		f(static_cast<basar::gui::tie::ViewConnPtr<ViewConn1>&>(ev));
	}
}

//------------------------------------------------------------------------------
void test_viewconnptr()
{
	// Im Use-Case-Controller
	m_VC1->init();
	m_VC1->show();
	m_VC1->shutdown();

	// irgendwann ruft der SystemEventManager
	//handleIrgendwas(m_VC1);
	handleIrgendwas(basar::appl::NullEventSource::getInstance());

	m_VC1.reset();
}
