/*
* @file                                            
* @author Anke Klink                      
* @date 2008                            
*/    


#include <UnitTest++/UnitTest++.h>
#include "libbasardbaspect.h"
#include "libbasarproperty_propertytable_xiterator.h"
#include "libbasarproperty_propertydescriptionlistref.h"

#include "buttablepropertydescriptions.h"

#include "testconnection.h"
#include "testnames.h"

using basar::Int32;
using basar::VarString;
using basar::ConstString;
using basar::cmnutil::ParameterList;
using basar::property::PropertyTable_YIterator;
using basar::db::aspect::AccessorPropertyTable_YIterator;
using basar::db::aspect::AccessorPropertyTable_XIterator;
using basar::db::aspect::AccessorInstanceRef;
using basar::db::aspect::AccessorPropertyTableRef;
using basar::db::aspect::ConnectionRef;
using basar::db::aspect::Manager;
using basar::db::aspect::ExecuteResultInfo;

using basar::test::unit::dbaspect::TestConnection;

#define TESTDDBACPECT_TESTYITERATOR
#ifdef TESTDDBACPECT_TESTYITERATOR

//------------------------------------------------------------------------------
SUITE(DbAspectYIterator)
{
//------------------------------------------------------------------------------

TEST(AccessorPropertyTable_YIterator_empty)
{
    AccessorPropertyTable_YIterator iter1;
    CHECK(iter1.isNull());

    AccessorPropertyTable_XIterator iterX;
    CHECK(iterX.isNull());
}

//------------------------------------------------------------------------------
TEST(AccessorPropertyTable_YIterator_select)
{
    ConnectionRef connRef = TestConnection::createDefaultConnectionRef();

    try
    {
        AccessorInstanceRef ai1 = Manager::getInstance().createAccessorInstance(
            "test5_AccessorInstanceRef",
            ACCESSORNAME_ButMasterTableReader_Int, 
            connRef,
            basar::db::aspect::FULL_CACHING, 
            true);

        ExecuteResultInfo executeresultinfo = ai1.execute(ACCESSMETHOD_ButMasterTableReader_Int);
        CHECK_EQUAL(basar::db::aspect::SUCCESS, executeresultinfo.getError());
        CHECK_EQUAL(0, (int)executeresultinfo.getAffectedRows());

        AccessorPropertyTableRef  aptr1 = ai1.getPropertyTable();

        try
        {
            Int32 i = 0;
            Int32 max = static_cast<Int32>(aptr1.size());
            // max is 10, but less then 2 will return errors in the tests below
            CHECK(max > 2);
            AccessorPropertyTable_YIterator iter1 = aptr1.begin();
            AccessorPropertyTable_YIterator iter2 = aptr1.begin();
            AccessorPropertyTable_YIterator iter3 = aptr1.begin();
            // ++ , -- 2 Rows:
            {
                CHECK_EQUAL(i,iter1.getRowNumber());
                iter1++;
                i++;
                CHECK_EQUAL(i,iter1.getRowNumber());  
                ++iter1;
                ++i;
                CHECK_EQUAL(i,iter1.getRowNumber());  
                iter1--;
                i--;
                CHECK_EQUAL(i,iter1.getRowNumber());  
                --iter1;
                --i;
                CHECK_EQUAL(i,iter1.getRowNumber());  
                CHECK(iter1 == iter2);
                CHECK_THROW(iter2--, basar::OutOfRangeIteratorException);
                CHECK_THROW(--iter2, basar::OutOfRangeIteratorException);
                CHECK_THROW(iter1--, basar::OutOfRangeIteratorException);
                CHECK_THROW(--iter1, basar::OutOfRangeIteratorException);
                iter2 = aptr1.end();
                CHECK_THROW(iter2++, basar::OutOfRangeIteratorException);
                CHECK_THROW(++iter2, basar::OutOfRangeIteratorException);
            }

            // ++ , -- all rows
            {
                iter3 = iter1;
                for (int j=0; !iter1.isEnd();++j)
                {
                    CHECK_EQUAL(j,iter3.getRowNumber());
                    ++iter3;
                    CHECK_EQUAL(j,iter1.getRowNumber());
                    iter2 = iter1++;
                    CHECK_EQUAL(j,iter2.getRowNumber());
                }
                iter3=iter2;
                for (int j=max-1; !iter2.isBegin();--j)
                {
                    CHECK_EQUAL(j,iter3.getRowNumber());
                    --iter3;
                    CHECK_EQUAL(j,iter2.getRowNumber());
                    iter1 = iter2--;
                    CHECK_EQUAL(j,iter1.getRowNumber());
                }
                CHECK_EQUAL(0,iter2.getRowNumber());
            }
        }
        catch(basar::Exception & ex)
        {
            connRef.close();
            std::cout << ex.what() << std::endl;
            const bool InnerExceptionOccured = false;
            CHECK(InnerExceptionOccured);
        }
        aptr1.clear();
    }
    catch (basar::Exception & ex)
    {
        std::cout << ex.what() << std::endl;
        const bool ExceptionOccured = false;
        CHECK(ExceptionOccured);
    }
    connRef.close();
}

//------------------------------------------------------------------------------
TEST(AccessorPropertyTable_YIterator_InsertType_Reset_after_Execute)
{
    ConnectionRef connRef = TestConnection::createDefaultConnectionRef();

    try
    {
        AccessorInstanceRef ai1 = Manager::getInstance().createAccessorInstance(
            "test6_AccessorInstanceRef",
            ACCESSORNAME_TestInsertUpdate_InsertOne, 
            connRef,
            basar::db::aspect::FULL_CACHING, 
            false);

        AccessorPropertyTableRef propTab = ai1.getPropertyTable();

        AccessorPropertyTable_YIterator yit = propTab.insert( basar::FOR_INSERT);

        yit.setInt32( PROPDEF_COLINT.getName(), 42 );

        ExecuteResultInfo res = ai1.execute( ACCESSMETHOD_TestInsertUpdate_InsertOne, yit );

        CHECK_EQUAL( false, res.hasError() );

        CHECK_EQUAL( basar::FOR_CLEAN, yit.getInsertType() );

    }
    catch (basar::Exception & ex)
    {
        std::cout << ex.what() << std::endl;
        const bool ExceptionOccured = false;
        CHECK(ExceptionOccured);
    }
    connRef.close();
}

//------------------------------------------------------------------------------
	#define DEF_COLINT	"colint"
	#define DEF_COLSTR  "colstr"

	ConstString ACCDEF  = "ToStreamAccDefName" ;
	ConstString ACCINST = "ToStreamAccInstName";

	namespace ns_ToStream
	{
		BEGIN_ACCESSOR_DEFINITION(ACCDEF)
			PROPERTY_DESCRIPTION_LIST("int32  " DEF_COLINT ";"
									  "string " DEF_COLSTR ";")
		END_ACCESSOR_DEFINITION
	}

//------------------------------------------------------------------------------
TEST(ToStream)
{
	using std::ostringstream;
	using basar::FOR_CLEAN;

	try
	{
		{
			AccessorPropertyTable_YIterator yit;

			CHECK(yit.isNull());
			CHECK_THROW(yit.toStream(), basar::NullReferenceException);
		}

		{
			AccessorPropertyTable_YIterator yit;

			AccessorInstanceRef	accInst = Manager::getInstance().createAccessorInstance(ACCINST,
					 																	ACCDEF,
																						ConnectionRef());
			AccessorPropertyTableRef tbl = accInst.getPropertyTable();

			{
				ostringstream out;

				CHECK(tbl.empty());

				yit = tbl.begin();
				CHECK(tbl.end() == yit);

				yit.toStream(out);
				CHECK_EQUAL("", out.str().c_str());
			}

			{
				ConstString EXPECT = 
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

				tbl.insert(FOR_CLEAN);
				yit = tbl.begin();
				CHECK(tbl.end() != yit);

				ostringstream out;
				yit.toStream(out);
				VarString cmp = out.str();
				// without 1st line, because it contains dynamic memory address
				CHECK_EQUAL(exp.c_str(), cmp.right('\n').c_str());
			}

			{
				ConstString EXPECT = 
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

				yit.setInt32 (DEF_COLINT, 3    );
				yit.setString(DEF_COLSTR, "abc");

				CHECK(tbl.end() != yit);

				ostringstream out;
				yit.toStream(out);
				VarString cmp = out.str();
				// without 1st line, because it contains dynamic memory address
				CHECK_EQUAL(exp.c_str(), cmp.right('\n').c_str());
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

//------------------------------------------------------------------------------

#define DEF_CMP_COLINT	"colint"
#define DEF_CMP_COLSTR  "colstr"
#define DEF_CMP_COLSTR2 "colstr2"

ConstString CMP_ACCDEF1  = "cmpAccDefName1" ;
ConstString CMP_ACCDEF2  = "cmpAccDefName2";
ConstString CMP_ACCDEF3  = "cmpAccDefName3";

ConstString CMP_ACCINST1 = "cmpAccInstName1";
ConstString CMP_ACCINST2 = "cmpAccInstName2";
ConstString CMP_ACCINST3 = "cmpAccInstName3";

namespace ns_Cmp
{
	BEGIN_ACCESSOR_DEFINITION(CMP_ACCDEF1)
		PROPERTY_DESCRIPTION_LIST("int32  " DEF_CMP_COLINT ";"
								  "string " DEF_CMP_COLSTR ";")
	END_ACCESSOR_DEFINITION
}

namespace ns_Cmp2
{
	BEGIN_ACCESSOR_DEFINITION(CMP_ACCDEF2)
		PROPERTY_DESCRIPTION_LIST("int32  " DEF_CMP_COLINT  ";"
								  "string " DEF_CMP_COLSTR2 ";"
								  "string " DEF_CMP_COLSTR  ";")
	END_ACCESSOR_DEFINITION
}

namespace ns_Cmp3
{
	BEGIN_ACCESSOR_DEFINITION(CMP_ACCDEF3)
		PROPERTY_DESCRIPTION_LIST("int16  " DEF_CMP_COLINT  ";")
	END_ACCESSOR_DEFINITION
}

//------------------------------------------------------------------------------
class FixtureCmp
{
public:
	FixtureCmp() : accInst1   (Manager::getInstance().createAccessorInstance(CMP_ACCINST1,
					 														 CMP_ACCDEF1,
																			 ConnectionRef())),
				   accInst2   (Manager::getInstance().createAccessorInstance(CMP_ACCINST2,
					 														 CMP_ACCDEF2,
																			 ConnectionRef())),				    
				   accInst3   (Manager::getInstance().createAccessorInstance(CMP_ACCINST3,
					 														 CMP_ACCDEF3,
																			 ConnectionRef())),				    
				   tbl1       (accInst1.getPropertyTable()),
				   tbl2       (accInst2.getPropertyTable()),
				   tbl3       (accInst3.getPropertyTable())
	{

		CHECK(tbl1.empty());
		CHECK(tbl2.empty());
		CHECK(tbl3.empty());

		yit1Row   = tbl1.insert(basar::FOR_CLEAN);
		yit1Empty = tbl1.insert(basar::FOR_CLEAN);
		yit1End   = tbl1.end();

		yit2Row   = tbl2.insert(basar::FOR_CLEAN);
		yit2Empty = tbl2.insert(basar::FOR_CLEAN);
		yit2End   = tbl2.end();

		yit3Row   = tbl3.insert(basar::FOR_CLEAN);

		CHECK(!tbl1.empty());
		CHECK(!tbl2.empty());
		CHECK(!tbl3.empty());

		CHECK(yit1Row   == tbl1.begin());
		CHECK(yit1Row   != yit1Empty);
		CHECK(yit1Row   != yit1End);
		CHECK(yit1Empty != yit1End);
		CHECK_EQUAL(basar::SS_UNSET, yit1Empty.getState().getStates());

		CHECK(yit2Row   == tbl2.begin());
		CHECK(yit2Row   != yit2Empty);
		CHECK(yit2Row   != yit2End);
		CHECK(yit2Empty != yit2End);
		CHECK_EQUAL(basar::SS_UNSET, yit2Empty.getState().getStates());

		CHECK(yit3Row   == tbl3.begin());
		CHECK(yit3Row   != tbl3.end  ());
	};

	~FixtureCmp()
	{
	};

	AccessorPropertyTable_YIterator yit1Row;
	AccessorPropertyTable_YIterator yit1Empty;
	AccessorPropertyTable_YIterator yit1End;

	AccessorPropertyTable_YIterator yit2Row;
	AccessorPropertyTable_YIterator yit2Empty;
	AccessorPropertyTable_YIterator yit2End;

	AccessorPropertyTable_YIterator yit3Row;

	AccessorInstanceRef	     accInst1;
	AccessorInstanceRef	     accInst2;
	AccessorInstanceRef	     accInst3;

	AccessorPropertyTableRef tbl1;
	AccessorPropertyTableRef tbl2;
	AccessorPropertyTableRef tbl3;
};

//------------------------------------------------------------------------------
TEST_FIXTURE(FixtureCmp, compareSetValues_Null)
{
	AccessorPropertyTable_YIterator yit;
	AccessorPropertyTable_YIterator yitCmp;

	CHECK(yit   .isNull());
	CHECK(yitCmp.isNull());

	CHECK_THROW(yit.compareSetValues(yitCmp), basar::InvalidIteratorException);
}

//------------------------------------------------------------------------------
TEST_FIXTURE(FixtureCmp, compareSet_EndIterator)
{
	CHECK_THROW(yit1End.compareSetValues(yit1End).first, 
		        basar::InvalidIteratorException);
}

//------------------------------------------------------------------------------
TEST_FIXTURE(FixtureCmp, compareSetValues_EmptyIterator)
{
	CHECK_THROW(yit1Empty.compareSetValues(yit1End).first,
		        basar::InvalidIteratorException);

	CHECK(yit1Empty.compareSetValues(yit1Empty).first );
	CHECK(yit1Empty.compareSetValues(yit2Empty).first );
	CHECK(yit2Empty.compareSetValues(yit1Empty).first );
}

//------------------------------------------------------------------------------
TEST_FIXTURE(FixtureCmp, compareSetValues_ValueIterator_SpecialCase)
{
	yit1Row.setInt32(DEF_CMP_COLINT, 999);

	CHECK_THROW(yit1Row.compareSetValues(yit1End).first,
		        basar::InvalidIteratorException);

	CHECK( ! yit1Row  .compareSetValues(yit1Empty).first );
	CHECK( ! yit1Empty.compareSetValues(yit1Row  ).first );
	CHECK( ! yit1Row  .compareSetValues(yit2Empty).first );
	CHECK( ! yit2Empty.compareSetValues(yit1Row  ).first );
}

//------------------------------------------------------------------------------
TEST_FIXTURE(FixtureCmp, compareSetValues_ValueIterator_Equal_1Attr)
{
	yit1Row.setInt32(DEF_CMP_COLINT, 999);
	yit2Row.setInt32(DEF_CMP_COLINT, 999);

	CHECK_EQUAL(basar::SS_CLEAN, yit1Row.getState().getStates());
	CHECK_EQUAL(basar::SS_CLEAN, yit2Row.getState().getStates());

	CHECK(   yit1Row  .compareSetValues(yit1Row).first );
	CHECK(   yit1Row  .compareSetValues(yit2Row).first );
	CHECK(   yit2Row  .compareSetValues(yit1Row).first );
}

//------------------------------------------------------------------------------
TEST_FIXTURE(FixtureCmp, compareSetValues_ValueIterator_Equal_2Attr)
{
	yit1Row.setInt32 (DEF_CMP_COLINT,  999 );
	yit1Row.setString(DEF_CMP_COLSTR, "abc");

	yit2Row.setInt32 (DEF_CMP_COLINT,  999 );
	yit2Row.setString(DEF_CMP_COLSTR, "abc");

	CHECK(yit1Row.compareSetValues(yit2Row).first );
	CHECK(yit2Row.compareSetValues(yit1Row).first );
}

//------------------------------------------------------------------------------
TEST_FIXTURE(FixtureCmp, compareSetValues_ValueIterator_NotEqualValue)
{
	yit1Row.setInt32(DEF_CMP_COLINT, 999);
	yit2Row.setInt32(DEF_CMP_COLINT, 998);

	CHECK( ! yit1Row.compareSetValues(yit2Row).first );
	CHECK( ! yit2Row.compareSetValues(yit1Row).first );

	std::ostringstream out;
	yit1Row.compareSetValues(yit2Row).second.toStream(out);

	VarString actual = out.str();
	CHECK_EQUAL("#0     name : <colint>\n"
                "       value: <values not equal!>\n"
				"------------------------------------------------\n",
		        actual.right('\n').c_str());
}

//------------------------------------------------------------------------------
TEST_FIXTURE(FixtureCmp, compareSetValues_ValueIterator_NotEqual_3Attr)
{
	yit1Row.setInt32 (DEF_CMP_COLINT,  999 );
	yit1Row.setString(DEF_CMP_COLSTR, "abc");

	yit2Row.setInt32 (DEF_CMP_COLINT ,  999 );
	yit2Row.setString(DEF_CMP_COLSTR , "abc");
	yit2Row.setString(DEF_CMP_COLSTR2, "xyz");

	CHECK( ! yit1Row.compareSetValues(yit2Row).first );
	CHECK(   yit2Row.compareSetValues(yit1Row).first );

	std::ostringstream out;
	yit1Row.compareSetValues(yit2Row).second.toStream(out);

	VarString actual = out.str();
	CHECK_EQUAL("#0     name : <colstr2>\n"
                "       value: <not contained and set for lhs YIterator!>\n"
				"------------------------------------------------\n",
		        actual.right('\n').c_str());
}

//------------------------------------------------------------------------------
TEST_FIXTURE(FixtureCmp, compareSetValues_ValueIterator_NotEqualValue_NotContainedAndSet)
{
	yit1Row.setInt32 (DEF_CMP_COLINT,  999 );
	yit1Row.setString(DEF_CMP_COLSTR, "abc");

	yit2Row.setInt32(DEF_CMP_COLINT , 999);

	CHECK( ! yit1Row  .compareSetValues(yit2Row).first );
	CHECK( ! yit2Row  .compareSetValues(yit1Row).first );

	std::ostringstream out;
	yit1Row.compareSetValues(yit2Row).second.toStream(out);

	VarString actual = out.str();
	CHECK_EQUAL("#0     name : <colstr>\n"
                "       value: <not contained and set for rhs YIterator!>\n"
				"------------------------------------------------\n",
		        actual.right('\n').c_str());
}

//------------------------------------------------------------------------------
TEST_FIXTURE(FixtureCmp, compareSetValues_ValueIterator_NotEqualType)
{
	yit1Row.setInt32(DEF_CMP_COLINT, 999);
	yit3Row.setInt16(DEF_CMP_COLINT, 999);

	CHECK( ! yit1Row.compareSetValues(yit3Row).first );
	CHECK( ! yit3Row.compareSetValues(yit1Row).first );

	std::ostringstream out;
	yit1Row.compareSetValues(yit3Row).second.toStream(out);

	VarString actual = out.str();
	CHECK_EQUAL("#0     name : <colint>\n"
                "       value: <types not equal!>\n"
				"------------------------------------------------\n",
		        actual.right('\n').c_str());
}

//------------------------------------------------------------------------------
TEST_FIXTURE(FixtureCmp, compareSetValues_AttrList_Equal)
{
	yit1Row.setInt32(DEF_CMP_COLINT, 999);
	yit2Row.setInt32(DEF_CMP_COLINT, 998);

	ParameterList attrs;
	attrs.push_back("rubbish"); 

	CHECK( yit1Row.compareSetValues(yit2Row, attrs).first );
	CHECK( yit2Row.compareSetValues(yit1Row, attrs).first );
}

//------------------------------------------------------------------------------
TEST_FIXTURE(FixtureCmp, compareSetValues_AttrList_Equal_ExcludedAttribute)
{
	yit1Row.setInt32 (DEF_CMP_COLINT,  999 );
	yit1Row.setString(DEF_CMP_COLSTR, "abc");

	yit2Row.setInt32 (DEF_CMP_COLINT ,  999 );
	yit2Row.setString(DEF_CMP_COLSTR , "abc");
	yit2Row.setString(DEF_CMP_COLSTR2, "xyz");

	ParameterList attrs;
	attrs.push_back(DEF_CMP_COLINT); 
	attrs.push_back(DEF_CMP_COLSTR); 

	CHECK( yit1Row.compareSetValues(yit2Row, attrs).first );
	CHECK( yit2Row.compareSetValues(yit1Row, attrs).first );
}

//------------------------------------------------------------------------------
TEST_FIXTURE(FixtureCmp, compareSetValues_AttrList_NotEqual)
{
	yit1Row.setInt32(DEF_CMP_COLINT, 999);
	yit2Row.setInt32(DEF_CMP_COLINT, 998);

	ParameterList attrs;
	attrs.push_back(DEF_CMP_COLINT); 

	CHECK( ! yit1Row.compareSetValues(yit2Row, attrs).first );
	CHECK( ! yit2Row.compareSetValues(yit1Row, attrs).first );
}

//------------------------------------------------------------------------------
TEST_FIXTURE(FixtureCmp, compareSetValues_AttrList_NotEqual_NotContained)
{
	yit1Row.setInt32 (DEF_CMP_COLINT,  999 );
	yit1Row.setString(DEF_CMP_COLSTR, "abc");

	yit2Row.setInt32 (DEF_CMP_COLINT ,  999 );
	yit2Row.setString(DEF_CMP_COLSTR , "abc");
	yit2Row.setString(DEF_CMP_COLSTR2, "xyz");

	ParameterList attrs;
	attrs.push_back(DEF_CMP_COLSTR2); 

	CHECK( ! yit1Row.compareSetValues(yit2Row, attrs).first );
	CHECK(   yit2Row.compareSetValues(yit1Row, attrs).first );
}

//------------------------------------------------------------------------------
TEST_FIXTURE(FixtureCmp, compareSetValues_AttrList_NotEqual_NotContained_CompleteAttrList)
{
	yit1Row.setInt32 (DEF_CMP_COLINT,  999 );
	yit1Row.setString(DEF_CMP_COLSTR, "abc");

	yit2Row.setInt32 (DEF_CMP_COLINT ,  999 );
	yit2Row.setString(DEF_CMP_COLSTR , "abc");
	yit2Row.setString(DEF_CMP_COLSTR2, "xyz");

	ParameterList attrs;
	attrs.push_back(DEF_CMP_COLINT ); 
	attrs.push_back(DEF_CMP_COLSTR ); 
	attrs.push_back(DEF_CMP_COLSTR2); 

	CHECK( ! yit1Row.compareSetValues(yit2Row, attrs).first );
	CHECK(   yit2Row.compareSetValues(yit1Row, attrs).first );

	std::ostringstream out;
	yit1Row.compareSetValues(yit2Row).second.toStream(out);

	VarString actual = out.str();
	CHECK_EQUAL("#0     name : <colstr2>\n"
                "       value: <not contained and set for lhs YIterator!>\n"
				"------------------------------------------------\n",
		        actual.right('\n').c_str());
}

// ----------------------------------------------------------
}	// SUITE

// ----------------------------------------------------------
#endif
// ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~  ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~
