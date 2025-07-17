//----------------------------------------------------------------------------
/*! \file
 *  \brief  helper methods
 *  \author Michael Eichenlaub
 *  \date   23.11.2005
 */
//----------------------------------------------------------------------------

#include "helper.h"

#include <boost/make_shared.hpp>

#include "libbasarcmnutil_datetimetools.h"

#include "labelbehavour.h"
#include "lineeditbehavour.h"
#include "texteditbehavour.h"
#include "comboboxbehavour.h"
#include "pushbuttonbehavour.h"
#include "listwidgetbehavour.h"
#include "tablewidgetbehavour.h"
#include "checkboxbehavour.h"
#include "radiobuttonbehavour.h"
#include "datetimeeditbehavour.h"
#include "timeeditbehavour.h"
#include "dateeditbehavour.h"
#include "basartablewidgetbehavour.h"
#include "multicomboboxbehavour.h"
#include "basardatetimeeditbehaviour.h"
#include "basardateeditbehaviour.h"
#include "basartimeeditbehaviour.h"
#include "groupboxbehavour.h"

#include "libbasarproperty_propertystate.h"
#include "libbasardbaspect_accessorpropertytable_yiterator.h"

#pragma warning (push)
#pragma warning (disable: 4231 4244 4481 4512 4800)
#include <QtWidgets/QWidget>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QTextEdit>
#include <QtWidgets/QGroupBox>
#include <QtWidgets/QListWidget>
#include <QtWidgets/QTableWidget>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QComboBox>
#include <QtWidgets/QCheckBox>
#include <QtWidgets/QRadioButton>
#include <QtWidgets/QDateEdit>
#include <QtWidgets/QDateTimeEdit>
#include <QtWidgets/QTimeEdit>
#pragma warning (pop)

#include "libbasarqtwidget_tablewidget.h"
#include "libbasarqtwidget_multicombobox.h"
#include "libbasarqtwidget_datetimeedit.h"
#include "libbasarqtwidget_dateedit.h"
#include "libbasarqtwidget_timeedit.h"

#pragma warning (disable: 4505)

//----------------------------------------------------------------------------
namespace basar {
namespace gui   {
namespace tie   {

//----------------------------------------------------------------------------
using cmnutil::DateTimeTools;

//----------------------------------------------------------------------------
//! \throw no-throw
//! \return shared pointer of controlbehavour of given QObject
ControlBehavourSharedPtr Helper::getControlBehavour ( const QObject               & obj,
													  GuiPropertyTable            * pt,
													  std::list<QWidget*>::iterator it)
{

    ControlBehavourSharedPtr p;

	switch (getWidgetType(obj))
	{
    case qLabel:			p = boost::make_shared<LabelBehavour>(pt, it); break;
	case qLineEdit:			p = boost::make_shared<LineEditBehavour>(pt, it); break;
	case qTextEdit:			p = boost::make_shared<TextEditBehavour>(pt, it); break;
	case qComboBox:			p = boost::make_shared<ComboBoxBehavour>(pt, it); break;
	case qPushButton:		p = boost::make_shared<PushButtonBehavour>(pt, it); break;
	case qListWidget:		p = boost::make_shared<ListWidgetBehavour>(pt, it); break;
	case qTableWidget:		p = boost::make_shared<TableWidgetBehavour>(pt, it); break;
	case qCheckBox:			p = boost::make_shared<CheckBoxBehavour>(pt, it); break;
	case qRadioButton:		p = boost::make_shared<RadioButtonBehavour>(pt, it); break;
	case qDateTimeEdit:		p = boost::make_shared<DateTimeEditBehavour>(pt, it); break;
	case qTimeEdit:			p = boost::make_shared<TimeEditBehavour>(pt, it); break;
	case qDateEdit:			p = boost::make_shared<DateEditBehavour>(pt, it); break;
	case basarTableWidget:	p = boost::make_shared<BasarTableWidgetBehavour>(pt, it); break;
	case multiComboBox:		p = boost::make_shared<MultiComboBoxBehavour>(pt, it); break;
	case basarDateTimeEdit:	p = boost::make_shared<BasarDateTimeEditBehaviour>(pt, it); break;
	case basarDateEdit:		p = boost::make_shared<BasarDateEditBehaviour>(pt, it); break;
	case basarTimeEdit:		p = boost::make_shared<BasarTimeEditBehaviour>(pt, it); break;
	case qGroupBox:			p = boost::make_shared<GroupBoxBehavour>(pt, it); break;

	default:				p = boost::make_shared<ControlBehavour>(pt, it);
	}

    //store reference to self as weak_ptr
    p->setSelf(p);
    //called here as function requires virtual invocation & the call to setSelf
    p->registerForNotification();

    return p;
}

//----------------------------------------------------------------------------
//! \throw no-throw
//! \return type of given QObject
WidgetTypes	Helper::getWidgetType(const QObject & obj)
{
	// !!! order is important because of multilevel inheritance hierarchy !!!

	// --- first check basar's widgets!
	if ( 0 != qobject_cast <const BasarTableWidget *> (&obj) )
		return basarTableWidget;

	if ( 0 != qobject_cast <const BasarDateTimeEdit *> (&obj) )
		return basarDateTimeEdit;

	if ( 0 != qobject_cast <const BasarDateEdit *> (&obj) )
		return basarDateEdit;

	if ( 0 != qobject_cast <const BasarTimeEdit *> (&obj) )
		return basarTimeEdit;

	if ( 0 != qobject_cast <const MultiComboBox *> (&obj) )
		return multiComboBox;


	// --- then check Qt's derived widgets!
	if ( 0 != qobject_cast <const QDateEdit *> (&obj) )
		return qDateEdit;

	if ( 0 != qobject_cast <const QTimeEdit *> (&obj) )
		return qTimeEdit;


	// --- then check Qt's core widgets!
	if ( 0 != qobject_cast <const QLineEdit *> (&obj) )
		return qLineEdit;

	if ( 0 != qobject_cast <const QTextEdit *> (&obj) )
		return qTextEdit;

	if ( 0 != qobject_cast <const QLabel *> (&obj) )
		return qLabel;

	if ( 0 != qobject_cast <const QGroupBox *> (&obj) )
		return qGroupBox;

	if ( 0 != qobject_cast <const QListWidget *> (&obj) )
		return qListWidget;

	if ( 0 != qobject_cast <const QTableWidget *> (&obj) )
		return qTableWidget;

	if ( 0 != qobject_cast <const QPushButton *> (&obj) )
		return qPushButton;

	if ( 0 != qobject_cast <const QComboBox *> (&obj) )
		return qComboBox;

	if ( 0 != qobject_cast <const QCheckBox *> (&obj) )
		return qCheckBox;

	if ( 0 != qobject_cast <const QRadioButton *> (&obj) )
		return qRadioButton;

	if ( 0 != qobject_cast <const QDateTimeEdit *> (&obj) )
		return qDateTimeEdit;

	return qUnknown;
}

//----------------------------------------------------------------------------
//! \throw  no-throw
//! \return widget's accessible (if not empty) or object name
VarString Helper::getWidgetName(const QWidget & widget)
{
	if (widget.accessibleName().isEmpty())
		return widget.objectName().toLatin1().constData();
	else
		return widget.accessibleName().toLatin1().constData();
}

//----------------------------------------------------------------------------
//! \throw no-throw
//! \retval true if valid string exists 
//! \return otherwise returns false
bool Helper::getStringFromAcc(db::aspect::AccessorPropertyTable_YIterator accit,													
							  property::NumberColumns                     index,
							  I18nString                                & value,
							  property::PropertyType                    & type ) 
{
	value = "";

	if (accit.getState(index) == SS_UNSET || accit.getState(index) == SS_UNKNOWN)
		return 	false;

	switch (type.getType())
	{
	case UNKNOWN:
		return false;

    case INT64:
        value.format("%lld", accit.getInt64(index));
        break;

	case INT32:
		value.format("%d",accit.getInt32(index));
		break;

	case INTDATE:
		{
			Date date(accit.getInt32(index));
			if (date.isValid())
				value = date.toStrDate();
		}
		break;

	case INTTIME:
		{
			Time time(Time::NULL_DATE, accit.getInt32(index));
			if (time.isValidTime())
				value = time.toStrTime();
		}
		break;

	case INT16:
		value.format("%d", accit.getInt16(index));
		break;
	
	case FLOAT64:
		value.format("%f", accit.getFloat64(index));
		break;
	
	case FLOAT32:
		value.format("%f", accit.getFloat32(index));
		break;
	
	case DATETIME:
		if (accit.getDateTime(index).isValid() ) 
            value = accit.getDateTime(index).toStrDateTime();
		break;

	case DATE:
		if (accit.getDate(index).isValid())
			value = accit.getDate(index).toStrDate();
		break;
		
	case TIME:
		if (accit.getTime(index).isValidTime())
			value = accit.getTime(index).toStrTime();
		break;

    case TIMESPAN:
        value = accit.getTimeSpan(index).toNormalizedString();
        break;

	case DECIMAL:
		value = accit.getDecimal(index).toString();
		break;
	
	case STRING:
		value = accit.getString(index);
		break;
	
	default:
		value = "UNKNOWN";
		return false;
	}

	return true;
}

//----------------------------------------------------------------------------
//! \throw no-throw
//! \retval true if type is valid 
//! \return otherwise returns false
bool Helper::setStringToAcc(db::aspect::AccessorPropertyTable_YIterator accit,
							property::NumberColumns                     index,
							const I18nString                          & value,
							property::PropertyType                    & type,
							VarString                                 & msg)
{
	msg = "";

	switch (type.getType())
	{
    case INT64:
        if ( ! value.empty() )
            accit.setInt64(index, value.stoll());
        break;
	case INT32:
		if ( ! value.empty() )
			accit.setInt32(index, value.stoi());
		break;

	case INTDATE:
		{
			Date date;
			date.fromStrDate(value);
			if (date.isValid())
				accit.setInt32(index, date.getDate());
		}
		break;

	case INTTIME:
		{
			Time time;
			time.fromStrTime(value);
			if (time.isValidTime())
				accit.setInt32(index, time.getTime());
		}
		break;

	case INT16:
		if ( ! value.empty() )
			accit.setInt16(index, static_cast<Int16>(value.stoi()));
		break;
	
	case FLOAT64:
		if ( ! value.empty() )
			accit.setFloat64(index, value.stof());
		break;

	case FLOAT32:
		if ( ! value.empty() )
			accit.setFloat32(index, static_cast<Float32>(value.stof()));
		break;
	
	case DECIMAL:
		if ( ! value.empty() )
			accit.setDecimal(index, Decimal(value));
		break;

	case STRING:
		accit.setString(index, value);
		break;
	
	case DATETIME:
		{
			DateTime datetime;
			datetime.fromStrDateTime(value);
			if (datetime.isValid())
				accit.setDateTime(index, datetime);
		}
		break;

	case DATE:
		{
			Date date;
			date.fromStrDate(value);
			if (date.isValid())
				accit.setDate(index, date);
		}
		break;

	case TIME:
		{
			Time tm;
			tm.fromStrTime(value);
			if (tm.isValidTime())
				accit.setTime(index, tm);
		}
		break;
    case TIMESPAN:
        {
            TimeSpan ts(value);
			if (ts.isValid())
			{
				accit.setTimeSpan(index, ts);
			}
        }
		break;
	default:
		msg = "The type is unknown.";
		return false;
	}

	return true;
}

//----------------------------------------------------------------------------
//! \throw no-throw
//! \retval true if valid datetime exists
//! \return otherwise returns false
bool Helper::getDateTimeFromAcc(db::aspect::AccessorPropertyTable_YIterator accit,													
							    property::NumberColumns                     index,
							    DateTime                                  & value,
							    property::PropertyType                    & type) 
{
	value.setInvalid();

	if (accit.getState(index) == SS_UNSET  || 
		accit.getState(index) == SS_UNKNOWN  )
		return true;

	switch (type.getType())
	{
	case DATETIME:
		value = accit.getDateTime(index);
		break;

	case UNKNOWN:
	default:
		return false;
	}

	return true;
}

//----------------------------------------------------------------------------
//! \throw no-throw
//! \retval true if valid timespan exists
//! \return otherwise returns false
bool Helper::getTimeSpanFromAcc(db::aspect::AccessorPropertyTable_YIterator accit,													
							    property::NumberColumns                     index,
							    TimeSpan                                  & value,
							    property::PropertyType                    & type) 
{
	value.setInvalid();

	if (accit.getState(index) == SS_UNSET  || 
		accit.getState(index) == SS_UNKNOWN  )
		return true;

	switch (type.getType())
	{
	case TIMESPAN:
		value = accit.getTimeSpan(index);
		break;

	case UNKNOWN:
	default:
		return false;
	}

	return true;
}

//----------------------------------------------------------------------------
//! \throw no-throw
//! \retval true if valid datetime exists
//! \return otherwise returns false
bool Helper::getTimeFromAcc(db::aspect::AccessorPropertyTable_YIterator accit,													
							property::NumberColumns                     index,
							Time                                      & value,
							property::PropertyType                    & type) 
{
	value.setInvalidTime();

	if ( accit.getState(index) == SS_UNSET   || 
		 accit.getState(index) == SS_UNKNOWN   )
		return 	true;

	switch (type.getType())
	{
	case INTTIME:
	case INT32:
		{
			Int32 tm = accit.getInt32(index);

			if (DateTimeTools::isValidTime(tm))
				value.setTime(tm);
			else
				value.setInvalid();
		}
		break;

	case TIME:
		value = accit.getTime(index);
		break;

	case UNKNOWN:
	default:
		return false;
	}

	return true;
}

//----------------------------------------------------------------------------
//! \throw no-throw
//! \retval true if valid datetime exists
//! \return otherwise returns false
bool Helper::getDateFromAcc(db::aspect::AccessorPropertyTable_YIterator accit,													
							property::NumberColumns                     index,
							Date                                      & value,
							property::PropertyType                    & type) 
{
	value.setInvalid();

	if (accit.getState(index) == SS_UNSET  || 
		accit.getState(index) == SS_UNKNOWN  )
		return 	true;

	switch (type.getType())
	{
	case INTDATE:
	case INT32:
		{
			Int32 dt = accit.getInt32(index);

			if ( DateTimeTools::isValidDate(dt) )
				value.setDate(dt);
			else 
				value.setInvalid();
		}
		break;

	case DATE:
		value = accit.getDate(index);
		break;

	case UNKNOWN:
	default:
		return false;
	}

	return true;
}

//----------------------------------------------------------------------------
//! \throw no-throw
//! \retval true if type is valid 
//! \return otherwise returns false
bool Helper::setDateTimeToAcc(db::aspect::AccessorPropertyTable_YIterator accit,
							  property::NumberColumns                     index,
							  const DateTime                            & value,
							  property::PropertyType                    & type,
							  VarString                                 & msg
						     )
{
	msg = "";

	switch (type.getType())
	{
	case DATETIME:
		accit.setDateTime(index,value);
		break;

	default:
		msg = "The type is unknown.";
		return false;
	}

	return true;
}

//----------------------------------------------------------------------------
//! \throw no-throw
//! \retval true if type is valid 
//! \return otherwise returns false
bool Helper::setTimeToAcc(db::aspect::AccessorPropertyTable_YIterator accit,
						  property::NumberColumns                     index,
						  const Time                                & value,
						  property::PropertyType                    & type,
						  VarString                                 & msg)
{
	msg = "";

	switch (type.getType())
	{
	case INT32:
	case INTTIME:
		//! never thousands seconds from gui to db
		accit.setInt32(index,value.getTime());
		break;

	case TIME:
		accit.setTime(index,value);
		break;

	default:
		msg = "The type is unknown.";
		return false;
	}

	return true;
}
//----------------------------------------------------------------------------
//! \throw no-throw
//! \retval true if type is valid 
//! \return otherwise returns false
bool Helper::setDateToAcc( db::aspect::AccessorPropertyTable_YIterator accit,
						   property::NumberColumns                     index,
						   const Date                                & value,
						   property::PropertyType                    & type,
						   VarString                                 & msg)
{
	msg = "";

	switch (type.getType())
	{
	case INT32:
	case INTDATE:
		accit.setInt32(index, value.getDate());
		break;

	case DATE:
		accit.setDate(index, value);
		break;

	default:
		msg = "The type is unknown.";
		return false;
	}

	return true;
}
//------------------------------------------------------------------------------
}	// namespace tie
}	// namespace gui
}	// namespace basar
