//----------------------------------------------------------------------------
/*! \file
 *  \brief  public library interface: shared ptr to interface class for email attachment
 *  \author Marco Köppendörfer
 *  \date   02.12.2013
 */
//----------------------------------------------------------------------------

#ifndef GUARD_LIBBASARCMNUTIL_IATTACHMENTPTR_H
#define GUARD_LIBBASARCMNUTIL_IATTACHMENTPTR_H

#include <boost/shared_ptr.hpp>

namespace basar
{
	namespace cmnutil
	{
		class IAttachment;
		
		/*! \brief  typedef to boost::shared_ptr<IAttachment> */
		typedef boost::shared_ptr<IAttachment> IAttachmentPtr;
	}
}



#endif //GUARD_LIBBASARCMNUTIL_IATTACHMENTPTR_H
