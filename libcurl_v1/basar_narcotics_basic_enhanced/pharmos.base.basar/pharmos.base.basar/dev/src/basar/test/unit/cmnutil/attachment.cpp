#include <UnitTest++/UnitTest++.h>
#include "libbasarcmnutil.h"


//------------------------------------------------------------------------------
SUITE(EmailAttachment)
{
	TEST(InvalidFileName)
	{
		using basar::ConstString;
		using basar::cmnutil::Attachment;
		
		ConstString INVALID_FILE = "nixda.dat";
		
		CHECK_THROW(Attachment::createFromFile(INVALID_FILE), basar::cmnutil::BasarFileNotAccessibleException);
	}
}