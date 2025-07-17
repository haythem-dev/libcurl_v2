/*
* @file                                            
* @author Anke Klink                      
* @date 2008                            
*/    


#include <UnitTest++/UnitTest++.h>
#include "libbasarguitie.h"

#include "libbasardbaspect_accessorpropertytable_yiterator.h"

#include "stringmatcherholdermorelines.h"
#include "testconnection.h"
#include "buttablepropertydescriptions.h"
#include "accessornames.h"
#include "macros.h"
#include "loggerpool.h"
#include "indexcountholder.h"
#include "basematcherholdertestszenario.h"

#pragma warning (push)
#pragma warning(disable: 4481)
#include <QtWidgets/QTableWidget>
#pragma warning (pop)

#include "matcherstringsmorelinestwostringsreversemockcessor.h"
#include "matcherstringsmorelinesstringsreversemockcessor.h"
#include "matcherstringsmorelinestwostringsmockcessor.h"

#include <boost/make_shared.hpp>

using basar::ConstString;
using basar::VarString;

using basar::gui::tie::GuiPropertyTable_YIterator;
using basar::gui::tie::GuiPropertyTableRef;

using basar::db::aspect::AccessorPropertyTable_YIterator;
using basar::db::aspect::AccessorPropertyTableRef;

using basar::test::unit::guitie::BaseMatcherHolder;
using basar::test::unit::guitie::BaseMatcherHolderTestSzenario;
using basar::test::unit::guitie::Direction;

using basar::test::unit::guitie::IndexCountHolder;
using basar::test::unit::guitie::StringMatcherHolderMoreLines;

namespace basar
{
	namespace test
	{
		namespace unit 
		{
			namespace guitie
			{
				// ------------------------------------------------------------------------------
				const VarString StringMatcherHolderMoreLines::csm_propdefstring1 = PROPDEF_STRING1.getName();
				const VarString StringMatcherHolderMoreLines::csm_propdefstring2 = PROPDEF_STRING2.getName();

				basar::ConstString  StringMatcherHolderMoreLines::csm_textarray[15][3] = { 
					{ "A00", "B00", "C00"},
					{ "A01", "B01", "C01"},
					{ "A02", "B02", "C02"},
					{ "A03", "B03", "C03"}, 
					{ "A04", "B04", "C04"},
					{ "A05", "B05", "C05"}, 
					{ "A06", "B06", "C06"},
					{ "A07", "B07", "C07"},
					{ "A08", "B08", "C08"},
					{ "A09", "B09", "C09"},
					{ "A10", "B10", "C10"},
					{ "A11", "B11", "C11"},
					{ "A12", "B12", "C12"},
					{ "A13", "B13", "C13"},
					{ "A14", "B14", "C14"} 
				};

				basar::ConstString  StringMatcherHolderMoreLines::csm_selectresultarray[10][2] = { 
					{ "1" , "COLCH21" } ,
					{ "2" , "COLCH22" } ,
					{ "3" , "COLCH23" } ,
					{ "4" , "COLCH24" } ,
					{ "5" , "COLCH25" } ,
					{ "6" , "COLCH26" } ,
					{ "7" , "COLCH27" } ,
					{ "8" , "COLCH28" } ,
					{ "9" , "COLCH29" } ,
					{ "0" , "COLCH210" } 
				};
				// ------------------------------------------------------------------------------
				StringMatcherHolderMoreLines::StringMatcherHolderMoreLines() 
				{ 
					QStringList list;
					list << csm_propdefstring1.c_str();
					list << csm_propdefstring2.c_str();
					QStringList added;
					added << PROPDEF_STRING3.getName().c_str();

					setStringListNames(list, added);


					m_string1Number = 0;
					m_string2Number =  1;
				}
				// -------------------------------------------------------------------------------
				StringMatcherHolderMoreLines::~StringMatcherHolderMoreLines()
				{
				}
				// -------------------------------------------------------------------------------
				basar::ConstString StringMatcherHolderMoreLines::getText(int row, int col) 
				{
					int ctext = col;
					if (m_inverse)
					{
						if (0 == col)
						{
							ctext = 1;
						}
						else if (1 == col)
						{
							ctext = 0;
						}
					}
					return csm_textarray[row][ctext];
				}
				// -------------------------------------------------------------------------------
				bool StringMatcherHolderMoreLines::initTableWidget()
				{
					bool retval = TableWidgetMatcherHolder::initTableWidget();
					if (m_inverse)
					{
						m_string1Number = 1;
						m_string2Number =  0;
					}
					else
					{
						m_string1Number = 0;
						m_string2Number =  1;
					}	 
					return retval;
				}
				// -------------------------------------------------------------------------------
				bool StringMatcherHolderMoreLines::setAccessorInfo()
				{
					bool retval = false;
					if (m_inverse)
					{
                        if (m_AccessorMap.end() == m_AccessorMap.find(ACCESSORNAME_SELECT_TWOSTRINGS_REVERSE))
                        {
                            basar::db::aspect::IAccessorSharedPtr acc = boost::make_shared< MatcherStringsMoreLinesTwoStringsReverseMockCessor >(ACCESSORNAME_SELECT_TWOSTRINGS_REVERSE);
                            m_AccessorMap.insert(AccessorMap::value_type(ACCESSORNAME_SELECT_TWOSTRINGS_REVERSE, acc));
                        }
                        if (m_AccessorMap.end() == m_AccessorMap.find(ACCESSORNAME_SELECT_STRINGS_REVERSE))
                        {
                            basar::db::aspect::IAccessorSharedPtr acc = boost::make_shared< MatcherStringsMoreLinesStringsReverseMockCessor >(ACCESSORNAME_SELECT_STRINGS_REVERSE);
                            m_AccessorMap.insert(AccessorMap::value_type(ACCESSORNAME_SELECT_STRINGS_REVERSE, acc));
                        }

						retval = BaseMatcherHolder::setAccessorInfo(
							ACCESSORNAME_SELECT_TWOSTRINGS_REVERSE,
							ACCESSORNAME_SELECT_STRINGS_REVERSE,
							ACCESSORMETHODNAME_SELECT_MORELINES,
							ACCESSORMETHODNAME_SELECT_MORELINES,
							false);
                        
					}
					else
					{
                        if (m_AccessorMap.end() == m_AccessorMap.find(ACCESSORNAME_SELECT_TWOSTRINGS))
                        {
                            basar::db::aspect::IAccessorSharedPtr acc = boost::make_shared< MatcherStringsMoreLinesTwoStringsMockCessor >(ACCESSORNAME_SELECT_TWOSTRINGS);
                            m_AccessorMap.insert(AccessorMap::value_type(ACCESSORNAME_SELECT_TWOSTRINGS, acc));
                        }
                        if (m_AccessorMap.end() == m_AccessorMap.find(ACCESSORNAME_SELECT_STRINGS))
                        {
                            basar::db::aspect::IAccessorSharedPtr acc = boost::make_shared<MatcherStringsMoreLinesTwoStringsMockCessor>(ACCESSORNAME_SELECT_STRINGS);
                            m_AccessorMap.insert(AccessorMap::value_type(ACCESSORNAME_SELECT_STRINGS, acc));
                        }

						retval = BaseMatcherHolder::setAccessorInfo( 
							ACCESSORNAME_SELECT_TWOSTRINGS,
							ACCESSORNAME_SELECT_STRINGS,
							ACCESSORMETHODNAME_SELECT_MORELINES,
							ACCESSORMETHODNAME_SELECT_MORELINES,
							false);
					}
					return retval;
				}
				// -------------------------------------------------------------------------------



				// -------------------------------------------------------------------------------
				void StringMatcherHolderMoreLines::checkEqual(
					Direction::DirEnum dir, 
					int row,
					GuiPropertyTable_YIterator & guiiter , 
					AccessorPropertyTable_YIterator & acciter ,
					BaseMatcherHolderTestSzenario & /* test */)
				{
					VarString one = guiiter.getString(csm_propdefstring1);
					VarString two = guiiter.getString(csm_propdefstring2);

					CHECK_EQUAL_DIRECTION(dir, acciter.getString(csm_propdefstring1), one);
					CHECK_EQUAL_DIRECTION(dir, acciter.getString(csm_propdefstring2), two);
					if (Direction::RL == dir)
					{
						CHECK(!(getText(row,m_string1Number) == one));
						CHECK(!(getText(row,m_string2Number) == two));

						CHECK_EQUAL(csm_selectresultarray[row][m_string1Number], one  );
						CHECK_EQUAL(csm_selectresultarray[row][m_string2Number], two  ); 

						CHECK_EQUAL(csm_selectresultarray[row][m_string1Number],  m_tableWidget->item(row, m_string1Number)->text().toStdString()  );
						CHECK_EQUAL(csm_selectresultarray[row][m_string2Number],  m_tableWidget->item(row, m_string2Number)->text().toStdString() );
					}
					else
					{
						CHECK(!(csm_selectresultarray[row][m_string1Number] == one));
						CHECK(!(csm_selectresultarray[row][m_string2Number] == two));

						CHECK_EQUAL(getText(row,m_string1Number), one);
						CHECK_EQUAL(getText(row,m_string2Number),two);
					}
				}

				// -------------------------------------------------------------------------------
				void StringMatcherHolderMoreLines::checkEqualToText(
					int row,
					GuiPropertyTable_YIterator & guiiter , 
					BaseMatcherHolderTestSzenario & /* test */)
				{
					CHECK_EQUAL(getText(row,m_string1Number), guiiter.getString(csm_propdefstring1));
					CHECK_EQUAL(getText(row,m_string2Number), guiiter.getString(csm_propdefstring2));
					CHECK_EQUAL(getText(row,m_string1Number), m_tableWidget->item(row, m_string1Number)->text().toStdString());
					CHECK_EQUAL(getText(row,m_string2Number), m_tableWidget->item(row, m_string2Number)->text().toStdString());
				}

				// -------------------------------------------------------------------------------
				void StringMatcherHolderMoreLines::checkEqualToSelect(
					int row,
					AccessorPropertyTable_YIterator & acciter ,
					BaseMatcherHolderTestSzenario & /* test */)
				{
					CHECK_EQUAL(csm_selectresultarray[row][m_string1Number], acciter.getString(csm_propdefstring1));
					CHECK_EQUAL(csm_selectresultarray[row][m_string2Number], acciter.getString(csm_propdefstring2));
				}


				// -------------------------------------------------------------------------------

				void StringMatcherHolderMoreLines::checkEqualNoMemory(
					Direction::DirEnum dir, 
					int row,
					int abs,
					GuiPropertyTable_YIterator & guiiter , 
					AccessorPropertyTable_YIterator & acciter ,
					BaseMatcherHolderTestSzenario &  test )
				{
					TEXTOUTPUT("chechking row nr. " << row );
					int tableWidgetRow = row-abs;

					if (Direction::RL == dir)
					{
						if (row < abs)
						{
							CHECK_THROW(guiiter.getString(csm_propdefstring1), basar::gui::tie::UnableToWriteInLineOfControlException);
							CHECK_THROW(guiiter.getString(csm_propdefstring2), basar::gui::tie::UnableToWriteInLineOfControlException);
						}
						else // if (row >= abs)
						{
							CHECK_EQUAL(acciter.getString(csm_propdefstring1),  m_tableWidget->item(tableWidgetRow, m_string1Number)->text().toStdString());
							CHECK_EQUAL(acciter.getString(csm_propdefstring2),  m_tableWidget->item(tableWidgetRow, m_string2Number)->text().toStdString());

							CHECK_EQUAL(csm_selectresultarray[row][m_string1Number],  m_tableWidget->item(tableWidgetRow, m_string1Number)->text().toStdString());
							CHECK_EQUAL(csm_selectresultarray[row][m_string2Number],  m_tableWidget->item(tableWidgetRow, m_string2Number)->text().toStdString());

							CHECK_EQUAL_DIRECTION(dir, acciter.getString(csm_propdefstring1), guiiter.getString(csm_propdefstring1));
							CHECK_EQUAL_DIRECTION(dir, acciter.getString(csm_propdefstring2), guiiter.getString(csm_propdefstring2));
						}
					}
					else //(Direction::LR == dir)
					{ 
						if (row < abs)
						{
							checkEqualToSelect(row, acciter,test);
							CHECK_THROW(guiiter.getString(csm_propdefstring1), basar::gui::tie::UnableToWriteInLineOfControlException);
							CHECK_THROW(guiiter.getString(csm_propdefstring2), basar::gui::tie::UnableToWriteInLineOfControlException);
						}
						else  
						{
							VarString one = guiiter.getString(csm_propdefstring1);
							VarString two = guiiter.getString(csm_propdefstring2);

							CHECK_EQUAL_DIRECTION(dir, acciter.getString(csm_propdefstring1), one);
							CHECK_EQUAL_DIRECTION(dir, acciter.getString(csm_propdefstring2), two);
							CHECK(!(csm_selectresultarray[row][m_string1Number] == one));
							CHECK(!(csm_selectresultarray[row][m_string2Number] == two));
							CHECK_EQUAL(getText(tableWidgetRow,m_string1Number),one);
							CHECK_EQUAL(getText(tableWidgetRow,m_string2Number),two);
							CHECK_EQUAL(one, m_tableWidget->item(tableWidgetRow, m_string1Number)->text().toStdString()  );
							CHECK_EQUAL(two, m_tableWidget->item(tableWidgetRow, m_string2Number)->text().toStdString() ); 
						}
					}
				}
				// -------------------------------------------------------------------------------

				void StringMatcherHolderMoreLines::checkInitsOk()
				{
					if (hasMoreGuiNamesTest())
					{
						CHECK_EQUAL(3,	m_guiColCount);
					}
					else
					{
						CHECK_EQUAL(2,	m_guiColCount);
					}
				}
				// -------------------------------------------------------------------------------


	           int StringMatcherHolderMoreLines::calcLinesRightToLeftNoMemory(
					std::map<int, LINE> & linemap,
					const std::vector<IndexCountHolder> & matched,
					int max,
					int tableWidgetRows,
					BaseMatcherHolderTestSzenario &  test)
				{
					int calcAbsnumber = 0;
					int i=0, k=0, j=0;
					for (; i<max; ++i)
					{
						linemap[i] = KEPT;
					}
					std::vector<IndexCountHolder>::const_iterator iter1;
					for (iter1 = matched.begin(); iter1 != matched.end(); ++iter1)
					{
						std::vector<int> vec = iter1->getInnerIndices();
						std::vector<int>::const_iterator iter2;
						for (iter2 = vec.begin(); iter2 != vec.end(); ++iter2)
						{
							i = *iter2;
							linemap[i] = SAME;
							if (test.checkPolicy(basar::gui::tie::INSERT))
							{
								TEXTOUTPUT("hallo insert = "  << test.checkPolicy(basar::gui::tie::INSERT) << ", tableWidgetRows = " << tableWidgetRows);
								if (i < calcAbsnumber)
								{
									for ( k= i + tableWidgetRows; k<calcAbsnumber+tableWidgetRows; ++k)
									{
										linemap[k] = NONE;
									}
									calcAbsnumber = i;
								}
								else if (i >= calcAbsnumber + tableWidgetRows)
								{
									TEXTOUTPUT("hallo i >= calcAbsnumber + tableWidgetRows");
									k = calcAbsnumber + (calcAbsnumber + tableWidgetRows - i) + 1;
									if (k<0) k=0;
									for (j = calcAbsnumber; j< k; ++j)
									{
										linemap[j] = NONE;
									}
									for (j = calcAbsnumber + tableWidgetRows; j< i; ++j)
									{
										linemap[j] = KEPT;
									}
									calcAbsnumber = k;
								}

							}
						}
					}
					return calcAbsnumber;
				}
				// -------------------------------------------------------------------------------
				void StringMatcherHolderMoreLines::checkLinesAfterIndexRightToLeftMatcher(
					const std::vector<IndexCountHolder> & matched,
					int max,
					BaseMatcherHolderTestSzenario &  test ) 
				{
					int realabsnumber = getMatcher().getLeft().getAbsoluteIndex();
					int tableWidgetRows = m_tableWidget->rowCount();
					int calcAbsnumber = 0;
					std::map<int, LINE> linemap;
					int i=0;
					calcAbsnumber = calcLinesRightToLeftNoMemory(linemap, matched, max, tableWidgetRows, test);
					CHECK_EQUAL(calcAbsnumber,realabsnumber);
					// debug:
					if (realabsnumber != calcAbsnumber)
					{
						linemap.clear();
						calcAbsnumber = calcLinesRightToLeftNoMemory(linemap, matched, max, tableWidgetRows, test);
					}

					std::map<int, LINE>::const_iterator iter;
					GuiPropertyTableRef guiprop = getMatcher().getLeft();
					AccessorPropertyTableRef accprop = getMatcher().getRight();
					GuiPropertyTable_YIterator guiiter = guiprop.begin();
					AccessorPropertyTable_YIterator acciter = accprop.begin();
					i=0;
					for (iter = linemap.begin(); iter != linemap.end()&& !acciter.isEnd() && !guiiter.isEnd(); ++iter,++i, ++acciter, ++guiiter)
					{
						TEXTOUTPUT("Line[" << i << "]= " << linemap[i]);
						CHECK_EQUAL(i, iter->first);
						if (NONE == iter->second)
						{
							CHECK_THROW(guiiter.getString(csm_propdefstring1), basar::gui::tie::UnableToWriteInLineOfControlException);
							CHECK_THROW(guiiter.getString(csm_propdefstring2), basar::gui::tie::UnableToWriteInLineOfControlException);
						}
						else if (SAME == iter->second)
						{
							int twRow = i-realabsnumber;
							CHECK(twRow >= 0);
							if (twRow >= 0)
							{
								CHECK_EQUAL(acciter.getString(csm_propdefstring1),  m_tableWidget->item(twRow, m_string1Number)->text().toStdString());
								CHECK_EQUAL(acciter.getString(csm_propdefstring2),  m_tableWidget->item(twRow, m_string2Number)->text().toStdString());

								CHECK_EQUAL(csm_selectresultarray[i][m_string1Number],  m_tableWidget->item(twRow, m_string1Number)->text().toStdString());
								CHECK_EQUAL(csm_selectresultarray[i][m_string2Number],  m_tableWidget->item(twRow, m_string2Number)->text().toStdString());

								CHECK_EQUAL_DIRECTION(Direction::RL, acciter.getString(csm_propdefstring1), guiiter.getString(csm_propdefstring1));
								CHECK_EQUAL_DIRECTION(Direction::RL, acciter.getString(csm_propdefstring2), guiiter.getString(csm_propdefstring2));
							}
						}
						else  // KEEP
						{
							int twRow = i - realabsnumber;
							CHECK(twRow >= 0);
							if (twRow >= 0)
							{
								if (i<tableWidgetRows)
								{
									CHECK_EQUAL(getText(i,m_string1Number), guiiter.getString(csm_propdefstring1));
									CHECK_EQUAL(getText(i,m_string2Number), guiiter.getString(csm_propdefstring2));
									CHECK_EQUAL(getText(i,m_string1Number), m_tableWidget->item(twRow, m_string1Number)->text().toStdString());
									CHECK_EQUAL(getText(i,m_string2Number), m_tableWidget->item(twRow, m_string2Number)->text().toStdString());
								}
								else
								{
									CHECK_EQUAL("", guiiter.getString(csm_propdefstring1));
									CHECK_EQUAL("", guiiter.getString(csm_propdefstring2));
									CHECK_EQUAL("", m_tableWidget->item(twRow, m_string1Number)->text().toStdString());
									CHECK_EQUAL("", m_tableWidget->item(twRow, m_string2Number)->text().toStdString());
								}
							}
						}
					}
				}
				// -----------------------------------------------------------------------------

				int StringMatcherHolderMoreLines::checkAfterLeftToRightIndexCount(
					const IndexCountHolder & indexcount,
  				    BaseMatcherHolderTestSzenario & testiter)
				{
					int maxnumber = testiter.calculateMinRowNumber(Direction::LeftToRight, m_accRowCount, m_guiRowCount,indexcount );
					
					TEXTOUTPUT2(testiter.toText() << " with " << indexcount.toText() << " calculates maxnumber = " << maxnumber);


					GuiPropertyTable_YIterator guiiter = getMatcher().getLeft().begin();
					AccessorPropertyTable_YIterator acciter = getMatcher().getRight().begin();
					int i=0;
					int j=0;
					// nothing changed on the first:
					for (; i<indexcount.m_start && !acciter.isEnd() && !guiiter.isEnd(); ++i)
					{
						checkEqualToSelect(i, acciter , testiter); 
						++acciter;
						++guiiter;
					}
					CHECK_EQUAL(indexcount.m_start, i);
					// the changed values:
					for (; j<indexcount.m_number && !acciter.isEnd() && !guiiter.isEnd(); ++i, ++j)
					{
						checkEqual(Direction::LR, i, guiiter , acciter , testiter);
						++acciter;
						++guiiter;
					}
					CHECK_EQUAL(indexcount.m_number ,j); 
					CHECK_EQUAL(j + indexcount.m_start,i); 
					// nothing changed on the rest:
					for (; i<maxnumber && !acciter.isEnd() && !guiiter.isEnd(); ++i)
					{
						checkEqualToSelect(i,  acciter , testiter);
						++acciter;
						++guiiter;
					}
					CHECK_EQUAL(maxnumber, i); 
					TEXTOUTPUT2(testiter.toText() << " with " << indexcount.toText() << " runs to index " << i << " maxnumber = " << maxnumber);

					return maxnumber;
				}
			}
		}
	}
}
// ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~  ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~
