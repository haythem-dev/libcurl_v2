/** $Id$                                          
*                                                  
* @file                                            
* @author Anke Klink                      
* @date 2008                            
*/ 

#include <UnitTest++/UnitTest++.h>
#include <libbasarcmnutil.h>
#include <libbasardbaspect.h>
#include <libbasarappl.h> 
#include <libbasarguitie.h>
#include <libbasardbaspect_usings.h>
#include <libbasarqtwidget_tablewidget.h>


#pragma warning (push)
#pragma warning(disable: 4127 4244 4251 4311 4312 4512 4800)
#include <QT/QWidget.h>
#include <QT/QLabel.h>
#pragma warning (pop)

#include "selects.h"
#include "buttablepropertydescriptions.h"
#include "buttablepropertydescriptionvectors.h"
#include "accessornames.h" 
#include "basematcherholder.h"
#include "basematcherholdertestszenario.h"
#include "basematcherholdertestszenariolist.h"
#include "indexcountholder.h"
#include "matcherholderlist.h"
#include "macros.h"
#include "stringmatcherholdermorelines.h"
#include "loggerpool.h"

using basar::ConstString;
using basar::VarString;

using basar::gui::tie::MatcherRef;
using basar::gui::tie::GuiPropertyTable_YIterator;
using basar::gui::tie::GuiPropertyTableRef;

using basar::db::aspect::AccessorInstanceRef;
using basar::db::aspect::ConnectionRef;
using basar::db::aspect::AccessorPropertyTable_YIterator;
using basar::db::aspect::AccessorPropertyTableRef;
using basar::db::aspect::ExecuteResultInfo;

using basar::test::unit::guitie::TestConnection;
using basar::test::unit::guitie::BaseMatcherHolder;
using basar::test::unit::guitie::BaseMatcherHolderTestSzenario;
using basar::test::unit::guitie::MatcherHolderList;
using basar::test::unit::guitie::Direction;

using basar::test::unit::guitie::BaseMatcherHolderTestSzenarioList;
using basar::test::unit::guitie::IndexCountHolder;
using basar::test::unit::guitie::StringMatcherHolderMoreLines;



//------------------------------------------------------------------------------
SUITE(GuiTieTest_Matcher_Strings_NoMemory_MoreLines_underconstruction)
{
	typedef basar::gui::tie::MatcherPolicyType MatcherPolicyType;

	// ------------------------------------------------------------------------------------------------------------------------------------------------
		TEST_FIXTURE(StringMatcherHolderMoreLines, GuiTieTest_Matcher_Strings_MoreLines_NoMemory_LeftToRight_int_withoutMemory_2)
	{
		std::vector<TableWidgetKind> tableWidgetKindVec = getTableWidgetKindVector();
		std::vector<TableWidgetKind>::const_iterator tableWidgetKindIter;
		for(tableWidgetKindIter = tableWidgetKindVec.begin(); tableWidgetKindIter != tableWidgetKindVec.end(); ++tableWidgetKindIter)
		{
			try
			{
				CHECK(initTableWidgetKind(*tableWidgetKindIter));
				GuiPropertyTableRef guiprop = getMatcher().getLeft();
				AccessorPropertyTableRef accprop = getMatcher().getRight();
				GuiPropertyTable_YIterator guiiter = guiprop.begin();
				AccessorPropertyTable_YIterator acciter = accprop.begin();

				std::vector<BaseMatcherHolderTestSzenario> tests = BaseMatcherHolderTestSzenarioList::getPolicyTestSzenarios();
				std::vector<BaseMatcherHolderTestSzenario>::iterator testiter;
				for (testiter = tests.begin(); testiter != tests.end(); ++testiter)
				{
					try
					{
						CHECK(testiter->start("GuiTieTest_Matcher_Strings_MoreLines_NoMemory_LeftToRight_int_withoutMemoryine", *this));
						TEXTOUTPUT(testiter->toText());

						CHECK(select());

						guiprop = getMatcher().getLeft();
						accprop = getMatcher().getRight();
						CHECK_EQUAL(0,guiprop.getAbsoluteIndex());

						CHECK_EQUAL(m_accRowCount, static_cast<int>(accprop.size()));
						CHECK_EQUAL(m_guiRowCount, static_cast<int>(guiprop.size()));

						guiprop.setMemoryPolicy(basar::gui::tie::WITHOUT_MEMORY);


						if (testiter->throwsToManyEntriesException())
						{
							CHECK_THROW(getMatcher().LeftToRight(0, 1), basar::Exception);
							CHECK_THROW(getMatcher().LeftToRight(4, 7), basar::Exception);
						}
						else
						{
							//std::cout << "hallo " << testiter->toText() << std::endl;

							if (!testiter->checkPolicy(basar::gui::tie::INSERT) && 
								getRowTestNumber() == MORE_GUI)
							{
								CHECK_EQUAL(0,getMatcher().LeftToRight(27, 1));
								CHECK_EQUAL(0,getMatcher().LeftToRight(27, 0));
								CHECK_EQUAL(0,getMatcher().LeftToRight(27, 2));
							}
							else
							{
								CHECK_THROW(getMatcher().LeftToRight(27, 1), basar::Exception); 
								CHECK_THROW(getMatcher().LeftToRight(27, 0), basar::Exception); 
								CHECK_THROW(getMatcher().LeftToRight(27, 2), basar::Exception); 
							}
						
							// various, working tests
							{
								int maxnumber = testiter->calculateMinRowNumber(Direction::LeftToRight, m_accRowCount, m_guiRowCount);
								std::vector<IndexCountHolder> indexcountlist;
								indexcountlist.push_back(IndexCountHolder(1,-2, maxnumber));
								//indexcountlist.push_back(IndexCountHolder(1,1, maxnumber)); 
								//indexcountlist.push_back(IndexCountHolder(1,-1, maxnumber));  

								std::vector<IndexCountHolder>::const_iterator indexcount;
								for (indexcount = indexcountlist.begin(); indexcount != indexcountlist.end(); ++indexcount)
								{     
									try
									{
										reset();
										testiter->restart(*this);
										CHECK(select());
										CHECK_EQUAL(indexcount->m_number,getMatcher().LeftToRight(indexcount->m_index, indexcount->m_count));
										getMatcher().LeftToRight(indexcount->m_index, indexcount->m_count);
										//if (maxnumber > m_guiRowCount) m_guiRowCount = maxnumber;
										checkAfterLeftToRightIndexCount(testResults_,m_details,*indexcount,*testiter);
									}
									catch (basar::Exception & ex)
									{
										std::cout << " Exception in" << testiter->toText()
											<< " (index,count,start,number)=(" << indexcount->m_index << "," << indexcount->m_count << "," << indexcount->m_start << "," <<indexcount->m_number << ")" << std::endl 
											<< ex.what() << std::endl;
										const bool ExceptionOccured3 = false;
										CHECK(ExceptionOccured3);
									}
									catch (...)
									{
										std::cout << "Non Basar Exception in" << testiter->toText()
											<< " (index,count,start,number)=(" << indexcount->m_index << "," << indexcount->m_count << "," << indexcount->m_start << "," <<indexcount->m_number << ")" << std::endl 
											<< std::endl;
										const bool NonBasarExceptionOccured3 = false;
										CHECK(NonBasarExceptionOccured3);

									}
								}
							}
						}
					}
					catch (basar::Exception & ex)
					{
						std::cout << "Exception in" << testiter->toText() << std::endl
							<< ex.what() << std::endl;
						const bool ExceptionOccured = false;
						CHECK(ExceptionOccured);
					}
				}
			}
			catch (basar::Exception & ex)
			{
				std::cout << ex.what() << std::endl;
				const bool ExceptionOccured2 = false;
				CHECK(ExceptionOccured2);
				// 
			}
		}
	}


}

// ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~  ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~

