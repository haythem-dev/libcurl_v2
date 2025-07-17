//----------------------------------------------------------------------------
/*! \file
 *  \brief  build SQL - statements; inherited
 *  \author Roland Kiefert
 *  \date   14.07.2005
 */
//----------------------------------------------------------------------------

#include "libbasardbaspect_sqlstringbuilder.h"
#include "libbasardbaspect_stringbuildertypeindexes.h"
#include "sqlstringbuilderimpl.h"
#include "sqlpreparedstringbuilderimpl.h"

/*--------------------------------------------------------------------------------------------------*/
namespace basar {
namespace db {
namespace aspect {

/*--------------------------------------------------------------------------------------------------*/
/*!	\throw no-throw  */
SQLStringBuilder::SQLStringBuilder(StringbuilderType type /* = STRINGBUILDER*/) 
{
    if (PREPARED_STRINGBUILDER == type)
    {
        m_pBuilder = boost::shared_ptr<ISQLStringBuilderImpl>(new SQLPreparedStringBuilderImpl());
    }
    else
    {
        m_pBuilder = boost::shared_ptr<ISQLStringBuilderImpl>(new SQLStringBuilderImpl());
    }
}
	 
/*--------------------------------------------------------------------------------------------------*/
/*!	\throw no-throw  */
SQLStringBuilder::~SQLStringBuilder() 
{
}

/*--------------------------------------------------------------------------------------------------*/
/*!	\throw UnknownPropertyNameException 
	\retval true if the statement is executable 
	\return otherwise returns false
	\param propertyList is an iterator to a row of the Propertytable */
bool SQLStringBuilder::build(const AccessorPropertyTable_YIterator& propertyList) 
{
	m_pBuilder->clear();
	m_pBuilder->setPropertyList( propertyList );
	bool b = isExecutable();
	
	if (b) 
	{
		buildSQLString();
	}

	return b;
}

/*--------------------------------------------------------------------------------------------------*/
/*!	\throw no-throw 
	\return the property list / y-iterator to an accessor property table */
const AccessorPropertyTable_YIterator SQLStringBuilder::getPropertyList() const
{
	return m_pBuilder->getPropertyList();
}

/*--------------------------------------------------------------------------------------------------*/
/*!	\throw no-throw 
	\param paramList list containing default parameters for all sql-statements of this builder */
void SQLStringBuilder::setParameterList( const basar::cmnutil::ParameterList paramList )
{
	m_pBuilder->setParameterList( paramList );
}

/*--------------------------------------------------------------------------------------------------*/
/*!	\throw no-throw  
    \return ParameterList */
basar::cmnutil::ParameterList  SQLStringBuilder::getParameterList() const
{
  return m_pBuilder->getParameterList();
}

/*--------------------------------------------------------------------------------------------------*/
/*!	\throw no-throw 
	\return SQL-statement */
const VarString& SQLStringBuilder::getSQLString() const
{
	return m_pBuilder->getSQLString();
}

/*--------------------------------------------------------------------------------------------------*/
/*!	\throw no-throw */
void SQLStringBuilder::clear()
{
	m_pBuilder->clear();
}

/*--------------------------------------------------------------------------------------------------*/
/*!	\throw no-throw 
	\return IndexTypeCollection */
const IndexTypeCollection& SQLStringBuilder::getPreparedParameterTypePositionCollection() const
{
    return m_pBuilder->getPreparedParameterTypePositionCollection();
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/*!	Checks if all paramter given in rPropertyNames are set in the Y-Iterator
    \throw no-throw 
	\return true if all given properties are set, so that sqlstring is executable 
	\return false otherwise 
	\param rPropertyNames names of columns in a Propertytable */
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
bool SQLStringBuilder::arePropertiesSet(const PropertyName& rPropertyNames) const
{
	return m_pBuilder->arePropertiesSet(rPropertyNames);
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/*!	Checks if all paramter given in rPropertyNames are set in the Y-Iterator
    \throw no-throw 
	\retval true if all given properties are set, so that sqlstring is executable 
	\return false otherwise 
	\param rParaList names of columns in a Propertytable */
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
bool SQLStringBuilder::arePropertiesSet(const basar::cmnutil::ParameterList& rParaList) const
{
	return m_pBuilder->arePropertiesSet(rParaList);
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/*!	
    Checks if all paramter given in rPropertyNames are set in the Y-Iterator and if
    at least one is in the state rState
    \throw no-throw 
	\return true if all given properties are set and at least one of 
            the given properties is in one of the given states, so that 
            sqlstring is executable
	\return false otherwise 
	\param rPropertyNames names of columns in a Propertytable 
	\param rState state of the Property */
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
bool SQLStringBuilder::arePropertiesSetAndOneMustBeInState( const PropertyName& rPropertyNames, 
														    const PropertyStateSet& rState) const
{
	return m_pBuilder->arePropertiesSetAndOneMustBeInState( rPropertyNames, rState );
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/*!	Checks if all paramter given in rPropertyNames are set in the Y-Iterator and if
    at least one is in the state rState
	\retval true if all given properties (rPropertyNames) are set and at least 
            one of the given properties is in one of the given states, 
            so that sqlstring is executable
	\return false otherwise 
	\param rPropertyNames names of columns in a Propertytable 
	\param rState state of the Property */
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
bool SQLStringBuilder::arePropertiesSetAndOneMustBeInState( 
    const basar::cmnutil::ParameterList& rPropertyNames, 
	const PropertyStateSet& rState) const
{
	return m_pBuilder->arePropertiesSetAndOneMustBeInState( rPropertyNames, rState );
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/*!	Checks if all paramter given in rPropertyNames are set in the Y-Iterator and if
    at least one is in the state rState
	\retval true if all given properties are set and at least one of the 
            given properties is in one of the given states, so that sqlstring is executable 
	\return false otherwise 
	\param rPropertyNames names of columns in a Propertytable 
	\param rState state of the Property */
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
bool SQLStringBuilder::arePropertiesSetAndOneMustBeInState( 
    const PropertyName& rPropertyNames, 
	const VarString& rState) const
{
	return m_pBuilder->arePropertiesSetAndOneMustBeInState( rPropertyNames, rState );
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/*!	Checks if all paramter given in rPropertyNames are set in the Y-Iterator and if
    at least one is in the state rState
	\return true if all given properties are set and at least 
            one of the given properties is in one of the given states, so that 
            sqlstring is executable 
	\return false otherwise 
	\param rPropertyNames names of columns in a Propertytable 
	\param rState state of the Property */
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
bool SQLStringBuilder::arePropertiesSetAndOneMustBeInState( 
    const basar::cmnutil::ParameterList& rPropertyNames, 
	const VarString& rState) const
{
	return m_pBuilder->arePropertiesSetAndOneMustBeInState( rPropertyNames, rState );
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/*!	Checks if all paramter given in rPropertyNames are set in the Y-Iterator and if
    all are in the state rState
	\return true if all given properties are set and at all of 
            the given properties are in one of the given states, so that 
            sqlstring is executable 
            at least one of the properties has to be listed in the Y-iterator 
            only default Paramter returns false 
	\return false otherwise 
	\param rPropertyNames names of columns in a Propertytable 
	\param rState state of the Property */
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
bool SQLStringBuilder::arePropertiesSetAndAllInState( const PropertyName& rPropertyNames, 
													  const PropertyStateSet& rState) const
{
	return m_pBuilder->arePropertiesSetAndAllInState( rPropertyNames, rState );
}
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/*!	Checks if all paramter given in rPropertyNames are set in the Y-Iterator and if
    all are in the state rState
	\return true if all given properties are set and at all of the given
            properties are in one of the given states, so that sqlstring is executable  
            at least one of the properties has to be listed in the Y-iterator 
            only default Paramter returns false 
	\return false otherwise 
	\param rPropertyNames names of columns in a Propertytable 
	\param rState state of the Property */
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
bool SQLStringBuilder::arePropertiesSetAndAllInState( const basar::cmnutil::ParameterList&  rPropertyNames, 
													  const VarString& rState) const
{
	return m_pBuilder->arePropertiesSetAndAllInState( rPropertyNames, rState );
}
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/*!	Checks if all paramter given in rPropertyNames are set in the Y-Iterator and if
    all are in the state rState
	\return true if all given properties are set and at all of 
            the given properties are in one of the given states, 
            so that sqlstring is executable  
            at least one of the properties has to be listed in the Y-iterator 
            only default Paramter returns false 
	\return false otherwise 
	\param rPropertyNames names of columns in a Propertytable 
	\param rState state of the Property */
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
bool SQLStringBuilder::arePropertiesSetAndAllInState( const basar::cmnutil::ParameterList&  rPropertyNames, 
													  const PropertyStateSet& rState) const
{
	return m_pBuilder->arePropertiesSetAndAllInState( rPropertyNames, rState );
}
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/*!	Checks if all paramter given in rPropertyNames are set in the Y-Iterator and if
    all are in the state rState
	\return true if all given properties are set and all of the 
            given properties are in one of the given states, so that 
            sqlstring is executable
            at least one of the properties has to be listed in the Y-iterator 
            only default Paramter returns false 
	\return false otherwise 
	\param rPropertyNames names of columns in a Propertytable 
	\param rState state of the Property */
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
bool SQLStringBuilder::arePropertiesSetAndAllInState( const PropertyName& rPropertyNames, 
													  const VarString& rState) const
{
	return m_pBuilder->arePropertiesSetAndAllInState( rPropertyNames, rState );
}
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/*!	 Checks is all the Properties listed in the 1st parameter are set in 
    in the aggregated AccessorPropertyTable_YIterator
    \return	true if all given properties (1st parameter as ParameterList) are set 
             and at least one of the given properties 
			(2nd parameter as ParameterList) is in one of the given states, 
            so that the sqlstring can be executed.
	\return false otherwise 
	\param	plShouldBeSet represents the properties which have to be set in the aggregated AccessorPropertyTable_YIterator.
	\param  plOneMustBeInState represents the properties which have to be set in the aggregated AccessorPropertyTable_YIterator
			and have to be in one of the given states (pss).
	\param  pss represents the state one of the properties of the 2nd parameter (propertyNamesOneMustBeInState)
			has to be in.
	\throw no-throw */
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
bool SQLStringBuilder::are1stPropertiesSetAnd2ndOneInState(	const basar::cmnutil::ParameterList& plShouldBeSet, 
															const basar::cmnutil::ParameterList& plOneMustBeInState, 
															const PropertyStateSet& pss ) const
{
	return m_pBuilder->are1stPropertiesSetAnd2ndOneInState( plShouldBeSet, plOneMustBeInState, pss );
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/*!	Checks is all the Properties listed in the 1st parameter are set in 
    in the aggregated AccessorPropertyTable_YIterator
    and if at least one of the parameter in the 2nd parameter is in state pss
    \return	true if all given properties (1st parameter as ParameterList) are set 
            and at least one of the given properties 
			(2nd parameter as ParameterList) is in one of the given states, 
            so that the sqlstring can be executed.
	\return false otherwise 
	\param	plShouldBeSet represents the properties which have to be set in the aggregated AccessorPropertyTable_YIterator.
	\param  plOneMustBeInState represents the properties which have to be set in the aggregated AccessorPropertyTable_YIterator
			and have to be in one of the given states (pss).
	\param  pss represents the state one of the properties of the 2nd parameter (propertyNamesOneMustBeInState)
			has to be in.
	\throw no-throw */
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
bool SQLStringBuilder::are1stPropertiesSetAnd2ndOneInState(	const basar::cmnutil::ParameterList& plShouldBeSet, 
															const basar::cmnutil::ParameterList& plOneMustBeInState, 
															const VarString& pss ) const
{
	return m_pBuilder->are1stPropertiesSetAnd2ndOneInState( plShouldBeSet, plOneMustBeInState, pss );
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/*! Checks is all the Properties listed in the 1st parameter are set in 
    in the aggregated AccessorPropertyTable_YIterator
    and if at least one of the parameter in the 2nd parameter is in state pss
    \return true if all given properties (1st parameter as ParameterList) are set and at
             least one of the given properties 
			(2nd parameter as ParameterList) is in one of the given states, 
            so that the sqlstring can be executed.
	\return false otherwise 
	\param	propertyNamesShouldBeSet represents the properties which have to be set in the aggregated 
			AccessorPropertyTable_YIterator.
	\param  propertyNamesOneMustBeInState represents the properties which have to be set in the aggregated 
			AccessorPropertyTable_YIterator and have to be in one of the given states (pss).
	\param  pss represents the possible states one of the properties of the 2nd parameter (propertyNamesOneMustBeInState)
			has to be in.
	\throw no-throw */
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
bool SQLStringBuilder::are1stPropertiesSetAnd2ndOneInState(	const PropertyName& propertyNamesShouldBeSet, 
															const PropertyName& propertyNamesOneMustBeInState, 
															const PropertyStateSet& pss ) const
{
	return m_pBuilder->are1stPropertiesSetAnd2ndOneInState( propertyNamesShouldBeSet, propertyNamesOneMustBeInState, pss );
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/*!	Checks is all the Properties listed in the 1st parameter are set in 
    in the aggregated AccessorPropertyTable_YIterator
    and if at least one of the parameter in the 2nd parameter is in state pss
    \return	true if all given properties (1st parameter as ParameterList) are set and at least one of the given properties 
			(2nd parameter as ParameterList) is in one of the given states, so that the sqlstring can be executed.
	\return false otherwise 
	\param	propertyNamesShouldBeSet represents the properties which have to be set in the aggregated 
			AccessorPropertyTable_YIterator.
	\param  propertyNamesOneMustBeInState represents the properties which have to be set in the aggregated 
			AccessorPropertyTable_YIterator and have to be in one of the given states (pss).
	\param  rState represents the state (as string) one of the properties of the 2nd parameter (propertyNamesOneMustBeInState)
			has to be in.
	\throw no-throw */
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
bool SQLStringBuilder::are1stPropertiesSetAnd2ndOneInState(	const PropertyName& propertyNamesShouldBeSet, 
															const PropertyName& propertyNamesOneMustBeInState, 
															const VarString& rState ) const
{
	return m_pBuilder->are1stPropertiesSetAnd2ndOneInState( propertyNamesShouldBeSet, propertyNamesOneMustBeInState, rState );
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////
/*!	\retval	true if given property is set and yiterator is not end 
	\return false otherwise 
	\param	rPropertyName property to check if it is set
	\throw	no-throw */
///////////////////////////////////////////////////////////////////////////////////////////////////////////
bool	SQLStringBuilder::isNotEndAndSet( const PropertyName& rPropertyName ) const
{
	return m_pBuilder->isNotEndAndSet( rPropertyName );
}
	
///////////////////////////////////////////////////////////////////////////////////////////////////////////
/*!	\retval	true if given property is set and contained in yiterator
	\return	otherwise returns false
	\param	rPropertyName property to check
	\throw	no-throw */
///////////////////////////////////////////////////////////////////////////////////////////////////////////
bool	SQLStringBuilder::isContainedAndSet( const PropertyName& rPropertyName ) const
{
	return m_pBuilder->isContainedAndSet( rPropertyName );
}

/*!	\throw no-throw 
	\return a pointer to the class SQLStringBuilder 
	\param rLiteral is a string to append to the SQLString */
SQLStringBuilder& SQLStringBuilder::literal(const VarString& rLiteral)
{
	m_pBuilder->literal(rLiteral);
	return *this;
}

/*!	\throw UnknownPropertyNameException 
	\return a pointer to the class SQLStringBuilder 
	\param rPropertyNames parameter names of the property table */
SQLStringBuilder& SQLStringBuilder::value(const PropertyName& rPropertyNames)
{
	m_pBuilder->value(rPropertyNames);
	return *this;
}

/*!	\throw UnknownPropertyNameException 
	\return a pointer to the class SQLStringBuilder 
	\param rPattern string, which is  interpreted in this function */
SQLStringBuilder& SQLStringBuilder::resolve(const VarString& rPattern)
{
	m_pBuilder->resolve(rPattern);
	return *this;
}

/*!	\throw UnknownPropertyNameException 
	\return the value of the parameter name 
	\param rPropertyNames parameter names of the property table */
const VarString SQLStringBuilder::getParamValue(const PropertyName& rPropertyNames) const
{
	return m_pBuilder->getParamValue(rPropertyNames);
}

/*!	\throw no-throw 
	\retval true if it is a query
	\return otherwise returns false */
std::pair<bool, QueryTypeEnum> SQLWriteStringBuilder::isQuery() const
{
	return std::make_pair<bool, QueryTypeEnum>( false, MULTI_QUERY );
}

/*!	\throw no-throw 
	\retval true if it is a query 
	\return otherwise returns false */
std::pair<bool, QueryTypeEnum> SQLQueryStringBuilder::isQuery() const
{
	return std::make_pair<bool, QueryTypeEnum>( true, MULTI_QUERY );
}

/*!	\throw no-throw 
	\retval true if it is prepareable
	\return otherwise returns false */
bool SQLQueryStringBuilder::isPrepareable() const
{
    return false; //note: returns false, because thats the default (i.e. builders are not prepareable in general)
}

/*!	\throw no-throw 
	\retval true if it is prepareable
	\return otherwise returns false */
bool SQLWriteStringBuilder::isPrepareable() const
{
    return false; //note: returns false, because thats the default (i.e. builders are not prepareable in general)
}

/*!	\throw no-throw 
	\retval true if it is a query
	\return otherwise returns false */
std::pair<bool, QueryTypeEnum> SQLPrepareableWriteStringBuilder::isQuery() const
{
	return std::make_pair<bool, QueryTypeEnum>( false, MULTI_QUERY );
}

/*!	\throw no-throw 
	\retval true if it is a query
	\return otherwise returns false */
std::pair<bool, QueryTypeEnum> SQLPrepareableQueryStringBuilder::isQuery() const
{
	return std::make_pair<bool, QueryTypeEnum>( true, MULTI_QUERY );
}

/*!	\throw no-throw 
	\retval true if it is prepareable
	\return otherwise returns false */
bool SQLPrepareableQueryStringBuilder::isPrepareable() const
{
    return true;
}

/*!	\throw no-throw 
	\retval true if it is prepareable
	\return otherwise returns false */
bool SQLPrepareableWriteStringBuilder::isPrepareable() const
{
    return true;
}

}//aspect
}//db
}//basar
