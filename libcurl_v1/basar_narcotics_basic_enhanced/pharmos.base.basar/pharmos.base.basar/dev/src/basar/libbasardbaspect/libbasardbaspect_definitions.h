#ifndef GUARD_LIBBASARDBASPECT_DEFINITIONS_H
#define GUARD_LIBBASARDBASPECT_DEFINITIONS_H
//-------------------------------------------------------------------------------------------------------//
/*! \file
 *  \brief  public library interface: definitions für component basar::db::aspect
 *  \author Björn Bischof
 *  \date   10.11.2005
 */
//-------------------------------------------------------------------------------------------------------//

//-------------------------------------------------------------------------------------------------------//
#include "libbasarproperty_definitions.h"
#include "libbasardbsql_definitions.h"

//-------------------------------------------------------------------------------------------------------//
// forward declaration section
//-------------------------------------------------------------------------------------------------------//
namespace basar
{

	namespace property 
	{
		class PropertyDescriptionListRef;
		class PropertyStateSet;
		class PropertyTableSnapshotRef;
		class PropertyState;
		class PropertyType;
		class PropertyTable_XIterator;
	}

    namespace db
    {
        namespace aspect
        {
            class IAccessor;
            class Accessor;
            class AccessorInstanceRef;
            class AccessorDefinitionRef;
            class AccessMethodRef;
            class SQLStringBuilderRef;
            class CacheController;
            struct StringbuilderTypeIndexes; 
        } //end namespace aspect
    } //end namespace db
} //end namepspace basar

//-------------------------------------------------------------------------------------------------------//
// namespaces
//-------------------------------------------------------------------------------------------------------//

namespace basar
{
namespace db {
namespace aspect {

//-------------------------------------------------------------------------------------------------------//
// typedef section
//-------------------------------------------------------------------------------------------------------//

typedef VarString										AccessorName;		//!< name of accessor
typedef VarString										AccessMethodName;	//!< access method name as string

typedef property::ColumnName							ColumnName;			//!< name of a column as string
typedef property::SQLString								SQLString;			//!< string type of a column
typedef VarString										PropertyName;		//!< name of a property

typedef boost::weak_ptr <IAccessor>						IAccessorWeakPtr;				//!< "weak" reference to object (itself) that's already managed by a shared_ptr (IAccessor interface implementation)
typedef boost::shared_ptr <IAccessor>					IAccessorSharedPtr;				//!< Boost shared pointer to Accessor implementation
typedef boost::weak_ptr <Accessor>						AccessorWeakPtr;				//!< "weak" reference to object (itself) that's already managed by a shared_ptr
typedef boost::shared_ptr <Accessor>					AccessorSharedPtr;				//!< Boost shared pointer to Accessor
typedef boost::shared_ptr <CacheController>				CacheControllerSharedPtr;		//!< collection of accessor-names + instances
typedef std::map <AccessMethodName, AccessMethodRef>	CollAccessorMethodRef;			//!< collection of mapping between accessmethods (represented by name) and their refs
typedef std::map <AccessorName, AccessorInstanceRef>	CollAccessorInstanceRef;		//!< collection of accessor-names + instances
typedef CollAccessorInstanceRef::iterator				CollAccessorInstanceRefIt;		//!< iterator for collection of accessor-names + instances
typedef std::map <AccessorName, AccessorDefinitionRef>	CollAccessorDefinitionRef;		//!< collection of accessor-names + definitions
typedef CollAccessorDefinitionRef::iterator				CollAccessorDefinitionRefIt;	//!< iterator for collection of accessor-names + definitions

typedef std::list <SQLStringBuilderRef>					CollSQLStringBuilderRef;		//!< collection of sql-stringbuilders

typedef CollAccessorInstanceRef::size_type				AccessorListSize;				//!< size of accessor collection
typedef CollAccessorDefinitionRef::size_type			AccessMethodListSize;			//!< size of access method list
typedef CollSQLStringBuilderRef::size_type				SQLStringBuilderListSize;		//!< size of sql-stringbuilder collection
typedef SQLStringBuilderListSize						SQLStringBuilderIndex;			//!< index of sql-stringbuilder collection

typedef property::NumberRows							NumberRows;						//!< number of rows
typedef property::ColumnIndex							ColumnIndex;					//!< index of column
typedef property::MaxSizeRows							MaxSizeRows;					//!< maximum number of rows

typedef property::PropertyDescriptionListRef		    PropertyDescriptionListRef;		//!< from basar::property
typedef	property::PropertyStateSet					    PropertyStateSet;				//!< from basar::property
typedef property::PropertyTableSnapshotRef			    PropertyTableSnapshotRef;		//!< from basar::property
typedef property::PropertyState						    PropertyState;					//!< from basar::property
typedef property::PropertyType						    PropertyType;					//!< from basar::property
typedef property::PropertyTable_XIterator			    AccessorPropertyTable_XIterator;//!< from basar::property

typedef std::vector<StringbuilderTypeIndexes>           IndexTypeCollection;            //!< collection of indices and types of parameters found in sql string for prepared statement
//-------------------------------------------------------------------------------------------------------//
// enum section
//-------------------------------------------------------------------------------------------------------//
/*! \brief	The caching policy determines how the propertytable is filled with 
	 resultset hits of the database. There are four approaches:
			1. ON_DEMAND_CACHING	: 
			2. FULL_CACHING			:
			3. SINGLE_ROW_CACHING	:
			4. NO_CACHING			: */
enum CachingPolicyEnum
{
	ON_DEMAND_CACHING,	//!< cached when asked for
	FULL_CACHING,		//!< completly cached at start up
	SINGLE_ROW_CACHING, //!< only one single row in cache (previous deleted)
	NO_CACHING			//!< nothing cached
};

/*! \brief	Different states for cursors on infx. */
enum DatabaseHintEnum
{
	NO_HINT						= 0x0000,	//!< do NOT hold cursor open over transactions
	HOLD_RECORDSET				= 0x0001	//!< HOLD cursor open 
};

/*! \brief	type of a query */
enum QueryTypeEnum
{
	SINGLE_QUERY,		//!< query which delivers only one row
	MULTI_QUERY,		//!< query which delivers more than one row
	AGGREGATE_QUERY		//!< query which delivers one decimal value (count*, sum, avg, min, max)
};

/*! \brief helper enum for determine with DefaultParameter */
enum DefaultParameterEnum
{
	FIRSTBUILDER,
	INALLBUILDER
};

/*! \brief	return type of error in the method execute */
enum ExecuteErrorEnum
{
    SUCCESS               =  0,	//!< no error, execution succeeded
	SQL_ERROR		      = -1,	//!< an undefined error occurred while sql statement was executed in database (old name)
    SQL_ERROR_UNKNOWN     = -1,	//!< an undefined error occurred while sql statement was executed in database
    NOT_EXECUTABLE	      = -2,	//!< no sql statment was executed
    SQL_ERROR_NONSPECIFIC = -3,	//!< non-specific error, "rest/remainder" of errors, at present all not INSERT_DUPL errors
    SQL_ERROR_DUPL_INSERT = -4	//!< insert statement returned "duplicate insert" error 
   };

//! \brief result of AccessorInstanceRef::execute(), holds error value and number of affected rows
struct ExecuteResultInfo
{
	typedef BULong RowNumber;	//!< type for row counting

    //! default Constructor
    ExecuteResultInfo() : m_error       (SUCCESS), 
		                  m_affectedRows(0)
    {
    }

    //! \brief  checks error
	//! \retval true  AccessorInstanceRef::execute() failed
	//! \retval false AccessorInstanceRef::execute() succeeded
    bool hasError() const
    {
        return (SUCCESS != m_error);
    }

	//! \brief  get error value
	//! \return error value of AccessorInstanceRef::execute()
	ExecuteErrorEnum getError() const
	{
		return m_error;
	}

	//! \brief get number of affected rows
	//! \return number of affected rows
	RowNumber getAffectedRows() const
	{
		return m_affectedRows;
	}

	ExecuteErrorEnum m_error;         //!< error value
    RowNumber        m_affectedRows;  //!< number of affected rows
    sql::DBError     m_DbError;       //!< raw error info from sql layer
};

//--------------------------------------------------------------------------------------------//
}	// namespace aspect
}	// namespace db
}	// namespace basar

//-------------------------------------------------------------------------------------------------------//
#endif	// GUARD_LIBBASARDBASPECT_DEFINITIONS_H
