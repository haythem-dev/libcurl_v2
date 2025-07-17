#ifndef FTPEXCEPTION_H_GUARD
#define FTPEXCEPTION_H_GUARD
//-------------------------------------------------------------------------------------------------------------------------//
// include section
//-------------------------------------------------------------------------------------------------------------------------//
#include "commonheader.h"

//-------------------------------------------------------------------------------------------------------------------------//
// namespace section
//-------------------------------------------------------------------------------------------------------------------------//
namespace narcotics
{
namespace exception
{

//----------------------------------------------------------------------------
class FTPException : public basar::cmnutil::BasarRuntimeException
{
public:
	FTPException( const basar::cmnutil::ExceptionInfoStruct& sInfo );
	virtual ~FTPException( void );
};

} //namespace exception
} //namespace narcotics

#endif //FTPEXCEPTION_H_GUARD