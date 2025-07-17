//----------------------------------------------------------------------------
/*! \file
 *  \brief  a plugin for confortabel multiple column combobox
 *  \author Roland Kiefert
 *  \date   26.01.2006
 */
//----------------------------------------------------------------------------

#ifdef _WIN32
#pragma warning (push)
#pragma warning(disable: 4481) // C4481: nonstandard extension used: override specifier 'override'
#pragma warning(disable: 4512) // qobject_impl.h - C4512: 'QtPrivate::QSlotObjectBase' : assignment operator could not be generated
#endif
#include "libbasarqtwidget_multicombobox.h"
#ifdef _WIN32
#pragma warning (pop)
#endif

#include "multicomboboxplugin.h"

//!	\throw no-throw 
MultiCombBoxPlugin::MultiCombBoxPlugin(QObject *parent)
    : QObject(parent)
{
    initialized = false;
}

//!	\throw no-throw 
void MultiCombBoxPlugin::initialize(QDesignerFormEditorInterface * /* core */)
{
    if (initialized)
        return;

    initialized = true;
}

//!	\throw no-throw 
//! \retval true if the widget has been initialized;
bool MultiCombBoxPlugin::isInitialized() const
{
    return initialized;
}

//!	\throw no-throw 
//! \return a new instance of the TableWidget, with the given parent.
QWidget *MultiCombBoxPlugin::createWidget(QWidget *parent)
{
    return new MultiComboBox(parent);
}

//!	\throw no-throw 
//! \return the name of the class that implements the custom widget.
QString MultiCombBoxPlugin::name() const
{
    return "MultiComboBox";
}

//!	\throw no-throw
//! \return QString giving group of widget 
QString MultiCombBoxPlugin::group() const
{
	return "Input Widgets";
}

//!	\throw no-throw
//! \return default QIcon
QIcon MultiCombBoxPlugin::icon() const
{
    return QIcon();
}

//!	\throw no-throw
//! \return QString giving tooltip
QString MultiCombBoxPlugin::toolTip() const
{
    return "basar combobox with multi columns";
}

//!	\throw no-throw
//! \return QString giving WhatsThis\?
QString MultiCombBoxPlugin::whatsThis() const
{
    return "basar combobox with multi columns";
}

//!	\throw no-throw 
//! \return bool indicating if this widget is a container
bool MultiCombBoxPlugin::isContainer() const
{
    return false;
}

//!	\throw no-throw
//! \return QString giving name of include file
QString MultiCombBoxPlugin::includeFile() const
{
    return "libbasarqtwidget_multicombobox.h";
}

//!	\throw no-throw
//! \return QString indicating code template (here, empty string)
QString MultiCombBoxPlugin::codeTemplate() const
{
    return "";
}
