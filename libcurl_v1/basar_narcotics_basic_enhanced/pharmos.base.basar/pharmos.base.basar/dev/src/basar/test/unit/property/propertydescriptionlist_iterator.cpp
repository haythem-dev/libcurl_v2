#include <UnitTest++/UnitTest++.h>

#include "libbasarproperty.h"
#include "propertydescriptionlist.h"
#include "propertytable.h"

using basar::VarString;
using basar::property::PropertyDescriptionListRef;
using basar::property::PropertyDescriptionList;
using basar::property::PropertyDescription;
using basar::property::PropertyDescriptionList_Iterator;

//------------------------------------------------------------------------------
SUITE(property_Test_PropertyDescriptionListIterator)
{
    TEST(property_Test_PropertyDescription_PropertyDescription_Iterator)
    {
        PropertyDescriptionListRef pdl( PropertyDescriptionList::create() );

        PropertyDescription test1("testcol1", basar::INT32);
        PropertyDescription test2("testcol2", basar::STRING);
        PropertyDescription test3("testcol3", "string");
        PropertyDescription test4("testcol4");
        PropertyDescription test5("testcol5", "unknown");
        PropertyDescription test6("testcol6", "int32");
        PropertyDescription test7("testcol7", "timespan");
        PropertyDescription test8("testcol8", "int64");
        PropertyDescription test9("testcol9", "binary");

        pdl.push_back(test1);
        pdl.push_back(test2);
        pdl.push_back(test3);
        pdl.push_back(test4);
        pdl.push_back(test5);
        pdl.push_back(test6);
        pdl.push_back(test7);
        pdl.push_back(test8);
        pdl.push_back(test9);

        {
            PropertyDescriptionList_Iterator it = pdl.begin();

            CHECK_EQUAL(test1.getName(), it->first);
            CHECK_EQUAL(test1.getType().getType(), it->second.getType());
            ++it;

            CHECK_EQUAL(test2.getName(), it->first);
            CHECK_EQUAL(test2.getType().getType(), it->second.getType());
            ++it;

            CHECK_EQUAL(test3.getName(), it->first);
            CHECK_EQUAL(test3.getType().getType(), it->second.getType());
            ++it;

            CHECK_EQUAL(test4.getName(), it->first);
            CHECK_EQUAL(test4.getType().getType(), it->second.getType());
            ++it;

            CHECK_EQUAL(test5.getName(), it->first);
            CHECK_EQUAL(test5.getType().getType(), it->second.getType());
            ++it;

            CHECK_EQUAL(test6.getName(), it->first);
            CHECK_EQUAL(test6.getType().getType(), it->second.getType());
            ++it;

            CHECK_EQUAL(test7.getName(), it->first);
            CHECK_EQUAL(test7.getType().getType(), it->second.getType());
            ++it;

            CHECK_EQUAL(test8.getName(), it->first);
            CHECK_EQUAL(test8.getType().getType(), it->second.getType());
            ++it;

            CHECK_EQUAL(test9.getName(), it->first);
            CHECK_EQUAL(test9.getType().getType(), it->second.getType());
            ++it;

            CHECK(it == pdl.end());

        }

        //check (*it). vs ->
        for(PropertyDescriptionList_Iterator it = pdl.begin(); it != pdl.end(); ++it)
        {
            CHECK_EQUAL((*it).first, it->first);
            CHECK_EQUAL((*it).second.getType(), it->second.getType());
        }
    }
}