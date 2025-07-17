
#include <UnitTest++/UnitTest++.h>
#include "libbasarguitie.h"

#include "libbasarproperty_propertydescription.h"
#include "libbasardbaspect_connectionref.h"
#include "libbasardbaspect_accessorpropertytable_yiterator.h"

#include "testconnection.h"

#include <Qt/QLineEdit.h>



#include "buttablepropertydescriptions.h"
#include "buttablepropertydescriptionvectors.h"
#include "accessornames.h"
#include "basematcherholder.h"
#include "basematcherholdertestszenario.h"
#include "basematcherholdertestszenariolist.h"
#include "matcherholderlist.h"
#include "macros.h"
#include "loggerpool.h"

using basar::ConstString;
using basar::VarString;
using basar::TimeSpan;

using basar::gui::tie::MatcherRef;
using basar::gui::tie::GuiPropertyTable_YIterator;
using basar::gui::tie::GuiPropertyTableRef;

using basar::db::aspect::AccessorInstanceRef;
using basar::db::aspect::ConnectionRef;
using basar::db::aspect::AccessorPropertyTable_YIterator;
using basar::db::aspect::AccessorPropertyTableRef;
using basar::db::aspect::ExecuteResultInfo;

using basar::property::PropertyDescription;

using basar::test::unit::guitie::TestConnection;
using basar::test::unit::guitie::BaseMatcherHolder;
using basar::test::unit::guitie::BaseMatcherHolderTestSzenario;
using basar::test::unit::guitie::BaseMatcherHolderTestSzenarioList;
using basar::test::unit::guitie::MatcherHolderList;
using basar::test::unit::guitie::Direction;
using basar::test::unit::guitie::IndexCountHolder;

 
//------------------------------------------------------------------------------
SUITE(Matcher_TimeSpan)
{
	// -----------------------------------------------------------------
	typedef basar::gui::tie::MatcherPolicyType MatcherPolicyType;
 
	//#define TEXTOUTPUT(XXX) std::cout << "Test: " << XXX << std::endl;
	//#define TEXTOUTPUT2(XXX) std::cout << "Test2: " << XXX << std::endl;;

	static ConstString colSpan1          = PROPDEF_COLSPAN1  .getName().c_str(); 

	// --------------------------------------------------------------------------
	class TimeSpanMatcherHolder : public BaseMatcherHolder
	{
		bool m_init;

	protected:
		QLineEdit		* m_lineedit;

		TimeSpan		  m_defaultTimeSpan;

		int m_guiColNumber;
		int m_accColNumber;

	public:

		// -----------------------------------------------------------------
		TimeSpanMatcherHolder() : m_accColNumber(static_cast<int>(propDescVecButTableDescTimeSpan.m_vector.size()))
		{ 
			m_guiColNumber = m_accColNumber;
			m_defaultTimeSpan = TimeSpan("32 16:23:12.321");
			m_init         = false;
		}

		// -----------------------------------------------------------------
		~TimeSpanMatcherHolder()
		{
		}

		// -----------------------------------------------------------------
		bool initGui()
		{
			delete m_widget;
			m_widget		= new QWidget();

			m_lineedit		= new QLineEdit(m_defaultTimeSpan.toNormalizedString().c_str(), m_widget);
			m_lineedit		->setObjectName(colSpan1);

			resetGuiValues();

			return true;
		}

		// -----------------------------------------------------------------
		bool resetGuiValues() 
		{
			m_lineedit->setText(m_defaultTimeSpan.toNormalizedString().c_str());
			return true;
		}

		// -----------------------------------------------------------------
		bool init() 
		{
			bool retval  = true;

			if (!m_init )
			{
				initGui();
				retval = setAccessorInfo (ACCESSORNAME_ButMasterTableReader_TimeSpan,
										  ACCESSORNAME_ButMasterTableReader_TimeSpan,
										  ACCESSORMETHODNAME_SELECT_ONELINE,
										  ACCESSORMETHODNAME_SELECT_ONELINE,
										  false);
				if (retval)
				{
					retval = resetAccessorAndMatcher();
					m_init = retval; 
				}
			}
			return retval;
		}

		// -----------------------------------------------------------------
		bool start()
		{
			bool retval = true;

			if (!m_init)
				retval = init();

			if (retval)
			{
				if (m_needsReset)
				{
					retval = initGui(); 
					m_needsReset = true;
				}

				resetGuiValues();
				retval = BaseMatcherHolder::start();
			}

			return retval;
		}

		// -----------------------------------------------------------------
		void checkEqual(Direction::DirEnum                dir, 
						GuiPropertyTable_YIterator      & guiiter , 
						AccessorPropertyTable_YIterator & acciter , 
						BaseMatcherHolderTestSzenario   & /*test*/)
		{
			CHECK_EQUAL_DIRECTION(dir, acciter.getTimeSpan    (colSpan1)     .toNormalizedString(), guiiter.getString    (colSpan1));
		}

		// -----------------------------------------------------------------
		void checkNotEqual (Direction::DirEnum             /* dir */, 
							GuiPropertyTable_YIterator      & guiiter , 
							AccessorPropertyTable_YIterator & acciter , 
							BaseMatcherHolderTestSzenario   & /*test*/)
		{
			CHECK( acciter.getTimeSpan    (colSpan1)     .toNormalizedString() != guiiter.getString    (colSpan1));
		}

		// -----------------------------------------------------------------
		void checkEqualToText(Direction::DirEnum              dir, 
							  GuiPropertyTable_YIterator    & guiiter ,  
							  BaseMatcherHolderTestSzenario & /*test*/)
		{
			CHECK_EQUAL_DIRECTION(dir, m_defaultTimeSpan.toNormalizedString(), guiiter.getString    (colSpan1));
		}
	};

	// --------------------------------------------------------------------------
	TEST_FIXTURE(TimeSpanMatcherHolder, GuiTieTest_Matcher_TimeSpan_Start)
	{
		try
		{
			CHECK(init());

			GuiPropertyTableRef             guiprop = getMatcher().getLeft();
			AccessorPropertyTableRef        accprop = getMatcher().getRight();
			GuiPropertyTable_YIterator      guiiter = guiprop.begin();
			AccessorPropertyTable_YIterator acciter = accprop.begin();

			CHECK_EQUAL(0, static_cast<int>(accprop.size()));
			CHECK_EQUAL(1, static_cast<int>(guiprop.size())); 

			CHECK(!guiiter.isEnd());
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

			CHECK_EQUAL(1, static_cast<int>(accprop.size()));
			CHECK_EQUAL(1, static_cast<int>(guiprop.size()));
			CHECK(!guiiter.isEnd());
			CHECK(!acciter.isEnd());
			CHECK(acciter.isBegin());
			CHECK(guiiter.isBegin());
			CHECK(!acciter.isNull());
			CHECK(!guiiter.isNull());

			resetGuiValues();

			guiprop = getMatcher().getLeft();
			accprop = getMatcher().getRight();
			guiiter = guiprop.begin();
			acciter = accprop.begin();

			CHECK_EQUAL(1, static_cast<int>(accprop.size()));
			CHECK_EQUAL(1, static_cast<int>(guiprop.size()));
			CHECK_EQUAL(m_guiColNumber, static_cast<int>(guiiter.getNumberOfColumns()));
			CHECK_EQUAL(m_accColNumber, static_cast<int>(acciter.getNumberOfColumns()));

			CHECK_EQUAL(0,guiprop.getAbsoluteIndex());

			CHECK(!guiiter.isEnd());
			CHECK(!acciter.isEnd());
			CHECK(acciter.isBegin());
			CHECK(guiiter.isBegin());
			CHECK(!acciter.isNull());
			CHECK(!guiiter.isNull());

			{
				guiprop.clear();

				CHECK(!guiiter.isEnd  ());
				CHECK( guiiter.isBegin());
				CHECK(!guiiter.isNull ());

				CHECK_EQUAL("" , guiiter.getString    (colSpan1)     .c_str());

				CHECK_EQUAL("" , m_lineedit     ->text().toAscii().constData());
			}
		}
		catch (basar::Exception & ex)
		{
			std::cout << ex.what() << std::endl;
			const bool ExceptionOccured = false;
			CHECK(ExceptionOccured);
		}
	}

	// --------------------------------------------------------------------------
	TEST_FIXTURE(TimeSpanMatcherHolder, GuiTieTest_Matcher_TimeSpan_RightToLeft_NoVar_oneLine)
	{
		try
		{
			CHECK(init());

			GuiPropertyTableRef             guiprop = getMatcher().getLeft();
			AccessorPropertyTableRef        accprop = getMatcher().getRight();
			GuiPropertyTable_YIterator      guiiter = guiprop.begin();
			AccessorPropertyTable_YIterator acciter = accprop.begin();

			std::vector<BaseMatcherHolderTestSzenario> tests = 
				BaseMatcherHolderTestSzenarioList::getPolicyTestSzenarios(MORE_GUI, NORM);
			std::vector<BaseMatcherHolderTestSzenario>::iterator testiter;

			for (testiter = tests.begin(); testiter != tests.end(); ++testiter)
			{
				try
				{
					CHECK(testiter->start("GuiTieTest_Matcher_TimeSpan_RightToLeft_NoVar_oneLine", *this));
					TEXTOUTPUT(testiter->toText());
		
					CHECK(select());

					guiprop = getMatcher().getLeft();
					accprop = getMatcher().getRight();

					CHECK_EQUAL(1, static_cast<int>(accprop.size()));
					CHECK_EQUAL(1, static_cast<int>(guiprop.size()));

					guiiter = guiprop.begin();
					checkEqualToText(Direction::RL, guiiter , *testiter);


					CHECK_EQUAL(0,guiprop.getAbsoluteIndex());

					if ( testiter->throwsConvertException() )
					{
						CHECK_THROW( getMatcher().RightToLeft(), basar::Exception);
					}
					else if ( !testiter->checkPolicy(basar::gui::tie::CONVERTTYPE) )
					{
						//this if filters all policies containing basar::gui::tie::SKIP 
						//because TimeSpan as no other gui representation than string
						//and match looks for a timespan guitype resulting in not matching
						//TimeSpan to gui qlineedit
						std::cout << testiter->toText() << " - skipped test" << std::endl;
					}
					else
					{          
						CHECK_EQUAL(1,getMatcher().RightToLeft());

						guiprop = getMatcher().getLeft();
						accprop = getMatcher().getRight();
						CHECK_EQUAL(1, static_cast<int>(accprop.size()));
						CHECK_EQUAL(1, static_cast<int>(guiprop.size()));
						CHECK_EQUAL(0,guiprop.getAbsoluteIndex());

						guiiter = guiprop.begin();
						acciter = accprop.begin();

						checkEqual(Direction::RL, guiiter , acciter, *testiter);

						CHECK_EQUAL("23 12:32:16.123"		 , m_lineedit     ->text().toAscii().constData());


						guiiter++;
						CHECK(guiiter.isEnd());
						acciter++;
						CHECK(acciter.isEnd());

						// todo test unset
						// todo test greater than unset.
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

	// --------------------------------------------------------------------------
	TEST_FIXTURE(TimeSpanMatcherHolder, GuiTieTest_Matcher_TimeSpan_LeftToRight_NoVar_oneLine)
	{
		try
		{
			CHECK(init());

			GuiPropertyTableRef             guiprop = getMatcher().getLeft();
			AccessorPropertyTableRef        accprop = getMatcher().getRight();
			GuiPropertyTable_YIterator      guiiter = guiprop.begin();
			AccessorPropertyTable_YIterator acciter = accprop.begin();

			std::vector<BaseMatcherHolderTestSzenario> tests =
				BaseMatcherHolderTestSzenarioList::getPolicyTestSzenarios(NORM, NORM);
			std::vector<BaseMatcherHolderTestSzenario>::iterator testiter;
			for (testiter = tests.begin(); testiter != tests.end(); ++testiter)
			{
				try
				{					
					CHECK(testiter->start("GuiTieTest_Matcher_TimeSpan_LeftToRight_NoVar_oneLine", *this));

					CHECK(select());

					accprop = getMatcher().getRight();
					guiprop = getMatcher().getLeft();
					CHECK_EQUAL(1, static_cast<int>(accprop.size()));
					CHECK_EQUAL(1, static_cast<int>(guiprop.size()));
					CHECK_EQUAL(0,guiprop.getAbsoluteIndex());


					if ( testiter->throwsConvertException() )
					{
						CHECK_THROW( getMatcher().RightToLeft(), basar::Exception);
					}
					else if ( !testiter->checkPolicy(basar::gui::tie::CONVERTTYPE) )
					{
						//this if filters all policies containing basar::gui::tie::SKIP 
						//because TimeSpan as no other gui representation than string
						//and match looks for a timespan guitype resulting in not matching
						//TimeSpan to gui qlineedit
						std::cout << testiter->toText() << " - skipped test" << std::endl;
					}
					else
					{  

						CHECK_EQUAL(1,getMatcher().LeftToRight());

						accprop = getMatcher().getRight();
						guiprop = getMatcher().getLeft();

						CHECK_EQUAL(1, static_cast<int>(guiprop.size()));
						CHECK_EQUAL(1, static_cast<int>(accprop.size()));

						guiiter = guiprop.begin();
						acciter = accprop.begin();

						checkEqual(Direction::LR, guiiter , acciter, *testiter);

						CHECK_EQUAL("32 16:23:12.321"		 , m_lineedit     ->text().toAscii().constData());

						guiiter++;
						CHECK(guiiter.isEnd());
						acciter++;
						CHECK(acciter.isEnd());
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
	// --------------------------------------------------------------------------
	TEST_FIXTURE(TimeSpanMatcherHolder, GuiTieTest_Matcher_TimeSpan_RightToLeft_iter_oneLine)
	{
		try
		{
			CHECK(init());

			GuiPropertyTableRef             guiprop = getMatcher().getLeft();
			AccessorPropertyTableRef        accprop = getMatcher().getRight();
			GuiPropertyTable_YIterator      guiiter = guiprop.begin();
			AccessorPropertyTable_YIterator acciter = accprop.begin();

			std::vector<BaseMatcherHolderTestSzenario> tests = 
				BaseMatcherHolderTestSzenarioList::getPolicyTestSzenarios(NORM, NORM);
			std::vector<BaseMatcherHolderTestSzenario>::iterator testiter;
			for (testiter = tests.begin(); testiter != tests.end(); ++testiter)
			{
				try
				{
					CHECK(testiter->start("GuiTieTest_Matcher_TimeSpan_RightToLeft_iter_oneLine", *this));
					TEXTOUTPUT(testiter->toText());
					CHECK(select());

					guiprop = getMatcher().getLeft();
					accprop = getMatcher().getRight();

					CHECK_EQUAL(1, static_cast<int>(accprop.size()));
					CHECK_EQUAL(1, static_cast<int>(guiprop.size()));
					CHECK_EQUAL(0,guiprop.getAbsoluteIndex());

					guiiter = guiprop.begin();
					acciter = accprop.begin();

					checkEqualToText(Direction::RL, guiiter , *testiter);

					if ( testiter->throwsConvertException() )
					{
						CHECK_THROW( getMatcher().RightToLeft(guiiter, acciter), basar::Exception);
					}
					else if ( !testiter->checkPolicy(basar::gui::tie::CONVERTTYPE) )
					{
						//this if filters all policies containing basar::gui::tie::SKIP 
						//because TimeSpan as no other gui representation than string
						//and match looks for a timespan guitype resulting in not matching
						//TimeSpan to gui qlineedit
						std::cout << testiter->toText() << " - skipped test" << std::endl;
					}
					else
					{          
						getMatcher().RightToLeft(guiiter, acciter);
						checkEqual(Direction::RL, guiiter , acciter, *testiter);

						CHECK_EQUAL("23 12:32:16.123"		 , m_lineedit     ->text().toAscii().constData());
						// todo test unset
						// todo test greater than unset.
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
	// --------------------------------------------------------------------------
	TEST_FIXTURE(TimeSpanMatcherHolder, GuiTieTest_Matcher_TimeSpan_LeftToRight_iter_oneLine)
	{
		try
		{
			CHECK(init());

			GuiPropertyTableRef guiprop = getMatcher().getLeft();
			AccessorPropertyTableRef accprop = getMatcher().getRight();
			GuiPropertyTable_YIterator guiiter = guiprop.begin();
			AccessorPropertyTable_YIterator acciter = accprop.begin();

			std::vector<BaseMatcherHolderTestSzenario> tests =
				BaseMatcherHolderTestSzenarioList::getPolicyTestSzenarios(NORM, NORM);
			std::vector<BaseMatcherHolderTestSzenario>::iterator testiter;
			for (testiter = tests.begin(); testiter != tests.end(); ++testiter)
			{
				try
				{
					CHECK(testiter->start("GuiTieTest_Matcher_TimeSpan_LeftToRight_iter_oneLine", *this));

					CHECK(select());

					accprop = getMatcher().getRight();
					guiprop = getMatcher().getLeft();
					CHECK_EQUAL(1, static_cast<int>(accprop.size()));
					CHECK_EQUAL(1, static_cast<int>(guiprop.size()));

					guiiter = guiprop.begin();
					acciter = accprop.begin();

					CHECK_EQUAL(0,guiprop.getAbsoluteIndex());

					if ( testiter->throwsConvertException() )
					{
						CHECK_THROW( getMatcher().LeftToRight(guiiter, acciter), basar::Exception);
					}
					else if ( !testiter->checkPolicy(basar::gui::tie::CONVERTTYPE) )
					{
						//this if filters all policies containing basar::gui::tie::SKIP 
						//because TimeSpan as no other gui representation than string
						//and match looks for a timespan guitype resulting in not matching
						//TimeSpan to gui qlineedit
						std::cout << testiter->toText() << " - skipped test" << std::endl;
					}
					else
					{  
						getMatcher().LeftToRight(guiiter, acciter);
						checkEqual(Direction::LR, guiiter , acciter, *testiter);

						CHECK_EQUAL("32 16:23:12.321"		 , m_lineedit     ->text().toAscii().constData());

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
	// --------------------------------------------------------------------------
	TEST_FIXTURE(TimeSpanMatcherHolder, GuiTieTest_Matcher_TimeSpan_RightToLeft_int_oneLine)
	{
		try
		{
			CHECK(init());

			GuiPropertyTableRef guiprop = getMatcher().getLeft();
			AccessorPropertyTableRef accprop = getMatcher().getRight();
			GuiPropertyTable_YIterator guiiter = guiprop.begin();
			AccessorPropertyTable_YIterator acciter = accprop.begin();

			std::vector<BaseMatcherHolderTestSzenario> tests = 
				BaseMatcherHolderTestSzenarioList::getPolicyTestSzenarios(NORM, NORM);
			std::vector<BaseMatcherHolderTestSzenario>::iterator testiter;
			for (testiter = tests.begin(); testiter != tests.end(); ++testiter)
			{
				try
				{
					CHECK(testiter->start("GuiTieTest_Matcher_TimeSpan_RightToLeft_int_oneLine", *this));
					TEXTOUTPUT(testiter->toText());
					CHECK(select());

					guiprop = getMatcher().getLeft();
					accprop = getMatcher().getRight();

					CHECK_EQUAL(1, static_cast<int>(accprop.size()));
					CHECK_EQUAL(1, static_cast<int>(guiprop.size()));
					CHECK_EQUAL(0,guiprop.getAbsoluteIndex());

					guiiter = guiprop.begin();
					acciter = accprop.begin();

					checkEqualToText(Direction::RL, guiiter , *testiter);

					if ( testiter->throwsConvertException() )
					{
						CHECK_THROW( getMatcher().RightToLeft(guiiter, acciter), basar::Exception);
					}
					else if ( !testiter->checkPolicy(basar::gui::tie::CONVERTTYPE) )
					{
						//this if filters all policies containing basar::gui::tie::SKIP 
						//because TimeSpan as no other gui representation than string
						//and match looks for a timespan guitype resulting in not matching
						//TimeSpan to gui qlineedit
						std::cout << testiter->toText() << " - skipped test" << std::endl;
					}
					else
					{          
						CHECK_EQUAL(1,getMatcher().RightToLeft(0, 1));
						checkEqual(Direction::RL, guiiter , acciter, *testiter);

						resetGuiValues();
						checkEqualToText(Direction::RL, guiiter , *testiter);

						CHECK_EQUAL(1,getMatcher().RightToLeft(0, 5));
						checkEqual(Direction::RL, guiiter , acciter, *testiter);

						CHECK_EQUAL("23 12:32:16.123"		 , m_lineedit     ->text().toAscii().constData());

						resetGuiValues();

						CHECK_EQUAL(0,getMatcher().RightToLeft(1, 5));

						CHECK_THROW( getMatcher().RightToLeft(24,1), basar::Exception);
						CHECK_THROW( getMatcher().RightToLeft(-10,-10), basar::Exception);

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
	// --------------------------------------------------------------------------
	TEST_FIXTURE(TimeSpanMatcherHolder, GuiTieTest_Matcher_TimeSpan_LeftToRight_int_oneLine)
	{
		try
		{
			CHECK(init());

			GuiPropertyTableRef guiprop = getMatcher().getLeft();
			AccessorPropertyTableRef accprop = getMatcher().getRight();
			GuiPropertyTable_YIterator guiiter = guiprop.begin();
			AccessorPropertyTable_YIterator acciter = accprop.begin();

			std::vector<BaseMatcherHolderTestSzenario> tests =
				BaseMatcherHolderTestSzenarioList::getPolicyTestSzenarios(MORE_GUI, NORM);
			std::vector<BaseMatcherHolderTestSzenario>::iterator testiter;
			for (testiter = tests.begin(); testiter != tests.end(); ++testiter)
			{
				try
				{
					CHECK(testiter->start("GuiTieTest_Matcher_TimeSpan_LeftToRight_iter_oneLine", *this));

					CHECK(select());

					accprop = getMatcher().getRight();
					guiprop = getMatcher().getLeft();
					CHECK_EQUAL(1, static_cast<int>(accprop.size()));
					CHECK_EQUAL(1, static_cast<int>(guiprop.size()));

					guiiter = guiprop.begin();
					acciter = accprop.begin();

					CHECK_EQUAL(0,guiprop.getAbsoluteIndex());

					if ( testiter->throwsConvertException() )
					{
						CHECK_THROW( getMatcher().LeftToRight(0, 1), basar::Exception);
					}
					else if ( !testiter->checkPolicy(basar::gui::tie::CONVERTTYPE) )
					{
						//this if filters all policies containing basar::gui::tie::SKIP 
						//because TimeSpan as no other gui representation than string
						//and match looks for a timespan guitype resulting in not matching
						//TimeSpan to gui qlineedit
						std::cout << testiter->toText() << " - skipped test" << std::endl;
					}
					else
					{  
						CHECK_EQUAL(1, getMatcher().LeftToRight(0, 1));
						guiiter = guiprop.begin();
						acciter = accprop.begin();

						checkEqual(Direction::LR, guiiter , acciter, *testiter);

						CHECK(select());
						guiiter = guiprop.begin();
						acciter = accprop.begin();
						checkNotEqual(Direction::LR, guiiter , acciter, *testiter);

						if (testiter->checkPolicy(basar::gui::tie::INSERT))
						{
							CHECK_THROW( getMatcher().LeftToRight(0, 5), basar::Exception);
						}
						else
						{
							CHECK_EQUAL(1, getMatcher().LeftToRight(0, 5));
						}
						guiiter = guiprop.begin();
						acciter = accprop.begin();
						checkEqual(Direction::LR, guiiter , acciter, *testiter);

						CHECK_EQUAL("32 16:23:12.321"		 , m_lineedit     ->text().toAscii().constData());

						CHECK_THROW( getMatcher().LeftToRight(24,1), basar::Exception);
						CHECK_THROW( getMatcher().LeftToRight(-10,-10), basar::Exception);

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
	// ----------------------------------------------------------
	TEST_FIXTURE(TimeSpanMatcherHolder, GuiTieTest_Matcher_TimeSpan_RightToLeft_iter_policy_oneLine)
	{
		try
		{
			CHECK(init());

			GuiPropertyTableRef guiprop = getMatcher().getLeft();
			AccessorPropertyTableRef accprop = getMatcher().getRight();
			GuiPropertyTable_YIterator guiiter = guiprop.begin();
			AccessorPropertyTable_YIterator acciter = accprop.begin();

			std::vector<BaseMatcherHolderTestSzenario> tests = 
				BaseMatcherHolderTestSzenarioList::getPolicyTestSzenarios(NORM, NORM);
			std::vector<BaseMatcherHolderTestSzenario>::iterator testiter;
			for (testiter = tests.begin(); testiter != tests.end(); ++testiter)
			{
				try
				{
					CHECK(testiter->start("GuiTieTest_Matcher_TimeSpan_RightToLeft_iter_policy_oneLine", *this));
					TEXTOUTPUT(testiter->toText());
					CHECK(select());

					guiprop = getMatcher().getLeft();
					accprop = getMatcher().getRight();

					CHECK_EQUAL(1, static_cast<int>(accprop.size()));
					CHECK_EQUAL(1, static_cast<int>(guiprop.size()));
					CHECK_EQUAL(0,guiprop.getAbsoluteIndex());

					guiiter = guiprop.begin();
					acciter = accprop.begin();

					checkEqualToText(Direction::RL, guiiter , *testiter);

					std::vector<MatcherPolicyType> policyvec = BaseMatcherHolderTestSzenarioList::getTestMatcherPolicyTypeVector();
					std::vector<MatcherPolicyType>::const_iterator policyiter;
					for (policyiter = policyvec.begin(); policyiter != policyvec.end(); ++policyiter)
					{
						try
						{
							
							if ( testiter->throwsConvertException(*policyiter) )
							{
								CHECK_THROW( getMatcher().RightToLeft(guiiter, acciter, *policyiter), basar::Exception);
							}
							else if ( !testiter->checkPolicy(*policyiter, basar::gui::tie::CONVERTTYPE) )
							{
								//this if filters all policies containing basar::gui::tie::SKIP 
								//because TimeSpan as no other gui representation than string
								//and match looks for a timespan guitype resulting in not matching
								//TimeSpan to gui qlineedit
								std::cout << testiter->toText() <<  " overriding policy to: " << *policyiter << " - skipped test" << std::endl;
							}
							else			
							{         								
								getMatcher().RightToLeft(guiiter, acciter, *policyiter);
								checkEqual(Direction::RL, guiiter , acciter, *testiter);

								resetGuiValues();
								checkEqualToText(Direction::RL, guiiter , *testiter);
								checkNotEqual(Direction::RL, guiiter , acciter, *testiter);

								MatcherRef::RightToLeft(guiiter, acciter, *policyiter);
								checkEqual(Direction::RL, guiiter , acciter, *testiter);
							}
						}

						catch (basar::Exception & ex)
						{
							std::cout << " Exception in" << testiter->toText() << " with poliy:" << *policyiter << std::endl << ex.what() << std::endl;
							const bool InnerExceptionOccured = false;
							CHECK(InnerExceptionOccured);
						}
						catch (...)
						{
							std::cout << "Non Basar Exception in" << testiter->toText() << " with poliy:" << *policyiter << std::endl;
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
	// --------------------------------------------------------------------------
	TEST_FIXTURE(TimeSpanMatcherHolder, GuiTieTest_Matcher_TimeSpan_LeftToRight_iter_policy_oneLine)
	{
		try
		{
			CHECK(init());

			GuiPropertyTableRef guiprop = getMatcher().getLeft();
			AccessorPropertyTableRef accprop = getMatcher().getRight();
			GuiPropertyTable_YIterator guiiter = guiprop.begin();
			AccessorPropertyTable_YIterator acciter = accprop.begin();

			std::vector<BaseMatcherHolderTestSzenario> tests =
				BaseMatcherHolderTestSzenarioList::getPolicyTestSzenarios(NORM, NORM);
			std::vector<BaseMatcherHolderTestSzenario>::iterator testiter;
			for (testiter = tests.begin(); testiter != tests.end(); ++testiter)
			{
				try
				{
					CHECK(testiter->start("GuiTieTest_Matcher_TimeSpan_LeftToRight_iter_policy_oneLine", *this));

					CHECK(select());

					accprop = getMatcher().getRight();
					guiprop = getMatcher().getLeft();
					CHECK_EQUAL(1, static_cast<int>(accprop.size()));
					CHECK_EQUAL(1, static_cast<int>(guiprop.size()));

					guiiter = guiprop.begin();
					acciter = accprop.begin();

					CHECK_EQUAL(0,guiprop.getAbsoluteIndex());

					std::vector<MatcherPolicyType> policyvec = BaseMatcherHolderTestSzenarioList::getTestMatcherPolicyTypeVector();
					std::vector<MatcherPolicyType>::const_iterator policyiter;
					for (policyiter = policyvec.begin(); policyiter != policyvec.end(); ++policyiter)
					{
						try
						{

							if ( testiter->throwsConvertException(*policyiter) )
							{
								CHECK_THROW( getMatcher().LeftToRight(guiiter, acciter, *policyiter), basar::Exception);
							}
							else if ( !testiter->checkPolicy(*policyiter, basar::gui::tie::CONVERTTYPE) )
							{
								//this if filters all policies containing basar::gui::tie::SKIP 
								//because TimeSpan as no other gui representation than string
								//and match looks for a timespan guitype resulting in not matching
								//TimeSpan to gui qlineedit
								std::cout << testiter->toText() <<  " overriding policy to: " << *policyiter << " - skipped test" << std::endl;
							}
							else 
							{ 
								getMatcher().LeftToRight(guiiter, acciter, *policyiter) ;
								accprop = getMatcher().getRight();
								guiprop = getMatcher().getLeft();
								guiiter = guiprop.begin();
								acciter = accprop.begin();
								checkEqual(Direction::LR, guiiter , acciter, *testiter);

								resetGuiValues();
								CHECK(select());
								accprop = getMatcher().getRight();
								guiprop = getMatcher().getLeft();
								guiiter = guiprop.begin();
								acciter = accprop.begin();
								checkEqualToText(Direction::RL, guiiter , *testiter);
								checkNotEqual(Direction::RL, guiiter , acciter, *testiter);

								MatcherRef::RightToLeft(guiiter, acciter, *policyiter);
								checkEqual(Direction::RL, guiiter , acciter, *testiter);
							}
						}
						catch (basar::Exception & ex)
						{
							std::cout << " Exception in" << testiter->toText() << " with poliy:" << *policyiter << std::endl << ex.what() << std::endl;
							const bool InnerExceptionOccured = false;
							CHECK(InnerExceptionOccured);
						}
						catch (...)
						{
							std::cout << "Non Basar Exception in" << testiter->toText() << " with poliy:" << *policyiter << std::endl;
							const bool InnerNonBasarExceptionOccured = false;
							CHECK(InnerNonBasarExceptionOccured);
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
	// --------------------------------------------------------------------------
}

// ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~  ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~
