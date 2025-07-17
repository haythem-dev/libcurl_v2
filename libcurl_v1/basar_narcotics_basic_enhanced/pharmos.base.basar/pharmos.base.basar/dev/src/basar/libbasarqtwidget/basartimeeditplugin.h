//----------------------------------------------------------------------------
/*! \file
 *  \brief  interface for plugin for basar time edit widget
 *  \author Michael Eichenlaub 
 *  \date   18.08.2009
 */
//----------------------------------------------------------------------------

#ifndef GUARD_BASARTIMETIMEPLUGIN_H
#define GUARD_BASARTIMETIMEPLUGIN_H

//------------------------------------------------------------------------------
#pragma warning (push)
#pragma warning(disable: 4127) // C4127: conditional expression is constant
#pragma warning(disable: 4512) // qobject_impl.h - C4512: 'QtPrivate::QSlotObjectBase' : assignment operator could not be generated
#include <QtDesigner/QDesignerCustomWidgetInterface>
#pragma warning (pop)

//----------------------------------------------------------------------------
/*! \brief  to make a plugin for time edit widget.

	final class
*/
class BasarTimeEditPlugin : public QObject, 
							public QDesignerCustomWidgetInterface
{
    Q_OBJECT
    Q_INTERFACES(QDesignerCustomWidgetInterface)

public:
	//! c'tor	\n no-throw
						BasarTimeEditPlugin		(QObject *parent = 0					//!< parent widget
												);

	//! returns true if the custom widget is intended to be used as a container; otherwise returns false.	\n no-throw
    bool				isContainer				() const;
	//! returns true if the widget has been initialized; otherwise returns false.	\n no-throw
	bool				isInitialized			() const;

	//! returns the icon used to represent the custom widget in Qt Designer's widget box.	\n no-throw
    QIcon				icon					() const;
	//! returns the code template that Qt Designer includes in forms that contain the custom widget when they are saved.	\n no-throw
    QString				codeTemplate			() const;
	//! returns the name of the group to which the custom widget belongs.	\n no-throw
    QString				group					() const;
	//! returns the path to the include file that uic uses when creating code for the custom widget.	\n no-throw
    QString				includeFile				() const;
	//! returns the name of the class that implements the custom widget.	\n no-throw
    QString				name					() const;
	//! returns a short description of the widget that can be used by Qt Designer in a tool tip.	\n     no-throw
	QString				toolTip					() const;
	//! returns a description of the widget that can be used by Qt Designer in "What's This?" help for the widget.	\n no-throw
    QString				whatsThis				() const;
	//! returns a new instance of the custom widget, with the given parent.	\n no-throw
    QWidget *			createWidget			(QWidget * parent	//!< parent widget
												);
	//! initializes the widget for use with the specified formEditor interface.	\n no-throw
    void				initialize				(QDesignerFormEditorInterface * core	//!< interface to Qt designer's components
												);

private:
    bool				m_inited;				//!< true if the widget has been initialized
};

//------------------------------------------------------------------------------
#endif //GUARD

