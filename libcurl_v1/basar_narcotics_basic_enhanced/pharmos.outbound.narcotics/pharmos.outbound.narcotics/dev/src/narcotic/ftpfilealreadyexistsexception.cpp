//-------------------------------------------------------------------------------------------------------------------------//
// include section
//-------------------------------------------------------------------------------------------------------------------------//
#include "commonheader.h"
#include "ftpfilealreadyexistsexception.h"

//-------------------------------------------------------------------------------------------------------------------------//
// namespace section
//-------------------------------------------------------------------------------------------------------------------------//
namespace narcotics
{
namespace exception
{

//----------------------------------------------------------------------------
FTPFileAlreadyExistsException::FTPFileAlreadyExistsException( const basar::cmnutil::ExceptionInfoStruct& sInfo ) 
	: FTPException( sInfo )
{
}

//----------------------------------------------------------------------------
FTPFileAlreadyExistsException::~FTPFileAlreadyExistsException()
{
}

} //namespace exception
} //namespace narcotics