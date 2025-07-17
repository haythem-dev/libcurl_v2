#include "commonheader.h"
#include "halfyearperiodhelper.h"
#include "libbasarcmnutil_logging.h"
#include "loggerpool.h"

halfyearperiodhelper::HalfYear halfyearperiodhelper::getHalfYearOfDate(const basar::DateTime & date, basar::Date & year)
{
	year = basar::Date(date.getYear(), 1, 1);

	const basar::Int16 month = date.getMonth();

	if (month <= 6)
	{
		return HalfYear::H1;
	}
	else
	{
		return HalfYear::H2;
	}
}

bool halfyearperiodhelper::isDateInCurrentOrPreviousHalfYear(const basar::DateTime & date)
{
	BLOG_TRACE(narcotics::LoggerPool::loggerUseCases, date.toStrDate().c_str());

	const basar::DateTime currentDate;

	BLOG_TRACE(narcotics::LoggerPool::loggerUseCases, currentDate.toStrDate().c_str());
    // date cannot be in the future
    const basar::Int16 dtYear = date.getYear();
    const basar::Int16 dtMonth = date.getMonth();
    const basar::Int16 dtday = date.getDay();

    const basar::Int16 presentYear =currentDate.getYear();
    const basar::Int16 presentMonth = currentDate.getMonth();
    const basar::Int16 presentDay = currentDate.getDay();

    bool sameYear = false;
    bool prevYear = false;

    if (dtYear == presentYear)
        sameYear = true;

    if(dtYear < presentYear )
        prevYear = true;

    if (dtYear > presentYear) //Future year
        return false;
    
    if(sameYear == true)
    {
        if(dtMonth < presentMonth )
        {
        }
        else if(dtMonth > presentMonth)
        {
            return false;
        }
        else if(dtMonth == presentMonth )
        {
            if( dtday <= presentDay)
            {
            }
            else
            {
                return false;
            }
        }
     }


         /*
	// date cannot be in the future
	if (date < currentDate )
	{
		return false;
	}
     */

	basar::Date currentYear;
	const HalfYear currentHalfYear = getHalfYearOfDate(currentDate, currentYear);

	basar::Date dateYear;
	const HalfYear dateHalfYear = getHalfYearOfDate(date, dateYear);

	// both dates are in the same year, the half year does'nt matter
	// or
	// date is in previous year and we are in H1 and date is in H2
	if ((dateYear == currentYear)
		|| (date.getYear() == currentYear.getYear() -1 && currentHalfYear == HalfYear::H1 && dateHalfYear == HalfYear::H2))
	{
		return true;
	}

	return false;
}
