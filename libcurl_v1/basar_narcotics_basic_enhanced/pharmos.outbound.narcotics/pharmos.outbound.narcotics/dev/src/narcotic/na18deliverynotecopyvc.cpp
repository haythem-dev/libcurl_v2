//-----------------------------------------------------------------------------------------------//
// includes
//-----------------------------------------------------------------------------------------------//
#include "commonheader.h"
#include "na18deliverynotecopyvc.h"
#include "loggerpool.h"
#include "qvalidatoritemdelegate.h"
#include "checkboxtabletransformer.h"

//-----------------------------------------------------------------------------------------------//
// namespaces
//-----------------------------------------------------------------------------------------------//
namespace narcotics {
namespace viewConn {

//-----------------------------------------------------------------------------------------------//
//!	\throw	no-throw */
//-----------------------------------------------------------------------------------------------//
NA18DeliveryNoteCopyVC::NA18DeliveryNoteCopyVC(QWidget* parent) : QDialog(parent)
{
	BLOG_TRACE_METHOD(LoggerPool::loggerViewConn, "NA18DeliveryNoteCopyVC::NA18DeliveryNoteCopyVC()");

	setupUi(this);
	createLayout();
	wireEvents();
}

//-----------------------------------------------------------------------------------------------//
//!	\throw	no-throw */
//-----------------------------------------------------------------------------------------------//
NA18DeliveryNoteCopyVC::~NA18DeliveryNoteCopyVC()
{
	BLOG_TRACE_METHOD(LoggerPool::loggerViewConn, "NA18DeliveryNoteCopyVC::~NA18DeliveryNoteCopyVC()");
}

//-----------------------------------------------------------------------------------------------//
//!	\throw	no-throw */
//-----------------------------------------------------------------------------------------------//
void NA18DeliveryNoteCopyVC::wireEvents()
{
	BLOG_TRACE_METHOD(LoggerPool::loggerViewConn, "NA18DeliveryNoteCopyVC::wireEvents()");

	QObject::connect(btnOK,				SIGNAL(clicked()				),
					this,				SLOT  (onBtnOk_clicked()		));
	QObject::connect(btnCancel,			SIGNAL(clicked()				),		
					this,				SLOT  (onBtnCancel_clicked()	));
	QObject::connect(rectificationcomment,	SIGNAL(textChanged()		),
					this,				SLOT  (textEdit_textChanged()	));
	QObject::connect(rbTheftLoss,		SIGNAL(toggled(bool)			),
					this,				SLOT  (onReason_toggled(bool)	));
	QObject::connect(rbRefusalOfAcceptance,	SIGNAL(toggled(bool)		),
					this,				SLOT  (onReason_toggled(bool)	));
	QObject::connect(rbQuantityCorrection,	SIGNAL(toggled(bool)		),
					this,				SLOT  (onReason_toggled(bool)	));
	QObject::connect(tabPositions,		SIGNAL(cellChanged(int, int)	),
					this,				SLOT(onPositions_QuantityChanged(int, int)));
}

//-----------------------------------------------------------------------------------------------//
//!	\throw	no-throw */
//-----------------------------------------------------------------------------------------------//
void NA18DeliveryNoteCopyVC::createLayout()
{
	BLOG_TRACE_METHOD(LoggerPool::loggerViewConn, "NA18DeliveryNoteCopyVC::createLayout()");

	QGridLayout* contributionVoucherLayout = new QGridLayout(grpContributionVoucher);
	contributionVoucherLayout->addWidget(labelContributionVoucherNo,	1, 1);
	contributionVoucherLayout->addWidget(labelCustomerNo,				2, 1);
	contributionVoucherLayout->addWidget(labelPurchaseorderNo,			3, 1);

	contributionVoucherLayout->addWidget(contributionvoucherno,		1, 2);
	contributionVoucherLayout->addWidget(customersupplierno,		2, 2);
	contributionVoucherLayout->addWidget(purchaseorderno,			3, 2);

	contributionVoucherLayout->addWidget(customersuppliername,		1, 3);
	contributionVoucherLayout->addWidget(customersupplierlocation,	2, 3);

	QVBoxLayout* reasonLayout = new QVBoxLayout(grpRectificationReason);
	reasonLayout->addWidget(rbTheftLoss);
	reasonLayout->addWidget(rbRefusalOfAcceptance);
	reasonLayout->addWidget(rbQuantityCorrection);

	QVBoxLayout* noteLayout = new QVBoxLayout(grpRectificationNote);
	noteLayout->addWidget(rectificationtext);
	noteLayout->addWidget(lbRectificationComment);
	noteLayout->addWidget(rectificationcomment);

	QHBoxLayout* btnLayout = new QHBoxLayout();
	btnLayout->addStretch();
	btnLayout->addWidget(btnOK);
	btnLayout->addWidget(btnCancel);

	QGridLayout* mainLayout = new QGridLayout(this);
	mainLayout->setRowStretch(1, 2);
	mainLayout->setRowStretch(2, 1);
	mainLayout->setRowStretch(3, 1);
	mainLayout->addWidget(grpContributionVoucher, 1, 1, 1, 2);
	mainLayout->addWidget(tabPositions,			  2, 1, 1, 2);
	mainLayout->addWidget(grpRectificationReason, 3, 1);
	mainLayout->addWidget(grpRectificationNote,   3, 2);
	mainLayout->addLayout(btnLayout, 4, 1, 1, 2);
}


//-----------------------------------------------------------------------------------------------//
/*!	\param	noteData is the AccessorPropertyTable necessary to create the matcher
 *  \todo FN ???
 *	\throw  no-throw */
//-----------------------------------------------------------------------------------------------//
void NA18DeliveryNoteCopyVC::init()
{
	BLOG_TRACE_METHOD(LoggerPool::loggerViewConn, "NA18DeliveryNoteCopyVC::init()");

	tabPositions->setEditableColumn(0, false);
	tabPositions->setEditableColumn(1, false);
	tabPositions->setEditableColumn(2, false);
	tabPositions->setEditableColumn(3, false);
	tabPositions->setEditableColumn(4, false);
//	tabPositions->setColumnHidden(2, true);

	QValidatorItemDelegate* delegate = new QValidatorItemDelegate(this);
	delegate->setColumnValidator(COLUMN_INDEX_QUANTITY, new QRegExpValidator(QRegExp("[0-9]{0,4}"), this));
	tabPositions->setItemDelegate(delegate);
}

//-----------------------------------------------------------------------------------------------//
/*!	\return	int indicates the value returned by a modal dialog. 
 *	\Value '1' represents QDialog::Accepted and value '0' represents QDialog::Rejected.
 *	\throw  no-throw */
//-----------------------------------------------------------------------------------------------//
basar::gui::tie::WidgetReturnEnum NA18DeliveryNoteCopyVC::show()
{
	BLOG_TRACE_METHOD(LoggerPool::loggerViewConn, "NA18DeliveryNoteCopyVC::show()");

	return basar::gui::tie::getWidgetReturnType( exec() );
}

//-----------------------------------------------------------------------------------------------//
/*!	\retval	true by default element of the erased one.
 *	\throw  no-throw */
//-----------------------------------------------------------------------------------------------//
bool NA18DeliveryNoteCopyVC::shutdown()
{
	BLOG_TRACE_METHOD(LoggerPool::loggerViewConn, "NA18DeliveryNoteCopyVC::shutdown()");

	return close();
}

//-----------------------------------------------------------------------------------------------//
//!	\throw	no-throw */
//-----------------------------------------------------------------------------------------------//
void NA18DeliveryNoteCopyVC::hide()
{
	BLOG_TRACE_METHOD(LoggerPool::loggerViewConn, "NA18DeliveryNoteCopyVC::hide()");

	return QWidget::hide();
}

//-----------------------------------------------------------------------------------------------//
//!	\throw	no-throw */
//-----------------------------------------------------------------------------------------------//
void NA18DeliveryNoteCopyVC::textEdit_textChanged()
{
	BLOG_TRACE_METHOD(LoggerPool::loggerViewConn, "NA18DeliveryNoteCopyVC::textEdit_textChanged()");

	rectificationcomment->blockSignals(true);
	QTextCursor cursor = rectificationcomment->textCursor();
	int pos = cursor.position();
	QString temp(rectificationcomment->toPlainText());
	temp = temp.replace("\n", " "); // kein Zeilenumbruch durch User, sonst wird auf Beleg nur die 1. Zeile gedruckt!
	rectificationcomment->setPlainText(temp.left(150)); // max 150 Zeichen
	cursor.setPosition(pos);
	rectificationcomment->setTextCursor(cursor);
	rectificationcomment->blockSignals(false);
}

//-----------------------------------------------------------------------------------------------//
//!	\throw	no-throw */
//-----------------------------------------------------------------------------------------------//
void NA18DeliveryNoteCopyVC::onBtnOk_clicked()
{
	BLOG_TRACE_METHOD(LoggerPool::loggerViewConn, "NA18DeliveryNoteCopyVC::onBtnOk_clicked()");

	// check for radio button selection
	if (rbTheftLoss->isChecked() || rbRefusalOfAcceptance->isChecked() || rbQuantityCorrection->isChecked())
	{
		accept();
	}
	else
	{
		basar::gui::tie::warningMsgBox(this, tr("No rectification reason selected!").toLocal8Bit().constData(),
			basar::gui::tie::ButtonOk,
			basar::gui::tie::ButtonOk,
			"Narcotics");
	}
}

//-----------------------------------------------------------------------------------------------//
//!	\throw	no-throw */
//-----------------------------------------------------------------------------------------------//
void NA18DeliveryNoteCopyVC::onBtnCancel_clicked()
{
	BLOG_TRACE_METHOD(LoggerPool::loggerViewConn, "NA18DeliveryNoteCopyVC::onBtnCancel_clicked()");

	reject();
}

//-----------------------------------------------------------------------------------------------//
//!	\throw	no-throw */
//-----------------------------------------------------------------------------------------------//
void NA18DeliveryNoteCopyVC::onReason_toggled(bool)
{
	BLOG_TRACE_METHOD(LoggerPool::loggerViewConn, "NA18DeliveryNoteCopyVC::onReason_toggled()");

	tabPositions->blockSignals(true);
	if (true == rbTheftLoss->isChecked())
	{
		//rectificationtext->setPlainText(tr("Please cancel contributionvoucher!")); // diesen Std-Text nicht mehr automatisch setzen ! SR-14034248
		rectificationtext->setPlainText("");
		resetChangedQuantities();
		setQuantityColumnEditable(false);
		tabPositions->showColumn(0);
	}
	else if (true == rbRefusalOfAcceptance->isChecked())
	{
		rectificationtext->setPlainText(tr("Please cancel contributionvoucher!"));
		resetChangedQuantities();
		setQuantityColumnEditable(false);
		tabPositions->showColumn(0);
	}
	else if (true == rbQuantityCorrection->isChecked())
	{
		rectificationtext->setPlainText(tr("Please change quantity: "));
		setQuantityColumnEditable(true);
		tabPositions->hideColumn(0);
		tabPositions->setCurrentCell(0, 1);
	}
	tabPositions->blockSignals(false);
}

//-----------------------------------------------------------------------------------------------//
//!	\throw	no-throw */
//-----------------------------------------------------------------------------------------------//

void NA18DeliveryNoteCopyVC::setQuantityColumnEditable(bool editable)
{
	for (int rowIndex = 0; rowIndex < tabPositions->rowCount(); ++rowIndex)
	{
		QTableWidgetItem* item = tabPositions->item(rowIndex, COLUMN_INDEX_QUANTITY);
		if (true == editable)
			item->setFlags(item->flags() | Qt::ItemIsEditable);
		else
			item->setFlags(item->flags() &~ Qt::ItemIsEditable);
	}
}

//-----------------------------------------------------------------------------------------------//
//!	\throw	no-throw */
//-----------------------------------------------------------------------------------------------//
void NA18DeliveryNoteCopyVC::resetChangedQuantities()
{
	for (int rowIndex = 0; rowIndex < tabPositions->rowCount(); ++rowIndex)
	{
		QString originalQuantity = tabPositions->item(rowIndex, COLUMN_INDEX_ORIGINAL_QUANTITY)->text();
		tabPositions->item(rowIndex, COLUMN_INDEX_QUANTITY)->setText(originalQuantity);
	}
}

//-----------------------------------------------------------------------------------------------//
//!	\throw	no-throw */
//-----------------------------------------------------------------------------------------------//
void NA18DeliveryNoteCopyVC::onPositions_QuantityChanged(int, int)
{
	BLOG_TRACE_METHOD(LoggerPool::loggerViewConn, "NA18DeliveryNoteCopyVC::onPositions_QuantityChanged()");

	QString newText;

	if (false == rbQuantityCorrection->isChecked())
	{
		return;
	}

	for (int rowIndex = 0; rowIndex < tabPositions->rowCount(); ++rowIndex)
	{
		if (tabPositions->item(rowIndex, COLUMN_INDEX_QUANTITY) != NULL &&
			tabPositions->item(rowIndex, COLUMN_INDEX_QUANTITY)->text() != tabPositions->item(rowIndex, COLUMN_INDEX_ORIGINAL_QUANTITY)->text())
		{
			if (false == newText.isEmpty())
			{
				newText.append(", ");
			}
			newText.append(tabPositions->item(rowIndex, COLUMN_INDEX_QUANTITY)->text() + 
				tr(" x Article No ") + 
				tabPositions->item(rowIndex, COLUMN_INDEX_ARTCLENO)->text());
		}
	}
	rectificationtext->setPlainText(tr("Please change quantity: ") + newText);
}

//-----------------------------------------------------------------------------------------------//
//!	\throw	no-throw */
//-----------------------------------------------------------------------------------------------//
void NA18DeliveryNoteCopyVC::matchToRectificationNote(basar::db::aspect::AccessorPropertyTable_YIterator yitNote)
{
	BLOG_TRACE_METHOD(LoggerPool::loggerViewConn, "NA18DeliveryNoteCopyVC::matchToRectificationNote()");

	basar::gui::tie::MatcherRef noteMatcher = basar::gui::tie::Manager::getInstance().createMatcher(this, yitNote.getPropertyTable());

	basar::VarString radioButtonNames;
	radioButtonNames.format("%d=%s;%d=%s;%d=%s;",
		constants::THEFT_LOSS,				rbTheftLoss->objectName().toLocal8Bit().constData(),
		constants::REFUSAL_OF_ACCEPTANCE,	rbRefusalOfAcceptance->objectName().toLocal8Bit().constData(),
		constants::QUANTITY_CORRECTION,		rbQuantityCorrection->objectName().toLocal8Bit().constData());

	basar::gui::tie::RadioButtonTransformer t1(basar::gui::tie::Transformer::ParamList(radioButtonNames), constants::RECTIFICATIONREASON);
	noteMatcher.push_back(t1);
	noteMatcher.LeftToRight();
}

//-----------------------------------------------------------------------------------------------//
//!	\throw	no-throw */
//-----------------------------------------------------------------------------------------------//
void NA18DeliveryNoteCopyVC::matchToContributionVoucherPos(basar::db::aspect::AccessorPropertyTableRef tabPos, int index)
{
	BLOG_TRACE_METHOD(LoggerPool::loggerViewConn, "NA18DeliveryNoteCopyVC::matchToContributionVoucherPos()");

	std::list<basar::VarString> attributeNames;
	QStringList					columnNames;

	attributeNames.push_back(constants::SELECTION);
	columnNames.push_back(tr("Selection"));

	attributeNames.push_back(constants::ARTICLECODE);
	columnNames.push_back(tr("Article No"));

	attributeNames.push_back(constants::QTYTRANSACTION);
	columnNames.push_back(tr("Quantity"));

	attributeNames.push_back(constants::ORIGINALQTYTRANSACTION);
	columnNames.push_back(tr("Original Quantity"));

	attributeNames.push_back(constants::PACKAGEUNIT);
	columnNames.push_back(tr("Packing Unit"));

	attributeNames.push_back(constants::ARTICLENAME);
	columnNames.push_back(tr("Article Name"));

	tabPositions->setHorizontalHeaderLabels(columnNames);

	basar::gui::tie::MatcherRef posMatcher = basar::gui::tie::Manager::getInstance().createMatcher(tabPositions, tabPos);
	posMatcher.transformProperties(tabPositions->horizontalHeaderLabels(), attributeNames);

	CheckBoxTableTransformer checkBoxTransformer(tabPositions, tr("Selection").toStdString(),
		constants::SELECTION, constants::SELECTION_ON, constants::SELECTION_OFF);
	posMatcher.push_back(checkBoxTransformer);

	if (index == -1)
		posMatcher.LeftToRight();
	else
		posMatcher.LeftToRight(index, 1);
}

//-----------------------------------------------------------------------------------------------//
//!	\throw	no-throw */
//-----------------------------------------------------------------------------------------------//
void NA18DeliveryNoteCopyVC::matchFromContributionVoucherHead(basar::db::aspect::AccessorPropertyTable_YIterator yitHead)
{
	BLOG_TRACE_METHOD(LoggerPool::loggerViewConn, "NA18DeliveryNoteCopyVC::matchFromContributionVoucherHead()");

	basar::gui::tie::MatcherRef headMatcher = basar::gui::tie::Manager::getInstance().createMatcher(grpContributionVoucher, yitHead.getPropertyTable());
	headMatcher.RightToLeft(headMatcher.getLeft().begin(), yitHead);
}

//-----------------------------------------------------------------------------------------------//
//!	\throw	no-throw */
//-----------------------------------------------------------------------------------------------//
void NA18DeliveryNoteCopyVC::matchFromContributionVoucherPos(basar::db::aspect::AccessorPropertyTableRef tabPos)
{
	BLOG_TRACE_METHOD(LoggerPool::loggerViewConn, "NA18DeliveryNoteCopyVC::matchFromContributionVoucherPos()");

	std::list<basar::VarString> attributeNames;
	QStringList					columnNames;

	attributeNames.push_back(constants::SELECTION);
	columnNames.push_back(tr("Selection"));

	attributeNames.push_back(constants::ARTICLECODE);
	columnNames.push_back(tr("Article No"));

	attributeNames.push_back(constants::QTYTRANSACTION);
	columnNames.push_back(tr("Quantity"));

	attributeNames.push_back(constants::ORIGINALQTYTRANSACTION);
	columnNames.push_back(tr("Original Quantity"));

	attributeNames.push_back(constants::PACKAGEUNIT);
	columnNames.push_back(tr("Packing Unit"));

	attributeNames.push_back(constants::ARTICLENAME);
	columnNames.push_back(tr("Article Name"));

	tabPositions->setHorizontalHeaderLabels(columnNames);

	basar::gui::tie::MatcherRef posMatcher = basar::gui::tie::Manager::getInstance().createMatcher(tabPositions, tabPos);
	posMatcher.transformProperties(tabPositions->horizontalHeaderLabels(), attributeNames);

	CheckBoxTableTransformer checkBoxTransformer(tabPositions, tr("Selection").toStdString(),
		constants::SELECTION, constants::SELECTION_ON, constants::SELECTION_OFF);
	posMatcher.push_back(checkBoxTransformer);

	tabPositions->blockSignals(true);
	int matchedRowCount = posMatcher.RightToLeft();
	tabPositions->setRowCount(matchedRowCount);
	tabPositions->blockSignals(false);
	tabPositions->resizeColumnsToContents();
	tabPositions->resizeRowsToContents();
}

//-----------------------------------------------------------------------------------------------//

} //namespace viewConn
} //namespace narcotics

//-----------------------------------------------------------------------------------------------//
