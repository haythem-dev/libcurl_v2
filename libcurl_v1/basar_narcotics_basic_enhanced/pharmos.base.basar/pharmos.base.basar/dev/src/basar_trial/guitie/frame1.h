#ifndef FRAME1_H
#define FRAME1_H

#pragma warning (push)
#pragma warning(disable: 4127 4244 4251 4311 4312 4512 4800)
#include "usings.h"
#include <QT/QFrame.h>
#include "ui_frame1.h"
#pragma warning (pop)


class Frame1 : public QFrame
{
    Q_OBJECT

public:
    Frame1(QWidget *parent = 0);
    ~Frame1();

private:
    Ui::Frame1Class ui;

    MatcherRef m_MR;
	AccessorInstanceRef m_AI;

private slots:
	void on_erasePushButton_clicked();
	void on_tableWidget_clicked(const QModelIndex &);
	void on_checkBox_stateChanged(int);
	void on_neuerDialogPushButton_2_clicked();
	void on_schliessenPushButton_clicked();
	void on_openPEPushButton_clicked();
	void on_qTableClearenPushButton_3_2_clicked();
	void on_qTableFuellenPushButton_3_clicked();
	void on_childrenEinlesenPushButton_clicked();
	void on_pushButton_2_clicked();
	void on_pushButton_clicked();
	void on_selectPushButton_clicked();
};

class MyTransformer : public Transformer
{
public:
	boost::shared_ptr<Transformer> create();
protected:
	virtual void	RightToLeft (basar::gui::tie::GuiPropertyTable_YIterator              guiIt,
		                         const basar::db::aspect::AccessorPropertyTable_YIterator accIt
								);
	virtual void	LeftToRight (const basar::gui::tie::GuiPropertyTable_YIterator  guiIt,
		                         basar::db::aspect::AccessorPropertyTable_YIterator accIt
								);
};

#endif // FRAME1_H
