//---------------------------------------------------------------------------//
/*! \file accessorpropertytablerefbeginfixture.cpp
 *  \brief definition of test fixture for test of begin()
 *  \author Dirk Kapusta
 *  \date 18.03.2009
 *  \version 00.00.00.01 first version
 */ //-----------------------------------------------------------------------//

#include <UnitTest++/UnitTest++.h>
#include <libbasardbaspect.h>
#include <libbasardbaspect_accessor.h>
#include "accessorpropertytablerefbeginfixture.h"
#include "testnames.h"
#include <iostream>

using namespace basar::db::aspect;

namespace db_aspect_unit_test
{
AccessorPropertyTableRefBeginFixture::AccessorPropertyTableRefBeginFixture()
{
	try
	{
		// create accessor instances
		basar::VarString accInstanceName;
		CachingPolicyCollection::const_iterator iter;
		for( iter = m_cachingPolicies.begin(); iter != m_cachingPolicies.end(); ++iter )
		{
			accInstanceName.format( "beginTest%ld", *iter );
			AccessorInstanceRef ai = Manager::getInstance().createAccessorInstance(
				accInstanceName,
				ACCESSORNAME_ButMasterTableReader_All, 
				m_connRef,
				*iter, 
				false );
			m_accessorInstances[ *iter ] = ai;
		}
	}
	catch( basar::Exception & e )
	{
		std::cout << "Exception caught: " << e.what().c_str() << std::endl;
		throw;
	}

	this->prepareDbTables();
}

AccessorPropertyTableRefBeginFixture::~AccessorPropertyTableRefBeginFixture()
{
	this->cleanDbTables();

	try
	{
		// clean up accessor instances
		AccessorInstanceCollection::iterator iter;
		for( iter = m_accessorInstances.begin(); iter != m_accessorInstances.end(); ++iter )
		{
			iter->second.reset();
		}
		m_accessorInstances.clear(); 
	}
	catch( basar::Exception & e )
	{
		std::cout << "Exception caught: " << e.what().c_str() << std::endl;
		//throw;         C4297 function assumed not to throw an exception but does
	}
}

//! \brief cleanup database from test data
void AccessorPropertyTableRefBeginFixture::cleanDbTables()
{
	try
	{
		// clean up property tables
		PropertyTableCollection::iterator iter;
		for( iter = m_propertyTables.begin(); iter != m_propertyTables.end(); ++iter )
		{
			iter->second.clear();
		}
		m_propertyTables.clear();
	}
	catch( basar::Exception & e )
	{
		std::cout << "Exception caught: " << e.what().c_str() << std::endl;
		throw;
	}
}

//! \brief prepare database with test data
void AccessorPropertyTableRefBeginFixture::prepareDbTables()
{
	try
	{
		// fill property tables
		AccessorInstanceCollection::iterator iter;
		for( iter = m_accessorInstances.begin(); iter != m_accessorInstances.end(); ++iter )
		{ 
			AccessorPropertyTableRef tab = iter->second.getPropertyTable();
			iter->second.execute( ACCESSMETHOD_ButMasterTableReader_All );
			m_propertyTables[ iter->first ] = tab;
		}
	}
	catch( basar::Exception & e )
	{
		std::cout << "Exception caught: " << e.what().c_str() << std::endl;
		throw;
	}
}

//! \brief test state count
void AccessorPropertyTableRefBeginFixture::testStateCount(
	basar::db::aspect::AccessorPropertyTableRef table,
	basar::SupportedStateEnum state,
	basar::Int32 countTarget
		)
{
	try
	{
		basar::Int32 count = 0;
		basar::db::aspect::AccessorPropertyTable_YIterator yit = table.begin( state );
		CHECK( PropertyStateSet( state ) == yit.getFilter() );
		while( !yit.isEnd() )
		{
			++count;
			++yit;
		}

		CHECK_EQUAL( countTarget, count );
	}
	catch( basar::Exception & e )
	{
		std::cout << "Exception caught: " << e.what().c_str() << std::endl;
		throw;
	}
}

} // namespace db_aspect_unit_test
