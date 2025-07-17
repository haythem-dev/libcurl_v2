//----------------------------------------------------------------------------
/*! \file
 *  \brief  interface for plugin for basar datetime edit widget
 *  \author Michael Eichenlaub 
 *  \date   18.08.2009
 */
//----------------------------------------------------------------------------

#ifdef _WIN32
#pragma warning (push)
#pragma warning(disable: 4481) // C4481: nonstandard extension used: override specifier 'override'
#pragma warning(disable: 4512) // qobject_impl.h - C4512: 'QtPrivate::QSlotObjectBase' : assignment operator could not be generated
#endif
#include "libbasarqtwidget_datetimeedit.h"
#ifdef _WIN32
#pragma warning (pop)
#endif


#include "basardatetimeeditplugin.h"

//----------------------------------------------------------------------------
//!	\throw no-throw
//!
BasarDateTimeEditPlugin::BasarDateTimeEditPlugin ( QObject *parent)
												 : QObject(parent)
{
    m_inited = false;
}

//----------------------------------------------------------------------------
//!	\throw no-throw
//!
void BasarDateTimeEditPlugin::initialize(QDesignerFormEditorInterface *)
{
    if (m_inited)
        return;

    m_inited = true;
}

//----------------------------------------------------------------------------
//!	\throw no-throw
//! \retval true if the widget has been initialized
//!
bool BasarDateTimeEditPlugin::isInitialized() const
{
    return m_inited;
}

//----------------------------------------------------------------------------
//!	\throw no-throw 
//! \return a new instance of the widget, with the given parent.
//!
QWidget *BasarDateTimeEditPlugin::createWidget(QWidget *parent)
{
    return new BasarDateTimeEdit(parent);
}

//----------------------------------------------------------------------------
//!	\throw no-throw 
//! \return the name of the class that implements the custom widget.
//!
QString BasarDateTimeEditPlugin::name() const
{
    return "BasarDateTimeEdit";
}

//----------------------------------------------------------------------------
//!	\throw no-throw 
//! \return QString giving group of widget
QString BasarDateTimeEditPlugin::group() const
{
	return "Input Widgets";
}

//----------------------------------------------------------------------------
//!	\throw no-throw 
//! \return default QIcon
QIcon BasarDateTimeEditPlugin::icon() const
{
    return QIcon();
}

//----------------------------------------------------------------------------
//!	\throw no-throw 
//! \return QString giving tooltip
QString BasarDateTimeEditPlugin::toolTip() const
{
    return "basar datetime edit widget";
}

//----------------------------------------------------------------------------
//!	\throw no-throw 
//! \return QString giving WhatsThis\?
QString BasarDateTimeEditPlugin::whatsThis() const
{
    return "basar datetime edit widget";
}

//----------------------------------------------------------------------------
//!	\throw no-throw 
//! \return bool indicating if this widget is a container
bool BasarDateTimeEditPlugin::isContainer() const
{
    return false;
}

//----------------------------------------------------------------------------
//!	\throw no-throw 
//! \return QString giving name of include file
QString BasarDateTimeEditPlugin::includeFile() const
{
    return "libbasarqtwidget_datetimeedit.h";
}

//----------------------------------------------------------------------------
//!	\throw no-throw 
//! \return QString indicating code template (here, empty string)
QString BasarDateTimeEditPlugin::codeTemplate() const
{
    return "";
}

