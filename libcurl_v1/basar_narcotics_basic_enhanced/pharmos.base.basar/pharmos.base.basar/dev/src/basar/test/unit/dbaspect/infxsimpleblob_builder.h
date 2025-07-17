#ifndef BASAR_TEST_UNIT_DBASPECT_INFXSIMPLEBLOB_BUILDER_H
#define BASAR_TEST_UNIT_DBASPECT_INFXSIMPLEBLOB_BUILDER_H

#include <libbasardbaspect_sqlstringbuilder.h>
#include "libbasarproperty_propertydescription.h"

CONST_PROPERTY_DESCRIPTION( PROPDEF_MYTEXT, "mytext", basar::STRING )
CONST_PROPERTY_DESCRIPTION( PROPDEF_MYBINARY, "mybinary", basar::BINARY )

class MyTextInsertBuilder : public basar::db::aspect::SQLPrepareableWriteStringBuilder
{
        // Inherited via SQLQueryStringBuilder
        virtual const boost::shared_ptr<SQLStringBuilder> create() const; /*override*/

        virtual bool isExecutable() const; /*override*/

        virtual void buildSQLString(); /*override*/
};

class MyNonPreparedTextInsertBuilder : public basar::db::aspect::SQLWriteStringBuilder
{
        // Inherited via SQLQueryStringBuilder
        virtual const boost::shared_ptr<SQLStringBuilder> create() const; /*override*/

        virtual bool isExecutable() const; /*override*/

        virtual void buildSQLString(); /*override*/
};

class MyTextSelectBuilder : public basar::db::aspect::SQLQueryStringBuilder
{
        // Inherited via SQLQueryStringBuilder
        virtual const boost::shared_ptr<SQLStringBuilder> create() const; /*override*/

        virtual bool isExecutable() const; /*override*/

        virtual void buildSQLString(); /*override*/
};

class MyBinaryInsertBuilder : public basar::db::aspect::SQLPrepareableWriteStringBuilder
{
        virtual const boost::shared_ptr<SQLStringBuilder> create() const; /*override*/

        virtual bool isExecutable() const; /*override*/

        virtual void buildSQLString(); /*override*/
};

class MyBinarySelectBuilder : public basar::db::aspect::SQLQueryStringBuilder
{
        virtual const boost::shared_ptr<SQLStringBuilder> create() const; /*override*/

        virtual bool isExecutable() const; /*override*/

        virtual void buildSQLString(); /*override*/
};

#endif

