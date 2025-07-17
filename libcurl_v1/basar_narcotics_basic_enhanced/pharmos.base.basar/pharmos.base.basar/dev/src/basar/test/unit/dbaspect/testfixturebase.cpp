//---------------------------------------------------------------------------//
/*! \file testfixturebase.cpp
 *  \brief definition of fixture base class
 *  \author Dirk Kapusta
 *  \date 24.06.2008
 *  \version 00.00.00.01 first version
 */ //-----------------------------------------------------------------------//

#include <UnitTest++/UnitTest++.h>
#include <libbasardbaspect.h>
#include "testfixturebase.h"
#include "testconnection.h"

namespace db_aspect_unit_test
{
TestFixtureBase::TestFixtureBase()
{
	m_connRef = basar::test::unit::dbaspect::TestConnection::createDefaultConnectionRef();

	using namespace basar::db::aspect;
	m_cachingPolicies.push_back( FULL_CACHING );
	m_cachingPolicies.push_back( ON_DEMAND_CACHING );
	m_cachingPolicies.push_back( SINGLE_ROW_CACHING );
	m_cachingPolicies.push_back( NO_CACHING );
}

TestFixtureBase::~TestFixtureBase()
{
	m_connRef.close();
}

} // namespace db_aspect_unit_test
