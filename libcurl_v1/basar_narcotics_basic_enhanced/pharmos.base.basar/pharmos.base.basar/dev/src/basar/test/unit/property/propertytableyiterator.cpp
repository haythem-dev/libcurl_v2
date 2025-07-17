/*
* @file                                 
* @author Anke Klink                      
* @date 2008                            
*/    


#include <UnitTest++/UnitTest++.h>

#include "libbasarproperty.h"
#include "propertydescriptionlist.h"
#include "propertytable.h"
#include "loggerpool.h"

using basar::Int32;
using basar::UInt32;
using basar::VarString;
using basar::ConstString;

using basar::property::PropertyDescriptionListRef;
using basar::property::PropertyDescriptionList;
using basar::property::PropertyDescription;
using basar::property::PropertyType;
using basar::property::PropertyDescriptionVector;
using basar::property::PropertyStateSet;
using basar::property::PropertyTableRef;
using basar::property::PropertyTable;
using basar::property::PropertyTable_YIterator;

//------------------------------------------------------------------------------
SUITE(PropertyTable_YIterator)
{
//// ----------------------------------------------------------

CONST_PROPERTY_DESCRIPTION(testcol1, "testcol1", basar::INT32)
CONST_PROPERTY_DESCRIPTION(testcol2, "testcol2", basar::INT32)

//// ----------------------------------------------------------
class Fixture
{		
public:
	PropertyTableRef  m_propertyTableRef;
	int               m_unsetColumn ;
	int               m_setColumn  ;

	std::vector<basar::SupportedStateEnum> m_enums;


	Fixture() : m_propertyTableRef(PropertyTable::create(initPropertyDescriptionListRef())),
		        m_unsetColumn (0),
		        m_setColumn   (1)
	{

		m_enums.push_back(basar::SS_CLEAN);
		m_enums.push_back(basar::SS_DELETE);
		m_enums.push_back(basar::SS_INSERT);
		m_enums.push_back(basar::SS_UNKNOWN);
		m_enums.push_back(basar::SS_UNSET);
		m_enums.push_back(basar::SS_UPDATE);

	}

	~Fixture()
	{
	}

	static PropertyDescriptionListRef initPropertyDescriptionListRef()
	{
		PropertyDescriptionListRef retval( PropertyDescriptionList::create() );

		retval.push_back( testcol1  );
		retval.push_back( testcol2 );

		return retval;
	}

	PropertyTable_YIterator checkOneNewLine(basar::InsertTypeEnum     forEnum, 
											basar::SupportedStateEnum ssEnum)
	{
		PropertyTable_YIterator yiter = m_propertyTableRef.insert(forEnum);

		CHECK_EQUAL(basar::SS_UNSET, yiter.getState(0).getState());
		CHECK_EQUAL(basar::SS_UNSET, yiter.getState(1).getState());
		CHECK_EQUAL(basar::SS_UNSET, yiter.getState().getStates());

		yiter.setInt32(m_setColumn,0);
		CHECK_EQUAL(ssEnum, yiter.getState(m_setColumn).getState());
		CHECK_EQUAL(basar::SS_UNSET, yiter.getState(m_unsetColumn).getState());

		PropertyStateSet ps = yiter.getState().getStates();
		CHECK_EQUAL(ssEnum, ps.getStates()); 

		std::vector<basar::SupportedStateEnum>::const_iterator iter;
		if (basar::SS_UNKNOWN != ssEnum)
		{
			for (iter = m_enums.begin(); iter != m_enums.end(); ++iter)
			{
				//TEXTOUTPUT2( *iter << " ? " << ssEnum);
				CHECK_EQUAL((*iter) == ssEnum, ps.contains(*iter)); 
			}
		}
		else // SS_UNKNOWN contains all:
		{
			for (iter = m_enums.begin(); iter != m_enums.end(); ++iter)
			{ 
				CHECK(ps.contains(*iter)); 
			}
		}

		return yiter;
	}
};

// -------------------------------------------------------------------------------------------
TEST_FIXTURE (Fixture, InsertRows)
{
	try
	{
		PropertyTable_YIterator yiter  = m_propertyTableRef.begin();
		PropertyTable_YIterator yiter2 = m_propertyTableRef.begin();
		CHECK(yiter.isBegin());
		CHECK(yiter.isEnd());

		yiter  = checkOneNewLine(basar::FOR_INSERT , basar::SS_INSERT);
		yiter  = checkOneNewLine(basar::FOR_UPDATE , basar::SS_UPDATE);
		yiter  = checkOneNewLine(basar::FOR_DELETE , basar::SS_DELETE);
		yiter  = checkOneNewLine(basar::FOR_UNKNOWN, basar::SS_UNKNOWN);

		yiter2 = m_propertyTableRef.markForDelete(yiter);
		CHECK_EQUAL(basar::SS_UNSET , yiter2.getState(m_unsetColumn).getState());
		CHECK_EQUAL(basar::SS_DELETE, yiter2.getState(m_setColumn  ).getState());
		CHECK_EQUAL(basar::SS_UNSET , yiter .getState(m_unsetColumn).getState());
		CHECK_EQUAL(basar::SS_DELETE, yiter .getState(m_setColumn  ).getState());

		yiter = checkOneNewLine(basar::FOR_CLEAN, basar::SS_CLEAN);

		yiter2= m_propertyTableRef.markForDelete(yiter);
		CHECK_EQUAL(basar::SS_UNSET , yiter2.getState(m_unsetColumn).getState());
		CHECK_EQUAL(basar::SS_DELETE, yiter2.getState(m_setColumn  ).getState());
		CHECK_EQUAL(basar::SS_UNSET , yiter .getState(m_unsetColumn).getState());
		CHECK_EQUAL(basar::SS_DELETE, yiter .getState(m_setColumn  ).getState());
	}
	catch (basar::Exception & ex)
	{
		std::cout 
			<< ex.what() << std::endl;
		const bool ExceptionOccured = false;
		CHECK(ExceptionOccured);
		throw;
	}
}

// -------------------------------------------------------------------------------------------
TEST_FIXTURE (Fixture, SetPropertyState)
{
	PropertyTable_YIterator yiter = m_propertyTableRef.begin();
	CHECK(yiter.isBegin());
	CHECK(yiter.isEnd());

	{
		try
		{
			yiter = m_propertyTableRef.insert(basar::FOR_UNKNOWN);

			CHECK_EQUAL(basar::SS_UNSET, yiter.getState(0).getState());
			CHECK_EQUAL(basar::SS_UNSET, yiter.getState(1).getState());

			yiter.setInt32(0,0);
			CHECK_EQUAL(basar::SS_UNKNOWN, yiter.getState(0).getState());
			CHECK_EQUAL(basar::SS_UNSET, yiter.getState(1).getState());

			// no exception, as it is the same PropertyState (op 193b)
			yiter.setPropertyState(0, basar::SS_UNKNOWN);
			yiter.setPropertyState(1, basar::SS_UNSET);

			CHECK_THROW(yiter.setPropertyState(1, basar::SS_UNKNOWN), basar::Exception);

			// can't change:
			CHECK_THROW(yiter.setPropertyState(0, basar::SS_INSERT), basar::Exception);
			CHECK_THROW(yiter.setPropertyState(1, basar::SS_INSERT), basar::Exception);

			// check OP 193:
			CHECK_EQUAL(basar::SS_UNKNOWN, yiter.getState(0).getState());
			CHECK_EQUAL(basar::SS_UNSET,   yiter.getState(1).getState());

			CHECK_EQUAL(basar::SS_UNKNOWN, yiter.getState(0).getState());
			CHECK_EQUAL(basar::SS_UNSET,   yiter.getState(1).getState());

			// no exception, as it is the same PropertyState (op 193b)
			yiter.setPropertyState(0, basar::SS_UNKNOWN);
			yiter.setPropertyState(1, basar::SS_UNSET);

		}
		catch (basar::Exception & ex)
		{
			std::cout 
				<< ex.what() << std::endl;
			const bool ExceptionOccured = false;
			CHECK(ExceptionOccured);
			throw;
		}
	}
}

// -------------------------------------------------------------------------------------------
// check handling of assigned iterator for states SS_UNKNOWN, SS_UNSET
TEST_FIXTURE(Fixture, AssignStates_CopyCtor)
{
	try
	{
		// SS_UNKNOWN -> SS_ALL_STATES_SET
		for (Int32 state = 0x01; state < 0x40; ++state)
		{
			PropertyTable_YIterator yit(state);

			CHECK_EQUAL(0U   , yit.getFilteredStep()            );
			CHECK_EQUAL(state, yit.getFilter      ().getStates());


			PropertyTable_YIterator yitAssigned(yit);

			CHECK_EQUAL(0U, yitAssigned.getFilteredStep());

			if (basar::SS_UNSET == state)
				CHECK_EQUAL(basar::SS_UNKNOWN, yitAssigned.getFilter().getStates());
			else
				CHECK_EQUAL(state            , yitAssigned.getFilter().getStates());
		}
	}
	catch (basar::Exception & ex)
	{
		std::cout 
			<< ex.what() << std::endl;
		const bool ExceptionOccured = false;
		CHECK(ExceptionOccured);
		throw;
	}
	catch (...)
	{
		std::cout << "unknown exception" << std::endl;
		const bool ExceptionOccured = false;
		CHECK(ExceptionOccured);
	}
}

// -------------------------------------------------------------------------------------------
// check handling of assigned iterator for states SS_UNKNOWN, SS_UNSET
TEST_FIXTURE(Fixture, AssignStates_AssignOperator)
{
	try
	{
		// SS_UNKNOWN -> SS_ALL_STATES_SET
		for (Int32 state = 0x01; state < 0x40; ++state)
		{
			PropertyTable_YIterator yit(state);

			CHECK_EQUAL(0U   , yit.getFilteredStep()            );
			CHECK_EQUAL(state, yit.getFilter      ().getStates());


			PropertyTable_YIterator yitAssigned(basar::SS_UPDATE | basar::SS_INSERT);

			yitAssigned = yit;

			CHECK_EQUAL(0U, yitAssigned.getFilteredStep());

			if (basar::SS_UNSET == state)
				CHECK_EQUAL(basar::SS_UNKNOWN, yitAssigned.getFilter().getStates());
			else
				CHECK_EQUAL(state            , yitAssigned.getFilter().getStates());
		}
	}
	catch (basar::Exception & ex)
	{
		std::cout 
			<< ex.what() << std::endl;
		const bool ExceptionOccured = false;
		CHECK(ExceptionOccured);
		throw;
	}
	catch (...)
	{
		std::cout << "unknown exception" << std::endl;
		const bool ExceptionOccured = false;
		CHECK(ExceptionOccured);
	}
}

// -------------------------------------------------------------------------------------------
CONST_PROPERTY_DESCRIPTION(colint, "colint", basar::INT32)
CONST_PROPERTY_DESCRIPTION(colstr, "colstr", basar::STRING)

// -------------------------------------------------------------------------------------------
TEST(ToStream)
{
	using std::ostringstream;
	using basar::FOR_CLEAN;

	try
	{
		{
			PropertyTable_YIterator yit;
			CHECK(yit.isNull());
			CHECK_THROW(yit.toStream(), basar::NullReferenceException);
		}

		{
			PropertyTable_YIterator yit;

			PropertyDescriptionListRef pdl(PropertyDescriptionList::create());
			pdl.push_back(colint);
			pdl.push_back(colstr);

			PropertyTableRef propTbl(PropertyTable::create(pdl));

			{
				CHECK(propTbl.empty());

				yit = propTbl.begin();
				CHECK(!yit.isNull());
				CHECK(propTbl.end() == yit);

				ostringstream out;
				yit.toStream(out);
				CHECK_EQUAL("", out.str().c_str());
			}

			{
				ConstString EXPECT = 
					"--- PropertyTable_YIterator @ 0x%0*lx -------------------\n"
					"#0     name : <colint>\n"
					"       value: <>\n"
					"       state: <SS_UNSET>\n"
					"       type : <int32>\n"
					"------------------------------------------------\n"
					"#1     name : <colstr>\n"
					"       value: <>\n"
					"       state: <SS_UNSET>\n"
					"       type : <string>\n"
					"------------------------------------------------\n";

				VarString exp;
				exp.format(EXPECT, sizeof(size_t) * 2, &yit);

				propTbl.insert(FOR_CLEAN);
				CHECK(!propTbl.empty());

				yit = propTbl.begin();
				CHECK(!yit.isNull());
				CHECK(propTbl.end() != yit);

				ostringstream out;
				yit.toStream(out);
				CHECK_EQUAL(exp.c_str(), out.str().c_str());
			}

			{
				ConstString EXPECT = 
					"--- PropertyTable_YIterator @ 0x%0*lx -------------------\n"
					"#0     name : <colint>\n"
					"       value: <3>\n"
					"       state: <SS_CLEAN>\n"
					"       type : <int32>\n"
					"------------------------------------------------\n"
					"#1     name : <colstr>\n"
					"       value: <abc>\n"
					"       state: <SS_CLEAN>\n"
					"       type : <string>\n"
					"------------------------------------------------\n";

				VarString exp;
				exp.format(EXPECT, sizeof(size_t) * 2, &yit);

				CHECK(propTbl.end() != yit);

				yit.setInt32 (colint.getName(), 3    );
				yit.setString(colstr.getName(), "abc");

				ostringstream out;
				yit.toStream(out);
				CHECK_EQUAL(exp.c_str(), out.str().c_str());
			}
		}
	}
    catch(basar::Exception & ex)
    {
        std::cout << ex.what() << std::endl;
        const bool f = false;
        CHECK(f);
    }

}

// -------------------------------------------------------------------------------------------
TEST_FIXTURE(Fixture, SafeAccess)
{
	PropertyTable_YIterator yIt;

	// no PropTbl
	CHECK_THROW(yIt.getInt32(testcol1.getName()), basar::NullReferenceException);

	// no PropTbl
	yIt.reset();
	CHECK_THROW(yIt.getInt32(testcol1.getName()), basar::NullReferenceException);

	// yIt == m_propertyTableRef.end()
	yIt = m_propertyTableRef.begin();
	//CHECK_THROW(yIt.getInt32(testcol1.getName()), basar::InvalidIteratorException);

	// insert row and set value
	yIt = m_propertyTableRef.insert(basar::FOR_CLEAN);
	yIt.setInt32(testcol1.getName(), 4);
	CHECK_EQUAL(4, yIt.getInt32(testcol1.getName()));
}

// -------------------------------------------------------------------------------------------
TEST_FIXTURE(Fixture, UnsafeAccess)
{
	PropertyTable_YIterator yIt;

	try
	{
		// no PropTbl
		CHECK_THROW(yIt.getInt32(testcol1.getName()), basar::NullReferenceException);

		// no PropTbl
		yIt.reset();
		CHECK_THROW(yIt.getInt32(testcol1.getName()), basar::NullReferenceException);

#if defined _WIN32 && !defined NDEBUG
		//{
		//	bool ExceptionAccessViolation = false;	

		//	try
		//	{
		//		// yIt == m_propertyTableRef.end()
		//		yIt = m_propertyTableRef.begin();
		//		yIt.getInt32(testcol1.getName());
		//	}
		//	catch (...)
		//	{
		//		ExceptionAccessViolation = true;		// probably access violation
		//	}
		//	CHECK(ExceptionAccessViolation);
		//}
#endif

		// insert row and set value
		yIt = m_propertyTableRef.insert(basar::FOR_CLEAN);
		yIt.setInt32(testcol1.getName(), 4);
		CHECK_EQUAL(4, yIt.getInt32(testcol1.getName()));
	}
	catch (basar::Exception & ex)
	{
		std::cout << ex.what() << std::endl;
		const bool ExceptionOccured = false;
		CHECK(ExceptionOccured);
	}
	catch (...)
	{
		std::cout << "unknown exception" << std::endl;
		const bool ExceptionOccured = false;
		CHECK(ExceptionOccured);
	}
}

// -------------------------------------------------------------------------------------------
TEST_FIXTURE(Fixture, Performance_for_Check)
{
	static const int LIM = 2000;

	PropertyTable_YIterator yIt;
	UnitTest::Timer         tm;
	double                  ms;
	VarString               str;

	{
		tm.Start();

		for (int i = 0; i < LIM; ++i)
		{
			yIt = m_propertyTableRef.insert(basar::FOR_CLEAN);
			yIt.setInt32(testcol1.getName(),  4);
			yIt.setInt32(testcol2.getName(), -4);
		}

		ms = tm.GetTimeInMs();
		str.format("filling PropertyTable   : %7.1f ms for %d rows", ms, LIM);
		TEXTOUTPUT_INFO(str);
		CHECK_EQUAL(LIM, (int)m_propertyTableRef.size());
	}

	{

		tm.Start();

		yIt = m_propertyTableRef.begin();

		for (int i = 0; i < LIM; ++i)
		{
			++yIt;
		}

		ms = tm.GetTimeInMs();

		str.format("increment unchecked yIt : %7.1f ms", ms);
		TEXTOUTPUT_INFO(str);
	}

	{

		tm.Start();

		yIt = m_propertyTableRef.begin();

		for (int i = 0; i < LIM; ++i)
		{
			++yIt;
		}

		ms = tm.GetTimeInMs();

		str.format("increment checked   yIt : %7.1f ms", ms);
		TEXTOUTPUT_INFO(str);
	}

	{

		tm.Start();

		yIt = m_propertyTableRef.begin();

		for (int i = 0; i < LIM; ++i, ++yIt)
		{
			yIt.getInt32(testcol1.getName());
			yIt.getInt32(testcol2.getName());
		}

		ms = tm.GetTimeInMs();

		str.format("getting unchecked values: %7.1f ms", ms);
		TEXTOUTPUT_INFO(str);
	}

	{

		tm.Start();

		yIt = m_propertyTableRef.begin();

		for (int i = 0; i < LIM; ++i, ++yIt)
		{
			yIt.getInt32(testcol1.getName());
			yIt.getInt32(testcol2.getName());
		}

		ms = tm.GetTimeInMs();

		str.format("getting checked   values: %7.1f ms", ms);
		TEXTOUTPUT_INFO(str);
	}
}

// ----------------------------------------------------------
}	// SUITE
// ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~  ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~
