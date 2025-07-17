//--------------------------------------------------------------------------------------------------------//
/*! \file
 *  \brief  all functions implementation AccessorInstanceRef
 *  \author Roland Kiefert
 *  \date   18.07.2005
 */
//--------------------------------------------------------------------------------------------------------//

//--------------------------------------------------------------------------------------------------------//
// includes
//--------------------------------------------------------------------------------------------------------//

#include "libbasardbaspect_accessor.h"

#include "libbasarproperty_propertytablesnapshotref.h"
#include "accessor.h"
#include "libbasardbaspect_accessmethod.h"
#include "libbasardbaspect_accessorpropertytable_yiterator.h"

//--------------------------------------------------------------------------------------------------------//
// namespaces
//--------------------------------------------------------------------------------------------------------//
namespace basar {
namespace db {
namespace aspect {


////////////////////////////////////////////////////////////////////////////////////////////////////////////
//!	\throw no-throw
////////////////////////////////////////////////////////////////////////////////////////////////////////////
AccessorInstanceRef::AccessorInstanceRef()
{
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////
//!	\throw no-throw
////////////////////////////////////////////////////////////////////////////////////////////////////////////
AccessorInstanceRef::~AccessorInstanceRef()
{
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////
/*!	\throw  NullReferenceException
	\return Reference to the property description list */
////////////////////////////////////////////////////////////////////////////////////////////////////////////
const property::PropertyDescriptionListRef AccessorInstanceRef::getPropertyDescriptionList() const
{
	return getImpl()->getPropertyDescriptionList();
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////
/*!	\throw  NullReferenceException, InvalidStateException
	\return const reference to a property table / cacher  */
////////////////////////////////////////////////////////////////////////////////////////////////////////////
const AccessorPropertyTableRef AccessorInstanceRef::getPropertyTable() const
{
	return getImpl()->getPropertyTable();
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////
/*!	\throw  NullReferenceException, InvalidStateException
	\return reference to a property table / cacher  */
////////////////////////////////////////////////////////////////////////////////////////////////////////////
AccessorPropertyTableRef AccessorInstanceRef::getPropertyTable()
{
	return getImpl()->getPropertyTable();
}
/*!	\throw UnknownAccessMethodException, NullReferenceException
	\return Reference to an AccessMethod
	\param rName name of an accessmethod */
const AccessMethodRef AccessorInstanceRef::getAccessMethod( const AccessMethodName& rName ) const 
{
	return getImpl()->getAccessMethod(rName); 
}

/*!	\throw  NullReferenceException
	\return Reference to the AccessMethod list */
const AccessMethodListRef AccessorInstanceRef::getAccessMethodList() const 
{
	return 	getImpl()->getAccessMethodList(); 
}

/*!	\throw  UnknownPropertyNameException, NullReferenceException
	\return ExecuteResultInfo resultstruct Errornumber / AffectedRows
	\param  rName name of an accessmethod 
	\param  yit iterator of property table 
	\param  clearBeforeSelect if true then the property will be cleared before the selection 
	\param  flagExcept in case of error: DB exception / return code
	\param  dbHint Hint for the database engine (e.g. for optimisation)
	*/
const ExecuteResultInfo AccessorInstanceRef::execute(	const AccessMethodName& rName, 
											AccessorPropertyTable_YIterator yit,
											bool clearBeforeSelect, 
											bool flagExcept, 
                                            DatabaseHintEnum dbHint )
{
	return getImpl()->execute( rName, yit, clearBeforeSelect, flagExcept, dbHint );
}

/*!	\throw  UnknownPropertyNameException, NullReferenceException
	\return ExecuteResultInfo resultstruct Errornumber / AffectedRows
	\param rName name of an accessmethod 
	\param yBeg iterator of property table begin
	\param yEnd iterator of property table end 
	\param  clearBeforeSelect if true then the property will be cleared before the first selection 
	\param  flagExcept in case of error: DB exception / return code
	\param  dbHint Hint for the database engine (e.g. for optimisation)
	*/
const ExecuteResultInfo AccessorInstanceRef::execute(	const AccessMethodName& rName, 
											AccessorPropertyTable_YIterator yBeg, 
											AccessorPropertyTable_YIterator yEnd,
											bool clearBeforeSelect, 
											bool flagExcept,
                                            DatabaseHintEnum dbHint)
{
	return getImpl()->execute( rName, yBeg, yEnd, clearBeforeSelect, flagExcept, dbHint );
}
/*!	\throw  UnknownPropertyNameException, NullReferenceException
	\return ExecuteResultInfo resultstruct Errornumber / AffectedRows
	\param  rName name of an accessmethod 
	\param  clearBeforeSelect if true then the property will be cleared before selection 
	\param  flagExcept in case of error: DB exception / return code
	\param  dbHint Hint for the database engine (e.g. for optimisation)
	*/
const ExecuteResultInfo AccessorInstanceRef::execute(	const AccessMethodName& rName,
											bool clearBeforeSelect, 
											bool flagExcept,
                                            DatabaseHintEnum dbHint)
{
	return getImpl()->execute( rName, clearBeforeSelect, flagExcept, dbHint );
}

/*!	\throw  UnknownPropertyNameException, NullReferenceException
	\return result of aggregate-select 
	\param  rName name of an accessmethod */
const std::pair<bool, Decimal> AccessorInstanceRef::executeAggregate( const AccessMethodName& rName )
{
	return getImpl()->executeAggregate( rName );
}

/*!	\throw  UnknownPropertyNameException, NullReferenceException
	\return result of aggregate-select 
	\param  rName name of an accessmethod
	\param  yit iterator of property table */
const std::pair<bool, Decimal> AccessorInstanceRef::executeAggregate( const AccessMethodName& rName, 
															AccessorPropertyTable_YIterator yit )
{
	return getImpl()->executeAggregate( rName, yit );
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////
//!	\param  paramList	list containing default values 
//!	\param  rName		name of access method for which all stringbuilders get this default params (default empty)
//!	\throw	UnknownMethodNameException
/////////////////////////////////////////////////////////////////////////////////////////////////////////
void AccessorInstanceRef::setDefaultParameter( cmnutil::ParameterList paramList, const AccessMethodName& rName )
{
	return getImpl()->setDefaultParameter( paramList, rName );
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////
//!	\param  rDefParams	string containing default values (separated by ';')
//!	\param  rName		name of access method for which all stringbuilders get this default params (default empty)
//!	\throw	UnknownMethodNameException
/////////////////////////////////////////////////////////////////////////////////////////////////////////
void AccessorInstanceRef::setDefaultParameter( VarString& rDefParams, const AccessMethodName& rName )
{
	return getImpl()->setDefaultParameter( rDefParams, rName );
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////
//!	\param  defParams	string containing default values (';'-separated)
//!	\param  rName		name of access method for which all stringbuilders get this default params (default empty)
//!	\throw	UnknownMethodNameException
/////////////////////////////////////////////////////////////////////////////////////////////////////////
void AccessorInstanceRef::setDefaultParameter( ConstString defParams, const AccessMethodName& rName )
{
	return getImpl()->setDefaultParameter( defParams, rName );
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////
/*!  returns the given ParameterList, that is set by 
 *   AcessorInstanceRef::setDefaultParameterList and is
 *   stored in the map Acessor::m_MethodDefaultParameter
 *   That is neccessary as the DefaultParamter in the suservent
 *   SQLStringBuilder may be changed seperatetly and because it is 
 *   possible to give a DefaultParameterList for all Methods. 
 *   If there is no DefaultParameterList stored in the map for a
 *   given Method, the DefaultParamterList of the first 
 *   SQLStringBuilder (begin) within this Method is returned.
 *  \param  rName		name of access method for which all stringbuilders get this default params (default empty)
 *   \return the underlying Accessor's ParameterList
 *	\throw	UnknownMethodNameException
 */
/////////////////////////////////////////////////////////////////////////////////////////////////////////
cmnutil::ParameterList AccessorInstanceRef::getDefaultParameter(const AccessMethodName& rName) const 
{
	return getImpl()->getDefaultParameter( rName );
}


/*! \throw  no-throw
	\return name of the accessor */
const VarString & AccessorInstanceRef::getAccessorName() const
{
	return getImpl()->getAccessorName();
}

/*! \throw  no-throw
	\param name name of the accessor */
void AccessorInstanceRef::setAccessorName( const VarString name )
{
	getImpl()->setAccessorName(name);
}

/*!	\throw  NullReferenceException
	\return pointer to Accessor */
const boost::shared_ptr<IAccessor>& AccessorInstanceRef::getImpl() const
{
	if (m_pAccessor.get() == 0)
	{
		ExceptInfo sInfo( "basar.db.aspect.AccessorInstanceRef.getImpl()", 
						  "Reference of Accessor in the accessor instance ref - no pointer to impl-class", 
						  __FILE__, __LINE__);
		throw NullReferenceException(sInfo);
	}
	return m_pAccessor;
}

/*!	\throw  no-throw
	\return pointer to Reference of Accessor Instance 
	\param pAccessor a shared pointer to its implementation class */
AccessorInstanceRef::AccessorInstanceRef( const boost::shared_ptr<IAccessor> pAccessor )
: m_pAccessor(pAccessor)
{
}
/*!	\throw no-throw
	\retval true  if shared pointer to implementation object is null 
	\return otherwise returns false */
bool AccessorInstanceRef::isNull() const
{
	return (0 == m_pAccessor.get());
}
///////////////////////////////////////////////////////////////////////////////////////////////////////////
/*!	\throw no-throw */
///////////////////////////////////////////////////////////////////////////////////////////////////////////
void AccessorInstanceRef::reset()
{
	m_pAccessor.reset();
}
//--------------------------------------------------------------------------------------------------------//
// transaction methods
//--------------------------------------------------------------------------------------------------------//
////////////////////////////////////////////////////////////////////////////////////////////////////////////
//!	\throw	NullReferenceException 
////////////////////////////////////////////////////////////////////////////////////////////////////////////
void AccessorInstanceRef::beginTransaction()
{
	getImpl()->beginTransaction();
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////
//!	\throw	NullReferenceException 
////////////////////////////////////////////////////////////////////////////////////////////////////////////
void AccessorInstanceRef::commitTransaction()
{
	getImpl()->commitTransaction();
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////
//!	\throw	NullReferenceException 
////////////////////////////////////////////////////////////////////////////////////////////////////////////
void AccessorInstanceRef::rollbackTransaction()
{
	getImpl()->rollbackTransaction();
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////
//! \return Reference to the created PropertyTableSnapshot
//!	\throw	NullReferenceException 
////////////////////////////////////////////////////////////////////////////////////////////////////////////
property::PropertyTableSnapshotRef AccessorInstanceRef::createSnapshot()
{
	return getImpl()->createSnapshot();
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////
//! \param  snapShot reference to snapshot-propertytable
//!	\throw	NullReferenceException 
////////////////////////////////////////////////////////////////////////////////////////////////////////////
void AccessorInstanceRef::resetToSnapshot( property::PropertyTableSnapshotRef snapShot )
{
	getImpl()->resetToSnapshot( snapShot );
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////
//!	\return	Reference to the Connection
//!	\throw	NullReferenceException
////////////////////////////////////////////////////////////////////////////////////////////////////////////
const ConnectionRef AccessorInstanceRef::getConnection() const
{
	return getImpl()->getConnection();
}
///////////////////////////////////////////////////////////////////////////////////////
//!	\retval	true if a transaction is active
//!	\return otherwise returns false 
//!	\throw	no-throw
///////////////////////////////////////////////////////////////////////////////////////
bool AccessorInstanceRef::isInTransaction() const
{
	return getImpl()->isInTransaction();
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////
//!	\param	toCreate passed argument of type bool indicating if the internal snapshot has to be created.
//!	\throw	no-throw
////////////////////////////////////////////////////////////////////////////////////////////////////////////
void AccessorInstanceRef::switchCreateInternalSnapshot( const bool toCreate )
{
	getImpl()->switchCreateInternalSnapshot( toCreate );
}

// -------------------------------------------------------------------------------------------
Int64 AccessorInstanceRef::getLastInsertedSerial(Int32 nr) 
{
    return getImpl()->getLastInsertedSerial(nr); 
} 
// -------------------------------------------------------------------------------------------
bool AccessorInstanceRef::hasLastInsertedSerial() 
{
    return getImpl()->hasLastInsertedSerial();  
} 
// -------------------------------------------------------------------------------------------
Int32 AccessorInstanceRef::getNumberOfLastInsertedSerials()
{
    return getImpl()->getNumberOfLastInsertedSerials();  
}
// -------------------------------------------------------------------------------------------
void AccessorInstanceRef::clearLastInsertedSerials() 
{ 
    getImpl()->clearLastInsertedSerials();  
} 
// -------------------------------------------------------------------------------------------

void AccessorInstanceRef::setAccessorExecuteListener(
   const AccessorExecuteListener & ref)
{
    getImpl()->setAccessorExecuteListener(ref);  
}
// -------------------------------------------------------------------------------------------
void AccessorInstanceRef::removeAccessorExecuteListener()
{
    getImpl()->removeAccessorExecuteListener();  
}
// -------------------------------------------------------------------------------------------
AccessorExecuteListenerRef AccessorInstanceRef::getAccessorExecuteListener() const
{
    return getImpl()->getAccessorExecuteListener();  
}
// -------------------------------------------------------------------------------------------

} // aspect
} // db
} // basar
