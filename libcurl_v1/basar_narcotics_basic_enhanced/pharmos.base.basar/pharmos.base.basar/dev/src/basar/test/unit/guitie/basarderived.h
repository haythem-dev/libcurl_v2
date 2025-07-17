#ifndef  UTDBASPECT_BASARDERIVED_H
#define  UTDBASPECT_BASARDERIVED_H

#include "libbasarqtwidget_datetimeedit.h"
#include "libbasarqtwidget_dateedit.h"
#include "libbasarqtwidget_timeedit.h"

// --------------------------------------------------------------------------
class BasarDateEditDerived : public BasarDateEdit		
{
	Q_OBJECT
public:
	BasarDateEditDerived (const QDate & date , QWidget * parent);
private:
							BasarDateEditDerived	(const BasarDateEditDerived &);
	BasarDateEditDerived &	operator =				(const BasarDateEditDerived &);
};

// --------------------------------------------------------------------------
class BasarTimeEditDerived : public BasarTimeEdit		
{
	Q_OBJECT
public:
	BasarTimeEditDerived (const QTime & time , QWidget * parent);
private:
							BasarTimeEditDerived	(const BasarTimeEditDerived &);
	BasarTimeEditDerived & operator =				(const BasarTimeEditDerived &);
};

// --------------------------------------------------------------------------
class BasarDateTimeEditDerived : public BasarDateTimeEdit	
{
	Q_OBJECT
public:
	BasarDateTimeEditDerived (const QDateTime & datetime , QWidget * parent);
};

// --------------------------------------------------------------------------
#endif
