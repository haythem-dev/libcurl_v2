/** $Id$                                          
*                                                  
* @file                                            
* @author Anke Klink                      
* @date 2008                            
*/    

#include <UnitTest++/UnitTest++.h>
#include "libbasardbaspect.h"

#include "testaccessorholder.h"

#include "libbasarproperty.h"
#include "libbasardbsql.h"


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
using basar::Int64;
namespace basar
{
	namespace test
	{
		namespace unit
		{
			namespace dbaspect
			{

				// ---------------------------------------------------------------------------------------------------
				// struct SelectAccessor
				// ---------------------------------------------------------------------------------------------------

				/*!
				* cleans the data 
				*/
				void TestAccessorHolder::SelectAccessor::clean()
				{
					m_propTable.clear();
					m_propTable.reset();
					m_ai.reset();
					m_accessorMethod = "";
					m_yiter = AccessorPropertyTable_YIterator();
					m_Error = ExecuteResultInfo();
				}

				/*!
				* initializes the object, creates the 
				* AccessorInstanceRef and calls select
				*/
				void  TestAccessorHolder::SelectAccessor::init(
					ConnectionRef & connref, 
					ConstString     accessorname1, 
					ConstString     realaccessorname, 
					ConstString     accessormethod)
				{
					m_accessorMethod = accessormethod;
					m_ai = basar::db::aspect::Manager::getInstance().createAccessorInstance(
						accessorname1,
						realaccessorname, 
						connref,
						basar::db::aspect::ON_DEMAND_CACHING);
					select();
				}

				/*!
				* executes the m_accessorMethod Method and initites
				* m_propTable and m_yiter
				* @return Error ocurred
				*/
				bool TestAccessorHolder::SelectAccessor::select()
				{
					m_Error = m_ai.execute(m_accessorMethod, true);
					m_propTable = m_ai.getPropertyTable();
					m_yiter = m_propTable.begin();
					return isOk();
				}

				/*!
				* Sets the PropertyState to one column of the Y-Iterator
				*/
				void TestAccessorHolder::SelectAccessor::setPropertyState(
					const basar::VarString & col,  //!< columnnamme
					basar::SupportedStateEnum  state //!< state to be set
					)
				{
					try
					{
						TEXTOUTPUT2("YIterator::setPropertyState(" << col<< "," << PropertyState(state).toString() << " ) from " << m_yiter.getState (col).toString());
						m_yiter.setPropertyState (col,state);
					}
					catch (basar::Exception & ex)
					{
						TEXTOUTPUT_WARN ("YIterator::setPropertyState: Can't set state" << ex.what());

					}
				}

				/*!
				* Sets the PropertyState to all columns of the Y-Iterator
				*/
				void TestAccessorHolder::SelectAccessor::setPropertyState( basar::SupportedStateEnum  state)
				{
					AccessorPropertyTable_XIterator xiter;
					bool onlyUnset = true;
					for (xiter = m_yiter.begin(); xiter != m_yiter.end() ; ++xiter)
					{
						try
						{
							if (basar::SS_UNSET == xiter.getState().getState())
							{
								// can't set (would be an Exception.
							}
							else
							{
								onlyUnset = false;
								TEXTOUTPUT2("XIterator::setPropertyState(" << PropertyState(state).toString() << " ) from " << xiter.getState().toString());
								xiter.setPropertyState(state);
							}
						}
						catch (basar::Exception & ex)
						{
							TEXTOUTPUT_WARN ("XIterator::Can't set state" << ex.what());

						}
					}
					if (onlyUnset)
					{
						TEXTOUTPUT_WARN ("XIterator::Can't set state " << PropertyState(state).toString()  << "- all states are unset");
					}
				}

				/*!
				* returns if the execute Statement was sucessfull and if more than
				* 0 lines are selected.
				*/
				bool TestAccessorHolder::SelectAccessor::isOk() const
				{
					return (
						(basar::db::aspect::SUCCESS == m_Error.getError())  &&
						(!m_yiter.isEnd())
						);
				}
				// ---------------------------------------------------------------------------------------------------
				// TestAccessorHolder::TestAccessor
				// ---------------------------------------------------------------------------------------------------

				/*!
				* initializes the object, creates the 
				* AccessorInstanceRef and calls select
				*/
				void TestAccessorHolder::TestAccessor::init(
					ConnectionRef & connref, 
					ConstString     accessorname1, 
					ConstString     realaccessorname)
				{
					m_ai = Manager::getInstance().createAccessorInstance(
						accessorname1,
						realaccessorname, 
						connref,
						basar::db::aspect::ON_DEMAND_CACHING);
				}

				/*!
				* resets the AccessorInstanceRef
				*/
				void TestAccessorHolder::TestAccessor::clean()
				{
					m_ai.reset();
				}


				// ---------------------------------------------------------------------------------------------------
				// TestAccessorHolder::AccessorMethodEntry::
				// ---------------------------------------------------------------------------------------------------


				/*!
				* constructor:
				*/
				TestAccessorHolder::AccessorMethodEntry::AccessorMethodEntry(
					const VarString & name, 
					basar::SupportedStateEnum s1,
					basar::SupportedStateEnum s2):
				m_methodname(name),
					m_usedSupportedStateEnum(s1),
					m_otherSupportedStateEnum(s2),
					m_changeColser(false),
					m_removeColser(false),
					m_resetSelectIterator(false)
				{
					// at SS_INSERT the colserlist will be removed and
					// the listed colser will not be used
					// for SS_DELETE the select Iterator has to be reset
					// for SS_DELETE and SS_UPDATE the listed colser will 
					// be used
					switch (s1)
					{
					case basar::SS_INSERT:
						{
							m_resetSelectIterator = false;
							m_changeColser = false;
							m_removeColser = true;
						}
						break;
					case basar::SS_DELETE :
						{
							m_resetSelectIterator = true;
							m_changeColser = true;
							m_removeColser = false;
						}
						break;
					case basar::SS_UPDATE :
						{
							m_resetSelectIterator = false;
							m_changeColser = true;
							m_removeColser = false;
						}
						break;
					default:
						{
							m_resetSelectIterator = false;
							m_changeColser = false;
							m_removeColser = false;
						}
					}
				} 

				/*!
				* returns the colser value number nr 
				* from the AccessorInstanceRef, 
				* calls ai.getLastInsertedSerial(nr)
				*/
				std::pair<bool , Int64>  TestAccessorHolder::AccessorMethodEntry::getColser(
					AccessorInstanceRef & ai,  //!< AccessorInstance, the LastInsertedSerial is read from
					Int32 nr                   //!< number of the LastInsertedSerial to be returned
					) const
				{
					std::pair<bool , Int64> retval(false,0);
					if (ai.getNumberOfLastInsertedSerials() >= nr)
					{
						retval.first = true;
						retval.second = ai.getLastInsertedSerial(nr);
					}
					return retval;
				}

				/*!
				* changes the colser variable on the 
				* AccessorPropertyTable_YIterator yiter
				* to the nr. LastInsertedSerial from the
				* AccessorInstanceRef ai
				*/
				bool  TestAccessorHolder::AccessorMethodEntry::changeColserIfNecessary(
					AccessorPropertyTable_YIterator & yiter, //!< YIterator, to be changed
					AccessorInstanceRef & ai,                //!< AccessorInstance, the LastInsertedSerial is read from
					Int32 nr  ,                                 //!< number of the LastInsertedSerial to be returned
					bool & colserCanged
					) const
				{
					bool retval = true;
					colserCanged = false;
					if (m_changeColser)
					{
						std::pair<bool , Int64> p = getColser( ai, nr);
						if (p.first)
						{
							yiter.setInt32(PROPDEF_COLSER.getName(), static_cast<basar::Int32>( p.second));
							colserCanged = true;
						}
						retval = p.first;
					}
					return retval; 
				}

				/*!
				* changes the colser variable on the ParameterList plist
				* to the nr. LastInsertedSerial from the
				* AccessorInstanceRef ai
				*/
				bool  TestAccessorHolder::AccessorMethodEntry::changeColserIfNecessary(
					ParameterList & plist,         //!< ParameterList, to be changed
					AccessorInstanceRef & ai,      //!< AccessorInstance, the LastInsertedSerial is read from
					Int32 nr          ,               //!< number of the LastInsertedSerial to be returned
					bool & colserCanged
					) const
				{
					bool retval = true;
					colserCanged = false;
					if (m_changeColser)
					{
						std::pair<bool , Int64> p = getColser( ai, nr);
						if (p.first)
						{
							VarString s;
							s.format("%lld", p.second);
							plist.setValue(PROPDEF_COLSER.getName(), s);
							colserCanged = true;
						}
						retval = p.first;
					}
					return retval;
				}
				// ---------------------------------------------------------------------------------------------------
				// TestAccessorHolder::AccessorMethodEntryList::
				// ---------------------------------------------------------------------------------------------------


				/*! 
				* constructor
				*/
				TestAccessorHolder::AccessorMethodEntryList::AccessorMethodEntryList()
				{
					m_numberOfTestMethods = 0;
					m_numberOfInsertTestMethods = 0;
				}

				/*!
				* returns a SupportedStateEnum that is not s1 and not s2
				* f.ex getOtherSupportedStateEnum(Insert,Delete) => Update
				*/
				basar::SupportedStateEnum 
					TestAccessorHolder::AccessorMethodEntryList::getOtherSupportedStateEnum(
					basar::SupportedStateEnum s1, basar::SupportedStateEnum s2) const
				{
					basar::SupportedStateEnum retval = basar::SS_DELETE;
					if (basar::SS_INSERT == s1)
					{
						if (basar::SS_DELETE == s2)
						{
							retval = basar::SS_UPDATE;
						}
					}
					else if (basar::SS_UPDATE == s1)
					{
						if (basar::SS_DELETE == s2)
						{
							retval = basar::SS_INSERT;
						}
					}
					else if (basar::SS_DELETE == s1)
					{
						if (basar::SS_INSERT == s2)
						{
							retval = basar::SS_UPDATE;
						}
						else
						{
							retval = basar::SS_INSERT;
						}
					}
					return retval;
				}

				/*!
				* adds a new entry and changes the m_otherSupportedStateEnum of the previous entry,
				* to not be used the state of this test, in order to start with states that are 
				* seen as wrong for the test (SS_CLEAN can't be set for an Y-Iterator Value)
				*/
				void TestAccessorHolder::AccessorMethodEntryList::addEntry(
					basar::SupportedStateEnum  s1, const VarString & name)
				{
					std::vector<AccessorMethodEntry>::reverse_iterator iter = m_entries.rbegin();
					if (iter != m_entries.rend())
					{
						iter->m_otherSupportedStateEnum = getOtherSupportedStateEnum(iter->m_usedSupportedStateEnum, s1);
					}
					m_entries.push_back(AccessorMethodEntry(name, s1, getOtherSupportedStateEnum(s1, s1)));
					m_numberOfTestMethods++;
					if (basar::SS_INSERT == s1)
					{
						m_numberOfInsertTestMethods++;
					}
				}

				// ---------------------------------------------------------------------------------------------------
				//TestAccessorHolder:
				// ---------------------------------------------------------------------------------------------------

				/*!
				* the constructor does not call the AcessorInstances as it will be 
				* calles outside of our own test scenario, the test starts with
				* a call to the member function start:
				* CHECK(start());
				*/
				TestAccessorHolder::TestAccessorHolder() 
				{
					m_colsernumber = 0;
					m_idOfEnsureTestInIntVector = 2;
					m_selectIntVector.resize(3);

					m_paramListNotEnoughValues.push_back(PROPDEF_COLCH2.getName(),"COLCH21");

					m_paramListNonIntValues.push_back(PROPDEF_COLCH2.getName(),"COLCH21");
					m_paramListNonIntValues.push_back(PROPDEF_COLCH1.getName(),"1");

					m_paramListAllEnsureValues.push_back(PROPDEF_COLCH2.getName(),"COLCH21");
					m_paramListAllEnsureValues.push_back(PROPDEF_COLCH1.getName(),"1");
					m_paramListAllEnsureValues.push_back(PROPDEF_COLSER.getName(),"1");
					m_paramListAllEnsureValues.push_back(PROPDEF_COLINT.getName(),"100001");
					m_paramListAllEnsureValues.push_back(PROPDEF_COLSMINT.getName(),"2001");

				}

				/*!
				* The desructor does not call reset, as the test should do that 
				* itself
				*/
				TestAccessorHolder::~TestAccessorHolder() 
				{

				}

				/*!
				* resets all values and closes the Connection
				*/
				void TestAccessorHolder::reset()
				{
					m_colsernumber = 0;

					m_selectAllEnsureParameters.clean();

					std::vector<SelectAccessor>::iterator iter;
					for (iter = m_selectIntVector.begin(); iter != m_selectIntVector.end(); ++iter)
					{
						iter->clean();
					}
					m_ensureTest.clean();

					basar::db::sql::ConnectionRef sqlConn = m_connRef.getSqlConnection();
					sqlConn.rollback();
					m_connRef.close();
				}

				/*!
				* returns if all select statements have returned valid
				* results
				*/
				bool TestAccessorHolder::isSelectOk()
				{
					bool retval = m_selectAllEnsureParameters.isOk();
					if (retval)
					{
						std::vector<SelectAccessor>::const_iterator iter;
						for (iter = m_selectIntVector.begin(); iter != m_selectIntVector.end(); ++iter)
						{
							retval = (retval && (iter->isOk()));
						}
					}
					return retval;
				}


				/*!
				* returns the size of the selectIntVector:
				*/
				Int32  TestAccessorHolder::getSelectIntVectorSize()
				{
					return static_cast<Int32>( m_selectIntVector.size());
				}

				/*!
				* start Routine that creates the Connection and 
				* initializes the AcessorInstances (create and 
				* execute Select Method) and retuns isSelectOk()
				* This is the first test to be done
				* CHECK(start())
				*/
				bool TestAccessorHolder::start( )
				{
					m_connRef = TestConnection::createDefaultConnectionRef();
					basar::db::sql::ConnectionRef sqlConn = m_connRef.getSqlConnection();
					sqlConn.begin();


					m_ensureTest.init(m_connRef,
						"ensureTest",
						ACCESSORNAME_TestSqlBuilder_EnsureParameters); 

					m_selectAllEnsureParameters.init(m_connRef,
						"yiteratorholder_EnsureProperties_All",
						ACCESSORNAME_TestSqlBuilder_EnsureParameters_SelectYIterator, 
						ACCESSMETHOD_TestSqlBuilder_EnsureParameters_SelectYIterator_AllEnsureParameters);

					m_selectIntVector.resize(3);
					m_selectIntVector[0].init(
						m_connRef,
						"yiteratorholder_EnsureProperties_IntOnly", 
						ACCESSORNAME_TestSqlBuilder_EnsureParameters_SelectYIterator_IntOnly,
						ACCESSMETHOD_TestSqlBuilder_EnsureParameters_SelectYIterator_IntOnly);

					m_selectIntVector[1].init(
						m_connRef,
						"yiteratorholder_EnsureProperties_IntOnly_FromAll",
						ACCESSORNAME_TestSqlBuilder_EnsureParameters_SelectYIterator, 
						ACCESSMETHOD_TestSqlBuilder_EnsureParameters_SelectYIterator_IntOnly);

					// select on the EnsureTest itself:
					m_selectIntVector[2].m_ai = m_ensureTest.m_ai;
					m_selectIntVector[2].m_accessorMethod = ACCESSMETHOD_TestSqlBuilder_EnsureParameters_SelectYIterator_IntOnly;
					m_selectIntVector[2].select();
					m_idOfEnsureTestInIntVector = 2;

					return  isSelectOk();
				}

				/*!
				* all that is to be done before the test with a new Method,
				* f.ex. remove the Colser List (clearLastInsertedSerials), if
				* m_removeColser is set to true (Insert-Test)
				*/
				bool TestAccessorHolder::startNewAccessorMethodTest(
					const AccessorMethodEntry & entry //!< the method that is tobe tested
					)
				{ 
					m_colsernumber = 0; 
					if (entry.m_removeColser)
					{       
						m_ensureTest.m_ai.clearLastInsertedSerials();
					}
					SHOW_METHODNAME("starting", entry.m_methodname );
					return true;
				}

				/*!
				* all that is to be done before the new test with a Method and a Y-Iterator
				* f.ex. reselect the Y-Iterator, when m_resetSelectIterator is
				* set to true (Delete-Test)
				* and change the colser variable, when m_changeColser is set
				* to true (Delete or Update Test)
				* advances the colsernumber, so each time startNewTest is called a
				* new colser variable is set.
				*/
				bool TestAccessorHolder::startNewTest( 
					const  AccessorMethodEntry & entry, //!< the method that is to be tested
					SelectAccessor & acc ,               //!< the Accessor the holds the Y-Iterator
					bool & colserCanged
					)
				{
					SHOW_METHODNAME("starting new test on ", entry.m_methodname << " with colsernr " << m_colsernumber << " colsersize = " << m_ensureTest.m_ai.getNumberOfLastInsertedSerials());
					bool retval = true;
					if (entry.m_resetSelectIterator) 
					{
						retval = acc.select();
					}
					if (retval)
					{
						retval = entry.changeColserIfNecessary(acc.m_yiter,m_ensureTest.m_ai, m_colsernumber, colserCanged);
						if (retval && entry.m_changeColser)
						{
							SHOW_METHODNAME(
								"changeColserIfNecessary  ", 
								entry.m_methodname 
								<< " changeColserIfNecessary with colsernr " << m_colsernumber 
								<< " colser = " << m_ensureTest.m_ai.getLastInsertedSerial(m_colsernumber)
								<< "yiter->colser = " << acc.m_yiter.getInt32("colser"));
						} 
					}
					m_colsernumber++;
					return retval;
				}

				/*!
				* all that is to be done before the new test with a Method and a ParameterList (no Y-Iterator)
				* change the colser variable, when m_changeColser is set
				* to true (Delete or Update Test)
				* advances the colsernumber, so each time startNewTest is called a
				* new colser variable is set.
				*/
				bool TestAccessorHolder::startNewTest( 
					const AccessorMethodEntry & entry,  //!< the method that is to be tested
					ParameterList  & plist     ,         //!< the Default Paramter List, where the colser variable is to be set to
					bool & colserCanged)
				{
					bool retval = entry.changeColserIfNecessary(plist,m_ensureTest.m_ai, m_colsernumber, colserCanged );
					m_colsernumber++;
					return retval;
				}

				/*!
				* all that is to be done after all tests with a new Method,
				* f.ex. reselect all Y-Iterator when m_resetSelectIterator is
				* set to true 
				*/
				bool TestAccessorHolder::endAccessorMethodTest(
					const AccessorMethodEntry & entry //!< the method that was tested
					)
				{ 
					if (entry.m_resetSelectIterator)
					{  
						m_selectAllEnsureParameters.select();
						m_selectIntVector[0].select();
						m_selectIntVector[1].select();
					}
					SHOW_METHODNAME("end", entry.m_methodname );
					return isSelectOk();
				}


				/*!
				* sets all colums of the Y-Iterator to the state 
				* and executes the method
				*/
				ExecuteResultInfo TestAccessorHolder::setPropertyAndExecute( 
					basar::VarString & method,            //!< the method that is to be tested 
					SelectAccessor & accessor,            //!< the y-Iterator that is to be set to state 
					basar::SupportedStateEnum state       //!< the state the y-Iterator is to be set to
					)
				{
					accessor.setPropertyState(state);
					return m_ensureTest.m_ai.execute(method, accessor.m_yiter);
				}


				/*!
				* List of all the methods that are tested in EnsureParametersAreSet
				*/
				Int32 TestAccessorHolder::addMethodsEnsureAreSet(AccessorMethodEntryList & methodnames)
				{
					Int32 number = 3;
					methodnames.addEntry(basar::SS_UNKNOWN,ACCESSMETHOD_TestSqlBuilder_EnsureParameters_AreSet_UsePropTab_SelectWhere);
					methodnames.addEntry(basar::SS_UNKNOWN,ACCESSMETHOD_TestSqlBuilder_EnsureParameters_AreSet_UseStringWithBlanks_SelectWhere);
					methodnames.addEntry(basar::SS_UNKNOWN,ACCESSMETHOD_TestSqlBuilder_EnsureParameters_AreSet_UseStringWithoutBlanks_SelectWhere);
					int i= 0;
					std::vector<AccessorMethodEntry>::reverse_iterator iter = methodnames.m_entries.rbegin();
					for (iter = methodnames.m_entries.rbegin(); iter != methodnames.m_entries.rend() && i<number; ++iter)
					{
						iter->m_resetSelectIterator = true;
					}
					return number;
				}

				/*!
				* List of all the methods that are tested in EnsureParametersAreSetAndAllInState
				* the list has to be structured as a list of an TestTripel(InsertTest, UpdateTest, DeleteTest)
				*/
				Int32 TestAccessorHolder::addMethodsEnsureAreSetAndAllInState(AccessorMethodEntryList  & methodnames)
				{ 
					methodnames.addEntry(basar::SS_INSERT,ACCESSMETHOD_TestSqlBuilder_EnsureParameters_AreSetAndAllInState_UsePropTab_Insert);
					methodnames.addEntry(basar::SS_UPDATE,ACCESSMETHOD_TestSqlBuilder_EnsureParameters_AreSetAndAllInState_UsePropTab_Update);
					methodnames.addEntry(basar::SS_DELETE,ACCESSMETHOD_TestSqlBuilder_EnsureParameters_AreSetAndAllInState_UsePropTab_Delete);

					methodnames.addEntry(basar::SS_INSERT,ACCESSMETHOD_TestSqlBuilder_EnsureParameters_AreSetAndAllInState_UseStringWithBlanks_Insert);
					methodnames.addEntry(basar::SS_UPDATE,ACCESSMETHOD_TestSqlBuilder_EnsureParameters_AreSetAndAllInState_UseStringWithBlanks_Update);
					methodnames.addEntry(basar::SS_DELETE,ACCESSMETHOD_TestSqlBuilder_EnsureParameters_AreSetAndAllInState_UseStringWithBlanks_Delete);

					methodnames.addEntry(basar::SS_INSERT,ACCESSMETHOD_TestSqlBuilder_EnsureParameters_AreSetAndAllInState_UseStringWithoutBlanks_Insert);
					methodnames.addEntry(basar::SS_UPDATE,ACCESSMETHOD_TestSqlBuilder_EnsureParameters_AreSetAndAllInState_UseStringWithoutBlanks_Update);
					methodnames.addEntry(basar::SS_DELETE,ACCESSMETHOD_TestSqlBuilder_EnsureParameters_AreSetAndAllInState_UseStringWithoutBlanks_Delete);

					return 9;
				} 

				/*!
				* List of all the methods that are tested in EnsureParametersAreSetAndOneInState
				* the list has to be structured as a list of an TestTripel(InsertTest, UpdateTest, DeleteTest)
				*/
				Int32 TestAccessorHolder::addMethodsEnsureAreSetAndOneInState(AccessorMethodEntryList & methodnames)
				{
					methodnames.addEntry(basar::SS_INSERT,ACCESSMETHOD_TestSqlBuilder_EnsureParameters_AreSetAnd1InState_UsePropTab_Insert);
					methodnames.addEntry(basar::SS_UPDATE,ACCESSMETHOD_TestSqlBuilder_EnsureParameters_AreSetAnd1InState_UsePropTab_Update);
					methodnames.addEntry(basar::SS_DELETE,ACCESSMETHOD_TestSqlBuilder_EnsureParameters_AreSetAnd1InState_UsePropTab_Delete);

					methodnames.addEntry(basar::SS_INSERT,ACCESSMETHOD_TestSqlBuilder_EnsureParameters_AreSetAnd1InState_UseStringWithBlanks_Insert);
					methodnames.addEntry(basar::SS_UPDATE,ACCESSMETHOD_TestSqlBuilder_EnsureParameters_AreSetAnd1InState_UseStringWithBlanks_Update);
					methodnames.addEntry(basar::SS_DELETE,ACCESSMETHOD_TestSqlBuilder_EnsureParameters_AreSetAnd1InState_UseStringWithBlanks_Delete);

					methodnames.addEntry(basar::SS_INSERT,ACCESSMETHOD_TestSqlBuilder_EnsureParameters_AreSetAnd1InState_UseStringWithoutBlanks_Insert);
					methodnames.addEntry(basar::SS_UPDATE,ACCESSMETHOD_TestSqlBuilder_EnsureParameters_AreSetAnd1InState_UseStringWithoutBlanks_Update);
					methodnames.addEntry(basar::SS_DELETE,ACCESSMETHOD_TestSqlBuilder_EnsureParameters_AreSetAnd1InState_UseStringWithoutBlanks_Delete);

					return 9;
				}

				/*!
				* List of all the methods that 
				* the list has to be structured as a list of an TestTripel(InsertTest, UpdateTest, DeleteTest)
				*/
				Int32 TestAccessorHolder::addMethodsEnsure1AreSetAnd2OneInState(TestAccessorHolder::AccessorMethodEntryList & methodnames)
				{
					methodnames.addEntry(basar::SS_INSERT,ACCESSMETHOD_TestSqlBuilder_EnsureParameters_1AreSetAnd2InState_UseStringWithoutBlanks_UseStringWithoutBlanks_Insert);
					methodnames.addEntry(basar::SS_UPDATE,ACCESSMETHOD_TestSqlBuilder_EnsureParameters_1AreSetAnd2InState_UseStringWithoutBlanks_UseStringWithoutBlanks_Update);
					methodnames.addEntry(basar::SS_DELETE,ACCESSMETHOD_TestSqlBuilder_EnsureParameters_1AreSetAnd2InState_UseStringWithoutBlanks_UseStringWithoutBlanks_Delete);

					methodnames.addEntry(basar::SS_INSERT,ACCESSMETHOD_TestSqlBuilder_EnsureParameters_1AreSetAnd2InState_UseStringWithoutBlanks_UseStringWithBlanks_Insert);
					methodnames.addEntry(basar::SS_UPDATE,ACCESSMETHOD_TestSqlBuilder_EnsureParameters_1AreSetAnd2InState_UseStringWithoutBlanks_UseStringWithBlanks_Update);
					methodnames.addEntry(basar::SS_DELETE,ACCESSMETHOD_TestSqlBuilder_EnsureParameters_1AreSetAnd2InState_UseStringWithoutBlanks_UseStringWithBlanks_Delete);

					methodnames.addEntry(basar::SS_INSERT,ACCESSMETHOD_TestSqlBuilder_EnsureParameters_1AreSetAnd2InState_UseStringWithBlanks_UseStringWithoutBlanks_Insert);
					methodnames.addEntry(basar::SS_UPDATE,ACCESSMETHOD_TestSqlBuilder_EnsureParameters_1AreSetAnd2InState_UseStringWithBlanks_UseStringWithoutBlanks_Update);
					methodnames.addEntry(basar::SS_DELETE,ACCESSMETHOD_TestSqlBuilder_EnsureParameters_1AreSetAnd2InState_UseStringWithBlanks_UseStringWithoutBlanks_Delete);

					methodnames.addEntry(basar::SS_INSERT,ACCESSMETHOD_TestSqlBuilder_EnsureParameters_1AreSetAnd2InState_UseStringWithBlanks_UseStringWithBlanks_Insert);
					methodnames.addEntry(basar::SS_UPDATE,ACCESSMETHOD_TestSqlBuilder_EnsureParameters_1AreSetAnd2InState_UseStringWithBlanks_UseStringWithBlanks_Update);
					methodnames.addEntry(basar::SS_DELETE,ACCESSMETHOD_TestSqlBuilder_EnsureParameters_1AreSetAnd2InState_UseStringWithBlanks_UseStringWithBlanks_Delete);

					methodnames.addEntry(basar::SS_INSERT,ACCESSMETHOD_TestSqlBuilder_EnsureParameters_1AreSetAnd2InState_UsePropTab_UsePropTab_Insert);
					methodnames.addEntry(basar::SS_UPDATE,ACCESSMETHOD_TestSqlBuilder_EnsureParameters_1AreSetAnd2InState_UsePropTab_UsePropTab_Update);
					methodnames.addEntry(basar::SS_DELETE,ACCESSMETHOD_TestSqlBuilder_EnsureParameters_1AreSetAnd2InState_UsePropTab_UsePropTab_Delete);

					return 15 + addMethodsEnsure1AreSetAnd2OneInStateUnsetAndInStateTest(methodnames);
				}

				/*!
				* List of all the methods that are tested in EnsureParameters1AreSetAnd2OneInState
				* the list has to be structured as a list of an TestTripel(InsertTest, UpdateTest, DeleteTest)
				*/
				Int32 TestAccessorHolder::addMethodsEnsure1AreSetAnd2OneInStateUnsetAndInStateTest(AccessorMethodEntryList  & methodnames)
				{
					methodnames.addEntry(basar::SS_INSERT,ACCESSMETHOD_TestSqlBuilder_EnsureParameters_1AreSetAnd2InState_UnsetAndInStateTest_UseStringWithoutBlanks_UseStringWithoutBlanks_Insert);
					methodnames.addEntry(basar::SS_UPDATE,ACCESSMETHOD_TestSqlBuilder_EnsureParameters_1AreSetAnd2InState_UnsetAndInStateTest_UseStringWithoutBlanks_UseStringWithoutBlanks_Update);
					methodnames.addEntry(basar::SS_DELETE,ACCESSMETHOD_TestSqlBuilder_EnsureParameters_1AreSetAnd2InState_UnsetAndInStateTest_UseStringWithoutBlanks_UseStringWithoutBlanks_Delete);

					methodnames.addEntry(basar::SS_INSERT,ACCESSMETHOD_TestSqlBuilder_EnsureParameters_1AreSetAnd2InState_UnsetAndInStateTest_UseStringWithoutBlanks_UseStringWithBlanks_Insert);
					methodnames.addEntry(basar::SS_UPDATE,ACCESSMETHOD_TestSqlBuilder_EnsureParameters_1AreSetAnd2InState_UnsetAndInStateTest_UseStringWithoutBlanks_UseStringWithBlanks_Update);
					methodnames.addEntry(basar::SS_DELETE,ACCESSMETHOD_TestSqlBuilder_EnsureParameters_1AreSetAnd2InState_UnsetAndInStateTest_UseStringWithoutBlanks_UseStringWithBlanks_Delete);

					methodnames.addEntry(basar::SS_INSERT,ACCESSMETHOD_TestSqlBuilder_EnsureParameters_1AreSetAnd2InState_UnsetAndInStateTest_UseStringWithBlanks_UseStringWithoutBlanks_Insert);
					methodnames.addEntry(basar::SS_UPDATE,ACCESSMETHOD_TestSqlBuilder_EnsureParameters_1AreSetAnd2InState_UnsetAndInStateTest_UseStringWithBlanks_UseStringWithoutBlanks_Update);
					methodnames.addEntry(basar::SS_DELETE,ACCESSMETHOD_TestSqlBuilder_EnsureParameters_1AreSetAnd2InState_UnsetAndInStateTest_UseStringWithBlanks_UseStringWithoutBlanks_Delete);

					methodnames.addEntry(basar::SS_INSERT,ACCESSMETHOD_TestSqlBuilder_EnsureParameters_1AreSetAnd2InState_UnsetAndInStateTest_UseStringWithBlanks_UseStringWithBlanks_Insert);
					methodnames.addEntry(basar::SS_UPDATE,ACCESSMETHOD_TestSqlBuilder_EnsureParameters_1AreSetAnd2InState_UnsetAndInStateTest_UseStringWithBlanks_UseStringWithBlanks_Update);
					methodnames.addEntry(basar::SS_DELETE,ACCESSMETHOD_TestSqlBuilder_EnsureParameters_1AreSetAnd2InState_UnsetAndInStateTest_UseStringWithBlanks_UseStringWithBlanks_Delete);

					methodnames.addEntry(basar::SS_INSERT,ACCESSMETHOD_TestSqlBuilder_EnsureParameters_1AreSetAnd2InState_UnsetAndInStateTest_UsePropTab_UsePropTab_Insert);
					methodnames.addEntry(basar::SS_UPDATE,ACCESSMETHOD_TestSqlBuilder_EnsureParameters_1AreSetAnd2InState_UnsetAndInStateTest_UsePropTab_UsePropTab_Update);
					methodnames.addEntry(basar::SS_DELETE,ACCESSMETHOD_TestSqlBuilder_EnsureParameters_1AreSetAnd2InState_UnsetAndInStateTest_UsePropTab_UsePropTab_Delete);   

					return 10;
				}

				bool TestAccessorHolder::checkYIteratorAfterExecutionAndReselectAtDelete(
					basar::SupportedStateEnum & usedSupportedStateEnum, 
					SelectAccessor & selectaccessor)
				{
					if (basar::SS_DELETE == usedSupportedStateEnum)
					{
						selectaccessor.select();
					}

					basar::property::PropertyStateSet ps = selectaccessor.m_yiter.getState();

					return (basar::SS_CLEAN == ps.getStates());
				}

			}
		}
	}
}
// ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~  ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~
