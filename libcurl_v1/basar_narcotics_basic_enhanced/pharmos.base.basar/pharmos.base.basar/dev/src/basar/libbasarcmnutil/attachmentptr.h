//----------------------------------------------------------------------------
/*! \file
 *  \brief  shared ptr to class for email attachment
 *  \author Marco Köppendörfer
 *  \date   02.12.2013
 */
//----------------------------------------------------------------------------

#ifndef GUARD_LIBBASARCMNUTIL_ATTACHMENTPTR_H
#define GUARD_LIBBASARCMNUTIL_ATTACHMENTPTR_H

#include <boost/shared_ptr.hpp>

namespace basar
{
	namespace cmnutil
	{
		class Attachment;
		
		/*! \brief  typedef to boost::shared_ptr<Attachment> */
		typedef boost::shared_ptr<Attachment> AttachmentPtr;
	}
}



#endif //GUARD_LIBBASARCMNUTIL_ATTACHMENTPTR_H
