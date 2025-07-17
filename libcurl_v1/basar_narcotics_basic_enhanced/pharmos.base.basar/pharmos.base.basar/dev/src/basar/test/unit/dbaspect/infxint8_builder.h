#ifndef BASAR_TEST_UNIT_DBASPECT_INFXINT8_BUILDER_H
#define BASAR_TEST_UNIT_DBASPECT_INFXINT8_BUILDER_H

#include <libbasardbaspect_sqlstringbuilder.h>
#include "libbasarproperty_propertydescription.h"

CONST_PROPERTY_DESCRIPTION( PROPDEF_MYINT8, "myint8", basar::INT64 )

class MyInt8InsertBuilder : public basar::db::aspect::SQLWriteStringBuilder
{
        // Inherited via SQLQueryStringBuilder
        virtual const boost::shared_ptr<SQLStringBuilder> create() const; /*override*/

        virtual bool isExecutable() const; /*override*/

        virtual void buildSQLString(); /*override*/
};

class MyInt8SelectBuilder : public basar::db::aspect::SQLQueryStringBuilder
{
        // Inherited via SQLQueryStringBuilder
        virtual const boost::shared_ptr<SQLStringBuilder> create() const; /*override*/

        virtual bool isExecutable() const; /*override*/

        virtual void buildSQLString(); /*override*/
};

class MyInt8PreparedInsertBuilder : public basar::db::aspect::SQLPrepareableWriteStringBuilder
{
        // Inherited via SQLQueryStringBuilder
        virtual const boost::shared_ptr<SQLStringBuilder> create() const; /*override*/

        virtual bool isExecutable() const; /*override*/

        virtual void buildSQLString(); /*override*/
};

class MyInt8PreparedSelectBuilder : public basar::db::aspect::SQLPrepareableQueryStringBuilder
{
        // Inherited via SQLQueryStringBuilder
        virtual const boost::shared_ptr<SQLStringBuilder> create() const; /*override*/

        virtual bool isExecutable() const; /*override*/

        virtual void buildSQLString(); /*override*/
};

class MyBigIntInsertBuilder : public basar::db::aspect::SQLWriteStringBuilder
{
        // Inherited via SQLQueryStringBuilder
        virtual const boost::shared_ptr<SQLStringBuilder> create() const; /*override*/

        virtual bool isExecutable() const; /*override*/

        virtual void buildSQLString(); /*override*/
};

class MyBigIntSelectBuilder : public basar::db::aspect::SQLQueryStringBuilder
{
        // Inherited via SQLQueryStringBuilder
        virtual const boost::shared_ptr<SQLStringBuilder> create() const; /*override*/

        virtual bool isExecutable() const; /*override*/

        virtual void buildSQLString(); /*override*/
};

class MyBigIntPreparedInsertBuilder : public basar::db::aspect::SQLPrepareableWriteStringBuilder
{
        // Inherited via SQLQueryStringBuilder
        virtual const boost::shared_ptr<SQLStringBuilder> create() const; /*override*/

        virtual bool isExecutable() const; /*override*/

        virtual void buildSQLString(); /*override*/
};

class MyBigIntPreparedSelectBuilder : public basar::db::aspect::SQLPrepareableQueryStringBuilder
{
        // Inherited via SQLQueryStringBuilder
        virtual const boost::shared_ptr<SQLStringBuilder> create() const; /*override*/

        virtual bool isExecutable() const; /*override*/

        virtual void buildSQLString(); /*override*/
};

#endif
