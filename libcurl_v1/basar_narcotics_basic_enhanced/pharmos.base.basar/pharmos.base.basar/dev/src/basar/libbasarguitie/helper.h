//----------------------------------------------------------------------------
/*! \file
 *  \brief  helper methods
 *  \author Michael Eichenlaub
 *  \date   23.11.2005
 */
//----------------------------------------------------------------------------

#ifndef GUARD_LIBBASARGUITIE_HELPER_H
#define GUARD_LIBBASARGUITIE_HELPER_H

//----------------------------------------------------------------------------
#include <boost/shared_ptr.hpp>
#include <boost/weak_ptr.hpp>
#include "libbasarproperty_definitions.h"
#include "libbasarproperty_propertytype.h"
#include "libbasarguitie_definitions.h"

/*----------------------------------------------------------------------------*/
class QObject;
class QWidget;

/*----------------------------------------------------------------------------*/
namespace basar {
	namespace db {
		namespace aspect {
			class AccessorPropertyTable_YIterator;
		}
	}

namespace gui   {
namespace tie   {

/*----------------------------------------------------------------------------*/
class GuiPropertyTable;
class ControlBehavour;

//! \brief struct for matching accessor- and gui- propertytable
struct IndexStruct
{
	basar::property::ColumnIndex	accProperty;			//!< index in the propertytable of the accessor
	basar::property::PropertyType	accType;				//!< type in the propertytable of the accessor
	basar::property::ColumnIndex	guiProperty;			//!< index in the propertytable of the gui
	basar::property::PropertyType	guiType;				//!< type in the propertytable of the gui
};

//!	\brief	shared pointer to GuiPropertyTable
typedef boost::shared_ptr< ControlBehavour >	ControlBehavourSharedPtr;	

typedef boost::weak_ptr< ControlBehavour >      ControlBehaviourWeakPtr;

//------------------------------------------------------------------------------
//! class for useful gui-helper methods \n final class
class Helper
{
public:
	//! getting the control behavour for any Qt object \n no-throw 
	static ControlBehavourSharedPtr	getControlBehavour	(const QObject               & obj,	//!< Qt object to be checked
														 GuiPropertyTable            * pt,	//!< pointer to GuiPropertyTable
														 std::list<QWidget*>::iterator it	//!< iterator to QWidget-list
														);
	//! getting the basar widget type enum for any Qt object \n no-throw 
	static WidgetTypes		getWidgetType		(const QObject & obj						//!< Qt object to be checked
												);
	//! get widget's accessible name (if given) or object name	\n no-throw
	static VarString		getWidgetName		(const QWidget & widget						//! Qt widget to be queried
												);

	//! setting a string to an accessor				 \n no-throw 
	static bool				setStringToAcc		(db::aspect::AccessorPropertyTable_YIterator accit,	//!< y-iterator in which will be written
												 property::NumberColumns                     index,	//!< index of the field
												 const I18nString                          & value,	//!< value to be set
												 property::PropertyType                    & type,	//!< type of the iterator
												 VarString                                 & msg	//!< error message
												);

	//! getting a string from an accessor				 \n no-throw 
	static bool				getStringFromAcc	(db::aspect::AccessorPropertyTable_YIterator accit,	//!< y-iterator from which user wants to read
												 property::NumberColumns                     index,	//!< index from the field
												 I18nString                                & value,	//!< variable where the value will be put in
												 property::PropertyType                    & type	//!< type of the iterator
												);

	//! setting a datetime to an accessor				 \n no-throw 
	static bool				setDateTimeToAcc	(db::aspect::AccessorPropertyTable_YIterator accit,	//!< y-iterator in which will be written
												 property::NumberColumns                     index,	//!< index of the field
												 const DateTime                            & value,	//!< value to be set
												 property::PropertyType                    & type,	//!< type of the iterator
												 VarString                                 & msg	//!< error message
												);

	//! setting a time to an accessor					 \n no-throw 
	static bool				setTimeToAcc		(db::aspect::AccessorPropertyTable_YIterator accit,	//!< y-iterator in which will be written
												 property::NumberColumns                     index,	//!< index of the field
												 const Time                                & value,	//!< value to be set
												 property::PropertyType                    & type,	//!< type of the iterator
												 VarString                                 & msg	//!< error message
												);

	//! setting a date to an accessor					 \n no-throw 
	static bool				setDateToAcc		(db::aspect::AccessorPropertyTable_YIterator accit,	//!< y-iterator in which will be written
												 property::NumberColumns                     index,	//!< index of the field
												 const Date                                & value,	//!< value to be set
												 property::PropertyType                    & type,	//!< type of the iterator
												 VarString                                 & msg	//!< error message
												);

	//! getting a datetime from an accessor				 \n no-throw 
	static bool				getDateTimeFromAcc	(db::aspect::AccessorPropertyTable_YIterator accit,	//!< y-iterator from which user wants to read
												 property::NumberColumns                     index,	//!< index of the field
												 DateTime                                  & value,	//!< variable where the value will be put in
												 property::PropertyType                    & type	//!< type of the iterator
												);

	//! getting a TimeSpan from an accessor				 \n no-throw 
	static bool				getTimeSpanFromAcc	(db::aspect::AccessorPropertyTable_YIterator accit,	//!< y-iterator from which user wants to read
												 property::NumberColumns                     index,	//!< index of the field
												 TimeSpan                                  & value,	//!< variable where the value will be put in
												 property::PropertyType                    & type	//!< type of the iterator
												);

	//! getting a time from an accessor					\n no-throw 
	static bool				getTimeFromAcc		(db::aspect::AccessorPropertyTable_YIterator accit,	//!< y-iterator from which user wants to read
												 property::NumberColumns                     index,	//!< index of the field
												 Time                                      & value,	//!< variable where the value will be put in
												 property::PropertyType                    & type	//!< type of the iterator
												);

	//! getting a datetime from an accessor				 \n no-throw 
	static bool				getDateFromAcc		(db::aspect::AccessorPropertyTable_YIterator accit,	//!< y-iterator from which user wants to read
												 property::NumberColumns                     index,	//!< index of the field
												 Date                                      & value,	//!< variable where the value will be put in
												 property::PropertyType                    & type	//!< type of the iterator
												);
};

//------------------------------------------------------------------------------
}	// namespace tie
}	// namespace gui
}	// namespace basar

//----------------------------------------------------------------------------
#endif	// GUARD
