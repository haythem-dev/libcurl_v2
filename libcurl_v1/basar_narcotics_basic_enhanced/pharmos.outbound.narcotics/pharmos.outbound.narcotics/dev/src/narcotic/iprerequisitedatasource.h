#ifndef GUARD_IPREREQUISITEDATASOURCE
#define GUARD_IPREREQUISITEDATASOURCE

//-------------------------------------------------------------------------------------------------//
// includes
//-------------------------------------------------------------------------------------------------//
#include "iprerequisitedatasource.h"

//-------------------------------------------------------------------------------------------------//
// namespace
//-------------------------------------------------------------------------------------------------//
namespace narcotic
{
namespace useCase
{
namespace services
{

class IPrerequisiteDataSource
{
public:
	virtual bool doesContainString( const basar::VarString& searchedString ) const = 0;

	virtual bool doesContainBitmapForUserName( basar::VarString username ) const = 0;

	IPrerequisiteDataSource();
	virtual ~IPrerequisiteDataSource( void );
};


} //namespace services
} //namespace useCase
} //namespace narcotic

#endif //GUARD_IPREREQUISITEDATASOURCE