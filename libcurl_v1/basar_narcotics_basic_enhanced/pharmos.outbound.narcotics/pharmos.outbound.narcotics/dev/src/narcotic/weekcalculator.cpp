//----------------------------------------------------------------------------
/*! \file
 *  \brief  calculate serial week for contribution vouchers
 *  \author Benjamin Endlicher
 *  \date   23.09.2010
 */
//----------------------------------------------------------------------------
//-----------------------------------------------------------------------------------------------//
// includes
//-----------------------------------------------------------------------------------------------//
#include "commonheader.h"
#include "weekcalculator.h"
#include "loggerpool.h"

//-------------------------------------------------------------------------------------------------//
/*!	\throw no-throw */
//-------------------------------------------------------------------------------------------------//
WeekCalculator::WeekCalculator()
{
	BLOG_TRACE_METHOD( narcotics::LoggerPool::loggerUseCases, "WeekCalculator::WeekCalculator()" );
}

//-------------------------------------------------------------------------------------------------//
/*!	\throw no-throw */
//-------------------------------------------------------------------------------------------------//
WeekCalculator::~WeekCalculator()
{
	BLOG_TRACE_METHOD( narcotics::LoggerPool::loggerUseCases, "WeekCalculator::~WeekCalculator()" );
}

//----------------------------------------------------------------------------
//! \return serial week for contribution voucher
//! \throw no-throw
basar::Int32 WeekCalculator::calculateSerialWeek( const basar::VarString& startdatetxt )
{
	BLOG_TRACE_METHOD( narcotics::LoggerPool::loggerUseCases, "WeekCalculator::calculateSerialWeek()" );

	basar::VarString serialWeektxt;
	basar::Int32 weekDifference;

	// start date
	basar::Date startDate( startdatetxt.stoi() );
	basar::Int16 startWeek = startDate.getWeekOfYear();
	// current date
	basar::Date currentDate = basar::cmnutil::Date::getCurrent();
	basar::Int16 currentWeek = currentDate.getWeekOfYear();

	if( currentDate.getYear() == startDate.getYear() && currentWeek >= startWeek )
	{
		weekDifference = 1 + currentWeek - startWeek;
	}
	else
	{
		weekDifference = 1 +																		// increase to next running number
						( getNoOfWeeks( startDate.getYear() ) - startWeek +							// number of weeks in start year
						currentWeek +																// number of weeks in current year
						getNoOfWeeksBetweenYears( startDate.getYear(), currentDate.getYear() ) )	// number of weeks in years between start year - current year
						% 999;
	}

	return weekDifference;
}

//----------------------------------------------------------------------------
//! \return number of weeks for given year
//! \throw no-throw
basar::Int32 WeekCalculator::getNoOfWeeks( basar::Int16 year )
{
	BLOG_TRACE_METHOD( narcotics::LoggerPool::loggerUseCases, "WeekCalculator::getNoOfWeeks()" );

	basar::Int32 noOfWeeks;
	basar::Date firstDayOfYear = basar::Date( year, 01, 01 );
	basar::Date lastDayOfYear = basar::Date( year, 12, 31 );
	if( ( firstDayOfYear.getDayOfWeek() == basar::cmnutil::Date::THURSDAY ) || ( lastDayOfYear.getDayOfWeek() == basar::cmnutil::Date::THURSDAY ) ){
		return ( noOfWeeks = 53 );
	} else {
		return ( noOfWeeks = 52 );
	}
}

//----------------------------------------------------------------------------
//! \return number of weeks between two given years
//! \throw no-throw
basar::Int32 WeekCalculator::getNoOfWeeksBetweenYears( basar::Int16 startYear, basar::Int16 endYear )
{
	BLOG_TRACE_METHOD( narcotics::LoggerPool::loggerUseCases, "WeekCalculator::getNoOfWeeksBetweenYears()" );

	// week 1 is already in Dec
	if (1 == basar::cmnutil::Date::getCurrent().getWeekOfYear() && 12 == basar::cmnutil::Date::getCurrent().getMonth())
		endYear++;

	// week 52/53 is still in Jan.
	if (52 <= basar::cmnutil::Date::getCurrent().getWeekOfYear() && 1 == basar::cmnutil::Date::getCurrent().getMonth())
		endYear--;

	basar::Int32 numberOfWeeksBetween = 0;
	basar::Int16 currentYearBetween = startYear + 1;
	while( currentYearBetween != endYear )
	{
		numberOfWeeksBetween += getNoOfWeeks( currentYearBetween );
		currentYearBetween++;
	}

	return numberOfWeeksBetween;
}