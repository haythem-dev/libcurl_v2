#ifndef TABLEWIDGETDATASELECTIONCHANGEDTESTFRAME_H
#define TABLEWIDGETDATASELECTIONCHANGEDTESTFRAME_H


#pragma warning (push)
#pragma warning(disable: 4127 4244 4251 4311 4312 4512 4800)
#include "usings.h"
#include <QT/QFrame.h>
#include <QT/QShortcut.h>
#include "selects.h"
#include "ui_tablewidgetdataselectionchangedtestframe.h"
#pragma warning (pop)

class TableWidgetdataSelectionChangedTestFrame : public QWidget
{
	Q_OBJECT

public:
	TableWidgetdataSelectionChangedTestFrame(QWidget *parent = 0);
	~TableWidgetdataSelectionChangedTestFrame();

public slots:
	void dataSelectionChanged();

private:
	Ui::TableWidgetdataSelectionChangedTestFrameClass ui;

	MatcherRef m_matcher;
	AccessorInstanceRef m_accessor;
	int m_times;
	 basar::db::aspect::CachingPolicyEnum m_cachingPolicy;
	 basar::gui::tie::MemoryPolicyEnum m_memoryPolicy;

     void change();
private slots:
	void on_checkBox_stateChanged(int);
	void on_comboBox_2_currentIndexChanged(int);

	void on_pushButton_3_clicked();
	void on_comboBox_currentIndexChanged(int);
	void on_pushButton_2_clicked();
	void on_pushButton_clicked();
};

#endif // TABLEWIDGETDATASELECTIONCHANGEDTESTFRAME_H
