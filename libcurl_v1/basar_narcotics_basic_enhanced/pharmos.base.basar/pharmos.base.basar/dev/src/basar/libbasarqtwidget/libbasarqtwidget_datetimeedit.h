//----------------------------------------------------------------------------
/*! \file
 *  \brief  basar datetime edit widget
 *  \author Michael Eichenlaub 
 *  \date   18.08.2009
 */
//----------------------------------------------------------------------------

#ifndef GUARD_LIBBASARQTWIDGET_DATETIMEEDIT_H
#define GUARD_LIBBASARQTWIDGET_DATETIMEEDIT_H

//----------------------------------------------------------------------------
#include <QtDesigner/QDesignerExportWidget>

#pragma warning (push)
#pragma warning(disable: 4127)
#pragma warning(disable: 4481)
#include <QtWidgets/QDateTimeEdit>
#pragma warning (pop)

//----------------------------------------------------------------------------
/*! \brief  basar datetime edit widget

	final class
*/
class QDESIGNER_WIDGET_EXPORT BasarDateTimeEdit : public QDateTimeEdit
{
	Q_OBJECT

public:
	//! c'tor	\n no-throw
	explicit			BasarDateTimeEdit	(QWidget         * parent = 0	//!< parent widget
											);
	//! c'tor	\n no-throw
	explicit			BasarDateTimeEdit	(const QDateTime & datetime,	//!< datetime to set
											 QWidget         * parent = 0	//!< parent widget
											);
	//! c'tor	\n no-throw
	explicit			BasarDateTimeEdit	(const QDate     & date,    	//!< date to set
											 QWidget         * parent = 0	//!< parent widget
											);
	//! c'tor	\n no-throw
	explicit			BasarDateTimeEdit	(const QTime     & time,		//!< time to set
											 QWidget         * parent = 0	//!< parent widget
											);

private:
						BasarDateTimeEdit	(const BasarDateTimeEdit & r);	//!< forbidden
	BasarDateTimeEdit & operator =			(const BasarDateTimeEdit & r);	//!< forbidden
};

//----------------------------------------------------------------------------
#endif	// GUARD_LIBBASARQTWIDGET_DATETIMEEDIT_H
