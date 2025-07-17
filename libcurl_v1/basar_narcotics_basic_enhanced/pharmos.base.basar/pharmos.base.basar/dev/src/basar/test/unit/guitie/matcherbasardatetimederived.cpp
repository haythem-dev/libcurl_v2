/*
* @file                                            
* @author Michael Eichenlaub
* @date 2009                            
*/ 

#include <UnitTest++/UnitTest++.h>
#include "libbasarguitie.h"

#include "libbasarproperty_propertydescription.h"
#include "libbasardbaspect_connectionref.h"
#include "libbasardbaspect_accessorpropertytable_yiterator.h"

#include "basematcherholder.h"
#include "basematcherholdertestszenario.h"
#include "testconnection.h"
#include "basematcherholdertestszenariolist.h"
#include "buttablepropertydescriptions.h"
#include "buttablepropertydescriptionvectors.h"
#include "accessornames.h"
#include "matcherholderlist.h"
#include "macros.h"
#include "loggerpool.h"

#include "basarderived.h"

#include "basardatetimemockcessor.h"

#include <boost/make_shared.hpp>

using basar::ConstString;
using basar::VarString;
using basar::Date;
using basar::DateTime;
using basar::Time;

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
SUITE(Matcher_BasarDateTime_Derived)
{
	// -----------------------------------------------------------------
	typedef basar::gui::tie::MatcherPolicyType MatcherPolicyType;
 
	//#define TEXTOUTPUT(XXX) std::cout << "Test: " << XXX << std::endl;
	//#define TEXTOUTPUT2(XXX) std::cout << "Test2: " << XXX << std::endl;;

	static const int month = 8;
	static const int day   = 25; 
	static const int year  = 2008;
	static const int hour  = 1;
	static const int min   = 1; 
	static const int sec   = 1;
	static const int msec  = 0;

	static ConstString colDate          = PROPDEF_COLDATE   .getName().c_str(); 
	static ConstString colTime          = PROPDEF_COLTIME1  .getName().c_str();
	static ConstString colDateTime1     = PROPDEF_COLDT1    .getName().c_str();
	static ConstString colDateTime2     = PROPDEF_COLDT2    .getName().c_str();
	static ConstString colDateTime_date = PROPDEF_COLDT_DATE.getName().c_str();
	static ConstString colDateTime_time = PROPDEF_COLDT_TIME.getName().c_str();


	// --------------------------------------------------------------------------
	class BasarDateTimeMatcherHolder : public BaseMatcherHolder
	{
		bool m_init;

	protected:
		BasarDateEditDerived     * m_date;
		BasarDateTimeEditDerived * m_datetime1;
		BasarDateTimeEditDerived * m_datetime2;
		BasarTimeEditDerived     * m_time;
		BasarDateTimeEditDerived * m_datetime_date;
		BasarDateTimeEditDerived * m_datetime_time;

		basar::Date     m_defaultDate;
		basar::Time     m_defaultTime;

		int m_guiColNumber;
		int m_accColNumber;

	public:

		// -----------------------------------------------------------------
		BasarDateTimeMatcherHolder() : m_accColNumber(static_cast<int>(propDescVecButTableDescAllDates.m_vector.size()))
		{ 
			m_guiColNumber = m_accColNumber + 1;
			m_init         = false;
			m_defaultDate  = basar::Date(year,month,day);
			m_defaultTime  = basar::Time(hour, min, sec, msec);

            setAccessorInfo(ACCESSORNAME_ButMasterTableReader_AllDates,
                ACCESSORNAME_ButMasterTableReader_AllDates,
                ACCESSORMETHODNAME_SELECT_ONELINE,
                ACCESSORMETHODNAME_SELECT_ONELINE,
                false);

            m_AccessorMap.insert(AccessorMap::value_type(ACCESSORNAME_ButMasterTableReader_AllDates, boost::make_shared<BasarDateTimeMockCessor>(ACCESSORNAME_ButMasterTableReader_AllDates) ));
		}

		// -----------------------------------------------------------------
		~BasarDateTimeMatcherHolder()
		{
		}

		// -----------------------------------------------------------------
		bool initGui()
		{
			QDate     defaultDate     = QDate    (year,month,day);
			QTime     defaultTime     = QTime    (hour, min, sec, msec);
			QDateTime defaultDateTime = QDateTime(defaultDate, defaultTime);

			delete m_widget;
			m_widget = new QWidget();

			m_date			= new BasarDateEditDerived    (defaultDate    , m_widget); 
			m_datetime1		= new BasarDateTimeEditDerived(defaultDateTime, m_widget); 
			m_datetime2		= new BasarDateTimeEditDerived(defaultDateTime, m_widget); 
			m_time			= new BasarTimeEditDerived    (defaultTime    , m_widget); 
			m_datetime_date = new BasarDateTimeEditDerived(defaultDateTime, m_widget); 
			m_datetime_time = new BasarDateTimeEditDerived(defaultDateTime, m_widget); 

			m_date		   ->setObjectName(colDate);
			m_datetime1	   ->setObjectName(colDateTime1);
			m_datetime2	   ->setObjectName(colDateTime2);
			m_time		   ->setObjectName(colTime);
			m_datetime_date->setObjectName(colDateTime_date);
			m_datetime_time->setObjectName(colDateTime_time);

			resetGuiValues();

			return true;
		}

		// -----------------------------------------------------------------
		bool resetGuiValues() 
		{
			QDate defaultDate = QDate(year, month, day);
			QTime defaultTime = QTime(hour, min, sec, msec);

			m_date		   ->setDate(defaultDate);
			m_datetime1	   ->setDate(defaultDate);
			m_datetime2	   ->setDate(defaultDate);
			m_datetime1	   ->setTime(defaultTime);
			m_datetime2	   ->setTime(defaultTime);
			m_time		   ->setTime(defaultTime);
			m_datetime_date->setDate(defaultDate);
			m_datetime_time->setDate(defaultDate);
			m_datetime_date->setTime(defaultTime);
			m_datetime_time->setTime(defaultTime);

			return true;
		}

		// -----------------------------------------------------------------
		bool init() 
		{
			bool retval  = true;

			if (!m_init )
			{
				initGui();

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
			CHECK_EQUAL_DIRECTION(dir, acciter.getDate    (colDate)     .toStrDate(), guiiter.getDate    (colDate)     .toStrDate());
			CHECK_EQUAL_DIRECTION(dir, acciter.getDateTime(colDateTime1).toStrDate(), guiiter.getDateTime(colDateTime1).toStrDate());
			CHECK_EQUAL_DIRECTION(dir, acciter.getDateTime(colDateTime2).toStrDate(), guiiter.getDateTime(colDateTime2).toStrDate());

			CHECK_EQUAL_DIRECTION(dir, acciter.getDateTime(colDateTime1).toStrTime(), guiiter.getDateTime(colDateTime1).toStrTime());
			CHECK_EQUAL_DIRECTION(dir, acciter.getDateTime(colDateTime2).toStrTime(), guiiter.getDateTime(colDateTime2).toStrTime());
			CHECK_EQUAL_DIRECTION(dir, acciter.getTime    (colTime)     .toStrTime(), guiiter.getTime    (colTime)     .toStrTime());

			CHECK_EQUAL_DIRECTION(dir, acciter.getDateTime(colDateTime_date).toStrDate(), guiiter.getDateTime(colDateTime_date).toStrDate());
			CHECK_EQUAL_DIRECTION(dir, acciter.getDateTime(colDateTime_time).toStrDate(), guiiter.getDateTime(colDateTime_time).toStrDate());

			if ("00:00:00.000" != guiiter.getDateTime(colDateTime_date).toStrTime())
				CHECK_EQUAL_DIRECTION(dir, acciter.getDateTime(colDateTime_date).toStrTime(), guiiter.getDateTime(colDateTime_date).toStrTime());

			CHECK_EQUAL_DIRECTION(dir, acciter.getDateTime(colDateTime_time).toStrTime(), guiiter.getDateTime(colDateTime_time).toStrTime());
		}

		// -----------------------------------------------------------------
		void checkNotEqual (Direction::DirEnum             /* dir */, 
							GuiPropertyTable_YIterator      & guiiter , 
							AccessorPropertyTable_YIterator & acciter , 
							BaseMatcherHolderTestSzenario   & /*test*/)
		{
			CHECK( acciter.getDate(colDate).toStrDate()          !=  guiiter.getDate(colDate).toStrDate());
			CHECK( acciter.getDateTime(colDateTime1).toStrDate() !=  guiiter.getDateTime(colDateTime1).toStrDate());
			CHECK( acciter.getDateTime(colDateTime2).toStrDate() !=  guiiter.getDateTime(colDateTime2).toStrDate());

			CHECK( acciter.getDateTime(colDateTime1).toStrTime() !=  guiiter.getDateTime(colDateTime1).toStrTime());
			CHECK( acciter.getDateTime(colDateTime2).toStrTime() !=  guiiter.getDateTime(colDateTime2).toStrTime());
			CHECK( acciter.getTime(colTime).toStrTime()          !=  guiiter.getTime(colTime).toStrTime());

			CHECK( acciter.getDateTime(colDateTime_date).toStrDate() != guiiter.getDateTime(colDateTime_date).toStrDate());
			CHECK( acciter.getDateTime(colDateTime_time).toStrDate() != guiiter.getDateTime(colDateTime_time).toStrDate());
			CHECK( acciter.getDateTime(colDateTime_date).toStrTime() != guiiter.getDateTime(colDateTime_date).toStrTime());
			CHECK( acciter.getDateTime(colDateTime_time).toStrTime() != guiiter.getDateTime(colDateTime_time).toStrTime());
		}

		// -----------------------------------------------------------------
		void checkEqualToText(Direction::DirEnum              dir, 
							  GuiPropertyTable_YIterator    & guiiter ,  
							  BaseMatcherHolderTestSzenario & /*test*/)
		{
			CHECK_EQUAL_DIRECTION(dir, m_defaultDate.toStrDate(), guiiter.getDate    (colDate)     .toStrDate());
			CHECK_EQUAL_DIRECTION(dir, m_defaultDate.toStrDate(), guiiter.getDateTime(colDateTime1).toStrDate());
			CHECK_EQUAL_DIRECTION(dir, m_defaultDate.toStrDate(), guiiter.getDateTime(colDateTime2).toStrDate()); 

			CHECK_EQUAL_DIRECTION(dir, m_defaultTime.toStrTime(), guiiter.getDateTime(colDateTime1).toStrTime());
			CHECK_EQUAL_DIRECTION(dir, m_defaultTime.toStrTime(), guiiter.getDateTime(colDateTime2).toStrTime());
			CHECK_EQUAL_DIRECTION(dir, m_defaultTime.toStrTime(), guiiter.getTime    (colTime)     .toStrTime());

			CHECK_EQUAL_DIRECTION(dir, m_defaultDate.toStrDate(), guiiter.getDateTime(colDateTime_date).toStrDate());
			CHECK_EQUAL_DIRECTION(dir, m_defaultDate.toStrDate(), guiiter.getDateTime(colDateTime_time).toStrDate());
			CHECK_EQUAL_DIRECTION(dir, m_defaultTime.toStrTime(), guiiter.getDateTime(colDateTime_date).toStrTime());
			CHECK_EQUAL_DIRECTION(dir, m_defaultTime.toStrTime(), guiiter.getDateTime(colDateTime_time).toStrTime());
		}
	};

	// --------------------------------------------------------------------------
	TEST_FIXTURE(BasarDateTimeMatcherHolder, GuiTieTest_Matcher_BasarDateTime_Start)
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

				CHECK_EQUAL("31.12.9999"             , guiiter.getDate    (colDate)     .toStrDate    ());
				CHECK_EQUAL("31.12.9999 00:00:00.000", guiiter.getDateTime(colDateTime1).toStrDateTime());
				CHECK_EQUAL("31.12.9999 00:00:00.000", guiiter.getDateTime(colDateTime2).toStrDateTime()); 
				CHECK_EQUAL("31.12.9999 00:00:00.000", guiiter.getTime    (colTime)     .toStrDateTime());
				CHECK_EQUAL("00:00:00.000"           , guiiter.getTime    (colTime)     .toStrTime    ());

				CHECK_EQUAL("31.12.9999 00:00:00.000", guiiter.getDateTime(colDateTime_date).toStrDateTime());
				CHECK_EQUAL("31.12.9999 00:00:00.000", guiiter.getDateTime(colDateTime_time).toStrDateTime());

				CHECK_EQUAL("14.09.1752"		 , m_date     ->text().toLocal8Bit().constData());
				CHECK_EQUAL("14.09.1752 00:00:00", m_datetime1->text().toLocal8Bit().constData());
				CHECK_EQUAL("14.09.1752 00:00:00", m_datetime2->text().toLocal8Bit().constData());
				CHECK_EQUAL("00:00:00"           , m_time     ->text().toLocal8Bit().constData());

				CHECK_EQUAL("14.09.1752 00:00:00", m_datetime_date->text().toLocal8Bit().constData());
				CHECK_EQUAL("14.09.1752 00:00:00", m_datetime_time->text().toLocal8Bit().constData());
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
	TEST_FIXTURE(BasarDateTimeMatcherHolder, GuiTieTest_Matcher_BasarDateTime_RightToLeft_NoVar_oneLine)
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
					CHECK(testiter->start("GuiTieTest_Matcher_BasarDateTime_RightToLeft_NoVar_oneLine", *this));
					TEXTOUTPUT(testiter->toText());
					CHECK(select());

					guiprop = getMatcher().getLeft();
					accprop = getMatcher().getRight();

					CHECK_EQUAL(1, static_cast<int>(accprop.size()));
					CHECK_EQUAL(1, static_cast<int>(guiprop.size()));

					guiiter = guiprop.begin();
					checkEqualToText(Direction::RL, guiiter , *testiter);


					CHECK_EQUAL(0,guiprop.getAbsoluteIndex());

					if ( testiter->throwsToManyEntriesException() )
					{
						CHECK_THROW( getMatcher().RightToLeft(), basar::Exception);
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

						CHECK_EQUAL("01.01.2000"		 , m_date     ->text().toLocal8Bit().constData());
						CHECK_EQUAL("01.01.2000 01:02:03", m_datetime1->text().toLocal8Bit().constData());
						CHECK_EQUAL("01.01.2000 01:02:03", m_datetime2->text().toLocal8Bit().constData());
						CHECK_EQUAL("01:02:03"           , m_time     ->text().toLocal8Bit().constData());

						CHECK_EQUAL("31.08.2019 23:59:59", m_datetime_date->text().toLocal8Bit().constData());
						CHECK_EQUAL("14.09.1752 23:46:57", m_datetime_time->text().toLocal8Bit().constData());

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
	TEST_FIXTURE(BasarDateTimeMatcherHolder, GuiTieTest_Matcher_BasarDateTime_LeftToRight_NoVar_oneLine)
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
					CHECK(testiter->start("GuiTieTest_Matcher_BasarDateTime_LeftToRight_NoVar_oneLine", *this));

					CHECK(select());

					accprop = getMatcher().getRight();
					guiprop = getMatcher().getLeft();
					CHECK_EQUAL(1, static_cast<int>(accprop.size()));
					CHECK_EQUAL(1, static_cast<int>(guiprop.size()));
					CHECK_EQUAL(0,guiprop.getAbsoluteIndex());

					if ( testiter->throwsToManyEntriesException() )
					{
						CHECK_THROW( getMatcher().RightToLeft(), basar::Exception);
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

						CHECK_EQUAL("25.08.2008"		 , m_date     ->text().toLocal8Bit().constData());
						CHECK_EQUAL("25.08.2008 01:01:01", m_datetime1->text().toLocal8Bit().constData());
						CHECK_EQUAL("25.08.2008 01:01:01", m_datetime2->text().toLocal8Bit().constData());
						CHECK_EQUAL("01:01:01"           , m_time     ->text().toLocal8Bit().constData());

						CHECK_EQUAL("25.08.2008 01:01:01", m_datetime_date->text().toLocal8Bit().constData());
						CHECK_EQUAL("25.08.2008 01:01:01", m_datetime_time->text().toLocal8Bit().constData());

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
	TEST_FIXTURE(BasarDateTimeMatcherHolder, GuiTieTest_Matcher_BasarDateTime_RightToLeft_iter_oneLine)
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
					CHECK(testiter->start("GuiTieTest_Matcher_BasarDateTime_RightToLeft_iter_oneLine", *this));
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

					if ( testiter->throwsToManyEntriesException() )
					{
						CHECK_THROW( getMatcher().RightToLeft(guiiter, acciter), basar::Exception);
					}
					else
					{          
						getMatcher().RightToLeft(guiiter, acciter);
						checkEqual(Direction::RL, guiiter , acciter, *testiter);

						CHECK_EQUAL("01.01.2000"		 , m_date     ->text().toLocal8Bit().constData());
						CHECK_EQUAL("01.01.2000 01:02:03", m_datetime1->text().toLocal8Bit().constData());
						CHECK_EQUAL("01.01.2000 01:02:03", m_datetime2->text().toLocal8Bit().constData());
						CHECK_EQUAL("01:02:03"           , m_time     ->text().toLocal8Bit().constData());

						CHECK_EQUAL("31.08.2019 23:59:59", m_datetime_date->text().toLocal8Bit().constData());
						CHECK_EQUAL("14.09.1752 23:46:57", m_datetime_time->text().toLocal8Bit().constData());

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
	TEST_FIXTURE(BasarDateTimeMatcherHolder, GuiTieTest_Matcher_BasarDateTime_LeftToRight_iter_oneLine)
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
					CHECK(testiter->start("GuiTieTest_Matcher_BasarDateTime_LeftToRight_iter_oneLine", *this));

					CHECK(select());

					accprop = getMatcher().getRight();
					guiprop = getMatcher().getLeft();
					CHECK_EQUAL(1, static_cast<int>(accprop.size()));
					CHECK_EQUAL(1, static_cast<int>(guiprop.size()));

					guiiter = guiprop.begin();
					acciter = accprop.begin();

					CHECK_EQUAL(0,guiprop.getAbsoluteIndex());

					if ( testiter->throwsToManyEntriesException() )
					{
						CHECK_THROW( getMatcher().LeftToRight(guiiter, acciter), basar::Exception);
					}
					else
					{  
						getMatcher().LeftToRight(guiiter, acciter);
						checkEqual(Direction::LR, guiiter , acciter, *testiter);

						CHECK_EQUAL("25.08.2008"		 , m_date     ->text().toLocal8Bit().constData());
						CHECK_EQUAL("25.08.2008 01:01:01", m_datetime1->text().toLocal8Bit().constData());
						CHECK_EQUAL("25.08.2008 01:01:01", m_datetime2->text().toLocal8Bit().constData());
						CHECK_EQUAL("01:01:01"           , m_time     ->text().toLocal8Bit().constData());

						CHECK_EQUAL("25.08.2008 01:01:01", m_datetime_date->text().toLocal8Bit().constData());
						CHECK_EQUAL("25.08.2008 01:01:01", m_datetime_time->text().toLocal8Bit().constData());
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
	TEST_FIXTURE(BasarDateTimeMatcherHolder, GuiTieTest_Matcher_BasarDateTime_RightToLeft_int_oneLine)
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
					CHECK(testiter->start("GuiTieTest_Matcher_BasarDateTime_RightToLeft_int_oneLine", *this));
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

					if ( testiter->throwsToManyEntriesException() )
					{
						CHECK_THROW( getMatcher().RightToLeft(guiiter, acciter), basar::Exception);
					}
					else
					{          
						CHECK_EQUAL(1,getMatcher().RightToLeft(0, 1));
						checkEqual(Direction::RL, guiiter , acciter, *testiter);

						resetGuiValues();
						checkEqualToText(Direction::RL, guiiter , *testiter);

						CHECK_EQUAL(1,getMatcher().RightToLeft(0, 5));
						checkEqual(Direction::RL, guiiter , acciter, *testiter);

						CHECK_EQUAL("01.01.2000"		 , m_date     ->text().toLocal8Bit().constData());
						CHECK_EQUAL("01.01.2000 01:02:03", m_datetime1->text().toLocal8Bit().constData());
						CHECK_EQUAL("01.01.2000 01:02:03", m_datetime2->text().toLocal8Bit().constData());
						CHECK_EQUAL("01:02:03"           , m_time     ->text().toLocal8Bit().constData());

						CHECK_EQUAL("31.08.2019 23:59:59", m_datetime_date->text().toLocal8Bit().constData());
						CHECK_EQUAL("14.09.1752 23:46:57", m_datetime_time->text().toLocal8Bit().constData());

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
	TEST_FIXTURE(BasarDateTimeMatcherHolder, GuiTieTest_Matcher_BasarDateTime_LeftToRight_int_oneLine)
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
					CHECK(testiter->start("GuiTieTest_Matcher_BasarDateTime_LeftToRight_iter_oneLine", *this));

					CHECK(select());

					accprop = getMatcher().getRight();
					guiprop = getMatcher().getLeft();
					CHECK_EQUAL(1, static_cast<int>(accprop.size()));
					CHECK_EQUAL(1, static_cast<int>(guiprop.size()));

					guiiter = guiprop.begin();
					acciter = accprop.begin();

					CHECK_EQUAL(0,guiprop.getAbsoluteIndex());

					if ( testiter->throwsToManyEntriesException() )
					{
						CHECK_THROW( getMatcher().LeftToRight(0, 1), basar::Exception);
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

						CHECK_EQUAL("25.08.2008"		 , m_date     ->text().toLocal8Bit().constData());
						CHECK_EQUAL("25.08.2008 01:01:01", m_datetime1->text().toLocal8Bit().constData());
						CHECK_EQUAL("25.08.2008 01:01:01", m_datetime2->text().toLocal8Bit().constData());
						CHECK_EQUAL("01:01:01"           , m_time     ->text().toLocal8Bit().constData());

						CHECK_EQUAL("25.08.2008 01:01:01", m_datetime_date->text().toLocal8Bit().constData());
						CHECK_EQUAL("25.08.2008 01:01:01", m_datetime_time->text().toLocal8Bit().constData());

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
	TEST_FIXTURE(BasarDateTimeMatcherHolder, GuiTieTest_Matcher_BasarDateTime_RightToLeft_iter_policy_oneLine)
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
					CHECK(testiter->start("GuiTieTest_Matcher_BasarDateTime_RightToLeft_iter_policy_oneLine", *this));
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
							if ( testiter->throwsToManyEntriesException(*policyiter) )
							{
								CHECK_THROW( getMatcher().RightToLeft(guiiter, acciter, *policyiter), basar::Exception);
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
	TEST_FIXTURE(BasarDateTimeMatcherHolder, GuiTieTest_Matcher_BasarDateTime_LeftToRight_iter_policy_oneLine)
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
					CHECK(testiter->start("GuiTieTest_Matcher_BasarDateTime_LeftToRight_iter_policy_oneLine", *this));

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

							if ( testiter->throwsToManyEntriesException(*policyiter) )
							{
								CHECK_THROW( getMatcher().LeftToRight(guiiter, acciter, *policyiter), basar::Exception);
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
