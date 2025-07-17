//----------------------------------------------------------------------------
/*! \file
 *  \brief  basar date edit widget
 *  \author Michael Eichenlaub 
 *  \date   18.08.2009
 */
//----------------------------------------------------------------------------

#ifndef GUARD_LIBBASARQTWIDGET_DATEEDIT_H
#define GUARD_LIBBASARQTWIDGET_DATEEDIT_H

//----------------------------------------------------------------------------
#include <QtDesigner/QDesignerExportWidget>

#pragma warning (push)
#pragma warning(disable: 4127)
#include <QtWidgets/QDateEdit>
#pragma warning (pop)

//----------------------------------------------------------------------------
/*! \brief  basar date edit widget

	final class
*/
class QDESIGNER_WIDGET_EXPORT BasarDateEdit : public QDateEdit
{
	Q_OBJECT

public:
	//! c'tor	\n no-throw
	explicit		BasarDateEdit		(QWidget     * parent = 0	//!< parent widget
										);
	explicit		BasarDateEdit		(const QDate & date,		//!< date to set
		                                 QWidget     * parent = 0	//!< parent widget
										);
private:
					BasarDateEdit		(const BasarDateEdit & r);	//!< forbidden
	BasarDateEdit & operator =			(const BasarDateEdit & r);	//!< forbidden
};

//----------------------------------------------------------------------------
#endif	// GUARD_LIBBASARQTWIDGET_DATEEDIT_H
