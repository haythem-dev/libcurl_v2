//---------------------------------------------------------------------------------------------//
//! \file
//! \brief  internal implementation for the caching policy 'no caching'
//! \author Bjoern Bischof
//! \date   10.10.2006
//---------------------------------------------------------------------------------------------//

//---------------------------------------------------------------------------------------//
// includes
//---------------------------------------------------------------------------------------//
#include "nocaching.h"

#include "libbasarproperty_exceptions.h"
#include "libbasarproperty_propertydescriptionlistref.h"
#include "libbasarproperty_propertytable_xiterator.h"
#include "libbasardbaspect_accessorpropertytable_yiterator.h"
#include "cachecontroller.h"
#include "libbasardbaspect_exceptions.h"

//---------------------------------------------------------------------------------------------//
// namespaces
//---------------------------------------------------------------------------------------------//
namespace basar  {
namespace db	 {
namespace aspect {

//---------------------------------------------------------------------------------------------//
// object section
//---------------------------------------------------------------------------------------------//
////////////////////////////////////////////////////////////////////////////////////////////////
//! \throw  no-throw
/*! \param  cacheController here a plain pointer to the concerning cacheController. 
			It is stored as a member to delegate severals methods. */
////////////////////////////////////////////////////////////////////////////////////////////////
NoCaching::NoCaching ( CacheController * cacheController )
					 : CachingPolicy( cacheController )
{
}
////////////////////////////////////////////////////////////////////////////////////////////////
//! \throw  no-throw
////////////////////////////////////////////////////////////////////////////////////////////////
NoCaching::~NoCaching()
{
}

////////////////////////////////////////////////////////////////////////////////////////////////
/*! \retval	true if the passed AccessorPropertyTable_YIterator represents the first iterator (begin())
  ! \retval	otherwise false
//! \param  r is the AccessorPropertyTable_YIterator which internal cachController is asked for 
			the begin()-method.
  ! \throw  no-throw */
////////////////////////////////////////////////////////////////////////////////////////////////
bool NoCaching::isBegin(const AccessorPropertyTable_YIterator& r) const
{ 
	return ( 0 == r.m_RowNumber );
}

////////////////////////////////////////////////////////////////////////////////////////////////
/*! \retval	true if the passed AccessorPropertyTable_YIterator represents one position after 
			the last iterator (end())
  ! \retval	otherwise false
//! \param  r is the AccessorPropertyTable_YIterator which internal cachController is asked for 
			the end()-method.
  ! \throw  no-throw */
////////////////////////////////////////////////////////////////////////////////////////////////
bool NoCaching::isEnd(const AccessorPropertyTable_YIterator& r) const
{ 
	return ( ITERATOR_ENDPOS == r.m_RowNumber );
}

////////////////////////////////////////////////////////////////////////////////////////////////
/*! \retval	true if the internal row number is updated
  ! \retval	otherwise false
  ! \param  replicated is used in derived classes
  ! \param  r is the AccessorPropertyTable_YIterator which is controlled internally 
			if it is equivalent to an end()-iterator.
  ! \throw  no-throw */
////////////////////////////////////////////////////////////////////////////////////////////////
bool NoCaching::updateRowNumber( const bool replicated, AccessorPropertyTable_YIterator & r )
{
	if( replicated )
		r.m_RowNumber = static_cast<Int32>(r.m_pCacheController->m_Resultset.getFetchedRows()) - 1;
	else
		r.m_RowNumber = ITERATOR_ENDPOS;

	return true;
}
////////////////////////////////////////////////////////////////////////////////////////////////
/*! \return	AccessorPropertyTable_XIterator represents the xiterator to the first attribut 
			of the AccessorPropertyTable_YIterator.
  ! \param  r is the AccessorPropertyTable_YIterator which is the begin()-iterator returned of.
  ! \throw  XIteratorNotAvailableException */
////////////////////////////////////////////////////////////////////////////////////////////////
AccessorPropertyTable_XIterator NoCaching::begin( const AccessorPropertyTable_YIterator& ) const
{
	static ConstString fun = "basar.db.ascpect.NoCaching.begin(yit)";

	if (true)
	{
		throw XIteratorNotAvailableException(ExceptInfo( fun, 
														"PropertyTable_XIterator is not available in context of "
														"AccessorPropertyTable_YIterator with caching policy "
														"'NO_CACHING'.", 
														__FILE__, __LINE__) );
	}

	return AccessorPropertyTable_XIterator();
}

////////////////////////////////////////////////////////////////////////////////////////////////
/*! \return	AccessorPropertyTable_XIterator represents the xiterator to the first attribut 
			having its state in the passsed PropertyStateSet.
  ! \param  r is the AccessorPropertyTable_YIterator which is the begin()-method delegated to.
  ! \param  state set iterator to first element with this state
  ! \throw  XIteratorNotAvailableException */
////////////////////////////////////////////////////////////////////////////////////////////////
AccessorPropertyTable_XIterator NoCaching::begin(	const PropertyStateSet          & , 
													AccessorPropertyTable_YIterator & )
{
	static ConstString fun = "basar.db.ascpect.NoCaching.begin(PropertyStateSet)";

	if (true)
	{
		throw XIteratorNotAvailableException( ExceptInfo(fun, 
														 "PropertyTable_XIterator is not available in context of "
														 "AccessorPropertyTable_YIterator with caching policy "
														 "'NO_CACHING'.", 
														 __FILE__, __LINE__) );
	}

	return AccessorPropertyTable_XIterator();
}

////////////////////////////////////////////////////////////////////////////////////////////////
/*! \return	AccessorPropertyTable_XIterator represents the xiterator one step after the last 
			attribut of the AccessorPropertyTable_YIterator.
  ! \param  r is the AccessorPropertyTable_YIterator which is the end()-iterator returned of.
  ! \throw  XIteratorNotAvailableException */
////////////////////////////////////////////////////////////////////////////////////////////////
AccessorPropertyTable_XIterator NoCaching::end(const AccessorPropertyTable_YIterator& ) const
{
	static ConstString fun = "basar.db.ascpect.NoCaching.end()";

	if (true)
	{
		throw XIteratorNotAvailableException(ExceptInfo( fun, 
														"PropertyTable_XIterator is not available in context of "
														"AccessorPropertyTable_YIterator with caching policy "
														"'NO_CACHING'.", 
														__FILE__, __LINE__) );
	}

	return AccessorPropertyTable_XIterator(); 
}
////////////////////////////////////////////////////////////////////////////////////////////////
/*! \return	PropertyStateSet representing the internal filter (important for filtered iteration)
  ! \param  r is the AccessorPropertyTable_YIterator which is the getFilter()-method delegated to.
  ! \throw  NoFilterAvailableException */
////////////////////////////////////////////////////////////////////////////////////////////////
const PropertyStateSet& NoCaching::getFilter(const AccessorPropertyTable_YIterator& ) const
{
	static ConstString fun = "basar.db.ascpect.NoCaching.getFilter()";

	if (true)
	{
		throw NoFilterAvailableException ( ExceptInfo( fun, 
														"with caching policy 'NO_CACHING' "
														"it is not allowed to ask for filter "
														"(propertystateset)!",
														__FILE__, __LINE__ ) );
	}

    //for compiler warning only
    static const PropertyStateSet pss;

	return pss;
}
////////////////////////////////////////////////////////////////////////////////////////////////
/*! \return	PropertyStateSet representing the internal state (important for changing states).
  ! \param  r is the AccessorPropertyTable_YIterator which is the getState()-method delegated to.
  ! \throw  NoStateAvailableException */
////////////////////////////////////////////////////////////////////////////////////////////////
const PropertyStateSet NoCaching::getState(const AccessorPropertyTable_YIterator& ) const
{
	static ConstString fun = "basar.db.ascpect.NoCaching.getState()";

	if (true)
	{
		throw NoStateAvailableException(ExceptInfo( fun, 
													"with caching policy 'NO_CACHING' "
													"it is not allowed to ask for state!",
													__FILE__, __LINE__ ) );
	}

	return PropertyStateSet();
}
////////////////////////////////////////////////////////////////////////////////////////////////
/*! \retval	true if the given attribut name has the passed datatytpe and has furthermore a state
			which occurs in the passed PropertyStateSet.
  ! \retval	otherwise false
  ! \param  rColName is the attributname
  ! \param  pt is the datatype
  ! \param  pss is the set of states which is looked up in 
  ! \param  r is the AccessorPropertyTable_YIterator which is the contains()-method is executed on.
  ! \throw  NoStateAvailableException */
////////////////////////////////////////////////////////////////////////////////////////////////
bool NoCaching::contains(	const ColumnName & , 
							const PropertyType , 
							const PropertyStateSet , 
							const AccessorPropertyTable_YIterator& ) const
{
	static ConstString fun =	"basar.db.ascpect.NoCaching.contains()";

	if (true)
	{
		throw NoStateAvailableException(ExceptInfo(	fun, 
													"with caching policy 'NO_CACHING' "
													"it is not allowed to ask if a state is "
													"contained in propertystateset!",
													__FILE__, __LINE__ ) );	
	}

	return false;
}
////////////////////////////////////////////////////////////////////////////////////////////////
/*! \retval	true if the given attribut name has the passed datatytpe and has furthermore a state
			which occurs in the passed PropertyStateSet.
  ! \retval	otherwise false
  ! \param  p is a pair of an attribut name and the datatype
  ! \param  pss is the set of states which is looked up in 
  ! \param  r is the AccessorPropertyTable_YIterator which is the contains()-method is executed on.
  ! \throw  NoStateAvailableException */
////////////////////////////////////////////////////////////////////////////////////////////////
bool NoCaching::contains(const std::pair<ColumnName, PropertyType> & ,
      				     const PropertyStateSet                      ,
						 const AccessorPropertyTable_YIterator     & ) const
{
	static ConstString fun = "basar.db.ascpect.NoCaching.contains(pair<>)";

	if (true)
	{
		throw NoStateAvailableException( ExceptInfo( fun, 
													"with caching policy 'NO_CACHING' "
													"it is not allowed to ask if a state is "
													"contained in propertystateset!",
													__FILE__, __LINE__ ) );	
	}

	return false;
}
////////////////////////////////////////////////////////////////////////////////////////////////
/*! \return	represents the got value
  ! \param  rColName represents the attribute name which is the value returned for.
  ! \param  r is the AccessorPropertyTable_YIterator which is the getInt32()-method is executed on.
  ! \throw  no-throw */
////////////////////////////////////////////////////////////////////////////////////////////////
const Int32& NoCaching::getInt32( const ColumnName                      & rColName, 
						   const AccessorPropertyTable_YIterator & r ) const
{
	return r.m_pCacheController->m_Resultset.getInt32( rColName );
}
////////////////////////////////////////////////////////////////////////////////////////////////
/*! \return	represents the got value
  ! \param  index represents the attribute which is the value returned for.
  ! \param  r is the AccessorPropertyTable_YIterator which is the getInt32()-method is executed on.
  ! \throw  no-throw */
////////////////////////////////////////////////////////////////////////////////////////////////
const Int32& NoCaching::getInt32( const ColumnIndex                       index, 
						   const AccessorPropertyTable_YIterator & r ) const
{
	return r.m_pCacheController->m_Resultset.getInt32( 
				r.m_pCacheController->getPropertyDescriptionList().getName( index ) );
}
////////////////////////////////////////////////////////////////////////////////////////////////
/*! \return	represents the got value
  ! \param  rColName represents the attribute name which is the value returned for.
  ! \param  r is the AccessorPropertyTable_YIterator which is the getInt16()-method is executed on.
  ! \throw  no-throw */
////////////////////////////////////////////////////////////////////////////////////////////////
const Int16& NoCaching::getInt16(const ColumnName                      & rColName, 
						  const AccessorPropertyTable_YIterator & r ) const
{
	return r.m_pCacheController->m_Resultset.getInt16( rColName );
}
////////////////////////////////////////////////////////////////////////////////////////////////
/*! \return	represents the got value
  ! \param  index represents the attribute which is the value returned for.
  ! \param  r is the AccessorPropertyTable_YIterator which is the getInt16()-method is executed on.
  ! \throw  no-throw */
////////////////////////////////////////////////////////////////////////////////////////////////
const Int16& NoCaching::getInt16(const ColumnIndex                       index, 
						  const AccessorPropertyTable_YIterator & r ) const
{
	return r.m_pCacheController->m_Resultset.getInt16 ( 
					r.m_pCacheController->getPropertyDescriptionList().getName( index ) );
}
////////////////////////////////////////////////////////////////////////////////////////////////
/*! \return	represents the got value
  ! \param  rColName represents the attribute name which is the value returned for.
  ! \param  r is the AccessorPropertyTable_YIterator which is the getSQLString()-method is executed on.
  ! \throw  FatalErrorException */
////////////////////////////////////////////////////////////////////////////////////////////////
const SQLString NoCaching::getSQLString( const ColumnName                      & rColName, 
										 const AccessorPropertyTable_YIterator & r ) const
{
	static ConstString fun = "basar.db.aspect.NoCaching::getSQLString()";

	I18nString                 s;
	PropertyDescriptionListRef pdl = r.m_pCacheController->getPropertyDescriptionList();
	db::sql::ResultsetRef      rs  = m_pCacheController->m_Resultset;

	switch (pdl.getType(rColName).getType())
	{
	case INT16:
		s.itos(rs.getInt16(rColName));
		break;

	case INT32:
	case INTTIME:
	case INTDATE:
		s.itos(rs.getInt32(rColName));
		break;

	case FLOAT32:
		s.ftos(rs.getFloat32(rColName));
		break;

	case FLOAT64:
		s.ftos(rs.getFloat64(rColName));
		break;

	case DECIMAL:
		s = rs.getDecimal(rColName).toNonLocString();
		break;

	case STRING:
		// changes V1.0.5.0 the string value is no longer 
		// enclosed in SQL_VALUE_DELIM, as that is not
		// very practical for like statements

		// --- duplicate "'" if it occurs in string-value
		s = rs.getString(rColName);
	    
		if (s.length() > 0)
		{
			I18nString::size_type pos = s.find(SQL_VALUE_DELIM);

			while ( pos != VarString::npos )
			{
				s.insert(++pos, SQL_VALUE_DELIM);
				pos = s.find(SQL_VALUE_DELIM, ++pos);
			}
		}

		break;

	case DATETIME:
		s = rs.getDateTime(rColName).toString(basar::cmnutil::FORMAT_SQL_DATETIME_INFORMIX);
		break;

	case DATE:
		s = rs.getDate(rColName).toString(basar::cmnutil::FORMAT_SQL_DATE_INFORMIX);
		break;

	case TIME:
		s = rs.getTime(rColName).toString(basar::cmnutil::FORMAT_SQL_TIME_INFORMIX);
		break;

	default:
		{
			// this branch should never fire
			VarString msg;
			msg.format(	"An undefined type is set: property '%s'.\n"
				        "Valid types are: UNKNOWN,\n"
						"                 INT16, INT32, FLOAT32, FLOAT64, DECIMAL,\n"
						"                 STRING,\n"
						"                 DATETIME, DATE, TIME, INTDATE, INTTIME.\n",
						pdl.getName(pdl.getIndex(rColName)).c_str() ); 
			throw property::FatalErrorException( ExceptInfo( fun, msg, __FILE__, __LINE__) );	
		}
	}

	return s;
}
////////////////////////////////////////////////////////////////////////////////////////////////
/*! \return	represents the got value
  ! \param  rColName represents the attribute name which is the value returned for.
  ! \param  r is the AccessorPropertyTable_YIterator which is the getConstString()-method is executed on.
  ! \throw  no-throw */
////////////////////////////////////////////////////////////////////////////////////////////////
ConstBuffer NoCaching::getConstString( const ColumnName                      & rColName, 
									   const AccessorPropertyTable_YIterator & r ) const
{
	return r.m_pCacheController->m_Resultset.getString( rColName );
}
////////////////////////////////////////////////////////////////////////////////////////////////
/*! \return	represents the got value
  ! \param  index represents the attribute which is the value returned for.
  ! \param  r is the AccessorPropertyTable_YIterator which is the getString()-method is executed on.
  ! \throw  no-throw */
////////////////////////////////////////////////////////////////////////////////////////////////
ConstBuffer NoCaching::getConstString( const ColumnIndex                       index, 
									   const AccessorPropertyTable_YIterator & r ) const
{
	return r.m_pCacheController->m_Resultset.getString ( 
					r.m_pCacheController->getPropertyDescriptionList().getName( index ) );
}
////////////////////////////////////////////////////////////////////////////////////////////////
/*! \return	represents the got value
  ! \param  rColName represents the attribute name which is the value returned for.
  ! \param  r is the AccessorPropertyTable_YIterator which is the getString()-method is executed on.
  ! \throw  no-throw */
////////////////////////////////////////////////////////////////////////////////////////////////
const I18nString& NoCaching::getString( const ColumnName                      & rColName, 
									   const AccessorPropertyTable_YIterator & r ) const
{
    static I18nString value;

    basar::ConstBuffer strBuf = r.m_pCacheController->m_Resultset.getString( rColName );

    if ( 0 != value.compare( strBuf ) )
    {
        value.assign( strBuf );
    }
    
	return value;
}
////////////////////////////////////////////////////////////////////////////////////////////////
/*! \return	represents the got value
  ! \param  index represents the attribute which is the value returned for.
  ! \param  r is the AccessorPropertyTable_YIterator which is the getString()-method is executed on.
  ! \throw  no-throw */
////////////////////////////////////////////////////////////////////////////////////////////////
const I18nString& NoCaching::getString( const ColumnIndex                       index, 
									   const AccessorPropertyTable_YIterator & r ) const
{
	return getString( r.m_pCacheController->getPropertyDescriptionList().getName( index ), r );
}
////////////////////////////////////////////////////////////////////////////////////////////////
/*! \return	represents the got value
  ! \param  rColName represents the attribute name which is the value returned for.
  ! \param  r is the AccessorPropertyTable_YIterator which is the getFloat64()-method is executed on.
  ! \throw  no-throw */
////////////////////////////////////////////////////////////////////////////////////////////////
const Float64& NoCaching::getFloat64( const ColumnName                      & rColName, 
							   const AccessorPropertyTable_YIterator & r ) const
{
	return r.m_pCacheController->m_Resultset.getFloat64( rColName );
}
////////////////////////////////////////////////////////////////////////////////////////////////
/*! \return	represents the got value
  ! \param  index represents the attribute which is the value returned for.
  ! \param  r is the AccessorPropertyTable_YIterator which is the getFloat64()-method is executed on.
  ! \throw  no-throw */
////////////////////////////////////////////////////////////////////////////////////////////////
const Float64& NoCaching::getFloat64(const ColumnIndex                       index, 
							  const AccessorPropertyTable_YIterator & r ) const
{
	return r.m_pCacheController->m_Resultset.getFloat64 ( 
					r.m_pCacheController->getPropertyDescriptionList().getName( index ) );
}
////////////////////////////////////////////////////////////////////////////////////////////////
/*! \return	represents the got value
  ! \param  rColName represents the attribute name which is the value returned for.
  ! \param  r is the AccessorPropertyTable_YIterator which is the getFloat32()-method is executed on.
  ! \throw  no-throw */
////////////////////////////////////////////////////////////////////////////////////////////////
const Float32& NoCaching::getFloat32(	const ColumnName                      & rColName, 
								const AccessorPropertyTable_YIterator & r ) const
{
	return r.m_pCacheController->m_Resultset.getFloat32( rColName );
}
////////////////////////////////////////////////////////////////////////////////////////////////
/*! \return	represents the got value
  ! \param  index represents the attribute which is the value returned for.
  ! \param  r is the AccessorPropertyTable_YIterator which is the getFloat32()-method is executed on.
  ! \throw  no-throw */
////////////////////////////////////////////////////////////////////////////////////////////////
const Float32& NoCaching::getFloat32( const ColumnIndex                       index, 
							   const AccessorPropertyTable_YIterator & r ) const
{
	return r.m_pCacheController->m_Resultset.getFloat32 ( 
					r.m_pCacheController->getPropertyDescriptionList().getName( index ) );
}
////////////////////////////////////////////////////////////////////////////////////////////////
/*! \return	represents the got value
  ! \param  rColName represents the attribute name which is the value returned for.
  ! \param  r is the AccessorPropertyTable_YIterator which is the getDateTime()-method is executed on.
  ! \throw  no-throw */
////////////////////////////////////////////////////////////////////////////////////////////////
const DateTime& NoCaching::getDateTime( const ColumnName                      & rColName, 
									    const AccessorPropertyTable_YIterator & r ) const
{
	return r.m_pCacheController->m_Resultset.getDateTime( rColName );
}
////////////////////////////////////////////////////////////////////////////////////////////////
/*! \return	represents the got value
  ! \param  index represents the attribute which is the value returned for.
  ! \param  r is the AccessorPropertyTable_YIterator which is the getDateTime()-method is executed on.
  ! \throw  no-throw */
////////////////////////////////////////////////////////////////////////////////////////////////
const DateTime& NoCaching::getDateTime( const ColumnIndex                       index, 
									    const AccessorPropertyTable_YIterator & r ) const
{
	return r.m_pCacheController->m_Resultset.getDateTime ( 
				r.m_pCacheController->getPropertyDescriptionList().getName( index ) );
}
////////////////////////////////////////////////////////////////////////////////////////////////
/*! \return	represents the got value
  ! \param  rColName represents the attribute name which is the value returned for.
  ! \param  r is the AccessorPropertyTable_YIterator which is the getDate()-method is executed on.
  ! \throw  no-throw */
////////////////////////////////////////////////////////////////////////////////////////////////
const Date& NoCaching::getDate( const ColumnName                      & rColName, 
							    const AccessorPropertyTable_YIterator & r ) const
{
	return r.m_pCacheController->m_Resultset.getDate( rColName );
}
////////////////////////////////////////////////////////////////////////////////////////////////
/*! \return	represents the got value
  ! \param  index represents the attribute which is the value returned for.
  ! \param  r is the AccessorPropertyTable_YIterator which is the getDate()-method is executed on.
  ! \throw  no-throw */
////////////////////////////////////////////////////////////////////////////////////////////////
const Date& NoCaching::getDate( const ColumnIndex                       index, 
							    const AccessorPropertyTable_YIterator & r ) const
{
	return r.m_pCacheController->m_Resultset.getDate ( 
				r.m_pCacheController->getPropertyDescriptionList().getName( index ) );
}
////////////////////////////////////////////////////////////////////////////////////////////////
/*! \return	represents the got value
  ! \param  rColName represents the attribute name which is the value returned for.
  ! \param  r is the AccessorPropertyTable_YIterator which is the getDate()-method is executed on.
  ! \throw  no-throw */
////////////////////////////////////////////////////////////////////////////////////////////////
const Time& NoCaching::getTime( const ColumnName                      & rColName, 
							    const AccessorPropertyTable_YIterator & r ) const
{
	return r.m_pCacheController->m_Resultset.getTime( rColName ); 
}
////////////////////////////////////////////////////////////////////////////////////////////////
/*! \return	represents the got value
  ! \param  index represents the attribute which is the value returned for.
  ! \param  r is the AccessorPropertyTable_YIterator which is the getDate()-method is executed on.
  ! \throw  no-throw */
////////////////////////////////////////////////////////////////////////////////////////////////
const Time& NoCaching::getTime( const ColumnIndex                       index, 
							    const AccessorPropertyTable_YIterator & r ) const
{
	return r.m_pCacheController->m_Resultset.getTime ( 
				r.m_pCacheController->getPropertyDescriptionList().getName( index ) );
}
////////////////////////////////////////////////////////////////////////////////////////////////
/*! \return	represents the got value
  ! \param  rColName represents the attribute name which is the value returned for.
  ! \param  r is the AccessorPropertyTable_YIterator which is the getDecimal()-method is executed on.
  ! \throw  no-throw */
////////////////////////////////////////////////////////////////////////////////////////////////
const Decimal& NoCaching::getDecimal(const ColumnName                      & rColName, 
									 const AccessorPropertyTable_YIterator & r ) const
{
	return r.m_pCacheController->m_Resultset.getDecimal( rColName );
}
////////////////////////////////////////////////////////////////////////////////////////////////
/*! \return	represents the got value
  ! \param  index represents the attribute which is the value returned for.
  ! \param  r is the AccessorPropertyTable_YIterator which is the getDecimal()-method is executed on.
  ! \throw  no-throw */
////////////////////////////////////////////////////////////////////////////////////////////////
const Decimal& NoCaching::getDecimal(const ColumnIndex                       index, 
									 const AccessorPropertyTable_YIterator & r ) const
{
	return r.m_pCacheController->m_Resultset.getDecimal ( 
				r.m_pCacheController->getPropertyDescriptionList().getName( index ) );
}
////////////////////////////////////////////////////////////////////////////////////////////////
//! \throw  NoSetAllowedException
////////////////////////////////////////////////////////////////////////////////////////////////
void NoCaching::checkIsAllowedToSet() const
{
	static ConstString fun = "basar.db.ascpect.NoCaching.checkIsAllowedToSet()";
	
	throw NoSetAllowedException( ExceptInfo( fun, 
											 "with caching policy 'NO_CACHING' "
											 "it is not allowed to set any values to "
											 "PropertyTableRef!", 
											 __FILE__, __LINE__) );
}
////////////////////////////////////////////////////////////////////////////////////////////////
//! \throw  NoGetAllowedException
//! \param  r is the AccessorPropertyTable_YIterator which is even important in successor classes.
////////////////////////////////////////////////////////////////////////////////////////////////
void NoCaching::checkIsAllowedToGet(const AccessorPropertyTable_YIterator & r) const
{
	static ConstString fun = "basar.db.ascpect.NoCaching.checkIsAllowedToGet()";

	if( (static_cast<Int32>(r.m_pCacheController->m_Resultset.getFetchedRows())-1) != r.m_RowNumber )
	{
		VarString msg;
		msg.format("with caching policy 'NO_CACHING' "
					"it is not allowed to get any values, if current index of "
					"YIterator (line number <%d>) is not congruent "
					"to the number of datasets of resultset <%d>!",
					r.m_RowNumber, 
					static_cast<Int32>(r.m_pCacheController->m_Resultset.getFetchedRows()) - 1 );

		throw NoGetAllowedException( ExceptInfo( fun, msg, __FILE__, __LINE__) );
	}
}
//-------------------------------------------------------------------------------------------------------------------------//
// cachecontroller specific methods
//-------------------------------------------------------------------------------------------------------------------------//
////////////////////////////////////////////////////////////////////////////////////////////////
//! \return void 
/*! \param  toCreate determines if the internal snapshot will be created 
	(important in the context of transactions) */
//! \throw  InvalidValueException
////////////////////////////////////////////////////////////////////////////////////////////////
void NoCaching::switchCreateInternalSnapshot( bool  )
{
	static ConstString fun = "basar.db.aspect.NoCaching.switchCreateInternalSnapshot(bool)";
	
	throw InvalidValueException( ExceptInfo( fun, 
											"with caching policy 'NO_CACHING' "
											"it is not allowed to switch off/on creation of "
											"internal snapshot!", 
											__FILE__, __LINE__) );
}

////////////////////////////////////////////////////////////////////////////////////////////////
//! \return void 
//! \param	yit is the AccessorPropertyTable_YIterator which the markForDelete()-method is executed on.
//! \throw  InvalidMethodCallException
////////////////////////////////////////////////////////////////////////////////////////////////
void NoCaching::markForDelete( AccessorPropertyTable_YIterator& )
{
	static ConstString fun = "basar.db.aspect.NoCaching.markForDelete()";
	
	throw InvalidMethodCallException ( ExceptInfo( 	fun, 
													"with caching policy 'NO_CACHING' "
													"it is not allowed to call method 'markForDelete()'!", 
													__FILE__, __LINE__) );
}

AccessorPropertyTable_YIterator NoCaching::atRow( const basar::Int32 /*row*/ )
{
    static ConstString fun = "basar.db.aspect.NoCaching.atRow()";

    throw InvalidMethodCallException( ExceptInfo ( fun, "with caching policy 'NO_CACHING' it is not allowed to call method 'atRow()!", __FILE__, __LINE__ ) );
}

////////////////////////////////////////////////////////////////////////////////////////////////
//! \retval true if call for replicate() has to be skipped
//! \retval otherwise false
//! \throw  no-throw
////////////////////////////////////////////////////////////////////////////////////////////////
bool NoCaching::mustReplicateMethodTerminated() const
{
	return true;
}
////////////////////////////////////////////////////////////////////////////////////////////////
//! \return 0 or ITERATOR_ENDPOS
//! \throw  RowNoLongerAvailableException
////////////////////////////////////////////////////////////////////////////////////////////////
Int32 NoCaching::getIndex4HitResult() const
{
	ConstString fun = "basar.db.aspect.SingleRowCaching.getIndex4HitResult()";

	switch (m_pCacheController->m_Resultset.getFetchedRows())
	{
	case 1:
		return 0;
	
	case 0:
		return ITERATOR_ENDPOS;

	default:
		if (true)
		{
			throw RowNoLongerAvailableException( ExceptInfo( fun, 
															"with caching policy 'NO_CACHING' "
															"it is allowed to restart iteration only "
															"if max. one row has already been read. "
															"To check for equality to begin() "
															"use isBegin() instead!", 
															__FILE__, __LINE__) );
		}

		return 0;
	}
}
////////////////////////////////////////////////////////////////////////////////////////////////
//! \return void
//! \throw  NoFilteredBeginAllowedException
////////////////////////////////////////////////////////////////////////////////////////////////
void NoCaching::checkFilteredBeginAllowed() const
{ 
	ConstString fun = "basar.db.aspect.NoCaching.checkFilteredBeginAllowed()";

	throw NoFilteredBeginAllowedException(ExceptInfo(fun,	
													"with caching policy 'NO_CACHING' "
													"it is not allowed to iterate in a filtered manner!", 
													__FILE__, __LINE__) );
}

////////////////////////////////////////////////////////////////////////////////////////////////

} // aspect
} // db
} // basar
