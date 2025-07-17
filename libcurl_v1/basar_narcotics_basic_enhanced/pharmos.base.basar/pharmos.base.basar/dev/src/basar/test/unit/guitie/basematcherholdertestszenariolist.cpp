/** $Id$                                          
*                                                  
* @file                                            
* @author Anke Klink                      
* @date 2008                            
*/    



#include <UnitTest++/UnitTest++.h>
#include "libbasarguitie.h"

#include "basematcherholdertestszenariolist.h"

namespace basar
{
    namespace test
    {
        namespace unit
        {
            namespace guitie
            {
                // --------------------------------------------------------------------------
                std::vector<MatcherPolicyType>  BaseMatcherHolderTestSzenarioList::getTestMatcherPolicyTypeVector()
                {
                    std::vector<MatcherPolicyType> policyvec;
                    policyvec.push_back(basar::gui::tie::SKIP);

                    policyvec.push_back(0);
                    policyvec.push_back(basar::gui::tie::INSERT);
                    policyvec.push_back(basar::gui::tie::CONVERTTYPE);
                    policyvec.push_back(basar::gui::tie::DISREGARD_DELETED);

                    policyvec.push_back(basar::gui::tie::INSERT | basar::gui::tie::CONVERTTYPE);
                    policyvec.push_back(basar::gui::tie::INSERT | basar::gui::tie::SKIP);
                    policyvec.push_back(basar::gui::tie::SKIP | basar::gui::tie::CONVERTTYPE);

                    policyvec.push_back(basar::gui::tie::CONVERTTYPE | basar::gui::tie::INSERT | basar::gui::tie::SKIP);

                    return policyvec;
                }
                // --------------------------------------------------------------------------
                void  BaseMatcherHolderTestSzenarioList::addColTests(
                    std::vector<BaseMatcherHolderTestSzenario> & retval ,
                    BaseMatcherHolder::TestNumber rowtest,

                    bool usePolicy,
                    MatcherPolicyType policy)
                {                    
                    retval.push_back(BaseMatcherHolderTestSzenario( BaseMatcherHolder::EQUAL,          rowtest, usePolicy, policy));   
                    retval.push_back(BaseMatcherHolderTestSzenario( BaseMatcherHolder::MORE_GUI, rowtest, usePolicy, policy));
                    retval.push_back(BaseMatcherHolderTestSzenario( BaseMatcherHolder::MORE_ACC, rowtest, usePolicy, policy));
                    retval.push_back(BaseMatcherHolderTestSzenario( BaseMatcherHolder::MORE_BOTH,     rowtest, usePolicy, policy));
                }

                // --------------------------------------------------------------------------
                void  BaseMatcherHolderTestSzenarioList::addRowTests(
                    std::vector<BaseMatcherHolderTestSzenario> & retval ,
                    BaseMatcherHolder::TestNumber coltest,
                    bool usePolicy,
                    MatcherPolicyType policy)
                {                    
                    retval.push_back(BaseMatcherHolderTestSzenario(coltest, BaseMatcherHolder::EQUAL,     usePolicy, policy));   
                    retval.push_back(BaseMatcherHolderTestSzenario(coltest, BaseMatcherHolder::MORE_GUI,  usePolicy, policy));
                    retval.push_back(BaseMatcherHolderTestSzenario(coltest, BaseMatcherHolder::MORE_ACC,  usePolicy, policy));
                }



                // --------------------------------------------------------------------------

                void BaseMatcherHolderTestSzenarioList::addAllTests(
                    std::vector<BaseMatcherHolderTestSzenario> & retval,
                    bool usePolicy,
                    MatcherPolicyType policy)
                {
                    addColTests(retval, BaseMatcherHolder::EQUAL,         usePolicy, policy); 
                    addColTests(retval, BaseMatcherHolder::MORE_GUI,usePolicy, policy); 
                    addColTests(retval, BaseMatcherHolder::MORE_ACC,usePolicy, policy); 
                }
                // --------------------------------------------------------------------------
                std::vector<BaseMatcherHolderTestSzenario> BaseMatcherHolderTestSzenarioList::getBaseTestSzenarios()
                {
                    std::vector<BaseMatcherHolderTestSzenario> retval;
                    addAllTests(retval, false, 0);
                    return retval;
                }
                // --------------------------------------------------------------------------
                std::vector<BaseMatcherHolderTestSzenario> BaseMatcherHolderTestSzenarioList::getPolicyTestSzenarios()
                {
                    std::vector<BaseMatcherHolderTestSzenario> retval = getBaseTestSzenarios();
                    std::vector<MatcherPolicyType> policyvec = getTestMatcherPolicyTypeVector();
                    std::vector<MatcherPolicyType>::const_iterator policyiter;
                    for (policyiter = policyvec.begin(); policyiter != policyvec.end(); ++policyiter)
                    {
                        addAllTests(retval, true, *policyiter);
                    }
                    return retval;
                }
                // --------------------------------------------------------------------------
                std::vector<BaseMatcherHolderTestSzenario> BaseMatcherHolderTestSzenarioList::getBaseTestSzenariosOneRowTest(BaseMatcherHolder::TestNumber number)
                {
                    std::vector<BaseMatcherHolderTestSzenario> retval;
                    addColTests(retval, number ,false, 0);
                    return retval;

                }
                // --------------------------------------------------------------------------
                std::vector<BaseMatcherHolderTestSzenario> BaseMatcherHolderTestSzenarioList::getPolicyTestSzenariosOneRowTest(BaseMatcherHolder::TestNumber number)
                {
                    std::vector<BaseMatcherHolderTestSzenario> retval = getBaseTestSzenariosOneRowTest(number);
                    std::vector<MatcherPolicyType> policyvec = getTestMatcherPolicyTypeVector();
                    std::vector<MatcherPolicyType>::const_iterator policyiter;
                    for (policyiter = policyvec.begin(); policyiter != policyvec.end(); ++policyiter)
                    {
                        addColTests(retval, number, true, *policyiter);
                    }
                    return retval;

                }
                // --------------------------------------------------------------------------
                std::vector<BaseMatcherHolderTestSzenario> BaseMatcherHolderTestSzenarioList::getBaseTestSzenariosOneColTest(BaseMatcherHolder::TestNumber number)
                {
                    std::vector<BaseMatcherHolderTestSzenario> retval;
                    addRowTests(retval, number ,false, 0);
                    return retval;
                }
                // --------------------------------------------------------------------------
                std::vector<BaseMatcherHolderTestSzenario> BaseMatcherHolderTestSzenarioList::getPolicyTestSzenariosOneColTest(BaseMatcherHolder::TestNumber number)
                {
                    std::vector<BaseMatcherHolderTestSzenario> retval = getBaseTestSzenariosOneColTest(number);
                    std::vector<MatcherPolicyType> policyvec = getTestMatcherPolicyTypeVector();
                    std::vector<MatcherPolicyType>::const_iterator policyiter;
                    for (policyiter = policyvec.begin(); policyiter != policyvec.end(); ++policyiter)
                    {
                        addRowTests(retval, number, true, *policyiter);
                    }
                    return retval;

                }
                // --------------------------------------------------------------------------
                std::vector<BaseMatcherHolderTestSzenario> 
                    BaseMatcherHolderTestSzenarioList::getBaseTestSzenarios(
                    BaseMatcherHolder::TestNumber col, 
                    BaseMatcherHolder::TestNumber row)
                {

                    std::vector<BaseMatcherHolderTestSzenario> retval;
                    retval.push_back(BaseMatcherHolderTestSzenario(col, row,     false, 0));   

                    return retval;

                }
                // --------------------------------------------------------------------------
                std::vector<BaseMatcherHolderTestSzenario> 
                    BaseMatcherHolderTestSzenarioList::getPolicyTestSzenarios(
                    BaseMatcherHolder::TestNumber col, 
                    BaseMatcherHolder::TestNumber row)
                {
                    std::vector<BaseMatcherHolderTestSzenario> retval = getBaseTestSzenarios(col, row);
                    std::vector<MatcherPolicyType> policyvec = getTestMatcherPolicyTypeVector();
                    std::vector<MatcherPolicyType>::const_iterator policyiter;
                    for (policyiter = policyvec.begin(); policyiter != policyvec.end(); ++policyiter)
                    {
                        retval.push_back(BaseMatcherHolderTestSzenario(col, row,     true, *policyiter));   

                    }
                    return retval;
                }
                // --------------------------------------------------------------------------
            std::vector<BaseMatcherHolderTestSzenario> 
						BaseMatcherHolderTestSzenarioList::getInsertableOnlyPolicyTestSzenarios(BaseMatcherHolder::TestNumber row, BaseMatcherHolder::TestNumber col)
			{
					std::vector<BaseMatcherHolderTestSzenario> retval = getBaseTestSzenarios(col, row);
                    
					std::vector<MatcherPolicyType> policyvec = getTestMatcherPolicyTypeVector();
                    
					std::vector<MatcherPolicyType>::const_iterator policyiter;
                    for (policyiter = policyvec.begin(); policyiter != policyvec.end(); ++policyiter)
                    {
						if (BaseMatcherHolderTestSzenario::checkPolicy(*policyiter, basar::gui::tie::INSERT))
						{ 
                          retval.push_back(BaseMatcherHolderTestSzenario(col, row,     true, *policyiter));   
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
