#ifndef  UTDBASPECT_QTDERIVED_H
#define  UTDBASPECT_QTDERIVED_H

#pragma warning (push)
#pragma warning(disable: 4127 4481)
#include <QtWidgets/QDateEdit>
#include <QtWidgets/QDateTimeEdit>
#include <QtWidgets/QTimeEdit>
#pragma warning (pop)

// --------------------------------------------------------------------------
class DateEditDerived : public QDateEdit		
{
	Q_OBJECT
public:
	DateEditDerived (const QDate & date , QWidget * parent);
private:
						DateEditDerived	(const DateEditDerived &);
	DateEditDerived & operator =		(const DateEditDerived &);
};

// --------------------------------------------------------------------------
class TimeEditDerived : public QTimeEdit		
{
	Q_OBJECT
public:
	TimeEditDerived (const QTime & time , QWidget * parent);
private:
						TimeEditDerived	(const TimeEditDerived &);
	TimeEditDerived & operator =		(const TimeEditDerived &);
};

// --------------------------------------------------------------------------
class DateTimeEditDerived : public QDateTimeEdit	
{
	Q_OBJECT
public:
	DateTimeEditDerived (const QDateTime & datetime , QWidget * parent);
};

// --------------------------------------------------------------------------
#endif
