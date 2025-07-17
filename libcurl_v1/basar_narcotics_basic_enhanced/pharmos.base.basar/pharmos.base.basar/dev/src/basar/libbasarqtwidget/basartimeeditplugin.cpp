//----------------------------------------------------------------------------
/*! \file
 *  \brief  interface for plugin for basar time edit widget
 *  \author Michael Eichenlaub 
 *  \date   18.08.2009
 */
//----------------------------------------------------------------------------

#ifdef _WIN32
#pragma warning (push)
#pragma warning(disable: 4481) // C4481: nonstandard extension used: override specifier 'override'
#pragma warning(disable: 4512) // qobject_impl.h - C4512: 'QtPrivate::QSlotObjectBase' : assignment operator could not be generated
#endif
#include "libbasarqtwidget_timeedit.h"
#ifdef _WIN32
#pragma warning (pop)
#endif

#include "basartimeeditplugin.h"

//----------------------------------------------------------------------------
//!	\throw no-throw
//!
BasarTimeEditPlugin::BasarTimeEditPlugin ( QObject *parent)
										 : QObject(parent)
{
    m_inited = false;
}

//----------------------------------------------------------------------------
//!	\throw no-throw
//!
void BasarTimeEditPlugin::initialize(QDesignerFormEditorInterface *)
{
    if (m_inited)
        return;

    m_inited = true;
}

//----------------------------------------------------------------------------
//!	\throw no-throw
//! \retval true if the widget has been initialized
//!
bool BasarTimeEditPlugin::isInitialized() const
{
    return m_inited;
}

//----------------------------------------------------------------------------
//!	\throw no-throw 
//! \return a new instance of the widget, with the given parent.
//!
QWidget *BasarTimeEditPlugin::createWidget(QWidget *parent)
{
    return new BasarTimeEdit(parent);
}

//----------------------------------------------------------------------------
//!	\throw no-throw 
//! \return the name of the class that implements the custom widget.
//!
QString BasarTimeEditPlugin::name() const
{
    return "BasarTimeEdit";
}

//----------------------------------------------------------------------------
//!	\throw no-throw 
//! \return QString giving group of widget
QString BasarTimeEditPlugin::group() const
{
	return "Input Widgets";
}

//----------------------------------------------------------------------------
//!	\throw no-throw 
//! \return default QIcon
QIcon BasarTimeEditPlugin::icon() const
{
    return QIcon();
}

//----------------------------------------------------------------------------
//!	\throw no-throw 
//! \return QString giving tooltip
QString BasarTimeEditPlugin::toolTip() const
{
    return "basar time edit widget";
}

//----------------------------------------------------------------------------
//!	\throw no-throw 
//! \return QString giving WhatsThis\?
QString BasarTimeEditPlugin::whatsThis() const
{
    return "basar time edit widget";
}

//----------------------------------------------------------------------------
//!	\throw no-throw 
//! \return bool indicating if this widget is a container
bool BasarTimeEditPlugin::isContainer() const
{
    return false;
}

//----------------------------------------------------------------------------
//!	\throw no-throw 
//! \return QString giving name of include file
QString BasarTimeEditPlugin::includeFile() const
{
    return "libbasarqtwidget_timeedit.h";
}

//----------------------------------------------------------------------------
//!	\throw no-throw 
//! \return QString indicating code template (here, empty string)
QString BasarTimeEditPlugin::codeTemplate() const
{
    return "";
}
