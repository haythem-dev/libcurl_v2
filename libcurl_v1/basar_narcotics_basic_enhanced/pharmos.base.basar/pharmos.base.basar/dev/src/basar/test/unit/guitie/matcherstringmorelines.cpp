/*
* @file                                            
* @author Anke Klink                      
* @date 2008                            
*/ 

#include <UnitTest++/UnitTest++.h>
#include "libbasarguitie.h"

#include "libbasardbaspect_connectionref.h"
#include "libbasardbaspect_accessorpropertytable_yiterator.h"

#include "testconnection.h"
#include "basematcherholder.h"
#include "basematcherholdertestszenario.h"
#include "basematcherholdertestszenariolist.h"
#include "indexcountholder.h"
#include "matcherholderlist.h"
#include "stringmatcherholdermorelines.h"
#include "loggerpool.h"

#pragma warning (push)
#pragma warning(disable: 4481)
#include <QtWidgets/QTableWidget>
#pragma warning (pop)

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

extern bool g_RUNSHORT;

//------------------------------------------------------------------------------
SUITE(GuiTieTest_Matcher_Strings_MoreLines)
{
	typedef basar::gui::tie::MatcherPolicyType MatcherPolicyType;

	//#define TEXTOUTPUT(XXX) std::cout << XXX << std::endl;
	//#define TEXTOUTPUT2(XXX) std::cout << XXX << std::endl;

	// --------------------------------------------------------------------------
	struct TwoStringMatcherHolderMoreLines: public MatcherHolderList<StringMatcherHolderMoreLines>
	{
		boost::shared_ptr<StringMatcherHolderMoreLines> m_one;
		boost::shared_ptr<StringMatcherHolderMoreLines> m_reverse;

		TwoStringMatcherHolderMoreLines()
		{

		}

		~TwoStringMatcherHolderMoreLines()
		{
		}

		bool init()
		{
			bool retval = false;
			m_one = boost::shared_ptr<StringMatcherHolderMoreLines>(new StringMatcherHolderMoreLines);
			m_reverse = boost::shared_ptr<StringMatcherHolderMoreLines>(new StringMatcherHolderMoreLines);
			if((0 != m_one.get()) && (0 != m_reverse.get()))
			{
				retval  = m_one->init(false);
				retval = retval && m_reverse->init(true); 

				m_list.push_back(m_reverse);
				m_list.push_back(m_one);
			}

			return retval;
		}
	};

	// --------------------------------------------------------------------------
	TEST_FIXTURE(StringMatcherHolderMoreLines, GuiTieTest_Matcher_Strings_MoreLines_Start)
	{
		BLOG_TRACE_METHOD(basar::test::unit::guitie::LoggerPool::logPerf, UnitTest::CurrentTest::Details()->testName);

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
				AccessorPropertyTable_YIterator acciter =accprop.begin();

				CHECK_EQUAL(0, static_cast<int>(accprop.size()));
				CHECK_EQUAL(0, static_cast<int>(guiprop.size())); 

				CHECK(guiiter.isEnd());
				CHECK(acciter.isEnd());
				CHECK(acciter.isBegin());
				CHECK(guiiter.isBegin());
				CHECK(!acciter.isNull());
				CHECK(!guiiter.isNull());

				CHECK(select());

				guiprop = getMatcher().getLeft();
				accprop = getMatcher().getRight();
				guiiter = guiprop.begin();
				acciter = accprop.begin();

				CHECK_EQUAL(m_accRowCount, static_cast<int>(accprop.size()));
				CHECK_EQUAL(0,             static_cast<int>(guiprop.size()));
				CHECK(guiiter.isEnd());
				CHECK(!acciter.isEnd());
				CHECK(acciter.isBegin());
				CHECK(guiiter.isBegin());
				CHECK(!acciter.isNull());
				CHECK(!acciter.isNull());

				resetGuiValues();

				guiprop = getMatcher().getLeft();
				accprop = getMatcher().getRight();
				guiiter = guiprop.begin();
				acciter = accprop.begin();

				CHECK_EQUAL(m_accRowCount, static_cast<int>(accprop.size()));
				CHECK_EQUAL(m_guiRowCount, static_cast<int>(guiprop.size()));
				CHECK_EQUAL(0,guiprop.getAbsoluteIndex());


				CHECK(!guiiter.isEnd());
				CHECK(!acciter.isEnd());
				CHECK(acciter.isBegin());
				CHECK(guiiter.isBegin());
				CHECK(!acciter.isNull());
				CHECK(!acciter.isNull());

				CHECK_EQUAL(getText(0,m_string1Number),guiiter.getString(csm_propdefstring1));
				CHECK_EQUAL(getText(0,m_string2Number),guiiter.getString(csm_propdefstring2));

				checkInitsOk();
			}
			catch (basar::Exception & ex)
			{
				std::cout << ex.what() << std::endl;
				const bool ExceptionOccured = false;
				CHECK(ExceptionOccured);
			}
		}
	}
	// ------------------------------------------------------------------------------------------------------------------------------------------------
	TEST_FIXTURE(TwoStringMatcherHolderMoreLines, GuiTieTest_TwoMatcher_Start)
	{
		BLOG_TRACE_METHOD(basar::test::unit::guitie::LoggerPool::logPerf, UnitTest::CurrentTest::Details()->testName);

		try
		{
			CHECK(init());
            CHECK(m_one->start());
            CHECK(m_reverse->start());

            CHECK(select());

			GuiPropertyTable_YIterator guiiter1      = m_one->getMatcher().getLeft().begin();
			AccessorPropertyTable_YIterator acciter1 = m_one->getMatcher().getRight().begin();
			GuiPropertyTable_YIterator guiiter2      = m_reverse->getMatcher().getLeft().begin();
			AccessorPropertyTable_YIterator acciter2 = m_reverse->getMatcher().getRight().begin();

			CHECK_EQUAL(acciter1.getString(StringMatcherHolderMoreLines::csm_propdefstring1), acciter2.getString(StringMatcherHolderMoreLines::csm_propdefstring1));
			CHECK_EQUAL(acciter1.getString(StringMatcherHolderMoreLines::csm_propdefstring2),acciter2.getString(StringMatcherHolderMoreLines::csm_propdefstring2));

			CHECK_EQUAL(StringMatcherHolderMoreLines::csm_propdefstring1, guiiter1.getName(0));
			CHECK_EQUAL(StringMatcherHolderMoreLines::csm_propdefstring2, guiiter1.getName(1));
			CHECK_EQUAL(StringMatcherHolderMoreLines::csm_propdefstring1, guiiter2.getName(1));
			CHECK_EQUAL(StringMatcherHolderMoreLines::csm_propdefstring2, guiiter2.getName(0));

			CHECK_EQUAL(StringMatcherHolderMoreLines::csm_propdefstring1, acciter1.getName(0));
			CHECK_EQUAL(StringMatcherHolderMoreLines::csm_propdefstring2, acciter1.getName(1));
			CHECK(StringMatcherHolderMoreLines::csm_propdefstring1!= acciter2.getName(0));
			CHECK(StringMatcherHolderMoreLines::csm_propdefstring2!= acciter2.getName(1));

			CHECK_EQUAL (acciter1.getString(0),acciter2.getString(1));
			CHECK_EQUAL (acciter1.getString(0),acciter2.getString(1));
			CHECK (acciter1.getString(0) != acciter2.getString(0));
			CHECK (acciter1.getString(1)!= acciter2.getString(1));

			CHECK_EQUAL (guiiter1.getString(0),guiiter2.getString(1));
			CHECK_EQUAL (guiiter1.getString(0),guiiter2.getString(1));
			CHECK (guiiter1.getString(0) != guiiter2.getString(0));
			CHECK (guiiter1.getString(1) != guiiter2.getString(1));

		}
		catch (basar::Exception & ex)
		{
			std::cout << ex.what() << std::endl;
			const bool ExceptionOccured = false;
			CHECK(ExceptionOccured);
		}
	}

	// ------------------------------------------------------------------------------------------------------------------------------------------------
	TEST_FIXTURE(StringMatcherHolderMoreLines, GuiTieTest_Matcher_Strings_MoreLines_RightToLeft_NoVar_withMemory)
	{
		BLOG_TRACE_METHOD(basar::test::unit::guitie::LoggerPool::logPerf, UnitTest::CurrentTest::Details()->testName);

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
						CHECK(testiter->start("GuiTieTest_Matcher_Strings_MoreLines_RightToLeft_NoVar_withMemory", *this));
						TEXTOUTPUT(testiter->toText());
						checkInitsOk();
						CHECK(select());

						guiprop = getMatcher().getLeft();
						accprop = getMatcher().getRight();

						CHECK_EQUAL(m_accRowCount, static_cast<int>(accprop.size()));
						CHECK_EQUAL(m_guiRowCount, static_cast<int>(guiprop.size()));

						guiprop.setMemoryPolicy(basar::gui::tie::WITH_MEMORY);
						CHECK_EQUAL(0,guiprop.getAbsoluteIndex());

						if (testiter->throwsToManyEntriesException())
						{
							CHECK_THROW( getMatcher().RightToLeft(), basar::Exception);
							CHECK_THROW( getMatcher().RightToLeft(), basar::Exception); 
						}
						else
						{          
							int maxnumber = testiter->calculateMinRowNumber(Direction::RightToLeft, m_accRowCount, m_guiRowCount);
							int guinumber = (m_guiRowCount > maxnumber) ? m_guiRowCount : maxnumber;

							CHECK_EQUAL(maxnumber,getMatcher().RightToLeft());

							guiprop = getMatcher().getLeft();
							accprop = getMatcher().getRight();
							CHECK_EQUAL(m_accRowCount, static_cast<int>(accprop.size()));
							CHECK_EQUAL(guinumber,     static_cast<int>(guiprop.size()));
							CHECK_EQUAL(0,guiprop.getAbsoluteIndex());

							guiiter = guiprop.begin();
							acciter = accprop.begin();

							int number = 0;
							for (; (!guiiter.isEnd()) && (!acciter.isEnd()); ++acciter, ++guiiter, ++number)
							{
								checkEqual(Direction::RL, number, guiiter , acciter , *testiter);
							}
							CHECK_EQUAL(maxnumber, number);
							if (number < m_guiRowCount)
							{                         
								// no changes there:
								for (; (!guiiter.isEnd()) && (number < m_guiRowCount); ++guiiter, ++number)
								{
									checkEqualToText(number, guiiter , *testiter);                
								}
								CHECK_EQUAL(m_guiRowCount, number);
							}
							// and the other way around:
							{
								resetGuiValues();
								int rowcount = m_tableWidget->rowCount();
								guiiter = getMatcher().getLeft().begin();
								number = 0;
								for (; (!guiiter.isEnd()) ;  ++guiiter, ++number)
								{
									CHECK_EQUAL(getText(number,m_string1Number),guiiter.getString(csm_propdefstring1));
									CHECK_EQUAL(getText(number,m_string2Number),guiiter.getString(csm_propdefstring2));
								}
								CHECK_EQUAL(rowcount, number);

								maxnumber = testiter->calculateMinRowNumber(Direction::LeftToRight, m_accRowCount, rowcount);

								CHECK_EQUAL(maxnumber,getMatcher().LeftToRight());

								guiiter = getMatcher().getLeft().begin();
								acciter = getMatcher().getRight().begin();

								number = 0;
								for (; (!guiiter.isEnd()) && (!acciter.isEnd()); ++acciter, ++guiiter, ++number)
								{
									checkEqual(Direction::LR, number, guiiter , acciter , *testiter);
								}
								CHECK_EQUAL(maxnumber, number);
								if (number < m_accRowCount)
								{
									// no changes there:
									for (; (!acciter.isEnd()) && (number < m_accRowCount); ++acciter, ++number)
									{
										checkEqualToSelect(number, acciter , *testiter);   
									}
									CHECK_EQUAL(m_accRowCount, number);
								}
							}
						}
					}
					catch (basar::Exception & ex)
					{
						std::cout << " Exception in" << testiter->toText() << std::endl << ex.what() << std::endl;
						const bool ExceptionOccured = false;
						CHECK(ExceptionOccured);
					}
					catch (...)
					{
						std::cout << "Non Basar Exception in" << testiter->toText() << std::endl;
						const bool NonBasarExceptionOccured = false;
						CHECK(NonBasarExceptionOccured);
					}
				}
			}
			catch (basar::Exception & ex)
			{
				std::cout << ex.what() << std::endl;
				const bool ExceptionOccured2 = false;
				CHECK(ExceptionOccured2);
			}
			catch (...)
			{
				std::cout << "Exception ocurred" << std::endl;
				const bool NonBasarExceptionOccured2 = false;
				CHECK(NonBasarExceptionOccured2);
				throw;
			}

		}
	}

	// ------------------------------------------------------------------------------------------------------------------------------------------------
	TEST_FIXTURE(StringMatcherHolderMoreLines, GuiTieTest_Matcher_Strings_MoreLines_LeftToRight_NoVar_withMemory)
	{
		BLOG_TRACE_METHOD(basar::test::unit::guitie::LoggerPool::logPerf, UnitTest::CurrentTest::Details()->testName);

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
						CHECK(testiter->start("GuiTieTest_Matcher_Strings_MoreLines_LeftToRight_NoVar_withMemory", *this));

						CHECK(select());

						accprop = getMatcher().getRight();
						guiprop = getMatcher().getLeft();
						CHECK_EQUAL(m_accRowCount, static_cast<int>(accprop.size()));
						CHECK_EQUAL(m_guiRowCount, static_cast<int>(guiprop.size()));

						guiprop.setMemoryPolicy(basar::gui::tie::WITH_MEMORY);

						if (testiter->throwsToManyEntriesException())
						{
							CHECK_THROW( getMatcher().LeftToRight(), basar::Exception);
						}
						else
						{  
							int maxnumber = testiter->calculateMinRowNumber(Direction::LeftToRight, m_accRowCount, m_guiRowCount);
							int accnumber = (m_accRowCount < maxnumber) ? maxnumber : m_accRowCount;

							CHECK_EQUAL(maxnumber,getMatcher().LeftToRight());

							accprop = getMatcher().getRight();
							guiprop = getMatcher().getLeft();

							CHECK_EQUAL(m_guiRowCount, static_cast<int>(guiprop.size()));
							CHECK_EQUAL(accnumber,     static_cast<int>(accprop.size()));

							guiiter = guiprop.begin();
							acciter = accprop.begin();

							int number = 0;
							for (; (!guiiter.isEnd()) && (!acciter.isEnd()); ++acciter, ++guiiter, ++number)
							{
								checkEqual(Direction::LR, number, guiiter , acciter , *testiter);

							}
							CHECK_EQUAL(maxnumber, number);
							if (number < m_accRowCount)
							{
								// no changes there:
								for (; (!acciter.isEnd()) && (number < m_accRowCount); ++acciter, ++number)
								{
									checkEqualToSelect(number,  acciter , *testiter);

								}
								CHECK_EQUAL(m_accRowCount, number);
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

	// ------------------------------------------------------------------------------------------------------------------------------------------------	
	TEST_FIXTURE (StringMatcherHolderMoreLines, GuiTieTest_Matcher_Strings_MoreLines_RightToLeft_Iter_internal)
	{ 
		BLOG_TRACE_METHOD(basar::test::unit::guitie::LoggerPool::logPerf, UnitTest::CurrentTest::Details()->testName);

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

				std::vector<BaseMatcherHolderTestSzenario> tests = BaseMatcherHolderTestSzenarioList::getPolicyTestSzenariosOneRowTest(NORM);
				std::vector<BaseMatcherHolderTestSzenario>::iterator testiter;
				for (testiter = tests.begin(); testiter != tests.end(); ++testiter)
				{
					try
					{
						CHECK(testiter->start("GuiTieTest_Matcher_Strings_MoreLines_RightToLeft_Iter_internal", *this));
						TEXTOUTPUT(testiter->toText());
						CHECK(select());

						guiprop = getMatcher().getLeft();
						accprop = getMatcher().getRight();
						CHECK_EQUAL(m_accRowCount, static_cast<int>(accprop.size()));
						CHECK_EQUAL(m_guiRowCount, static_cast<int>(guiprop.size()));

						if (testiter->throwsToManyEntriesException())
						{
							guiiter = guiprop.begin();
							acciter = accprop.begin();
							CHECK_THROW( getMatcher().RightToLeft(guiiter, acciter), basar::Exception);
						}
						else
						{       
							guiiter = guiprop.begin();
							acciter = accprop.begin();
							int number = 0;
							for (; ((number<m_accRowCount) && (!acciter.isEnd())&& (!guiiter.isEnd())); ++number, acciter++, guiiter++)
							{
								getMatcher().RightToLeft(guiiter,acciter);
								checkEqual(Direction::RL, number, guiiter , acciter , *testiter);

							}
							CHECK_EQUAL(m_accRowCount, number);
						}
					}
					catch (basar::Exception & ex)
					{
						std::cout << " Exception in" << testiter->toText() << std::endl << ex.what() << std::endl;
						const bool ExceptionOccured = false;
						CHECK(ExceptionOccured);
					}
					catch (...)
					{
						std::cout << "Non Basar Exception in" << testiter->toText() << std::endl;
						const bool NonBasarExceptionOccured = false;
						CHECK(NonBasarExceptionOccured);
					}
				}
			}
			catch (basar::Exception & ex)
			{
				std::cout << ex.what() << std::endl;
				const bool ExceptionOccured2 = false;
				CHECK(ExceptionOccured2);
			}
			catch (...)
			{
				std::cout << "Exception ocurred" << std::endl;
				const bool NonBasarExceptionOccured2 = false;
				CHECK(NonBasarExceptionOccured2);
				throw;
			}

		}
	}

	// ------------------------------------------------------------------------------------------------------------------------------------------------
	TEST_FIXTURE(StringMatcherHolderMoreLines, GuiTieTest_Matcher_Strings_MoreLines_LeftToRight_Iter_internal)
	{
		BLOG_TRACE_METHOD(basar::test::unit::guitie::LoggerPool::logPerf, UnitTest::CurrentTest::Details()->testName);

		std::vector<TableWidgetKind> tableWidgetKindVec = getTableWidgetKindVector();
		std::vector<TableWidgetKind>::const_iterator tableWidgetKindIter;
		for(tableWidgetKindIter = tableWidgetKindVec.begin(); tableWidgetKindIter != tableWidgetKindVec.end(); ++tableWidgetKindIter)
		{  
			try
			{
				CHECK(initTableWidgetKind(*tableWidgetKindIter));
				GuiPropertyTable_YIterator guiiter;
				AccessorPropertyTable_YIterator acciter;

				std::vector<BaseMatcherHolderTestSzenario> tests = BaseMatcherHolderTestSzenarioList::getPolicyTestSzenariosOneRowTest(NORM);
				std::vector<BaseMatcherHolderTestSzenario>::iterator testiter;
				for (testiter = tests.begin(); testiter != tests.end(); ++testiter)
				{
					try
					{
						CHECK(testiter->start("GuiTieTest_Matcher_Strings_MoreLines_LeftToRight_Iter_internal", *this));
						TEXTOUTPUT(testiter->toText());

						CHECK(select());
						if (testiter->throwsToManyEntriesException())
						{
							guiiter = getMatcher().getLeft().begin();
							acciter = getMatcher().getRight().begin();
							CHECK_THROW( getMatcher().LeftToRight(guiiter, acciter), basar::Exception);
						}
						else
						{  
							guiiter = getMatcher().getLeft().begin();
							acciter = getMatcher().getRight().begin(); 

							int number = 0;

							for (; ((number<m_accRowCount) && (!acciter.isEnd()) && (!guiiter.isEnd())); ++number, acciter++, guiiter++)
							{
								getMatcher().LeftToRight(guiiter, acciter);
								checkEqual(Direction::LR, number, guiiter , acciter , *testiter);

							}
							CHECK_EQUAL(m_accRowCount, number);
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
	// ------------------------------------------------------------------------------------------------------------------------------------------------
	TEST_FIXTURE(StringMatcherHolderMoreLines, GuiTieTest_Matcher_Strings_MoreLines_RightToLeft_int_withMemory)
	{
		BLOG_TRACE_METHOD(basar::test::unit::guitie::LoggerPool::logPerf, UnitTest::CurrentTest::Details()->testName);

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

				std::vector<BaseMatcherHolderTestSzenario> tests = BaseMatcherHolderTestSzenarioList::getPolicyTestSzenariosOneRowTest(MORE_ACC);
				std::vector<BaseMatcherHolderTestSzenario>::iterator testiter;
				for (testiter = tests.begin(); testiter != tests.end(); ++testiter)
				{
					try
					{
						CHECK(testiter->start("GuiTieTest_Matcher_Strings_MoreLines_RightToLeft_int_withMemory", *this));
						TEXTOUTPUT(testiter->toText());
						CHECK(select());

						guiprop = getMatcher().getLeft();
						accprop = getMatcher().getRight();
						CHECK_EQUAL(0,guiprop.getAbsoluteIndex());

						CHECK_EQUAL(m_accRowCount, static_cast<int>(accprop.size()));
						CHECK_EQUAL(m_guiRowCount, static_cast<int>(guiprop.size()));

						guiprop.setMemoryPolicy(basar::gui::tie::WITH_MEMORY);

						if (testiter->throwsToManyEntriesException())
						{
							CHECK_THROW( getMatcher().RightToLeft(0,1), basar::Exception);
							CHECK_THROW( getMatcher().RightToLeft(1,2), basar::Exception);
							CHECK_THROW( getMatcher().RightToLeft(-10,-10), basar::Exception);
						}
						else
						{   
							TEXTOUTPUT2(testiter->toText());
							// test exception throwing:
							{             
								// negative index:
								CHECK_THROW(getMatcher().RightToLeft(-1, 1), basar::Exception);

								// beyond begin()
								CHECK_THROW(getMatcher().RightToLeft(0, -2), basar::Exception);
								// beyond begin()
								CHECK_THROW(getMatcher().RightToLeft(2, -27), basar::Exception); 
							}

							if (testiter->checkPolicy(basar::gui::tie::INSERT) 
								|| getRowTestNumber() == MORE_GUI)
							{
								CHECK_THROW(getMatcher().RightToLeft(27, 1), basar::Exception); 
								CHECK_THROW(getMatcher().RightToLeft(27, 0), basar::Exception); 
								CHECK_THROW(getMatcher().RightToLeft(27, 2), basar::Exception); 
							}
							else
							{
								CHECK_EQUAL(0,getMatcher().RightToLeft(27, 1));
								CHECK_EQUAL(0,getMatcher().RightToLeft(27, 0));
								CHECK_EQUAL(0,getMatcher().RightToLeft(27, 2));
							}


							// various, working tests
							{
								int maxnumber = testiter->calculateMinRowNumber(Direction::RightToLeft, m_accRowCount, m_guiRowCount);
								std::vector<IndexCountHolder> indexcountlist;
								indexcountlist.push_back(IndexCountHolder(0,1, maxnumber));
								indexcountlist.push_back(IndexCountHolder(1,1, maxnumber));
								indexcountlist.push_back(IndexCountHolder(3,2, maxnumber));
								indexcountlist.push_back(IndexCountHolder(0,0, maxnumber));
								indexcountlist.push_back(IndexCountHolder(1,-1, maxnumber)); 
								indexcountlist.push_back(IndexCountHolder(1,-2, maxnumber)); 
								indexcountlist.push_back(IndexCountHolder(3,10, maxnumber)); 
								std::vector<IndexCountHolder>::const_iterator indexcount;
								for (indexcount = indexcountlist.begin(); indexcount != indexcountlist.end(); ++indexcount)
								{     
									try
									{
										TEXTOUTPUT2(testiter->toText() << " (index,count,start,number)=(" << indexcount->m_index << "," << indexcount->m_count << "," << indexcount->m_start << "," <<indexcount->m_number << ")");
										resetGuiValues();
										CHECK_EQUAL(indexcount->m_number,getMatcher().RightToLeft(indexcount->m_index, indexcount->m_count));
										maxnumber = testiter->calculateMinRowNumber(Direction::RightToLeft, m_accRowCount, m_guiRowCount,*indexcount );
										if (maxnumber > m_guiRowCount) m_guiRowCount = maxnumber;

										guiiter = getMatcher().getLeft().begin();
										acciter = getMatcher().getRight().begin();
										int i=0;
										int j=0;
										// nothing changed on the first:
										for (; i<indexcount->m_start && !acciter.isEnd() && !guiiter.isEnd(); ++i)
										{
											checkEqualToText(i, guiiter ,  *testiter);
											++acciter;
											++guiiter;
										}
										CHECK_EQUAL(indexcount->m_start, i);
										// the changed values:
										for (; j<indexcount->m_number && !acciter.isEnd() && !guiiter.isEnd(); ++i, ++j)
										{
											checkEqual(Direction::RL, i, guiiter , acciter , *testiter);
											++acciter;
											++guiiter;
										}
										CHECK_EQUAL(indexcount->m_number ,j); 
										CHECK_EQUAL(j + indexcount->m_start,i); 
										// nothing changed on the rest:
										for (; i<maxnumber && !acciter.isEnd() && !guiiter.isEnd(); ++i)
										{
											checkEqualToText(i, guiiter   , *testiter);
											++acciter;
											++guiiter;
										}
										CHECK_EQUAL(maxnumber, i); 
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

							// overflow:
							{
								reset();
								testiter->restart(*this);
								CHECK(select());
								int start = 3;
								int count = 10;

								//std::cout << "hallo " << testiter->toText() << " rowcount =  " << this->m_tableWidget->rowCount() << std::endl;
								if (testiter->checkPolicy(basar::gui::tie::INSERT))
								{
									CHECK_THROW(getMatcher().LeftToRight(start, count), basar::Exception);
								}
								else
								{
									if (getRowTestNumber() == MORE_GUI)
									{
										CHECK_EQUAL(3,getMatcher().LeftToRight(start, count));
								 }
								 else  
								 {
									 CHECK_THROW(getMatcher().LeftToRight(start, count), basar::Exception);
								 } 
								}
							}
						}
					}
					catch (basar::Exception & ex)
					{
						std::cout << " Exception in" << testiter->toText() << std::endl << ex.what() << std::endl;
						const bool ExceptionOccured2 = false;
						CHECK(ExceptionOccured2);
					}
					catch (...)
					{
						std::cout << "Non Basar Exception in" << testiter->toText() << std::endl;
						const bool NonBasarExceptionOccured2 = false;
						CHECK(NonBasarExceptionOccured2);
					}
				}
			}
			catch (basar::Exception & ex)
			{
				std::cout << ex.what() << std::endl;
				const bool ExceptionOccured1 = false;
				CHECK(ExceptionOccured1);
			}
			catch (...)
			{
				std::cout << "Exception ocurred" << std::endl;
				const bool NonBasarExceptionOccured1 = false;
				CHECK(NonBasarExceptionOccured1);
				throw;
			}
		}
	}
	// ------------------------------------------------------------------------------------------------------------------------------------------------
	TEST_FIXTURE(StringMatcherHolderMoreLines, GuiTieTest_Matcher_Strings_MoreLines_LeftToRight_int_withMemory)
	{
		BLOG_TRACE_METHOD(basar::test::unit::guitie::LoggerPool::logPerf, UnitTest::CurrentTest::Details()->testName);

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

				std::vector<BaseMatcherHolderTestSzenario> tests = BaseMatcherHolderTestSzenarioList::getPolicyTestSzenariosOneRowTest(NORM);
				std::vector<BaseMatcherHolderTestSzenario>::iterator testiter;
				for (testiter = tests.begin(); testiter != tests.end(); ++testiter)
				{
					try
					{
						CHECK(testiter->start("GuiTieTest_Matcher_Strings_MoreLines_LeftToRight_int_withMemoryine", *this));
						TEXTOUTPUT(testiter->toText());

						CHECK(select());

						guiprop = getMatcher().getLeft();
						accprop = getMatcher().getRight();
						CHECK_EQUAL(0,guiprop.getAbsoluteIndex());

						CHECK_EQUAL(m_accRowCount, static_cast<int>(accprop.size()));
						CHECK_EQUAL(m_guiRowCount, static_cast<int>(guiprop.size()));

						guiprop.setMemoryPolicy(basar::gui::tie::WITH_MEMORY);


						if (testiter->throwsToManyEntriesException())
						{
							CHECK_THROW(getMatcher().LeftToRight(0, 1), basar::Exception);
							CHECK_THROW(getMatcher().LeftToRight(4, 7), basar::Exception);
						}
						else
						{
							{             
								// negative index:
								CHECK_THROW(getMatcher().LeftToRight(-1, 1), basar::Exception);
								// index too high
								CHECK_THROW(getMatcher().LeftToRight(27, 1), basar::Exception);
								// beyond begin()
								CHECK_THROW(getMatcher().LeftToRight(0, -2), basar::Exception);
								// beyond begin()
								CHECK_THROW(getMatcher().LeftToRight(2, -27), basar::Exception); 


							}
							// various, working tests
							{
								int maxnumber = testiter->calculateMinRowNumber(Direction::LeftToRight, m_accRowCount, m_guiRowCount);
								std::vector<IndexCountHolder> indexcountlist;
								indexcountlist.push_back(IndexCountHolder(0,1, maxnumber));
								indexcountlist.push_back(IndexCountHolder(1,1, maxnumber));
								indexcountlist.push_back(IndexCountHolder(3,2, maxnumber));
								indexcountlist.push_back(IndexCountHolder(0,0, maxnumber));
								indexcountlist.push_back(IndexCountHolder(1,-1, maxnumber)); 
								indexcountlist.push_back(IndexCountHolder(1,-2, maxnumber)); 

								std::vector<IndexCountHolder>::const_iterator indexcount;
								for (indexcount = indexcountlist.begin(); indexcount != indexcountlist.end(); ++indexcount)
								{     
									try
									{
										resetGuiValues();
										CHECK(select());
										int number = getMatcher().LeftToRight(indexcount->m_index, indexcount->m_count);
										CHECK_EQUAL(indexcount->m_number, number);
										TEXTOUTPUT2("getMatcher().LeftToRight(" << indexcount->m_index << "," << indexcount->m_count << "returns " << number << " calculated was " << indexcount->m_number);

										maxnumber = checkAfterLeftToRightIndexCount(*indexcount,*testiter);
										if (maxnumber > m_guiRowCount) m_guiRowCount = maxnumber;
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

	// ------------------------------------------------------------------------------------------------------------------------------------------------
	TEST_FIXTURE (StringMatcherHolderMoreLines, GuiTieTest_Matcher_Strings_MoreLines_RightToLeft_policy)
	{ 
		if (g_RUNSHORT)
			return;

		BLOG_TRACE_METHOD(basar::test::unit::guitie::LoggerPool::logPerf, UnitTest::CurrentTest::Details()->testName);

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

				std::vector<BaseMatcherHolderTestSzenario> tests = BaseMatcherHolderTestSzenarioList::getPolicyTestSzenariosOneRowTest(NORM);
				std::vector<BaseMatcherHolderTestSzenario>::iterator testiter;
				for (testiter = tests.begin(); testiter != tests.end(); ++testiter)
				{
					try
					{
						CHECK(testiter->start("GuiTieTest_Matcher_Strings_MoreLines_RightToLeft_Iter_internal", *this));
						TEXTOUTPUT(testiter->toText());
						std::vector<MatcherPolicyType> policyvec = BaseMatcherHolderTestSzenarioList::getTestMatcherPolicyTypeVector();
						std::vector<MatcherPolicyType>::const_iterator policyiter;
						for (policyiter = policyvec.begin(); policyiter != policyvec.end(); ++policyiter)
						{		
							try
							{
								CHECK(select());
								resetGuiValues();

								guiprop = getMatcher().getLeft();
								accprop = getMatcher().getRight();
								guiiter = guiprop.begin();
								acciter = accprop.begin();
								CHECK_EQUAL(m_accRowCount, static_cast<int>(accprop.size()));
								CHECK_EQUAL(m_guiRowCount, static_cast<int>(guiprop.size()));

								if (testiter->throwsToManyEntriesException(*policyiter))
								{ 
									CHECK_THROW( MatcherRef::RightToLeft(guiiter, acciter, *policyiter), basar::Exception);
								}
								else
								{        
									int number = 0;
									for (; ((number<m_accRowCount) && (!acciter.isEnd()) && (!guiiter.isEnd())); ++number, acciter++, guiiter++)
									{
										MatcherRef::RightToLeft(guiiter,acciter, *policyiter);
										checkEqual(Direction::RL, number, guiiter , acciter , *testiter);
									}
									CHECK_EQUAL(m_accRowCount, number);
								} 
							}
							catch (basar::Exception & ex)
							{
								std::cout << " Exception in" << testiter->toText() << std::endl << ex.what() << std::endl;
								const bool InnerExceptionOccured = false;
								CHECK(InnerExceptionOccured);
							}
							catch (...)
							{
								std::cout << "Non Basar Exception in" << testiter->toText() << std::endl;
								const bool InnerNonBasarExceptionOccured = false;
								CHECK(InnerNonBasarExceptionOccured);
							}
						}
					}
					catch (basar::Exception & ex)
					{
						std::cout << " Exception in" << testiter->toText() << std::endl << ex.what() << std::endl;
						const bool ExceptionOccured = false;
						CHECK(ExceptionOccured);
					}
					catch (...)
					{
						std::cout << "Non Basar Exception in" << testiter->toText() << std::endl;
						const bool NonBasarExceptionOccured = false;
						CHECK(NonBasarExceptionOccured);
					}
				}
			}
			catch (basar::Exception & ex)
			{
				std::cout << ex.what() << std::endl;
				const bool ExceptionOccured2 = false;
				CHECK(ExceptionOccured2);
			}
			catch (...)
			{
				std::cout << "Exception ocurred" << std::endl;
				const bool NonBasarExceptionOccured2 = false;
				CHECK(NonBasarExceptionOccured2);
				throw;
			}

		}
	}

	// ------------------------------------------------------------------------------------------------------------------------------------------------
	TEST_FIXTURE(StringMatcherHolderMoreLines, GuiTieTest_Matcher_Strings_MoreLines_LeftToRight_policy)
	{
		if (g_RUNSHORT)
			return;

		BLOG_TRACE_METHOD(basar::test::unit::guitie::LoggerPool::logPerf, UnitTest::CurrentTest::Details()->testName);

		std::vector<TableWidgetKind> tableWidgetKindVec = getTableWidgetKindVector();
		std::vector<TableWidgetKind>::const_iterator tableWidgetKindIter;
		for(tableWidgetKindIter = tableWidgetKindVec.begin(); tableWidgetKindIter != tableWidgetKindVec.end(); ++tableWidgetKindIter)
		{ 
			try
			{
				CHECK(initTableWidgetKind(*tableWidgetKindIter));
				GuiPropertyTable_YIterator guiiter;
				AccessorPropertyTable_YIterator acciter;

				std::vector<BaseMatcherHolderTestSzenario> tests = BaseMatcherHolderTestSzenarioList::getPolicyTestSzenariosOneRowTest(NORM);
				std::vector<BaseMatcherHolderTestSzenario>::iterator testiter;
				for (testiter = tests.begin(); testiter != tests.end(); ++testiter)
				{ 
					CHECK(testiter->start("GuiTieTest_Matcher_Strings_MoreLines_LeftToRight_Iter_internal", *this));
					TEXTOUTPUT(testiter->toText());
					try
					{

						std::vector<MatcherPolicyType> policyvec = BaseMatcherHolderTestSzenarioList::getTestMatcherPolicyTypeVector();
						std::vector<MatcherPolicyType>::const_iterator policyiter;
						for (policyiter = policyvec.begin(); policyiter != policyvec.end(); ++policyiter)
						{		
							try
							{
								CHECK(select());
								guiiter = getMatcher().getLeft().begin();
								acciter = getMatcher().getRight().begin();
								if (testiter->throwsToManyEntriesException(*policyiter))
								{
									CHECK_THROW( MatcherRef::LeftToRight(guiiter, acciter,*policyiter), basar::Exception);
								}
								else
								{  

									int number = 0;
									for (; ((number<m_accRowCount) && (!acciter.isEnd())&& (!guiiter.isEnd())); ++number, acciter++, guiiter++)
									{
										MatcherRef::LeftToRight(guiiter, acciter, *policyiter);
										checkEqual(Direction::LR, number, guiiter , acciter , *testiter);

									}
									CHECK_EQUAL(m_accRowCount, number);
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
					}
				}
			}
			catch (basar::Exception & ex)
			{
				std::cout << ex.what() << std::endl;
				const bool ExceptionOccured3 = false;
				CHECK(ExceptionOccured3);
				// 
			}
		}
	}
	// ------------------------------------------------------------------------------------------------------------------------------------------------
	TEST_FIXTURE(StringMatcherHolderMoreLines, GuiTieTest_Matcher_Strings_MoreLines_RightToLeft_NoVar_withoutMemory)
	{
		if (g_RUNSHORT)
			return;

		BLOG_TRACE_METHOD(basar::test::unit::guitie::LoggerPool::logPerf, UnitTest::CurrentTest::Details()->testName);

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
				basar::gui::tie::MemoryPolicyEnum memory = basar::gui::tie::WITHOUT_MEMORY;
				for (testiter = tests.begin(); testiter != tests.end(); ++testiter)
				{
					try
					{
						CHECK(testiter->start("GuiTieTest_Matcher_Strings_MoreLines_NoMemory_RightToLeft_NoVar_withoutMemory", *this));
						TEXTOUTPUT(testiter->toText());
						CHECK(select());

						guiprop = getMatcher().getLeft();
						accprop = getMatcher().getRight();

						CHECK_EQUAL(m_accRowCount, static_cast<int>(accprop.size()));
						CHECK_EQUAL(m_guiRowCount, static_cast<int>(guiprop.size()));

						guiprop.setMemoryPolicy(memory);
						CHECK_EQUAL(0,guiprop.getAbsoluteIndex());

						if (testiter->throwsToManyEntriesException())
						{
							CHECK_THROW( getMatcher().RightToLeft(), basar::Exception);
							CHECK_THROW( getMatcher().RightToLeft(), basar::Exception); 
						}
						else 
						{          
							int maxnumber = testiter->calculateMinRowNumber(Direction::RightToLeft, m_accRowCount, m_guiRowCount);
							int guinumber = (m_guiRowCount > maxnumber) ? m_guiRowCount : maxnumber;
							int absnumber = testiter->calculateAbsoluteIndex(memory,Direction::RightToLeft, m_accRowCount, m_guiRowCount);

							int numberLines = getMatcher().RightToLeft();
							CHECK_EQUAL(maxnumber,numberLines);
							TEXTOUTPUT2("getMatcher().RightToLeft() returns " << numberLines << " calculated was " << maxnumber);

							guiprop = getMatcher().getLeft(); 
							accprop = getMatcher().getRight();
							CHECK_EQUAL(m_accRowCount, static_cast<int>(accprop.size()));
							CHECK_EQUAL(guinumber,     static_cast<int>(guiprop.size()));
							CHECK_EQUAL(absnumber,     guiprop.getAbsoluteIndex());

							guiiter = guiprop.begin();
							acciter = accprop.begin();

							int number = 0;

							for (; (!guiiter.isEnd()) && (!acciter.isEnd()); ++acciter, ++guiiter, ++number)
							{
								checkEqualNoMemory(Direction::RL, number, absnumber, guiiter , acciter , *testiter);
							}
							CHECK_EQUAL(maxnumber, number);
							if (number < m_guiRowCount)
							{                         
								// no changes there:
								for (; (!guiiter.isEnd()) && (number < m_guiRowCount); ++guiiter, ++number)
								{
									checkEqualToText(number, guiiter , *testiter);                
								}
								CHECK_EQUAL(m_guiRowCount, number);
							}
							// reset gui
							int rowcount = m_tableWidget->rowCount();
							{
								resetGuiValues();
								guiiter = getMatcher().getLeft().begin();
								number = 0;
								for (; (!guiiter.isEnd()) && number < absnumber;  ++guiiter, ++number)
								{
									CHECK_THROW(guiiter.getString(csm_propdefstring1), basar::Exception);
								}
								number = 0;
								for (; (!guiiter.isEnd()) ;  ++guiiter, ++number)
								{
									checkEqualToText(number, guiiter , *testiter);                
								}
								CHECK_EQUAL(rowcount, number);
							}
							int wholeNumberOfTheoreticalGuiNumber = rowcount + absnumber;
							// test LeftToRight:
							maxnumber = testiter->calculateMinRowNumber(Direction::LeftToRight, m_accRowCount, wholeNumberOfTheoreticalGuiNumber);
							CHECK_EQUAL(maxnumber,getMatcher().LeftToRight());

							guiiter = getMatcher().getLeft().begin();
							acciter = getMatcher().getRight().begin();

							number = 0;

							for (; (!guiiter.isEnd()) && (!acciter.isEnd()); ++acciter, ++guiiter, ++number)
							{
								checkEqualNoMemory(Direction::LR, number, absnumber, guiiter , acciter , *testiter);
							}
							CHECK_EQUAL(maxnumber, number);
							if (number < m_accRowCount)
							{
								// no changes there:
								for (; (!acciter.isEnd()) && (number < m_accRowCount); ++acciter, ++number)
								{
									checkEqualToSelect(number, acciter , *testiter);   
								}
								CHECK_EQUAL(m_accRowCount, number);
							}
						}
					}
					catch (basar::Exception & ex)
					{
						std::cout << " Exception in" << testiter->toText() << std::endl << ex.what() << std::endl;
						const bool ExceptionOccured = false;
						CHECK(ExceptionOccured);
					}
					catch (...)
					{
						std::cout << "Non Basar Exception in" << testiter->toText() << std::endl;
						const bool NonBasarExceptionOccured = false;
						CHECK(NonBasarExceptionOccured);
					}
				}
			}
			catch (basar::Exception & ex)
			{
				std::cout << ex.what() << std::endl;
				const bool ExceptionOccured2 = false;
				CHECK(ExceptionOccured2);
			}
			catch (...)
			{
				std::cout << "Exception ocurred" << std::endl;
				const bool NonBasarExceptionOccured2 = false;
				CHECK(NonBasarExceptionOccured2);
				throw;
			}
		}
	}

	// ------------------------------------------------------------------------------------------------------------------------------------------------
	TEST_FIXTURE(StringMatcherHolderMoreLines, GuiTieTest_Matcher_Strings_MoreLines_LeftToRight_NoVar_withoutMemory)
	{
		if (g_RUNSHORT)
			return;

		BLOG_TRACE_METHOD(basar::test::unit::guitie::LoggerPool::logPerf, UnitTest::CurrentTest::Details()->testName);

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
						CHECK(testiter->start("GuiTieTest_Matcher_Strings_MoreLines_NoMemory_LeftToRight_NoVar_withoutMemory", *this));

						CHECK(select());

						accprop = getMatcher().getRight();
						guiprop = getMatcher().getLeft();
						CHECK_EQUAL(m_accRowCount, static_cast<int>(accprop.size()));
						CHECK_EQUAL(m_guiRowCount, static_cast<int>(guiprop.size()));

						guiprop.setMemoryPolicy(basar::gui::tie::WITHOUT_MEMORY);

						if (testiter->throwsToManyEntriesException())
						{
							CHECK_THROW( getMatcher().LeftToRight(), basar::Exception);
						}
						else
						{  
							int maxnumber = testiter->calculateMinRowNumber(Direction::LeftToRight, m_accRowCount, m_guiRowCount);
							int accnumber = (m_accRowCount < maxnumber) ? maxnumber : m_accRowCount;
							int absnumber = 0;

							CHECK_EQUAL(maxnumber,getMatcher().LeftToRight());

							accprop = getMatcher().getRight();
							guiprop = getMatcher().getLeft();

							CHECK_EQUAL(m_guiRowCount, static_cast<int>(guiprop.size()));
							CHECK_EQUAL(accnumber,     static_cast<int>(accprop.size()));
							CHECK_EQUAL(absnumber,     guiprop.getAbsoluteIndex());
							//absnumber

							guiiter = guiprop.begin();
							acciter = accprop.begin();

							int number = 0;
							for (; (!guiiter.isEnd()) && (!acciter.isEnd()); ++acciter, ++guiiter, ++number)
							{
								checkEqualNoMemory(Direction::LR, number, absnumber, guiiter , acciter , *testiter);

							}
							CHECK_EQUAL(maxnumber, number);
							if (number < m_accRowCount)
							{
								// no changes there:
								for (; (!acciter.isEnd()) && (number < m_accRowCount); ++acciter, ++number)
								{
									checkEqualToSelect(number, acciter , *testiter);

								}
								CHECK_EQUAL(m_accRowCount, number);
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
			}
		}
	}

	// ------------------------------------------------------------------------------------------------------------------------------------------------	
	TEST_FIXTURE(StringMatcherHolderMoreLines, GuiTieTest_Matcher_Strings_MoreLines_RightToLeft_int_withoutMemory)
	{
		if (g_RUNSHORT)
			return;

		BLOG_TRACE_METHOD(basar::test::unit::guitie::LoggerPool::logPerf, UnitTest::CurrentTest::Details()->testName);

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
						CHECK(testiter->start("GuiTieTest_Matcher_Strings_MoreLines_NoMemory_RightToLeft_int_withoutMemory", *this));
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
							CHECK_THROW( getMatcher().RightToLeft(0,1), basar::Exception);
							CHECK_THROW( getMatcher().RightToLeft(1,2), basar::Exception);
							CHECK_THROW( getMatcher().RightToLeft(-10,-10), basar::Exception);
						}
						else
						{   
							TEXTOUTPUT2(testiter->toText()); 
							// test exception throwing: 
							{             
								// negative index:
								CHECK_THROW(getMatcher().RightToLeft(-1, 1), basar::Exception);
								// beyond begin()
								CHECK_THROW(getMatcher().RightToLeft(0, -2), basar::Exception);
								// beyond begin()
								CHECK_THROW(getMatcher().RightToLeft(2, -27), basar::Exception); 
							}
							if (testiter->checkPolicy(basar::gui::tie::INSERT) || getRowTestNumber() == MORE_GUI)
							{
								CHECK_THROW(getMatcher().RightToLeft(27, 1), basar::Exception); 
								CHECK_THROW(getMatcher().RightToLeft(27, 0), basar::Exception); 
								CHECK_THROW(getMatcher().RightToLeft(27, 2), basar::Exception); 
							}
							else
							{
								CHECK_EQUAL(0,getMatcher().RightToLeft(27, 1));
								CHECK_EQUAL(0,getMatcher().RightToLeft(27, 0));
								CHECK_EQUAL(0,getMatcher().RightToLeft(27, 2));
							}

							int maxnumber = testiter->calculateMinRowNumber(Direction::RightToLeft, m_accRowCount, m_guiRowCount);
							std::vector<IndexCountHolder> testindiceslist;
							testindiceslist.push_back(IndexCountHolder(0,0, maxnumber));
							testindiceslist.push_back(IndexCountHolder(0,1, maxnumber));
							testindiceslist.push_back(IndexCountHolder(1,1, maxnumber));

							testindiceslist.push_back(IndexCountHolder(3,2, maxnumber));

							testindiceslist.push_back(IndexCountHolder(1,-1, maxnumber)); 
							testindiceslist.push_back(IndexCountHolder(1,-2, maxnumber)); 
							testindiceslist.push_back(IndexCountHolder(3,10, maxnumber)); 
							std::vector<IndexCountHolder>::const_iterator testindex;
							for (testindex = testindiceslist.begin(); testindex != testindiceslist.end(); ++testindex)
							{
								try
								{
									reset();
									testiter->restart(*this);
									CHECK(select());
									guiprop = getMatcher().getLeft();
									accprop = getMatcher().getRight();
									CHECK_EQUAL(0,guiprop.getAbsoluteIndex());

									CHECK_EQUAL(m_accRowCount, static_cast<int>(accprop.size()));
									CHECK_EQUAL(m_guiRowCount, static_cast<int>(guiprop.size()));

									guiprop.setMemoryPolicy(basar::gui::tie::WITHOUT_MEMORY);

									TEXTOUTPUT2("RightToLeft(int,int) with: " << testindex->toText());
									std::vector<IndexCountHolder> innerindexcountlist;
									std::vector<IndexCountHolder> indexcountlist;

									select();
									guiiter = getMatcher().getLeft().begin();
									acciter = getMatcher().getRight().begin();
									int i=0;
									for (; !guiiter.isEnd() && !acciter.isEnd();++guiiter,++acciter, ++i)
									{
										checkEqualToText(i, guiiter ,  *testiter);
										checkEqualToSelect(i, acciter , *testiter); 
									}
									indexcountlist.push_back(*testindex);
									int number = getMatcher().RightToLeft(indexcountlist[0].m_index, indexcountlist[0].m_count);
									CHECK_EQUAL(indexcountlist[0].m_number, number);
									TEXTOUTPUT2(testiter->toText() << "getMatcher().RightToLeft(" << indexcountlist[0].m_index << "," << indexcountlist[0].m_count << ") returns " << number << " calculated was " << indexcountlist[0].m_number );

								}
								catch (basar::Exception & ex)
								{
									std::cout << " Exception in" << testiter->toText()
										<< " (index,count,start,number)=(" << testindex->m_index << "," << testindex->m_count << "," << testindex->m_start << "," <<testindex->m_number << ")" << std::endl 
										<< ex.what() << std::endl;
									const bool ExceptionOccured3 = false;
									CHECK(ExceptionOccured3);
								}
								catch (...)
								{
									std::cout << "Non Basar Exception in" << testiter->toText()
										<< " (index,count,start,number)=(" << testindex->m_index << "," << testindex->m_count << "," << testindex->m_start << "," <<testindex->m_number << ")" 
										<< std::endl;
									const bool NonBasarExceptionOccured3 = false;
									CHECK(NonBasarExceptionOccured3);

								}
							}
						}
					}
					catch (basar::Exception & ex)
					{
						std::cout << " Exception in" << testiter->toText() << std::endl << ex.what() << std::endl;
						const bool ExceptionOccured2 = false;
						CHECK(ExceptionOccured2);
					}
					catch (...)
					{
						std::cout << "Non Basar Exception in" << testiter->toText() << std::endl;
						const bool NonBasarExceptionOccured2 = false;
						CHECK(NonBasarExceptionOccured2);
					}
				}
			}
			catch (basar::Exception & ex)
			{
				std::cout << ex.what() << std::endl;
				const bool ExceptionOccured1 = false;
				CHECK(ExceptionOccured1);
			}
			catch (...)
			{
				std::cout << "Exception ocurred" << std::endl;
				const bool NonBasarExceptionOccured1 = false;
				CHECK(NonBasarExceptionOccured1);
				throw;
			}
		}
	}

	// ------------------------------------------------------------------------------------------------------------------------------------------------
	TEST_FIXTURE(StringMatcherHolderMoreLines, GuiTieTest_Matcher_Strings_MoreLines_NoMemory_RightToLeft_int_withoutMemory)
	{
		if (g_RUNSHORT)
			return;

		BLOG_TRACE_METHOD(basar::test::unit::guitie::LoggerPool::logPerf, UnitTest::CurrentTest::Details()->testName);

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
						CHECK(testiter->start("GuiTieTest_Matcher_Strings_MoreLines_NoMemory_RightToLeft_int_withoutMemory", *this));
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
							CHECK_THROW( getMatcher().RightToLeft(0,1), basar::Exception);
							CHECK_THROW( getMatcher().RightToLeft(1,2), basar::Exception);
							CHECK_THROW( getMatcher().RightToLeft(-10,-10), basar::Exception);
						}
						else
						{   
							TEXTOUTPUT2(testiter->toText()); 
							// test exception throwing: 
							{             
								// negative index:
								CHECK_THROW(getMatcher().RightToLeft(-1, 1), basar::Exception);
								// beyond begin()
								CHECK_THROW(getMatcher().RightToLeft(0, -2), basar::Exception);
								// beyond begin()
								CHECK_THROW(getMatcher().RightToLeft(2, -27), basar::Exception); 
							}
							if (testiter->checkPolicy(basar::gui::tie::INSERT) || getRowTestNumber() == MORE_GUI)
							{
								CHECK_THROW(getMatcher().RightToLeft(27, 1), basar::Exception); 
								CHECK_THROW(getMatcher().RightToLeft(27, 0), basar::Exception); 
								CHECK_THROW(getMatcher().RightToLeft(27, 2), basar::Exception); 
							}
							else
							{
								CHECK_EQUAL(0,getMatcher().RightToLeft(27, 1));
								CHECK_EQUAL(0,getMatcher().RightToLeft(27, 0));
								CHECK_EQUAL(0,getMatcher().RightToLeft(27, 2));
							}

							int maxnumber = testiter->calculateMinRowNumber(Direction::RightToLeft, m_accRowCount, m_guiRowCount);
							std::vector<IndexCountHolder> testindiceslist;
							testindiceslist.push_back(IndexCountHolder(0,0, maxnumber));
							testindiceslist.push_back(IndexCountHolder(0,1, maxnumber));
							testindiceslist.push_back(IndexCountHolder(1,1, maxnumber));

							testindiceslist.push_back(IndexCountHolder(3,2, maxnumber));

							testindiceslist.push_back(IndexCountHolder(1,-1, maxnumber)); 
							testindiceslist.push_back(IndexCountHolder(1,-2, maxnumber)); 
							testindiceslist.push_back(IndexCountHolder(3,10, maxnumber)); 
							std::vector<IndexCountHolder>::const_iterator testindex;
							for (testindex = testindiceslist.begin(); testindex != testindiceslist.end(); ++testindex)
							{
								try
								{
									reset();
									testiter->restart(*this);
									CHECK(select());
									guiprop = getMatcher().getLeft();
									accprop = getMatcher().getRight();
									CHECK_EQUAL(0,guiprop.getAbsoluteIndex());

									CHECK_EQUAL(m_accRowCount, static_cast<int>(accprop.size()));
									CHECK_EQUAL(m_guiRowCount, static_cast<int>(guiprop.size()));

									guiprop.setMemoryPolicy(basar::gui::tie::WITHOUT_MEMORY);

									TEXTOUTPUT2("RightToLeft(int,int) with: " << testindex->toText());
									std::vector<IndexCountHolder> innerindexcountlist;
									std::vector<IndexCountHolder> indexcountlist;

									select();
									guiiter = getMatcher().getLeft().begin();
									acciter = getMatcher().getRight().begin();
									int i=0;
									for (; !guiiter.isEnd() && !acciter.isEnd();++guiiter,++acciter, ++i)
									{
										checkEqualToText(i, guiiter ,  *testiter);
										checkEqualToSelect(i,   acciter , *testiter); 
									}
									indexcountlist.push_back(*testindex);
									int number = getMatcher().RightToLeft(indexcountlist[0].m_index, indexcountlist[0].m_count);
									CHECK_EQUAL(indexcountlist[0].m_number, number);
									TEXTOUTPUT2(testiter->toText() << "getMatcher().RightToLeft(" << indexcountlist[0].m_index << "," << indexcountlist[0].m_count << ") returns " << number << " calculated was " << indexcountlist[0].m_number );

								}
								catch (basar::Exception & ex)
								{
									std::cout << " Exception in" << testiter->toText()
										<< " (index,count,start,number)=(" << testindex->m_index << "," << testindex->m_count << "," << testindex->m_start << "," <<testindex->m_number << ")" << std::endl 
										<< ex.what() << std::endl;
									const bool ExceptionOccured3 = false;
									CHECK(ExceptionOccured3);
								}
								catch (...)
								{
									std::cout << "Non Basar Exception in" << testiter->toText()
										<< " (index,count,start,number)=(" << testindex->m_index << "," << testindex->m_count << "," << testindex->m_start << "," <<testindex->m_number << ")" 
										<< std::endl;
									const bool NonBasarExceptionOccured3 = false;
									CHECK(NonBasarExceptionOccured3);

								}
							}
						}
					}
					catch (basar::Exception & ex)
					{
						std::cout << " Exception in" << testiter->toText() << std::endl << ex.what() << std::endl;
						const bool ExceptionOccured2 = false;
						CHECK(ExceptionOccured2);
					}
					catch (...)
					{
						std::cout << "Non Basar Exception in" << testiter->toText() << std::endl;
						const bool NonBasarExceptionOccured2 = false;
						CHECK(NonBasarExceptionOccured2);
					}
				}
			}
			catch (basar::Exception & ex)
			{
				std::cout << ex.what() << std::endl;
				const bool ExceptionOccured1 = false;
				CHECK(ExceptionOccured1);
			}
			catch (...)
			{
				std::cout << "Exception ocurred" << std::endl;
				const bool NonBasarExceptionOccured1 = false;
				CHECK(NonBasarExceptionOccured1);
				throw;
			}
		}
	}

	// ------------------------------------------------------------------------------------------------------------------------------------------------	
	TEST_FIXTURE(StringMatcherHolderMoreLines, GuiTieTest_Matcher_Strings_MoreLines_NoMemory_RightToLeft_int_withoutMemory_successiveTest)
	{
		BLOG_TRACE_METHOD(basar::test::unit::guitie::LoggerPool::logPerf, UnitTest::CurrentTest::Details()->testName);

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

				std::vector<BaseMatcherHolderTestSzenario> tests = BaseMatcherHolderTestSzenarioList::getPolicyTestSzenariosOneRowTest(MORE_ACC);
				std::vector<BaseMatcherHolderTestSzenario>::iterator testiter;
				for (testiter = tests.begin(); testiter != tests.end(); ++testiter)
				{
					try
					{
						reset();
						CHECK(testiter->start("GuiTieTest_Matcher_Strings_MoreLines_NoMemory_RightToLeft_int_withoutMemory_successiveTest", *this));
						TEXTOUTPUT(testiter->toText());
						CHECK(select());

						guiprop = getMatcher().getLeft();
						accprop = getMatcher().getRight();
						CHECK_EQUAL(0,guiprop.getAbsoluteIndex());

						CHECK_EQUAL(m_accRowCount, static_cast<int>(accprop.size()));
						CHECK_EQUAL(m_guiRowCount, static_cast<int>(guiprop.size()));

						guiprop.setMemoryPolicy(basar::gui::tie::WITHOUT_MEMORY);
						if (!testiter->throwsToManyEntriesException())
						{

							TEXTOUTPUT2(testiter->toText()); 

							int maxnumber = testiter->calculateMinRowNumber(Direction::RightToLeft, m_accRowCount, m_guiRowCount);
							std::vector<IndexCountHolder> testindiceslist;
							testindiceslist.push_back(IndexCountHolder(0,1, maxnumber));
							testindiceslist.push_back(IndexCountHolder(1,1, maxnumber));
							testindiceslist.push_back(IndexCountHolder(3,2, maxnumber));
							testindiceslist.push_back(IndexCountHolder(1,-1, maxnumber)); 
							testindiceslist.push_back(IndexCountHolder(1,-2, maxnumber)); 
							testindiceslist.push_back(IndexCountHolder(3,10, maxnumber)); 
							std::vector<IndexCountHolder>::const_iterator testindex;
							// successive:
							std::vector<IndexCountHolder> successiveindexcountlist;

							guiprop = getMatcher().getLeft();
							accprop = getMatcher().getRight();
							CHECK_EQUAL(0,guiprop.getAbsoluteIndex());

							CHECK_EQUAL(m_accRowCount, static_cast<int>(accprop.size()));
							CHECK_EQUAL(m_guiRowCount, static_cast<int>(guiprop.size()));

							guiprop.setMemoryPolicy(basar::gui::tie::WITHOUT_MEMORY);

							guiiter =  getMatcher().getLeft().begin();
							acciter = getMatcher().getRight().begin();
							int i=0;
							for (; !guiiter.isEnd() && !acciter.isEnd();++guiiter,++acciter, ++i)
							{
								checkEqualToText(i, guiiter ,  *testiter);
								checkEqualToSelect(i,   acciter , *testiter); 
							}
							for (testindex = testindiceslist.begin(); testindex != testindiceslist.end(); ++testindex)
							{
								try
								{
									TEXTOUTPUT2("added (index,count,start,number)=(" << testindex->m_index << "," << testindex->m_count << "," << testindex->m_start << "," <<testindex->m_number << ")");

									// test 1 successive RightToLeft
									{
										successiveindexcountlist.push_back(*testindex);
										getMatcher().RightToLeft(testindex->m_index, testindex->m_count);
										checkLinesAfterIndexRightToLeftMatcher(successiveindexcountlist, maxnumber,*testiter);
									}

								}
								catch (basar::Exception & ex)
								{
									std::cout << " Exception in" << testiter->toText()
										<< " (index,count,start,number)=(" << testindex->m_index << "," << testindex->m_count << "," << testindex->m_start << "," <<testindex->m_number << ")" << std::endl 
										<< ex.what() << std::endl;
									const bool ExceptionOccured4 = false;
									CHECK(ExceptionOccured4);
								}
								catch (...)
								{
									std::cout << "Non Basar Exception in" << testiter->toText()
										<< " (index,count,start,number)=(" << testindex->m_index << "," << testindex->m_count << "," << testindex->m_start << "," <<testindex->m_number << ")" 
										<< std::endl;
									const bool NonBasarExceptionOccured4 = false;
									CHECK(NonBasarExceptionOccured4);
								}
							}

						}
					}
					catch (basar::Exception & ex)
					{
						std::cout << " Exception in" << testiter->toText() << std::endl << ex.what() << std::endl;
						const bool ExceptionOccured2 = false;
						CHECK(ExceptionOccured2);
					}
					catch (...)
					{
						std::cout << "Non Basar Exception in" << testiter->toText() << std::endl;
						const bool NonBasarExceptionOccured2 = false;
						CHECK(NonBasarExceptionOccured2);
					}
				}
			}
			catch (basar::Exception & ex)
			{
				std::cout << ex.what() << std::endl;
				const bool ExceptionOccured1 = false;
				CHECK(ExceptionOccured1);
			}
			catch (...)
			{
				std::cout << "Exception ocurred" << std::endl;
				const bool NonBasarExceptionOccured1 = false;
				CHECK(NonBasarExceptionOccured1);
				throw;
			}
		}
	}


	// ------------------------------------------------------------------------------------------------------------------------------------------------
	TEST_FIXTURE(StringMatcherHolderMoreLines, GuiTieTest_Matcher_Strings_MoreLines_NoMemory_LeftToRight_int_withoutMemory)
	{
		if (g_RUNSHORT)
			return;

		BLOG_TRACE_METHOD(basar::test::unit::guitie::LoggerPool::logPerf, UnitTest::CurrentTest::Details()->testName);

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
							// overflow:
							{
								int start = 3;
								int count = 10;

								//std::cout << "hallo " << testiter->toText() << " rowcount =  " << this->m_tableWidget->rowCount() << std::endl;
								if (testiter->checkPolicy(basar::gui::tie::INSERT))
								{
									CHECK_THROW(getMatcher().LeftToRight(start, count), basar::Exception);
								}
								else
								{
									if (getRowTestNumber() == MORE_GUI)
									{
										CHECK_EQUAL(3,getMatcher().LeftToRight(start, count));
								    }
								   else  
								    {
									 CHECK_THROW(getMatcher().LeftToRight(start, count), basar::Exception);
								    } 
								}
							}
							// various, working tests
							{
								int maxnumber = testiter->calculateMinRowNumber(Direction::LeftToRight, m_accRowCount, m_guiRowCount);
								std::vector<IndexCountHolder> indexcountlist;
								indexcountlist.push_back(IndexCountHolder(0,1, maxnumber));
								indexcountlist.push_back(IndexCountHolder(1,1, maxnumber)); 
								indexcountlist.push_back(IndexCountHolder(1,-1, maxnumber)); 
								// negative overflow:
								indexcountlist.push_back(IndexCountHolder(1,-2, maxnumber));  

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
										checkAfterLeftToRightIndexCount(*indexcount,*testiter);
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

	// ------------------------------------------------------------------------------------------------------------------------------------------------

}

// ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~  ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~
