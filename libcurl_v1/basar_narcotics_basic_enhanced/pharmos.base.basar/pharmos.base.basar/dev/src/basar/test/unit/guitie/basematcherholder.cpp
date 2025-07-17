/** $Id$                                          
*                                                  
* @file                                            
* @author Anke Klink                      
* @date 2008                            
*/    

#include <UnitTest++/UnitTest++.h>
#include "libbasarguitie.h"

#include "basematcherholder.h"
#include "testconnection.h"
#include "libbasardbaspect_manager.h"

namespace basar
{
	namespace test
	{
		namespace unit
		{
			namespace guitie
			{   
				// --------------------------------------------------------------------------
				using basar::db::aspect::ExecuteResultInfo;
				using basar::db::aspect::AccessorInstanceRef;

				using basar::gui::tie::MatcherRef;

				// --------------------------------------------------------------------------
				BaseMatcherHolder::BaseMatcherHolder()
				{
					m_widget = new QWidget(0);
					m_testnumberColTest = m_testnumberRowTest = EQUAL;
					m_needsReset = true;
				}

				// --------------------------------------------------------------------------
				BaseMatcherHolder::~BaseMatcherHolder()
				{
					cleanAccessor();
					if ( !m_connection.isNull())
						m_connection.close();
					delete (m_widget);
				}

				// --------------------------------------------------------------------------
				bool  BaseMatcherHolder::cleanAccessor()
				{
					if (m_accessor)
					{
						m_accessor->getPropertyTable().clear();
					}
					return true;
				}

				// --------------------------------------------------------------------------
				VarString BaseMatcherHolder::getAccessorName()
				{
					VarString retval = m_accessornameColTestNorm;
					if (hasMoreAccNamesTest())
					{
						retval = m_accessornameColTestMoreAccNames;

					}
					return retval;
				}

				// --------------------------------------------------------------------------
				VarString BaseMatcherHolder::getAccessorMethodName()
				{
					VarString retval = m_accessormethodnameColTestNorm;
					if (hasMoreAccNamesTest())
					{
						retval = m_accessormethodnameColTestMoreAccNames;

					}
					return retval;
				}

				// --------------------------------------------------------------------------
				void BaseMatcherHolder::setConnection()
				{
					m_connection = TestConnection::createDefaultConnectionRef();

				}

				// --------------------------------------------------------------------------
				bool BaseMatcherHolder::resetAccessorAndMatcher()
				{
					bool retval = false; 
					cleanAccessor();
					if (checkInitalisation())
					{
                        AccessorMap::iterator it = m_AccessorMap.find(getAccessorName());

                        if (it != m_AccessorMap.end())
                        {
                            m_accessor = it->second;
                        }
                        else
                        {
                            //TODO: exception
                            assert(false);
                        }

						m_matcher  = basar::gui::tie::Manager::getInstance().createMatcher(m_widget, m_accessor->getPropertyTable());
						m_needsReset = false;
						retval = true;
					}
					return retval;
				}

				// --------------------------------------------------------------------------
				bool BaseMatcherHolder::checkInitalisation()
				{
					bool retval = true;
					if (m_connection.isNull())
					{
						setConnection();
					}

					if (m_connection.isNull())
					{
						retval = false;
					}
					else if ( getAccessorName().length() < 1 || 
						getAccessorMethodName().length() < 1)
					{
						retval = false;
					}
					return retval;
				}

				// --------------------------------------------------------------------------
				bool BaseMatcherHolder::setAccessorInfo(
					const basar::VarString & accessornameNorm , 
					const basar::VarString & accessornameMoreAccNames,
					const basar::VarString & accessormethodnameNorm , 
					const basar::VarString & accessormethodnameMoreAccNames,
					bool useResetAccessorAndMatcher) 
				{
					bool retval = true;
					m_accessornameColTestNorm = accessornameNorm;
					m_accessornameColTestMoreAccNames = accessornameMoreAccNames;
					m_accessormethodnameColTestNorm = accessormethodnameNorm;
					m_accessormethodnameColTestMoreAccNames = accessormethodnameMoreAccNames;

                    m_needsReset = true;

					if (useResetAccessorAndMatcher)
					{
						retval = checkInitalisation();
						if (retval)
						{
							retval = resetAccessorAndMatcher();  
						}
					}

					return retval;
				}


				// --------------------------------------------------------------------------
				bool  BaseMatcherHolder::setTestNumber(TestNumber colnumber , TestNumber rownumber) 
				{
					bool retval = true;
					if (colnumber != m_testnumberColTest || rownumber != m_testnumberRowTest)
					{
						m_testnumberColTest = colnumber;
						m_testnumberRowTest = rownumber;
						m_needsReset = true; 
					}
					return retval;

				}
				bool BaseMatcherHolder::start(TestNumber colnumber, TestNumber rownumber)
				{
					bool retval = false;
					if (setTestNumber(colnumber, rownumber))
					{
						retval = start();
					}
					return retval;
				}

				bool BaseMatcherHolder::start()
				{
					bool retval = false;
					if (checkInitalisation())
					{
						retval = true;
						if (m_needsReset) 
						{
							retval = resetAccessorAndMatcher();   
						}
					}
					return retval;
				}

				// --------------------------------------------------------------------------
				bool BaseMatcherHolder::select()
				{
					return select(true);
				}

				// --------------------------------------------------------------------------
				bool  BaseMatcherHolder::select(bool flagRemoveBeforeSelect)
				{
					bool retval =  false;

					if (checkInitalisation())
					{
						if (m_needsReset)
						{
							resetAccessorAndMatcher();  
						}
						ExecuteResultInfo executeresultinfo;
						VarString name;

                        if (flagRemoveBeforeSelect)
                        {
                            cleanAccessor();
                        }

                        executeresultinfo = m_accessor->execute(getAccessorMethodName(), flagRemoveBeforeSelect, false, basar::db::aspect::HOLD_RECORDSET);

						retval = (basar::db::aspect::SUCCESS == executeresultinfo.getError());
					}

					return retval;
				}

				// --------------------------------------------------------------------------
				BaseMatcherHolder::TestNumber BaseMatcherHolder::getColTestNumber()const
				{
					return  m_testnumberColTest;
				}

				// --------------------------------------------------------------------------
				BaseMatcherHolder::TestNumber BaseMatcherHolder::getRowTestNumber()const
				{
					return m_testnumberRowTest;
				}

				// --------------------------------------------------------------------------
				MatcherRef & BaseMatcherHolder::getMatcher()
				{
					return m_matcher;
				}
				// --------------------------------------------------------------------------
				QWidget * BaseMatcherHolder::getGui()
				{
					return m_widget;
				}
				// --------------------------------------------------------------------------
				//AccessorInstanceRef & BaseMatcherHolder::getAccessor()
				//{
				//	return m_accessor;
				//}

				// ---------------------------------
				bool BaseMatcherHolder::resetGuiValues()
				{
					return true;
				}
				// --------------------------------------------------------------------------
				bool BaseMatcherHolder::hasMoreAccNamesTest()const
				{
					return (MORE_ACC == m_testnumberColTest || MORE_BOTH == m_testnumberColTest);
				}
				// --------------------------------------------------------------------------
				bool BaseMatcherHolder::hasMoreGuiNamesTest()const
				{
					return (MORE_GUI == m_testnumberColTest || MORE_BOTH == m_testnumberColTest);
				}
				// --------------------------------------------------------------------------
				bool BaseMatcherHolder::hasMoreAccRowsTest()const
				{
					return (MORE_ACC == m_testnumberRowTest);
				}
				// --------------------------------------------------------------------------
				bool BaseMatcherHolder::hasMoreGuiRowsTest()const
				{
					return (MORE_GUI == m_testnumberRowTest);

				}
				// --------------------------------------------------------------------------
				bool  BaseMatcherHolder::reset()
				{
					bool retval = resetAccessorAndMatcher();
					retval = retval && resetGuiValues();
					return retval;
				}
				// --------------------------------------------------------------------------

			}
		}
	}
}
// ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~  ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~
