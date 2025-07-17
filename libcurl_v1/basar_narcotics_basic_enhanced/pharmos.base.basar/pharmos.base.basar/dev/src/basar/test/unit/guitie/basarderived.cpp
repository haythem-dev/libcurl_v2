#include <UnitTest++/UnitTest++.h>
#include "libbasarguitie.h"

#include "basarderived.h"

// --------------------------------------------------------------------------
BasarDateEditDerived::BasarDateEditDerived (const QDate & date , QWidget * parent)
										    : BasarDateEdit(date, parent)	
{
}

// --------------------------------------------------------------------------
BasarTimeEditDerived::BasarTimeEditDerived (const QTime & time , QWidget * parent)
								           : BasarTimeEdit(time, parent)	
{
}

// --------------------------------------------------------------------------
BasarDateTimeEditDerived::BasarDateTimeEditDerived (const QDateTime & datetime , QWidget * parent)
										           : BasarDateTimeEdit(datetime, parent)	
{
}
