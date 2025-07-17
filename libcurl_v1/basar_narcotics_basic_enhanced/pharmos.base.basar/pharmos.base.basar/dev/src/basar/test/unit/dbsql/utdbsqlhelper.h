/*
* @file utdbsqlhelper.h                                                   
* @brief ...                                       
* @author Anke Klink                      
* @date 2007                            
*/    

#ifndef  ___MARATHON_LIB_BASAR_TEST_UNIT_DBSQL_UTDBSQLHELPER_H__ 
#define  ___MARATHON_LIB_BASAR_TEST_UNIT_DBSQL_UTDBSQLHELPER_H__ 

//! Copy from u-nitTest with special output with toString() or other function (outputfunction)
#define CHECK_EQUAL_SHOW_FUNCT(expected, actual, outputfunction) \
	do \
	{ \
	try { \
	if (!(expected == actual)) \
			{  \
			UnitTest::MemoryOutStream stream; \
			stream << "Expected " << expected.outputfunction() << " but was " << actual.outputfunction(); \
			UnitTest::CurrentTest::Results()->OnTestFailure(UnitTest::TestDetails(*UnitTest::CurrentTest::Details(), __LINE__), stream.GetText()); \
			} \
	} \
	catch (...) { \
	UnitTest::CurrentTest::Results()->OnTestFailure(UnitTest::TestDetails(*UnitTest::CurrentTest::Details(), __LINE__), \
	"Unhandled exception in CHECK_EQUAL_TOSTRING(" #expected ", " #actual ")"); \
	} \
	} while (0)

//! Copy from u-nitTest with special output with outputfunction1 / outputfunction2
#define CHECK_EQUAL_SHOW_FUNCT2(expected, actual, outputfunction1 ,outputfunction2 ) \
	do \
	{ \
	try { \
	if (!(expected == actual)) \
			{  \
			UnitTest::MemoryOutStream stream; \
			stream << "Expected " << expected.outputfunction1() << " " << expected.outputfunction2() \
			<< " but was " << actual.outputfunction1()<< " " << actual.outputfunction2() ; \
			UnitTest::CurrentTest::Results()->OnTestFailure(UnitTest::TestDetails(*UnitTest::CurrentTest::Details(), __LINE__), stream.GetText()); \
			} \
	} \
	catch (...) { \
	UnitTest::CurrentTest::Results()->OnTestFailure(UnitTest::TestDetails(*UnitTest::CurrentTest::Details(), __LINE__), \
	"Unhandled exception in CHECK_EQUAL_TOSTRING(" #expected ", " #actual ")"); \
	} \
	} while (0)

namespace basar
{
	namespace test
	{
		namespace unit
		{
			namespace dbsql
			{
				//! some Defines for the Unit Tests
				struct Defines
				{
					static const basar::Float64 csm_defaultDecimalCloseTolerance; //!< Tolenance on equal function for Decimal Values
					static const basar::Int32 csm_int32DefaultNullValue;          //!< Default Null Value
					static const basar::Int16 csm_int16DefaultNullValue;        //!< Default Null Value
					static const basar::Float64 csm_float64DefaultNullValue;    //!< Default Null Value
					static const basar::Float32 csm_float32DefaultNullValue;    //!< Default Null Value
					static const basar::Decimal csm_decimalDefaultNullValue;    //!< Default Null Value
					static const basar::Date csm_dateDefaultNullValue;          //!< Default Null Value
					//static const basar::DateTime csm_dateTimeDefaultNullValue;  //!< Default Null Value
					static const basar::Date getDateDefaultNullValue();         //!< Default Null Value
					//static const basar::DateTime getDateTimeDefaultNullValue(); //!< Default Null Value
					static const basar::ConstString csm_stringDefaultNullValue; //!< Default Null Value

				};
			}
		}
	}
}
#endif 

// ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~  ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~
