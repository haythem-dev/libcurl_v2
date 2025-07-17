#ifndef TABLEWIGETMULTMATCHSELECTFRAME_H
#define TABLEWIGETMULTMATCHSELECTFRAME_H

#pragma warning (push)
#pragma warning(disable: 4127 4244 4251 4311 4312 4512 4800)
#include "usings.h"
#include <QT/QFrame.h>
#include "ui_tablewidgetmultmatchselectframe.h"
#pragma warning (pop)

class TableWidgetMultMatchSelectFrame : public QFrame
{
    Q_OBJECT

public:
    TableWidgetMultMatchSelectFrame(QWidget *parent = 0);
    ~TableWidgetMultMatchSelectFrame();

    void setMatcher(MatcherRef a1, MatcherRef a2, bool first);

    bool hasMatcher();
    void  showException(basar::Exception &ex);

private:

    bool hasMatcherShowMessageBoxAtFalse();

    void  matchRL( MatcherRef & accmatcher,MatcherRef & guimatcher );
    void  matchLR( MatcherRef & accmatcher,MatcherRef & guimatcher );
    void  matchRLWithPolicy( MatcherRef & accmatcher,MatcherRef & guimatcher, Int32 policy );
    void  matchLRWithPolicy( MatcherRef & accmatcher,MatcherRef & guimatcher, Int32 policy );
    void  matchRLWithPolicy( MatcherRef & accmatcher,MatcherRef & guimatcher );
    void  matchLRWithPolicy( MatcherRef & accmatcher,MatcherRef & guimatcher  );

private:

    Ui::TableWidgetMultMatchSelectFrameClass ui;

    std::pair<MatcherRef, MatcherRef> m_matcherPair;
    MatcherRef m_matcher;
    Int32 m_policy;

public slots:

    void on_comboBox_currentIndexChanged(int);
    void on_comboBox_textChanged(const QString &);

    void on_pushButton_LRownp_clicked();
    void on_pushButton_RLownp_clicked();
    void on_pushButton_LR21p_clicked();
    void on_pushButton_LR12p_clicked();
    void on_pushButton_RL21p_clicked();
    void on_pushButton_RL12p_clicked();

    void on_pushButton_LR_index_clicked();
    void on_pushButton_RL_index_clicked();
    void on_pushButton_LR_clicked();    
    void on_pushButton_RL_clicked();
    void on_pushButton_LR12_clicked();
    void on_pushButton_LR21_clicked();
    void on_pushButton_RL12_clicked();
    void on_pushButton_RL21_clicked();

};

#endif // TABLEWIGETMULTMATCHSELECTFRAME_H
