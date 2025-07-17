//-------------------------------------------------------------------------------------------------//
/*! \file
 *  \brief  Definition of QValidatorItemDelegate.
 *  \author	Frank Naumann
 *  \date   
 */
//-------------------------------------------------------------------------------------------------//

#include "commonheader.h"
#include "qvalidatoritemdelegate.h"

//-------------------------------------------------------------------------------------------------//
QValidatorItemDelegate::QValidatorItemDelegate( QObject* parent ) 
: QItemDelegate( parent )
{
}

//-------------------------------------------------------------------------------------------------//
QValidatorItemDelegate::~QValidatorItemDelegate()
{
}

//-------------------------------------------------------------------------------------------------//
void QValidatorItemDelegate::setColumnValidator( int column, QValidator* validator )
{
  m_Validators.insert(column, validator);
}

//-------------------------------------------------------------------------------------------------//
QWidget* QValidatorItemDelegate::createEditor( QWidget* parent, const QStyleOptionViewItem& option, const QModelIndex& index ) const
{
  QWidget* widget = QItemDelegate::createEditor( parent, option, index );

  if( m_Validators.contains( index.column() ) )
  {
    if( widget->inherits("QLineEdit") )
      dynamic_cast<QLineEdit*>(widget)->setValidator(m_Validators.value(index.column()));
    else if( widget->inherits("QComboBox") )
      dynamic_cast<QComboBox*>(widget)->setValidator(m_Validators.value(index.column()));
  }
  return widget;
}