/** $Id$                                          
*                                                  
* @file                                   
* @author Anke Klink                      
* @date 2008                            
*/    

#include <UnitTest++/UnitTest++.h>

#include "libbasarproperty.h"
#include "propertydescriptionlist.h"
#include "propertytable.h"

using basar::VarString;
using basar::property::PropertyDescriptionListRef;
using basar::property::PropertyDescriptionList;
using basar::property::PropertyDescription;
using basar::property::PropertyType;

//------------------------------------------------------------------------------
SUITE(property_Test_PropertyDescriptionList)
{
    //// ----------------------------------------------------------

    TEST (property_Test_PropertyDescriptionList_PropertyDescriptionList)
    {
        try
        {
            PropertyDescriptionListRef propertydescriptionlistref( PropertyDescriptionList::create() );

            // push_back , construct and size:
            {
                propertydescriptionlistref.push_back( "testcol0", basar::INT32  );
                propertydescriptionlistref.push_back(PropertyDescription( "testcol1", basar::INT32 ) );
                propertydescriptionlistref.push_back(PropertyDescription( "testcol2", basar::STRING ) );
                propertydescriptionlistref.push_back(PropertyDescription( "testcol3", basar::TIMESPAN ) );

                CHECK_THROW(propertydescriptionlistref.push_back( "testcol2", basar::STRING ), basar::Exception);

                basar::property::NumberColumns numbercolumns = 4;
                CHECK_EQUAL(static_cast<unsigned int>(numbercolumns), static_cast<unsigned int>( propertydescriptionlistref.size()));

                propertydescriptionlistref.construct("int32 col_int32; int16   col_int16 ; string col_string ; timespan col_timespan ");
                numbercolumns += 4;
                CHECK_EQUAL(static_cast<unsigned int>(numbercolumns), static_cast<unsigned int>( propertydescriptionlistref.size()));

                propertydescriptionlistref.construct(";;;date col_date; datetime   col_datetime; time col_time ;");
                numbercolumns += 3;
                CHECK_EQUAL(static_cast<unsigned int>(numbercolumns), static_cast<unsigned int>( propertydescriptionlistref.size()));

                propertydescriptionlistref.construct("float32 col_float32; float64 col_float64; decimal col_decimal ");
                numbercolumns += 3;
                CHECK_EQUAL(static_cast<unsigned int>(numbercolumns), static_cast<unsigned int>( propertydescriptionlistref.size()));

                propertydescriptionlistref.construct(";");
                numbercolumns += 0;
                CHECK_EQUAL(static_cast<unsigned int>(numbercolumns), static_cast<unsigned int>( propertydescriptionlistref.size()));

                propertydescriptionlistref.construct(" ");
                numbercolumns += 0;
                CHECK_EQUAL(static_cast<unsigned int>(numbercolumns), static_cast<unsigned int>( propertydescriptionlistref.size()));

                CHECK_THROW(propertydescriptionlistref.push_back( "col_float32", basar::STRING ), basar::Exception);
                CHECK_THROW(propertydescriptionlistref.push_back( "col_timespan", basar::INT16 ), basar::Exception);
            }

            // contains:
            {
                CHECK(propertydescriptionlistref.contains("testcol0"));
                CHECK(propertydescriptionlistref.contains("testcol1"));
                CHECK(propertydescriptionlistref.contains("testcol2"));

                CHECK(propertydescriptionlistref.contains("col_int32"));
                CHECK(!propertydescriptionlistref.contains("nonsence"));

                CHECK(propertydescriptionlistref.contains("col_int32",PropertyType(basar::INT32)));
                CHECK(propertydescriptionlistref.contains("col_int16",PropertyType(basar::INT16)));
                CHECK(propertydescriptionlistref.contains("col_string",PropertyType(basar::STRING)));
                CHECK(propertydescriptionlistref.contains("col_date",PropertyType(basar::DATE)));
                CHECK(propertydescriptionlistref.contains("col_datetime",PropertyType(basar::DATETIME)));
                CHECK(propertydescriptionlistref.contains("col_time",PropertyType(basar::TIME)));
                CHECK(propertydescriptionlistref.contains("col_float32",PropertyType(basar::FLOAT32)));
                CHECK(propertydescriptionlistref.contains("col_float64",PropertyType(basar::FLOAT64)));
                CHECK(propertydescriptionlistref.contains("col_decimal",PropertyType(basar::DECIMAL)));

                CHECK(!propertydescriptionlistref.contains("col_int32",PropertyType(basar::INT16)));
                CHECK(!propertydescriptionlistref.contains("col_int16",PropertyType(basar::INT32)));
                CHECK(!propertydescriptionlistref.contains("col_string",PropertyType(basar::INT32)));
                CHECK(!propertydescriptionlistref.contains("col_date",PropertyType(basar::INT32)));
                CHECK(!propertydescriptionlistref.contains("col_datetime",PropertyType(basar::INT32)));
                CHECK(!propertydescriptionlistref.contains("col_time",PropertyType(basar::INT32)));
                CHECK(!propertydescriptionlistref.contains("col_float32",PropertyType(basar::INT32)));
                CHECK(!propertydescriptionlistref.contains("col_float64",PropertyType(basar::INT32)));
                CHECK(!propertydescriptionlistref.contains("col_decimal",PropertyType(basar::INT32)));
            }

            // getIndex , getName
            {
                CHECK_EQUAL(0,static_cast<  int>( propertydescriptionlistref.getIndex("testcol0")));
                CHECK_EQUAL(1,static_cast< int>( propertydescriptionlistref.getIndex("testcol1")));
                CHECK_EQUAL(2,static_cast< int>( propertydescriptionlistref.getIndex("testcol2")));
                CHECK_THROW(propertydescriptionlistref.getIndex( "nonsence col"), basar::Exception);

                CHECK_EQUAL("testcol0",  propertydescriptionlistref.getName(0));
                CHECK_EQUAL("testcol1", propertydescriptionlistref.getName(1));
                CHECK_EQUAL("testcol2",  propertydescriptionlistref.getName(2));
                CHECK_THROW(propertydescriptionlistref.getName(static_cast<unsigned int>( -1)), basar::Exception);
            }

            // getType
            {
                CHECK_EQUAL(basar::INT32, propertydescriptionlistref.getType("col_int32").getType());
                CHECK_EQUAL(basar::INT16, propertydescriptionlistref.getType("col_int16").getType());
                CHECK_EQUAL(basar::STRING, propertydescriptionlistref.getType("col_string").getType());
                CHECK_EQUAL(basar::DATE, propertydescriptionlistref.getType("col_date").getType());
                CHECK_EQUAL(basar::DATETIME, propertydescriptionlistref.getType("col_datetime").getType());
                CHECK_EQUAL(basar::TIME, propertydescriptionlistref.getType("col_time").getType());
                CHECK_EQUAL(basar::FLOAT64, propertydescriptionlistref.getType("col_float64").getType());
                CHECK_EQUAL(basar::FLOAT32, propertydescriptionlistref.getType("col_float32").getType());
                CHECK_EQUAL(basar::DECIMAL, propertydescriptionlistref.getType("col_decimal").getType());

                CHECK_EQUAL(basar::INT32, propertydescriptionlistref.getType(propertydescriptionlistref.getIndex("col_int32")).getType());
                CHECK_EQUAL(basar::INT16, propertydescriptionlistref.getType(propertydescriptionlistref.getIndex("col_int16")).getType());
                CHECK_EQUAL(basar::STRING, propertydescriptionlistref.getType(propertydescriptionlistref.getIndex("col_string")).getType());
                CHECK_EQUAL(basar::DATE, propertydescriptionlistref.getType(propertydescriptionlistref.getIndex("col_date")).getType());
                CHECK_EQUAL(basar::DATETIME, propertydescriptionlistref.getType(propertydescriptionlistref.getIndex("col_datetime")).getType());
                CHECK_EQUAL(basar::TIME, propertydescriptionlistref.getType(propertydescriptionlistref.getIndex("col_time")).getType());
                CHECK_EQUAL(basar::FLOAT64, propertydescriptionlistref.getType(propertydescriptionlistref.getIndex("col_float64")).getType());
                CHECK_EQUAL(basar::FLOAT32, propertydescriptionlistref.getType(propertydescriptionlistref.getIndex("col_float32")).getType());
                CHECK_EQUAL(basar::DECIMAL, propertydescriptionlistref.getType(propertydescriptionlistref.getIndex("col_decimal")).getType());

            }

            // toStream:
            {
				VarString retval = 
					"#0     name: <testcol0>\n"
					"       type: <int32>\n"
					"------------------------------------------------\n"
					"#1     name: <testcol1>\n"
					"       type: <int32>\n"
					"------------------------------------------------\n"
					"#2     name: <testcol2>\n"
					"       type: <string>\n"
					"------------------------------------------------\n"
					"#3     name: <testcol3>\n"
					"       type: <timespan>\n"
					"------------------------------------------------\n"
					"#4     name: <col_int32>\n"
					"       type: <int32>\n"
					"------------------------------------------------\n"
					"#5     name: <col_int16>\n"
					"       type: <int16>\n"
					"------------------------------------------------\n"
					"#6     name: <col_string>\n"
					"       type: <string>\n"
					"------------------------------------------------\n"
					"#7     name: <col_timespan>\n"
					"       type: <timespan>\n"
					"------------------------------------------------\n"
					"#8     name: <col_date>\n"
					"       type: <date>\n"
					"------------------------------------------------\n"
					"#9     name: <col_datetime>\n"
					"       type: <datetime>\n"
					"------------------------------------------------\n"
					"#10    name: <col_time>\n"
					"       type: <time>\n"
					"------------------------------------------------\n"
					"#11    name: <col_float32>\n"
					"       type: <float32>\n"
					"------------------------------------------------\n"
					"#12    name: <col_float64>\n"
					"       type: <float64>\n"
					"------------------------------------------------\n"
					"#13    name: <col_decimal>\n"
					"       type: <decimal>\n"
					"------------------------------------------------\n";


                std::ostringstream str;
                propertydescriptionlistref.toStream(str);

				VarString actual = str.str();
				// without 1st dynamic line
                CHECK_EQUAL((retval.c_str()), (actual.right('\n').c_str()));
            }

            // toString
            {
                basar::VarString result1 = 
                    "int32 testcol0;int32 testcol1;string testcol2;timespan testcol3;int32 col_int32;int16 col_int16;"
                    "string col_string;timespan col_timespan;date col_date;datetime col_datetime;time col_time;"
                    "float32 col_float32;float64 col_float64;decimal col_decimal;";
                basar::VarString stringresult = propertydescriptionlistref.toString();
                CHECK_EQUAL((result1.c_str()), (stringresult.c_str()));

                // copy per string
                PropertyDescriptionListRef propertydescriptionlistref2( 
                    PropertyDescriptionList::create() );

                propertydescriptionlistref2.construct(stringresult);
                CHECK_EQUAL(static_cast<int>(propertydescriptionlistref.size()), static_cast<int>(propertydescriptionlistref2.size()));
                for (int i=0;i<static_cast<int>(propertydescriptionlistref2.size());++i)
                {
                    CHECK_EQUAL(propertydescriptionlistref.getType(i).getType(), propertydescriptionlistref2.getType(i).getType());
                    CHECK_EQUAL(propertydescriptionlistref.getName(i),propertydescriptionlistref2.getName(i));
                }
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
    }

    // ----------------------------------------------------------

}

// ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~  ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~
