#ifndef GUARD_LIBBASARGUIQT_TABLEWIDGET_H
#define GUARD_LIBBASARGUIQT_TABLEWIDGET_H
//--------------------------------------------------------------------------------------------------------------//
/*! \file
 *  \brief  intern interface tablewidget
 *  \author Roland Kiefert
 *  \date   19.01.2006
 */
//--------------------------------------------------------------------------------------------------------------//

//--------------------------------------------------------------------------------------------------------------//
// includes
//--------------------------------------------------------------------------------------------------------------//
#include <QtDesigner/QDesignerExportWidget>

#pragma warning (push)
#pragma warning(disable: 4127 4512 4481)
#include <QtWidgets/QTableWidget>
#pragma warning (pop)

#include "libbasar_definitions.h"

//----------------------------------------------------------------------------
/*! \brief  A class to make a comfortable tablewidget.

    This is a final class.
*/
class QDESIGNER_WIDGET_EXPORT BasarTableWidget : public QTableWidget
{
        Q_OBJECT
        Q_PROPERTY( bool HideVerticalHeader READ isVerticalHeaderHidden WRITE hideVerticalHeader )      //!< hiding of vertical header
        Q_PROPERTY( bool HideHorizontalHeader READ isHorizontalHeaderHidden WRITE hideHorizontalHeader ) //!< hiding of horizontal header

    signals:
        //! \brief signal for data Selection change
        void dataSelectionChanged();

    protected slots:
        //! \brief slot sends data Selection change \throw no-throw
        void emitDataSelectionChanged();

        //! \brief slot sends data Selection change when selceted item changes \throw no-throw
        void handleItemChanged (
            QTableWidgetItem* item   //!< the changed Item
        );

        //! \brief handles new inserted rows (for m_rollingverticalHeaders)
        void handleRowsInserted( const QModelIndex& parent, int start, int end );

        //! \brief handles removed rows (for m_rollingverticalHeaders)
        void handleRowsRemoved ( const QModelIndex& parent, int start, int end );

        //! \brief used for row Shortcuts, that shall select exactly one line, even if the shortcurt uses modifier
        void selectAlwaysOnlyOneRow( int );

    private slots:
        void updateSectionWidth( int logicalIndex, int oldSize, int newSize );
        void updateSectionHeight( int logicalIndex, int oldSize, int newSize );

        void onCellActivated( const QModelIndex& index );
        void onCellClicked( const QModelIndex& index );
        void onCellDoubleClicked( const QModelIndex& index );
        void onCellEntered( const QModelIndex& index );
        void onCellPressed( const QModelIndex& index );

    public:
        //! \brief manage priority between row and column
        enum PriorEnum
        {
            ColumnSeniorToRow,  //!< column has higher priority
            RowSeniorToColumn   //!< row has higher priority
        };

        //! \brief constructor \n no-throw
        explicit BasarTableWidget( QWidget* parent = 0  //!< parent widget
                                 );
        //! \brief overloaded constructor \n no-throw
        BasarTableWidget( int rows,                     //!< number of rows in table widget
                          int columns,                    //!< number of columns in table widget
                          QWidget* parent = 0             //!< parent widget
                        );

        //! \brief sets the current item to *item. \n no-throw
        void setCurrentItem( QTableWidgetItem* item     //!< item which should be set
                           );
        //! \brief sets the item for the given row and column to *item. \n  no-throw
        void setItem( int row,                          //!< row of the item
                      int column,                        //!< column of the item
                      QTableWidgetItem* item             //!< item which should be set
                    );

        //! \brief defines the sortation if the row overwrites the column or the other way. \n no-throw
        void setPrior( PriorEnum prior                  //!< prior art of prior enum
                     );

        //! \brief checks if vertical header is hidden \n no-throw
        bool isVerticalHeaderHidden() const;
        //! \brief checks if horizontal header is hidden \n  no-throw
        bool isHorizontalHeaderHidden() const;
        //! \brief checks if resize rows to contents is set \n   no-throw
        bool isResizeRowsToContents() const;
        //! \brief checks if resize columns to contents is set \n    no-throw
        bool isResizeColumnsToContents() const;

        bool isStretchLastHorizontalSection() const;

        //! \brief hides the vertical header from the tablewidget \n no-throw
        void hideVerticalHeader( bool hidden = true     //!< true the header will be hidden
                               );
        //! \brief hides the horizontal header from the tablewidget \n no-throw
        void hideHorizontalHeader( bool hidden = true   //!< true the header will be hidden
                                 );

        //! \brief Sets the flags for the given row. \n no-throw
        void setFlagsRow( int row,                      //!< row of the table widget
                          Qt::ItemFlags flags            //!< the flags for the row
                        );
        //! \brief Sets the flag editable for the given row. \n no-throw
        void setEditableRow( int row,                   //!< row of the table widget
                             bool editable               //!< is the row editable?
                           );
        //! \brief Sets the background color for the given row. \n no-throw
        void setBackgroundColorRow( int row,            //!< row of the table widget
                                    const QColor& color  //!< the background color for the row
                                  );
        //! \brief Sets the text color for the given row. \n no-throw
        void setTextColorRow( int row,                  //!< row of the table widget
                              const QColor& color        //!< the text color for the row
                            );
        //! \brief Sets the font for the given row. \n no-throw
        void setFontRow( int row,                       //!< row of the table widget
                         const QFont& font               //!< the font for the row
                       );
        //! \brief Sets the height for the given row. \n no-throw
        void setRowHeight( int row,                 //!< row of the table widget
                           int height                //!< is the height for the row
                         );

        //! \brief Sets the flags for the given column. \n no-throw
        void setFlagsColumn( int column,            //!< column of the table widget
                             Qt::ItemFlags flags     //!< the flags for the column
                           );
        //! \brief Sets the flag "editable" for the given column. \n no-throw
        void setEditableColumn( int column,             //!< column of the table widget
                                bool editable            //!< is the column editable?
                              );
        //! \brief Sets the background color for the given column. \n no-throw
        void setBackgroundColorColumn( int column,          //!< column of the table widget
                                       const QColor& color   //!< the background color for the column
                                     );
        //! \brief Sets the text color for the given column. \n no-throw
        void setTextColorColumn( int column,            //!< column of the table widget
                                 const QColor& color     //!< the text color for the column
                               );
        //! \brief Sets the font for the given column. \n no-throw
        void setFontColumn( int column,             //!< column of the table widget
                            const QFont& font        //!< the font for the column
                          );

        //! \brief Sets the alignment for the given column. \n no-throw
        void setTextAlignmentColumn( int column,    //!< column of the table widget
                                     int alignment   //!< is the alignment for the column
                                   );

        //! \brief Sets the resize rows to contents for the tabel widget. \n no-throw
        void setResizeRowsToContents( bool resizeRowsToContents     //!< set the resize rows to contents for the table widget
                                    );
        //! \brief Sets the resize rows to contents for the tabel widget. \n no-throw
        void setResizeColumnsToContents( bool resizeColumnsToContents //!< set the resize columns to contents for the table widget
                                       );

        void setStretchLastHorizontalSection( bool stretchLastSection );

        //! \brief returns list of all horizontal header labels of the table widget. \n no-throw
        const std::list<basar::VarString> horizontalHeaderLabels() const;

        //! \brief returns list of all vertical header labels of the table widget. \n no-throw
        const std::list<basar::VarString> verticalHeaderLabels() const;

        //! \brief clears the tablewidget, but not the headers \n no-throw
        void clearContext();

        //! \brief uses A,B,C as rolling Vertical Header Names and Shotcuts to SelectRows \n no-throw
        void setABCVerticalHeaderPolicy( Qt::ShortcutContext context = Qt::WidgetShortcut );

        //! \brief rolling means, vertical Headers don't change when rows are inserted or removed \n no-throw
        void setAbsoluteVerticalHeaders( const QStringList& );

        //! \brief rolling means, vertical Headers don't change when rows are inserted or removed"\n no-throw
        void setAbsoluteVerticalHeaders( bool );

        //! \brief rolling means, vertical Headers don't change when rows are inserted or removed \n no-throw
        void setAbsoluteVerticalHeaders( const std::list<basar::VarString>& );

        //! \brief uses shortcutList (as strings) to select Rows \n no-throw
        void setShotcutsToSelectRows( const QStringList&, Qt::ShortcutContext context = Qt::WidgetShortcut );

        //! \brief uses shortcutList (as strings) to select Rows \n no-throw
        void setShotcutsToSelectRows( const std::list<basar::VarString>&, Qt::ShortcutContext context = Qt::WidgetShortcut );

        void enableFrozenView( basar::Int32 columns );

        void disableFrozenView();

        virtual void resizeEvent( QResizeEvent* event );
        virtual QModelIndex moveCursor( CursorAction cursorAction, Qt::KeyboardModifiers modifiers );

    public slots:

        void resizeRowsToContents();
        void resizeColumnsToContents();

    protected:

        //! \brief searches for the iterator with the global atrributes. \n no-throw
        bool findSort( int row,                         //!< row of the table widget item
                       int column                        //!< column of the table widget item
                     );
        //! \brief adds the global attributes to the item. \n no-throw
        int setAttributes( QTableWidgetItem* item   //!< item who will be add to the table widget
                         );
        //! \brief refreshes the items of the given column \n no-throw
        void refreshColumn( int column              //!< column of the table widget
                          );
        //! \brief refreshes the items of the given row \n no-throw
        void refreshRow( int row                    //!< row of the table widget
                       );

        //! \brief initialize slots \throw no-throw
        void initSlots();

        //! \brief initialize slots \throw no-throw
        void initRowSlots();

        //! \brief sends SelectedDataChanged Signal for PageUp/Down \throw no-throw
        virtual void keyPressEvent (
            QKeyEvent* event        //!< the event
        );

        //! \brief writes the entries from varstringlist into list
        inline void changeToQStringList(
            const std::list<basar::VarString>& varstringlist,  //!< list to be copied
            QStringList& list                                  //!< result list
        );

        inline void updateFrozenTableViewGeometry();

        inline int getSumColumnWidth() const;

        struct Attributes
        {
            bool            isFlags;                //!< flags is valid
            Qt::ItemFlags   flags;                  //!< the flags for the item
            bool            isBackground;           //!< background color is valid
            QColor          background;             //!< the background color for the item
            bool            isText;                 //!< text color is valid
            QColor          text;                   //!< the text color for the item
            bool            isFont;                 //!< font is valid
            QFont           font;                   //!< the font for the item
            bool            isEditable;             //!< editable is valid
            bool            editable;               //!< true item is editable
            bool            isHeight;               //!< height is valid
            int             height;                 //!< the height of the item
            bool            isAlignment;            //!< alignment is valid
            int             alignment;              //!< the alignment of the item
        };

        std::map<int, Attributes>           m_colColumnSort;                //!< map for the columns
        std::map<int, Attributes>           m_colRowSort;                   //!< map for the rows
        std::map<int, Attributes>::iterator m_colItRow;                     //!< iterator to the map row
        std::map<int, Attributes>::iterator m_colItColumn;                  //!< iterator to the map column
        PriorEnum                           m_prior;                        //!< sortation of the priority
        bool                                m_verticalHeader;               //!< true if vertical header is hidden
        bool                                m_horizontalHeader;             //!< true if horizontal header is hidden
        bool                                m_resizeRowsToContents;         //!< true if resizeRowsToContents is enabled
        bool                                m_resizeColumnsToContents;      //!< true if resizeColumnsToContents is enabled
        bool                                m_stretchLastHorizontalSection; //!< true if strechLastSection is enabled

        std::pair<bool, QStringList>        m_useAbsoluteVerticalHeaders;   //!< Vertical Headers rolling

        QTableView*                         m_FrozenTableView;              //!< Tableview for freezing the first n columns
        basar::Int32                        m_FrozenColumns;
};

#endif // GUARD_LIBBASARGUIQT_TABLEWIDGET_H

