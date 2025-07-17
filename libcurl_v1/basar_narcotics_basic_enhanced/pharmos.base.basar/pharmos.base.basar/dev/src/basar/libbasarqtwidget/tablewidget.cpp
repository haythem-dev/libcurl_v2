//--------------------------------------------------------------------------------------------------------------//
/*! \file
*  \brief  a comfortable table widget
*  \author Roland Kiefert
*  \date   19.01.2006
*/
//--------------------------------------------------------------------------------------------------------------//

//--------------------------------------------------------------------------------------------------------------//
// includes
//--------------------------------------------------------------------------------------------------------------//
#include "libbasarqtwidget_tablewidget.h"

#include "libbasarcmnutil_bstring.h"
#include "libbasarqtwidget_integersignalemitter.h"

#include <QtWidgets/QHeaderView>
#include <QtWidgets/QScrollBar>

#ifdef _WIN32
#pragma warning (push)
#pragma warning (disable: 4127) //C4127: conditional expression is constant
#pragma warning (disable: 4244) //C4244: 'return' : conversion from 'qreal' to 'int', possible loss of data
#pragma warning (disable: 4251) //C4251: 'QInputEvent::modState' : class 'QFlags<Enum>' needs to have dll-interface to be used by clients of class 'QInputEvent'
#endif
#include <QtGui/QKeyEvent>
#ifdef _WIN32
#pragma warning (pop)
#endif


#include <QtWidgets/QShortcut>

#pragma warning( disable: 4505 ) //due to Qt5 new signal/slot connect syntax

//--------------------------------------------------------------------------------------------------------------//
// class definition
//--------------------------------------------------------------------------------------------------------------//
//! \throw no-throw
BasarTableWidget::BasarTableWidget( QWidget* parent ) : QTableWidget( parent ),
    m_prior( ColumnSeniorToRow ),
    m_verticalHeader( false ),
    m_horizontalHeader( false ),
    m_resizeRowsToContents( false ),
    m_resizeColumnsToContents( false ),
    m_stretchLastHorizontalSection( false ),
    m_useAbsoluteVerticalHeaders( std::pair<bool, QStringList>( false, QStringList() ) ),
    m_FrozenTableView( nullptr ),
    m_FrozenColumns( 0 )
{
    initSlots();
}
//! \throw no-throw
BasarTableWidget::BasarTableWidget( int rows, int columns, QWidget* parent ) : QTableWidget( rows, columns, parent ),
    m_prior( ColumnSeniorToRow ),
    m_verticalHeader( false ),
    m_horizontalHeader( false ),
    m_resizeRowsToContents( false ),
    m_resizeColumnsToContents( false ),
    m_stretchLastHorizontalSection( false ),
    m_useAbsoluteVerticalHeaders( std::pair<bool, QStringList>( false, QStringList() ) ),
    m_FrozenTableView( nullptr ),
    m_FrozenColumns( 0 )
{
    initSlots();
}
//! \throw no-throw
void BasarTableWidget::setCurrentItem( QTableWidgetItem* item )
{
    int height = 0;

    if ( findSort( currentRow(), currentColumn() ) )
    {
        height = setAttributes( item );
    }

    QTableWidget::setCurrentItem( item );

    if ( height != 0 )
    {
        QTableWidget::setRowHeight( currentRow(), height );
    }
}

//! \throw no-throw
void BasarTableWidget::setItem( int row, int column, QTableWidgetItem* item )
{
    int height = 0;

    if ( findSort( row, column ) )
    {
        height = setAttributes( item );
    }

    QTableWidget::setItem( row, column, item );

    if ( height != 0 )
    {
        QTableWidget::setRowHeight( row, height );
    }
}

//! \throw no-throw
void BasarTableWidget::setPrior( PriorEnum prior )
{
    m_prior = prior;
}

/*! \throw no-throw
\retval true if the vertical header is hidden
\return otherwise returns false */
bool BasarTableWidget::isVerticalHeaderHidden() const
{
    return m_verticalHeader;
}

/*! \throw no-throw
\retval true if the horizontal header is hidden
\return otherwise returns false */
bool BasarTableWidget::isHorizontalHeaderHidden() const
{
    return m_horizontalHeader;
}

/*! \throw no-throw
\retval true if the m_resize rows to contents is set
\return otherwise returns false */
bool BasarTableWidget::isResizeRowsToContents() const
{
    return m_resizeRowsToContents;
}

/*! \throw no-throw
\retval true if the m_resize rows to contents is set
\return otherwise returns false */
bool BasarTableWidget::isResizeColumnsToContents() const
{
    return m_resizeColumnsToContents;
}

bool BasarTableWidget::isStretchLastHorizontalSection() const
{
    return m_stretchLastHorizontalSection;
}

//! \throw no-throw
void BasarTableWidget::hideVerticalHeader( bool hidden )
{
    m_verticalHeader = hidden;

    if ( m_verticalHeader )
    {
        verticalHeader()->hide();
    }
    else
    {
        verticalHeader()->show();
    }
}
//! \throw no-throw
void BasarTableWidget::hideHorizontalHeader( bool hidden )
{
    m_horizontalHeader = hidden;

    if ( m_horizontalHeader )
    {
        horizontalHeader()->hide();
    }
    else
    {
        horizontalHeader()->show();
    }
}
//! \throw no-throw
void BasarTableWidget::setFlagsRow( int row, Qt::ItemFlags flags )
{
    m_colItRow = m_colRowSort.find( row );

    if ( m_colItRow != m_colRowSort.end() )
    {
        m_colItRow->second.isFlags = true;
        m_colItRow->second.flags = flags;
    }
    else
    {
        Attributes help;
        help.isBackground = false;
        help.isFlags = true;
        help.isFont = false;
        help.isText = false;
        help.isEditable = false;
        help.isHeight = false;
        help.isAlignment = false;
        help.flags = flags;
        m_colRowSort.insert( std::map<int, Attributes>::value_type( row, help ) );
    }

    refreshRow( row );
}

//! \throw no-throw
void BasarTableWidget::setEditableRow( int row, bool editable )
{
    m_colItRow = m_colRowSort.find( row );

    if ( m_colItRow != m_colRowSort.end() )
    {
        m_colItRow->second.isEditable = true;
        m_colItRow->second.editable = editable;
    }
    else
    {
        Attributes help;
        help.isBackground = false;
        help.isFlags = false;
        help.isFont = false;
        help.isText = false;
        help.isEditable = true;
        help.isHeight = false;
        help.isAlignment = false;
        help.editable = editable;
        m_colRowSort.insert( std::map<int, Attributes>::value_type( row, help ) );
    }

    refreshRow( row );
}

//! \throw no-throw
void BasarTableWidget::setBackgroundColorRow( int row, const QColor& color )
{
    m_colItRow = m_colRowSort.find( row );

    if ( m_colItRow != m_colRowSort.end() )
    {
        m_colItRow->second.isBackground = true;
        m_colItRow->second.background = color;
    }
    else
    {
        Attributes help;
        help.isBackground = true;
        help.isFlags = false;
        help.isFont = false;
        help.isText = false;
        help.isEditable = false;
        help.isHeight = false;
        help.isAlignment = false;
        help.background = color;
        m_colRowSort.insert( std::map<int, Attributes>::value_type( row, help ) );
    }

    refreshRow( row );
}

//! \throw no-throw
void BasarTableWidget::setTextColorRow( int row, const QColor& color )
{
    m_colItRow = m_colRowSort.find( row );

    if ( m_colItRow != m_colRowSort.end() )
    {
        m_colItRow->second.isText = true;
        m_colItRow->second.text = color;
    }
    else
    {
        Attributes help;
        help.isBackground = false;
        help.isFlags = false;
        help.isFont = false;
        help.isText = true;
        help.isEditable = false;
        help.isHeight = false;
        help.isAlignment = false;
        help.text = color;
        m_colRowSort.insert( std::map<int, Attributes>::value_type( row, help ) );
    }

    refreshRow( row );
}

//! \throw no-throw
void BasarTableWidget::setFontRow( int row, const QFont& font )
{
    m_colItRow = m_colRowSort.find( row );

    if ( m_colItRow != m_colRowSort.end() )
    {
        m_colItRow->second.isFont = true;
        m_colItRow->second.font = font;
    }
    else
    {
        Attributes help;
        help.isBackground = false;
        help.isFlags = false;
        help.isFont = true;
        help.isText = false;
        help.isEditable = false;
        help.isHeight = false;
        help.isAlignment = false;
        help.font = font;
        m_colRowSort.insert( std::map<int, Attributes>::value_type( row, help ) );
    }

    refreshRow( row );
}

//! \throw no-throw
void BasarTableWidget::setRowHeight( int row, const int height )
{
    m_colItRow = m_colRowSort.find( row );

    if ( m_colItRow != m_colRowSort.end() )
    {
        m_colItRow->second.isHeight = true;
        m_colItRow->second.height = height;
    }
    else
    {
        Attributes help;
        help.isBackground = false;
        help.isFlags = false;
        help.isFont = false;
        help.isText = false;
        help.isEditable = false;
        help.isHeight = true;
        help.isAlignment = false;
        help.height = height;
        m_colRowSort.insert( std::map<int, Attributes>::value_type( row, help ) );
    }

    refreshRow( row );
}

//! \throw no-throw
void BasarTableWidget::setFlagsColumn( int column, Qt::ItemFlags flags )
{
    m_colItColumn = m_colColumnSort.find( column );

    if ( m_colItColumn != m_colColumnSort.end() )
    {
        m_colItColumn->second.isFlags = true;
        m_colItColumn->second.flags = flags;
    }
    else
    {
        Attributes help;
        help.isBackground = false;
        help.isFlags = true;
        help.isFont = false;
        help.isText = false;
        help.isEditable = false;
        help.isHeight = false;
        help.isAlignment = false;
        help.flags = flags;
        m_colColumnSort.insert( std::map<int, Attributes>::value_type( column, help ) );
    }

    refreshColumn( column );
}

//! \throw no-throw
void BasarTableWidget::setEditableColumn( int column, bool editable )
{
    m_colItColumn = m_colColumnSort.find( column );

    if ( m_colItColumn != m_colColumnSort.end() )
    {
        m_colItColumn->second.isEditable = true;
        m_colItColumn->second.editable = editable;
    }
    else
    {
        Attributes help;
        help.isBackground = false;
        help.isFlags = false;
        help.isFont = false;
        help.isText = false;
        help.isEditable = true;
        help.isHeight = false;
        help.isAlignment = false;
        help.editable = editable;
        m_colColumnSort.insert( std::map<int, Attributes>::value_type( column, help ) );
    }

    refreshColumn( column );
}

//! \throw no-throw
void BasarTableWidget::setBackgroundColorColumn( int column, const QColor& color )
{
    m_colItColumn = m_colColumnSort.find( column );

    if ( m_colItColumn != m_colColumnSort.end() )
    {
        m_colItColumn->second.isBackground = true;
        m_colItColumn->second.background = color;
    }
    else
    {
        Attributes help;
        help.isBackground = true;
        help.isFlags = false;
        help.isFont = false;
        help.isText = false;
        help.isEditable = false;
        help.isHeight = false;
        help.isAlignment = false;
        help.background = color;
        m_colColumnSort.insert( std::map<int, Attributes>::value_type( column, help ) );
    }

    refreshColumn( column );
}

//! \throw no-throw
void BasarTableWidget::setTextColorColumn( int column, const QColor& color )
{
    m_colItColumn = m_colColumnSort.find( column );

    if ( m_colItColumn != m_colColumnSort.end() )
    {
        m_colItColumn->second.isText = true;
        m_colItColumn->second.text = color;
    }
    else
    {
        Attributes help;
        help.isBackground = false;
        help.isFlags = false;
        help.isFont = false;
        help.isText = true;
        help.isEditable = false;
        help.isHeight = false;
        help.isAlignment = false;
        help.text = color;
        m_colColumnSort.insert( std::map<int, Attributes>::value_type( column, help ) );
    }

    refreshColumn( column );
}

//! \throw no-throw
void BasarTableWidget::setFontColumn( int column, const QFont& font )
{
    m_colItColumn = m_colColumnSort.find( column );

    if ( m_colItColumn != m_colColumnSort.end() )
    {
        m_colItColumn->second.isFont = true;
        m_colItColumn->second.font = font;
    }
    else
    {
        Attributes help;
        help.isBackground = false;
        help.isFlags = false;
        help.isFont = true;
        help.isText = false;
        help.isEditable = false;
        help.isHeight = false;
        help.isAlignment = false;
        help.font = font;
        m_colColumnSort.insert( std::map<int, Attributes>::value_type( column, help ) );
    }

    refreshColumn( column );
}

//! \throw no-throw
void BasarTableWidget::setTextAlignmentColumn( int column, const int alignment )
{
    m_colItColumn = m_colColumnSort.find( column );

    if ( m_colItColumn != m_colColumnSort.end() )
    {
        m_colItColumn->second.isAlignment = true;
        m_colItColumn->second.alignment = alignment;
    }
    else
    {
        Attributes help;
        help.isBackground = false;
        help.isFlags = false;
        help.isFont = false;
        help.isText = false;
        help.isEditable = false;
        help.isHeight = false;
        help.isAlignment = true;
        help.alignment = alignment;
        m_colColumnSort.insert( std::map<int, Attributes>::value_type( column, help ) );
    }

    refreshColumn( column );
}

//! \throw no-throw
void BasarTableWidget::setResizeRowsToContents( bool resizeRowsToContents )
{
    m_resizeRowsToContents = resizeRowsToContents;

    if ( m_resizeRowsToContents )
    {
        this->resizeRowsToContents();
    }
}

//! \throw no-throw
void BasarTableWidget::setResizeColumnsToContents( bool resizeColumnsToContents )
{
    m_resizeColumnsToContents = resizeColumnsToContents;

    if ( m_resizeColumnsToContents )
    {
        this->resizeColumnsToContents();
    }
}

void BasarTableWidget::setStretchLastHorizontalSection( bool stretchLastSection )
{
    m_stretchLastHorizontalSection = stretchLastSection;

    if ( nullptr != this->horizontalHeader() )
    {
        this->horizontalHeader()->setStretchLastSection( m_stretchLastHorizontalSection );
    }
}

/*! \throw no-throw
*   \retval true if the item was found, otherwise returns false
*/
bool BasarTableWidget::findSort( int row, int column )
{
    m_colItRow = m_colRowSort.find( row );
    m_colItColumn = m_colColumnSort.find( column );

    if ( m_colItRow == m_colRowSort.end() && m_colItColumn == m_colColumnSort.end() )
    {
        return false;
    }

    return true;
}

//! \throw no-throw
//! \return if attribute is height, height gets returned, otherwise 0
int BasarTableWidget::setAttributes( QTableWidgetItem* item )
{
    if ( m_colItRow != m_colRowSort.end() && m_colItColumn != m_colColumnSort.end() )
    {
        if ( m_prior == BasarTableWidget::RowSeniorToColumn )
        {
            if ( m_colItRow->second.isFlags )
            {
                item->setFlags( m_colItRow->second.flags );
            }
            else if ( m_colItRow->second.isEditable )
                if ( m_colItRow->second.editable )
                {
                    item->setFlags( item->flags() | Qt::ItemIsEditable );
                }
                else
                {
                    item->setFlags( item->flags() & ( Qt::ItemIsDragEnabled | Qt::ItemIsDropEnabled | Qt::ItemIsEnabled | Qt::ItemIsSelectable | Qt::ItemIsTristate | Qt::ItemIsUserCheckable ) );
                }
            else if ( m_colItColumn->second.isFlags )
            {
                item->setFlags( m_colItColumn->second.flags );
            }
            else if ( m_colItColumn->second.isEditable )
                if ( m_colItColumn->second.editable )
                {
                    item->setFlags( item->flags() | Qt::ItemIsEditable );
                }
                else
                {
                    item->setFlags( item->flags() & ( Qt::ItemIsDragEnabled | Qt::ItemIsDropEnabled | Qt::ItemIsEnabled | Qt::ItemIsSelectable | Qt::ItemIsTristate | Qt::ItemIsUserCheckable ) );
                }

            if ( m_colItRow->second.isBackground )
            {
                item->setBackground( m_colItRow->second.background );
            }
            else if ( m_colItColumn->second.isBackground )
            {
                item->setBackground( m_colItColumn->second.background );
            }

            if ( m_colItRow->second.isText )
            {
                item->setForeground( m_colItRow->second.text );
            }
            else if ( m_colItColumn->second.isText )
            {
                item->setForeground( m_colItColumn->second.text );
            }

            if ( m_colItRow->second.isFont )
            {
                item->setFont( m_colItRow->second.font );
            }
            else if ( m_colItColumn->second.isFont )
            {
                item->setFont( m_colItColumn->second.font );
            }

            if ( m_colItColumn->second.isAlignment )
            {
                item->setTextAlignment( m_colItColumn->second.alignment );
            }
        }
        else
        {
            if ( m_colItColumn->second.isFlags )
            {
                item->setFlags( m_colItColumn->second.flags );
            }
            else if ( m_colItColumn->second.isEditable )
            {
                if ( m_colItColumn->second.editable )
                {
                    item->setFlags( item->flags() | Qt::ItemIsEditable );
                }
                else
                {
                    item->setFlags( item->flags() & ( Qt::ItemIsDragEnabled | Qt::ItemIsDropEnabled | Qt::ItemIsEnabled | Qt::ItemIsSelectable | Qt::ItemIsTristate | Qt::ItemIsUserCheckable ) );
                }
            }
            else if ( m_colItRow->second.isFlags )
            {
                item->setFlags( m_colItRow->second.flags );
            }
            else if ( m_colItRow->second.isEditable )
            {
                if ( m_colItRow->second.editable )
                {
                    item->setFlags( item->flags() | Qt::ItemIsEditable );
                }
                else
                {
                    item->setFlags( item->flags() & ( Qt::ItemIsDragEnabled | Qt::ItemIsDropEnabled | Qt::ItemIsEnabled | Qt::ItemIsSelectable | Qt::ItemIsTristate | Qt::ItemIsUserCheckable ) );
                }
            }

            if ( m_colItColumn->second.isBackground )
            {
                item->setBackground( m_colItColumn->second.background );
            }
            else if ( m_colItRow->second.isBackground )
            {
                item->setBackground( m_colItRow->second.background );
            }

            if ( m_colItColumn->second.isText )
            {
                item->setForeground( m_colItColumn->second.text );
            }
            else if ( m_colItRow->second.isText )
            {
                item->setForeground( m_colItRow->second.text );
            }

            if ( m_colItColumn->second.isFont )
            {
                item->setFont( m_colItColumn->second.font );
            }
            else if ( m_colItRow->second.isFont )
            {
                item->setFont( m_colItRow->second.font );
            }

            if ( m_colItColumn->second.isAlignment )
            {
                item->setTextAlignment( m_colItColumn->second.alignment );
            }
        }

        if ( m_colItRow->second.isHeight )
        {
            return m_colItRow->second.height;
        }
        else
        {
            return 0;
        }
    }

    if ( m_colItRow != m_colRowSort.end() && m_colItColumn == m_colColumnSort.end() )
    {
        if ( m_colItRow->second.isFlags )
        {
            item->setFlags( m_colItRow->second.flags );
        }
        else if ( m_colItRow->second.isEditable )
        {
            if ( m_colItRow->second.editable )
            {
                item->setFlags( item->flags() | Qt::ItemIsEditable );
            }
            else
            {
                item->setFlags( item->flags() & ( Qt::ItemIsDragEnabled | Qt::ItemIsDropEnabled | Qt::ItemIsEnabled | Qt::ItemIsSelectable | Qt::ItemIsTristate | Qt::ItemIsUserCheckable ) );
            }
        }

        if ( m_colItRow->second.isBackground )
        {
            item->setBackground( m_colItRow->second.background );
        }

        if ( m_colItRow->second.isText )
        {
            item->setForeground( m_colItRow->second.text );
        }

        if ( m_colItRow->second.isFont )
        {
            item->setFont( m_colItRow->second.font );
        }

        if ( m_colItRow->second.isHeight )
        {
            return m_colItRow->second.height;
        }
        else
        {
            return 0;
        }
    }

    if ( m_colItRow == m_colRowSort.end() && m_colItColumn != m_colColumnSort.end() )
    {
        if ( m_colItColumn->second.isFlags )
        {
            item->setFlags( m_colItColumn->second.flags );
        }
        else if ( m_colItColumn->second.isEditable )
        {
            if ( m_colItColumn->second.editable )
            {
                item->setFlags( item->flags() | Qt::ItemIsEditable );
            }
            else
            {
                item->setFlags( item->flags() & ( Qt::ItemIsDragEnabled | Qt::ItemIsDropEnabled | Qt::ItemIsEnabled | Qt::ItemIsSelectable | Qt::ItemIsTristate | Qt::ItemIsUserCheckable ) );
            }
        }

        if ( m_colItColumn->second.isBackground )
        {
            item->setBackground( m_colItColumn->second.background );
        }

        if ( m_colItColumn->second.isText )
        {
            item->setForeground( m_colItColumn->second.text );
        }

        if ( m_colItColumn->second.isFont )
        {
            item->setFont( m_colItColumn->second.font );
        }

        if ( m_colItColumn->second.isAlignment )
        {
            item->setTextAlignment( m_colItColumn->second.alignment );
        }
    }

    return 0;
}
//--------------------------------------------------------------------------------------------------------------//
/*! \return stringlist containing all horizontal header labels of the table widget
\throw  no-throw */
//--------------------------------------------------------------------------------------------------------------//
const std::list<basar::VarString> BasarTableWidget::horizontalHeaderLabels() const
{
    std::list<basar::VarString> list;

    if ( isHorizontalHeaderHidden() )
    {
        return list;
    }

    for ( int column = 0; column < columnCount(); ++column )
    {
        list.push_back( horizontalHeaderItem( column )->text().toLocal8Bit().data() );
    }

    return list;
}
//--------------------------------------------------------------------------------------------------------------//
/*! \return stringlist containing all vertical header labels of the table widget
\throw  no-throw */
//--------------------------------------------------------------------------------------------------------------//
const std::list<basar::VarString> BasarTableWidget::verticalHeaderLabels() const
{
    std::list<basar::VarString> list;

    if ( isVerticalHeaderHidden() )
    {
        return list;
    }

    for ( int row = 0; row < rowCount(); ++row )
    {
        QTableWidgetItem* item =  verticalHeaderItem( row );

        if ( nullptr != item )
        {
            list.push_back( item->text().toLocal8Bit().data() );
        }
    }

    return list;
}
//! \throw  no-throw
void BasarTableWidget::clearContext()
{
    std::list<basar::VarString> horizontalList;
    std::list<basar::VarString> verticalList;
    horizontalList = horizontalHeaderLabels();
    verticalList = verticalHeaderLabels();
    QStringList list;
    std::list<basar::VarString>::iterator it;
    clear();
    it = horizontalList.begin();

    for ( ; it != horizontalList.end(); ++it )
    {
        list << QString::fromLocal8Bit( ( *it ).c_str() );
    }

    if ( list.size() > 0 )
    {
        setHorizontalHeaderLabels( list );
    }

    it = verticalList.begin();
    list.clear();

    for ( ; it != verticalList.end(); ++it )
    {
        list << QString::fromLocal8Bit( ( *it ).c_str() );
    }

    if ( list.size() > 0 )
    {
        setVerticalHeaderLabels( list );
    }
}

//! \throw no-throw
void BasarTableWidget::refreshColumn( int column )
{
    QTableWidgetItem* tableItem = nullptr;

    for ( basar::Int32 i = 0; i < rowCount(); i++ )
    {
        tableItem = takeItem( i, column );

        if ( nullptr != tableItem )
        {
            setItem( i, column, tableItem );
        }
    }
}
//! \throw no-throw
void BasarTableWidget::refreshRow( int row )
{
    QTableWidgetItem* tableItem = nullptr;

    for ( basar::Int32 i = 0; i < columnCount(); i++ )
    {
        tableItem = takeItem( row, i );

        if ( nullptr != tableItem )
        {
            setItem( row, i, tableItem );
        }
    }
}

//! slot to emit the dataSelectionChanged signal \throw no-throw
void BasarTableWidget::emitDataSelectionChanged()
{
    dataSelectionChanged();
}

//! slot handles item changed signals \throw no-throw
void BasarTableWidget::handleItemChanged ( QTableWidgetItem* item )
{
    if ( item && item->isSelected() )
    {
        emitDataSelectionChanged();
    }
}
//! initializes slots \throw no-throw
void BasarTableWidget::initSlots()
{
    connect( this, SIGNAL( itemSelectionChanged() ), this, SLOT( emitDataSelectionChanged() ) );
    connect( this, SIGNAL( itemChanged( QTableWidgetItem* ) ), this, SLOT( handleItemChanged( QTableWidgetItem* ) ) );
}

//! initializes the slots to handle inserted rows and removed rows
void BasarTableWidget::initRowSlots()
{
    connect( this->model(), SIGNAL( rowsInserted ( const QModelIndex&, int  , int   ) ),
             this, SLOT( handleRowsInserted( const QModelIndex&, int  , int   ) ) );
    connect( this->model(), SIGNAL( rowsRemoved  ( const QModelIndex&, int  , int   ) ),
             this, SLOT( handleRowsRemoved( const QModelIndex&, int  , int   ) ) );
}

//! emits dataSelectionChanged for PageUp/PageDown \throw no-throw
void BasarTableWidget::keyPressEvent ( QKeyEvent* event )
{
    if ( event != 0 )
    {
        if ( !event->isAutoRepeat () )
        {
            int key = event->key();

            if ( Qt::Key_PageDown  ==  key || Qt::Key_PageUp  ==  key )
            {
                emitDataSelectionChanged();
                // test test test:
                event->ignore();
            }
        }
    }

    QTableWidget::keyPressEvent( event );
}

//! changes a list of VarStrings into a QStringList
inline void BasarTableWidget::changeToQStringList(
    const std::list<basar::VarString>& varstringlist,
    QStringList& retlist )
{
    std::list<basar::VarString>::const_iterator iter;

    for ( iter = varstringlist.begin(); iter != varstringlist.end(); ++iter )
    {
        retlist << QString::fromLocal8Bit( iter->c_str() );
    }
}

inline void BasarTableWidget::updateFrozenTableViewGeometry()
{
    if ( nullptr != m_FrozenTableView )
    {
        m_FrozenTableView->setGeometry( verticalHeader()->width() + frameWidth(),
                                        frameWidth(), getSumColumnWidth(),
                                        viewport()->height() + horizontalHeader()->height() );
    }
}

inline int BasarTableWidget::getSumColumnWidth() const
{
    int sumColumnWidth = 0;

    for ( int col = 0; col < m_FrozenColumns; ++col )
    {
        sumColumnWidth += columnWidth( col );
    }

    return sumColumnWidth;
}

/*!
 * The Vertical Headers are set to alphabetical Values "A", "B", "C", etc.
 * Each row can be selected by Shortcuts, of this values: "a" selects the first
 * row, ...
 * The vertical headers are rolled, that means, that when rows are inserted
 * or removed the vertical headers are rolled, and appear unchanged.
 * as the RowHeader
 * is used as representation of the Index, rather than as a representation
 * of the row
 *
 */
void BasarTableWidget::setABCVerticalHeaderPolicy(
    Qt::ShortcutContext context //!< specifies the Shortcut Context
)
{
    QStringList list ;
    list <<
         "A" << "B" << "C" << "D" << "E" << "F" << "G" <<
         "H" << "I" << "J" << "K" << "L" << "M" << "N" <<
         "O" << "P" << "Q" << "R" << "S" << "T" << "U" <<
         "V" << "W" << "X" << "Y" << "Z" ;
    QStringList list2;
    list2 <<
          "a" << "b" << "c" << "d" << "e" << "f" << "g" <<
          "h" << "i" << "j" << "k" << "l" << "m" << "n" <<
          "o" << "p" << "q" << "r" << "s" << "t" << "u" <<
          "v" << "w" << "x" << "y" << "z" ;

    QStringList list3 ;
    list3 <<
          "Shift+A" << "Shift+B" << "Shift+C" <<
          "Shift+D" << "Shift+E" << "Shift+F" <<
          "Shift+G" << "Shift+H" << "Shift+I" <<
          "Shift+J" << "Shift+K" << "Shift+L" <<
          "Shift+M" << "Shift+N" << "Shift+O" <<
          "Shift+P" << "Shift+Q" << "Shift+R" <<
          "Shift+S" << "Shift+T" << "Shift+U" <<
          "Shift+V" << "Shift+W" << "Shift+X" <<
          "Shift+Y" << "Shift+Z" ;

    QStringList list4;
    list4 <<
          "Shift+a" << "Shift+b" << "Shift+c" <<
          "Shift+d" << "Shift+e" << "Shift+f" <<
          "Shift+g" << "Shift+h" << "Shift+i" <<
          "Shift+j" << "Shift+k" << "Shift+l" <<
          "Shift+m" << "Shift+n" << "Shift+o" <<
          "Shift+p" << "Shift+q" << "Shift+r" <<
          "Shift+s" << "Shift+t" << "Shift+u" <<
          "Shift+v" << "Shift+w" << "Shift+x" <<
          "Shift+y" << "Shift+z" ;
    setAbsoluteVerticalHeaders( list );
    setShotcutsToSelectRows( list, context );
    setShotcutsToSelectRows( list2, context );
    setShotcutsToSelectRows( list3, context );
    setShotcutsToSelectRows( list4, context );
}

/*!
 * The vertical headers are rolled, that means, that when rows are inserted
 * or removed the vertical headers are rolled, and appear unchanged, as the RowHeader
 * is used as representation of the Index, rather than as a representation
 * of the row.
 * The list is stored seperately, as there may be more entries than rows.
 */
void BasarTableWidget::setAbsoluteVerticalHeaders(
    const QStringList& list  //!< the list of vertical headers
)
{
    m_useAbsoluteVerticalHeaders = std::pair<bool, QStringList>( true, list );
    setVerticalHeaderLabels ( list );
    initRowSlots();
}

/*!
 * The vertical headers are rolled, that means, that when rows are inserted
 * or removed the vertical headers are rolled, and appear unchanged,
 * as the RowHeader
 * is used as representation of the Index, rather than as a representation
 * of the row.
 * The list is stored seperately, as there may be more entries than rows.
 */
void BasarTableWidget::setAbsoluteVerticalHeaders(
    const std::list<basar::VarString>& varstringlist   //!< the list of vertical headers
)
{
    QStringList list;
    changeToQStringList( varstringlist, list );
    setAbsoluteVerticalHeaders( list );
}

/*!
 * If set to true, the vertical headers are rolled, that means, that when rows are inserted
 * or removed the vertical headers are rolled, and appear unchanged.
 * If set to false the normal setting of vertical headers is resumed.
 * Formerly set Vertical Headers are used.
 */
void BasarTableWidget::setAbsoluteVerticalHeaders(
    bool value //!< true - set Absolute Vertical Headers (uses the stored Headers)
)
{
    if ( value )
    {
        m_useAbsoluteVerticalHeaders.first = true;

        if ( 0 == m_useAbsoluteVerticalHeaders.second.size() )
        {
            std::list<basar::VarString> list = verticalHeaderLabels();
            setAbsoluteVerticalHeaders( list );
        }
        else
        {
            setAbsoluteVerticalHeaders( m_useAbsoluteVerticalHeaders.second );
        }
    }
    else
    {
        m_useAbsoluteVerticalHeaders.first = false;
    }
}

/*!
 *
 * Each row can be selected by a Shortcut, that is defined in the
 * QStringList. The Strings have the same convention as in
 * QKeySequence ( const QString & key ) The ShortcutContext can
 * also be specified.
 */
void BasarTableWidget::setShotcutsToSelectRows(
    const QStringList& list,    //!<  list of shortcuts as strings
    Qt::ShortcutContext context //!<  shortcut context
)
{
    QStringList::const_iterator iter;
    int i = 0;

    for ( iter = list.begin(); iter != list.end(); ++iter, ++i )
    {
        QShortcut* s = new QShortcut( *iter, this );
        s->setContext( context );
        IntegerSignalEmitter* e = new IntegerSignalEmitter( i, this );
        connect( s, SIGNAL( activated () ), e, SLOT( click() ) );
        connect( s, SIGNAL( activatedAmbiguously  () ), e, SLOT( click() ) );
        connect( e, SIGNAL( clicked( int ) ), this, SLOT( selectAlwaysOnlyOneRow ( int ) ) );

    }
}

/*!
 *
 * Each row can be selected by a Shortcut, that is defined in the
 * QStringList. The Strings have the same convention as in
 * QKeySequence ( const QString & key ) The ShortcutContext can
 * also be specified.
 */
void BasarTableWidget::setShotcutsToSelectRows(
    const std::list<basar::VarString>& varstringlist,  //!< list of shortcuts as strings
    Qt::ShortcutContext context //!<  shortcut context
)
{
    QStringList list;
    changeToQStringList( varstringlist, list );
    setShotcutsToSelectRows( list, context );
}

void BasarTableWidget::enableFrozenView( basar::Int32 columns )
{
    if ( 0 > columns )
    {
        return;
    }
    else if ( 0 == columns )
    {
        disableFrozenView();
    }
    else
    {
        m_FrozenColumns = columns;

        m_FrozenTableView = new QTableView( this );

        //connect the headers and scrollbars of both tableviews together
        connect( horizontalHeader(), &QHeaderView::sectionResized, this, &BasarTableWidget::updateSectionWidth );
        connect( verticalHeader(), &QHeaderView::sectionResized, this, &BasarTableWidget::updateSectionHeight );

        connect( m_FrozenTableView->verticalScrollBar(), &QScrollBar::valueChanged, verticalScrollBar(), &QScrollBar::setValue );
        connect( verticalScrollBar(), &QScrollBar::valueChanged, m_FrozenTableView->verticalScrollBar(), &QScrollBar::setValue );

        //connect user click events together
        connect( m_FrozenTableView, &QTableView::activated, this, &BasarTableWidget::onCellActivated );
        connect( m_FrozenTableView, &QTableView::clicked, this, &BasarTableWidget::onCellClicked );
        connect( m_FrozenTableView, &QTableView::doubleClicked, this, &BasarTableWidget::onCellDoubleClicked );
        connect( m_FrozenTableView, &QTableView::entered, this, &BasarTableWidget::onCellEntered );
        connect( m_FrozenTableView, &QTableView::pressed, this, &BasarTableWidget::onCellPressed );

        m_FrozenTableView->setModel( model() );
        m_FrozenTableView->setFocusPolicy( Qt::NoFocus );
        m_FrozenTableView->verticalHeader()->hide();
        m_FrozenTableView->horizontalHeader()->setSectionResizeMode( QHeaderView::Fixed );
        m_FrozenTableView->setSelectionModel( selectionModel() );
        m_FrozenTableView->setEditTriggers( editTriggers() );
        m_FrozenTableView->setSelectionBehavior( selectionBehavior() );

        viewport()->stackUnder( m_FrozenTableView );

        //m_FrozenTableView->setStyleSheet( "QTableView { border: none;"
        //                                  "background-color: #8EDE21;"
        //                                  "selection-background-color: #999}" ); //for demo purposes

        //adopt style of table
        auto style = this->styleSheet();

        if (false == style.isEmpty())
        {
            style.replace("QTableWidget", "QTableView");

            style.replace('}', "border: none; }");
        }
        else
        {
            style = "QTableView { border: none; }";
        }

        m_FrozenTableView->setStyleSheet(style);

        //adopt header style of table
        auto headerStyle = this->horizontalHeader()->styleSheet();

        m_FrozenTableView->horizontalHeader()->setStyleSheet(headerStyle);

        //adopt alternating row colors
        m_FrozenTableView->setAlternatingRowColors(this->alternatingRowColors());

        //hide additional columns in view
        for ( int col = m_FrozenColumns; col < model()->columnCount(); ++col )
        {
            m_FrozenTableView->setColumnHidden( col, true );
        }

        //set column width to column width of table widget
        for ( int col = 0; col < m_FrozenColumns; ++col )
        {
            m_FrozenTableView->setColumnWidth( col, columnWidth( col ) );
        }

        m_FrozenTableView->setHorizontalScrollBarPolicy( Qt::ScrollBarAlwaysOff );
        m_FrozenTableView->setVerticalScrollBarPolicy( Qt::ScrollBarAlwaysOff );
        m_FrozenTableView->show();

        updateFrozenTableViewGeometry();

        setHorizontalScrollMode( ScrollPerPixel );
        setVerticalScrollMode( ScrollPerPixel );
        m_FrozenTableView->setVerticalScrollMode( ScrollPerPixel );

        //check resize policies
        if ( true == m_resizeColumnsToContents )
        {
            m_FrozenTableView->resizeColumnsToContents();
        }

        if ( true == m_resizeRowsToContents )
        {
            m_FrozenTableView->resizeRowsToContents();
        }
    }
}

void BasarTableWidget::disableFrozenView()
{
    m_FrozenColumns = 0;

    delete m_FrozenTableView;

    m_FrozenTableView = nullptr;
}

void BasarTableWidget::resizeEvent( QResizeEvent* event )
{
    QTableView::resizeEvent( event );
    updateFrozenTableViewGeometry();
}

QModelIndex BasarTableWidget::moveCursor( CursorAction cursorAction,
        Qt::KeyboardModifiers modifiers )
{
    QModelIndex current = QTableView::moveCursor( cursorAction, modifiers );

    const int sumColumnWidth = getSumColumnWidth();

    if ( cursorAction == MoveLeft && current.column() > 0
            && visualRect( current ).topLeft().x() < sumColumnWidth )
    {
        const int newValue = horizontalScrollBar()->value() + visualRect( current ).topLeft().x() - sumColumnWidth;
        horizontalScrollBar()->setValue( newValue );
    }

    //jump up to other view
    if ( nullptr != m_FrozenTableView && current.column() < m_FrozenColumns )
    {
        m_FrozenTableView->setCurrentIndex( current );
    }

    return current;
}

void BasarTableWidget::resizeRowsToContents()
{
    QTableView::resizeRowsToContents();

    if ( nullptr != m_FrozenTableView )
    {
        m_FrozenTableView->resizeRowsToContents();
    }

}
void BasarTableWidget::resizeColumnsToContents()
{
    QTableView::resizeColumnsToContents();

    if ( nullptr != m_FrozenTableView )
    {
        m_FrozenTableView->resizeColumnsToContents();
    }
}


/*!
* slot that is called at this->model(), SIGNAL( rowsInserted )
* resets the AbsoluteVerticalHeaders to the new rows, as the RowHeader
* is used as representation of the Index, rather than as a representation
* of the row
* @param parent   modelIndex from SIGNAL( rowsInserted )
* @param start    start from SIGNAL( rowsInserted )
* @param end      end from SIGNAL( rowsInserted )
*/
void BasarTableWidget::handleRowsInserted( const QModelIndex& /* parent */, int /* start */, int /* end */ )
{
    if ( m_useAbsoluteVerticalHeaders.first )
    {
        setVerticalHeaderLabels ( m_useAbsoluteVerticalHeaders.second );
    }
}

/*!
* slot that is called at this->model(), SIGNAL( rowsRemoved )
* resets the AbsoluteVerticalHeaders to the new rows, as the RowHeader
* is used as representation of the Index, rather than as a representation
* of the row
* @param parent   modelIndex from SIGNAL( rowsRemoved )
* @param start    start from SIGNAL( rowsRemoved )
* @param end      end from SIGNAL( rowsRemoved )
*/
void BasarTableWidget::handleRowsRemoved ( const QModelIndex& /* parent */, int /* start */, int /* end */ )
{
    if ( m_useAbsoluteVerticalHeaders.first )
    {
        setVerticalHeaderLabels ( m_useAbsoluteVerticalHeaders.second );
    }
}
/*!
* slot that is called by the Shortcut that selects one line,
* note: if selectRow is used and the Shortcut has a Shift modifier
* more or none rows are selected.
*/
void BasarTableWidget::selectAlwaysOnlyOneRow( int row )
{
    //only react if passed row is in widget
    if ( row < this->rowCount() )
    {
        clearSelection();
        QTableWidgetSelectionRange range(  row,   0,   row, columnCount() - 1 );

        // the selected row shall also be current row (op 207):
        setCurrentCell( row, currentColumn() );

        // set the selection on the whole row:
        setRangeSelected ( range, true );
    }

}

void BasarTableWidget::updateSectionWidth( int logicalIndex, int /* oldSize */, int newSize )
{
    if ( logicalIndex < m_FrozenColumns )
    {
        if ( nullptr != m_FrozenTableView )
        {
            m_FrozenTableView->setColumnWidth( logicalIndex, newSize );
        }

        updateFrozenTableViewGeometry();
    }
}

void BasarTableWidget::updateSectionHeight( int logicalIndex, int /* oldSize */, int newSize )
{
    if ( nullptr != m_FrozenTableView )
    {
        m_FrozenTableView->setRowHeight( logicalIndex, newSize );
    }
}

void BasarTableWidget::onCellActivated( const QModelIndex& index )
{
    emit this->cellActivated( index.row(), index.column() );
}

void BasarTableWidget::onCellClicked( const QModelIndex& index )
{
    emit this->cellClicked( index.row(), index.column() );
}

void BasarTableWidget::onCellDoubleClicked( const QModelIndex& index )
{
    emit this->cellDoubleClicked( index.row(), index.column() );
}

void BasarTableWidget::onCellEntered( const QModelIndex& index )
{
    emit this->cellEntered( index.row(), index.column() );
}

void BasarTableWidget::onCellPressed( const QModelIndex& index )
{
    emit this->cellPressed( index.row(), index.column() );
}

