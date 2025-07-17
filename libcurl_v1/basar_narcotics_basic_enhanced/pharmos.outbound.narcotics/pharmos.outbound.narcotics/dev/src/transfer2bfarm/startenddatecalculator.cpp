//------------------------------------------------------------------------------
//includes
//------------------------------------------------------------------------------
#include "commonheader.h"
#include "startenddatecalculator.h"
#include "exstartdatecalculator.h"
#include "loggerpool.h"

//------------------------------------------------------------------------------
//namespace
//------------------------------------------------------------------------------
namespace narcotics {
namespace datecalc {

//------------------------------------------------------------------------------
DateInfo StartEndDateCalculator::calculateStartEndDate(basar::Date startDate, basar::Int32 weeksSinceStart)
{
	BLOG_TRACE( LoggerPool::getLoggerTransferBfarm(), "StartEndDateCalculator::calculateStartEndDate(basar::Date startDate, basar::Int32 weeksSinceStart)" );   

	if(weeksSinceStart < 1 )
	{
		throw narcotics::exceptions::ExStartDateCalculator( basar::ExceptInfo("StartEndDateCalculator::calculateStartEndDate(basar::Date startDate, basar::Int32 weeksSinceStart)",
				"weeksSinceStart < 0", __FILE__, __LINE__) ); 	
	}

	basar::VarString logMsg;
	logMsg.format("startDate %d, weeksSinceStart %d", startDate.getDate(), weeksSinceStart);
	BLOG_TRACE(LoggerPool::getLoggerTransferBfarm(), logMsg);
	
	startDate.addDays( (weeksSinceStart-1)*7 );

	startDate.addDays(-1 * startDate.getDayOfWeek());

	DateInfo info;
 
	info.setStartDate(startDate.getDate());
	
	startDate.addDays(6);

	info.setEndDate(startDate.getDate());

	return info;
}

//------------------------------------------------------------------------------
StartEndDateCalculator::StartEndDateCalculator()
{
}

//------------------------------------------------------------------------------
StartEndDateCalculator::~StartEndDateCalculator()
{
}

//------------------------------------------------------------------------------

} //namespace datecalc
} //namespace narcotics

//------------------------------------------------------------------------------
