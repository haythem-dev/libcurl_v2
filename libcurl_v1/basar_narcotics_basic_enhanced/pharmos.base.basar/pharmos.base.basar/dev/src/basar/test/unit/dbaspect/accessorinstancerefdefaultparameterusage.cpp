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
#include "testnames.h"
#include "buttablepropertydescriptions.h"

using namespace basar::db::aspect;

using basar::test::unit::dbaspect::TestConnection;
using basar::test::unit::dbaspect::TestAccessorExecuteListenerHoldLastEvent;
using basar::cmnutil::ParameterList;
using basar::VarString;

#define TESTDBACPECT_ACCESSORINSTANCEREF_DEFAULTPARAMETERUSAGE
#ifdef  TESTDBACPECT_ACCESSORINSTANCEREF_DEFAULTPARAMETERUSAGE

//------------------------------------------------------------------------------
SUITE(DBAcpect_AccessorInstanceRef_DefaultParameterUsage)
{
	TEST(DBAcpect_AccessorInstanceRef_DefaultParameterUsage_DefaultParameterUsage)
	{
		ConnectionRef connRef = TestConnection::createDefaultConnectionRef();
		try
		{
			AccessorInstanceRef ai1 = Manager::getInstance().createAccessorInstance(
				"test1",
				ACCESSORNAME_ButMasterTableReader_Int, 
				connRef,
				basar::db::aspect::FULL_CACHING);

			AccessorInstanceRef ai2 = Manager::getInstance().createAccessorInstance(
				"test2",
				ACCESSORNAME_TestSqlBuilder_IntAndOneString_SelectWhere, 
				connRef,
				basar::db::aspect::FULL_CACHING);

			{ 
				AccessorPropertyTableRef  aptr1;
				AccessorPropertyTableRef  aptr2;

				ExecuteResultInfo executeresultinfo = 
					ai1.execute(ACCESSMETHOD_ButMasterTableReader_Int);
				CHECK_EQUAL(basar::db::aspect::SUCCESS, executeresultinfo.getError());
				CHECK_EQUAL(0, (int)executeresultinfo.getAffectedRows());

				aptr1 = ai1.getPropertyTable();
				AccessorPropertyTable_YIterator iter1 = aptr1.begin();

				executeresultinfo = ai2.execute(ACCESSMETHOD_TestSqlBuilder_IntAndOneString_SelectWhere_2, iter1);
				CHECK_EQUAL(basar::db::aspect::NOT_EXECUTABLE, executeresultinfo.getError());

				executeresultinfo = ai2.execute(ACCESSMETHOD_TestSqlBuilder_IntAndOneString_SelectWhere_3, iter1);
				CHECK_EQUAL(basar::db::aspect::NOT_EXECUTABLE, executeresultinfo.getError());

				executeresultinfo = ai2.execute(ACCESSMETHOD_TestSqlBuilder_IntAndOneString_SelectWhere, iter1);
				CHECK_EQUAL(basar::db::aspect::NOT_EXECUTABLE, executeresultinfo.getError());

				ParameterList list;
				list.push_back(PROPDEF_COLCH2.getName(),"COLCH21");
				ai2.setDefaultParameter(list, ACCESSMETHOD_TestSqlBuilder_IntAndOneString_SelectWhere);
				ai2.setDefaultParameter(list, ACCESSMETHOD_TestSqlBuilder_IntAndOneString_SelectWhere_2);
				ai2.setDefaultParameter(list, ACCESSMETHOD_TestSqlBuilder_IntAndOneString_SelectWhere_3);

				executeresultinfo = ai2.execute(ACCESSMETHOD_TestSqlBuilder_IntAndOneString_SelectWhere, iter1);
				CHECK_EQUAL(basar::db::aspect::SUCCESS, executeresultinfo.getError());
				CHECK_EQUAL(0, (int)executeresultinfo.getAffectedRows());

				aptr2 = ai2.getPropertyTable();
				CHECK_EQUAL(1, static_cast<int>(aptr2.size()));
				aptr2.clear();

				ai2.setDefaultParameter(list, ACCESSMETHOD_TestSqlBuilder_IntAndOneString_SelectWhere_2);
				executeresultinfo = ai2.execute(ACCESSMETHOD_TestSqlBuilder_IntAndOneString_SelectWhere_2, iter1);
				CHECK_EQUAL(basar::db::aspect::SUCCESS, executeresultinfo.getError());
				CHECK_EQUAL(0, (int)executeresultinfo.getAffectedRows());

				aptr2 = ai2.getPropertyTable();
				CHECK_EQUAL(1, static_cast<int>(aptr2.size()));
				aptr2.clear();

				ai2.setDefaultParameter(list, ACCESSMETHOD_TestSqlBuilder_IntAndOneString_SelectWhere_3);
				executeresultinfo = ai2.execute(ACCESSMETHOD_TestSqlBuilder_IntAndOneString_SelectWhere_3, iter1);
				CHECK_EQUAL(basar::db::aspect::SUCCESS, executeresultinfo.getError());
				CHECK_EQUAL(0, (int)executeresultinfo.getAffectedRows());

				aptr2 = ai2.getPropertyTable();
				CHECK_EQUAL(1, static_cast<int>(aptr2.size()));
				aptr2.clear();
				aptr1.clear();
			}
		}
		catch (basar::Exception & ex)
		{
			connRef.close();	
			std::cout << ex.what() << std::endl;
			const bool ExceptionOccured = false;
			CHECK(ExceptionOccured);
			throw;
		}
		connRef.close();
	}

	TEST(DBAcpect_AccessorInstanceRef_DefaultParameterUsage_DefaultParameterUsage2)
	{
		ConnectionRef connRef = TestConnection::createDefaultConnectionRef();
		try
		{
			AccessorInstanceRef ai1 = Manager::getInstance().createAccessorInstance(
				"test1",
				ACCESSORNAME_TestSqlBuilder_EnsureTest, 
				connRef,
				basar::db::aspect::FULL_CACHING);

			AccessorInstanceRef ai2 = Manager::getInstance().createAccessorInstance(
				"test2",
				ACCESSORNAME_TestSqlBuilder_EnsureTest, 
				connRef,
				basar::db::aspect::FULL_CACHING);

			{ 
				AccessorPropertyTableRef  aptr1;
				AccessorPropertyTableRef  aptr2;
				ParameterList list;

				ExecuteResultInfo executeresultinfo = 
					ai1.execute(ACCESSMETHOD_TestSqlBuilder_IntAndTwoStrings_SelectIntsOnly);
				CHECK_EQUAL(basar::db::aspect::SUCCESS, executeresultinfo.getError());
				CHECK_EQUAL(0, (int)executeresultinfo.getAffectedRows());

				aptr1 = ai1.getPropertyTable();
				AccessorPropertyTable_YIterator iter1 = aptr1.begin();

				executeresultinfo = ai2.execute(ACCESSMETHOD_TestSqlBuilder_IntAndTwoStrings_SelectWhere_2, iter1);
				CHECK_EQUAL(basar::db::aspect::NOT_EXECUTABLE, executeresultinfo.getError());

				executeresultinfo = ai2.execute(ACCESSMETHOD_TestSqlBuilder_IntAndTwoStrings_SelectWhere_3, iter1);
				CHECK_EQUAL(basar::db::aspect::NOT_EXECUTABLE, executeresultinfo.getError());

				executeresultinfo = ai2.execute(ACCESSMETHOD_TestSqlBuilder_IntAndTwoStrings_SelectWhere, iter1);
				CHECK_EQUAL(basar::db::aspect::NOT_EXECUTABLE, executeresultinfo.getError());


				list.push_back(PROPDEF_COLCH2.getName(),"COLCH21");
				ai2.setDefaultParameter(list, ACCESSMETHOD_TestSqlBuilder_IntAndTwoStrings_SelectWhere);
				ai2.setDefaultParameter(list, ACCESSMETHOD_TestSqlBuilder_IntAndTwoStrings_SelectWhere_2);
				ai2.setDefaultParameter(list, ACCESSMETHOD_TestSqlBuilder_IntAndTwoStrings_SelectWhere_3);

				executeresultinfo = ai2.execute(ACCESSMETHOD_TestSqlBuilder_IntAndTwoStrings_SelectWhere_2, iter1);
				CHECK_EQUAL(basar::db::aspect::NOT_EXECUTABLE, executeresultinfo.getError());

				executeresultinfo = ai2.execute(ACCESSMETHOD_TestSqlBuilder_IntAndTwoStrings_SelectWhere_3, iter1);
				CHECK_EQUAL(basar::db::aspect::NOT_EXECUTABLE, executeresultinfo.getError());

				executeresultinfo = ai2.execute(ACCESSMETHOD_TestSqlBuilder_IntAndTwoStrings_SelectWhere, iter1);
				CHECK_EQUAL(basar::db::aspect::NOT_EXECUTABLE, executeresultinfo.getError());

				list.push_back(PROPDEF_COLCH1.getName(),"1");

				ai2.setDefaultParameter(list, ACCESSMETHOD_TestSqlBuilder_IntAndTwoStrings_SelectWhere_2);
				ai2.setDefaultParameter(list, ACCESSMETHOD_TestSqlBuilder_IntAndTwoStrings_SelectWhere_2);
				executeresultinfo = ai2.execute(ACCESSMETHOD_TestSqlBuilder_IntAndTwoStrings_SelectWhere_2, iter1);
				CHECK_EQUAL(basar::db::aspect::SUCCESS, executeresultinfo.getError());
				CHECK_EQUAL(0, (int)executeresultinfo.getAffectedRows());

				aptr2 = ai2.getPropertyTable();
				CHECK_EQUAL(1, static_cast<int>(aptr2.size()));
				aptr2.clear();

				ai2.setDefaultParameter(list, ACCESSMETHOD_TestSqlBuilder_IntAndTwoStrings_SelectWhere);

				executeresultinfo = ai2.execute(ACCESSMETHOD_TestSqlBuilder_IntAndTwoStrings_SelectWhere, iter1);
				CHECK_EQUAL(basar::db::aspect::SUCCESS, executeresultinfo.getError());
				CHECK_EQUAL(0, (int)executeresultinfo.getAffectedRows());

				aptr2 = ai2.getPropertyTable();
				CHECK_EQUAL(1, static_cast<int>(aptr2.size()));
				aptr2.clear();

				ai2.setDefaultParameter(list, ACCESSMETHOD_TestSqlBuilder_IntAndTwoStrings_SelectWhere_3);
				ai2.setDefaultParameter(list, ACCESSMETHOD_TestSqlBuilder_IntAndTwoStrings_SelectWhere_3);
				executeresultinfo = ai2.execute(ACCESSMETHOD_TestSqlBuilder_IntAndTwoStrings_SelectWhere_3, iter1);
				CHECK_EQUAL(basar::db::aspect::SUCCESS, executeresultinfo.getError());
				CHECK_EQUAL(0, (int)executeresultinfo.getAffectedRows());

				aptr2 = ai2.getPropertyTable();
				CHECK_EQUAL(1, static_cast<int>(aptr2.size()));
				aptr2.clear();

				aptr1.clear();
			}
		}
		catch (basar::Exception & ex)
		{
			connRef.close();	
			std::cout << ex.what() << std::endl;
			const bool ExceptionOccured = false;
			CHECK(ExceptionOccured);
			throw;
		}
		connRef.close();
	}

	TEST(DBAcpect_AccessorInstanceRef_DefaultParameterUsage_DefaultParameterUsage_Select_And_Insert)
	{
		ConnectionRef connRef = TestConnection::createDefaultConnectionRef();
		try
		{
			AccessorInstanceRef ai1 = Manager::getInstance().createAccessorInstance(
				"test_sqlbuilderWhere_Select_AccessorInstanceRef",
				ACCESSORNAME_ButMasterTableReader_Int, 
				connRef,
				basar::db::aspect::FULL_CACHING);

			AccessorInstanceRef ai2 = Manager::getInstance().createAccessorInstance(
				"test_sqlbuilderWhere_WhereSelect_AccessorInstanceRef_nodate",
				ACCESSORNAME_TestSqlBuilder_EnsureTest, 
				connRef,
				basar::db::aspect::FULL_CACHING);

			{ 
				AccessorPropertyTableRef  aptr1;
				AccessorPropertyTableRef  aptr2;

				ExecuteResultInfo executeresultinfo = 
					ai1.execute(ACCESSMETHOD_ButMasterTableReader_Int);
				CHECK_EQUAL(basar::db::aspect::SUCCESS, executeresultinfo.getError());
				CHECK_EQUAL(0, (int)executeresultinfo.getAffectedRows());

				aptr1 = ai1.getPropertyTable();
				AccessorPropertyTable_YIterator iter1 = aptr1.begin();


				basar::VarString defaultstring = "colch2=COLCH21;colch1=1;";         
				ai2.setDefaultParameter(defaultstring, ACCESSMETHOD_TestSqlBuilder_IntAndTwoStrings_SelectWhere_2);

				executeresultinfo = ai2.execute(ACCESSMETHOD_TestSqlBuilder_IntAndTwoStrings_SelectWhere_2, iter1);    
				CHECK_EQUAL(basar::db::aspect::SUCCESS, executeresultinfo.getError());

				ai2.setDefaultParameter(defaultstring, ACCESSMETHOD_TestSqlBuilder_IntAndTwoStrings_Insert);
				executeresultinfo = ai2.execute(ACCESSMETHOD_TestSqlBuilder_IntAndTwoStrings_Insert, iter1);
				CHECK_EQUAL(basar::db::aspect::NOT_EXECUTABLE, executeresultinfo.getError());

				iter1.setPropertyState (PROPDEF_COLINT.getName(),basar::SS_INSERT); 
				executeresultinfo = ai2.execute(ACCESSMETHOD_TestSqlBuilder_IntAndTwoStrings_Insert, iter1);

				CHECK_EQUAL(basar::db::aspect::SUCCESS, executeresultinfo.getError());
				CHECK_EQUAL(1, (int)executeresultinfo.getAffectedRows());

				aptr2 = ai2.getPropertyTable();
				CHECK_EQUAL(1, static_cast<int>(aptr2.size()));
				aptr2.clear();

				aptr1.clear();
			}
		}
		catch (basar::Exception & ex)
		{
			connRef.close();	
			std::cout << ex.what() << std::endl;
			const bool ExceptionOccured = false;
			CHECK(ExceptionOccured);
			throw;
		}
		connRef.close();
	}

	TEST(DBAcpect_AccessorInstanceRef_DefaultParameterUsage_DefaultParameterUsage_Insert)
	{
		ConnectionRef connRef = TestConnection::createDefaultConnectionRef();
		try
		{
			AccessorInstanceRef ai1 = Manager::getInstance().createAccessorInstance(
				"test1",
				ACCESSORNAME_TestSqlBuilder_EnsureTest, 
				connRef,
				basar::db::aspect::FULL_CACHING);

			AccessorInstanceRef ai2 = Manager::getInstance().createAccessorInstance(
				"test2",
				ACCESSORNAME_TestSqlBuilder_EnsureTest, 
				connRef,
				basar::db::aspect::FULL_CACHING);

			{ 
				AccessorPropertyTableRef  aptr1;
				AccessorPropertyTableRef  aptr2;

				ExecuteResultInfo executeresultinfo = 
					ai1.execute(ACCESSMETHOD_TestSqlBuilder_IntAndTwoStrings_SelectIntsOnly);
				CHECK_EQUAL(basar::db::aspect::SUCCESS, executeresultinfo.getError());
				CHECK_EQUAL(0, (int)executeresultinfo.getAffectedRows());

				aptr1 = ai1.getPropertyTable();
				AccessorPropertyTable_YIterator iter1 = aptr1.begin();


				basar::VarString defaultstring = "colch2=COLCH21;colch1=1;";         
				ai2.setDefaultParameter(defaultstring, ACCESSMETHOD_TestSqlBuilder_IntAndTwoStrings_Insert);
				iter1.setPropertyState (PROPDEF_COLINT.getName(),basar::SS_INSERT); 

				executeresultinfo = ai2.execute(ACCESSMETHOD_TestSqlBuilder_IntAndTwoStrings_Insert, iter1);

				CHECK_EQUAL(basar::db::aspect::SUCCESS, executeresultinfo.getError());
				CHECK_EQUAL(1, (int)executeresultinfo.getAffectedRows());

				aptr2 = ai2.getPropertyTable();
				CHECK_EQUAL(0, static_cast<int>(aptr2.size()));
				aptr2.clear();


				aptr1.clear();
			}
		}
		catch (basar::Exception & ex)
		{
			connRef.close();	
			std::cout << ex.what() << std::endl;
			const bool ExceptionOccured = false;
			CHECK(ExceptionOccured);
			throw;
		}
		connRef.close();
	}


	TEST(DBAcpect_AccessorInstanceRef_DefaultParameterUsage_DefaultParameterUsage_OneInsert)
	{
		ConnectionRef connRef = TestConnection::createDefaultConnectionRef();
		try
		{
			AccessorInstanceRef ai1 = Manager::getInstance().createAccessorInstance(
				"test_sqlbuilderWhere_Select_AccessorInstanceRef",
				ACCESSORNAME_ButMasterTableReader_Int, 
				connRef,
				basar::db::aspect::FULL_CACHING);

			AccessorInstanceRef ai2 = Manager::getInstance().createAccessorInstance(
				"test_sqlbuilderWhere_WhereSelect_AccessorInstanceRef_nodate",
				ACCESSORNAME_TestSqlBuilder_IntAndTwoStrings_OneInsert, 
				connRef,
				basar::db::aspect::FULL_CACHING);

			{ 
				AccessorPropertyTableRef  aptr1;
				AccessorPropertyTableRef  aptr2;

				ExecuteResultInfo executeresultinfo = 
					ai1.execute(ACCESSMETHOD_ButMasterTableReader_Int);
				CHECK_EQUAL(basar::db::aspect::SUCCESS, executeresultinfo.getError());
				CHECK_EQUAL(0, (int)executeresultinfo.getAffectedRows());

				aptr1 = ai1.getPropertyTable();
				AccessorPropertyTable_YIterator iter1 = aptr1.begin();

				basar::VarString defParam;
				defParam.format("%s=%d; %s=%s;",
					"colch1", 1,
					"colch2", "COLCH21");      
				ai2.setDefaultParameter(defParam, ACCESSMETHOD_TestSqlBuilder_IntAndTwoStrings_OneInsert);

				executeresultinfo = ai2.execute(ACCESSMETHOD_TestSqlBuilder_IntAndTwoStrings_OneInsert, iter1);

				CHECK_EQUAL(basar::db::aspect::SUCCESS, executeresultinfo.getError());
				CHECK_EQUAL(1, (int)executeresultinfo.getAffectedRows());

				aptr2 = ai2.getPropertyTable();
				CHECK_EQUAL(0, static_cast<int>(aptr2.size()));
				aptr2.clear();


				aptr1.clear();
			}
		}
		catch (basar::Exception & ex)
		{
			connRef.close();	
			std::cout << ex.what() << std::endl;
			const bool ExceptionOccured = false;
			CHECK(ExceptionOccured);
			throw;
		}
		connRef.close();
	}


	TEST(DBAcpect_AccessorInstanceRef_DefaultParameterUsage_Test_SetAndGet_oneSqlbuilderPerMethod)
	{
		ConnectionRef connRef = TestConnection::createDefaultConnectionRef();
		try
		{
			AccessorInstanceRef ai1 = Manager::getInstance().createAccessorInstance(
				"test1",
				ACCESSORNAME_TestDefaultParameter_SetAndGet, 
				connRef,
				basar::db::aspect::FULL_CACHING);
			{ 
				ParameterList setListEmpty;
				ParameterList setListA;
				ParameterList setListB;
				ParameterList setListC;
				ParameterList list0;
				ParameterList list1;
				ParameterList list2;
				ParameterList list3;

				AccessMethodRef aMR_OneSqlbuilder_1 = ai1.getAccessMethod(ACCESSMETHOD_OneSqlbuilder_1);
				AccessMethodRef aMR_OneSqlbuilder_2 = ai1.getAccessMethod(ACCESSMETHOD_OneSqlbuilder_2);
				AccessMethodRef aMR_OneSqlbuilder_3 = ai1.getAccessMethod(ACCESSMETHOD_OneSqlbuilder_3);

				SQLStringBuilderRef builder1 = aMR_OneSqlbuilder_1.getBuilder(0) ;
				SQLStringBuilderRef builder2 = aMR_OneSqlbuilder_2.getBuilder(0) ;
				SQLStringBuilderRef builder3 = aMR_OneSqlbuilder_3.getBuilder(0) ;

				setListA.push_back(PROPDEF_COLSER.getName(),"1");
				setListA.push_back(PROPDEF_COLINT.getName());
				setListB.push_back(PROPDEF_COLSMINT.getName(),"1");
				setListC.push_back(PROPDEF_COLFL.getName(),"1.0");

				// default start Value
				list0 = ai1.getDefaultParameter("");
				CHECK_EQUAL(0, static_cast< int>(list0.size()));
				CHECK(list0 == setListEmpty);
				list1 = ai1.getDefaultParameter(ACCESSMETHOD_OneSqlbuilder_1);  
				CHECK(list1 == setListEmpty);
				CHECK(builder1.getParameterList() == setListEmpty);
				CHECK(builder2.getParameterList() == setListEmpty);
				CHECK(builder3.getParameterList() == setListEmpty);

				// set one ParameterList on one Accessmethod:
				ai1.setDefaultParameter(setListB, ACCESSMETHOD_OneSqlbuilder_2);
				list0 = ai1.getDefaultParameter(""); 
				list1 = ai1.getDefaultParameter(ACCESSMETHOD_OneSqlbuilder_1);  
				list2 = ai1.getDefaultParameter(ACCESSMETHOD_OneSqlbuilder_2); 
				list3 = ai1.getDefaultParameter(ACCESSMETHOD_OneSqlbuilder_3); 
				CHECK(list0 == setListEmpty);
				CHECK(list1 == setListEmpty);
				CHECK(list2 == setListB);
				CHECK(list3 == setListEmpty);
				CHECK(builder1.getParameterList() == setListEmpty);
				CHECK(builder2.getParameterList() == setListB);
				CHECK(builder3.getParameterList() == setListEmpty);

				// set one ParameterList on an unset builder
				builder3.setParameterList(setListC);
				CHECK(builder3.getParameterList() == setListC);
				list3 = ai1.getDefaultParameter(ACCESSMETHOD_OneSqlbuilder_3); 
				CHECK(list3 == setListC);

				// set one ParameterList on all Accessmethod:
				ai1.setDefaultParameter(setListA, "");
				list0 = ai1.getDefaultParameter("");
				CHECK(list0 == setListA); 
				list1 = ai1.getDefaultParameter(ACCESSMETHOD_OneSqlbuilder_1);
				CHECK(list1 == setListA);
				list2 = ai1.getDefaultParameter(ACCESSMETHOD_OneSqlbuilder_2);
				CHECK(list2 == setListA);
				list3 = ai1.getDefaultParameter(ACCESSMETHOD_OneSqlbuilder_2);
				CHECK(list3 == setListA);
				CHECK(builder1.getParameterList() == setListA);
				CHECK(builder2.getParameterList() == setListA);
				CHECK(builder3.getParameterList() == setListA);

				// set one ParameterList on one Accessmethod:
				ai1.setDefaultParameter(setListB, ACCESSMETHOD_OneSqlbuilder_2);
				list0 = ai1.getDefaultParameter(""); 
				list1 = ai1.getDefaultParameter(ACCESSMETHOD_OneSqlbuilder_1);  
				list2 = ai1.getDefaultParameter(ACCESSMETHOD_OneSqlbuilder_2); 
				list3 = ai1.getDefaultParameter(ACCESSMETHOD_OneSqlbuilder_3); 
				CHECK(list0 == setListA);
				CHECK(list1 == setListA);
				CHECK(list2 == setListB);
				CHECK(list3 == setListA);
				CHECK(builder1.getParameterList() == setListA);
				CHECK(builder2.getParameterList() == setListB);
				CHECK(builder3.getParameterList() == setListA);

				// set one ParameterList on an all set builder
				builder3.setParameterList(setListC);
				CHECK(builder3.getParameterList() == setListC);
				list3 = ai1.getDefaultParameter(ACCESSMETHOD_OneSqlbuilder_3); 

				// caution: still A, because Accessor does not know about builder change
				CHECK(list3 == setListA);

				// caution: set one ParameterList on an set builder
				builder2.setParameterList(setListC);
				CHECK(builder2.getParameterList() == setListC);
				list2 = ai1.getDefaultParameter(ACCESSMETHOD_OneSqlbuilder_2); 
				CHECK(!(list2 == setListC));
				CHECK(list2 == setListB);

			}
		}
		catch (basar::Exception & ex)
		{
			connRef.close();	
			std::cout << ex.what() << std::endl;
			const bool ExceptionOccured = false;
			CHECK(ExceptionOccured);
			throw;
		}
		connRef.close();
	}


	TEST(DBAcpect_AccessorInstanceRef_DefaultParameterUsage_Test_SetAndGet_moreThanOneSqlbuilderPerMethod)
	{
		ConnectionRef connRef = TestConnection::createDefaultConnectionRef();
		try
		{
			AccessorInstanceRef ai1 = Manager::getInstance().createAccessorInstance(
				"test1",
				ACCESSORNAME_TestDefaultParameter_SetAndGet, 
				connRef,
				basar::db::aspect::FULL_CACHING);
			{ 
				ParameterList setListEmpty;
				ParameterList setListA;
				ParameterList setListB;
				ParameterList setListC;
				ParameterList list0;
				ParameterList list1;
				ParameterList list2;
				ParameterList list3;

				AccessMethodRef aMR_OneSqlbuilder_1 = ai1.getAccessMethod(ACCESSMETHOD_OneSqlbuilder_1);
				AccessMethodRef aMR_OneSqlbuilder_M = ai1.getAccessMethod(ACCESSMETHOD_MoreSqlbuilder_1);
				SQLStringBuilderRef builder1  = aMR_OneSqlbuilder_1.getBuilder(0) ;
				SQLStringBuilderRef builderM1 = aMR_OneSqlbuilder_M.getBuilder(0) ;
				SQLStringBuilderRef builderM2 = aMR_OneSqlbuilder_M.getBuilder(1) ;
				SQLStringBuilderRef builderM3 = aMR_OneSqlbuilder_M.getBuilder(2) ;

				setListA.push_back(PROPDEF_COLSER.getName(),"1");
				setListA.push_back(PROPDEF_COLINT.getName());
				setListB.push_back(PROPDEF_COLSMINT.getName(),"1");
				setListC.push_back(PROPDEF_COLFL.getName(),"1.0");

				// default start Value
				list0 = ai1.getDefaultParameter("");
				CHECK_EQUAL(0, static_cast< int>(list0.size()));
				CHECK(list0 == setListEmpty);
				list1 = ai1.getDefaultParameter(ACCESSMETHOD_OneSqlbuilder_1);  
				CHECK(list1 == setListEmpty);
				CHECK(builder1.getParameterList()  == setListEmpty);
				CHECK(builderM1.getParameterList() == setListEmpty);
				CHECK(builderM2.getParameterList() == setListEmpty);
				CHECK(builderM3.getParameterList() == setListEmpty);

				// change second builder
		     	builderM2.setParameterList(setListB);
				CHECK(builderM2.getParameterList() == setListB);
				list3 = ai1.getDefaultParameter(ACCESSMETHOD_MoreSqlbuilder_1); 
				CHECK(list3 == setListEmpty);

				// change first builder
				builderM1.setParameterList(setListC);
				CHECK(builderM1.getParameterList() == setListC);
				list3 = ai1.getDefaultParameter(ACCESSMETHOD_MoreSqlbuilder_1); 
				CHECK(list3 == setListC);


				// set one ParameterList on multi Accessmethod:
				ai1.setDefaultParameter(setListA, ACCESSMETHOD_MoreSqlbuilder_1);
				list0 = ai1.getDefaultParameter(""); 
				list1 = ai1.getDefaultParameter(ACCESSMETHOD_OneSqlbuilder_1);  
				list2 = ai1.getDefaultParameter(ACCESSMETHOD_MoreSqlbuilder_1); 
				CHECK(list0 == setListEmpty);
				CHECK(list1 == setListEmpty);
				CHECK(list2 == setListA);
				CHECK(builder1.getParameterList()  == setListEmpty);
				CHECK(builderM1.getParameterList() == setListA);
				CHECK(builderM2.getParameterList() == setListA);
				CHECK(builderM3.getParameterList() == setListA);

                // change first builder
				builderM1.setParameterList(setListC);
				CHECK(builderM1.getParameterList() == setListC);
				list3 = ai1.getDefaultParameter(ACCESSMETHOD_MoreSqlbuilder_1); 
				CHECK(list3 == setListA);

			}
		}
		catch (basar::Exception & ex)
		{
			connRef.close();	
			std::cout << ex.what() << std::endl;
			const bool ExceptionOccured = false;
			CHECK(ExceptionOccured);
			throw;
		}
		connRef.close();
	}

}
#endif
// ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~  ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~
