#ifndef TABLEWIDGETMATCHANDSIZETESTFRAME_H
#define TABLEWIDGETMATCHANDSIZETESTFRAME_H


#pragma warning (push)
#pragma warning(disable: 4127 4244 4251 4311 4312 4512 4800)
#include "usings.h"
#include <QT/QFrame.h>
#include <QT/QShortcut.h>
#include "selects.h"
#include "ui_tablewidgetmatchandsizetestframe.h"
#pragma warning (pop)

class TableWidgetMatchAndSizeTestFrame : public QWidget
{
	Q_OBJECT

public:
	TableWidgetMatchAndSizeTestFrame(QWidget *parent = 0);
	~TableWidgetMatchAndSizeTestFrame();

private:
	Ui::TableWidgetMatchAndSizeTestFrameClass ui;

	MatcherRef m_matcher;
	AccessorInstanceRef m_accessor;
	int m_times;
	basar::db::aspect::CachingPolicyEnum m_cachingPolicy;
	basar::gui::tie::MemoryPolicyEnum m_memoryPolicy;

    void createAccessorAndMatch();
	void setColumnHeaders();

private slots:
	void on_comboCachingPolicy_currentIndexChanged( int );
	void on_comboMemoryPolicy_currentIndexChanged( int );
	void on_pbClear_clicked();
	void on_pbMatch_clicked();
	void on_pbResizeColumns_clicked();
	void on_pbResizeRows_clicked();
	void on_cbExceptions_stateChanged( int );
};

#endif // TABLEWIDGETMATCHANDSIZETESTFRAME_H
