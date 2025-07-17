//-------------------------------------------------------------------------------------------------//
// includes
//-------------------------------------------------------------------------------------------------//
#include "commonheader.h"
#include "prerequisiteschecker.h"
#include "loggerpool.h"

//-------------------------------------------------------------------------------------------------//
// namespace
//-------------------------------------------------------------------------------------------------//
namespace narcotic
{
namespace useCase
{
namespace services
{

//-------------------------------------------------------------------------------------------------//
bool PrerequisitesCheckerSV::areElectronicDeliveryNotePrerequisitesFullfilled( basar::VarString userName, basar::Int16 branchno )
{
	bool userSignatureAvailable = m_DataSource->doesContainBitmapForUserName( userName );

	if( !userSignatureAvailable )
	{
		if( narcotics::LoggerPool::loggerNarcotics.isEnabledFor(log4cplus::ERROR_LOG_LEVEL) )
		{
			basar::VarString msg;
			msg.format( "narcotics: usersignature for user %s and branch %d not available", userName.c_str(), branchno );

			narcotics::LoggerPool::loggerNarcotics.forcedLog(log4cplus::ERROR_LOG_LEVEL, msg.c_str(), __FILE__, __LINE__);
		}
	}

	return userSignatureAvailable;
}

//-------------------------------------------------------------------------------------------------//
PrerequisitesCheckerSV::PrerequisitesCheckerSV( boost::shared_ptr<IPrerequisiteDataSource> dataSource )
: m_DataSource( dataSource )
{
}

//-------------------------------------------------------------------------------------------------//
PrerequisitesCheckerSV::~PrerequisitesCheckerSV( void )
{
}

} //namespace services
} //namespace useCase
} //namespace narcotic