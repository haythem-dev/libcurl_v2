//----------------------------------------------------------------------------
/*! \file
 *  \brief  Qt datetime widgets
 *  \author Michael Eichenlaub
 *  \date   19.08.2009
 */
//----------------------------------------------------------------------------

#include <UnitTest++/UnitTest++.h>

#include "libbasarcmnutil.h"
#include "libbasarqtwidget.h"

#include <QtCore/QDateTime>
#include <QtWidgets/QDateTimeEdit>

// QDate was changed between 5.10 and 5.11
#if (QT_VERSION > QT_VERSION_CHECK(5, 10, 0))
#   define YEAR_MAX "31.12.9999"
#else
#   define YEAR_MAX "31.12.7999"
#endif

//------------------------------------------------------------------------------
using basar::ConstString;
using basar::cmnutil::Locale;
using basar::gui::widget::I18n;

//------------------------------------------------------------------------------
SUITE(TestDateTime)
{

//------------------------------------------------------------------------------
class Fixture
{
public:
	Fixture	() : 	fx_d (2008, 12, 23),
					fx_t (  23, 59, 23, 987),
                    fx_dt(QDate(2009, 10, 19),	QTime( 21, 3, 28, 123)),
					fx_d_max (9999, 12, 31),
					fx_t_max (  23, 59, 59, 999),
                    fx_dt_max(QDate(9999, 12, 31),	QTime( 23, 59, 59, 999))
	{
	}

	QDate		fx_d;
	QTime		fx_t;
	QDateTime	fx_dt;

	QDate		fx_d_max;
	QTime		fx_t_max;
	QDateTime	fx_dt_max;
};

//------------------------------------------------------------------------------
TEST_FIXTURE(Fixture, QtWidget)
{
	I18n::setI18n(Locale::GERMAN_DE);

    QDateEdit     d_ed;
    QTimeEdit     t_ed;
	QDateTimeEdit dt_ed;

	d_ed .setDate    (fx_d );
	t_ed .setTime    (fx_t );
	dt_ed.setDateTime(fx_dt);

	CHECK_EQUAL("23.12.08"      , d_ed .text().toLocal8Bit().constData());
	CHECK_EQUAL("23:59"         , t_ed .text().toLocal8Bit().constData());
	CHECK_EQUAL("19.10.09 21:03", dt_ed.text().toLocal8Bit().constData()); 
}

//------------------------------------------------------------------------------
TEST_FIXTURE(Fixture, QtWidget_max)
{
	I18n::setI18n(Locale::GERMAN_DE);

    QDateEdit     d_ed;
    QTimeEdit     t_ed;
	QDateTimeEdit dt_ed;

	CHECK_EQUAL("14.09.52"      , d_ed .minimumDate    ().toString(Qt::DefaultLocaleShortDate).toLocal8Bit().constData());
	CHECK_EQUAL("31.12.99"      , d_ed .maximumDate    ().toString(Qt::DefaultLocaleShortDate).toLocal8Bit().constData());
	CHECK_EQUAL("00:00"         , t_ed .minimumTime    ().toString(Qt::DefaultLocaleShortDate).toLocal8Bit().constData());
	CHECK_EQUAL("23:59"         , t_ed .maximumTime    ().toString(Qt::DefaultLocaleShortDate).toLocal8Bit().constData());
	CHECK_EQUAL("14.09.52 00:00", dt_ed.minimumDateTime().toString(Qt::DefaultLocaleShortDate).toLocal8Bit().constData());
	CHECK_EQUAL("31.12.99 23:59", dt_ed.maximumDateTime().toString(Qt::DefaultLocaleShortDate).toLocal8Bit().constData());

	d_ed .setDate    (fx_d_max );
	t_ed .setTime    (fx_t_max );
	dt_ed.setDateTime(fx_dt_max);

	CHECK_EQUAL("31.12.99"      , d_ed .text().toLocal8Bit().constData());
	CHECK_EQUAL("23:59"         , t_ed .text().toLocal8Bit().constData());
	CHECK_EQUAL("31.12.99 23:59", dt_ed.text().toLocal8Bit().constData()); 
}

//------------------------------------------------------------------------------
TEST_FIXTURE(Fixture, QtWidget_max_displayformat)
{
	I18n::setI18n(Locale::GERMAN_DE);

    QDateEdit     d_ed;
    QTimeEdit     t_ed;
	QDateTimeEdit dt_ed;

	ConstString d_fmt  = basar::gui::widget::I18n::getPhDateFormat    ();
	ConstString t_fmt  = basar::gui::widget::I18n::getPhTimeFormat    ();
	ConstString dt_fmt = basar::gui::widget::I18n::getPhDateTimeFormat();

	d_ed .setDisplayFormat(d_fmt );
	t_ed .setDisplayFormat(t_fmt );
	dt_ed.setDisplayFormat(dt_fmt);

	CHECK_EQUAL("14.09.1752"         , d_ed .minimumDate    ().toString(d_fmt).toLocal8Bit().constData());
	CHECK_EQUAL(YEAR_MAX             , d_ed .maximumDate    ().toString(d_fmt).toLocal8Bit().constData());
	CHECK_EQUAL("00:00:00"           , t_ed .minimumTime    ().toString(t_fmt).toLocal8Bit().constData());
	CHECK_EQUAL("23:59:59"           , t_ed .maximumTime    ().toString(t_fmt).toLocal8Bit().constData());
	CHECK_EQUAL("14.09.1752 00:00:00", dt_ed.minimumDateTime().toString(dt_fmt).toLocal8Bit().constData());
	CHECK_EQUAL(YEAR_MAX " 23:59:59" , dt_ed.maximumDateTime().toString(dt_fmt).toLocal8Bit().constData());

	d_ed .setDate    (fx_d_max );
	t_ed .setTime    (fx_t_max );
	dt_ed.setDateTime(fx_dt_max);

	CHECK_EQUAL(YEAR_MAX            , d_ed .text().toLocal8Bit().constData());
	CHECK_EQUAL("23:59:59"          , t_ed .text().toLocal8Bit().constData());
	CHECK_EQUAL(YEAR_MAX " 23:59:59", dt_ed.text().toLocal8Bit().constData()); 
}

//------------------------------------------------------------------------------
TEST_FIXTURE(Fixture, basarWidget)
{
	I18n::setI18n(Locale::GERMAN_DE);

    BasarDateEdit     d_ed;
    BasarTimeEdit     t_ed;
	BasarDateTimeEdit dt_ed;

	d_ed .setDate    (fx_d );
	t_ed .setTime    (fx_t );
	dt_ed.setDateTime(fx_dt);

	CHECK_EQUAL("23.12.2008"         , d_ed .text().toLocal8Bit().constData());
	CHECK_EQUAL("23:59:23"           , t_ed .text().toLocal8Bit().constData());
	CHECK_EQUAL("19.10.2009 21:03:28", dt_ed.text().toLocal8Bit().constData()); 
}

//------------------------------------------------------------------------------
TEST_FIXTURE(Fixture, basarWidget_max)
{
	I18n::setI18n(Locale::GERMAN_DE);

    BasarDateEdit     d_ed;
    BasarTimeEdit     t_ed;
	BasarDateTimeEdit dt_ed;

	ConstString d_fmt  = basar::gui::widget::I18n::getPhDateFormat    ();
	ConstString t_fmt  = basar::gui::widget::I18n::getPhTimeFormat    ();
	ConstString dt_fmt = basar::gui::widget::I18n::getPhDateTimeFormat();

	CHECK_EQUAL("14.09.1752"         , d_ed .minimumDate    ().toString(d_fmt).toLocal8Bit().constData());
	CHECK_EQUAL(YEAR_MAX             , d_ed .maximumDate    ().toString(d_fmt).toLocal8Bit().constData());
	CHECK_EQUAL("00:00:00"           , t_ed .minimumTime    ().toString(t_fmt).toLocal8Bit().constData());
	CHECK_EQUAL("23:59:59"           , t_ed .maximumTime    ().toString(t_fmt).toLocal8Bit().constData());
	CHECK_EQUAL("14.09.1752 00:00:00", dt_ed.minimumDateTime().toString(dt_fmt).toLocal8Bit().constData());
	CHECK_EQUAL(YEAR_MAX " 23:59:59" , dt_ed.maximumDateTime().toString(dt_fmt).toLocal8Bit().constData());

	d_ed .setDate    (fx_d_max );
	t_ed .setTime    (fx_t_max );
	dt_ed.setDateTime(fx_dt_max);

	CHECK_EQUAL(YEAR_MAX            , d_ed .text().toLocal8Bit().constData());
	CHECK_EQUAL("23:59:59"          , t_ed .text().toLocal8Bit().constData());
	CHECK_EQUAL(YEAR_MAX " 23:59:59", dt_ed.text().toLocal8Bit().constData()); 
}

//------------------------------------------------------------------------------
}	// SUITE
