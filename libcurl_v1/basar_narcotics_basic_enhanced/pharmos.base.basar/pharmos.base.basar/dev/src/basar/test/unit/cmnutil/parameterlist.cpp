/** $Id$                                                                                        
* @file parameterlist.cpp                                                                      
* @author Anke Klink                      
* @date 2007                            
*/    

#include <UnitTest++/UnitTest++.h>
#include "libbasarcmnutil.h"

using basar::cmnutil::DivisionByZeroException;
using basar::cmnutil::OverflowException;
using basar::cmnutil::ParameterList;
using basar::cmnutil::ParameterList_Iterator;
using basar::cmnutil::BString;

using basar::Int16;
using basar::Int32;
using basar::Float32;
using basar::Float64;
using basar::VarString;
using basar::ConstString;


//------------------------------------------------------------------------------
SUITE(TestParameterList)
{
	//-------------------------------------------------------------------------
	TEST(TestParameterList_ParameterListIterator_isNull_reset_isBegin_isEnd)
	{
		ParameterList_Iterator testiter;
		CHECK(testiter.isNull());
		testiter.reset();
		CHECK(testiter.isNull());
		ParameterList list;
		CHECK(list.begin() == list.end());

		testiter = list.begin();
		CHECK(!testiter.isNull());
		CHECK(testiter.isBegin());
		CHECK(testiter.isEnd());

		testiter = list.end();
		CHECK(!testiter.isNull());
		CHECK(testiter.isEnd());
		CHECK(testiter.isBegin());

		basar::cmnutil::ParamListSize size = 0;
		CHECK(size == list.size());
 
		testiter.reset();
		CHECK(testiter.isNull()); 
	} 

	//-------------------------------------------------------------------------
	TEST(Keys_and_values)
	{
		{
			VarString pairlist1 = " key-2 =46363125;"
								"key-1 = -6867913 ;"
								" key0 = -33216 ;"
								"key1=val1;"
								"key2  = val2   ;"
								"key3 ;"
								"key4;"
								"key5= =====;"
								"key6a=;"
								"key6b= ;"
								"key6c =;"
								"key6d = ;"
								";"
								"=;"
								"key7= val 7 ";
			ParameterList list1(pairlist1);

			CHECK(!list1.isValueEmpty("key-1"));
			CHECK(!list1.isValueEmpty("key-2"));
			CHECK(!list1.isValueEmpty("key0"));
			CHECK(!list1.isValueEmpty("key1"));
			CHECK(!list1.isValueEmpty("key2"));
			CHECK( list1.isValueEmpty("key3"));
			CHECK( list1.isValueEmpty("key4"));
			CHECK(!list1.isValueEmpty("key5"));
			CHECK( list1.isValueEmpty("key6a"));
			CHECK( list1.isValueEmpty("key6b"));
			CHECK( list1.isValueEmpty("key6c"));
			CHECK( list1.isValueEmpty("key6d"));
			CHECK( list1.isValueEmpty(""));
			CHECK(!list1.isValueEmpty("key7"));

			CHECK_THROW(list1.isValueEmpty("nokey"), basar::UnknownPropertyNameException);

			CHECK_EQUAL("-6867913", list1.getValue("key-1"));
			CHECK_EQUAL("46363125", list1.getValue("key-2"));
			CHECK_EQUAL("-33216"  , list1.getValue("key0"));
			CHECK_EQUAL("val1"    , list1.getValue("key1"));
			CHECK_EQUAL("val2"    , list1.getValue("key2"));
			CHECK_EQUAL(""        , list1.getValue("key3"));
			CHECK_EQUAL(""        , list1.getValue("key4"));
			CHECK_EQUAL("====="   , list1.getValue("key5"));
			CHECK_EQUAL(""        , list1.getValue("key6a"));
			CHECK_EQUAL(""        , list1.getValue("key6b"));
			CHECK_EQUAL(""        , list1.getValue("key6c"));
			CHECK_EQUAL(""        , list1.getValue("key6d"));
			CHECK_EQUAL(""        , list1.getValue(""));
			CHECK_EQUAL("val 7"   , list1.getValue("key7"));
			CHECK_THROW(list1.getValue("nokey"), basar::UnknownPropertyNameException);

			CHECK(list1.isKeyContained("key1"));
			CHECK(list1.isKeyContained("key3")); 
			CHECK(list1.isKeyContained("")); 
			CHECK(!list1.isKeyContained("nokey"));
		}

		{
			ParameterList list("");

			CHECK      ( !list.isKeyContained("")); 
			CHECK_THROW(list.isValueEmpty(""), basar::UnknownPropertyNameException);
			CHECK_THROW(list.getValue    (""), basar::UnknownPropertyNameException);
		}

		{
			ParameterList list("=");

			CHECK      (list.isKeyContained("")); 
			CHECK      (list.isValueEmpty(""));
			CHECK_EQUAL("", list.getValue(""));
		}

		{
			ParameterList list("=;");

			CHECK      (list.isKeyContained("")); 
			CHECK      (list.isValueEmpty(""));
			CHECK_EQUAL("", list.getValue(""));
		}

		{
			ParameterList list("=abcd");

			CHECK      (list.isKeyContained("")); 
			CHECK      (!list.isValueEmpty(""));
			CHECK_EQUAL("abcd", list.getValue(""));
		}

		{
			ParameterList list("=abcd;");

			CHECK      (list.isKeyContained("")); 
			CHECK      (!list.isValueEmpty(""));
			CHECK_EQUAL("abcd", list.getValue(""));
		}

		{
			ParameterList list("A =abcd efg;");

			CHECK      (list.isKeyContained("A")); 
			CHECK      (!list.isValueEmpty("A"));
			CHECK_EQUAL("abcd efg", list.getValue("A"));
		}

		{
			ParameterList list("A = abcd efg;");

			CHECK      (list.isKeyContained("A")); 
			CHECK      (!list.isValueEmpty("A"));
			CHECK_EQUAL("abcd efg", list.getValue("A"));
		}
	}

	//-------------------------------------------------------------------------
	TEST(Iterator)
	{
		VarString pairlist1 = " key-2 =46363125;"
			                  "key-1 = -6867913 ;"
			                  " key0 = -33216 ;"
							  "key1=val1;"
							  "key2  = val2   ;"
							  "key3 ;"
							  "key4;"
							  "key5= =====;"
							  ";"
							  "key6;"
							  "key7= val 7 ";
		ParameterList list1(pairlist1);

		// regard: map is sorted(!)
		ParameterList_Iterator iter = list1.begin();
		CHECK_EQUAL("key-1",iter->first);
		CHECK_EQUAL("-6867913",iter->second);
		++iter;
		CHECK_EQUAL("key-2",iter->first);
		CHECK_EQUAL("46363125",iter->second);
		++iter;
		CHECK_EQUAL("key0",iter->first);
		CHECK_EQUAL("-33216",iter->second);
		++iter;
		CHECK_EQUAL("key1",iter->first);
		CHECK_EQUAL("val1",iter->second);
		++iter;
		CHECK_EQUAL("key2",iter->first);
		CHECK_EQUAL("val2",iter->second);
		++iter;
		CHECK_EQUAL("key3",iter->first);
		CHECK_EQUAL("",iter->second);
		++iter;
		CHECK_EQUAL("key4",iter->first);
		CHECK_EQUAL("",iter->second);
		++iter;
		CHECK_EQUAL("key5",iter->first);
		CHECK_EQUAL("=====",iter->second);
		++iter;
		CHECK_EQUAL("key6",iter->first);
		CHECK_EQUAL("",iter->second);
		++iter;
		CHECK_EQUAL("key7",iter->first);
		CHECK_EQUAL("val 7",iter->second);
		++iter;
		CHECK(iter.isEnd());
	}

	//-------------------------------------------------------------------------
	TEST(toString)
	{
		{
			VarString     pairlist = "key1";
			ParameterList list(pairlist);

			CHECK_EQUAL("key1;", list.toString().c_str());
		}

		{
			VarString     pairlist     = ";";
			ParameterList list(pairlist);

			CHECK_EQUAL("", list.toString().c_str());
		}

		{
			VarString     pairlist     = "=;";
			ParameterList list(pairlist);

			CHECK_EQUAL(";", list.toString().c_str());
		}

		{
			VarString     pairlist = "=; =; = ;";
			ParameterList list(pairlist);

			CHECK_EQUAL(";", list.toString().c_str());
		}
	}

	//-------------------------------------------------------------------------
	TEST(size)
	{
		VarString pairlist1 = " key-2 =46363125;"
			                  "key-1 = -6867913 ;"
			                  " key0 = -33216 ;"
							  "key1=val1;"
							  "key2  = val2   ;"
							  "key3 ;"
							  "key4;"
							  "key5= =====;"
							  ";"
							  "key6;"
							  "key7= val 7 ";
		VarString pairlist2 = "key1";
		VarString pairlist3 = ";";

		const int listsize1 = 10;
		const int listsize2 =  1;
		const int listsize3 =  0;

		ParameterList list1(pairlist1);
		ParameterList list2(pairlist2);
		ParameterList list3(pairlist3);

		CHECK(list1.begin() != list1.end());
		CHECK_EQUAL(listsize1 , static_cast<int>(list1.size()));

		CHECK(list2.begin() != list2.end());
		CHECK_EQUAL(listsize2 , static_cast<int>(list2.size()));

		CHECK(list3.begin() == list3.end());
		CHECK_EQUAL(listsize3 , static_cast<int>(list3.size()));

		ParameterList list1a(list1.toString());
		ParameterList list1b(list1);

		CHECK_EQUAL(listsize1 , static_cast<int>(list1a.size()));
		CHECK_EQUAL(listsize1 , static_cast<int>(list1b.size()));
	}

	//-------------------------------------------------------------------------
	TEST(set)
	{
		VarString pairlist1 = " key-2 =46363125;"
			                  "key-1 = -6867913 ;"
			                  " key0 = -33216 ;"
							  "key1=val1;"
							  "key2  = val2   ;"
							  "key3 ;"
							  "key4;"
							  "key5= =====;"
							  ";"
							  "key6;"
							  "key7= val 7 ";
		ParameterList list1(pairlist1);

		list1.setValue("key3","val3");
		CHECK_EQUAL("val3", list1.getValue("key3"));

		list1.setValue("key3","val3  ");
		CHECK_EQUAL("val3  ", list1.getValue("key3"));
		CHECK(!list1.isValueEmpty("key3"));
	}

	//-------------------------------------------------------------------------
	TEST(clear)
	{
		VarString pairlist1 = " key-2 =46363125;"
			                  "key-1 = -6867913 ;"
			                  " key0 = -33216 ;"
							  "key1=val1;"
							  "key2  = val2   ;"
							  "key3 ;"
							  "key4;"
							  "key5= =====;"
							  ";"
							  "key6;"
							  "key7= val 7 ";
		ParameterList list1(pairlist1);

		list1.clear();
		CHECK(list1.begin() == list1.end());
		CHECK_EQUAL(0 , static_cast<int>(list1.size()));
	}

	//-------------------------------------------------------------------------
	TEST(TestParameterList_ParameterList_relational_operator)
	{
		ParameterList l1; 
		ParameterList l2; 
		CHECK((l1 == l2)); 
		
		l1.push_back("test");
		CHECK(!(l1 == l2));
		
		l2.push_back("test");
		CHECK((l1 == l2));
		
		l1.push_back("test");
		CHECK((l1 == l2));
		
		l1.setValue("test", "a");
		CHECK(!(l1 == l2));
		
		l2.setValue("test", "a"); 
		CHECK((l1 == l2));
		
		l2.setValue("test", "aa");
		CHECK(!(l1 == l2));
		
		ParameterList l3 = l1;
		CHECK((l1 == l3));
		
		ParameterList l4;
		CHECK(!(l1 == l4));

		l4 = l1;
		CHECK((l1 == l4));
	}

	//-------------------------------------------------------------------------
	TEST(TestParameterList_toStream)
	{
		VarString pairlist = 
			" key-2 =46363125;key-1 = -6867913 ;"
			" key0 = -33216 ;key1=val1;key2  = val2   ;"
			"key3 ;key4;key5= =====;;key6;key7= val 7 ";

		ParameterList list(pairlist);

		std::ostringstream out;
		list.toStream(out);

		VarString actual = out.str();

		ConstString EXPECTED = 
			"#0     name : <key-1>\n"
			"       value: <-6867913>\n"
			"------------------------------------------------\n"
			"#1     name : <key-2>\n"
			"       value: <46363125>\n"
			"------------------------------------------------\n"
			"#2     name : <key0>\n"
			"       value: <-33216>\n"
			"------------------------------------------------\n"
			"#3     name : <key1>\n"
			"       value: <val1>\n"
			"------------------------------------------------\n"
			"#4     name : <key2>\n"
			"       value: <val2>\n"
			"------------------------------------------------\n"
			"#5     name : <key3>\n"
			"       value: <>\n"
			"------------------------------------------------\n"
			"#6     name : <key4>\n"
			"       value: <>\n"
			"------------------------------------------------\n"
			"#7     name : <key5>\n"
			"       value: <=====>\n"
			"------------------------------------------------\n"
			"#8     name : <key6>\n"
			"       value: <>\n"
			"------------------------------------------------\n"
			"#9     name : <key7>\n"
			"       value: <val 7>\n"
			"------------------------------------------------\n";

		CHECK_EQUAL(EXPECTED, actual.right('\n').c_str());
	}
}

// ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~  ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~

