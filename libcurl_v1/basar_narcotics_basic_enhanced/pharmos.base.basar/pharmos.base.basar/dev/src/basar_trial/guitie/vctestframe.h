#ifndef VCTESTFRAME_H
#define VCTESTFRAME_H
 
#pragma warning (push)
#pragma warning(disable: 4127 4244 4251 4311 4312 4512 4800)
#include "usings.h"
#include <QT/QFrame.h>
#include "ui_vctestframe.h"
#pragma warning (pop)
#include "test1vc.h"
#include "libbasarguitie.h"

class VCTestFrame : public QFrame
{
	Q_OBJECT

public:
	VCTestFrame(QWidget *parent = 0);
	~VCTestFrame();

private:
	Ui::VCTestFrameClass ui;
	bool m_withParent;
	
	basar::gui::tie::ViewConnPtr<Test1VC> m_vcp;

	struct Policies
	{
		bool m_withTemporaryParent;
		bool m_store;
		bool m_showTwice;
	} ;

	void show(const Policies & policies);
    void  checkStoredVC();


private slots: 
	void on_pushButton_3_clicked();
	void on_pushButton_2_clicked();
	void on_textEdit_textChanged();
	void on_radioButton_2_toggled(bool);
	void on_radioButton_toggled(bool);
	void on_pushButton_clicked();
};

#endif // VCTESTFRAME_H
