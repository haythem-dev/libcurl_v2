#include "tablewidgetmultmatchselectframe.h"

TableWidgetMultMatchSelectFrame::TableWidgetMultMatchSelectFrame(QWidget *parent)
: QFrame(parent)
{
    ui.setupUi(this);
    ui.comboBox->addItem("0 (clean)", QVariant(0));
    ui.comboBox->addItem("INSERT", QVariant(basar::gui::tie::INSERT));
    ui.comboBox->addItem("CONVERTTYPE", QVariant(basar::gui::tie::CONVERTTYPE));
    ui.comboBox->addItem("SKIP ", QVariant(basar::gui::tie::SKIP));
    ui.comboBox->addItem("DISREGARD_DELETED", QVariant(basar::gui::tie::DISREGARD_DELETED));
    ui.comboBox->addItem("INSERT | CONVERTTYPE | SKIP", QVariant(int(basar::gui::tie::CONVERTTYPE | basar::gui::tie::INSERT | basar::gui::tie::SKIP)));

}

void TableWidgetMultMatchSelectFrame::showException(basar::Exception &ex)
{
   QMessageBox::information(this, "Exception", QString(ex.what().c_str()));
}

void TableWidgetMultMatchSelectFrame::setMatcher(MatcherRef a1, MatcherRef a2, bool first)
{
    m_matcherPair.first = a1;
    m_matcherPair.second = a2;
    if (first)
    {
        ui.groupBox->setTitle("Matcher 1");
        m_matcher = a1;
    }
    else
    {
        ui.groupBox->setTitle("Matcher 2");
        m_matcher = a2;
    }
}

bool TableWidgetMultMatchSelectFrame::hasMatcher()
{
    return (!(m_matcherPair.first.isNull() || m_matcherPair.second.isNull()));
}

bool  TableWidgetMultMatchSelectFrame::hasMatcherShowMessageBoxAtFalse()
{
    bool retval = hasMatcher();
    if (!retval)
    {
        QMessageBox::information(this, "Error", "Not possible");
    }
    return retval;
}

void   TableWidgetMultMatchSelectFrame::matchRL(MatcherRef & guimatcher, MatcherRef & accmatcher)
{
    GuiPropertyTable_YIterator guiiter =  guimatcher.getLeft().begin();
    AccessorPropertyTable_YIterator acciter = accmatcher.getRight().begin();
    for ( ; (!acciter.isEnd()) && (!guiiter.isEnd()) ; ++acciter, ++guiiter )
    {
        m_matcher.RightToLeft(guiiter,acciter);
    }
}

void   TableWidgetMultMatchSelectFrame::matchLR(MatcherRef & guimatcher,MatcherRef & accmatcher)
{
    GuiPropertyTable_YIterator guiiter =  guimatcher.getLeft().begin();
    AccessorPropertyTable_YIterator acciter = accmatcher.getRight().begin();
    for ( ; (!acciter.isEnd()) && (!guiiter.isEnd()) ; ++acciter, ++guiiter )
    {
        m_matcher.LeftToRight(guiiter,acciter);
    }
}
void TableWidgetMultMatchSelectFrame::matchRLWithPolicy(MatcherRef & guimatcher, MatcherRef & accmatcher )
{
    matchRLWithPolicy(guimatcher,accmatcher,m_policy);
}

void TableWidgetMultMatchSelectFrame::matchLRWithPolicy(MatcherRef & guimatcher, MatcherRef & accmatcher )
{
    matchLRWithPolicy(guimatcher,accmatcher,m_policy);
}

void TableWidgetMultMatchSelectFrame::matchRLWithPolicy(MatcherRef & guimatcher, MatcherRef & accmatcher, Int32 policy )
{
    GuiPropertyTable_YIterator guiiter =  guimatcher.getLeft().begin();
    AccessorPropertyTable_YIterator acciter = accmatcher.getRight().begin();
    for ( ; (!acciter.isEnd()) && (!guiiter.isEnd()) ; ++acciter, ++guiiter )
    {
        m_matcher.RightToLeft(guiiter,acciter, policy);
    }
}



void   TableWidgetMultMatchSelectFrame::matchLRWithPolicy(MatcherRef & guimatcher,MatcherRef & accmatcher, Int32 policy)
{
    GuiPropertyTable_YIterator guiiter =  guimatcher.getLeft().begin();
    AccessorPropertyTable_YIterator acciter = accmatcher.getRight().begin();
    for ( ; (!acciter.isEnd()) && (!guiiter.isEnd()) ; ++acciter, ++guiiter )
    {
        m_matcher.LeftToRight(guiiter,acciter, policy);
    }
}

TableWidgetMultMatchSelectFrame::~TableWidgetMultMatchSelectFrame()
{

}


void TableWidgetMultMatchSelectFrame::on_pushButton_RL_clicked()
{
    try
    {
        if (hasMatcherShowMessageBoxAtFalse())
        {
            m_matcher.RightToLeft();    
        }
    }
    catch (basar::Exception & ex)
    {
        showException(ex);
    }
}

void TableWidgetMultMatchSelectFrame::on_pushButton_LR_clicked()
{
    try
    {
        if (hasMatcherShowMessageBoxAtFalse())
        {
            m_matcher.LeftToRight();
        }
    }
    catch (basar::Exception & ex)
    {
        showException(ex);
    }
}


void TableWidgetMultMatchSelectFrame::on_pushButton_RL21_clicked()
{
    try
    {
        if (hasMatcherShowMessageBoxAtFalse())
        {
            matchRL(m_matcherPair.second,m_matcherPair.first);
        }
    }
    catch (basar::Exception & ex)
    {
        showException(ex);
    }
}

void TableWidgetMultMatchSelectFrame::on_pushButton_RL12_clicked()
{
    try
    {
        if (hasMatcherShowMessageBoxAtFalse())
        {
            matchRL(m_matcherPair.first,m_matcherPair.second);
        }
    }
    catch (basar::Exception & ex)
    {
        showException(ex);
    }
}


void TableWidgetMultMatchSelectFrame::on_pushButton_LR21_clicked()
{
    try
    {
        if (hasMatcherShowMessageBoxAtFalse())
        {
            matchLR(m_matcherPair.second,m_matcherPair.first);
        }
    }
    catch (basar::Exception & ex)
    {
        showException(ex);
    }
}

void TableWidgetMultMatchSelectFrame::on_pushButton_LR12_clicked()
{
    try
    {
        if (hasMatcherShowMessageBoxAtFalse())
        {
            matchLR(m_matcherPair.first,m_matcherPair.second);
        }
    }
    catch (basar::Exception & ex)
    {
        showException(ex);
    }
}



void TableWidgetMultMatchSelectFrame::on_pushButton_RL_index_clicked()
{
    try
    {
        if (hasMatcherShowMessageBoxAtFalse())
        {
            m_matcher.RightToLeft(2,5);
        }
    }
    catch (basar::Exception & ex)
    {
        showException(ex);
    }
}

void TableWidgetMultMatchSelectFrame::on_pushButton_LR_index_clicked()
{
    try
    {
        if (hasMatcherShowMessageBoxAtFalse())
        {
            m_matcher.LeftToRight(2,5);
        }
    }
    catch (basar::Exception & ex)
    {
        showException(ex);
    }
}

void TableWidgetMultMatchSelectFrame::on_pushButton_RL12p_clicked()
{
    try
    {
        if (hasMatcherShowMessageBoxAtFalse())
        {
            matchRLWithPolicy(m_matcherPair.first,m_matcherPair.second);
        }
    }
    catch (basar::Exception & ex)
    {
        showException(ex);
    }
}

void TableWidgetMultMatchSelectFrame::on_pushButton_RL21p_clicked()
{
    try
    {
        if (hasMatcherShowMessageBoxAtFalse())
        {
            matchRLWithPolicy(m_matcherPair.second,m_matcherPair.first);
        }
    }
    catch (basar::Exception & ex)
    {
        showException(ex);
    }
}

void TableWidgetMultMatchSelectFrame::on_pushButton_RLownp_clicked()
{
    try
    {
        if (hasMatcherShowMessageBoxAtFalse())
        {
            matchRLWithPolicy(m_matcher ,m_matcher );
        }
    }
    catch (basar::Exception & ex)
    {
        showException(ex);
    }
}

void TableWidgetMultMatchSelectFrame::on_pushButton_LR12p_clicked()
{
    try
    {
        if (hasMatcherShowMessageBoxAtFalse())
        {
            matchLRWithPolicy(m_matcherPair.first,m_matcherPair.second);
        }
    }
    catch (basar::Exception & ex)
    {
        showException(ex);
    }
}

void TableWidgetMultMatchSelectFrame::on_pushButton_LR21p_clicked()
{
    try
    {
        if (hasMatcherShowMessageBoxAtFalse())
        {
            matchLRWithPolicy(m_matcherPair.second,m_matcherPair.first);
        }
    }
    catch (basar::Exception & ex)
    {
        showException(ex);
    }
}

void TableWidgetMultMatchSelectFrame::on_pushButton_LRownp_clicked()
{
    try
    {
        if (hasMatcherShowMessageBoxAtFalse())
        {
            matchLRWithPolicy(m_matcher ,m_matcher );
        }
    }
    catch (basar::Exception & ex)
    {
        showException(ex);
    }
}

void TableWidgetMultMatchSelectFrame::on_comboBox_textChanged(const QString &)
{
    QMessageBox::information(this, "Error", "does not change policy");
}

void TableWidgetMultMatchSelectFrame::on_comboBox_currentIndexChanged(int number)
{
    bool ok = true;
    m_policy = ui.comboBox->itemData(number).toInt(&ok);
}