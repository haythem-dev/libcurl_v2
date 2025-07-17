//----------------------------------------------------------------------------
/*! \file
 *  \brief  interface for plugin for basar date edit widget
 *  \author Michael Eichenlaub 
 *  \date   18.08.2009
 */
//----------------------------------------------------------------------------

#ifdef _WIN32
#pragma warning (push)
#pragma warning(disable: 4481) // C4481: nonstandard extension used: override specifier 'override'
#pragma warning(disable: 4512) // qobject_impl.h - C4512: 'QtPrivate::QSlotObjectBase' : assignment operator could not be generated
#endif
#include "libbasarqtwidget_dateedit.h"
#ifdef _WIN32
#pragma warning (pop)
#endif


#include "basardateeditplugin.h"

//----------------------------------------------------------------------------
//!	\throw no-throw
//!
BasarDateEditPlugin::BasarDateEditPlugin ( QObject *parent)
										 : QObject(parent)
{
    m_inited = false;
}

//----------------------------------------------------------------------------
//!	\throw no-throw
//!
void BasarDateEditPlugin::initialize(QDesignerFormEditorInterface *)
{
    if (m_inited)
        return;

    m_inited = true;
}

//----------------------------------------------------------------------------
//!	\throw no-throw
//! \retval true if the widget has been initialized
//!
bool BasarDateEditPlugin::isInitialized() const
{
    return m_inited;
}

//----------------------------------------------------------------------------
//!	\throw no-throw 
//! \return a new instance of the widget, with the given parent.
//!
QWidget *BasarDateEditPlugin::createWidget(QWidget *parent)
{
    return new BasarDateEdit(parent);
}

//----------------------------------------------------------------------------
//!	\throw no-throw 
//! \return the name of the class that implements the custom widget.
//!
QString BasarDateEditPlugin::name() const
{
    return "BasarDateEdit";
}

//----------------------------------------------------------------------------
//!	\throw no-throw 
//! \return QString giving group of widget
QString BasarDateEditPlugin::group() const
{
	return "Input Widgets";
}

//----------------------------------------------------------------------------
//!	\throw no-throw 
//! \return default QIcon
QIcon BasarDateEditPlugin::icon() const
{
    return QIcon();
}

//----------------------------------------------------------------------------
//!	\throw no-throw 
//! \return QString giving tooltip
QString BasarDateEditPlugin::toolTip() const
{
    return "basar date edit widget";
}

//----------------------------------------------------------------------------
//!	\throw no-throw 
//! \return QString giving WhatsThis\?
QString BasarDateEditPlugin::whatsThis() const
{
    return "basar date edit widget";
}

//----------------------------------------------------------------------------
//!	\throw no-throw 
//! \return bool indicating if this widget is a container
bool BasarDateEditPlugin::isContainer() const
{
    return false;
}

//----------------------------------------------------------------------------
//!	\throw no-throw 
//! \return QString giving name of include file
QString BasarDateEditPlugin::includeFile() const
{
    return "libbasarqtwidget_dateedit.h";
}

//----------------------------------------------------------------------------
//!	\throw no-throw 
//! \return QString indicating code template (here, empty string)
QString BasarDateEditPlugin::codeTemplate() const
{
    return "";
}
