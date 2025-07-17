//----------------------------------------------------------------------------
/*! \file
 *  \brief  plugin for tablewidget
 *  \author Roland Kiefert 
 *  \date   06.02.2006
 */
//----------------------------------------------------------------------------



#ifdef _WIN32
#pragma warning (push)
#pragma warning(disable: 4481) // C4481: nonstandard extension used: override specifier 'override'
#pragma warning(disable: 4512) // qobject_impl.h - C4512: 'QtPrivate::QSlotObjectBase' : assignment operator could not be generated
#endif
#include "libbasarqtwidget_tablewidget.h"
#ifdef _WIN32
#pragma warning (pop)
#endif

#include "tablewidgetplugin.h"

//!	\throw no-throw 
TableWidgetPlugin::TableWidgetPlugin(QObject *parent)
    : QObject(parent)
{
    initialized = false;
}

//!	\throw no-throw 
void TableWidgetPlugin::initialize(QDesignerFormEditorInterface * /* core */)
{
    if (initialized)
        return;

    initialized = true;
}

//!	\throw no-throw 
//! \retval true if the widget has been initialized;
bool TableWidgetPlugin::isInitialized() const
{
    return initialized;
}

//!	\throw no-throw
//! \return a new instance of the TableWidget, with the given parent.
QWidget *TableWidgetPlugin::createWidget(QWidget *parent)
{
    return new BasarTableWidget(parent);
}

//!	\throw no-throw 
//! \return the name of the class that implements the custom widget.
QString TableWidgetPlugin::name() const
{
    return "BasarTableWidget";
}

//!	\throw no-throw 
//! \return the name of the group to which the custom widget belongs.
QString TableWidgetPlugin::group() const
{
	return "Item Widgets (Item-Based)";
}

//!	\throw no-throw 
//! \return the icon used to represent the custom widget in Qt Designer's widget box.
QIcon TableWidgetPlugin::icon() const
{
    return QIcon();
}

//!	\throw no-throw 
//! \return a short description of the widget that can be used by Qt Designer in a tool tip.
QString TableWidgetPlugin::toolTip() const
{
    return "basar table widget with row and column properties";
}

//!	\throw no-throw 
//! \return a description of the widget that can be used by Qt Designer in "What's This?" help for the widget.
QString TableWidgetPlugin::whatsThis() const
{
    return "basar table widget with row and column properties";
}

//!	\throw no-throw 
//! \retval true if the custom widget is intended to be used as a container;
bool TableWidgetPlugin::isContainer() const
{
    return false;
}

//!	\throw no-throw 
//! \return the path to the include file that uic uses when creating code for the custom widget.
QString TableWidgetPlugin::includeFile() const
{
    return "libbasarqtwidget_tablewidget.h";
}

//!	\throw no-throw 
//! \return the code template that Qt Designer includes in forms that contain the custom widget when they are saved.
QString TableWidgetPlugin::codeTemplate() const
{
    return "";
}

