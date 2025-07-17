#ifndef FTPFILEALREADYEXISTSEXCEPTION_H_GUARD
#define FTPFILEALREADYEXISTSEXCEPTION_H_GUARD

//-------------------------------------------------------------------------------------------------------------------------//
// include section
//-------------------------------------------------------------------------------------------------------------------------//
#include "commonheader.h"
#include "ftpexception.h"

//-------------------------------------------------------------------------------------------------------------------------//
// namespace section
//-------------------------------------------------------------------------------------------------------------------------//
namespace narcotics
{
namespace exception
{

//----------------------------------------------------------------------------
class FTPFileAlreadyExistsException:public FTPException
{
public:
	FTPFileAlreadyExistsException( const basar::cmnutil::ExceptionInfoStruct& sInfo );
	virtual ~FTPFileAlreadyExistsException();
};

} //namespace exception
} //namespace narcotics

#endif //FTPFILEALREADYEXISTSEXCEPTION_H_GUARD