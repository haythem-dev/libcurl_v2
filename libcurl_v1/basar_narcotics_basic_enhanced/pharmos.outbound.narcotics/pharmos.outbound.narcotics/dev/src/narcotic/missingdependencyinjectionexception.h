//--------------------------------------------------------------------------------------------------//
/*! \file 
 *  \brief      exception indicating missing dependency injection
 *  \author     Steffen Heinlein
 *  \date       22.06.2012
 *  \version    00.00.00.01 first version
 */
//--------------------------------------------------------------------------------------------------//

#ifndef GUARD_NARCOTICS_MISSINGDEPENDENCYINJECTIONEXCEPTION_H
#define GUARD_NARCOTICS_MISSINGDEPENDENCYINJECTIONEXCEPTION_H

//--------------------------------------------------------------------------------------------------//
// include section
//--------------------------------------------------------------------------------------------------//
#include <libbasarcmnutil_exceptions.h>

//--------------------------------------------------------------------------------------------------//
// class declaration section
//--------------------------------------------------------------------------------------------------//
namespace infrastructure
{	
	class MissingDependencyInjectionException : public basar::Exception
	{
		public:			
			MissingDependencyInjectionException( const basar::ExceptInfo& sInfo );
			~MissingDependencyInjectionException();

		private:			
			MissingDependencyInjectionException();					    			
			MissingDependencyInjectionException& operator=( MissingDependencyInjectionException& );
	};

} //namespace infrastructure

#endif //GUARD_NARCOTICS_MISSINGDEPENDENCYINJECTIONEXCEPTION_H