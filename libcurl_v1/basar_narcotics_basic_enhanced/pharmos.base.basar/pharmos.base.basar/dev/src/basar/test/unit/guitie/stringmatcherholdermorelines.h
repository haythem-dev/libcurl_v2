/** $Id$                                          
*                                                  
* @file                                            
* @author Anke Klink                      
* @date 2008                            
*/    


#ifndef  __BASAR_TEST_UNIT_GUITIE_STRINGMATCHERHOLDERMORELINES_H__ 
#define  __BASAR_TEST_UNIT_GUITIE_STRINGMATCHERHOLDERMORELINES_H__ 

#include "tablewidgetmatcherholder.h"
#include "direction.h"

namespace basar
{
	namespace test
	{
		namespace unit
		{
			namespace guitie
			{
				struct IndexCountHolder;
				class  BaseMatcherHolderTestSzenario;

				// --------------------------------------------------------------------------
				/*!
				* Helperclass for tests in matcherstringmorelines 
				* with one TableWidget
				*/
				class StringMatcherHolderMoreLines : public TableWidgetMatcherHolder
				{  
				public:
					static const VarString csm_propdefstring1;
					static const VarString csm_propdefstring2;

					static basar::ConstString csm_textarray[15][3];
					static basar::ConstString csm_selectresultarray[10][2];

					enum LINE
					{
						NONE,
						SAME,
						KEPT
					} ;

				protected:

					int m_string1Number;
					int m_string2Number;

					virtual basar::ConstString getText(int row, int col) ;
					virtual bool setAccessorInfo();	
					virtual bool initTableWidget();

				public:

					StringMatcherHolderMoreLines();
					~StringMatcherHolderMoreLines();

					int calcLinesRightToLeftNoMemory(
						std::map<int, LINE> & linemap,
						const std::vector<IndexCountHolder> & matched,
						int max,
						int tableWidgetRows,
						BaseMatcherHolderTestSzenario &  test);


					void checkInitsOk();

					void checkEqual(
						Direction::DirEnum dir, 
						int row,
						basar::gui::tie::GuiPropertyTable_YIterator        & guiiter , 
						basar::db::aspect::AccessorPropertyTable_YIterator & acciter ,
						BaseMatcherHolderTestSzenario & /* test */);
					void checkEqualToText(
						int row,
						basar::gui::tie::GuiPropertyTable_YIterator & guiiter , 
						BaseMatcherHolderTestSzenario               & /* test */);

					void checkEqualToSelect(
						int row,
						basar::db::aspect::AccessorPropertyTable_YIterator & acciter ,
						BaseMatcherHolderTestSzenario                      & /* test */);

					void checkEqualNoMemory(
						Direction::DirEnum dir, 
						int row,
						int abs,
						basar::gui::tie::GuiPropertyTable_YIterator        & guiiter , 
						basar::db::aspect::AccessorPropertyTable_YIterator & acciter ,
						BaseMatcherHolderTestSzenario                      & /* test */);


					void checkLinesAfterIndexRightToLeftMatcher(
						const std::vector<IndexCountHolder> & matched,
						int max,
						BaseMatcherHolderTestSzenario & /* test */);

					int checkAfterLeftToRightIndexCount(
						const IndexCountHolder & indexcount,
						BaseMatcherHolderTestSzenario & testiter);

				};
			}
		}
	}
}
#endif 

// ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~  ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~
