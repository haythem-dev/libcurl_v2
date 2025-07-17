//----------------------------------------------------------------------------
/*! \file
 *  \brief  build SQL - statements reference
 *  \author Roland Kiefert
 *  \date   14.07.2005
 */
//----------------------------------------------------------------------------

#include "libbasardbaspect_sqlstringbuilder.h"

//--------------------------------------------------------------------------------------------------
namespace basar {
namespace db {
namespace aspect {

//--------------------------------------------------------------------------------------------------
/*!	\throw no-throw 
	\param builder a shared pointer to implementation class */
SQLStringBuilderRef::SQLStringBuilderRef(const boost::shared_ptr<SQLStringBuilder> builder) : m_pBuilder(builder)
{
}

//--------------------------------------------------------------------------------------------------
/*!	\throw no-throw */
SQLStringBuilderRef::SQLStringBuilderRef()
{
}

//--------------------------------------------------------------------------------------------------
/*!	\throw no-throw
	\retval true if shared pointer to implementation object is null
	\return otherwise returns false */
bool SQLStringBuilderRef::isNull() const
{
	return (0 == m_pBuilder.get());
}

//--------------------------------------------------------------------------------------------------
/*!	\throw no-throw */
void SQLStringBuilderRef::reset()
{
	m_pBuilder.reset();
}

//--------------------------------------------------------------------------------------------------
/*!	\throw UnknownPropertyNameException, NullReferenceException
	\retval true if the class is executable 
	\return otherwise returns false
	\param propertyList is an iterator of a row of the Propertytable */
bool SQLStringBuilderRef::build( const AccessorPropertyTable_YIterator& propertyList )
{
	return getImpl()->build( propertyList );
}

//--------------------------------------------------------------------------------------------------
/*!	\throw NullReferenceException
	\param paramList list containing default parameters for sql-statements */
void SQLStringBuilderRef::setParameterList( const basar::cmnutil::ParameterList paramList )
{
	return getImpl()->setParameterList( paramList );
}

//--------------------------------------------------------------------------------------------------
/*!	\throw NullReferenceException
    \return ParameterList */
basar::cmnutil::ParameterList   SQLStringBuilderRef::getParameterList() const
{
	return getImpl()->getParameterList( );
}

//--------------------------------------------------------------------------------------------------
/*!	\throw NullReferenceException
	\return SQL-statement */
const VarString& SQLStringBuilderRef::getSQLString() const
{
	return getImpl()->getSQLString();
}

//--------------------------------------------------------------------------------------------------
/*!	\throw NullReferenceException
	\retval true if the statement is a query
	\return otherwise returns false */
std::pair<bool, QueryTypeEnum> SQLStringBuilderRef::isQuery() const
{
	return getImpl()->isQuery();
}

//--------------------------------------------------------------------------------------------------
/*!	\throw NullReferenceException
	\retval true if the statement is prepareable
	\return otherwise returns false */
bool SQLStringBuilderRef::isPrepareable() const
{
    return getImpl()->isPrepareable();
}

//--------------------------------------------------------------------------------------------------
/*!	\throw no-throw
	\return IndexTypeCollection */
const IndexTypeCollection& SQLStringBuilderRef::getPreparedParameterTypePositionCollection() const
{
    return getImpl()->getPreparedParameterTypePositionCollection();
}

//--------------------------------------------------------------------------------------------------
/*!	\throw NullReferenceException 
	\return pointer to SQLStringBuilder */
const boost::shared_ptr<SQLStringBuilder>& SQLStringBuilderRef::getImpl() const
{
	if (0 == m_pBuilder.get())
	{
		ExceptInfo sInfo("basar.db.aspect.SQLStringBuilderRef.getImpl", "Reference of SQLStringBuilder is no pointer to impl-class", __FILE__, __LINE__);
		throw NullReferenceException(sInfo);
	}
	return m_pBuilder;
}

//--------------------------------------------------------------------------------------------------
/*!	\throw NullReferenceException 
	\return shared pointer to SQLStringBuilder */
const boost::shared_ptr<SQLStringBuilder> SQLStringBuilderRef::create() const
{ 
	return getImpl()->create();
}

//--------------------------------------------------------------------------------------------------
}//aspect
}//db
}//basar
