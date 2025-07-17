//---------------------------------------------------------------------------//
/*! \file accessorpropertytablerefbeginfixture.h
 *  \brief declaration of test fixture for test of begin()
 *  \author Dirk Kapusta
 *  \date 18.03.2009
 *  \version 00.00.00.01 first version
 */ //-----------------------------------------------------------------------//

#ifndef GUARD_ACCESSORPROPERTYTABLEREFBEGINFIXTURE_H
#define GUARD_ACCESSORPROPERTYTABLEREFBEGINFIXTURE_H

#include "testfixturebase.h"

namespace db_aspect_unit_test
{
class AccessorPropertyTableRefBeginFixture : public TestFixtureBase
{
public:
	/*! \brief std constructor \n \throw basar::Exception */
	AccessorPropertyTableRefBeginFixture();
	/*! \brief virtual destructor \n \throw basar::Exception */
	virtual	~AccessorPropertyTableRefBeginFixture();

protected:
	/*! \brief test state count \n \throw basar::Exception */
	void testStateCount(
		basar::db::aspect::AccessorPropertyTableRef table,
		basar::SupportedStateEnum state,
		basar::Int32 countTarget
			);

	AccessorInstanceCollection			m_accessorInstances;
	PropertyTableCollection				m_propertyTables;
	
private:
	/*! \brief copy constructor \n no-throw */
	AccessorPropertyTableRefBeginFixture ( const AccessorPropertyTableRefBeginFixture & r );
	/*! \brief assign operator \n no-throw */
	AccessorPropertyTableRefBeginFixture operator = ( const AccessorPropertyTableRefBeginFixture & r );

	/*! \brief cleanup database from test data \n \throw basar::Exception */
	void cleanDbTables();
	/*! \brief prepare database with test data \n \throw basar::Exception */
	void prepareDbTables();
};

} // end namespace db_aspect_unit_test

#endif // GUARD_ACCESSORPROPERTYTABLEREFBEGINFIXTURE_H
