//------------------------------------------------------------------------------
//includes
//------------------------------------------------------------------------------
#include "commonheader.h"
#include "dateinfo.h"
#include "loggerpool.h"

//------------------------------------------------------------------------------
//namespaces
//------------------------------------------------------------------------------
namespace narcotics
{
namespace datecalc
{

//------------------------------------------------------------------------------
void DateInfo::addDaysToStartDate(basar::Int32 nDays)
{
	BLOG_TRACE( LoggerPool::getLoggerTransferBfarm(), "DateInfo::addDaysToStartDate(basar::Int32 nDays))" );

	basar::Date date(m_startDate);

	date.addDays(nDays);

	m_startDate = date.getDate();
}

//------------------------------------------------------------------------------
void DateInfo::setStartDate(basar::Int32 startDate)
{
	BLOG_TRACE( LoggerPool::getLoggerTransferBfarm(), "DateInfo::setStartDate(basar::Int32 startDate)" );
	m_startDate = startDate;
}

//------------------------------------------------------------------------------
basar::Int32 DateInfo::getStartDate()
{
	BLOG_TRACE( LoggerPool::getLoggerTransferBfarm(), "DateInfo::getStartDate()" );
	return m_startDate;
}

//------------------------------------------------------------------------------
void DateInfo::setEndDate(basar::Int32 endDate)
{
	BLOG_TRACE( LoggerPool::getLoggerTransferBfarm(), "DateInfo::setEndDate(basar::Int32 endDate)" );

	m_endDate = endDate;
}

//------------------------------------------------------------------------------
basar::Int32 DateInfo::getEndDate()
{
	return m_endDate;
}

//------------------------------------------------------------------------------
DateInfo::DateInfo(void)
{
}

//------------------------------------------------------------------------------
DateInfo::~DateInfo(void)
{
}

} //namespace datecalc
} //namespace narcotics