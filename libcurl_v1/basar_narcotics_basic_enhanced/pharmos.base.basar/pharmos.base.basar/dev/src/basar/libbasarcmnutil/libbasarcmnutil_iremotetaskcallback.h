//----------------------------------------------------------------------------
/*! \file
 *  \brief  remote task callback (interface for listener)
 *  \author Marco Köppendörfer
 *  \date   30.09.2013
 */
//----------------------------------------------------------------------------
#ifndef GUARD_IREMOTETASKCALLBACK_H
#define GUARD_IREMOTETASKCALLBACK_H

//--------------------------------------------------------------------------------------------------//
//  forward declaration section
//--------------------------------------------------------------------------------------------------//
namespace basar
{
	namespace cmnutil
	{
		 class BString;
	}
}

//--------------------------------------------------------------------------------------------------//
//  interface declaration section
//--------------------------------------------------------------------------------------------------//
namespace basar
 {
	 namespace cmnutil
	 {
		 class IRemoteTaskCallBack
		 {
			public:
				
				/*! \brief virtual(!) d'tor */
				virtual ~IRemoteTaskCallBack(){};

				/*! \brief notify method for clients to implement */
				virtual void notify(const BString& message) = 0;
		 };
	 }
 }

#endif //GUARD_IREMOTETASKCALLBACK_H
