//----------------------------------------------------------------------------
/*! \file
 *  \brief  public library interface
 *  \author Roland Kiefert
 *  \date   19.09.2005
 */
//----------------------------------------------------------------------------
#ifndef GUARD_LIBBASARGUITIE_DEFINITIONS_H
#define GUARD_LIBBASARGUITIE_DEFINITIONS_H

//----------------------------------------------------------------------------
#include <boost/shared_ptr.hpp>
#include "libbasar_definitions.h"

//----------------------------------------------------------------------------
// forward declaration
class QObject;

//----------------------------------------------------------------------------
namespace basar {
namespace gui {
namespace tie {

//----------------------------------------------------------------------------
//! array for the names of widget controls
const char * const widgetString [] = 
{
	"QLabel",
	"QLineEdit",
	"QTextEdit",
	"QComboBox",
	"QPushButton",
	"QListWidget",
	"QTableWidget",
	"QCheckBox",
	"QRadioButton",
	"QDateTimeEdit",
	"QTimeEdit",
	"QDateEdit",
	"BasarTableWidget",
	"MultiComboBox",
	"BasarDateTimeEdit",
	"BasarDateEdit",
	"BasarTimeEdit",
	"QGroupBox",

	"",
	"unknown widget"
};

//----------------------------------------------------------------------------
//! enum widgetTypes defines the position in the widgetString
enum WidgetTypes 
{	
	qFirstWidget = 0			,			//!< first widget

	qLabel		 = qFirstWidget	,			//!< position for Label
	qLineEdit					,			//!< position for LineEdit
	qTextEdit					,			//!< position for TextEdit
	qComboBox					,			//!< position for ComboBox
	qPushButton					,			//!< position for PushButton
	qListWidget					,			//!< position for ListWidget
	qTableWidget				,			//!< position for TableWidget
	qCheckBox                   ,           //!< position for CheckBox
	qRadioButton                ,           //!< position for RadioButton
	qDateTimeEdit               ,           //!< position for DateTimeEdit
	qTimeEdit                   ,           //!< position for TimeEdit
	qDateEdit                   ,           //!< position for DateEdit
	basarTableWidget            ,           //!< position for basarTableWidget
	multiComboBox               ,           //!< position for MultiComboBox
	basarDateTimeEdit			,			//!< position for basarDateTimeEdit
	basarDateEdit				,			//!< position for basarDateEdit
	basarTimeEdit				,			//!< position for basarTimeEdit
	qGroupBox				    ,           //!< position for GroupBox

	qUnset						,			//!< is not set
	qUnknown					,			//!< unknown widget

	qLastWidget  = qUnset - 1  	,			//! last widget -> necessary for number of enum items
};

//----------------------------------------------------------------------------
/*! \brief WidgetPolicy assigns a certain value for every ctrl and one value for all ctrls 
 *  to be able to decide which ctrl must not be read in. 
 *  \note important: corresponds to WidgetTypes' order! */
enum WidgetPolicyEnum 
{
	WITH_ALL_CTRLS				= 0x00000,	//!< with all controls 

	WITHOUT_LABEL				= 0x00001,	//!< without widget Label
	WITHOUT_LINEEDIT			= 0x00002,	//!< without widget LineEdit
	WITHOUT_TEXTEDIT			= 0x00004,	//!< without widget TextEdit
	WITHOUT_COMBOBOX			= 0x00008,	//!< without widget ComboBox
	WITHOUT_PUSHBUTTON			= 0x00010,	//!< without widget PushButton
	WITHOUT_LISTWIDGET			= 0x00020,	//!< without widget ListWidget
	WITHOUT_TABLEWIDGET			= 0x00040,	//!< without widget TableWidget
	WITHOUT_CHECKBOX			= 0x00080,	//!< without widget CheckBox
	WITHOUT_RADIOBUTTON			= 0x00100,	//!< without widget RadioButton
	WITHOUT_DATETIMEEDIT		= 0x00200,	//!< without widget DateTimeEdit
	WITHOUT_TIMEEDIT			= 0x00400,	//!< without widget TimeEdit
	WITHOUT_DATEEDIT			= 0x00800,	//!< without widget DateEdit
	WITHOUT_BASARTABLEWIDGET	= 0x01000,	//!< without widget BasarTableWidget
	WITHOUT_MULTICOMBOBOX		= 0x02000,	//!< without widget MultiComboBox
	WITHOUT_BASARDATETIMEEDIT   = 0x04000,	//!< without widget BasarDateTimeEdit
	WITHOUT_BASARDATEEDIT		= 0x08000,	//!< without widget BasarDateEdit
	WITHOUT_BASARTIMEEDIT       = 0x10000, 	//!< without widget BasarTimeEdit
	WITHOUT_GROUPBOX			= 0x20000,	//!< without widget GroupBox
};

//----------------------------------------------------------------------------
//! \brief enum EventPolicyEnum defines all possible event states
//! \note important: corresponds to WidgetTypes' order!
enum EventPolicyEnum 
{	
	LABEL_ON				= 0x00000001,	//!< event handling switch for widget Label
	LINEEDIT_ON				= 0x00000002,	//!< event handling switch for widget LineEdit
	TEXTEDIT_ON				= 0x00000004,	//!< event handling switch for widget TextEdit
	COMBOBOX_ON				= 0x00000008,	//!< event handling switch for widget ComboBox
	PUSHBUTTON_ON			= 0x00000010,	//!< event handling switch for widget PushButton
	LISTWIDGET_ON			= 0x00000020,	//!< event handling switch for widget ListWidget
	TABLEWIDGET_ON			= 0x00000040,	//!< event handling switch for widget TableWidget
	CHECKBOX_ON				= 0x00000080,	//!< event handling switch for widget CheckBox
	RADIOBUTTON_ON			= 0x00000100,	//!< event handling switch for widget RadioButton
	DATETIMEEDIT_ON			= 0x00000200,	//!< event handling switch for widget DateTimeEdit
	TIMEEDIT_ON				= 0x00000400,	//!< event handling switch for widget TimeEdit
	DATEEDIT_ON				= 0x00000800,	//!< event handling switch for widget DateEdit
	BASARTABLEWIDGET_ON		= 0x00001000,	//!< event handling switch for widget BasarTableWidget
	MULTICOMBOBOX_ON		= 0x00002000,	//!< event handling switch for widget MultiComboBox
	BASARDATETIMEEDIT_ON	= 0x00004000,	//!< event handling switch for widget BasarDateTimeEdit
	BASARDATEEDIT_ON		= 0x00008000,	//!< event handling switch for widget BasarDateEdit
	BASARTIMEEDIT_ON		= 0x00010000,	//!< event handling switch for widget BasarTimeEdit
	GROUPBOX_ON				= 0x00020000,	//!< event handling switch for widget GroupBox

	LOCAL_ON				= 0x40000000,	//!< event handling switch for local eventhandler
	GLOBAL_ON				= 0x80000000,	//!< event handling switch for global eventhandler
};

//! Event Policy values are composed by EventPolicyEnum bits
typedef Int32 EventPolicyType;

//----------------------------------------------------------------------------
//! \brief structure to correlate widget with eventhandler
struct IdEventHandler
{
	WidgetTypes					type;				//!< widget type that eventhandler handles
	boost::shared_ptr<QObject>  pEventHandler;		//!< eventhandler's address
};

//----------------------------------------------------------------------------
//! enum MatcherPolicyEnum defines all possible states of matcher
enum MatcherPolicyEnum 
{	
	INSERT				= 0x01,		//!< if there are more lines in one table as in the other one -> insert a row
	CONVERTTYPE			= 0x02,		//!< if the name of a property is the same, but the type is different -> typecast	
	SKIP				= 0x04,		//!< if a column is only in one property table you leave it out.
	DISREGARD_DELETED	= 0x08		//!< if a row is in state SS_DELETE then it is ignored.
};

//----------------------------------------------------------------------------
//! enum WidgetReturnEnum defines all possible return states of a widget
enum WidgetReturnEnum 
{	
	WIDGET_CANCELED	= -1,	//!< cancel button pressed on dialog
	WIDGET_OK		=  0	//!< ok button pressed on dialog
};

//---------------------------------------------------------------------------------------//
// enum section
//---------------------------------------------------------------------------------------//
/*! \brief  caching policy of a multi line ctrl, representing if all shown lines are stored or not */
enum MemoryPolicyEnum 
{
	/*! \brief	means that hits which should be shown in a multi line ctrl 
				will only be added if the necessary lines are left in the ctrl. 
				Otherwise so many hits will be erased of the ctrl as needed, so that 
				a vertical scroll bar will never appear.*/
	WITHOUT_MEMORY, 
	/*! \brief	means that all shown hits of a multi line ctrl will 
				automatically be appended */
	WITH_MEMORY		
};

//----------------------------------------------------------------------------
//! Matcher Policy values are composed by MatcherPolicyEnum bits
typedef Int32 MatcherPolicyType;

//----------------------------------------------------------------------------
/*!
 * Typedef for a struct that houses the Information,
 * from where a handleException/handleUnknownException Function
 * is called. The Information on the location of the call is
 * structured as the information of the location where the
 * exception is thrown, therefor basar::ExceptInfo
 * is used as value, the exceptionType Value is currently unused.
 */
typedef basar::ExceptInfo ExceptionFoundInfo;

//----------------------------------------------------------------------------
} // tie
} // gui
} // basar

#endif // GUARD
