////////////////////////////////////////////////////////////////////////////////////////////////////
/*! \file
 *  \brief  class represents the column name and the datatype of the database query
 *  \author Bischof Björn 
 *  \date   25.07.2005
 */
////////////////////////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////////////////////////
// includes
////////////////////////////////////////////////////////////////////////////////////////////////////


#include "propertydescriptionlist.h"

#include "libbasarproperty_propertydescriptionlist_iterator.h"
#include "libbasarproperty_propertydescription.h"
#include "libbasarproperty_propertytype.h"
#include "libbasarproperty_exceptions.h"
#include "loggerpool.h"

#include "../macro.h"

#include <stdexcept>


#define DELIMITER ';' //!< delimiter to divide the columns of a property description list at string construction

////////////////////////////////////////////////////////////////////////////////////////////////////
// namespaces
////////////////////////////////////////////////////////////////////////////////////////////////////
namespace basar
{
namespace property
{

////////////////////////////////////////////////////////////////////////////////////////////////////
//! \return	shared_ptr of this class
//! \throw no-throw
////////////////////////////////////////////////////////////////////////////////////////////////////
PdlSharedPtr PropertyDescriptionList::create()
{
    PdlSharedPtr p( new PropertyDescriptionList() ); // RAII
    p->setThisPtr(p);
    return p;
}
//--------------------------------------------------------------------------//
// implementation
//--------------------------------------------------------------------------//
////////////////////////////////////////////////////////////////////////////////////////////////////
//!	\throw no-throw
////////////////////////////////////////////////////////////////////////////////////////////////////
PropertyDescriptionList::PropertyDescriptionList()
{
}
////////////////////////////////////////////////////////////////////////////////////////////////////
//!	\throw no-throw
////////////////////////////////////////////////////////////////////////////////////////////////////
PropertyDescriptionList::~PropertyDescriptionList()
{
}
////////////////////////////////////////////////////////////////////////////////////////////////////
/*! \param	rValuePairs is one successional string to build datatypes and columnnames
    \throw  InvalidParameterException, UnknownPropertyNameException */
////////////////////////////////////////////////////////////////////////////////////////////////////
void PropertyDescriptionList::construct( const I18nString& rValuePairs )
{
	FUNCTION_NAME( basar.property.PropertyDescriptionList.construct(VarString) )
	
	basar::ConstString subDelimiter = " ";
    basar::ConstString delimiter	= ";";
	m_ConstructString = rValuePairs;
	m_ConstructString.cleanchar();	// whitespaces and other non printable chars are replaced by blanks (0x20)
    m_ConstructString.trim();
    
    bool removeEmpty = true; //< no check on empty values, it is possible to just have ";" - Description
    bool aceptNoLastDelimiter = true; //< there has to be no last ";" Value, or ?

    basar::cmnutil::CollBString substrings = 
        m_ConstructString.tokenizeAndTrim(delimiter, basar::cmnutil::BString::FIRST_END, aceptNoLastDelimiter, removeEmpty);
 
    basar::cmnutil::CollBString::const_iterator iter;
    aceptNoLastDelimiter = true; //< we have no blank as last parameter
    removeEmpty = true; //< more than on blank possible
    for (iter = substrings.begin(); iter != substrings.end(); ++iter)
    {
       basar::cmnutil::CollBString valuepair = iter->tokenizeAndTrim(subDelimiter, basar::cmnutil::BString::FIRST_END, aceptNoLastDelimiter, removeEmpty);
       if (2 != valuepair.size())
       {
			VarString msg;
			msg.format("Data type or column name missed as parameter in pair-string <%s> !", rValuePairs.c_str());
			throw InvalidParameterException( ExceptInfo( fun, msg.c_str(), __FILE__, __LINE__) );
       }
       push_back( valuepair[1], PropertyType( valuepair[0] ));
    }
}

///////////////////////////////////////////////////////////////////////////////////////////////
/*! \param	rValuePairs pair of strings to set to PropertyDescriptionList
	\throw	string based exception indicates that no space was given to separate the 
			datatype from the columnname */
///////////////////////////////////////////////////////////////////////////////////////////////
void PropertyDescriptionList::construct( const PropertyDescriptionVector& rValuePairs )
{
    std::vector<PropertyDescription>::const_iterator iter;
    for (iter = rValuePairs.m_vector.begin(); iter != rValuePairs.m_vector.end(); ++iter)
    {
       push_back(*iter);
    }
}

//------------------------------------------------------------------------------------------------//
//	push_back - methods
//------------------------------------------------------------------------------------------------//
////////////////////////////////////////////////////////////////////////////////////////////////////
/*! \throw  PropertyNameAlreadyExistsException
    \param  rName	represents the name of the column */
////////////////////////////////////////////////////////////////////////////////////////////////////
void PropertyDescriptionList::push_back( const ColumnName& rName )
{
	push_back( rName, PropertyType() );
}
////////////////////////////////////////////////////////////////////////////////////////////////////
/*! \throw  PropertyNameAlreadyExistsException
    \param  rName:      represents the name of the column
    \param  propType:   represents the datatype (PropertyType(Dflt:UNKNOWN)) of the concerned column */
////////////////////////////////////////////////////////////////////////////////////////////////////
void PropertyDescriptionList::push_back( const ColumnName& rName, const PropertyType& propType )
{

	push_back( ColNamePropTypePair(rName, propType) );
}

///////////////////////////////////////////////////////////////////////////////////////////////
/*! \param	pd is the combination of a columnname and a type (SupportedTypeEnum) which 
			is added to the already existing columns
	\throw  PropertyNameAlreadyExistsException */
///////////////////////////////////////////////////////////////////////////////////////////////
void PropertyDescriptionList::push_back( const PropertyDescription & pd)
 {
        push_back(pd.getName(), pd.getType());
 }

////////////////////////////////////////////////////////////////////////////////////////////////////
/*! \throw  PropertyNameAlreadyExistsException
    \param  rPair:   represents a pair of a columnname and the corresponding datatype */
////////////////////////////////////////////////////////////////////////////////////////////////////
void PropertyDescriptionList::push_back( const ColNamePropTypePair& rPair )
{
	FUNCTION_NAME( basar.property.PropertyDescriptionList.push_back(ColNamePropTypePair) )

	if (LoggerPool::propertyDescriptionList().isEnabledFor(log4cplus::TRACE_LOG_LEVEL))
	{
		VarString msg;
        msg.format("PropertyDescriptionList::push_back: ColumnName = %s, Type = %s", rPair.first.c_str() , rPair.second.toString().c_str() );
        LoggerPool::propertyDescriptionList().forcedLog(log4cplus::TRACE_LOG_LEVEL, msg.c_str(), __FILE__, __LINE__);
	}

	if( contains(rPair.first) )
    {
		VarString msg;
		msg.format( "A propertyname with identical name <%s> has been added yet!", rPair.first.c_str() );
		throw PropertyNameAlreadyExistsException( ExceptInfo( fun, msg.c_str(), __FILE__, __LINE__) );
	}   
	m_ColNamePropTypeColl.push_back( rPair );
}
//------------------------------------------------------------------------------------------------//
//	contains - methods
//------------------------------------------------------------------------------------------------//
////////////////////////////////////////////////////////////////////////////////////////////////////
/*! \throw  no-throw
    \retval true if passed ColumnName occurs in the PropertyDescriptionList
	\return otherwise returns false
	\param  rName ColumnName that is searched in the PropertyDescriptionList */
////////////////////////////////////////////////////////////////////////////////////////////////////
bool PropertyDescriptionList::contains( const ColumnName& rName ) const
{
	return contains( std::make_pair(rName, PropertyType()) );
}
////////////////////////////////////////////////////////////////////////////////////////////////////
/*! \throw	no-throw
    \retval true if passed ColumnName occurs with passed type in the PropertyDescriptionList
	\return otherwise returns false
	\param  rName:      represents the name of the column passed as const reference
    \param  propType:   represents the datatype of the concerned column */
////////////////////////////////////////////////////////////////////////////////////////////////////
bool PropertyDescriptionList::contains( const ColumnName& rName, const PropertyType& propType ) const
{
	return contains( std::make_pair(rName, propType) );
}
////////////////////////////////////////////////////////////////////////////////////////////////////
/*! \throw  no-throw
    \retval true if passed ColumnName occurs with passed type in the PropertyDescriptionList
	\return otherwise returns false
	\param  rPair:	represents a const pair of columnname and datatype.
					Datatype (PropertyType) with value UNKNOWN will match all column names.
					I.e.: If the datatype is UNKNOWN, it will be disregarded and only the 
					column name is checked. */
////////////////////////////////////////////////////////////////////////////////////////////////////
bool PropertyDescriptionList::contains( const ColNamePropTypePair& rPair ) const
{
    /*! \todo	should the search-algorithm used here 
				std::find_if(m_ColNamePropTypeColl.begin(), m_ColNamePropTypeColl.end(), name); */

	if( rPair.second.getType() == basar::UNKNOWN ){
        ColNamePropTypeColl::const_iterator it = m_ColNamePropTypeColl.begin();
        while( m_ColNamePropTypeColl.end()!= it ){
            if( rPair.first == it->first ){
                return true;
            }
            ++it;
        }
    } else {
        //return contains( ColNamePropTypePair(rName, PropType) );
		if( m_ColNamePropTypeColl.end() != 
			std::find(m_ColNamePropTypeColl.begin(), m_ColNamePropTypeColl.end(), rPair) ){
			return true;
		}
    }
    return false;
}
////////////////////////////////////////////////////////////////////////////////////////////////////
/*! \return RowSize
	\throw	no-throw*/
////////////////////////////////////////////////////////////////////////////////////////////////////
NumberColumns PropertyDescriptionList::size() const
{
    return m_ColNamePropTypeColl.size();
}
////////////////////////////////////////////////////////////////////////////////////////////////////
/*! \throw	UnknownColumnNameException
    \return index for given column
    \param  rColName a columnname as a const reference is passed 
			to get the index of that column */
////////////////////////////////////////////////////////////////////////////////////////////////////
ColumnIndex PropertyDescriptionList::getIndex( const ColumnName& rColName ) const
{ 
    ColumnIndex retval = 0;
    FUNCTION_NAME( basar.property.PropertyDescriptionList.getIndex(ColumnName) )
	ColNamePropTypeColl::const_iterator it = m_ColNamePropTypeColl.begin();   
    for( ; m_ColNamePropTypeColl.end()!= it; ++it)
    {
        if( rColName == it->first )
        {
			break;
        }
        ++retval;
    }
    if (m_ColNamePropTypeColl.end() == it)
    {
	  VarString msg;
	  msg.format( "Passed name of the column: <%s> does not exist!", rColName.c_str() );
      throw UnknownPropertyNameException( ExceptInfo(	fun, msg.c_str(), __FILE__, __LINE__) );
    }
    return retval;
}
////////////////////////////////////////////////////////////////////////////////////////////////////
/*! \throw  OutOfRangeIndexException
    \return ColumnName (string based)
    \param  index an index as a value is passed to get the corresponding columnname */
////////////////////////////////////////////////////////////////////////////////////////////////////
const ColumnName& PropertyDescriptionList::getName( const ColumnIndex index )const
{
    FUNCTION_NAME( basar.property.PropertyDescriptionList.getName(%d) )

	try 
	{
        return m_ColNamePropTypeColl.at( index ).first;
    }
	catch ( const std::out_of_range& r )
	{
		basar::VarString fun1;
		fun1.format(fun, index);
		throw OutOfRangeIndexException( ExceptInfo( fun1, r.what(), __FILE__, __LINE__) );
    }
}
////////////////////////////////////////////////////////////////////////////////////////////////////
/*! \throw	OutOfRangeIndexException
    \return const PropertyType for given index
    \param  index an index as a value is passed to get the corresponding propertytype */
////////////////////////////////////////////////////////////////////////////////////////////////////
const PropertyType & PropertyDescriptionList::getType( const ColumnIndex index ) const
{
    FUNCTION_NAME( basar.property.PropertyDescriptionList.getType(RowSize) )
	
	try 
	{
        return m_ColNamePropTypeColl.at( index ).second;
    } 
	catch ( const std::out_of_range& r )
	{
		throw OutOfRangeIndexException(ExceptInfo( fun, r.what(), __FILE__, __LINE__) );
    }
}
////////////////////////////////////////////////////////////////////////////////////////////////////
/*! \throw  UnknownPropertyNameException
    \return PropertyType for given ColumnName
    \param  rColName via const reference a columnname is passed 
					  to get the corresponding propertytype */
////////////////////////////////////////////////////////////////////////////////////////////////////
const PropertyType& PropertyDescriptionList::getType( const ColumnName& rColName ) const
{   
    FUNCTION_NAME( basar.property.PropertyDescriptionList.getType(ColumnName) )
		
	ColNamePropTypeColl::const_iterator	itEnd = m_ColNamePropTypeColl.end();

	const PropertyType * pType = 0;
	bool                 found = false;

    for( ColNamePropTypeColl::const_iterator it = m_ColNamePropTypeColl.begin(); itEnd != it; ++it )
	{
        if( rColName == it->first )
		{
            pType = &it->second;
			found = true;
			break;
		}
    }

	if (!found)
	{
		VarString msg;
		msg.format( "Passed name of the column: <%s> does not exist!", rColName.c_str() );
    	throw UnknownPropertyNameException( ExceptInfo(	fun, msg.c_str(), __FILE__, __LINE__) );
	}

	return *pType;
}
////////////////////////////////////////////////////////////////////////////////////////////////////
/*! \throw no-throw
    \return PropertyDescriptionList_Iterator pointing to first element */
////////////////////////////////////////////////////////////////////////////////////////////////////
PropertyDescriptionList_Iterator PropertyDescriptionList::begin() // const
{
    return PropertyDescriptionList_Iterator( thisPtr(), m_ColNamePropTypeColl.begin() );
}
////////////////////////////////////////////////////////////////////////////////////////////////////
/*! \throw no-throw
    \return PropertyDescriptionList_Iterator pointing one position beyond the last element */
////////////////////////////////////////////////////////////////////////////////////////////////////
PropertyDescriptionList_Iterator PropertyDescriptionList::end() // const
{
    return PropertyDescriptionList_Iterator( thisPtr(), m_ColNamePropTypeColl.end() );
}
////////////////////////////////////////////////////////////////////////////////////////////////////
/*! \throw  no-throw */
////////////////////////////////////////////////////////////////////////////////////////////////////
void PropertyDescriptionList::clear()
{
    m_ColNamePropTypeColl.clear();
}
////////////////////////////////////////////////////////////////////////////////////////////////////
/*! \param	p a simple pointer of this class is converted to a weak-pointer
	\throw  no-throw */
////////////////////////////////////////////////////////////////////////////////////////////////////
void PropertyDescriptionList::setThisPtr( PdlWeakPtr p )
{
	m_this_as_weak_ptr = p;
}
////////////////////////////////////////////////////////////////////////////////////////////////////
/*! \return const SharedPtr to property description list
	\throw  no-throw */
////////////////////////////////////////////////////////////////////////////////////////////////////
const PdlSharedPtr PropertyDescriptionList::thisPtr() const
{
	return PdlSharedPtr( m_this_as_weak_ptr );
}
////////////////////////////////////////////////////////////////////////////////////////////////////
/*! \return SharedPtr to property description list
	\throw  no-throw */
////////////////////////////////////////////////////////////////////////////////////////////////////
PdlSharedPtr PropertyDescriptionList::thisPtr() 
{
	return PdlSharedPtr( m_this_as_weak_ptr );
}
////////////////////////////////////////////////////////////////////////////////////////////////////
//! \return	shared_ptr of this class
//!	\throw  no-throw 
////////////////////////////////////////////////////////////////////////////////////////////////////
PdlSharedPtr PropertyDescriptionList::deepCopy()
{
    PdlSharedPtr p( new PropertyDescriptionList() ); 
	p->m_ColNamePropTypeColl = m_ColNamePropTypeColl;
    return p;
}
////////////////////////////////////////////////////////////////////////////////////////////////////
/*! \param	strm parameter is default initialized and could be overwritten by any std::ostream object,
			e.g. by the error channel or any object of a ofstream file.
  	\throw  no-throw */
////////////////////////////////////////////////////////////////////////////////////////////////////
void PropertyDescriptionList::toStream( std::ostream& strm ) const
{
	ColNamePropTypeColl::const_iterator  itBegin = m_ColNamePropTypeColl.begin();
	ColNamePropTypeColl::const_iterator  itEnd   = m_ColNamePropTypeColl.end  ();
	ColNamePropTypeColl::difference_type dt;

	for (ColNamePropTypeColl::const_iterator it = itBegin; it != itEnd; ++it)
	{
		if ( it == itBegin)
		{
			VarString addr;
			addr.format("0x%0*lx", sizeof(size_t) * 2, this);

			strm << "--- PropertyDescriptionList @ " << addr
				 << " -------------------" << std::endl;
		}

		dt = std::distance( itBegin, it);

		VarString no;
		no.format("#%-5d", dt);

		strm << no
			 <<       " name: <" << it->first             << ">"   << std::endl
			 << "       type: <" << it->second.toString() << ">"   << std::endl
			 << "------------------------------------------------" << std::endl;
	}
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/*!
 *  writes the data to a string (in the same 
 *  format as the construction string)
 *  f.ex. "string col1;int col2;"
 *  \return string (in the same format as the construction string) f.ex. "string col1;int col2;"
 */
VarString PropertyDescriptionList::toString() const
{
    basar::VarString retval;
    ColNamePropTypeColl::const_iterator iter;
    for (iter = m_ColNamePropTypeColl.begin(); iter != m_ColNamePropTypeColl.end(); ++iter)
    {
        retval += iter->second.toString();
        retval += " ";
        retval += iter->first;
        retval += ";";
    }  
    return retval;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
} // End namespace property
} // End namespace basar
