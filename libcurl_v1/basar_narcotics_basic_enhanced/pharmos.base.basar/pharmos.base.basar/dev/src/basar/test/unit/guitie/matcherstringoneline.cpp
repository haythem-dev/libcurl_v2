/*
* @file                                            
* @author Anke Klink                      
* @date 2008                            
*/    

#include <UnitTest++/UnitTest++.h>
#include "libbasarguitie.h"

#include "libbasardbaspect_connectionref.h"
#include "libbasardbaspect_accessorpropertytable_yiterator.h"
#include "libbasarproperty_propertytable_xiterator.h"
#include "libbasarproperty_propertystate.h"

#include "libbasardbaspect.h"

#include "testconnection.h"
#include "basematcherholder.h"
#include "basematcherholdertestszenario.h"

#include "buttablepropertydescriptions.h"
#include "accessornames.h"
#include "basematcherholdertestszenariolist.h"
#include "matcherholderlist.h"
#include "loggerpool.h"

#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>

#include "matcherstringonelinemockcessor.h"
#include "matcherstringonelinestringsreversemockcessor.h"
#include "matcherstringonelinetwostringsreversemockcessor.h"

#include <boost/make_shared.hpp>

using basar::ConstString;
using basar::VarString;

using basar::gui::tie::MatcherRef;
using basar::gui::tie::GuiPropertyTable_YIterator;

using basar::db::aspect::AccessorInstanceRef;
using basar::db::aspect::ConnectionRef;
using basar::db::aspect::AccessorPropertyTable_YIterator;
using basar::db::aspect::ExecuteResultInfo;

using basar::test::unit::guitie::TestConnection;
using basar::test::unit::guitie::BaseMatcherHolder;
using basar::test::unit::guitie::BaseMatcherHolderTestSzenario;
using basar::test::unit::guitie::MatcherHolderList;


using basar::test::unit::guitie::BaseMatcherHolderTestSzenarioList;

extern bool g_RUNSHORT;

//------------------------------------------------------------------------------
SUITE(GuiTieTest_Matcher_Strings_OneLine)
{
//#define TEXTOUTPUT(XXX) std::cout << XXX << std::endl;   

	typedef basar::gui::tie::MatcherPolicyType MatcherPolicyType;

	static const VarString STRING1 = PROPDEF_STRING1.getName();
	static const VarString STRING2 = PROPDEF_STRING2.getName(); 
	// ----------------------------------------------------------------------------------------------------------
	class StringMatcherHolderOneLine : public BaseMatcherHolder
	{ 
	private:

		bool m_inverse;
		bool m_init;  

		QLabel * m_labelString1;
		QLabel * m_labelString2;
		QLabel * m_labelString3;
	public:
		std::vector< basar::VarString > m_text;

		StringMatcherHolderOneLine()
		{ 
			m_text.push_back("A");
			m_text.push_back("B");
			m_text.push_back("C");
			m_labelString1 = m_labelString2 = m_labelString3 = 0;
			m_inverse = m_init = false;
		}

		~StringMatcherHolderOneLine()
		{
			if (0 != m_labelString3)
			{
				m_labelString3->setParent(m_widget);
			}
		}


		bool resetGuiValues()
		{
			m_labelString1->setText(m_text.at(0).c_str());
			m_labelString2->setText(m_text.at(1).c_str());
			m_labelString3->setText(m_text.at(2).c_str());
			return true;
		}

		bool init(bool inverse = false) 
		{
			bool retval  = true;
			if (m_init && inverse != m_inverse)
			{
				retval = false;   
			}
			else if (!m_init )
			{
				m_inverse = inverse;
				if (m_inverse)
				{ 
					m_labelString2 = new QLabel(m_widget);
					m_labelString1 = new QLabel(m_widget);

                    if (m_AccessorMap.end() == m_AccessorMap.find(ACCESSORNAME_SELECT_TWOSTRINGS_REVERSE))
                    {
                        basar::db::aspect::IAccessorSharedPtr acc = boost::make_shared<MatcherStringOneLineTwoStringsReverseMockCessor>(ACCESSORNAME_SELECT_TWOSTRINGS_REVERSE);
                        m_AccessorMap.insert(AccessorMap::value_type(ACCESSORNAME_SELECT_TWOSTRINGS_REVERSE, acc));
                    }

                    if (m_AccessorMap.end() == m_AccessorMap.find(ACCESSORNAME_SELECT_STRINGS_REVERSE))
                    {
                        basar::db::aspect::IAccessorSharedPtr acc = boost::make_shared<MatcherStringOneLineStringsReverseMockCessor>(ACCESSORNAME_SELECT_STRINGS_REVERSE);
                        m_AccessorMap.insert(AccessorMap::value_type(ACCESSORNAME_SELECT_STRINGS_REVERSE, acc));
                    }

					retval = setAccessorInfo(
						ACCESSORNAME_SELECT_TWOSTRINGS_REVERSE,
						ACCESSORNAME_SELECT_STRINGS_REVERSE,
						ACCESSORMETHODNAME_SELECT_ONELINE,
						ACCESSORMETHODNAME_SELECT_ONELINE,
						false);
				}
				else
				{
					m_labelString1 = new QLabel(m_widget);
					m_labelString2 = new QLabel(m_widget);

                    if (m_AccessorMap.end() == m_AccessorMap.find(ACCESSORNAME_SELECT_TWOSTRINGS))
                    {
                        basar::db::aspect::IAccessorSharedPtr acc = boost::make_shared<MatcherStringOneLineMockCessor>(ACCESSORNAME_SELECT_TWOSTRINGS);
                        m_AccessorMap.insert(AccessorMap::value_type(ACCESSORNAME_SELECT_TWOSTRINGS, acc));
                    }

                    if (m_AccessorMap.end() == m_AccessorMap.find(ACCESSORNAME_SELECT_STRINGS))
                    {
                        basar::db::aspect::IAccessorSharedPtr acc = boost::make_shared<MatcherStringOneLineMockCessor>(ACCESSORNAME_SELECT_STRINGS);
                        m_AccessorMap.insert(AccessorMap::value_type(ACCESSORNAME_SELECT_STRINGS, acc));
                    }

					retval = setAccessorInfo( 
						ACCESSORNAME_SELECT_TWOSTRINGS,
						ACCESSORNAME_SELECT_STRINGS,
						ACCESSORMETHODNAME_SELECT_ONELINE,
						ACCESSORMETHODNAME_SELECT_ONELINE,
						false);
				}

				m_labelString2->setObjectName (STRING2.c_str());
				m_labelString1->setObjectName (STRING1.c_str());

				m_labelString3 = new QLabel(0);
				m_labelString3->setObjectName (PROPDEF_STRING3.getName().c_str());

				resetGuiValues();

				if (retval)
				{
					retval = resetAccessorAndMatcher();
					m_init = retval; 
				}


			}
			return retval;
		}

		bool start()
		{
			bool retval = true;
			if (!m_init)
			{
				retval =  init();
			}
			if (retval)
			{
				resetGuiValues();

				if (hasMoreGuiNamesTest())
				{
					m_labelString3->setParent(m_widget);
				}
				else 
				{
					m_labelString3->setParent(0);
				}
				m_needsReset = true;
				retval =  BaseMatcherHolder::start();
			}
			return retval;
		}

	};
	// ----------------------------------------------------------------------------------------------------------
	struct TwoStringMatcherHolderOneLine : public MatcherHolderList<StringMatcherHolderOneLine>
	{
		boost::shared_ptr<StringMatcherHolderOneLine> m_one;
		boost::shared_ptr<StringMatcherHolderOneLine> m_reverse;

		TwoStringMatcherHolderOneLine()
		{

		}

		~TwoStringMatcherHolderOneLine()
		{
		}

		bool init()
		{
			bool retval = false;
			m_one = boost::shared_ptr<StringMatcherHolderOneLine>(new StringMatcherHolderOneLine);
			m_reverse = boost::shared_ptr<StringMatcherHolderOneLine>(new StringMatcherHolderOneLine);
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

	// ----------------------------------------------------------------------------------------------------------
	TEST_FIXTURE(StringMatcherHolderOneLine, GuiTieTest_Matcher_Strings_OneLine_Start)
	{
		BLOG_TRACE_METHOD(basar::test::unit::guitie::LoggerPool::logPerf, m_details.testName);

		try
		{
			CHECK(init());

			GuiPropertyTable_YIterator guiiter =  getMatcher().getLeft().begin();
			AccessorPropertyTable_YIterator acciter = getMatcher().getRight().begin();

			CHECK(!guiiter.isEnd());
			CHECK(acciter.isEnd());
			CHECK(acciter.isBegin());
			CHECK(guiiter.isBegin());
			CHECK(!acciter.isNull());
			CHECK(!guiiter.isNull());


			select();
			CHECK(select());

			guiiter = getMatcher().getLeft().begin();
			acciter = getMatcher().getRight().begin();

			CHECK(!guiiter.isEnd());
			CHECK(!acciter.isEnd());
			CHECK(acciter.isBegin());
			CHECK(guiiter.isBegin());
			CHECK(!acciter.isNull());
			CHECK(!acciter.isNull());
		}
		catch (basar::Exception & ex)
		{
			std::cout << ex.what() << std::endl;
			const bool ExceptionOccured = false;
			CHECK(ExceptionOccured);
		}
	}

	// ----------------------------------------------------------------------------------------------------------
	TEST_FIXTURE(TwoStringMatcherHolderOneLine, GuiTieTest_TwoMatcher_Start)
	{
		BLOG_TRACE_METHOD(basar::test::unit::guitie::LoggerPool::logPerf, m_details.testName);

		try
		{
			CHECK(init());
			GuiPropertyTable_YIterator guiiter1 =  m_one->getMatcher().getLeft().begin();
			AccessorPropertyTable_YIterator acciter1 = m_one->getMatcher().getRight().begin();
			GuiPropertyTable_YIterator guiiter2 =  m_reverse->getMatcher().getLeft().begin();
			AccessorPropertyTable_YIterator acciter2 =m_reverse-> getMatcher().getRight().begin();

			CHECK(m_one->start( ));
			CHECK(m_reverse->start());

			CHECK(select());

			guiiter1 =  m_one->getMatcher().getLeft().begin();
			acciter1 =  m_one->getMatcher().getRight().begin();
			guiiter2 =  m_reverse->getMatcher().getLeft().begin();
			acciter2 =  m_reverse->getMatcher().getRight().begin();

			CHECK_EQUAL(acciter1.getString(STRING1),acciter2.getString(STRING1));
			CHECK_EQUAL(acciter1.getString(STRING2),acciter2.getString(STRING2));

			CHECK_EQUAL(STRING1, guiiter1.getName(0));
			CHECK_EQUAL(STRING2, guiiter1.getName(1));
			CHECK_EQUAL(STRING1, guiiter2.getName(1));
			CHECK_EQUAL(STRING2, guiiter2.getName(0));

			CHECK_EQUAL(STRING1, acciter1.getName(0));
			CHECK_EQUAL(STRING2, acciter1.getName(1));
			CHECK_EQUAL(STRING1, acciter2.getName(1));
			CHECK_EQUAL(STRING2, acciter2.getName(0));

			CHECK_EQUAL (acciter1.getString(0),acciter2.getString(1));
			CHECK_EQUAL (acciter1.getString(0),acciter2.getString(1));
			CHECK (acciter1.getString(0) != acciter2.getString(0));
			CHECK (acciter1.getString(1)!= acciter2.getString(1));

			CHECK_EQUAL (guiiter1.getString(0),guiiter2.getString(1));
			CHECK_EQUAL (guiiter1.getString(0),guiiter2.getString(1));
			CHECK (guiiter1.getString(0) != guiiter2.getString(0));
			CHECK (guiiter1.getString(1)!= guiiter2.getString(1));

		}
		catch (basar::Exception & ex)
		{
			std::cout << ex.what() << std::endl;
			const bool ExceptionOccured = false;
			CHECK(ExceptionOccured);
		}
	}

	// ----------------------------------------------------------------------------------------------------------
	TEST_FIXTURE(StringMatcherHolderOneLine, GuiTieTest_Matcher_Strings_OneLine_RightToLeft_NoVar)
	{
		BLOG_TRACE_METHOD(basar::test::unit::guitie::LoggerPool::logPerf, m_details.testName);

		try
		{
			CHECK(init());

			GuiPropertyTable_YIterator guiiter =  getMatcher().getLeft().begin();
			AccessorPropertyTable_YIterator acciter = getMatcher().getRight().begin();

			std::vector<BaseMatcherHolderTestSzenario> tests = BaseMatcherHolderTestSzenarioList::getPolicyTestSzenariosOneRowTest(NORM);
			std::vector<BaseMatcherHolderTestSzenario>::iterator testiter;
			for (testiter = tests.begin(); testiter != tests.end(); ++testiter)
			{
				try
				{
					CHECK(testiter->start("GuiTieTest_Matcher_Strings_OneLine_RightToLeft_NoVar", *this));
					TEXTOUTPUT(testiter->toText());
					CHECK(select());

					guiiter =  getMatcher().getLeft().begin();
					acciter =  getMatcher().getRight().begin();

					CHECK_EQUAL(m_text.at(0),guiiter.getString(STRING1));
					CHECK_EQUAL(m_text.at(1),guiiter.getString(STRING2));

					if (testiter->throwsToManyEntriesException())
					{
						CHECK_THROW( getMatcher().RightToLeft(), basar::Exception);
					}
					else
					{

						CHECK_EQUAL(1,getMatcher().RightToLeft());

						guiiter = getMatcher().getLeft().begin();
						acciter = getMatcher().getRight().begin();

						CHECK_EQUAL(acciter.getString(STRING1),guiiter.getString(STRING1));
						CHECK_EQUAL(acciter.getString(STRING2),guiiter.getString(STRING2));

						CHECK(!(m_text.at(0)== guiiter.getString(STRING1)));
						CHECK(!(m_text.at(1) == guiiter.getString(STRING2)));

						resetGuiValues();

						CHECK_EQUAL(m_text.at(0),guiiter.getString(STRING1));
						CHECK_EQUAL(m_text.at(1),guiiter.getString(STRING2));

						CHECK_EQUAL(1,getMatcher().LeftToRight());

						CHECK_EQUAL(guiiter.getString(STRING1), acciter.getString(STRING1));
						CHECK_EQUAL(guiiter.getString(STRING2),acciter.getString(STRING2));

						CHECK_EQUAL(m_text.at(0),acciter.getString(STRING1));
						CHECK_EQUAL(m_text.at(1),acciter.getString(STRING2));
					}
				}
				catch (basar::Exception & ex)
				{
					std::cout << " Exception in" << testiter->toText() << std::endl << ex.what() << std::endl;
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

	// ----------------------------------------------------------------------------------------------------------
	TEST_FIXTURE(StringMatcherHolderOneLine, GuiTieTest_Matcher_Strings_OneLine_LeftToRight_NoVar)
	{
		BLOG_TRACE_METHOD(basar::test::unit::guitie::LoggerPool::logPerf, m_details.testName);

		try
		{
			CHECK(init());

			GuiPropertyTable_YIterator guiiter =  getMatcher().getLeft().begin();
			AccessorPropertyTable_YIterator acciter = getMatcher().getRight().begin();

			std::vector<BaseMatcherHolderTestSzenario> tests = BaseMatcherHolderTestSzenarioList::getPolicyTestSzenariosOneRowTest(NORM);
			std::vector<BaseMatcherHolderTestSzenario>::iterator testiter;
			for (testiter = tests.begin(); testiter != tests.end(); ++testiter)
			{
				try
				{
					CHECK(testiter->start("GuiTieTest_Matcher_Strings_OneLine_LeftToRight_NoVar", *this));

					CHECK(select());

					guiiter =  getMatcher().getLeft().begin();
					acciter = getMatcher().getRight().begin();


					CHECK_EQUAL(m_text.at(0),guiiter.getString(STRING1));
					CHECK_EQUAL(m_text.at(1),guiiter.getString(STRING2));


					if (testiter->throwsToManyEntriesException())
					{
						CHECK_THROW( getMatcher().LeftToRight(), basar::Exception);
						TEXTOUTPUT("SKIPEXCEPTION");
					}
					else
					{
						CHECK_EQUAL(1,getMatcher().LeftToRight());

						guiiter = getMatcher().getLeft().begin();
						acciter = getMatcher().getRight().begin();

						CHECK_EQUAL(guiiter.getString(STRING1),acciter.getString(STRING1));
						CHECK_EQUAL(guiiter.getString(STRING2),acciter.getString(STRING2));

						CHECK_EQUAL(m_text.at(0),acciter.getString(STRING1));
						CHECK_EQUAL(m_text.at(1),acciter.getString(STRING2));
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

	// ----------------------------------------------------------------------------------------------------------
	TEST_FIXTURE (StringMatcherHolderOneLine, GuiTieTest_Matcher_Strings_OneLine_RightToLeft_Iter_internal)
	{
		BLOG_TRACE_METHOD(basar::test::unit::guitie::LoggerPool::logPerf, m_details.testName);

		try
		{
			CHECK(init());
			GuiPropertyTable_YIterator guiiter =  getMatcher().getLeft().begin();
			AccessorPropertyTable_YIterator acciter = getMatcher().getRight().begin();

			std::vector<BaseMatcherHolderTestSzenario> tests = BaseMatcherHolderTestSzenarioList::getPolicyTestSzenariosOneRowTest(NORM);
			std::vector<BaseMatcherHolderTestSzenario>::iterator testiter;
			for (testiter = tests.begin(); testiter != tests.end(); ++testiter)
			{
				try
				{        
					CHECK(testiter->start("GuiTieTest_Matcher_Strings_OneLine_RightToLeft_Iter_reverseTest", *this));
					CHECK(select());

					guiiter =  getMatcher().getLeft().begin();
					acciter =  getMatcher().getRight().begin();

					CHECK_EQUAL(m_text.at(0),guiiter.getString(STRING1));
					CHECK_EQUAL(m_text.at(1),guiiter.getString(STRING2));


					if (testiter->throwsToManyEntriesException())
					{
						CHECK_THROW(getMatcher().RightToLeft(guiiter, acciter), basar::Exception);
					}
					else
					{

						getMatcher().RightToLeft(guiiter, acciter);

						guiiter = getMatcher().getLeft().begin();
						acciter = getMatcher().getRight().begin();

						CHECK_EQUAL(acciter.getString(STRING1),guiiter.getString(STRING1));
						CHECK_EQUAL(acciter.getString(STRING2),guiiter.getString(STRING2));

						CHECK(!(m_text.at(0)== guiiter.getString(STRING1)));
						CHECK(!(m_text.at(1) == guiiter.getString(STRING2)));

						resetGuiValues();

						CHECK_EQUAL(m_text.at(0),guiiter.getString(STRING1));
						CHECK_EQUAL(m_text.at(1),guiiter.getString(STRING2));

						getMatcher().LeftToRight(guiiter, acciter);

						CHECK_EQUAL(guiiter.getString(STRING1),acciter.getString(STRING1));
						CHECK_EQUAL(guiiter.getString(STRING2),acciter.getString(STRING2));

						CHECK_EQUAL(m_text.at(0),acciter.getString(STRING1));
						CHECK_EQUAL(m_text.at(1),acciter.getString(STRING2));
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
			const bool ExceptionOccured = false;
			CHECK(ExceptionOccured);
		}


	}

	// ----------------------------------------------------------------------------------------------------------
	TEST_FIXTURE(StringMatcherHolderOneLine, GuiTieTest_Matcher_Strings_OneLine_LeftToRight_Iter_internal)
	{
		BLOG_TRACE_METHOD(basar::test::unit::guitie::LoggerPool::logPerf, m_details.testName);

		try
		{
			CHECK(init());

			GuiPropertyTable_YIterator guiiter =  getMatcher().getLeft().begin();
			AccessorPropertyTable_YIterator acciter = getMatcher().getRight().begin();

			std::vector<BaseMatcherHolderTestSzenario> tests = BaseMatcherHolderTestSzenarioList::getPolicyTestSzenariosOneRowTest(NORM);
			std::vector<BaseMatcherHolderTestSzenario>::iterator testiter;
			for (testiter = tests.begin(); testiter != tests.end(); ++testiter)
			{
				try
				{
					CHECK(testiter->start("GuiTieTest_Matcher_Strings_OneLine_LeftToRight_Iter_internal", *this));

					CHECK(select());

					guiiter =  getMatcher().getLeft().begin();
					acciter = getMatcher().getRight().begin();


					CHECK_EQUAL(m_text.at(0),guiiter.getString(STRING1));
					CHECK_EQUAL(m_text.at(1),guiiter.getString(STRING2));



					if (testiter->throwsToManyEntriesException())
					{
						CHECK_THROW(getMatcher().LeftToRight(guiiter, acciter), basar::Exception);
					}
					else
					{

						getMatcher().LeftToRight(guiiter, acciter);

						guiiter = getMatcher().getLeft().begin();
						acciter = getMatcher().getRight().begin();

						CHECK_EQUAL(acciter.getString(STRING1),guiiter.getString(STRING1));
						CHECK_EQUAL(acciter.getString(STRING2),guiiter.getString(STRING2));

						CHECK_EQUAL(m_text.at(0),acciter.getString(STRING1));
						CHECK_EQUAL(m_text.at(1),acciter.getString(STRING2));
					}
				}
				catch (basar::Exception & ex)
				{
					std::cout << "Exception in" << testiter->toText() << std::endl
						<< ex.what() << std::endl;
					const bool ExceptionOccured = false;
					CHECK(ExceptionOccured);
					//  
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

	// ----------------------------------------------------------------------------------------------------------
	TEST_FIXTURE (TwoStringMatcherHolderOneLine, GuiTieTest_Matcher_Strings_OneLine_RightToLeft_Iter_reverseTest)
	{
		BLOG_TRACE_METHOD(basar::test::unit::guitie::LoggerPool::logPerf, m_details.testName);

		try
		{
			CHECK(init());
			GuiPropertyTable_YIterator guiiter1 =  m_one->getMatcher().getLeft().begin();
			AccessorPropertyTable_YIterator acciter1 = m_one->getMatcher().getRight().begin();
			GuiPropertyTable_YIterator guiiter2 =  m_reverse->getMatcher().getLeft().begin();
			AccessorPropertyTable_YIterator acciter2 =m_reverse-> getMatcher().getRight().begin();

			std::vector<BaseMatcherHolderTestSzenario> tests = BaseMatcherHolderTestSzenarioList::getPolicyTestSzenariosOneRowTest(BaseMatcherHolder::NORM);
			std::vector<BaseMatcherHolderTestSzenario>::iterator testiter;
			for (testiter = tests.begin(); testiter != tests.end(); ++testiter)
			{
				try
				{
					CHECK(testiter->start("GuiTieTest_Matcher_Strings_OneLine_RightToLeft_Iter_reverseTest_A", *(m_one.get())));
					CHECK(testiter->start("GuiTieTest_Matcher_Strings_OneLine_RightToLeft_Iter_reverseTest_B", *(m_reverse.get())));

					CHECK(select());

					guiiter1 =  m_one->getMatcher().getLeft().begin();
					acciter1 =  m_one->getMatcher().getRight().begin();
					guiiter2 =  m_reverse->getMatcher().getLeft().begin();
					acciter2 =  m_reverse->getMatcher().getRight().begin();

					CHECK_EQUAL(STRING1, guiiter1.getName(0));
					CHECK_EQUAL(STRING2, guiiter1.getName(1));
					CHECK_EQUAL(STRING1, guiiter2.getName(1));
					CHECK_EQUAL(STRING2, guiiter2.getName(0));

					CHECK_EQUAL(STRING1, acciter1.getName(0));
					CHECK_EQUAL(STRING2, acciter1.getName(1));
					CHECK(STRING1!= acciter2.getName(0));
					CHECK(STRING2!= acciter2.getName(1));

					if (testiter->throwsToManyEntriesException())
					{
						CHECK_THROW(m_one->getMatcher().RightToLeft(guiiter1, acciter1), basar::Exception);
					}
					else
					{
						// 1. nor acc1 -> gui1
						{
							m_one->getMatcher().RightToLeft(guiiter1, acciter1);

							CHECK_EQUAL(acciter1.getString(STRING1),guiiter1.getString(STRING1));
							CHECK_EQUAL(acciter1.getString(STRING2),guiiter1.getString(STRING2));

						}

						// 2.  mixed test acc2 -> gui1
						{
							m_one->getMatcher().RightToLeft(guiiter1, acciter2);
							CHECK_EQUAL(acciter2.getString(STRING1),guiiter1.getString(STRING1));
							CHECK_EQUAL(acciter2.getString(STRING2),guiiter1.getString(STRING2));

							CHECK(!(m_one->m_text.at(0)== guiiter1.getString(STRING1)));
							CHECK(!(m_one->m_text.at(1) == guiiter1.getString(STRING2)));

							m_one->resetGuiValues();


							CHECK_EQUAL(m_one->m_text.at(0),guiiter1.getString(STRING1));
							CHECK_EQUAL(m_one->m_text.at(1),guiiter1.getString(STRING2));

							m_one->getMatcher().LeftToRight(guiiter1, acciter2);

							CHECK_EQUAL(acciter2.getString(STRING1),guiiter1.getString(STRING1));
							CHECK_EQUAL(acciter2.getString(STRING2),guiiter1.getString(STRING2));

							CHECK_EQUAL(m_one->m_text.at(0),acciter2.getString(STRING1));
							CHECK_EQUAL(m_one->m_text.at(1),acciter2.getString(STRING2));
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
			const bool ExceptionOccured = false;
			CHECK(ExceptionOccured);

		}

	}

	// ----------------------------------------------------------------------------------------------------------
	TEST_FIXTURE (TwoStringMatcherHolderOneLine, GuiTieTest_Matcher_Strings_OneLine_LeftToRight_Iter_reverseTest)
	{
		BLOG_TRACE_METHOD(basar::test::unit::guitie::LoggerPool::logPerf, m_details.testName);

		try
		{
			CHECK(init());
			GuiPropertyTable_YIterator guiiter1 =  m_one->getMatcher().getLeft().begin();
			AccessorPropertyTable_YIterator acciter1 = m_one->getMatcher().getRight().begin();
			GuiPropertyTable_YIterator guiiter2 =  m_reverse->getMatcher().getLeft().begin();
			AccessorPropertyTable_YIterator acciter2 =m_reverse-> getMatcher().getRight().begin();

			std::vector<BaseMatcherHolderTestSzenario> tests = BaseMatcherHolderTestSzenarioList::getPolicyTestSzenariosOneRowTest(BaseMatcherHolder::NORM);
			std::vector<BaseMatcherHolderTestSzenario>::iterator testiter;
			for (testiter = tests.begin(); testiter != tests.end(); ++testiter)
			{
				try
				{
					CHECK(testiter->start("GuiTieTest_Matcher_Strings_OneLine_LeftToRight_Iter_reverseTest_A", *(m_one.get())));
					CHECK(testiter->start("GuiTieTest_Matcher_Strings_OneLine_LeftToRight_Iter_reverseTest_B", *(m_reverse.get())));

					CHECK(select());

					guiiter1 =  m_one->getMatcher().getLeft().begin();
					acciter1 =  m_one->getMatcher().getRight().begin();
					guiiter2 =  m_reverse->getMatcher().getLeft().begin();
					acciter2 =  m_reverse->getMatcher().getRight().begin();

					CHECK_EQUAL(STRING1, guiiter1.getName(0));
					CHECK_EQUAL(STRING2, guiiter1.getName(1));
					CHECK_EQUAL(STRING1, guiiter2.getName(1));
					CHECK_EQUAL(STRING2, guiiter2.getName(0));

					CHECK_EQUAL(STRING1, acciter1.getName(0));
					CHECK_EQUAL(STRING2, acciter1.getName(1));
					CHECK(STRING1!= acciter2.getName(0));
					CHECK(STRING2!= acciter2.getName(1));

					if (testiter->throwsToManyEntriesException())
					{
						CHECK_THROW(m_one->getMatcher().LeftToRight(guiiter1, acciter1), basar::Exception);
					}
					else
					{

						// 1. nor acc1 -> gui1
						{
							m_one->getMatcher().LeftToRight(guiiter1, acciter1);

							CHECK_EQUAL(acciter1.getString(STRING1),guiiter1.getString(STRING1));
							CHECK_EQUAL(acciter1.getString(STRING2),guiiter1.getString(STRING2));

						}

						// 2.  mixed test acc2 -> gui1
						{

							m_one->getMatcher().LeftToRight(guiiter1, acciter2);

							CHECK_EQUAL(acciter2.getString(STRING1),guiiter1.getString(STRING1));
							CHECK_EQUAL(acciter2.getString(STRING2),guiiter1.getString(STRING2));

							CHECK_EQUAL(m_one->m_text.at(0),acciter2.getString(STRING1));
							CHECK_EQUAL(m_one->m_text.at(1),acciter2.getString(STRING2));
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
			const bool ExceptionOccured = false;
			CHECK(ExceptionOccured);

		}
	}
	// ----------------------------------------------------------------------------------------------------------
	TEST_FIXTURE (StringMatcherHolderOneLine, GuiTieTest_Matcher_Strings_OneLine_RightToLeft_int_oneLine)
	{
		BLOG_TRACE_METHOD(basar::test::unit::guitie::LoggerPool::logPerf, m_details.testName);

		try
		{
			CHECK(init());
			GuiPropertyTable_YIterator guiiter =  getMatcher().getLeft().begin();
			AccessorPropertyTable_YIterator acciter = getMatcher().getRight().begin();

			std::vector<BaseMatcherHolderTestSzenario> tests = BaseMatcherHolderTestSzenarioList::getPolicyTestSzenariosOneRowTest(NORM);
			std::vector<BaseMatcherHolderTestSzenario>::iterator testiter;
			for (testiter = tests.begin(); testiter != tests.end(); ++testiter)
			{
				try
				{        
					CHECK(testiter->start("GuiTieTest_Matcher_Strings_OneLine_RightToLeft_int_oneLine", *this));
					CHECK(select());

					guiiter =  getMatcher().getLeft().begin();
					acciter =  getMatcher().getRight().begin();

					CHECK_EQUAL(m_text.at(0),guiiter.getString(STRING1));
					CHECK_EQUAL(m_text.at(1),guiiter.getString(STRING2));

					if (testiter->throwsToManyEntriesException())
					{
						CHECK_THROW(getMatcher().RightToLeft(0, 1), basar::Exception);
					}
					else
					{
						std::vector<int> countvec;
						countvec.push_back(1);    // one line 
						countvec.push_back(2);    // more lines, are ok (no exception)
						std::vector<int>::const_iterator count;
						for (count = countvec.begin(); count != countvec.end(); ++count)
						{
							CHECK_EQUAL(1,getMatcher().RightToLeft(0, *count));

							guiiter = getMatcher().getLeft().begin();
							acciter = getMatcher().getRight().begin();

							CHECK_EQUAL(acciter.getString(STRING1),guiiter.getString(STRING1));
							CHECK_EQUAL(acciter.getString(STRING2),guiiter.getString(STRING2));

							CHECK(!(m_text.at(0) == guiiter.getString(STRING1)));
							CHECK(!(m_text.at(1) == guiiter.getString(STRING2)));

							resetGuiValues();

							CHECK_EQUAL(m_text.at(0),guiiter.getString(STRING1));
							CHECK_EQUAL(m_text.at(1),guiiter.getString(STRING2));

							// use of indices that are not present:
							//CHECK_THROW(getMatcher().RightToLeft(1, *count), basar::Exception);
							//CHECK_THROW(getMatcher().RightToLeft(-1, *count), basar::Exception);
							CHECK_THROW(getMatcher().RightToLeft(2, *count), basar::Exception);
							CHECK_THROW(getMatcher().RightToLeft(14, *count), basar::Exception);
							CHECK_THROW(getMatcher().RightToLeft(0, -2), basar::Exception);

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
			const bool ExceptionOccured = false;
			CHECK(ExceptionOccured);
		}
	}

	// ----------------------------------------------------------------------------------------------------------
	TEST_FIXTURE(StringMatcherHolderOneLine, GuiTieTest_Matcher_Strings_OneLine_LeftToRight_int_oneLine)
	{
		BLOG_TRACE_METHOD(basar::test::unit::guitie::LoggerPool::logPerf, m_details.testName);

		try
		{
			CHECK(init());

			GuiPropertyTable_YIterator guiiter =  getMatcher().getLeft().begin();
			AccessorPropertyTable_YIterator acciter = getMatcher().getRight().begin();

			std::vector<BaseMatcherHolderTestSzenario> tests = BaseMatcherHolderTestSzenarioList::getPolicyTestSzenariosOneRowTest(NORM);
			std::vector<BaseMatcherHolderTestSzenario>::iterator testiter;
			for (testiter = tests.begin(); testiter != tests.end(); ++testiter)
			{
				try
				{
					CHECK(testiter->start("GuiTieTest_Matcher_Strings_OneLine_LeftToRight_int_oneLine", *this));

					CHECK(select());

					guiiter =  getMatcher().getLeft().begin();
					acciter = getMatcher().getRight().begin();

					CHECK_EQUAL(m_text.at(0),guiiter.getString(STRING1));
					CHECK_EQUAL(m_text.at(1),guiiter.getString(STRING2));


					if (testiter->throwsToManyEntriesException())
					{
						CHECK_THROW(getMatcher().LeftToRight(0, 1), basar::Exception);
					}
					else
					{
						std::vector<int> countvec;
						countvec.push_back(1);    // one line 
						countvec.push_back(0);    // all remaining lines 
						std::vector<int>::const_iterator count;
						for (count = countvec.begin(); count != countvec.end(); ++count)
						{
							getMatcher().LeftToRight(0, *count);

							guiiter = getMatcher().getLeft().begin();
							acciter = getMatcher().getRight().begin();

							CHECK_EQUAL(acciter.getString(STRING1),guiiter.getString(STRING1));
							CHECK_EQUAL(acciter.getString(STRING2),guiiter.getString(STRING2));

							CHECK_EQUAL(m_text.at(0),acciter.getString(STRING1));
							CHECK_EQUAL(m_text.at(1),acciter.getString(STRING2));
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
	// ----------------------------------------------------------------------------------------------------------
	TEST_FIXTURE (StringMatcherHolderOneLine, GuiTieTest_Matcher_Strings_OneLine_RightToLeft_Iter_Policy_internal)
	{
		if (g_RUNSHORT)
			return;

		BLOG_TRACE_METHOD(basar::test::unit::guitie::LoggerPool::logPerf, m_details.testName);

		try
		{
			CHECK(init());
			GuiPropertyTable_YIterator guiiter =  getMatcher().getLeft().begin();
			AccessorPropertyTable_YIterator acciter = getMatcher().getRight().begin();

			std::vector<BaseMatcherHolderTestSzenario> tests = BaseMatcherHolderTestSzenarioList::getPolicyTestSzenariosOneRowTest(NORM);
			std::vector<BaseMatcherHolderTestSzenario>::iterator testiter;
			for (testiter = tests.begin(); testiter != tests.end(); ++testiter)
			{
				try
				{  
					std::vector<MatcherPolicyType> policyvec = BaseMatcherHolderTestSzenarioList::getTestMatcherPolicyTypeVector();
					std::vector<MatcherPolicyType>::const_iterator policyiter;
					for (policyiter = policyvec.begin(); policyiter != policyvec.end(); ++policyiter)
					{
						try
						{   
							CHECK(testiter->start("GuiTieTest_Matcher_Strings_OneLine_RightToLeft_Iter_Policy_internal", *this));
							TEXTOUTPUT("started:       " << testiter->toText() << " with LR/RL-policy "<< *policyiter );
							CHECK(select());

							guiiter =  getMatcher().getLeft().begin();
							acciter =  getMatcher().getRight().begin();

							CHECK_EQUAL(m_text.at(0),guiiter.getString(STRING1));
							CHECK_EQUAL(m_text.at(1),guiiter.getString(STRING2));

							if (testiter->throwsToManyEntriesException(*policyiter))
							{
								CHECK_THROW(getMatcher().RightToLeft(guiiter, acciter, *policyiter), basar::Exception);
								CHECK_THROW(MatcherRef::RightToLeft(guiiter, acciter, *policyiter), basar::Exception);
							}
							else
							{
								MatcherRef::RightToLeft(guiiter, acciter, *policyiter);

								guiiter = getMatcher().getLeft().begin();
								acciter = getMatcher().getRight().begin();

								CHECK_EQUAL(acciter.getString(STRING1),guiiter.getString(STRING1));
								CHECK_EQUAL(acciter.getString(STRING2),guiiter.getString(STRING2));

								CHECK(!(m_text.at(0)== guiiter.getString(STRING1)));
								CHECK(!(m_text.at(1) == guiiter.getString(STRING2)));

								resetGuiValues();

								CHECK_EQUAL(m_text.at(0),guiiter.getString(STRING1));
								CHECK_EQUAL(m_text.at(1),guiiter.getString(STRING2));

								MatcherRef::LeftToRight(guiiter, acciter, *policyiter);

								CHECK_EQUAL(acciter.getString(STRING1),guiiter.getString(STRING1));
								CHECK_EQUAL(acciter.getString(STRING2),guiiter.getString(STRING2));

								CHECK_EQUAL(m_text.at(0),acciter.getString(STRING1));
								CHECK_EQUAL(m_text.at(1),acciter.getString(STRING2));
							}
						}
						catch (basar::Exception & ex)
						{
							std::cout << "Exception in" << testiter->toText() << " with LR/RL-policy "<< *policyiter << std::endl
								<< ex.what() << std::endl;
							const bool ExceptionOccured = false;
							CHECK(ExceptionOccured);
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
			const bool ExceptionOccured = false;
			CHECK(ExceptionOccured);
		}
	}


	// ----------------------------------------------------------------------------------------------------------
	TEST_FIXTURE(StringMatcherHolderOneLine, GuiTieTest_Matcher_Strings_OneLine_LeftToRight_Iter_Policy_internal)
	{
		if (g_RUNSHORT)
			return;

		BLOG_TRACE_METHOD(basar::test::unit::guitie::LoggerPool::logPerf, m_details.testName);

		try
		{
			CHECK(init());

			GuiPropertyTable_YIterator guiiter =  getMatcher().getLeft().begin();
			AccessorPropertyTable_YIterator acciter = getMatcher().getRight().begin();

			std::vector<BaseMatcherHolderTestSzenario> tests = BaseMatcherHolderTestSzenarioList::getPolicyTestSzenariosOneRowTest(NORM);
			std::vector<BaseMatcherHolderTestSzenario>::iterator testiter;
			for (testiter = tests.begin(); testiter != tests.end(); ++testiter)
			{
				try
				{
					std::vector<MatcherPolicyType> policyvec = BaseMatcherHolderTestSzenarioList::getTestMatcherPolicyTypeVector();
					std::vector<MatcherPolicyType>::const_iterator policyiter;
					for (policyiter = policyvec.begin(); policyiter != policyvec.end(); ++policyiter)
					{
						try
						{   
							CHECK(testiter->start("GuiTieTest_Matcher_Strings_OneLine_LeftToRight_Iter_Policy_internal", *this));
							TEXTOUTPUT("started:       " << testiter->toText() << " with LR/RL-policy "<< *policyiter );

							CHECK(select());

							guiiter =  getMatcher().getLeft().begin();
							acciter = getMatcher().getRight().begin();

							CHECK_EQUAL(m_text.at(0),guiiter.getString(STRING1));
							CHECK_EQUAL(m_text.at(1),guiiter.getString(STRING2));


							if (testiter->throwsToManyEntriesException(*policyiter))
							{
								CHECK_THROW(getMatcher().LeftToRight(guiiter, acciter,*policyiter), basar::Exception);
								CHECK_THROW(MatcherRef::LeftToRight(guiiter, acciter,*policyiter), basar::Exception);  
							}
							else
							{
								MatcherRef::LeftToRight(guiiter, acciter, *policyiter);

								guiiter = getMatcher().getLeft().begin();
								acciter = getMatcher().getRight().begin();

								CHECK_EQUAL(acciter.getString(STRING1),guiiter.getString(STRING1));
								CHECK_EQUAL(acciter.getString(STRING2),guiiter.getString(STRING2));

								CHECK_EQUAL(m_text.at(0),acciter.getString(STRING1));
								CHECK_EQUAL(m_text.at(1),acciter.getString(STRING2));
							}
						}
						catch (basar::Exception & ex)
						{
							std::cout << "Exception in" << testiter->toText() << " with LR/RL-policy "<< *policyiter << std::endl
								<< ex.what() << std::endl;
							const bool ExceptionOccured = false;
							CHECK(ExceptionOccured);
						}
					}
				}
				catch (basar::Exception & ex)
				{
					std::cout << "Exception in" << testiter->toText() << std::endl
						<< ex.what() << std::endl;
					const bool ExceptionOccured = false;
					CHECK(ExceptionOccured);
					//  
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

	// ----------------------------------------------------------------------------------------------------------

    TEST( DisregardDeleted )
    {
        QWidget* w = new QWidget();
        QLineEdit* e = new QLineEdit(w);

        QString expected("A");

        e->setText(expected);

        e->setObjectName( STRING1.c_str() );

        basar::db::aspect::ConnectionRef conn = TestConnection::createDefaultConnectionRef();

        basar::db::aspect::AccessorInstanceRef acc = basar::db::aspect::Manager::getInstance().createAccessorInstance("test", 
																						ACCESSORNAME_SELECT_STRINGS, 
																						conn, 
																						basar::db::aspect::FULL_CACHING);
        basar::gui::tie::MatcherRef matcher  = basar::gui::tie::Manager::getInstance().createMatcher(w, acc.getPropertyTable());

        ExecuteResultInfo res = acc.execute(ACCESSORMETHODNAME_SELECT_ONELINE);

        basar::db::aspect::AccessorPropertyTable_YIterator yit = acc.getPropertyTable().begin();

        acc.getPropertyTable().markForDelete( yit );

        matcher.setMatchingPolicy( basar::gui::tie::CONVERTTYPE | basar::gui::tie::INSERT | basar::gui::tie::SKIP | basar::gui::tie::DISREGARD_DELETED );

        matcher.RightToLeft();

        CHECK_EQUAL(expected.toStdString(), e->text().toStdString() );

        delete w;
    }


	// ----------------------------------------------------------------------------------------------------------

    TEST( MatchEmptyStringDefaultPolicy )
    {
        QWidget* w = new QWidget();
        QLineEdit* e = new QLineEdit(w);

        QString expected("");

        e->setText(expected);

        e->setObjectName( STRING1.c_str() );

        basar::db::aspect::ConnectionRef conn = TestConnection::createDefaultConnectionRef();

        basar::db::aspect::AccessorInstanceRef acc = basar::db::aspect::Manager::getInstance().createAccessorInstance("test", 
																						ACCESSORNAME_SELECT_STRINGS, 
																						conn, 
																						basar::db::aspect::FULL_CACHING);
        basar::gui::tie::MatcherRef matcher  = basar::gui::tie::Manager::getInstance().createMatcher(w, acc.getPropertyTable());

        ExecuteResultInfo res = acc.execute(ACCESSORMETHODNAME_SELECT_ONELINE);

        basar::db::aspect::AccessorPropertyTable_YIterator yit = acc.getPropertyTable().begin();

        matcher.LeftToRight();

        CHECK_EQUAL(expected.toStdString(), yit.getString(STRING1) );

        CHECK_EQUAL(basar::SS_UPDATE, yit.begin().getState().getState() );

        delete w;
    }
}
// ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~  ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~
