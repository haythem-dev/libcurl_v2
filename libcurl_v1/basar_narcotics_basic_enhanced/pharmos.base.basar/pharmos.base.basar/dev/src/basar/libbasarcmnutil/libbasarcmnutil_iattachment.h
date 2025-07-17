//----------------------------------------------------------------------------
/*! \file
 *  \brief  public library interface: interface class for email attachment
 *  \author Marco Köppendörfer
 *  \date   02.12.2013
 */
//----------------------------------------------------------------------------

#ifndef GUARD_LIBBASARCMNUTIL_IATTACHMENT_H
#define GUARD_LIBBASARCMNUTIL_IATTACHMENT_H


//----------------------------------------------------------------------------
// forward declaration section
//----------------------------------------------------------------------------
namespace basar
{
	namespace cmnutil
	{
		class BString;
	}
}

//----------------------------------------------------------------------------
// class declaration section
//----------------------------------------------------------------------------
namespace basar
{
	namespace cmnutil
	{
		//! IAttachment: implemented by Attachment, used by MailMessage	\n interface class
		class IAttachment
		{
			public:
				//! \brief virtual(!) d'tor
				virtual ~IAttachment(){};
				
				/*! \brief virtual method for getting base64 encoded data for attachment */
				virtual const BString& getBodyEncoded() const = 0;
				
				/*! \brief virtual method for getting header for encoded data */
				virtual BString getHeader() const = 0;			
		};

	}
}

#endif //GUARD_LIBBASARCMNUTIL_IATTACHMENT_H
