//--------------------------------------------------------------------------------------------------//
/*! \file 
 *  \brief      exception indicating missing data for printing
 *  \author     Steffen Heinlein
 *  \date       22.06.2012
 *  \version    00.00.00.01 first version
 */
//--------------------------------------------------------------------------------------------------//

#ifndef GUARD_NARCOTICS_MISSINGDATAFORPRINTINGEXCEPTION_H
#define GUARD_NARCOTICS_MISSINGDATAFORPRINTINGEXCEPTION_H

//--------------------------------------------------------------------------------------------------//
// include section
//--------------------------------------------------------------------------------------------------//
#include <libbasarcmnutil_exceptions.h>

//--------------------------------------------------------------------------------------------------//
// class declaration section
//--------------------------------------------------------------------------------------------------//
namespace infrastructure
{	

	class MissingDataForPrintingException : public basar::Exception
	{
		public:			
			MissingDataForPrintingException( const basar::ExceptInfo& sInfo );
			~MissingDataForPrintingException();

		private:			
			MissingDataForPrintingException();					    			
			MissingDataForPrintingException& operator=( MissingDataForPrintingException& );
	};

} //namespace util

#endif //GUARD_NARCOTICS_MISSINGDATAFORPRINTINGEXCEPTION_H