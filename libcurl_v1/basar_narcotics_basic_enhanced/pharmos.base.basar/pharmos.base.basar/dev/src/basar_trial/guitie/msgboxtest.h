#ifndef MSGBOXTEST_H
#define MSGBOXTEST_H

#include <libbasar_definitions.h>
#pragma warning (push)
#pragma warning(disable: 4127 4244 4251 4311 4312 4512 4800)
#include <QT/QFrame.h>
#include "ui_msgboxtest.h"
#pragma warning (pop)
#include <libbasarguitie_msgboxes.h>


class MsgBoxTest : public QFrame
{
    Q_OBJECT

public:
    MsgBoxTest(QWidget *parent = 0);
    ~MsgBoxTest();

private:
	void showMessageBox( QWidget * parent );
	const basar::gui::tie::MessageBoxButtons getButtons() const;
	const QString getResultText( basar::gui::tie::MessageBoxButtonEnum button ) const;

	Ui::MsgBoxTestClass ui;

private slots:
	void on_cbType_currentIndexChanged( int index );
	void on_btShowNull_clicked();
	void on_btShowThis_clicked();
};

#endif // MSGBOXTEST_H
