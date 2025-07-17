#include <UnitTest++/UnitTest++.h>
#include "libbasarguitie.h"

#include "qtderived.h"

// --------------------------------------------------------------------------
DateEditDerived::DateEditDerived (const QDate & date , QWidget * parent)
					             : QDateEdit(date, parent)	
{
}

// --------------------------------------------------------------------------
TimeEditDerived::TimeEditDerived (const QTime & time , QWidget * parent)
								 : QTimeEdit(time, parent)	
{
}

// --------------------------------------------------------------------------
DateTimeEditDerived::DateTimeEditDerived (const QDateTime & datetime , QWidget * parent)
										 : QDateTimeEdit(datetime, parent)	
{
}
