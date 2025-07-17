#ifndef GUARD_LIBBASARDBASPECT_MACROS_H
#define GUARD_LIBBASARDBASPECT_MACROS_H
//-----------------------------------------------------------------------------------------------//
/*! \file
 *  \brief  macros for sqlstringbuilder + accessor definition
 *  \author Thomas Hörath
 *  \date   26.08.2005
 */
//-----------------------------------------------------------------------------------------------//

//-----------------------------------------------------------------------------------------------//
#include "libbasardbaspect_sqlstringbuilder.h"

//-----------------------------------------------------------------------------------------------//
// namespaces
//-----------------------------------------------------------------------------------------------//
namespace basar {
namespace db {
namespace aspect {

//-----------------------------------------------------------------------------------------------//
// declaration
//-----------------------------------------------------------------------------------------------//
//----------------------------------------------------------------------------
//! macro to start declaring a SQLQueryStringBuilder-class
#define BEGIN_QUERY_BUILDER_DECLARATION(QueryBuilderName) \
	class QueryBuilderName: public basar::db::aspect::SQLQueryStringBuilder \
	{ \
	public: \
		/*! create new SQLQueryStringBuilder \return shared_ptr to SQLStringBuilder */ \
		virtual const boost::shared_ptr<basar::db::aspect::SQLStringBuilder> create() const; \
	protected: \
		/*! check if statement is ready for execution \return bool indicating statement is executeable */ \
		virtual bool isExecutable() const; \
		/*! resolve sql-statement */ \
		virtual void buildSQLString(); \
	private: \
		static basar::ConstString s_SqlStmt;/*!< sql-string */

//----------------------------------------------------------------------------
//! macro to start declaring a prepareable SQLQueryStringBuilder-class
#define BEGIN_PREPAREABLE_QUERY_BUILDER_DECLARATION(QueryBuilderName) \
    class QueryBuilderName : public basar::db::aspect::SQLPrepareableQueryStringBuilder \
    { \
    public: \
		/*! create new SQLQueryStringBuilder \return shared_ptr to SQLStringBuilder */ \
		virtual const boost::shared_ptr<basar::db::aspect::SQLStringBuilder> create() const; \
	protected: \
		/*! check if statement is ready for execution \return bool indicating statement is executeable */ \
		virtual bool isExecutable() const; \
		/*! resolve sql-statement */ \
		virtual void buildSQLString(); \
	private: \
		static basar::ConstString s_SqlStmt;/*!< sql-string */
		
//----------------------------------------------------------------------------
//! macro to start declaring a SQLSingleQueryStringBuilder-class
#define BEGIN_SINGLE_QUERY_BUILDER_DECLARATION(QueryBuilderName) \
	class QueryBuilderName: public basar::db::aspect::SQLQueryStringBuilder \
	{ \
	public: \
		/*! create new SQLSingleQueryStringBuilder \return shared_ptr to SQLStringBuilder */ \
		virtual const boost::shared_ptr<basar::db::aspect::SQLStringBuilder> create() const; \
		/*! builder has single query \return std::pair indicating builder has single query */ \
		virtual std::pair<bool, basar::db::aspect::QueryTypeEnum> isQuery() const \
		{ return std::make_pair<bool, basar::db::aspect::QueryTypeEnum>( true, basar::db::aspect::SINGLE_QUERY ); }; \
	protected: \
		/*! check if statement is ready for execution \return bool indicating statement is executeable */ \
		virtual bool isExecutable() const; \
		/*! resolve sql-statement */ \
		virtual void buildSQLString(); \
	private: \
		static basar::ConstString s_SqlStmt;/*!< sql-string */

//----------------------------------------------------------------------------
//! macro to start declaring a SQLSingleQueryStringBuilder-class
#define BEGIN_PREPAREABLE_SINGLE_QUERY_BUILDER_DECLARATION(QueryBuilderName) \
	class QueryBuilderName: public basar::db::aspect::SQLPrepareableQueryStringBuilder \
	{ \
	public: \
		/*! create new SQLSingleQueryStringBuilder \return shared_ptr to SQLStringBuilder */ \
		virtual const boost::shared_ptr<basar::db::aspect::SQLStringBuilder> create() const; \
		/*! builder has single query \return std::pair indicating builder has single query */ \
		virtual std::pair<bool, basar::db::aspect::QueryTypeEnum> isQuery() const \
		{ return std::make_pair<bool, basar::db::aspect::QueryTypeEnum>( true, basar::db::aspect::SINGLE_QUERY ); }; \
	protected: \
		/*! check if statement is ready for execution \return bool indicating statement is executeable */ \
		virtual bool isExecutable() const; \
		/*! resolve sql-statement */ \
		virtual void buildSQLString(); \
	private: \
		static basar::ConstString s_SqlStmt;/*!< sql-string */

//----------------------------------------------------------------------------
//! macro to start declaring a SQLAggregateQueryStringBuilder-class
#define BEGIN_AGGREGATE_QUERY_BUILDER_DECLARATION(QueryBuilderName) \
	class QueryBuilderName: public basar::db::aspect::SQLQueryStringBuilder \
	{ \
	public: \
		/*! create new SQLAggregateQueryStringBuilder \return shared_ptr to SQLStringBuilder */ \
		virtual const boost::shared_ptr<basar::db::aspect::SQLStringBuilder> create() const; \
		/*! builder has aggregate query \return std::pair indicating builder has aggregate query */ \
		virtual std::pair<bool, basar::db::aspect::QueryTypeEnum> isQuery() const \
		{ return std::make_pair<bool, basar::db::aspect::QueryTypeEnum>( true, basar::db::aspect::AGGREGATE_QUERY ); }; \
	protected: \
		/*! check if statement is ready for execution \return bool indicating statement is executeable */ \
		virtual bool isExecutable() const; \
		/*! resolve sql-statement */ \
		virtual void buildSQLString(); \
	private: \
		static basar::ConstString s_SqlStmt;/*!< sql-string */

//----------------------------------------------------------------------------
//! macro to start declaring a SQLAggregateQueryStringBuilder-class
#define BEGIN_PREPAREABLE_AGGREGATE_QUERY_BUILDER_DECLARATION(QueryBuilderName) \
	class QueryBuilderName: public basar::db::aspect::SQLPrepareableQueryStringBuilder \
	{ \
	public: \
		/*! create new SQLAggregateQueryStringBuilder \return shared_ptr to SQLStringBuilder */ \
		virtual const boost::shared_ptr<basar::db::aspect::SQLStringBuilder> create() const; \
		/*! builder has aggregate query \return std::pair indicating builder has aggregate query */ \
		virtual std::pair<bool, basar::db::aspect::QueryTypeEnum> isQuery() const \
		{ return std::make_pair<bool, basar::db::aspect::QueryTypeEnum>( true, basar::db::aspect::AGGREGATE_QUERY ); }; \
	protected: \
		/*! check if statement is ready for execution \return bool indicating statement is executeable */ \
		virtual bool isExecutable() const; \
		/*! resolve sql-statement */ \
		virtual void buildSQLString(); \
	private: \
		static basar::ConstString s_SqlStmt;/*!< sql-string */

//----------------------------------------------------------------------------
//! macro to start declaring a SQLWriteStringBuilder-class
#define BEGIN_WRITE_BUILDER_DECLARATION(WriteBuilderName) \
	class WriteBuilderName: public basar::db::aspect::SQLWriteStringBuilder \
	{ \
	public: \
		/*! create new SQLWriteStringBuilder \return shared_ptr to SQLStringBuilder */ \
		virtual const boost::shared_ptr<basar::db::aspect::SQLStringBuilder> create() const; \
	protected: \
		/*! check if statement is ready for execution \return bool indicating statement is executeable */ \
		virtual bool isExecutable() const; \
		/*! resolve sql-statement */ \
		virtual void buildSQLString(); \
	private: \
		static basar::ConstString s_SqlStmt;/*!< sql-string */


//----------------------------------------------------------------------------
//! macro to start declaring a prepareable SQLWriteStringBuilder-class
#define BEGIN_PREPAREABLE_WRITE_BUILDER_DECLARATION(WriteBuilderName) \
	class WriteBuilderName: public basar::db::aspect::SQLPrepareableWriteStringBuilder \
	{ \
	public: \
		/*! create new SQLWriteStringBuilder \return shared_ptr to SQLStringBuilder */ \
		virtual const boost::shared_ptr<basar::db::aspect::SQLStringBuilder> create() const; \
		/*! builder isPrepareable \return bool indicating builder is prepareable */ \
	protected: \
		/*! check if statement is ready for execution \return bool indicating statement is executeable */ \
		virtual bool isExecutable() const; \
		/*! resolve sql-statement */ \
		virtual void buildSQLString(); \
	private: \
		static basar::ConstString s_SqlStmt;/*!< sql-string */


//----------------------------------------------------------------------------
//! macro to finish the declaration of a SQLQueryStringBuilder-class
#define END_BUILDER_DECLARATION \
	\
	};

//-----------------------------------------------------------------------------------------------//
// definition
//-----------------------------------------------------------------------------------------------//
//----------------------------------------------------------------------------
//! macro to define a SQLQueryStringBuilder-class
//! \return shared_ptr to SQLStringBuilder
#define BUILDER_DEFINITION(BuilderName) \
	const boost::shared_ptr<basar::db::aspect::SQLStringBuilder> BuilderName::create() const\
	{ \
		return boost::shared_ptr<basar::db::aspect::SQLStringBuilder>(new BuilderName); \
	}

//----------------------------------------------------------------------------
//! macro to define the isExecutable-method of a SQLStringBuilder-class by calling the arePropertiesSetAndOneMustBeInState-method
#define ENSURE_PROPERTIES_ARE_SET_AND_ONE_IN_STATE(BuilderName,PropertyNames,State) \
	bool BuilderName::isExecutable() const \
	{ \
		return arePropertiesSetAndOneMustBeInState(PropertyNames, State); \
	}

//----------------------------------------------------------------------------
//! macro to define the isExecutable-method of a SQLStringBuilder-class by calling the arePropertiesSetAndAllInState-method
#define ENSURE_PROPERTIES_ARE_SET_AND_ALL_IN_STATE(BuilderName,PropertyNames,State) \
	bool BuilderName::isExecutable() const \
	{ \
		return arePropertiesSetAndAllInState(PropertyNames, State); \
	}

//----------------------------------------------------------------------------
//! macro to define the isExecutable-method of a SQLStringBuilder-class by calling the are1stPropertiesSetAnd2ndOneInState-method
#define ENSURE_FIRST_PROPERTIES_ARE_SET_AND_SECOND_ONE_IN_STATE(BuilderName,firstPropertyNames,secondPropertyNames,State) \
bool BuilderName::isExecutable() const \
	{ \
		return are1stPropertiesSetAnd2ndOneInState(firstPropertyNames,secondPropertyNames,State); \
	}

//----------------------------------------------------------------------------
//! macro to define the isExecutable-method of a SQLStringBuilder-class by calling the arePropertiesSet-method
#define ENSURE_PROPERTIES_ARE_SET(BuilderName,PropertyNames) \
	bool BuilderName::isExecutable() const \
	{ \
		return arePropertiesSet(PropertyNames); \
	}

//----------------------------------------------------------------------------
//! macro to define the isExecutable-method of a SQLStringBuilder-class with no checking of any properties
#define DONT_CHECK_PROPERTIES(BuilderName) \
	bool BuilderName::isExecutable() const \
	{ \
		return true; \
	}
	
//----------------------------------------------------------------------------
// definition for doxygen, so that SQL_PATTERN-Makro is scanned for "SQLSTMT" 
//! \defgroup SQLSTMT SQL statements set to SqlBuilder

//----------------------------------------------------------------------------
//! macro to define the buildSQLString-method of a SQLStringBuilder-class by calling the resolve-method
#define SQL_PATTERN(BuilderName,SQLString) \
	/*! \ingroup SQLSTMT */ \
	/*! sql-statement of SQLStringBuilder, defined via macro "SQL_PATTERN". */ \
	basar::ConstString BuilderName::s_SqlStmt = SQLString; \
	\
	void BuilderName::buildSQLString() \
	{ \
		resolve(s_SqlStmt); \
	}

//----------------------------------------------------------------------------
//! macro to start registration of an accessor before main
#define BEGIN_ACCESSOR_DEFINITION(AccessorName) \
		/*! definition of accessors. \return bool to force static init */ \
		static bool define(){ \
		basar::db::aspect::AccessorDefinitionRef adr = basar::db::aspect::Manager::getInstance().createAccessorDefinition(AccessorName); \
		basar::db::aspect::AccessMethodRef am;

//----------------------------------------------------------------------------
//! macro to add type + columns to a PropertyDescriptionList, which is registered to an AccessorDefinitionRef before
#define PROPERTY_DESCRIPTION_LIST(Columns) \
	basar::property::PropertyDescriptionListRef  pdl = adr.getPropertyDescriptionList(); \
		pdl.construct(Columns);
//----------------------------------------------------------------------------

//----------------------------------------------------------------------------
//! macro to get the PropertyDescriptionList, which is registered to an AccessorDefinitionRef before
#define  BEGIN_PROPERTY_DESCRIPTION_LIST \
	basar::property::PropertyDescriptionListRef  pdl = adr.getPropertyDescriptionList(); \
    pdl.construct("");

//----------------------------------------------------------------------------

//! macro to add type + columns to a PropertyDescriptionList, which is registered to an AccessorDefinitionRef before
#define PROPERTY_DESCRIPTION_LIST_ADD(Column) \
		pdl.push_back(Column);

//----------------------------------------------------------------------------
//! macro to end the PropertyDescriptionList definition (not realy necessaray, but if you start with a begin statement, you wan't an end statement
#define  END_PROPERTY_DESCRIPTION_LIST \
	

//----------------------------------------------------------------------------
//! macro to add an access-method to an AccessorDefinitionRef
#define ACCESS_METHOD(MethodName) \
			am = adr.createAccessMethod(MethodName); 

//----------------------------------------------------------------------------
//! macro to instance a SqlStringBuilder-class 
#define SQL_BUILDER_CREATE(BuilderName) \
				BuilderName builder ## BuilderName; 

//----------------------------------------------------------------------------
//! macro to add a SqlStringBuilder-class to an AccessMethodRef
#define SQL_BUILDER_PUSH_BACK(BuilderName) \
				am.push_back(builder ## BuilderName);

//----------------------------------------------------------------------------
//! macro to add an access-method to an AccessorDefinitionRef and push back one instance of a SqlStringBuilder-class
#define ACCESS_METHOD_SINGLE_BUILDER(MethodAndBuilderName) \
			am = adr.createAccessMethod(MethodAndBuilderName); \
			MethodAndBuilderName builder ## MethodAndBuilderName; \
			am.push_back(builder ## MethodAndBuilderName);

//----------------------------------------------------------------------------
//! macro to finish registration of an accessor
#define END_ACCESSOR_DEFINITION \
			return true; \
		} \
	static bool dummy = define(); /*!< just to start accessor definition without explicit call */

//----------------------------------------------------------------------------
//! macro to declare an accessor execute listener
#define BEGIN_ACCESSOREXECUTELISTENER_DECLARATION(MyAccessorExecuteListener) \
    class MyAccessorExecuteListener : public basar::db::aspect::AccessorExecuteListener \
     {       \
        public: \
        virtual  basar::db::aspect::AccessorExecuteListenerRef create() const \
        { \
           return  basar::db::aspect::AccessorExecuteListenerRef( \
                   boost::shared_ptr<basar::db::aspect::AccessorExecuteListener>(new MyAccessorExecuteListener(*this))); \
        } \
        virtual void executePerformed(const basar::db::aspect::AccessorExecuteEvent &); \
    
         
//----------------------------------------------------------------------------
//! macro to mark the end of an accessor execute listener declaration
#define END_ACCESSOREXECUTELISTENER_DECLARATION \
	\
	};

//----------------------------------------------------------------------------
//! macro to define an accessor execute listener function
#define ACCESSOREXECUTELISTENER_FUNCTION(MyAccessorExecuteListener) \
    void MyAccessorExecuteListener::executePerformed(const basar::db::aspect::AccessorExecuteEvent & event) \

//----------------------------------------------------------------------------
} // aspect
} // db
} // basar


#endif

