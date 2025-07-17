/** $Id$                                          
*                                                  
* @file                                            
* @author Anke Klink                      
* @date 2008                            
*/    


#ifndef  __BASAR_TEST_UNIT_GUITIE_BASEMATCHERHOLDERTESTSZENARIOLIST_H__ 
#define  __BASAR_TEST_UNIT_GUITIE_BASEMATCHERHOLDERTESTSZENARIOLIST_H__ 

#include <vector>
#include "basematcherholder.h"
#include "basematcherholdertestszenario.h"

namespace basar
{
	namespace test
	{
		namespace unit
		{
			namespace guitie
			{
				class BaseMatcherHolderTestSzenario;


				/**
				* returns a Suite (as vector) of BaseMatcherHolderTestSzenario Tests
				*/ 
				class BaseMatcherHolderTestSzenarioList 
				{    
					/**
					* adds all Row and Column Tests for one policy szenario
					*/
					static void addAllTests(
						std::vector<BaseMatcherHolderTestSzenario> & ,
						bool usePolicy,
						MatcherPolicyType policy);

					/**
					* adds all Column Tests for one policy/RowTest szenario
					*/
					static void addColTests(
						std::vector<BaseMatcherHolderTestSzenario> & ,
						BaseMatcherHolder::TestNumber rowtest,
						bool usePolicy,
						MatcherPolicyType policy);

					/**
					* adds all Row Tests for one policy/ColumnTest szenario
					*/
					static void addRowTests(
						std::vector<BaseMatcherHolderTestSzenario> & ,
						BaseMatcherHolder::TestNumber rowtest,
						bool usePolicy,
						MatcherPolicyType policy);
				public:                    

					/** 
					* returns a vector of all Policies that are to be tested
					*/
					static std::vector<MatcherPolicyType> getTestMatcherPolicyTypeVector();

					/**
					* returns all Tests (rows and columns Tests) with the default Policy 
					*/
					static std::vector<BaseMatcherHolderTestSzenario> getBaseTestSzenarios();

					/**
					* returns the BaseTestSzenarios and all rows and columns Tests for
					* all the Policies listed in getTestMatcherPolicyTypeVector
					*/
					static std::vector<BaseMatcherHolderTestSzenario> getPolicyTestSzenarios();

					/**
					* returns all columns Tests for one rows Test with the default Policy 
					*/
					static std::vector<BaseMatcherHolderTestSzenario> 
						getBaseTestSzenariosOneRowTest(BaseMatcherHolder::TestNumber);

					/**
					* returns all columns Tests and  Policy Tests for one row Test  
					*/
					static std::vector<BaseMatcherHolderTestSzenario> 
						getPolicyTestSzenariosOneRowTest(BaseMatcherHolder::TestNumber);

					/**
					* returns all row Tests for one column Test and with the default Policy 
					*/
					static std::vector<BaseMatcherHolderTestSzenario> 
						getBaseTestSzenariosOneColTest(BaseMatcherHolder::TestNumber);

					/**
					* returns all rows Tests and  Policy Tests for one columns Test  
					*/
					static std::vector<BaseMatcherHolderTestSzenario> 
						getPolicyTestSzenariosOneColTest(BaseMatcherHolder::TestNumber);

					/**
					* returns one TestSzenario with default Policy
					*/
					static std::vector<BaseMatcherHolderTestSzenario> 
						getBaseTestSzenarios(BaseMatcherHolder::TestNumber row, BaseMatcherHolder::TestNumber col);

					/**
					* returns allPolicy Tests for one Row and Column Test
					*/
					static std::vector<BaseMatcherHolderTestSzenario> 
						getPolicyTestSzenarios(BaseMatcherHolder::TestNumber row, BaseMatcherHolder::TestNumber col);
				
					/**
					* returns allPolicy Tests for one Row and Column Test
					*/
					static std::vector<BaseMatcherHolderTestSzenario> 
						getInsertableOnlyPolicyTestSzenarios(BaseMatcherHolder::TestNumber row, BaseMatcherHolder::TestNumber col);
				
				}; 

			}
		}
	}
}
#endif 

// ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~  ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~
