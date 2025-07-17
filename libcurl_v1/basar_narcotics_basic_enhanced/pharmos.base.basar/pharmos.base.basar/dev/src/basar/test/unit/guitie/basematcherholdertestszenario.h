/** $Id$                                          
*                                                  
* @file                                            
* @author Anke Klink                      
* @date 2008                            
*/    

#ifndef  __BASAR_TEST_UNIT_GUITIE_BASEMATCHERHOLDERSZENARIO_H__ 
#define  __BASAR_TEST_UNIT_GUITIE_BASEMATCHERHOLDERSZENARIO_H__ 

#include "direction.h"
#include "basematcherholder.h"

namespace basar
{
    namespace test
    {
        namespace unit
        {
            namespace guitie
            {
                typedef basar::gui::tie::MatcherPolicyType MatcherPolicyType;
                typedef basar::gui::tie::MatcherPolicyEnum MatcherPolicyEnum;
				
				struct IndexCountHolder;

				/**
				 * Test Szenario, to test a BaseMatcherHolder object
				 */
                class BaseMatcherHolderTestSzenario 
                {
						/*!
					*  Specfies the actual test, that is  done for columns :
					* <ul>
					*   <li> NORM/EQUAL: the columns in the AccessorpropertyTable and the Guipropertytable 
					*                        are defined identical (same names and types) </li>
					*        
					*   <li> MORE_GUI:  all the columns in the AccessorpropertyTable are identical 
					*                        defined in the Guipropertytable,
					*                        but there is one (or more) Columns in the Guipropertytable</li>
					*       
					*   <li> MORE_ACC: all the columns in the Guipropertytable are identical 
					*                        defined in the AccessorpropertyTable,
					*                        but there is one (or more) Columns in the AccessorpropertyTable</li>
					*   <li> MORE_BOTH: not all the columns in the Guipropertytable are  
					*                        defined in the AccessorpropertyTable,
					*                        there is one (or more) Columns defined in the AccessorpropertyTable and
					*                        one (or more) Columns in the Guipropertytable</li>
					* </ul>
					*/
                    BaseMatcherHolder::TestNumber m_testnumberColTest;

					/*!
					* Specfies the actual test, that is  done forrows
					* <ul>
					*   <li> NORM/EQUAL: the numb er of rows in the Accessor PropertyTable and the Guiproperty table identical </li>     
					*   <li> MORE_GUI: there are more lines in the  Guipropertytable than in the Accessorpropertytable</li>
					*      
					*   <li> MORE_ACC: there are more lines in the  Guipropertytable than in the Accessorpropertytable</li>
					*   <li> MORE_BOTH: not possible, as the rows are not named</li>
					* </ul>
					*/
                    BaseMatcherHolder::TestNumber m_testnumberRowTest;

					/**
					 * The Matcher Policy that is to be tested
					 */
                    MatcherPolicyType m_policy;

					/** 
					* is a special policy tested (m_policy), 
					* or the matchers own default Policy
					*/
                    bool m_usePolicy;

					/**
					 * has the test started
					 */
                    bool m_started;

					/** 
					* a test name used in Error Messages and Test Messages
					*/
                    VarString m_testname;

				private:
					/**
					 * returns if the policy SKIP is necessary in orde to 
					 * not get Exceptions at Matcherfunctions RightToLeft or LeftToRight
					 * returns the value(BaseMatcherHolder::EQUAL != m_testnumberColTest)
					 * and is used for the throwsToManyEntriesException(...)
					 * functions
					 */
                    bool needsCheckSkip();

                public:

					/**
					 * Constructor, with the Testnumbers and the policy to be tested 
					 */ 
                    BaseMatcherHolderTestSzenario(
                        BaseMatcherHolder::TestNumber nc ,
                        BaseMatcherHolder::TestNumber nr ,
                        bool usePolicy,
                        MatcherPolicyType policy);

					/**
					 * returns the Text associated with the TestNumber for Error and 
					 * Test Messages and for the method toText()
					 */
                    static VarString getTestNumberText(BaseMatcherHolder::TestNumber);

					/** 
					* returns a string that defines the Test
					* f.ex. "Szenario (GuiTieTest_Matcher_ConvertToStrings_RightToLeft_NoVar_withMemory) with (coltest=EQUAL/NORM,rowtest=MORE_ACC  ) and  with policy 4 "
					* or: "Szenario (GuiTieTest_Matcher_ConvertToStrings_LeftToRight_int_withMemoryine) with (coltest=EQUAL/NORM,rowtest=EQUAL/NORM) and no policy"
					*/
                    basar::VarString toText();  

					/**
					 * checks if the MatcherPolicyEnum p is set.
					 */
                    bool checkPolicy(MatcherPolicyEnum p);

					/**
					 * checks if the MatcherPolicyEnum p is part of the MatcherPolicyType policy.
					 */
                    static bool checkPolicy(MatcherPolicyType policy, MatcherPolicyEnum p);

					/**
					 * returns is the Test with throw an Exception, because there are to
					 * many Entries, that means the policy SKIP is not set and the Test has
					 * the Column Testnumber (MORE_ACC, MORE_GUI or MORE_BOTH)
					 */
                    bool throwsToManyEntriesException();

					/**
					 * returns is the Test with throw an Exception with this Policy (policy), 
					 * because there are to
					 * many Entries, that means the policy SKIP is not part of the policy parameter 
					 * and the Test has
					 * the Column Testnumber (MORE_ACC, MORE_GUI or MORE_BOTH)
					 */
                    bool throwsToManyEntriesException( MatcherPolicyType policy);

					/**
					 * returns is an Exception is thrown becouse the values can not be 
					 * converted in Type, that means that the policy CONVERT is not 
					 * set.
					 */
                    bool throwsConvertException();

					/**
					 * returns is an Exception is thrown becouse the values can not be 
					 * converted in Type, that means that the policy CONVERT is not 
					 * in parameter policy.
					 */
                    bool throwsConvertException( MatcherPolicyType policy);

					/**
					 * returns the Minumum Number of Rows that are matched in the Direction dir
					 * when there are acc number of rows in the Accessor PropertyTable and
					 * gui number of rows in the Gui PropertyTable
					 */
                    int calculateMinRowNumber(Direction::DirEnum dir,  int acc, int gui);

					/**
					 * returns the Minumum Number of Rows that are matched from index to count
					 * defined in the struct indexcountholder
					 * in the Direction dir
					 * when there are acc number of rows in the Accessor PropertyTable and
					 * gui number of rows in the Gui PropertyTable 
					 * f.ex. 
					 * <ul> <li> 
					 * dir = RL,
					 * acc = 20,
					 * gui = 20,
					 * indexcountholder.m_index = 0
                     * indexcountholder.m_count = 4
					 * returns 4 </li> <li> 
					 * dir = RL,
					 * acc = 1,
					 * gui = 1,
					 * indexcountholder.m_index = 0
                     * indexcountholder.m_count = 2
					 * returns 1</li>
					 * </ul>
					 */
                    int calculateMinRowNumber(
						Direction::DirEnum dir,  
						int acc, 
						int gui ,
						const IndexCountHolder & indexcountholder);

				

					/**
					 * returns the Minumum Number of Rows that are matched in the Direction dir
					 * when there are acc number of rows in the Accessor PropertyTable and
					 * gui number of rows in the Gui PropertyTable
					 */
                    int calculateMinRowNumber(Direction::DirEnum, MatcherPolicyType policy, int acc, int gui);
	
					/**
					 * calculates the Absolut Index of a GuiPropertyTable with NoMemory Policy
					 */
						int calculateAbsoluteIndex( 
						basar::gui::tie::MemoryPolicyEnum p,
						Direction::DirEnum dir,  
						int acc, 
						int gui );

					/**
					 * starts the Test with the Testname and the used BaseMatcherHolder Object
					 */
                    bool start(basar::ConstString name, BaseMatcherHolder & holder);

					/**
					 * starts the Test with the Testname and the used BaseMatcherHolder Object
					 */
                    bool restart(BaseMatcherHolder & holder);

                };

            }
        }
    }
}
#endif 

// ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~  ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~
