//----------------------------------------------------------------------------
/*! \file
 *  \brief  basar time edit widget
 *  \author Michael Eichenlaub 
 *  \date   18.08.2009
 */
//----------------------------------------------------------------------------

#ifndef GUARD_LIBBASARQTWIDGET_TIMEEDIT_H
#define GUARD_LIBBASARQTWIDGET_TIMEEDIT_H

//----------------------------------------------------------------------------
#include <QtDesigner/QDesignerExportWidget>

#pragma warning (push)
#pragma warning(disable: 4127)
#pragma warning(disable: 4512)
#include <QtWidgets/QTimeEdit>
#pragma warning (pop)


//----------------------------------------------------------------------------
/*! \brief  basar time edit widget

	final class
*/
class QDESIGNER_WIDGET_EXPORT BasarTimeEdit : public QTimeEdit
{
	Q_OBJECT

public:
	//! c'tor	\n no-throw
	explicit		BasarTimeEdit		(QWidget     * parent = 0	//!< parent widget
										);
	//! c'tor	\n no-throw
	explicit		BasarTimeEdit		(const QTime & time,		//!< time to set
		                                 QWidget     * parent = 0	//!< parent widget
										);

private:
					BasarTimeEdit		(const BasarTimeEdit & r);	//!< forbidden
	BasarTimeEdit & operator =			(const BasarTimeEdit & r);	//!< forbidden

};

//----------------------------------------------------------------------------
#endif	// GUARD_LIBBASARQTWIDGET_TIMEEDIT_H
