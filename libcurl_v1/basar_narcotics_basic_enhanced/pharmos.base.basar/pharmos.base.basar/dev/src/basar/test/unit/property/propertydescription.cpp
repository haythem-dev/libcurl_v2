/** $Id$                                          
*                                                  
* @file                                                                   
* @author Anke Klink                      
* @date 2008                            
*/    

#include <UnitTest++/UnitTest++.h>

#include "libbasarproperty.h"
#include "libbasarproperty_definitions.h"

#include "propertydescriptionlist.h"
#include "propertytable.h"

using  basar::property::PropertyDescriptionListRef;
using  basar::property::PropertyDescriptionList;
using  basar::property::PropertyDescription;
using  basar::property::PropertyType;
using  basar::property::PropertyDescriptionVector;



//------------------------------------------------------------------------------
SUITE(property_Test_PropertyDescription)
{
    //// ----------------------------------------------------------

    TEST (property_Test_PropertyDescription_PropertyDescription)
    {
        PropertyDescription test1("testcol1", basar::INT32);
        PropertyDescription test2("testcol2", basar::STRING);
        PropertyDescription test3("testcol3", "string");
        PropertyDescription test4("testcol4");
        PropertyDescription test5("testcol5", "unknown");
        PropertyDescription test6("testcol6", "int32");
        PropertyDescription test7("testcol7", "timespan");
        PropertyDescription test8("testcol8", "int64");
        PropertyDescription test9("testcol9", "binary");

        CHECK_THROW(PropertyDescription("testcol5", "unknown or not defined, or ..."), basar::UnknownPropertyTypeException);

        CHECK_EQUAL("testcol1",test1.getName());
        CHECK_EQUAL("testcol2",test2.getName());
        CHECK_EQUAL("testcol3",test3.getName());
        CHECK_EQUAL("testcol4",test4.getName());
        CHECK_EQUAL("testcol7",test7.getName());
        CHECK_EQUAL("testcol8", test8.getName());
        CHECK_EQUAL("testcol9", test9.getName());

        CHECK_EQUAL("testcol1",basar::VarString(test1));
        CHECK_EQUAL("testcol2",basar::VarString(test2));

        CHECK_EQUAL(basar::INT32,test1.getType().getType());
        CHECK_EQUAL(basar::STRING,test2.getType().getType());
        CHECK_EQUAL(basar::STRING,test3.getType().getType());
        CHECK_EQUAL(basar::UNKNOWN,test4.getType().getType());
        CHECK_EQUAL(basar::TIMESPAN, test7.getType().getType());
        CHECK_EQUAL(basar::INT64, test8.getType().getType());
        CHECK_EQUAL(basar::BINARY, test9.getType().getType());

        CHECK_EQUAL("int32 testcol1;",test1.toDescriptionString());
        CHECK_EQUAL("string testcol2;",test2.toDescriptionString());
        CHECK_EQUAL("string testcol3;",test3.toDescriptionString());
        CHECK_EQUAL("unknown testcol4;",test4.toDescriptionString());
        CHECK_EQUAL("timespan testcol7;", test7.toDescriptionString());
        CHECK_EQUAL("int64 testcol8;", test8.toDescriptionString());
        CHECK_EQUAL("binary testcol9;", test9.toDescriptionString());

		CHECK_EQUAL("tab.testcol1", test1.toStringWithTableName("tab")); 
		CHECK_EQUAL("tab.testcol2", test2.toStringWithTableName("tab"));
		CHECK_EQUAL("tab.testcol3", test3.toStringWithTableName("tab")); 
		CHECK_EQUAL("tab.testcol4", test4.toStringWithTableName("tab")); 

		CHECK_EQUAL("tab.testcol1 as testcol1", test1.toStringWithTableName("tab", true)); 
		CHECK_EQUAL("tab.testcol2 as testcol2", test2.toStringWithTableName("tab", true)); 
		CHECK_EQUAL("tab.testcol3 as testcol3", test3.toStringWithTableName("tab", true)); 
		CHECK_EQUAL("tab.testcol4 as testcol4", test4.toStringWithTableName("tab", true)); 

		CHECK_EQUAL("tab.testcol1", test1.toStringWithTableName("tab", false)); 
		CHECK_EQUAL("tab.testcol2", test2.toStringWithTableName("tab", false));
		CHECK_EQUAL("tab.testcol3", test3.toStringWithTableName("tab", false));
		CHECK_EQUAL("tab.testcol4", test4.toStringWithTableName("tab", false));

		CHECK_EQUAL("testcol1", test1.toName("", "")); 
		CHECK_EQUAL("tab.testcol1", test1.toName("tab.", "")); 
		CHECK_EQUAL("tab.testcol1 as testcol1", test1.toName("tab.", " as testcol1")); 
			
		CHECK_EQUAL("'#testcol1#'", test1.toSQLReplacementString("'", "'"));  
		CHECK_EQUAL("testcol1 = '#testcol1#'", test1.toEqualString("'", "'"));  
		CHECK_EQUAL("testcol1 = #testcol1#", test1.toEqualString("", ""));  
		CHECK_EQUAL("tab1.testcol1 = '#testcol1#'", test1.toEqualString("tab1.", "", "'","'"));  

		CHECK_EQUAL("testcol1 < '#testcol1#'", test1.toInEqualString(" < ", "'", "'"));  
		CHECK_EQUAL("testcol1 < #testcol1#", test1.toInEqualString(" < ", "", ""));  
		CHECK_EQUAL("tab1.testcol1 < '#testcol1#'", test1.toInEqualString(" < ","tab1.", "", "'","'"));  

		CHECK(test1==test1);
		CHECK(!(test2==test1)); 


		PropertyDescription a = test1;
		CHECK(a==test1);
		const PropertyDescription a_const = test1;
		CHECK(a_const==test1);

		 

    }

    TEST (property_Test_PropertyDescription_CONST_PROPERTY_DESCRIPTION)
    {

        CONST_PROPERTY_DESCRIPTION(test1,"testcol1", basar::INT32);
        CONST_PROPERTY_DESCRIPTION(test2,"testcol2", basar::STRING);
        CONST_PROPERTY_DESCRIPTION(test3,"testcol3", " string");
        CONST_PROPERTY_DESCRIPTION(test4,"testcol4", "unknown ");
        CONST_PROPERTY_DESCRIPTION(test5,"testcol5", "unknown");
        CONST_PROPERTY_DESCRIPTION(test6,"testcol6", "int32 ");

        CHECK_THROW(CONST_PROPERTY_DESCRIPTION(test7,"testcol7", "unknown or not defined, or ..."), basar::UnknownPropertyTypeException);

        CHECK_EQUAL("testcol1",test1.getName());
        CHECK_EQUAL("testcol2",test2.getName());
        CHECK_EQUAL("testcol3",test3.getName()); 
        CHECK_EQUAL("testcol4",test4.getName());

        CHECK_EQUAL("testcol2",basar::VarString(test2));

        const basar::VarString varTest1    (test1);
        basar::VarString       varTest2    (test1);
		basar::ConstString     constTest1 = test1.getName().c_str();
		basar::ConstString     constTest2  (test1.getName().c_str());
        CHECK_EQUAL("testcol1",varTest1);
        CHECK_EQUAL("testcol1",varTest2);
        CHECK_EQUAL("testcol1",constTest1);
        CHECK_EQUAL("testcol1",constTest2);

        CHECK_EQUAL(basar::INT32,test1.getType().getType());
        CHECK_EQUAL(basar::STRING,test2.getType().getType());
        CHECK_EQUAL(basar::STRING,test3.getType().getType());
        CHECK_EQUAL(basar::UNKNOWN,test4.getType().getType());

        CHECK_EQUAL("int32 testcol1;",test1.toDescriptionString());
        CHECK_EQUAL("string testcol2;",test2.toDescriptionString());
        CHECK_EQUAL("string testcol3;",test3.toDescriptionString());
        CHECK_EQUAL("unknown testcol4;",test4.toDescriptionString());

		CHECK_EQUAL("tab.testcol1", test1.toStringWithTableName("tab")); 
		CHECK_EQUAL("tab.testcol2", test2.toStringWithTableName("tab"));
		CHECK_EQUAL("tab.testcol3", test3.toStringWithTableName("tab")); 
		CHECK_EQUAL("tab.testcol4", test4.toStringWithTableName("tab")); 

		CHECK_EQUAL("tab.testcol1 as testcol1", test1.toStringWithTableName("tab", true)); 
		CHECK_EQUAL("tab.testcol2 as testcol2", test2.toStringWithTableName("tab", true)); 
		CHECK_EQUAL("tab.testcol3 as testcol3", test3.toStringWithTableName("tab", true)); 
		CHECK_EQUAL("tab.testcol4 as testcol4", test4.toStringWithTableName("tab", true)); 

		CHECK_EQUAL("tab.testcol1", test1.toStringWithTableName("tab", false)); 
		CHECK_EQUAL("tab.testcol2", test2.toStringWithTableName("tab", false));
		CHECK_EQUAL("tab.testcol3", test3.toStringWithTableName("tab", false));
		CHECK_EQUAL("tab.testcol4", test4.toStringWithTableName("tab", false));


		CHECK_EQUAL("tab.testcol1 as testcol1", test1.toAsStatment("tab", test1)); 
		CHECK_EQUAL("tab.testcol2 as testcol2", test2.toAsStatment("tab", test2)); 
		CHECK_EQUAL("tab.testcol3 as testcol3", test3.toAsStatment("tab", test3)); 
		CHECK_EQUAL("tab.testcol4 as testcol4", test4.toAsStatment("tab", test4)); 

		CHECK_EQUAL("tab.testcol1 as testcol4", test1.toAsStatment("tab", test4)); 
		CHECK_EQUAL("tab.testcol2 as testcol3", test2.toAsStatment("tab", test3)); 
		CHECK_EQUAL("tab.testcol3 as testcol2", test3.toAsStatment("tab", test2)); 
		CHECK_EQUAL("tab.testcol4 as testcol1", test4.toAsStatment("tab", test1)); 

	 
		CHECK_EQUAL("testcol1 as testcol1", test1.toAsStatment( test1)); 
		CHECK_EQUAL("testcol2 as testcol2", test2.toAsStatment( test2)); 
		CHECK_EQUAL("testcol3 as testcol3", test3.toAsStatment( test3)); 
		CHECK_EQUAL("testcol4 as testcol4", test4.toAsStatment( test4)); 

		CHECK_EQUAL("testcol1 as testcol4", test1.toAsStatment( test4)); 
		CHECK_EQUAL("testcol2 as testcol3", test2.toAsStatment( test3)); 
		CHECK_EQUAL("testcol3 as testcol2", test3.toAsStatment( test2)); 
		CHECK_EQUAL("testcol4 as testcol1", test4.toAsStatment( test1)); 
 
		CHECK_EQUAL(" NVL(testcol1,hallo) ", test1.toNVLString( "hallo")); 
		CHECK_EQUAL(" NVL(testcol2,hallo) ", test2.toNVLString( "hallo")); 
		CHECK_EQUAL(" NVL(testcol3,hallo) ", test3.toNVLString( "hallo")); 
		CHECK_EQUAL(" NVL(testcol4,hallo) ", test4.toNVLString( "hallo"));  
		CHECK_EQUAL(" NVL(tab.testcol1,hallo) ", test1.toNVLString( "hallo", "tab.")); 
		CHECK_EQUAL(" NVL(tab.testcol1x,hallo) ", test1.toNVLString( "hallo", "tab.", "x")); 

		CHECK_EQUAL(" NVL(testcol1,hallo)  as testcol1 ", test1.toNVLStringAs("hallo")); 
	    CHECK_EQUAL(" NVL(tab.testcol1,hallo)  as testcol1 ", test1.toNVLStringAs( "hallo", "tab.")); 
		CHECK_EQUAL(" NVL(tab.testcol1x,hallo)  as testcol1 ", test1.toNVLStringAs( "hallo", "tab.", "x")); 

		CHECK_EQUAL(" NVL(testcol1,hallo)  as testcol2 ", test1.toNVLStringAs(test2, "hallo")); 
		CHECK_EQUAL(" NVL(tab.testcol1,hallo)  as testcol2 ", test1.toNVLStringAs(test2, "hallo",  "tab.")); 
		CHECK_EQUAL(" NVL(tab.testcol1x,hallo)  as testcol2 ", test1.toNVLStringAs( test2, "hallo", "tab.", "x")); 

		CHECK(test1==test1);
		CHECK(!(test2==test1));  

		PropertyDescription a = test1;
		CHECK(a==test1);
        CONST_PROPERTY_DESCRIPTION(test1a,"testcol1", basar::INT32);
        CONST_PROPERTY_DESCRIPTION(test1b,"testcol1", basar::INT16);
        CONST_PROPERTY_DESCRIPTION(test1c,"testcol1c", basar::INT32);
		CHECK((test1a==test1)); 
		CHECK(!(test1b==test1)); 
		CHECK(!(test1c==test1)); 

    }

    TEST (property_Test_PropertyDescription_PropertyDescriptionVector)
    {
        CONST_PROPERTY_DESCRIPTION(test1,"testcol1", basar::INT32);
        CONST_PROPERTY_DESCRIPTION(test2,"testcol2", basar::STRING);
        CONST_PROPERTY_DESCRIPTION(test3,"testcol3", " string");
        CONST_PROPERTY_DESCRIPTION(test4,"testcol4", "unknown ");
        CONST_PROPERTY_DESCRIPTION(test5,"testcol5", "unknown");
        CONST_PROPERTY_DESCRIPTION(test6,"testcol6", "int32 ");

        const PropertyDescription a[] = {test1, test2, test3, test4, test5, test6} ;
        const PropertyDescriptionVector vec1(6, a );

        basar::ConstString propList = "testcol1;testcol2;testcol3;testcol4;testcol5;testcol6;";
        basar::ConstString descList = "int32 testcol1;string testcol2;string testcol3;unknown testcol4;unknown testcol5;int32 testcol6;";
        basar::ConstString commaList = "testcol1,testcol2,testcol3,testcol4,testcol5,testcol6";
        basar::ConstString tableList = "tab.testcol1,tab.testcol2,tab.testcol3,tab.testcol4,tab.testcol5,tab.testcol6";
        basar::ConstString tableListAs = "tab.testcol1 as testcol1,tab.testcol2 as testcol2,tab.testcol3 as testcol3,tab.testcol4 as testcol4,tab.testcol5 as testcol5,tab.testcol6 as testcol6";

        CHECK_EQUAL(propList,vec1.toPropertyString());
        CHECK_EQUAL(descList,vec1.toDescriptionString());
        CHECK_EQUAL(commaList,vec1.toCommaSeparatedNames());
        CHECK_EQUAL(tableList,vec1.toCommaSeparatedNames("tab"));
        CHECK_EQUAL(tableListAs,vec1.toCommaSeparatedNames("tab", true));
        CHECK_EQUAL(tableList,vec1.toCommaSeparatedNames("tab", false));

        std::vector<basar::VarString> stringvec = vec1.toStringList();
        int i=0;
        std::vector<basar::VarString>::const_iterator iter;
        for (iter = stringvec.begin(); iter != stringvec.end(); ++iter)
        {
          CHECK_EQUAL(a[i].getName(),*iter);
          ++i;
        }

    }
}
// ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~  ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~
