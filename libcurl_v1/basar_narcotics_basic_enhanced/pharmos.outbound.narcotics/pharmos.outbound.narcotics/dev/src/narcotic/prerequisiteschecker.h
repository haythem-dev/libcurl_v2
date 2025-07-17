#ifndef GUARD_PREREQUISITESCHECKER_H
#define GUARD_PREREQUISITESCHECKER_H

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

class PrerequisitesCheckerSV
{
private:
	boost::shared_ptr<IPrerequisiteDataSource> m_DataSource;

public:
	bool areElectronicDeliveryNotePrerequisitesFullfilled( basar::VarString userName, basar::Int16 branchno );

	PrerequisitesCheckerSV( boost::shared_ptr<IPrerequisiteDataSource> dataSource );

	virtual ~PrerequisitesCheckerSV( void );
};


} //namespace services
} //namespace useCase
} //namespace narcotic

#endif //GUARD_PREREQUISITESCHECKER_H