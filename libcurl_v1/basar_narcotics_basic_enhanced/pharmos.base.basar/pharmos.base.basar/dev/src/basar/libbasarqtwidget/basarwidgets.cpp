//----------------------------------------------------------------------------
/*! \file
 *  \brief  a collection to all basar widgets
 *  \author Roland Kiefert
 *  \date   06.02.2006
 */
//----------------------------------------------------------------------------

#include "libbasarqtwidget_basarwidgets.h"

#include "tablewidgetplugin.h"
#include "multicomboboxplugin.h"
#include "basardatetimeeditplugin.h"
#include "basardateeditplugin.h"
#include "basartimeeditplugin.h"

#include <QtCore/qplugin.h>


//----------------------------------------------------------------------------
//!	\throw no-throw 
//!
BasarWidgets::BasarWidgets(QObject *parent) : QObject(parent)
{
	widgets.append(new TableWidgetPlugin      (this));
	widgets.append(new MultiCombBoxPlugin     (this));
	widgets.append(new BasarDateTimeEditPlugin(this));
	widgets.append(new BasarDateEditPlugin    (this));
	widgets.append(new BasarTimeEditPlugin    (this));
}

//----------------------------------------------------------------------------
//!	\throw no-throw 
//! \return QList filled with pointers to custom widgets
QList<QDesignerCustomWidgetInterface*> BasarWidgets::customWidgets() const
{
	return widgets;
}

//----------------------------------------------------------------------------

