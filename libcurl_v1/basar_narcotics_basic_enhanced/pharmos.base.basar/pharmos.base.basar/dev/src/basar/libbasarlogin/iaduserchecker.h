//----------------------------------------------------------------------------
/*! \file	iaduserchecker.h
 *  \brief  
 *  \author	Cognizant
 *  \date   16.04.2014
*/
//----------------------------------------------------------------------------
#ifndef GUARD_IADUSERCHECKER_H
#define GUARD_IADUSERCHECKER_H

// include section
#include <boost/shared_ptr.hpp>
#include "libbasar_definitions.h"
#include "libbasarcmnutil_bstring.h"
#include "libbasarlogin_manager.h"

#include "configurationptr.h"

// class declaration section
namespace basar
{
	namespace login
	{	
		static const basar::VarString HOST_NAME			= "AD_HOST_NAME";
		static const basar::VarString SRCH_PATH			= "AD_SRCH_PATH";
		static const basar::VarString AUTH_USER_NAME	= "AD_USER_NAME";
		static const basar::VarString AUTH_COMMON_NAME	= "AD_COMMON_NAME";
		static const basar::VarString AUTH_PASSWORD		= "AD_PASSWORD";

		class IADUserChecker
		{

			public:
				typedef boost::shared_ptr< IADUserChecker >   IADUserCheckerPtr;	//!< shared pointer to IADUserChecker

				virtual ~IADUserChecker (){}

				virtual bool doLogin( const basar::cmnutil::CollBString& sUserName,const basar::VarString& sPassword ) = 0;

                virtual bool isAccountActive( const basar::VarString& accountName ) = 0;

			protected:
				IADUserChecker (){}	

		};
	} // namespace login	
} // namespace basar
#endif