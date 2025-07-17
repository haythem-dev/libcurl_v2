/** $Id$                                          
*                                                  
* @file                                            
* @author Anke Klink                      
* @date 2008                            
*/     

#include <UnitTest++/UnitTest++.h>
#include "libbasardbaspect.h"

#include "testconnection.h"
#include "testaccessorexecutelistener.h"
#include "testaccessorholder.h"

using namespace basar::db::aspect;

using basar::test::unit::dbaspect::TestConnection;
using basar::test::unit::dbaspect::TestAccessorExecuteListenerHoldLastEvent;
using basar::test::unit::dbaspect::TestAccessorHolder;

using basar::cmnutil::ParameterList;
using basar::property::PropertyDescription;
using basar::db::aspect::AccessorPropertyTable_XIterator;
using basar::SupportedStateEnum;
using basar::VarString;
using basar::Int32;

#define TESTDBACPECT_ACCESSORINSTANCEREF_ENSUREPARAMETERSET
#ifdef  TESTDBACPECT_ACCESSORINSTANCEREF_ENSUREPARAMETERSET

//------------------------------------------------------------------------------

/*!
* Test of:
* - Ensure Macros (with Strings and PropertyDescriptionVector)
* - Y-iterator, only, Default Values and Y-Iterator mixed, only Default Values
* - own Y-iterator (Test: Default Values and Y-Iterator mixed) and 
*   y-iterator from an other Accessor (Test: Y-iterator only and
*   Default Values and Y-Iterator mixed)
*/
SUITE(DBAcpect_AccessorInstanceRef_EnsureParameter)
{	
	TEST_FIXTURE(TestAccessorHolder,DBAcpect_AccessorInstanceRef_EnsureParameter_EnsureParameterAreSet)
	{
		// 1 Y-It test + 1 DefaultParam Test +  1 * mixed Test with 3 Yiterators
		const Int32 number_of_sucessfull_tests_in_one_go = 2 + getSelectIntVectorSize();
		try
		{
			CHECK(start());
			AccessorMethodEntryList methodnames;
			addMethodsEnsureAreSet(methodnames);

			std::vector<AccessorMethodEntry>::const_iterator methodnameptr;
			for (methodnameptr = methodnames.m_entries.begin(); methodnameptr != methodnames.m_entries.end(); ++methodnameptr)
			{ 
				basar::VarString  methodname = methodnameptr->m_methodname;
				CHECK(startNewAccessorMethodTest(*methodnameptr));

				ExecuteResultInfo executeresultinfo;
				std::vector<SelectAccessor>::iterator selectIntVectorIter = m_selectIntVector.begin();

				try
				{
					bool colserChanged = false;

					CHECK(startNewTest(*methodnameptr, m_selectAllEnsureParameters , colserChanged));
					// Y-iterator only
					{
						executeresultinfo = m_ensureTest.m_ai.execute(methodname, m_selectAllEnsureParameters.m_yiter);
						CHECK_EQUAL(basar::db::aspect::SUCCESS, executeresultinfo.getError());
						CHECK_EQUAL(0, (int)executeresultinfo.getAffectedRows());

						AccessorPropertyTableRef aptr2 = m_ensureTest.m_ai.getPropertyTable();
						CHECK_EQUAL(1, static_cast<int>(aptr2.size()));
						aptr2.clear();
					}

					// y-iterator and default values
					for (selectIntVectorIter = m_selectIntVector.begin(); selectIntVectorIter != m_selectIntVector.end(); ++selectIntVectorIter)
					{ 
						CHECK(startNewTest(*methodnameptr, *selectIntVectorIter , colserChanged));
						executeresultinfo = m_ensureTest.m_ai.execute(methodname, selectIntVectorIter->m_yiter);
						CHECK_EQUAL(basar::db::aspect::NOT_EXECUTABLE, executeresultinfo.getError());

						m_ensureTest.m_ai.setDefaultParameter(m_paramListNotEnoughValues, methodname);
						executeresultinfo = m_ensureTest.m_ai.execute(methodname,  selectIntVectorIter->m_yiter);
						CHECK_EQUAL(basar::db::aspect::NOT_EXECUTABLE, executeresultinfo.getError());

						m_ensureTest.m_ai.setDefaultParameter(m_paramListNonIntValues, methodname);

						executeresultinfo = m_ensureTest.m_ai.execute(methodname,  selectIntVectorIter->m_yiter);
						CHECK_EQUAL(basar::db::aspect::SUCCESS, executeresultinfo.getError());
						CHECK_EQUAL(0, (int)executeresultinfo.getAffectedRows());

						AccessorPropertyTableRef aptr2 = m_ensureTest.m_ai.getPropertyTable();
						CHECK_EQUAL(1, static_cast<int>(aptr2.size()));
						aptr2.clear();

						m_ensureTest.m_ai.setDefaultParameter(m_paramListEmpty, methodname);
					}

					// default Values Only
					{
						CHECK(startNewTest(*methodnameptr, m_paramListAllEnsureValues , colserChanged));
						executeresultinfo = m_ensureTest.m_ai.execute(methodname);
						CHECK_EQUAL(basar::db::aspect::NOT_EXECUTABLE, executeresultinfo.getError()); 

						m_ensureTest.m_ai.setDefaultParameter(m_paramListAllEnsureValues, methodname);

						executeresultinfo = m_ensureTest.m_ai.execute(methodname);  
						CHECK_EQUAL(basar::db::aspect::SUCCESS, executeresultinfo.getError());
						CHECK_EQUAL(0, (int)executeresultinfo.getAffectedRows());

						AccessorPropertyTableRef aptr2 = m_ensureTest.m_ai.getPropertyTable();
						CHECK_EQUAL(1, static_cast<int>(aptr2.size()));
						aptr2.clear();
					}
				}
				catch (basar::Exception & ex)
				{
					std::cout << "exception on test with method " 
						<< methodname 
						<< ": " 
						<< ex.what() << std::endl;
					const bool InnerExceptionOccured = false;
					CHECK(InnerExceptionOccured);
					// reset();
					//throw;
				}
				CHECK(endAccessorMethodTest(*methodnameptr));
				CHECK_EQUAL(number_of_sucessfull_tests_in_one_go, m_colsernumber);
				m_colsernumber = 0;
			}
		}
		catch (basar::Exception & ex)
		{
			reset();	
			std::cout << ex.what() << std::endl;
			const bool ExceptionOccured = false;
			CHECK(ExceptionOccured);
			throw;
		}
		reset();
	}


    TEST_FIXTURE(TestAccessorHolder,DBAcpect_AccessorInstanceRef_EnsureParameter_EnsureParameterAreSetAndAllInState)
	{
		// 1 Y-It test + 3 * mixed Test with 3 Yiterators
		const Int32 number_of_sucessfull_tests_in_one_go = 1 + 3 * getSelectIntVectorSize();

		try
		{
			AccessorMethodEntryList methodnames;
			addMethodsEnsureAreSetAndAllInState(methodnames);

			CHECK(start( ));
			std::vector<AccessorMethodEntry>::const_iterator methodnameptr;
			for (methodnameptr = methodnames.m_entries.begin(); methodnameptr != methodnames.m_entries.end(); ++methodnameptr)
			{ 
				basar::VarString  methodname = methodnameptr->m_methodname;
				basar::SupportedStateEnum usedSupportedStateEnum  = methodnameptr->m_usedSupportedStateEnum;
				basar::SupportedStateEnum otherSupportedStateEnum = methodnameptr->m_otherSupportedStateEnum;

				CHECK(startNewAccessorMethodTest(*methodnameptr));

				ExecuteResultInfo executeresultinfo;
				std::vector<SelectAccessor>::iterator selectIntVectorIter = m_selectIntVector.begin();
				try
				{
					bool colserChanged = false;
					CHECK(startNewTest(*methodnameptr, m_selectAllEnsureParameters  , colserChanged));

					// test y-iterator only :
					{
						int aprsize =  static_cast<int>(m_ensureTest.m_ai.getPropertyTable().size());
						executeresultinfo = m_ensureTest.m_ai.execute(methodname, m_selectAllEnsureParameters.m_yiter);
						CHECK_EQUAL(basar::db::aspect::NOT_EXECUTABLE, executeresultinfo.getError());

						m_selectAllEnsureParameters.setPropertyState (PROPDEF_COLINT.getName(),usedSupportedStateEnum); 
						executeresultinfo = m_ensureTest.m_ai.execute(methodname, m_selectAllEnsureParameters.m_yiter);
						CHECK_EQUAL(basar::db::aspect::NOT_EXECUTABLE, executeresultinfo.getError());

						executeresultinfo = setPropertyAndExecute(methodname, m_selectAllEnsureParameters, usedSupportedStateEnum );
						CHECK_EQUAL(basar::db::aspect::SUCCESS, executeresultinfo.getError());
						CHECK_EQUAL(1, (int)executeresultinfo.getAffectedRows());

						AccessorPropertyTableRef aptr2 = m_ensureTest.m_ai.getPropertyTable();
						CHECK_EQUAL(aprsize, static_cast<int>(aptr2.size()));


						// reset for next round:
						{
							m_colsernumber--;
							CHECK(startNewTest(*methodnameptr, m_selectAllEnsureParameters  , colserChanged));
							executeresultinfo = setPropertyAndExecute(methodname, m_selectAllEnsureParameters, otherSupportedStateEnum );
							CHECK_EQUAL(basar::db::aspect::NOT_EXECUTABLE, executeresultinfo.getError());
						}
					}

					// y-iterator and defaultvalues (no doubles)
					for (selectIntVectorIter = m_selectIntVector.begin(); selectIntVectorIter != m_selectIntVector.end(); ++selectIntVectorIter)
					{
						CHECK(startNewTest(*methodnameptr, *selectIntVectorIter , colserChanged));
						int aprsize =  static_cast<int>(m_ensureTest.m_ai.getPropertyTable().size());

						executeresultinfo = m_ensureTest.m_ai.execute(methodname, selectIntVectorIter->m_yiter);
						CHECK_EQUAL(basar::db::aspect::NOT_EXECUTABLE, executeresultinfo.getError());

						// test 1 (state + default paramter)
						{                            
							selectIntVectorIter->setPropertyState (PROPDEF_COLINT.getName(),usedSupportedStateEnum); 
							executeresultinfo = m_ensureTest.m_ai.execute(methodname, selectIntVectorIter->m_yiter);
							CHECK_EQUAL(basar::db::aspect::NOT_EXECUTABLE, executeresultinfo.getError());

							selectIntVectorIter->setPropertyState (PROPDEF_COLSER.getName(),usedSupportedStateEnum); 
							executeresultinfo = m_ensureTest.m_ai.execute(methodname, selectIntVectorIter->m_yiter);
							CHECK_EQUAL(basar::db::aspect::NOT_EXECUTABLE, executeresultinfo.getError());

							selectIntVectorIter->setPropertyState (PROPDEF_COLSMINT.getName(),usedSupportedStateEnum); 
							executeresultinfo = m_ensureTest.m_ai.execute(methodname, selectIntVectorIter->m_yiter);
							CHECK_EQUAL(basar::db::aspect::NOT_EXECUTABLE, executeresultinfo.getError());

							// default param:

							m_ensureTest.m_ai.setDefaultParameter(m_paramListNotEnoughValues, methodname);
							executeresultinfo = m_ensureTest.m_ai.execute(methodname, selectIntVectorIter->m_yiter);
							CHECK_EQUAL(basar::db::aspect::NOT_EXECUTABLE, executeresultinfo.getError());

							m_ensureTest.m_ai.setDefaultParameter(m_paramListNonIntValues, methodname);
							executeresultinfo = m_ensureTest.m_ai.execute(methodname, selectIntVectorIter->m_yiter);
							CHECK_EQUAL(basar::db::aspect::SUCCESS, executeresultinfo.getError());
							CHECK_EQUAL(1, (int)executeresultinfo.getAffectedRows());

							AccessorPropertyTableRef aptr2 = m_ensureTest.m_ai.getPropertyTable();
							if ( m_ensureTest.m_ai.getAccessorName() != selectIntVectorIter->m_ai.getAccessorName() 
								|| basar::SS_DELETE != usedSupportedStateEnum)
							{
								CHECK_EQUAL(aprsize, static_cast<int>(aptr2.size()));
							}
							else
							{
								// delete removes the deleted row on the select acessor:
								CHECK_EQUAL(0, static_cast<int>(aptr2.size()));
							}
						}

						// reset:
						{
							m_colsernumber--;
							CHECK(startNewTest(*methodnameptr, *selectIntVectorIter , colserChanged ));
							executeresultinfo = setPropertyAndExecute(methodname, *selectIntVectorIter, otherSupportedStateEnum );
							CHECK_EQUAL(basar::db::aspect::NOT_EXECUTABLE, executeresultinfo.getError());
						}
						// test 2 (default param + state)
						{
							CHECK(startNewTest(*methodnameptr, *selectIntVectorIter , colserChanged));
							executeresultinfo = m_ensureTest.m_ai.execute(methodname, selectIntVectorIter->m_yiter);
							CHECK_EQUAL(basar::db::aspect::NOT_EXECUTABLE, executeresultinfo.getError());

							selectIntVectorIter->setPropertyState (PROPDEF_COLINT.getName(),usedSupportedStateEnum); 
							executeresultinfo = m_ensureTest.m_ai.execute(methodname, selectIntVectorIter->m_yiter);
							CHECK_EQUAL(basar::db::aspect::NOT_EXECUTABLE, executeresultinfo.getError());

							selectIntVectorIter->setPropertyState (PROPDEF_COLSMINT.getName(),usedSupportedStateEnum); 
							executeresultinfo = m_ensureTest.m_ai.execute(methodname, selectIntVectorIter->m_yiter);
							CHECK_EQUAL(basar::db::aspect::NOT_EXECUTABLE, executeresultinfo.getError());

							selectIntVectorIter->setPropertyState (PROPDEF_COLSER.getName(),usedSupportedStateEnum); 
							executeresultinfo = m_ensureTest.m_ai.execute(methodname, selectIntVectorIter->m_yiter);
							CHECK_EQUAL(basar::db::aspect::SUCCESS, executeresultinfo.getError());
							CHECK_EQUAL(1, (int)executeresultinfo.getAffectedRows());
						}

						// reset:
						{
							m_colsernumber--;
							CHECK(startNewTest(*methodnameptr, *selectIntVectorIter , colserChanged));

							selectIntVectorIter->setPropertyState (PROPDEF_COLINT.getName(),otherSupportedStateEnum);
							executeresultinfo = m_ensureTest.m_ai.execute(methodname, selectIntVectorIter->m_yiter);
							CHECK_EQUAL(basar::db::aspect::NOT_EXECUTABLE, executeresultinfo.getError());
							selectIntVectorIter->setPropertyState (PROPDEF_COLSER.getName(),otherSupportedStateEnum);
							executeresultinfo = m_ensureTest.m_ai.execute(methodname, selectIntVectorIter->m_yiter);
							CHECK_EQUAL(basar::db::aspect::NOT_EXECUTABLE, executeresultinfo.getError());

							executeresultinfo = setPropertyAndExecute(methodname, *selectIntVectorIter, otherSupportedStateEnum );
							CHECK_EQUAL(basar::db::aspect::NOT_EXECUTABLE, executeresultinfo.getError());
						}
						m_ensureTest.m_ai.setDefaultParameter(m_paramListEmpty, methodname);
					}

					// test only default parameter:
					{
						// test without success:
						m_colsernumber--;
						CHECK(startNewTest(*methodnameptr, m_paramListAllEnsureValues , colserChanged));
						executeresultinfo = m_ensureTest.m_ai.execute(methodname);
						CHECK_EQUAL(basar::db::aspect::NOT_EXECUTABLE, executeresultinfo.getError());

						// if no y-iterator is listed no value is in state 
						m_ensureTest.m_ai.setDefaultParameter(m_paramListAllEnsureValues, methodname);
						executeresultinfo = m_ensureTest.m_ai.execute(methodname);
						CHECK_EQUAL(basar::db::aspect::NOT_EXECUTABLE, executeresultinfo.getError());
					}

					// test set on default values and in Y-iterator (doubled values)
					for (selectIntVectorIter = m_selectIntVector.begin(); selectIntVectorIter != m_selectIntVector.end(); ++selectIntVectorIter)
					{
						CHECK(startNewTest(*methodnameptr, *selectIntVectorIter , colserChanged));
						m_colsernumber--;
						CHECK(startNewTest(*methodnameptr, m_paramListAllEnsureValues , colserChanged));

						// if no y-iterator is listed no value is in state 
						m_ensureTest.m_ai.setDefaultParameter(m_paramListAllEnsureValues, methodname);
						executeresultinfo = m_ensureTest.m_ai.execute(methodname);
						CHECK_EQUAL(basar::db::aspect::NOT_EXECUTABLE, executeresultinfo.getError());

						executeresultinfo = m_ensureTest.m_ai.execute(methodname, selectIntVectorIter->m_yiter);
						CHECK_EQUAL(basar::db::aspect::NOT_EXECUTABLE, executeresultinfo.getError());

						selectIntVectorIter->setPropertyState (PROPDEF_COLINT.getName(),usedSupportedStateEnum); 
						executeresultinfo = m_ensureTest.m_ai.execute(methodname, selectIntVectorIter->m_yiter);
						CHECK_EQUAL(basar::db::aspect::NOT_EXECUTABLE, executeresultinfo.getError());

						executeresultinfo = setPropertyAndExecute(methodname, *selectIntVectorIter, usedSupportedStateEnum );
						CHECK_EQUAL(basar::db::aspect::SUCCESS, executeresultinfo.getError());
						CHECK_EQUAL(1, (int)executeresultinfo.getAffectedRows());
						// ready for next round: 
						{      
							// test without success:
							m_colsernumber--;
							CHECK(startNewTest(*methodnameptr, *selectIntVectorIter , colserChanged));
							// ready for next round:
							executeresultinfo = setPropertyAndExecute(methodname, m_selectAllEnsureParameters, otherSupportedStateEnum );
							CHECK_EQUAL(basar::db::aspect::NOT_EXECUTABLE, executeresultinfo.getError());
							m_ensureTest.m_ai.setDefaultParameter(m_paramListEmpty, methodname);
						}

					}
				}
				catch (basar::Exception & ex)
				{

					std::cout << "exception on test with method " 
						<< methodname 
						<< ": " 
						<< ex.what() << std::endl;
					const bool InnerExceptionOccured = false;
					CHECK(InnerExceptionOccured);
					// reset();
					//throw;
				}
				catch (...)
				{
					std::cout << "unknown exception on test with method " 
						<< methodname << std::endl;
					const bool UnknownInnerExceptionOccured = false;
					CHECK(UnknownInnerExceptionOccured);
				} 
				CHECK_EQUAL(number_of_sucessfull_tests_in_one_go, m_ensureTest.m_ai.getNumberOfLastInsertedSerials());
				CHECK_EQUAL(number_of_sucessfull_tests_in_one_go, m_colsernumber);

				CHECK(endAccessorMethodTest(*methodnameptr));
			}
		}
		catch (basar::Exception & ex)
		{
			reset();	
			std::cout << ex.what() << std::endl;
			const bool ExceptionOccured = false;
			CHECK(ExceptionOccured);
			throw;
		}
		reset();
	}




    TEST_FIXTURE(TestAccessorHolder,DBAcpect_AccessorInstanceRef_EnsureParameter_EnsureParameter1AreSetAnd2InState_UnsetAndInStateTest)
	{
		// 2 Y-It test + 6 * mixed Test with 3 Yiterators
		const Int32 number_of_sucessfull_tests_in_one_go = 2 + 6 * getSelectIntVectorSize();

		try
		{
			AccessorMethodEntryList methodnames;
			//Int32 number_of_sucessfull_methods = 
			addMethodsEnsure1AreSetAnd2OneInStateUnsetAndInStateTest(methodnames);
			Int32 methodtesttimes = 0;
			CHECK(start());
			std::vector<AccessorMethodEntry>::const_iterator methodnameptr;
			for (methodnameptr = methodnames.m_entries.begin(); methodnameptr != methodnames.m_entries.end(); ++methodnameptr)
			{ 
				methodtesttimes++;
				basar::VarString  methodname = methodnameptr->m_methodname; 

				basar::SupportedStateEnum usedSupportedStateEnum  = methodnameptr->m_usedSupportedStateEnum;
				basar::SupportedStateEnum otherSupportedStateEnum = methodnameptr->m_otherSupportedStateEnum;

				CHECK(startNewAccessorMethodTest(*methodnameptr));

				ExecuteResultInfo executeresultinfo;
				std::vector<SelectAccessor>::iterator selectIntVectorIter = m_selectIntVector.begin();             
				try
				{
					bool colserChanged = false;
					// test y-iterator only : 
					{
						CHECK(m_selectAllEnsureParameters.select());
						CHECK(startNewTest(*methodnameptr, m_selectAllEnsureParameters , colserChanged));

						executeresultinfo = m_ensureTest.m_ai.execute(methodname, m_selectAllEnsureParameters.m_yiter);
						CHECK_EQUAL(basar::db::aspect::NOT_EXECUTABLE, executeresultinfo.getError());

						m_selectAllEnsureParameters.setPropertyState (PROPDEF_COLINT.getName(),usedSupportedStateEnum); 

						executeresultinfo = m_ensureTest.m_ai.execute(methodname, m_selectAllEnsureParameters.m_yiter);
						CHECK_EQUAL(basar::db::aspect::SUCCESS, executeresultinfo.getError());
						CHECK_EQUAL(1, (int)executeresultinfo.getAffectedRows());

						CHECK(startNewTest(*methodnameptr, m_selectAllEnsureParameters , colserChanged));

						m_selectAllEnsureParameters.setPropertyState (PROPDEF_COLINT.getName(),basar::SS_UNSET); 
						executeresultinfo = m_ensureTest.m_ai.execute(methodname, m_selectAllEnsureParameters.m_yiter);
						CHECK_EQUAL(basar::db::aspect::NOT_EXECUTABLE, executeresultinfo.getError());

						// Unset can't be changed (exception since op 193)
						CHECK_THROW(m_selectAllEnsureParameters.m_yiter.setPropertyState (PROPDEF_COLINT.getName(),usedSupportedStateEnum), basar::Exception);
						// m_selectAllEnsureParameters.setPropertyState (PROPDEF_COLINT.getName(),usedSupportedStateEnum); 

						executeresultinfo = m_ensureTest.m_ai.execute(methodname, m_selectAllEnsureParameters.m_yiter);
						CHECK_EQUAL(basar::db::aspect::NOT_EXECUTABLE, executeresultinfo.getError());

						// not in list 2 => not intersted
						m_selectAllEnsureParameters.setPropertyState (PROPDEF_COLCH1.getName(),usedSupportedStateEnum); 
						executeresultinfo = m_ensureTest.m_ai.execute(methodname, m_selectAllEnsureParameters.m_yiter);
						CHECK_EQUAL(basar::db::aspect::NOT_EXECUTABLE, executeresultinfo.getError());

						// set another (unset of PROPDEF_COLINT is not intersted) :
						m_selectAllEnsureParameters.setPropertyState (PROPDEF_COLSMINT.getName(),usedSupportedStateEnum); 
						executeresultinfo = m_ensureTest.m_ai.execute(methodname, m_selectAllEnsureParameters.m_yiter);
						CHECK_EQUAL(basar::db::aspect::SUCCESS, executeresultinfo.getError());
						CHECK_EQUAL(1, (int)executeresultinfo.getAffectedRows());

						// ready for next round:
						if (basar::SS_DELETE == usedSupportedStateEnum)
						{ 
							CHECK(m_selectAllEnsureParameters.select()); 
						}
						else
						{
							executeresultinfo = setPropertyAndExecute(methodname, m_selectAllEnsureParameters, otherSupportedStateEnum );
							CHECK_EQUAL(basar::db::aspect::NOT_EXECUTABLE, executeresultinfo.getError());
						}
					}

					// test y-iterator + default value 
					for (selectIntVectorIter = m_selectIntVector.begin(); selectIntVectorIter != m_selectIntVector.end(); ++selectIntVectorIter)
					{
						std::vector<ParameterList> m_paramListVector;
						m_paramListVector.push_back(m_paramListNonIntValues);
						m_paramListVector.push_back(m_paramListAllEnsureValues);
						std::vector<ParameterList>::const_iterator paramlistiter;
						for (paramlistiter = m_paramListVector.begin(); paramlistiter != m_paramListVector.end(); ++paramlistiter)
						{

							CHECK(selectIntVectorIter->select());
							CHECK(startNewTest(*methodnameptr,  *selectIntVectorIter , colserChanged));


							m_ensureTest.m_ai.setDefaultParameter(m_paramListNotEnoughValues, methodname);
							executeresultinfo = m_ensureTest.m_ai.execute(methodname, selectIntVectorIter->m_yiter);
							CHECK_EQUAL(basar::db::aspect::NOT_EXECUTABLE, executeresultinfo.getError());

							executeresultinfo = m_ensureTest.m_ai.execute(methodname, selectIntVectorIter->m_yiter);
							CHECK_EQUAL(basar::db::aspect::NOT_EXECUTABLE, executeresultinfo.getError()); 

							// + default Parameter:
							m_ensureTest.m_ai.setDefaultParameter(*paramlistiter, methodname);
							executeresultinfo = m_ensureTest.m_ai.execute(methodname, selectIntVectorIter->m_yiter);
							CHECK_EQUAL(basar::db::aspect::NOT_EXECUTABLE, executeresultinfo.getError());

							selectIntVectorIter->setPropertyState (PROPDEF_COLINT,otherSupportedStateEnum);
							executeresultinfo = m_ensureTest.m_ai.execute(methodname, selectIntVectorIter->m_yiter);
							CHECK_EQUAL(basar::db::aspect::NOT_EXECUTABLE, executeresultinfo.getError());

							selectIntVectorIter->setPropertyState (PROPDEF_COLINT.getName(),usedSupportedStateEnum); 
							executeresultinfo = m_ensureTest.m_ai.execute(methodname, selectIntVectorIter->m_yiter);
							CHECK_EQUAL(basar::db::aspect::SUCCESS, executeresultinfo.getError());
							CHECK_EQUAL(1, (int)executeresultinfo.getAffectedRows());

							CHECK(startNewTest(*methodnameptr,  *selectIntVectorIter , colserChanged));

							// changed to other
							selectIntVectorIter->setPropertyState (PROPDEF_COLINT.getName(),otherSupportedStateEnum); 
							executeresultinfo = m_ensureTest.m_ai.execute(methodname, m_selectAllEnsureParameters.m_yiter);
							CHECK_EQUAL(basar::db::aspect::NOT_EXECUTABLE, executeresultinfo.getError());

							// changed back
							selectIntVectorIter->setPropertyState (PROPDEF_COLINT.getName(),usedSupportedStateEnum); 
							executeresultinfo = m_ensureTest.m_ai.execute(methodname, selectIntVectorIter->m_yiter);
							CHECK_EQUAL(basar::db::aspect::SUCCESS, executeresultinfo.getError());
							CHECK_EQUAL(1, (int)executeresultinfo.getAffectedRows());

							CHECK(startNewTest(*methodnameptr,  *selectIntVectorIter , colserChanged));

							// Unset can't be changed
							selectIntVectorIter->setPropertyState (PROPDEF_COLINT.getName(),basar::SS_UNSET); 
							executeresultinfo = m_ensureTest.m_ai.execute(methodname, selectIntVectorIter->m_yiter);
							CHECK_EQUAL(basar::db::aspect::NOT_EXECUTABLE, executeresultinfo.getError());

							// Unset can't be changed back:
							CHECK_THROW(selectIntVectorIter->m_yiter.setPropertyState(PROPDEF_COLINT.getName(),usedSupportedStateEnum), basar::Exception);
							//selectIntVectorIter->setPropertyState (PROPDEF_COLINT.getName(),usedSupportedStateEnum); 
							executeresultinfo = m_ensureTest.m_ai.execute(methodname, selectIntVectorIter->m_yiter);
							CHECK_EQUAL(basar::db::aspect::NOT_EXECUTABLE, executeresultinfo.getError());

							// set another (unset of PROPDEF_COLINT is not intersted) :
							selectIntVectorIter->setPropertyState (PROPDEF_COLSMINT.getName(),usedSupportedStateEnum); 
							executeresultinfo = m_ensureTest.m_ai.execute(methodname, selectIntVectorIter->m_yiter);
							CHECK_EQUAL(basar::db::aspect::SUCCESS, executeresultinfo.getError());
							CHECK_EQUAL(1, (int)executeresultinfo.getAffectedRows());

						}

						m_ensureTest.m_ai.setDefaultParameter(m_paramListEmpty, methodname);

					}
				}
				catch (basar::Exception & ex)
				{
					std::cout << "exception on test with method " 
						<< methodname 
						<< ": " 
						<< ex.what() << std::endl;
					const bool InnerExceptionOccured = false;
					CHECK(InnerExceptionOccured);
					// reset();
					//throw;
				} 
				catch (...)
				{
					std::cout << "unknown exception on test with method " 
						<< methodname << std::endl;
					const bool UnknownInnerExceptionOccured = false;
					CHECK(UnknownInnerExceptionOccured);
				}

				TEXTOUTPUT4( "Times = " << methodtesttimes  << "usedSupportedStateEnum = " << basar::property::PropertyState(usedSupportedStateEnum).toString() << " otherSupportedStateEnum = " <<basar::property::PropertyState(otherSupportedStateEnum).toString() << std::endl);
				CHECK_EQUAL(number_of_sucessfull_tests_in_one_go, m_ensureTest.m_ai.getNumberOfLastInsertedSerials());
				CHECK_EQUAL(number_of_sucessfull_tests_in_one_go, m_colsernumber);
				CHECK(endAccessorMethodTest(*methodnameptr));
			}
		} 
		catch (basar::Exception & ex)
		{
			reset();	
			std::cout << ex.what() << std::endl;
			const bool ExceptionOccured = false;
			CHECK(ExceptionOccured); 
			throw;
		}
		reset();
	}

	}

// ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~  ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~

#endif
// ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~  ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~



