//-------------------------------------------------------------------------------------------------//
/*! \file
 *  \brief  Declaration of QValidatorItemDelegate.
 *  \author	Frank Naumann
 *  \date   
 */
//-------------------------------------------------------------------------------------------------//
#ifndef GUARD_QVALIDATORITEMDELEGATE_H
#define GUARD_QVALIDATORITEMDELEGATE_H

//-------------------------------------------------------------------------------------------------//
//includes
//-------------------------------------------------------------------------------------------------//
#pragma warning (push) // to avoid warnings from QT always put QT-includes between push - pop
#pragma warning(disable: 4127 4244 4311 4312 4481 4512 4800)
#include <QtWidgets/QItemDelegate>
#pragma warning (pop)

//-------------------------------------------------------------------------------------------------//
/*! \brief Qt Helper class which realizes validator functionality in TableWidgets by subclassing
              item delegates. */
class QValidatorItemDelegate : public QItemDelegate
{
public:
  /*! \brief        The standard constructor.
      \param parent The parent of the delegate.
      \throw        no-throw */
  QValidatorItemDelegate( QObject* parent = 0 );

  /*! \brief        The standard destructor.
      \throw        no-throw */
  virtual ~QValidatorItemDelegate();

  /*! \brief           Sets a validator on a specific column.
      \param column    The column, which will be restricted by validator.
      \param validator The validator to set on column.
      \throw           no-throw */
  void setColumnValidator( int column, QValidator* validator );

  /*! \brief        Overwritten Qt method, sets the correct validator on editable columns.
      \param parent The editor's parent widget.
      \param option The item options.
      \param index  The model index for which editor will be created.
      \return       The created editor QWidget.
      \throw        no-throw */
  virtual QWidget* createEditor( QWidget* parent, const QStyleOptionViewItem& option, const QModelIndex& index ) const;

private:
  QMap<int, QValidator*> m_Validators; //!< Map of column/validator associations.
};

#endif // GUARD_QVALIDATORITEMDELEGATE_H