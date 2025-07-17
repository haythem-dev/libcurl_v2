/** $Id$
*
* @file
* @author Anke Klink
* @date 2008
*/

#include <UnitTest++/UnitTest++.h>
#include "libbasarguitie.h"

#include "basematcherholdertestszenario.h"

#include "indexcountholder.h"

namespace basar
{
    namespace test
    {
        namespace unit
        {
            namespace guitie
            {
                // --------------------------------------------------------------------------
                BaseMatcherHolderTestSzenario::BaseMatcherHolderTestSzenario(
                    BaseMatcherHolder::TestNumber nc ,
                    BaseMatcherHolder::TestNumber nr ,
                    bool usePolicy,
                    MatcherPolicyType policy):
                m_testnumberColTest(nc),
                    m_testnumberRowTest(nr),
                    m_policy(policy),
                    m_usePolicy(usePolicy),
                    m_started(false)
                {

                }
                // --------------------------------------------------------------------------
                VarString  BaseMatcherHolderTestSzenario::getTestNumberText(BaseMatcherHolder::TestNumber testnumber)
                {
                    VarString result; 
                    if (BaseMatcherHolder::EQUAL == testnumber)
                    {
                        result = "EQUAL/NORM";
                    }
                    else if (BaseMatcherHolder::MORE_GUI == testnumber)
                    {
                        result = "MORE_GUI  ";
                    }
                    else if (BaseMatcherHolder::MORE_ACC == testnumber)
                    {
                        result = "MORE_ACC  ";
                    }
                    else if (BaseMatcherHolder::MORE_BOTH == testnumber)
                    {
                        result = "MORE_BOTH ";
                    }
                    return result;
                }

                // --------------------------------------------------------------------------
                basar::VarString  BaseMatcherHolderTestSzenario::toText()
                {
                    VarString retval;
                    VarString policytext = "no policy ";
                    if (m_usePolicy )
                    {
                        policytext.format(" with policy %d ", m_policy);
						if (checkPolicy(m_policy, basar::gui::tie::INSERT))
						{
							policytext.append("-INSERT-");
						}
						if (checkPolicy(m_policy, basar::gui::tie::SKIP))
						{
							policytext.append("-SKIP-");
						}
						if (checkPolicy(m_policy, basar::gui::tie::CONVERTTYPE))
						{
							policytext.append("-CONVERTTYPE-");
						}
                    }
                    retval.format("Szenario (%s) with (coltest=%s,rowtest=%s) and %s", 
                        m_testname.c_str(), 
                        getTestNumberText(m_testnumberColTest).c_str(), 
                        getTestNumberText(m_testnumberRowTest).c_str(), 
                        policytext.c_str());
                    return retval;
                }

                // --------------------------------------------------------------------------
                bool  BaseMatcherHolderTestSzenario::start(
                    basar::ConstString  name, 
                    BaseMatcherHolder & holder)
                {
                        m_testname = name;
                       return restart(holder);
                }

                // --------------------------------------------------------------------------
                bool BaseMatcherHolderTestSzenario::restart(BaseMatcherHolder & holder)
				{
					m_started = holder.setTestNumber(m_testnumberColTest,m_testnumberRowTest);
                    if (m_started)
                    {
                        m_started = holder.start();

                        if (m_usePolicy && m_started)
                        {
                            holder.getMatcher().setMatchingPolicy(m_policy);
                        }
                    }

					return m_started;
				}

                // --------------------------------------------------------------------------
                bool BaseMatcherHolderTestSzenario::checkPolicy(MatcherPolicyType policy, MatcherPolicyEnum e )
                {
                    return (e  == (policy & e ));
                }

                // --------------------------------------------------------------------------
                bool BaseMatcherHolderTestSzenario::checkPolicy(MatcherPolicyEnum e)
                {
                    bool retval = true;
                    if (m_usePolicy)
                    {
                        retval = checkPolicy(m_policy, e);
                    }
                    else if (basar::gui::tie::DISREGARD_DELETED == e)
                    {
                        // standard is CONVERTTYPE | INSERT | SKIP
                        retval = false;
                    }
                    return retval;
                }

                // --------------------------------------------------------------------------
                bool  BaseMatcherHolderTestSzenario::needsCheckSkip()
                {
                    return (BaseMatcherHolder::EQUAL != m_testnumberColTest);
                }
                // --------------------------------------------------------------------------
                bool BaseMatcherHolderTestSzenario::throwsToManyEntriesException()
                {
                    bool retval = false;
                    if (needsCheckSkip())
                    {
                        retval = !(checkPolicy(basar::gui::tie::SKIP));
                    }
                    return retval;
                }

                // --------------------------------------------------------------------------
                bool BaseMatcherHolderTestSzenario::throwsToManyEntriesException( 
                    MatcherPolicyType policy)
                {
                    bool retval = false;
                    if (needsCheckSkip())
                    {
                        retval = !(checkPolicy(policy, basar::gui::tie::SKIP));
                    }
                    return retval;
                }
                // --------------------------------------------------------------------------
                bool BaseMatcherHolderTestSzenario::throwsConvertException() 
                {
                    bool retval = true;
                    if (checkPolicy(basar::gui::tie::CONVERTTYPE))
                    {
                        retval = false;
                    }
                    else if (checkPolicy(basar::gui::tie::SKIP))
                    {
                        retval = false;
                    }
                    return retval;
                }
                // --------------------------------------------------------------------------

                    bool BaseMatcherHolderTestSzenario::throwsConvertException( MatcherPolicyType policy) 
					{
						 bool retval = true;
                    if (checkPolicy(policy,basar::gui::tie::CONVERTTYPE))
                    {
                        retval = false;
                    }
                    else if (checkPolicy(policy,basar::gui::tie::SKIP))
                    {
                        retval = false;
                    }
                    return retval;
					}


                // --------------------------------------------------------------------------
                int BaseMatcherHolderTestSzenario::calculateMinRowNumber( 
                    Direction::DirEnum dir, 
                    int acc, 
                    int gui) 
                {
                    int retval = 0;
                    if (m_usePolicy)
                    {
                        retval = calculateMinRowNumber(dir,m_policy,acc, gui);
                    }
                    else
                    {
                        retval = calculateMinRowNumber(dir,basar::gui::tie::INSERT,acc, gui);
                    }
                    return retval;
                }
                // --------------------------------------------------------------------------
                int BaseMatcherHolderTestSzenario::calculateMinRowNumber( 
                    Direction::DirEnum dir, 
                    int acc, 
                    int gui, 
                    const IndexCountHolder & indexcountholder) 
                {
                    int retval =  calculateMinRowNumber(dir,acc, gui);
					int formerMax =  gui;
					if (Direction::LR == dir)
					{
						formerMax = acc;
					}
                    if (retval > formerMax)
                    {
                        int last = indexcountholder.m_start + indexcountholder.m_number ;
                        if (last < retval)
                        {
                            retval = (formerMax>last) ? formerMax : last;
                        }
                    }
                    return retval;
                }
                // --------------------------------------------------------------------------

                int BaseMatcherHolderTestSzenario::calculateMinRowNumber( 
                    Direction::DirEnum dir,  
                    MatcherPolicyType policy, 
                    int acc, 
                    int gui)
                {
                    int retval = acc;
                    if (gui < acc)
                    {
                        retval = gui;
                        if (Direction::RightToLeft == dir && 
                            checkPolicy(policy, basar::gui::tie::INSERT))
                        {
                            retval = acc;
                        }
                    }
                    else if (acc < gui) 
                    {
                        retval = acc;
                        if (Direction::LeftToRight == dir && 
                            checkPolicy(policy, basar::gui::tie::INSERT))
                        {
                            retval = gui;
                        }
                    }
                    return retval;
                }
                // --------------------------------------------------------------------------
					int BaseMatcherHolderTestSzenario::calculateAbsoluteIndex( 
						basar::gui::tie::MemoryPolicyEnum p,
						Direction::DirEnum dir,  
						int acc, 
						int gui )
					{
						int retval = 0;
						if (basar::gui::tie::WITHOUT_MEMORY == p)
						{
						   if (Direction::RightToLeft == dir && 
							   acc > gui &&
                               checkPolicy(basar::gui::tie::INSERT))
                          {
                             retval = (acc - gui);
                           }
						}
						return retval;
					}

                // --------------------------------------------------------------------------

            }
        }
    }
}
// ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~  ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~
