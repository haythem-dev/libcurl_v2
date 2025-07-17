//---------------------------------------------------------------------------//
/*! \file testfixturebase.h
 *  \brief declaration of fixture base class
 *  \author Dirk Kapusta
 *  \date 18.03.2009
 *  \version 00.00.00.01 first version
 */ //-----------------------------------------------------------------------//

#ifndef GUARD_TESTFIXTUREBASE_H
#define GUARD_TESTFIXTUREBASE_H

#include <vector>
#include <map>

namespace db_aspect_unit_test
{
//! type declarations
typedef std::vector<basar::db::aspect::CachingPolicyEnum> CachingPolicyCollection;
typedef std::map<basar::db::aspect::CachingPolicyEnum, basar::db::aspect::AccessorInstanceRef> AccessorInstanceCollection;
typedef std::map<basar::db::aspect::CachingPolicyEnum, basar::db::aspect::AccessorPropertyTableRef> PropertyTableCollection;
typedef std::map<basar::db::aspect::CachingPolicyEnum, basar::db::aspect::AccessorPropertyTable_YIterator> YIteratorCollection;

//---------------------------------------------------------------------------//
/*! \brief TestFixtureBase base class for test fixtures
		   \n throws no-throw 
 *  \author Dirk Kapusta
 *  \date 24.06.2008
 *  \version 00.00.00.01 first version
 */ //-----------------------------------------------------------------------//
class TestFixtureBase
{
public:
	/*! \brief std constructor \n no-throw */
	TestFixtureBase();
	/*! \brief virtual destructor \n no-throw */
	virtual	~TestFixtureBase();

protected:
	basar::db::aspect::ConnectionRef	m_connRef;
	CachingPolicyCollection				m_cachingPolicies;

private:
	/*! \brief copy constructor \n no-throw */
	TestFixtureBase ( const TestFixtureBase & r );
	/*! \brief assign operator \n no-throw */
	TestFixtureBase operator = ( const TestFixtureBase & r );
};

} // namespace db_aspect_unit_test

#endif	// GUARD_TESTFIXTUREBASE_H
