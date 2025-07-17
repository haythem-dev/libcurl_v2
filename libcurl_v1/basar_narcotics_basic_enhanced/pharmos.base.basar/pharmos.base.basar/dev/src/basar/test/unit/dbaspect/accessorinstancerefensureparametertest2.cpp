/** $Id$                                          
*                                                  
* @file                                            
* @author Anke Klink                      
* @date 2008                            
*/     

#include <UnitTest++/UnitTest++.h>
#include "libbasardbaspect.h"

#include "testconnection.h"
#include "testaccessorholder.h"
#include "libbasarproperty_propertystate.h"

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
SUITE(DBAcpect_AccessorInstanceRef_EnsureParameter2)
{

	TEST_FIXTURE(TestAccessorHolder,DBAcpect_AccessorInstanceRef_EnsureParameter_EnsureParameterAreSetAnd1InState_AND_EnsureParameter1AreSetAnd2InState_YiteratorOnly)
	{ 
		const Int32 number_of_sucessfull_tests_in_one_go = 1 ;  
		int methodtesttimes = 0;
		try
		{
			AccessorMethodEntryList methodnames;
			addMethodsEnsureAreSetAndOneInState(methodnames);
			addMethodsEnsure1AreSetAnd2OneInState(methodnames);

			CHECK(start());

			std::vector<AccessorMethodEntry>::const_iterator methodnameptr;
			for (methodnameptr = methodnames.m_entries.begin(); methodnameptr != methodnames.m_entries.end(); ++methodnameptr)
			{ 
				methodtesttimes++;

				basar::VarString  methodname = methodnameptr->m_methodname; 
				basar::SupportedStateEnum usedSupportedStateEnum  = methodnameptr->m_usedSupportedStateEnum;

				CHECK(startNewAccessorMethodTest(*methodnameptr));
				basar::property::PropertyStateSet ps;

				ExecuteResultInfo executeresultinfo;
				try
				{
					TEXTOUTPUT4( methodtesttimes << std::endl);
					// test y-iterator only :
					{
						ps = m_selectAllEnsureParameters.m_yiter.getState();
						CHECK(basar::SS_CLEAN == ps.getStates());

						int aprsize =  static_cast<int>(m_ensureTest.m_ai.getPropertyTable().size());
						bool colserCanged = false;
						CHECK(startNewTest(*methodnameptr, m_selectAllEnsureParameters, colserCanged));
						ps = m_selectAllEnsureParameters.m_yiter.getState();
						if (colserCanged)
						{
							CHECK(basar::SS_UPDATE == ps.getStates());
							CHECK(basar::SS_UPDATE == m_selectAllEnsureParameters.m_yiter.getState(PROPDEF_COLSER.getName()).getState());
						}
						else
						{
							CHECK(basar::SS_CLEAN == ps.getStates());
						}

						TEXTOUTPUT3( "basar::property::PropertyStateSet = " << ps.toString() << std::endl);

						if (basar::SS_UPDATE == usedSupportedStateEnum && colserCanged)
						{
							TEXTOUTPUT4("methodname = " << methodname << std::end);

							CHECK(basar::SS_UPDATE == ps.getStates());
							CHECK(basar::SS_UPDATE == m_selectAllEnsureParameters.m_yiter.getState(PROPDEF_COLSER.getName()).getState());
							executeresultinfo = m_ensureTest.m_ai.execute(methodname, m_selectAllEnsureParameters.m_yiter);
							//CHECK_EQUAL(basar::db::aspect::SUCCESS, executeresultinfo.getError());
							//CHECK_EQUAL(1, (int)executeresultinfo.getAffectedRows());
						} 
						TEXTOUTPUT4("methodname2 = " << methodname << std::endl);
						executeresultinfo = m_ensureTest.m_ai.execute(methodname, m_selectAllEnsureParameters.m_yiter);
						CHECK_EQUAL(basar::db::aspect::NOT_EXECUTABLE, executeresultinfo.getError());

						m_selectAllEnsureParameters.setPropertyState (PROPDEF_COLINT.getName(),usedSupportedStateEnum); 
						executeresultinfo = m_ensureTest.m_ai.execute(methodname, m_selectAllEnsureParameters.m_yiter);
						CHECK_EQUAL(basar::db::aspect::SUCCESS, executeresultinfo.getError());
						CHECK_EQUAL(1, (int)executeresultinfo.getAffectedRows());

						AccessorPropertyTableRef aptr2 = m_ensureTest.m_ai.getPropertyTable();                       
						CHECK_EQUAL(aprsize, static_cast<int>(aptr2.size()));

						// reset:
						CHECK(checkYIteratorAfterExecutionAndReselectAtDelete( usedSupportedStateEnum,m_selectAllEnsureParameters));

						{
							m_colsernumber--;
							//startNewTest(*methodnameptr,  m_paramListAllEnsureValues, colserCanged);
							CHECK(startNewTest(*methodnameptr,  m_paramListAllEnsureValues, colserCanged));
							executeresultinfo = m_ensureTest.m_ai.execute(methodname);
							CHECK_EQUAL(basar::db::aspect::NOT_EXECUTABLE, executeresultinfo.getError());

							// if no y-iterator is listed no value is in state 
							m_ensureTest.m_ai.setDefaultParameter(m_paramListAllEnsureValues, methodname);
							executeresultinfo = m_ensureTest.m_ai.execute(methodname);
							CHECK_EQUAL(basar::db::aspect::NOT_EXECUTABLE, executeresultinfo.getError());

							// no sucess:

						}

						// reset:
						CHECK(checkYIteratorAfterExecutionAndReselectAtDelete( usedSupportedStateEnum,m_selectAllEnsureParameters));


						TEXTOUTPUT3( "basar::property::PropertyStateSet = " << ps.toString()<< std::endl);

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
				TEXTOUTPUT3("Times = " << methodtesttimes  << "usedSupportedStateEnum = " << basar::property::PropertyState(usedSupportedStateEnum).toString() << " otherSupportedStateEnum = " <<basar::property::PropertyState(otherSupportedStateEnum).toString() );
				CHECK_EQUAL(number_of_sucessfull_tests_in_one_go, m_ensureTest.m_ai.getNumberOfLastInsertedSerials());
				CHECK(endAccessorMethodTest(*methodnameptr));
				ps = m_selectAllEnsureParameters.m_yiter.getState();
				CHECK(basar::SS_CLEAN == ps.getStates());
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


	TEST_FIXTURE(TestAccessorHolder,DBAcpect_AccessorInstanceRef_EnsureParameter_EnsureParameterAreSetAnd1InState_AND_EnsureParameter1AreSetAnd2InState_YIterator_AndDefaultValues_Test1_2)
	{ 
		// 1 Y-It test + 3 * mixed Test with 3 Yiterators
		const Int32 number_of_sucessfull_tests_in_one_go = 2 * getSelectIntVectorSize();
		int methodtesttimes = 0;
		try
		{
			AccessorMethodEntryList methodnames;

			addMethodsEnsureAreSetAndOneInState(methodnames);
			addMethodsEnsure1AreSetAnd2OneInState(methodnames);

			CHECK(start());

			std::vector<AccessorMethodEntry>::const_iterator methodnameptr;
			for (methodnameptr = methodnames.m_entries.begin(); methodnameptr != methodnames.m_entries.end(); ++methodnameptr)
			{ 
				methodtesttimes++;
				basar::VarString  methodname = methodnameptr->m_methodname; 

				basar::SupportedStateEnum usedSupportedStateEnum  = methodnameptr->m_usedSupportedStateEnum;
				basar::SupportedStateEnum otherSupportedStateEnum = methodnameptr->m_otherSupportedStateEnum;

				CHECK(startNewAccessorMethodTest(*methodnameptr));
				basar::property::PropertyStateSet ps;

				ExecuteResultInfo executeresultinfo;
				std::vector<SelectAccessor>::iterator selectIntVectorIter = m_selectIntVector.begin();             
				try
				{
					// test y-iterator + default value (no paramter doubled)
					int i = 0;
					for (selectIntVectorIter = m_selectIntVector.begin(); selectIntVectorIter != m_selectIntVector.end(); ++selectIntVectorIter, ++i)
					{
						bool colserChanged = false;
						CHECK(startNewTest(*methodnameptr,  *selectIntVectorIter,colserChanged ));
						m_ensureTest.m_ai.setDefaultParameter(m_paramListEmpty, methodname);

						int aprsize =  static_cast<int>(m_ensureTest.m_ai.getPropertyTable().size());
						executeresultinfo = m_ensureTest.m_ai.execute(methodname, selectIntVectorIter->m_yiter);
						CHECK_EQUAL(basar::db::aspect::NOT_EXECUTABLE, executeresultinfo.getError());

						// test 1  needs insert + default paramter ( test 1):
						{
							selectIntVectorIter->setPropertyState (PROPDEF_COLINT.getName(),usedSupportedStateEnum);

							if (basar::db::aspect::SUCCESS == m_ensureTest.m_ai.execute(methodname, selectIntVectorIter->m_yiter).getError())
							{
								ps = selectIntVectorIter->m_yiter.getState();

								CHECK(basar::SS_CLEAN == ps.getStates());
								selectIntVectorIter->setPropertyState (PROPDEF_COLINT.getName(),usedSupportedStateEnum);
							}
							executeresultinfo = m_ensureTest.m_ai.execute(methodname, selectIntVectorIter->m_yiter);
							CHECK_EQUAL(basar::db::aspect::NOT_EXECUTABLE, executeresultinfo.getError());

							// + default Parameter:
							m_ensureTest.m_ai.setDefaultParameter(m_paramListNotEnoughValues, methodname);
							executeresultinfo = m_ensureTest.m_ai.execute(methodname, selectIntVectorIter->m_yiter);
							CHECK_EQUAL(basar::db::aspect::NOT_EXECUTABLE, executeresultinfo.getError());

							m_ensureTest.m_ai.setDefaultParameter(m_paramListNonIntValues, methodname);
							executeresultinfo = m_ensureTest.m_ai.execute(methodname, selectIntVectorIter->m_yiter);
							CHECK_EQUAL(basar::db::aspect::SUCCESS, executeresultinfo.getError());
							CHECK_EQUAL(1, (int)executeresultinfo.getAffectedRows());

							AccessorPropertyTableRef aptr2 = m_ensureTest.m_ai.getPropertyTable();
							if (m_idOfEnsureTestInIntVector == i && basar::SS_DELETE == usedSupportedStateEnum)
							{
								CHECK_EQUAL(aprsize-1, static_cast<int>(aptr2.size()));
							}
							else
							{ 
								CHECK_EQUAL(aprsize, static_cast<int>(aptr2.size()));
							}
						}

						CHECK(checkYIteratorAfterExecutionAndReselectAtDelete( usedSupportedStateEnum, *selectIntVectorIter));


						// test 2  needs insert + default paramter ( test 2):
						{ 
							CHECK(startNewTest(*methodnameptr,  *selectIntVectorIter, colserChanged));
							selectIntVectorIter->setPropertyState (PROPDEF_COLINT,otherSupportedStateEnum);
							selectIntVectorIter->setPropertyState (PROPDEF_COLSER,otherSupportedStateEnum);
							executeresultinfo = m_ensureTest.m_ai.execute(methodname, selectIntVectorIter->m_yiter);
							CHECK_EQUAL(basar::db::aspect::NOT_EXECUTABLE, executeresultinfo.getError());

							selectIntVectorIter->setPropertyState (PROPDEF_COLINT.getName(),usedSupportedStateEnum); 
							selectIntVectorIter->setPropertyState (PROPDEF_COLSER.getName(),usedSupportedStateEnum); 
							executeresultinfo = m_ensureTest.m_ai.execute(methodname, selectIntVectorIter->m_yiter);
							CHECK_EQUAL(basar::db::aspect::SUCCESS, executeresultinfo.getError());
							CHECK_EQUAL(1, (int)executeresultinfo.getAffectedRows());
						}
						CHECK(checkYIteratorAfterExecutionAndReselectAtDelete( usedSupportedStateEnum, *selectIntVectorIter));
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
				TEXTOUTPUT3("Times = " << methodtesttimes  << "usedSupportedStateEnum = " << basar::property::PropertyState(usedSupportedStateEnum).toString() << " otherSupportedStateEnum = " <<basar::property::PropertyState(otherSupportedStateEnum).toString() );
				CHECK_EQUAL(number_of_sucessfull_tests_in_one_go, m_ensureTest.m_ai.getNumberOfLastInsertedSerials());
				CHECK(endAccessorMethodTest(*methodnameptr));
				ps = m_selectAllEnsureParameters.m_yiter.getState();
				CHECK(basar::SS_CLEAN == ps.getStates());
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


	TEST_FIXTURE(TestAccessorHolder,DBAcpect_AccessorInstanceRef_EnsureParameter_EnsureParameterAreSetAnd1InState_AND_EnsureParameter1AreSetAnd2InState_test3)
	{ 
		// 1 Y-It test + 3 * mixed Test with 3 Yiterators
		const Int32 number_of_sucessfull_tests_in_one_go = 1 * getSelectIntVectorSize();
		int methodtesttimes = 0;
		try
		{
			AccessorMethodEntryList methodnames;
			addMethodsEnsureAreSetAndOneInState(methodnames);
			addMethodsEnsure1AreSetAnd2OneInState(methodnames);

			CHECK(start());

			std::vector<AccessorMethodEntry>::const_iterator methodnameptr;
			for (methodnameptr = methodnames.m_entries.begin(); methodnameptr != methodnames.m_entries.end(); ++methodnameptr)
			{ 
				methodtesttimes++;
				basar::VarString  methodname = methodnameptr->m_methodname; 

				basar::SupportedStateEnum usedSupportedStateEnum  = methodnameptr->m_usedSupportedStateEnum;
				
				CHECK(startNewAccessorMethodTest(*methodnameptr));
				basar::property::PropertyStateSet ps;

				ExecuteResultInfo executeresultinfo;
				std::vector<SelectAccessor>::iterator selectIntVectorIter = m_selectIntVector.begin();             
				try
				{
				
					// test y-iterator + default value (no paramter doubled)
					for (selectIntVectorIter = m_selectIntVector.begin(); selectIntVectorIter != m_selectIntVector.end(); ++selectIntVectorIter)
					{
						bool colserChanged = false;
						CHECK(startNewTest(*methodnameptr,  *selectIntVectorIter,colserChanged ));
						
						ps = selectIntVectorIter->m_yiter.getState();
						if (colserChanged)
						{
							CHECK(basar::SS_UPDATE == ps.getStates());
							CHECK(basar::SS_UPDATE == selectIntVectorIter->m_yiter.getState(PROPDEF_COLSER.getName()).getState());
							if (basar::SS_UPDATE == usedSupportedStateEnum)
							{
								selectIntVectorIter->m_yiter.setPropertyState(PROPDEF_COLSER.getName(), basar::SS_INSERT);
						    	ps = selectIntVectorIter->m_yiter.getState();
								CHECK(basar::SS_INSERT == ps.getStates());
							}
						}
						else
						{
							CHECK(basar::SS_CLEAN == ps.getStates());
						}

						executeresultinfo = m_ensureTest.m_ai.execute(methodname, selectIntVectorIter->m_yiter);
						CHECK_EQUAL(basar::db::aspect::NOT_EXECUTABLE, executeresultinfo.getError());
	
					
						// if no y-iterator is listed no value is in state 
						m_ensureTest.m_ai.setDefaultParameter(m_paramListAllEnsureValues, methodname);
						executeresultinfo = m_ensureTest.m_ai.execute(methodname);
						CHECK_EQUAL(basar::db::aspect::NOT_EXECUTABLE, executeresultinfo.getError());

						
						executeresultinfo = m_ensureTest.m_ai.execute(methodname, selectIntVectorIter->m_yiter);
						CHECK_EQUAL(basar::db::aspect::NOT_EXECUTABLE, executeresultinfo.getError());

						selectIntVectorIter->setPropertyState (PROPDEF_COLINT.getName(),usedSupportedStateEnum); 
						executeresultinfo = m_ensureTest.m_ai.execute(methodname, selectIntVectorIter->m_yiter);

						CHECK_EQUAL(basar::db::aspect::SUCCESS, executeresultinfo.getError());
						CHECK_EQUAL(1, (int)executeresultinfo.getAffectedRows());

						CHECK(checkYIteratorAfterExecutionAndReselectAtDelete(usedSupportedStateEnum, *selectIntVectorIter));
						}
					//
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
				TEXTOUTPUT3("Times = " << methodtesttimes  << "usedSupportedStateEnum = " << basar::property::PropertyState(usedSupportedStateEnum).toString() << " otherSupportedStateEnum = " <<basar::property::PropertyState(otherSupportedStateEnum).toString() );
				CHECK_EQUAL(number_of_sucessfull_tests_in_one_go, m_ensureTest.m_ai.getNumberOfLastInsertedSerials());
				CHECK(endAccessorMethodTest(*methodnameptr));
				ps = m_selectAllEnsureParameters.m_yiter.getState();
				CHECK(basar::SS_CLEAN == ps.getStates());
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



