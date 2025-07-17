#include <UnitTest++/UnitTest++.h>
#include "libbasardbaspect.h"

#include "libbasardbsql.h"

#include "testconnection.h"

#include "float_builder.h"

#include <iostream>
#include <limits>

using namespace basar::test::unit::dbaspect;

basar::ConstString myAccDef = "accDef";
basar::ConstString myAccInst = "accInt";
basar::ConstString myAccMeth = "accMeth";

#ifdef _WIN32
#pragma warning (disable: 4715)
#endif
//------------------------------------------------------------------------------
SUITE( DBAspect_Float )
{

    class FloatTestFix
    {
        public:
            FloatTestFix() : m_Conn( TestConnection::createDefaultConnectionRef() )
            {
                init();
            }

            ~FloatTestFix()
            {
            }

        protected:
            basar::db::aspect::ConnectionRef getConn() const
            {
                return m_Conn;
            }

            basar::db::aspect::AccessorInstanceRef getAccInst() const
            {
                return m_AccInst;
            }

            basar::db::aspect::ExecuteResultInfo execute( basar::db::aspect::AccessorPropertyTable_YIterator yit ) try
            {
                return m_AccInst.execute( myAccMeth, yit );
            }
            catch (basar::Exception& ex)
            {
                std::cerr << "caugth exception: " << ex.what();
                throw; //win32 at least does not seem to generate a implicit throw (control is all wonky w/o throw)
            }

        private:

            void init()
            {
                //setup
                {
                    basar::db::sql::ConnectionRef sqlConn = m_Conn.getSqlConnection();

                    basar::db::sql::StatementRef stmt = sqlConn.createStatement();

                    stmt.execute( "create temp table mytest(myfloat float default 0 not null, mysmallfloat smallfloat default 0 not null)" );
                }
                if( true == basar::db::aspect::Manager::getInstance().hasAccessorDefinition( myAccDef ) )
                {
                    basar::db::aspect::Manager::getInstance().releaseAccessorDefinition( myAccDef );
                }

                basar::db::aspect::AccessorDefinitionRef accDef = basar::db::aspect::Manager::getInstance().createAccessorDefinition( myAccDef );

                accDef.getPropertyDescriptionList().push_back( PROPDEF_MYFLOAT );
                accDef.getPropertyDescriptionList().push_back( PROPDEF_MYSMALLFLOAT );

                {
                    basar::db::aspect::AccessMethodRef am = accDef.createAccessMethod( myAccMeth );
                    MyFloatInsertBuilder b;
                    am.push_back( b );
                }

                m_AccInst = accDef.createInstance( m_Conn );
            }

            basar::db::aspect::ConnectionRef m_Conn;

            basar::db::aspect::AccessorInstanceRef m_AccInst;
    };

    TEST_FIXTURE( FloatTestFix, float_test )
    {
        const basar::Float64 f64 = 1234.5678916345678;
        const basar::Float32 f32 = 1234.5678f;

        basar::db::aspect::AccessorInstanceRef accInst = this->getAccInst();

        basar::db::aspect::AccessorPropertyTable_YIterator yit = accInst.getPropertyTable().insert( basar::FOR_INSERT );

        yit.setFloat64( PROPDEF_MYFLOAT.getName(), f64 );
        yit.setFloat32( PROPDEF_MYSMALLFLOAT.getName(), f32 );

        this->execute( yit );

        basar::db::sql::ConnectionRef sqlConn = this->getConn().getSqlConnection();

        basar::db::sql::StatementRef stmt = sqlConn.createStatement();

        basar::db::sql::ResultsetRef res = stmt.executeQuery("select first 1 myfloat, mysmallfloat from mytest");

        CHECK(res.next());

        CHECK_CLOSE(f64, res.getFloat64("myfloat"), std::numeric_limits<double>::epsilon() / 10.0);
        CHECK_CLOSE(f32, res.getFloat32("mysmallfloat"), std::numeric_limits<float>::epsilon() / 10.0f);
    }

    TEST_FIXTURE( FloatTestFix, float_min_test)
    {
        const basar::Float64 f64 = std::numeric_limits<double>::min();
        const basar::Float32 f32 = std::numeric_limits<float>::min();

        basar::db::aspect::AccessorInstanceRef accInst = this->getAccInst();

        basar::db::aspect::AccessorPropertyTable_YIterator yit = accInst.getPropertyTable().insert( basar::FOR_INSERT );

        yit.setFloat64( PROPDEF_MYFLOAT.getName(), f64 );
        yit.setFloat32( PROPDEF_MYSMALLFLOAT.getName(), f32 );

        this->execute( yit );

        basar::db::sql::ConnectionRef sqlConn = this->getConn().getSqlConnection();

        basar::db::sql::StatementRef stmt = sqlConn.createStatement();

        basar::db::sql::ResultsetRef res = stmt.executeQuery("select first 1 myfloat, mysmallfloat from mytest");

        CHECK(res.next());

        CHECK_CLOSE(f64, res.getFloat64("myfloat"), std::numeric_limits<double>::epsilon() / 10.0);
        CHECK_CLOSE(f32, res.getFloat32("mysmallfloat"), std::numeric_limits<float>::epsilon() / 10.0f);
    }

    //testing max values leads to infx exception: An illegal floating point number has been found in the statement
    TEST_FIXTURE( FloatTestFix, float_test_large)
    {
        const basar::Float64 f64 = 99999999999999999999999999999999999999999.0;
        const basar::Float32 f32 = 999999999999999999999999999999999.0f;

        basar::db::aspect::AccessorInstanceRef accInst = this->getAccInst();

        basar::db::aspect::AccessorPropertyTable_YIterator yit = accInst.getPropertyTable().insert( basar::FOR_INSERT );

        yit.setFloat64( PROPDEF_MYFLOAT.getName(), f64 );
        yit.setFloat32( PROPDEF_MYSMALLFLOAT.getName(), f32 );

        this->execute( yit );

        basar::db::sql::ConnectionRef sqlConn = this->getConn().getSqlConnection();

        basar::db::sql::StatementRef stmt = sqlConn.createStatement();

        basar::db::sql::ResultsetRef res = stmt.executeQuery("select first 1 myfloat, mysmallfloat from mytest");

        CHECK(res.next());

        CHECK_CLOSE(f64, res.getFloat64("myfloat"), std::numeric_limits<double>::epsilon() / 10.0);
        CHECK_CLOSE(f32, res.getFloat32("mysmallfloat"), std::numeric_limits<float>::epsilon() / 10.0f);
    }

    TEST_FIXTURE( FloatTestFix, float_test_many_decimals)
    {
        const basar::Float64 f64 = 0.56789163456789127;
        const basar::Float32 f32 = 0.567891634f;

        basar::db::aspect::AccessorInstanceRef accInst = this->getAccInst();

        basar::db::aspect::AccessorPropertyTable_YIterator yit = accInst.getPropertyTable().insert( basar::FOR_INSERT );

        yit.setFloat64( PROPDEF_MYFLOAT.getName(), f64 );
        yit.setFloat32( PROPDEF_MYSMALLFLOAT.getName(), f32 );

        this->execute( yit );

        basar::db::sql::ConnectionRef sqlConn = this->getConn().getSqlConnection();

        basar::db::sql::StatementRef stmt = sqlConn.createStatement();

        basar::db::sql::ResultsetRef res = stmt.executeQuery("select first 1 myfloat, mysmallfloat from mytest");

        CHECK(res.next());

        CHECK_CLOSE(f64, res.getFloat64("myfloat"), std::numeric_limits<double>::epsilon() / 10.0);
        CHECK_CLOSE(f32, res.getFloat32("mysmallfloat"), std::numeric_limits<float>::epsilon() / 10.0f);
    }
}