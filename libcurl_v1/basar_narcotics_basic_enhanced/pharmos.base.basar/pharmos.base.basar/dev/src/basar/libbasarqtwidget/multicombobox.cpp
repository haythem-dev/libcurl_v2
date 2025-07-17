//----------------------------------------------------------------------------
/*! \file
 *  \brief  a comfortabel multiple column combobox
 *  \author Roland Kiefert
 *  \date   26.01.2006
 */
//----------------------------------------------------------------------------
#ifdef _WIN32
#pragma warning (push)
#pragma warning(disable: 4481) // C4481: nonstandard extension used: override specifier 'override'
#pragma warning(disable: 4512) // qobject_impl.h - C4512: 'QtPrivate::QSlotObjectBase' : assignment operator could not be generated
#endif
#include "libbasarqtwidget_multicombobox.h"
#ifdef _WIN32
#pragma warning (pop)
#endif

#include "libbasarcmnutil_bstring.h"
#include "libbasarcmnutil_exceptions.h"

#pragma warning (push)
#pragma warning(disable: 4127 4251 4481)
#include <QtWidgets/QHeaderView>
#include <QtGui/QStandardItemModel>
#include <QtWidgets/QTableView>
#pragma warning (pop)

#pragma warning(disable: 4505)

using basar::VarString;

//!	\throw no-throw 
MultiComboBox::MultiComboBox(QWidget *parent) : QComboBox(parent), 
											    m_ColumnCount(1),
												m_VerticalHeaderHidden(true),
												m_HorizontalHeaderHidden(false)
{
	m_View = new QTableView(this);
	QHeaderView* hv = new QHeaderView(Qt::Vertical);
	hv->setHidden(true);
	m_View->setVerticalHeader(hv);
	m_View->setSelectionBehavior(QAbstractItemView::SelectRows);
	this->setView(m_View);
	m_View->setMinimumHeight(70);
	m_Model =	qobject_cast<QStandardItemModel*>(this->view()->model());
	m_HorHeaderModel = qobject_cast<QStandardItemModel*>(m_View->horizontalHeader()->model());
}
//!	\throw no-throw 
MultiComboBox::~MultiComboBox()
{
	delete m_Model;
	delete m_View;
}
//!	\throw no-throw
//! \return column count of MultiComboBox
int MultiComboBox::columnCount()
{
	return m_ColumnCount;
}
//!	\throw BasarOutOfRangeIndexExceptionw 
void MultiComboBox::setColumnCount(int count)
{
	if (0 >= count)
	{
		VarString msg;
		msg.format("count=%d must be greater than 0",count);
		throw basar::cmnutil::BasarOutOfRangeIndexException(
			basar::ExceptInfo("MultiComboBox.setColumnCount(int)",
								msg,
								__FILE__, 
								__LINE__));
	}
	if (m_ColumnCount == count)
		return;
	if (m_ColumnCount > count)
		m_Model->removeColumns(m_ColumnCount-(m_ColumnCount-count),m_ColumnCount-count);
	if (m_ColumnCount < count)
	    m_Model->insertColumns(m_ColumnCount,count-m_ColumnCount);	
	m_ColumnCount = count;
}
//!	\throw no-throw 
void MultiComboBox::clear()
{
	m_Model->clear();
//	QComboBox::clear();
	m_ColumnCount = 0;
}
//!	\throw no-throw 
void MultiComboBox::clearContext()
{
	QList<QString> horizontalList;
	QList<QString> verticalList;

	bool horizontalHide = isHorizontalHeaderHidden();
	bool verticalHide = isVerticalHeaderHidden();

	for (basar::Int32 i=0; i < m_ColumnCount; ++i)
		horizontalList.push_back(getHorizontalHeaderItem(i));

	if (!horizontalHide)
		for (basar::Int32 i=0; i < count(); ++i)
			verticalList.push_back(getVerticalHeaderItem(i));

	int colCount = columnCount();
	int rowCount = count();	

	this->clear();

	setColumnCount(colCount);
	if (!horizontalHide)
		for (basar::Int32 i=0; i<horizontalList.count(); ++i)
			setHorizontalHeaderItem(i,horizontalList.value(i));

	if (!verticalHide)
	{
		m_Model->insertRows(0, rowCount);
		for (basar::Int32 i=0; i<verticalList.count(); ++i)
			setVerticalHeaderItem(i,verticalList.value(i));
	}
	hideHorizontalHeader(horizontalHide);
	hideVerticalHeader(verticalHide);
}
//!	\throw BasarOutOfRangeIndexException
void MultiComboBox::addItem(int row, int column, const QString & item)
{
	if (count() < row || 0 > row)
	{
		VarString msg;
		msg.format("row=%d must be greater equal than 0 and smaller than count()=%d of the box",row,count());
		throw basar::cmnutil::BasarOutOfRangeIndexException(
			basar::ExceptInfo("MultiComboBox.addItem()", msg, __FILE__, __LINE__));
	}
	if (column >= m_ColumnCount || 0 > column )
	{
		VarString msg;
		msg.format("column=%d must be greater equal than 0 and smaller than column count=%d of the box",column,m_ColumnCount);
		throw basar::cmnutil::BasarOutOfRangeIndexException(
			basar::ExceptInfo("MultiComboBox.addItem()", msg, __FILE__, __LINE__));
	}

	if (row == count())
	{
		QComboBox::addItem("");
		m_Model->setData(m_Model->index(row,column),item);
	}
	else
	{
		m_Model->setData(m_Model->index(row,column),item);
	}
}

//!	\throw BasarOutOfRangeIndexException
//! \return QString containing requested item's contents
QString MultiComboBox::itemText(int row, int column )
{
	if (count() <= row || 0 > row)
	{
		VarString msg;
		msg.format("row=%d must be greater equal than 0 and smaller than count()=%d of the box",row,m_ColumnCount);
		throw basar::cmnutil::BasarOutOfRangeIndexException(
			basar::ExceptInfo("MultiComboBox.addItem()",
								msg,
								__FILE__, 
								__LINE__));
	}
	if (column >= m_ColumnCount || 0 > column )
	{
		VarString msg;
		msg.format("column=%d must be greater equal than 0 and smaller than column count=%d of the box",column,m_ColumnCount);
		throw basar::cmnutil::BasarOutOfRangeIndexException(
			basar::ExceptInfo("MultiComboBox.addItem()",
								msg,
								__FILE__, 
								__LINE__));
	}
    return m_Model->data(m_Model->index(row,column)).toString();
}
//!	\throw no-throw
//! \return bool indicating if horizontal header is hidden
bool MultiComboBox::isHorizontalHeaderHidden()
{
	return m_HorizontalHeaderHidden;
}
//!	\throw no-throw
//! \return bool indicating if vertical header is hidden
bool MultiComboBox::isVerticalHeaderHidden()
{
	return m_VerticalHeaderHidden;
}

//!	\throw no-throw 
void MultiComboBox::hideHorizontalHeader(bool hidden /*= true*/)
{
	m_HorizontalHeaderHidden = hidden;
	QHeaderView* hv = m_View->horizontalHeader();
	hv->setHidden(hidden);
}

//!	\throw no-throw 
void MultiComboBox::hideVerticalHeader(bool hidden /*= true*/)
{
	m_VerticalHeaderHidden = hidden;
	QHeaderView* hv = m_View->verticalHeader();
	hv->setHidden(hidden);
}
//!	\throw BasarOutOfRangeIndexException 
void MultiComboBox::setHorizontalHeaderItem(int column, const QString &value)
{
	if (column >= m_ColumnCount || 0 > column)
	{
		VarString msg;
		msg.format("column=%d must be greater equal than 0 and smaller than column count=%d of the box",column,m_ColumnCount);
		throw basar::cmnutil::BasarOutOfRangeIndexException(
			basar::ExceptInfo("setHorizontalHeaderItem.addItem()",
								msg,
								__FILE__, 
								__LINE__));
	}
	m_Model->setHeaderData(column,Qt::Horizontal,value);
}
//!	\throw BasarOutOfRangeIndexException
//! \return QString containing text of requested horizontal header
QString MultiComboBox::getHorizontalHeaderItem(int column)
{
	if (column >= m_ColumnCount || 0 > column)
	{
		VarString msg;
		msg.format("column=%d must be greater equal than 0 and smaller than column count=%d of the box",column,m_ColumnCount);
		throw basar::cmnutil::BasarOutOfRangeIndexException(
			basar::ExceptInfo("setHorizontalHeaderItem.addItem()",
								msg,
								__FILE__, 
								__LINE__));
	}
	return m_Model->headerData(column,Qt::Horizontal).toString();
}
//!	\throw BasarOutOfRangeIndexException
void MultiComboBox::setVerticalHeaderItem(int row, const QString &value)
{
	if (row >= count() || 0 > row)
	{
		VarString msg;
		msg.format("row=%d must be greater equal than 0 and smaller than count()=%d of the box",row,count());
		throw basar::cmnutil::BasarOutOfRangeIndexException(
			basar::ExceptInfo("setVerticalHeaderItem.addItem()",
								msg,
								__FILE__, 
								__LINE__));
	}
	m_Model->setHeaderData(row,Qt::Vertical,value);
}
//!	\throw BasarOutOfRangeIndexException
//! \return QString containing text of requested vertical header
QString MultiComboBox::getVerticalHeaderItem(int row)
{
	if (row >= count() || 0 > row)
	{
		VarString msg;
		msg.format("row=%d must be greater equal than 0 and smaller than count()=%d of the box",row,count());
		throw basar::cmnutil::BasarOutOfRangeIndexException(
			basar::ExceptInfo("setHorizontalHeaderItem.addItem()",
								msg,
								__FILE__, 
								__LINE__));
	}
	return m_Model->headerData(row,Qt::Vertical).toString();
}

//!	\throw no-throw 
void MultiComboBox::setMinimumHeightList	(int height	)
{
	m_View->setMinimumHeight(height);
}

//!	\throw no-throw
//! \return minimum list height 
int	 MultiComboBox::getMinimumHeightList	()
{
	return m_View->minimumHeight();
}

//!	\throw no-throw 
void MultiComboBox::setMinimumWidthList	(int width	)
{
	m_View->setMinimumWidth(width);
}

//!	\throw no-throw
//! \return giving minimal list width
int	 MultiComboBox::getMinimumWidthList	()
{
	return m_View->minimumWidth();
}

//!	\throw BasarOutOfRangeIndexException 
void MultiComboBox::setMinimumSizeList	(int width, int height)
{
	m_View->setMinimumSize(width, height);
}

//!	\throw no-throw 
void MultiComboBox::resizeColumnsToContents	()
{
	m_View->resizeColumnsToContents();
}

//!	\throw BasarOutOfRangeIndexException 
void MultiComboBox::resizeColumnToContents	(int column)
{
	if (column >= m_ColumnCount || 0 > column)
	{
		VarString msg;
		msg.format("column=<%d> must be greater equal than 0 and smaller than column count=<%d>.", 
					column, m_ColumnCount);
		throw basar::cmnutil::BasarOutOfRangeIndexException(
			basar::ExceptInfo("resizeColumnToContents()", msg.c_str(), __FILE__, __LINE__));
	}
	m_View->resizeColumnToContents(column);
}

//!	\throw no-throw 
void MultiComboBox::resizeRowsToContents	()
{
	m_View->resizeRowsToContents();
}

//!	\throw BasarOutOfRangeIndexException 
void MultiComboBox::resizeRowToContents	(int row)
{
	if (row >= count() || 0 > row)
	{
		VarString msg;
		msg.format("row=<%d> must be greater equal than 0 and smaller than count()=<%d>.",
					row, count() );
		throw basar::cmnutil::BasarOutOfRangeIndexException(
			basar::ExceptInfo("resizeRowToContents()", msg.c_str(), __FILE__, __LINE__));
	}
	m_View->resizeRowToContents(row);
}

//!	\throw BasarOutOfRangeIndexException
//! \return width of requested column
int	 MultiComboBox::columnWidth (int column)
{
	if (column >= m_ColumnCount || 0 > column)
	{
		VarString msg;
		msg.format("column=<%d> must be greater equal than 0 and smaller than column count=<%d>.", 
					column, m_ColumnCount);
		throw basar::cmnutil::BasarOutOfRangeIndexException(
			basar::ExceptInfo("columnWidth()", msg.c_str(), __FILE__, __LINE__));
	}
	return m_View->columnWidth(column);
}

//!	\throw BasarOutOfRangeIndexException 
//! \return height of requested row
int	 MultiComboBox::rowHeight (int row)
{
	if (row >= count() || 0 > row)
	{
		VarString msg;
		msg.format("row=<%d> must be greater equal than 0 and smaller than count()=<%d>.",
					row, count() );
		throw basar::cmnutil::BasarOutOfRangeIndexException(
			basar::ExceptInfo("rowHeight()", msg.c_str(), __FILE__, __LINE__));
	}
	return m_View->rowHeight(row);
}

//!	\throw BasarOutOfRangeIndexException 
void MultiComboBox::setColumnWidth (int column, int width)
{
	if (column >= m_ColumnCount || 0 > column)
	{
		VarString msg;
		msg.format("column=<%d> must be greater equal than 0 and smaller than column count=<%d>.", 
					column, m_ColumnCount);
		throw basar::cmnutil::BasarOutOfRangeIndexException(
			basar::ExceptInfo("setColumnWidth()", msg.c_str(), __FILE__, __LINE__));
	}
	m_View->setColumnWidth(column, width);	
}

//!	\throw BasarOutOfRangeIndexException
void MultiComboBox::setRowHeight (int row, int height)
{
	if (row >= count() || 0 > row)
	{
		VarString msg;
		msg.format("row=<%d> must be greater equal than 0 and smaller than count()=<%d>.",
					row, count() );
		throw basar::cmnutil::BasarOutOfRangeIndexException(
			basar::ExceptInfo("setRowHeight()", msg.c_str(), __FILE__, __LINE__));
	}
	m_View->setRowHeight(row, height);
}

//!	\throw BasarOutOfRangeIndexException
void MultiComboBox::hideColumn (int column)
{
	if (column >= m_ColumnCount || 0 > column)
	{
		VarString msg;
		msg.format("column=<%d> must be greater equal than 0 and smaller than column count=<%d>.", 
					column, m_ColumnCount);
		throw basar::cmnutil::BasarOutOfRangeIndexException(
			basar::ExceptInfo("hideColumn()", msg.c_str(), __FILE__, __LINE__));
	}
	m_View->hideColumn(column);
}

//!	\throw BasarOutOfRangeIndexException
void MultiComboBox::showColumn (int column)
{
	if (column >= m_ColumnCount || 0 > column)
	{
		VarString msg;
		msg.format("column=<%d> must be greater equal than 0 and smaller than column count=<%d>.", 
					column, m_ColumnCount);
		throw basar::cmnutil::BasarOutOfRangeIndexException(
			basar::ExceptInfo("showColumn()", msg.c_str(), __FILE__, __LINE__));
	}
	m_View->showColumn(column);
}

//!	\throw BasarOutOfRangeIndexException 
void MultiComboBox::setColumnHidden (int column, bool hidden)
{
	if (column >= m_ColumnCount || 0 > column)
	{
		VarString msg;
		msg.format("column=<%d> must be greater equal than 0 and smaller than column count=<%d>.", 
					column, m_ColumnCount);
		throw basar::cmnutil::BasarOutOfRangeIndexException(
			basar::ExceptInfo("setColumnHidden()", msg.c_str(), __FILE__, __LINE__));
	}
	m_View->setColumnHidden(column, hidden);
}

//!	\throw BasarOutOfRangeIndexException
//! \return bool indicating requested column is hidden
bool MultiComboBox::isColumnHidden (int column)
{
	if (column >= m_ColumnCount || 0 > column)
	{
		VarString msg;
		msg.format("column=<%d> must be greater equal than 0 and smaller than column count=<%d>.", 
					column, m_ColumnCount);
		throw basar::cmnutil::BasarOutOfRangeIndexException(
			basar::ExceptInfo("isColumnHidden()", msg.c_str(), __FILE__, __LINE__));
	}
	return m_View->isColumnHidden(column);
}

//!	\throw BasarOutOfRangeIndexException 
void MultiComboBox::hideRow (int row)
{
	if (row >= count() || 0 > row)
	{
		VarString msg;
		msg.format("row=<%d> must be greater equal than 0 and smaller than count()=<%d>.",
					row, count() );
		throw basar::cmnutil::BasarOutOfRangeIndexException(
			basar::ExceptInfo("hideRow()", msg.c_str(), __FILE__, __LINE__));
	}
	m_View->hideRow(row);
}

//!	\throw BasarOutOfRangeIndexException 
void MultiComboBox::showRow (int row)
{
	if (row >= count() || 0 > row)
	{
		VarString msg;
		msg.format("row=<%d> must be greater equal than 0 and smaller than count()=<%d>.",
					row, count() );
		throw basar::cmnutil::BasarOutOfRangeIndexException(
			basar::ExceptInfo("showRow()", msg.c_str(), __FILE__, __LINE__));
	}
	m_View->showRow(row);
}

//!	\throw BasarOutOfRangeIndexException
void MultiComboBox::setRowHidden (int row, bool hidden)
{
	if (row >= count() || 0 > row)
	{
		VarString msg;
		msg.format("row=<%d> must be greater equal than 0 and smaller than count()=<%d>.",
					row, count() );
		throw basar::cmnutil::BasarOutOfRangeIndexException(
			basar::ExceptInfo("setRowHidden()", msg.c_str(), __FILE__, __LINE__));
	}
	m_View->setRowHidden(row, hidden);
}

//!	\throw BasarOutOfRangeIndexException 
//! \return bool indicating requested row is hidden
bool MultiComboBox::isRowHidden (int row)
{
	if (row >= count() || 0 > row)
	{
		VarString msg;
		msg.format("row=<%d> must be greater equal than 0 and smaller than count()=<%d>.",
					row, count() );
		throw basar::cmnutil::BasarOutOfRangeIndexException(
			basar::ExceptInfo("isRowHidden()", msg.c_str(), __FILE__, __LINE__));
	}
	return m_View->isRowHidden(row);
}

//!	\throw BasarOutOfRangeIndexException 
void MultiComboBox::selectRow ( int row	)
{
	if (row >= count() || 0 > row)
	{
		VarString msg;
		msg.format("row=<%d> must be greater equal than 0 and smaller than count()=<%d>.",
					row, count() );
		throw basar::cmnutil::BasarOutOfRangeIndexException(
			basar::ExceptInfo("selectRow()", msg.c_str(), __FILE__, __LINE__));
	}
	m_View->selectRow(row);
}

//!	\throw no-throw 
void MultiComboBox::setItemText( int row, int column, const QString & itemText )
{
	addItem(row, column, itemText);
}

//!	\throw BasarOutOfRangeIndexException 
void MultiComboBox::insertRow ( int rowIndex, std::list<basar::VarString> itemTexts, 
							    int modelColumn, const QVariant & userData )
{
	if (rowIndex > count() || 0 > rowIndex)
	{
		VarString msg;
		msg.format("row=<%d> must be greater than 0 and smaller than count()=<%d>.",
					rowIndex, count() );
		throw basar::cmnutil::BasarOutOfRangeIndexException(
			basar::ExceptInfo("insertRow()", msg.c_str(), __FILE__, __LINE__));
	}

	
	if ( itemTexts.size() != static_cast <std::list<VarString>::size_type> (m_ColumnCount) )
	{
		VarString msg;
		msg.format("size of item list=<%d> must be equal to number of columns=<%d>.",
					itemTexts.size(), m_ColumnCount );
		throw basar::cmnutil::BasarOutOfRangeIndexException(
			basar::ExceptInfo("insertRow()", msg.c_str(), __FILE__, __LINE__));
	}

	std::list<VarString>::iterator it = itemTexts.begin();
	basar::Int32 colCounter = 0;

	while (colCounter != modelColumn)
	{
		++colCounter;
		++it;
	}

	insertItem( rowIndex, QString::fromLocal8Bit((*it).c_str()), userData );
	
	it = itemTexts.begin();
	colCounter = 0;

	while ( colCounter < m_ColumnCount ) 
	{
		if (colCounter != modelColumn)
			addItem(rowIndex, colCounter, QString::fromLocal8Bit((*it).c_str()));

		++it;
		++colCounter;	
	}
}

//!	\throw BasarOutOfRangeIndexException 
void MultiComboBox::removeRow ( int rowIndex )
{
	if (rowIndex > count() || 0 > rowIndex)
	{
		VarString msg;
		msg.format("row=<%d> must be greater than 0 and smaller than count()=<%d>.",
					rowIndex, count() );
		throw basar::cmnutil::BasarOutOfRangeIndexException(
			basar::ExceptInfo("removeRow()", msg.c_str(), __FILE__, __LINE__));
	}
	removeItem ( rowIndex );
}

//--------------------------------------------------------------------------------------------------------------//
//!	\return	stringlist containing all horizontal header labels of the multi combo box
//!	\throw	no-throw 
//--------------------------------------------------------------------------------------------------------------//
const std::list<VarString> MultiComboBox::horizontalHeaderLabels()
{
	std::list<VarString> list;
	
	if (isHorizontalHeaderHidden()) 
		return list;
	
	for( int column = 0; column < columnCount(); ++column )
	{
		list.push_back( getHorizontalHeaderItem(column).toLocal8Bit().data() );
	}

	return list;
}

//--------------------------------------------------------------------------------------------------------------//
//!	\return	stringlist containing all vertical header labels of the multi combo box
//!	\throw	no-throw 
//--------------------------------------------------------------------------------------------------------------//
const std::list<VarString> MultiComboBox::verticalHeaderLabels()
{
	std::list<VarString> list;
	
	if (isVerticalHeaderHidden()) 
		return list;
	
	for( int row = 0; row < count(); ++row )
	{
		list.push_back( getVerticalHeaderItem(row).toLocal8Bit().data() );
	}
	
	return list;
}
