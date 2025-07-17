//---------------------------------------------------------------------------------------//
/*! \file
 *  \brief  base class implementation of caching policy
 *  \author Bjoern Bischof
 *  \date   10.10.2006
 */
//---------------------------------------------------------------------------------------//

//---------------------------------------------------------------------------------------//
// includes
//---------------------------------------------------------------------------------------//
#include "cachingpolicy.h"

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
CachingPolicy::CachingPolicy ( CacheController * cacheController )
							 : m_pCacheController(cacheController)
{
}

////////////////////////////////////////////////////////////////////////////////////////////////
//! \throw  no-throw
////////////////////////////////////////////////////////////////////////////////////////////////
CachingPolicy::~CachingPolicy()
{
}

////////////////////////////////////////////////////////////////////////////////////////////////
/*! \retval	true if the passed AccessorPropertyTable_YIterator represents the first iterator (begin())
  ! \retval	otherwise false
//! \param  r is the AccessorPropertyTable_YIterator which internal cachController is asked for 
			the begin()-method.
  ! \throw  no-throw */
////////////////////////////////////////////////////////////////////////////////////////////////
bool CachingPolicy::isBegin( const AccessorPropertyTable_YIterator& r ) const
{
	// for ON_DEMAND_CACHING and FULL_CACHING the regular equality comparison is used
	return r.m_pCacheController->begin() == r;
}

////////////////////////////////////////////////////////////////////////////////////////////////
/*! \retval	true if the passed AccessorPropertyTable_YIterator represents one position after 
			the last iterator (end())
  ! \retval	otherwise false
//! \param  r is the AccessorPropertyTable_YIterator which internal cachController is asked for 
			the end()-method.
  ! \throw  no-throw */
////////////////////////////////////////////////////////////////////////////////////////////////
bool CachingPolicy::isEnd(const AccessorPropertyTable_YIterator& r) const
{
	// for ON_DEMAND_CACHING and FULL_CACHING the regular equality comparison is used
	return r.m_pCacheController->end() == r;
}

////////////////////////////////////////////////////////////////////////////////////////////////
//! \throw  no-throw
//! \param  r is the AccessorPropertyTable_YIterator which is even important in successor classes.
////////////////////////////////////////////////////////////////////////////////////////////////
void CachingPolicy::preIncrement( AccessorPropertyTable_YIterator& )
{ 
}

////////////////////////////////////////////////////////////////////////////////////////////////
//! \throw  no-throw
//! \param  r is the AccessorPropertyTable_YIterator which is even important in successor classes.
////////////////////////////////////////////////////////////////////////////////////////////////
void CachingPolicy::postIncrement( AccessorPropertyTable_YIterator& )
{ // dflt. implementation
}
////////////////////////////////////////////////////////////////////////////////////////////////
/*! \retval	true if the internal row number is updated
  ! \retval	otherwise false
  ! \param  replicated is used in derived classes
  ! \param  r is the AccessorPropertyTable_YIterator which is controlled internally 
			if it is equivalent to an end()-iterator.
  ! \throw  no-throw */
////////////////////////////////////////////////////////////////////////////////////////////////
bool CachingPolicy::updateRowNumber( const bool, AccessorPropertyTable_YIterator& r)
{
	if( r.m_PtYit == r.m_pCacheController->m_PropertyTableRef.end() )
	{
		r.m_RowNumber = ITERATOR_ENDPOS;
		return true;
	}

	return false;
}
////////////////////////////////////////////////////////////////////////////////////////////////
/*! \return	AccessorPropertyTable_XIterator represents the xiterator to the first attribut 
			of the AccessorPropertyTable_YIterator.
  ! \param  r is the AccessorPropertyTable_YIterator which is the begin()-iterator returned of.
  ! \throw  no-throw */
////////////////////////////////////////////////////////////////////////////////////////////////
AccessorPropertyTable_XIterator CachingPolicy::begin( const AccessorPropertyTable_YIterator& r ) const
{
	return r.m_PtYit.begin();
}

////////////////////////////////////////////////////////////////////////////////////////////////
/*! \return	AccessorPropertyTable_XIterator represents the xiterator to the first attribut 
			having its state in the passsed PropertyStateSet.
  ! \param  r is the AccessorPropertyTable_YIterator which is the begin()-method delegated to.
  ! \param	state determines the first attribute with that state.
  ! \throw  no-throw */
////////////////////////////////////////////////////////////////////////////////////////////////
AccessorPropertyTable_XIterator CachingPolicy::begin( const PropertyStateSet          & state, 
													  AccessorPropertyTable_YIterator & r )
{
	return r.m_PtYit.begin(state);
}

////////////////////////////////////////////////////////////////////////////////////////////////
/*! \return	AccessorPropertyTable_XIterator represents the xiterator one step after the last 
			attribut of the AccessorPropertyTable_YIterator.
  ! \param  r is the AccessorPropertyTable_YIterator which is the end()-iterator returned of.
  ! \throw  no-throw */
////////////////////////////////////////////////////////////////////////////////////////////////
AccessorPropertyTable_XIterator CachingPolicy::end( const AccessorPropertyTable_YIterator& r ) const
{
	return r.m_PtYit.end();
}
////////////////////////////////////////////////////////////////////////////////////////////////
/*! \return	PropertyStateSet representing the internal filter (important for filtered iteration)
  ! \param  r is the AccessorPropertyTable_YIterator which is the getFilter()-method delegated to.
  ! \throw  no-throw */
////////////////////////////////////////////////////////////////////////////////////////////////
const PropertyStateSet& CachingPolicy::getFilter( const AccessorPropertyTable_YIterator& r ) const
{
	return r.m_PtYit.getFilter();
}
////////////////////////////////////////////////////////////////////////////////////////////////
/*! \return	PropertyStateSet representing the internal state (important for changing states).
  ! \param  r is the AccessorPropertyTable_YIterator which is the getState()-method delegated to.
  ! \throw  no-throw */
////////////////////////////////////////////////////////////////////////////////////////////////
const PropertyStateSet CachingPolicy::getState( const AccessorPropertyTable_YIterator& r ) const
{
	return r.m_PtYit.getState();
}
////////////////////////////////////////////////////////////////////////////////////////////////
/*! \retval	true if the given attribut name has the passed datatytpe and has furthermore a state
			which occurs in the passed PropertyStateSet.
  ! \retval	otherwise false
  ! \param  rColName is the attributname
  ! \param  pt is the datatype
  ! \param  pss is the set of states which is looked up in 
  ! \param  r is the AccessorPropertyTable_YIterator which is the contains()-method is executed on.
  ! \throw  no-throw */
////////////////////////////////////////////////////////////////////////////////////////////////
bool CachingPolicy::contains(	const ColumnName & rColName, 
								const PropertyType pt, 
								const PropertyStateSet pss, 
								const AccessorPropertyTable_YIterator& r ) const
{
	return r.m_PtYit.contains( rColName, pt, pss );
}
////////////////////////////////////////////////////////////////////////////////////////////////
/*! \retval	true if the given attribut name has the passed datatytpe and has furthermore a state
			which occurs in the passed PropertyStateSet.
  ! \retval	otherwise false
  ! \param  p is a pair of an attribut name and the datatype
  ! \param  pss is the set of states which is looked up in 
  ! \param  r is the AccessorPropertyTable_YIterator which is the contains()-method is executed on.
  ! \throw  no-throw */
////////////////////////////////////////////////////////////////////////////////////////////////
bool CachingPolicy::contains(	const std::pair<ColumnName, PropertyType> & p, 
								const PropertyStateSet                      pss, 
								const AccessorPropertyTable_YIterator     & r ) const
{
	return r.m_PtYit.contains( p, pss );
}
const Int64 & CachingPolicy::getInt64(const ColumnName & rColName, const AccessorPropertyTable_YIterator & r) const
{
    checkIsAllowedToGet(r);
    checkIsRowLongerAvailable(r);
    return r.m_PtYit.getInt64(rColName);
}
const Int64 & CachingPolicy::getInt64(const ColumnIndex index, const AccessorPropertyTable_YIterator & r) const
{
    checkIsAllowedToGet(r);
    checkIsRowLongerAvailable(r);
    return r.m_PtYit.getInt64(index);
}
////////////////////////////////////////////////////////////////////////////////////////////////
/*! \return	represents the got value
  ! \param  rColName represents the attribute name which is the value returned for.
  ! \param  r is the AccessorPropertyTable_YIterator which is the getInt32()-method is executed on.
  ! \throw  no-throw */
////////////////////////////////////////////////////////////////////////////////////////////////
const Int32& CachingPolicy::getInt32( const ColumnName                      & rColName, 
							   const AccessorPropertyTable_YIterator & r ) const
{
	checkIsAllowedToGet( r );
	checkIsRowLongerAvailable( r );
	return r.m_PtYit.getInt32( rColName );
}
////////////////////////////////////////////////////////////////////////////////////////////////
/*! \return	represents the got value
  ! \param  index represents the attribute which is the value returned for.
  ! \param  r is the AccessorPropertyTable_YIterator which is the getInt32()-method is executed on.
  ! \throw  no-throw */
////////////////////////////////////////////////////////////////////////////////////////////////
const Int32& CachingPolicy::getInt32( const ColumnIndex                       index, 
							   const AccessorPropertyTable_YIterator & r ) const
{
	checkIsAllowedToGet( r );
	checkIsRowLongerAvailable( r );
	return r.m_PtYit.getInt32( index );
}
////////////////////////////////////////////////////////////////////////////////////////////////
/*! \return	represents the got value
  ! \param  rColName represents the attribute name which is the value returned for.
  ! \param  r is the AccessorPropertyTable_YIterator which is the getInt16()-method is executed on.
  ! \throw  no-throw */
////////////////////////////////////////////////////////////////////////////////////////////////
const Int16& CachingPolicy::getInt16(const ColumnName                      & rColName, 
								const AccessorPropertyTable_YIterator & r ) const
{
	checkIsAllowedToGet( r );
	checkIsRowLongerAvailable( r );
	return r.m_PtYit.getInt16(rColName);
}
////////////////////////////////////////////////////////////////////////////////////////////////
/*! \return	represents the got value
  ! \param  index represents the attribute which is the value returned for.
  ! \param  r is the AccessorPropertyTable_YIterator which is the getInt16()-method is executed on.
  ! \throw  no-throw */
////////////////////////////////////////////////////////////////////////////////////////////////
const Int16& CachingPolicy::getInt16(const ColumnIndex                       index, 
							  const AccessorPropertyTable_YIterator & r ) const
{
	checkIsAllowedToGet( r );
	checkIsRowLongerAvailable( r );
	return r.m_PtYit.getInt16(index);
}
////////////////////////////////////////////////////////////////////////////////////////////////
/*! \return	represents the got value
  ! \param  rColName represents the attribute name which is the value returned for.
  ! \param  r is the AccessorPropertyTable_YIterator which is the getSQLString()-method is executed on.
  ! \throw  no-throw */
////////////////////////////////////////////////////////////////////////////////////////////////
const SQLString CachingPolicy::getSQLString( const ColumnName                      & rColName, 
											 const AccessorPropertyTable_YIterator & r ) const
{
	checkIsAllowedToGet( r );
	checkIsRowLongerAvailable( r );
	return r.m_PtYit.getSQLString( rColName );
}
////////////////////////////////////////////////////////////////////////////////////////////////
/*! \return	represents the got value
  ! \param  rColName represents the attribute name which is the value returned for.
  ! \param  r is the AccessorPropertyTable_YIterator which is the getConstString()-method is executed on.
  ! \throw  no-throw */
////////////////////////////////////////////////////////////////////////////////////////////////
ConstBuffer CachingPolicy::getConstString( const ColumnName                      & rColName, 
										   const AccessorPropertyTable_YIterator & r ) const
{
	checkIsAllowedToGet( r );
	checkIsRowLongerAvailable( r );
	return r.m_PtYit.getString(rColName).c_str();
}
////////////////////////////////////////////////////////////////////////////////////////////////
/*! \return	represents the got value
  ! \param  index represents the attribute which is the value returned for.
  ! \param  r is the AccessorPropertyTable_YIterator which is the getString()-method is executed on.
  ! \throw  no-throw */
////////////////////////////////////////////////////////////////////////////////////////////////
ConstBuffer CachingPolicy::getConstString( const ColumnIndex                       index, 
										   const AccessorPropertyTable_YIterator & r ) const
{
	checkIsAllowedToGet( r );
	checkIsRowLongerAvailable( r );
	return r.m_PtYit.getString(index).c_str();
}
////////////////////////////////////////////////////////////////////////////////////////////////
/*! \return	represents the got value
  ! \param  rColName represents the attribute name which is the value returned for.
  ! \param  r is the AccessorPropertyTable_YIterator which is the getString()-method is executed on.
  ! \throw  no-throw */
////////////////////////////////////////////////////////////////////////////////////////////////
const I18nString& CachingPolicy::getString( const ColumnName                      & rColName, 
										   const AccessorPropertyTable_YIterator & r ) const
{
	checkIsAllowedToGet( r );
	checkIsRowLongerAvailable( r );
	return r.m_PtYit.getString(rColName);
}
////////////////////////////////////////////////////////////////////////////////////////////////
/*! \return	represents the got value
  ! \param  index represents the attribute which is the value returned for.
  ! \param  r is the AccessorPropertyTable_YIterator which is the getString()-method is executed on.
  ! \throw  no-throw */
////////////////////////////////////////////////////////////////////////////////////////////////
const I18nString& CachingPolicy::getString( const ColumnIndex                       index, 
										   const AccessorPropertyTable_YIterator & r ) const
{
	checkIsAllowedToGet( r );
	checkIsRowLongerAvailable( r );
	return r.m_PtYit.getString(index);
}
////////////////////////////////////////////////////////////////////////////////////////////////
/*! \return	represents the got value
  ! \param  rColName represents the attribute name which is the value returned for.
  ! \param  r is the AccessorPropertyTable_YIterator which is the getFloat64()-method is executed on.
  ! \throw  no-throw */
////////////////////////////////////////////////////////////////////////////////////////////////
const Float64& CachingPolicy::getFloat64( const ColumnName                      & rColName,
								   const AccessorPropertyTable_YIterator & r ) const
{
	checkIsAllowedToGet( r );
	checkIsRowLongerAvailable( r );
	return r.m_PtYit.getFloat64(rColName);
}
////////////////////////////////////////////////////////////////////////////////////////////////
/*! \return	represents the got value
  ! \param  index represents the attribute which is the value returned for.
  ! \param  r is the AccessorPropertyTable_YIterator which is the getFloat64()-method is executed on.
  ! \throw  no-throw */
////////////////////////////////////////////////////////////////////////////////////////////////
const Float64& CachingPolicy::getFloat64(const ColumnIndex                       index, 
								  const AccessorPropertyTable_YIterator & r ) const
{
	checkIsAllowedToGet( r );
	checkIsRowLongerAvailable( r );
	return r.m_PtYit.getFloat64(index);
}
////////////////////////////////////////////////////////////////////////////////////////////////
/*! \return	represents the got value
  ! \param  rColName represents the attribute name which is the value returned for.
  ! \param  r is the AccessorPropertyTable_YIterator which is the getFloat32()-method is executed on.
  ! \throw  no-throw */
////////////////////////////////////////////////////////////////////////////////////////////////
const Float32& CachingPolicy::getFloat32( const ColumnName                      & rColName, 
								   const AccessorPropertyTable_YIterator & r ) const
{
	checkIsAllowedToGet( r );
	checkIsRowLongerAvailable( r );
	return r.m_PtYit.getFloat32(rColName);
}
////////////////////////////////////////////////////////////////////////////////////////////////
/*! \return	represents the got value
  ! \param  index represents the attribute which is the value returned for.
  ! \param  r is the AccessorPropertyTable_YIterator which is the getFloat32()-method is executed on.
  ! \throw  no-throw */
////////////////////////////////////////////////////////////////////////////////////////////////
const Float32& CachingPolicy::getFloat32( const ColumnIndex                       index, 
								   const AccessorPropertyTable_YIterator & r ) const
{
	checkIsAllowedToGet( r );
	checkIsRowLongerAvailable( r );
	return r.m_PtYit.getFloat32(index);
}
////////////////////////////////////////////////////////////////////////////////////////////////
/*! \return	represents the got value
  ! \param  rColName represents the attribute name which is the value returned for.
  ! \param  r is the AccessorPropertyTable_YIterator which is the getDateTime()-method is executed on.
  ! \throw  no-throw */
////////////////////////////////////////////////////////////////////////////////////////////////
const DateTime& CachingPolicy::getDateTime( const ColumnName                      & rColName, 
										    const AccessorPropertyTable_YIterator & r ) const
{
	checkIsAllowedToGet( r );
	checkIsRowLongerAvailable( r );
	return r.m_PtYit.getDateTime( rColName );
}
////////////////////////////////////////////////////////////////////////////////////////////////
/*! \return	represents the got value
  ! \param  index represents the attribute which is the value returned for.
  ! \param  r is the AccessorPropertyTable_YIterator which is the getDateTime()-method is executed on.
  ! \throw  no-throw */
////////////////////////////////////////////////////////////////////////////////////////////////
const DateTime& CachingPolicy::getDateTime( const ColumnIndex                       index, 
										    const AccessorPropertyTable_YIterator & r ) const
{
	checkIsAllowedToGet( r );
	checkIsRowLongerAvailable( r );
	return r.m_PtYit.getDateTime( index );
}
////////////////////////////////////////////////////////////////////////////////////////////////
/*! \return	represents the got value
  ! \param  rColName represents the attribute name which is the value returned for.
  ! \param  r is the AccessorPropertyTable_YIterator which is the getTimeSpan()-method is executed on.
  ! \throw  no-throw */
////////////////////////////////////////////////////////////////////////////////////////////////
const TimeSpan& CachingPolicy::getTimeSpan( const ColumnName                      & rColName, 
										    const AccessorPropertyTable_YIterator & r ) const
{
	checkIsAllowedToGet( r );
	checkIsRowLongerAvailable( r );
	return r.m_PtYit.getTimeSpan( rColName );
}
////////////////////////////////////////////////////////////////////////////////////////////////
/*! \return	represents the got value
  ! \param  index represents the attribute which is the value returned for.
  ! \param  r is the AccessorPropertyTable_YIterator which is the getTimeSpan()-method is executed on.
  ! \throw  no-throw */
////////////////////////////////////////////////////////////////////////////////////////////////
const TimeSpan& CachingPolicy::getTimeSpan( const ColumnIndex                       index, 
										    const AccessorPropertyTable_YIterator & r ) const
{
	checkIsAllowedToGet( r );
	checkIsRowLongerAvailable( r );
	return r.m_PtYit.getTimeSpan( index );
}
////////////////////////////////////////////////////////////////////////////////////////////////
/*! \return	represents the got value
  ! \param  rColName represents the attribute name which is the value returned for.
  ! \param  r is the AccessorPropertyTable_YIterator which is the getDate()-method is executed on.
  ! \throw  no-throw */
////////////////////////////////////////////////////////////////////////////////////////////////
const Date& CachingPolicy::getDate( const ColumnName                      & rColName, 
								    const AccessorPropertyTable_YIterator & r ) const
{
	checkIsAllowedToGet( r );
	checkIsRowLongerAvailable( r );

	return r.m_PtYit.getDate( rColName );
}
////////////////////////////////////////////////////////////////////////////////////////////////
/*! \return	represents the got value
  ! \param  index represents the attribute which is the value returned for.
  ! \param  r is the AccessorPropertyTable_YIterator which is the getDate()-method is executed on.
  ! \throw  no-throw */
////////////////////////////////////////////////////////////////////////////////////////////////
const Date& CachingPolicy::getDate( const ColumnIndex                       index, 
								    const AccessorPropertyTable_YIterator & r ) const
{
	checkIsAllowedToGet( r );
	checkIsRowLongerAvailable( r );

	return r.m_PtYit.getDate( index );
}
////////////////////////////////////////////////////////////////////////////////////////////////
/*! \return	represents the got value
  ! \param  rColName represents the attribute name which is the value returned for.
  ! \param  r is the AccessorPropertyTable_YIterator which is the getDate()-method is executed on.
  ! \throw  no-throw */
////////////////////////////////////////////////////////////////////////////////////////////////
const Time& CachingPolicy::getTime( const ColumnName                      & rColName, 
								    const AccessorPropertyTable_YIterator & r ) const
{
	checkIsAllowedToGet( r );
	checkIsRowLongerAvailable( r );

	return r.m_PtYit.getTime( rColName );
}
////////////////////////////////////////////////////////////////////////////////////////////////
/*! \return	represents the got value
  ! \param  index represents the attribute which is the value returned for.
  ! \param  r is the AccessorPropertyTable_YIterator which is the getDate()-method is executed on.
  ! \throw  no-throw */
////////////////////////////////////////////////////////////////////////////////////////////////
const Time& CachingPolicy::getTime( const ColumnIndex                       index, 
								    const AccessorPropertyTable_YIterator & r ) const
{
	checkIsAllowedToGet( r );
	checkIsRowLongerAvailable( r );

	return r.m_PtYit.getTime( index );
}
////////////////////////////////////////////////////////////////////////////////////////////////
/*! \return	represents the got value
  ! \param  rColName represents the attribute name which is the value returned for.
  ! \param  r is the AccessorPropertyTable_YIterator which is the getDecimal()-method is executed on.
  ! \throw  no-throw */
////////////////////////////////////////////////////////////////////////////////////////////////
const Decimal& CachingPolicy::getDecimal( const ColumnName                      & rColName, 
										  const AccessorPropertyTable_YIterator & r ) const
{
	checkIsAllowedToGet( r );
	checkIsRowLongerAvailable( r );
	return r.m_PtYit.getDecimal(rColName);
}
////////////////////////////////////////////////////////////////////////////////////////////////
/*! \return	represents the got value
  ! \param  index represents the attribute which is the value returned for.
  ! \param  r is the AccessorPropertyTable_YIterator which is the getDecimal()-method is executed on.
  ! \throw  no-throw */
////////////////////////////////////////////////////////////////////////////////////////////////
const Decimal& CachingPolicy::getDecimal( const ColumnIndex                       index, 
										  const AccessorPropertyTable_YIterator & r ) const
{
	checkIsAllowedToGet( r );
	checkIsRowLongerAvailable( r );
	return r.m_PtYit.getDecimal(index);
}
////////////////////////////////////////////////////////////////////////////////////////////////
/*! \return	represents the got value
  ! \param  rColName represents the attribute which is the value returned for.
  ! \param  r is the AccessorPropertyTable_YIterator which is the getBinary()-method is executed on.
  ! \throw  no-throw */
////////////////////////////////////////////////////////////////////////////////////////////////
const Binary & CachingPolicy::getBinary(const ColumnName & rColName, const AccessorPropertyTable_YIterator & r) const
{
    checkIsAllowedToGet( r );
    checkIsRowLongerAvailable( r );
    return r.m_PtYit.getBinary(rColName);
}
////////////////////////////////////////////////////////////////////////////////////////////////
/*! \return	represents the got value
  ! \param  index represents the attribute which is the value returned for.
  ! \param  r is the AccessorPropertyTable_YIterator which is the getBinary()-method is executed on.
  ! \throw  no-throw */
////////////////////////////////////////////////////////////////////////////////////////////////
const Binary & CachingPolicy::getBinary(const ColumnIndex index, const AccessorPropertyTable_YIterator & r) const
{
    checkIsAllowedToGet( r );
    checkIsRowLongerAvailable( r );
    return r.m_PtYit.getBinary(index);
}
  ////////////////////////////////////////////////////////////////////////////////////////////////
//! \throw  no-throw
////////////////////////////////////////////////////////////////////////////////////////////////
void CachingPolicy::checkIsAllowedToSet() const
{ 
	// empty dflt. implementation
}

////////////////////////////////////////////////////////////////////////////////////////////////
//! \throw  no-throw
//! \param  r is the AccessorPropertyTable_YIterator which is even important in successor classes.
////////////////////////////////////////////////////////////////////////////////////////////////
void CachingPolicy::checkIsRowLongerAvailable( const AccessorPropertyTable_YIterator & ) const
{ 
	// empty dflt. implementation
}

////////////////////////////////////////////////////////////////////////////////////////////////
//! \throw  no-throw
//! \param  r is the AccessorPropertyTable_YIterator which is even important in successor classes.
////////////////////////////////////////////////////////////////////////////////////////////////
void CachingPolicy::checkIsAllowedToGet( const AccessorPropertyTable_YIterator & ) const
{ 
	// empty dflt. implementation
}
//---------------------------------------------------------------------------------------------------------------------//
// cachecontroller specific methods
//---------------------------------------------------------------------------------------------------------------------//
////////////////////////////////////////////////////////////////////////////////////////////////
//! \return void 
/*! \param  toCreate determines if the internal snapshot will be created 
	(important in the context of transactions) */
//! \throw  no-throw
////////////////////////////////////////////////////////////////////////////////////////////////
void CachingPolicy::switchCreateInternalSnapshot( bool toCreate )
{
	m_pCacheController->m_createInternalSnapshot = toCreate;
}
////////////////////////////////////////////////////////////////////////////////////////////////
//! \return the number of the current propertytable
//! \throw  no-throw
////////////////////////////////////////////////////////////////////////////////////////////////
NumberRows CachingPolicy::size() const
{
	return m_pCacheController->m_PropertyTableRef.size(); 
}
////////////////////////////////////////////////////////////////////////////////////////////////
//! \return PropertyTableSnapshotRef representing the newly created snapshot of the propertytable
//! \throw  SnapshotNotAllowedToCreateException
////////////////////////////////////////////////////////////////////////////////////////////////
const PropertyTableSnapshotRef CachingPolicy::createSnapshot() const
{
	static ConstString func = "basar.db.aspect.CachingPolicy.createSnapshot()";

	if (true)
	{
		VarString msg;
		msg.format("snapshot is not allowed to be created with caching policy '%d'!", 
			       m_pCacheController->getCachingPolicy());

		throw SnapshotNotAllowedToCreateException( ExceptInfo(func, msg, __FILE__, __LINE__) );
	}

	return PropertyTableSnapshotRef();
}

////////////////////////////////////////////////////////////////////////////////////////////////
//! \param	yit is the AccessorPropertyTable_YIterator which the setStateUnknown()-method is executed on.
//! \throw  no-throw
//!
////////////////////////////////////////////////////////////////////////////////////////////////
void CachingPolicy::setStateUnknown( AccessorPropertyTable_YIterator &)
{
	static ConstString func = "basar.db.aspect.CachingPolicy.setStateUnknown()";

	VarString msg;
	msg.format("with caching policy '%s' it is not allowed to call method 'setStateUnknown()'!", 
               m_pCacheController->getCachingPolicy());

	throw InvalidMethodCallException ( ExceptInfo( func, msg, __FILE__, __LINE__) );
}

////////////////////////////////////////////////////////////////////////////////////////////////
//! \return void 
//! \param	yit is the AccessorPropertyTable_YIterator which the markForDelete()-method is executed on.
//! \throw  no-throw
////////////////////////////////////////////////////////////////////////////////////////////////
void CachingPolicy::markForDelete( AccessorPropertyTable_YIterator & yit )
{
	m_pCacheController->m_PropertyTableRef.markForDelete( yit.m_PtYit );
}
////////////////////////////////////////////////////////////////////////////////////////////////
//! \return AccessorPropertyTable_YIterator represents one dataset after the erased one.
//! \param	yItBegin is the first dataset of the interval that has to be deleted
//! \param	yItEnd is the iterator one beyond the last erased one
//! \throw  no-throw
////////////////////////////////////////////////////////////////////////////////////////////////
AccessorPropertyTable_YIterator CachingPolicy::erase( AccessorPropertyTable_YIterator &, 
													  AccessorPropertyTable_YIterator &)
{
	static ConstString fun = "basar.db.aspect.CachingPolicy.erase(range)";

	if (true)
	{
		VarString msg;
		msg.format("with caching policy '%d' it is not allowed to call method 'erase()'!", 
			       m_pCacheController->getCachingPolicy());
		throw InvalidMethodCallException ( ExceptInfo( 	fun, 
														msg,
														__FILE__, __LINE__ ) );
	}

	return AccessorPropertyTable_YIterator();
}

////////////////////////////////////////////////////////////////////////////////////////////////
//! \return AccessorPropertyTable_YIterator represents one dataset after the erased one.
//! \param	aptYIt is the dataset that has to be deleted from the propertytable
//! \throw  no-throw
////////////////////////////////////////////////////////////////////////////////////////////////
AccessorPropertyTable_YIterator CachingPolicy::erase( AccessorPropertyTable_YIterator &)
{
	static ConstString fun = "basar.db.aspect.CachingPolicy.erase(yit)";

	if (true)
	{
		VarString msg;
		msg.format("with caching policy '%d' it is not allowed to call method 'erase()'!", 
			       m_pCacheController->getCachingPolicy());
		throw InvalidMethodCallException ( ExceptInfo( 	fun, 
														msg,
														__FILE__, __LINE__ ) );
	}

	return AccessorPropertyTable_YIterator();
}

////////////////////////////////////////////////////////////////////////////////////////////////
//! \return AccessorPropertyTable_YIterator represents an iterator that points to newly inserted dataset.
//! \param	eInsertTypeEnum determines the kind of insert. Important for state transitions.
//! \throw  no-throw
////////////////////////////////////////////////////////////////////////////////////////////////
AccessorPropertyTable_YIterator CachingPolicy::insert( InsertTypeEnum )
{
	ConstString fun = "basar.db.aspect.CachingPolicy.insert()";

	if (true)
	{
		VarString msg;
		msg.format("with caching policy '%d' inserts are not allowed at all!", 
			       m_pCacheController->getCachingPolicy());
		throw InsertNotAllowedException( ExceptInfo(fun, msg, __FILE__, __LINE__) );
	}

	return AccessorPropertyTable_YIterator();
}

////////////////////////////////////////////////////////////////////////////////////////////////
//! \throw  no-throw
//!
////////////////////////////////////////////////////////////////////////////////////////////////
void CachingPolicy::checkFilteredBeginAllowed() const
{ 
	// empty dflt. implementation
}
////////////////////////////////////////////////////////////////////////////////////////////////
/*! \return AccessorPropertyTable_YIterator	pointing to the first dataset with right state
			in the result amount (propertyTable or resultset) */
//! \param	state determines the first attribute with that state.
//! \throw  no-throw
////////////////////////////////////////////////////////////////////////////////////////////////
AccessorPropertyTable_YIterator	CachingPolicy::beginRow( const PropertyStateSet& state )
{
	checkFilteredBeginAllowed(); // protected virtual method invocation

	Int32 index = ITERATOR_ENDPOS;
	property::PropertyTable_YIterator yItPropTab = m_pCacheController->m_PropertyTableRef.begin( state ); 
	
	if( m_pCacheController->m_Resultset.isNull() )
		index = getIndex4FilterResult( yItPropTab ); // protected virtual method invocation
	else
	{
		//	attention: the caching policy has not been checked here 
		//	because it is checked within the method 'this->begin()' 
		//	and this method 'this->begin()' has been called implicitly 
		//	in the method 'Accessor::execute' yet.	
		if ( yItPropTab == m_pCacheController->m_PropertyTableRef.end() )
		{
			if ( state.contains( SS_CLEAN ) && ( ! m_pCacheController->m_Resultset.isAfterLast()) )
			{
				replicate( m_pCacheController->m_Resultset.getFetchedRows() );
				index = static_cast<Int32>( m_pCacheController->m_Resultset.getFetchedRows() - 1 ); 
			}	
		}
		else
		{
			//	the return value of the method 'getFilteredStep()' indicates here the current row number,
			//	because this method counts the current row numbers from the beginning
			index = static_cast<Int32>( yItPropTab.getFilteredStep() ); 
		} 
	} 

	return AccessorPropertyTable_YIterator( m_pCacheController->getThis(), 
											yItPropTab, 
											index );
}

////////////////////////////////////////////////////////////////////////////////////////////////
/*! \return AccessorPropertyTable_YIterator	pointing to the first dataset in the result amount 
			(propertyTable or resultset) */
//! \throw  no-throw
////////////////////////////////////////////////////////////////////////////////////////////////
AccessorPropertyTable_YIterator CachingPolicy::beginRow()
{
	Int32 index		 = 0;
	bool  replicated = false;

	if( m_pCacheController->m_Resultset.isNull() )
		index = getIndex4EmptyResult(); // virtual method invocation
	else
	{
		if( m_pCacheController->m_Resultset.isBeforeFirst() )
		{		
			replicated = replicate( index );		
			checkFetchAll( replicated ); // virtual method invocation
			
			if( ! replicated )
				index = ITERATOR_ENDPOS;	// there is no row to fetch
		} 
		else 
			index = getIndex4HitResult(); // virtual method invocation
	} 

	return AccessorPropertyTable_YIterator( m_pCacheController->getThis(), 
											m_pCacheController->m_PropertyTableRef.begin(), 
											index );
}

////////////////////////////////////////////////////////////////////////////////////////////////
/*! \retval true if a new datatset is pushed to the propertytable or for the further logic 
			it is important to assume that. */
//!	\retval otherwise false
//!	\param	endPos determines the number of copied datasets
//! \throw  no-throw
////////////////////////////////////////////////////////////////////////////////////////////////
bool CachingPolicy::replicate( const UInt32 endPos )
{
	static ConstString func = "basar.db.aspect.CacheController.replicate()";

	// extracted from loops because of performance
	property::PropertyTable_YIterator                         yIt;
	property::PropertyTable_XIterator                         xIt;
	property::PropertyTable_XIterator                         yItEnd;
	std::vector<CacheController::IndexStruct>::iterator       itIdx;
	std::vector<CacheController::IndexStruct>::const_iterator itIdxEnd;
	CacheController::IndexStruct                              idx;
	basar::property::ColumnName                               colName;

	while( endPos >= m_pCacheController->m_Resultset.getFetchedRows() )
	{
		if ( !m_pCacheController->m_Resultset.next() ){
			return false;
		}
		
		if ( mustReplicateMethodTerminated() ){ // virtual method invocation
			return true;
		}

        //for single row caching: maybe don't clear proptab, but reset yit to begin -> problem: states should be CLEAN
		

		yIt = prepareForReplicate(); // virtual method invocation
		
		if ( m_pCacheController->m_Index.empty() )
		{
			yItEnd = yIt.end();

			for (xIt = yIt.begin(); xIt != yItEnd; ++xIt )
			{
				idx.type     = xIt.getType();
				idx.property = xIt.getIndex();

                colName = xIt.getName();

				if ( m_pCacheController->m_Resultset.isExist(colName))
				{
					idx.resultset = m_pCacheController->m_Resultset.getIndex(colName);
					m_pCacheController->m_Index.push_back(idx);

					switch (idx.type.getType())
					{
                    case INT64:
                        xIt.setInt64(m_pCacheController->m_Resultset.getInt64(idx.resultset));
                        break;

					case INT32:
					case INTDATE:
					case INTTIME:
						xIt.setInt32(m_pCacheController->m_Resultset.getInt32(idx.resultset)); 
						break;

					case INT16:
						xIt.setInt16(m_pCacheController->m_Resultset.getInt16(idx.resultset));
						break;

					case FLOAT64:
						xIt.setFloat64(m_pCacheController->m_Resultset.getFloat64(idx.resultset));
						break;
					
					case FLOAT32:
						xIt.setFloat32(m_pCacheController->m_Resultset.getFloat32(idx.resultset));
						break;

					case DECIMAL:
						xIt.setDecimal(m_pCacheController->m_Resultset.getDecimal(idx.resultset));
						break;
					
					case STRING:
						xIt.setString(m_pCacheController->m_Resultset.getString(idx.resultset));
						break;
					
					case DATETIME:
						xIt.setDateTime( m_pCacheController->m_Resultset.getDateTime(idx.resultset) );
						break;

					case TIMESPAN:
						xIt.setTimeSpan( m_pCacheController->m_Resultset.getTimeSpan(idx.resultset) );
						break;					

					case DATE:
						xIt.setDate( m_pCacheController->m_Resultset.getDate(idx.resultset) );
						break;
					
					case TIME:
						xIt.setTime( m_pCacheController->m_Resultset.getTime(idx.resultset) );
						break;
					
                    case BINARY:
                        xIt.setBinary( m_pCacheController->m_Resultset.getBinary(idx.resultset) );
                        break;
					default:
						{			
							VarString msg;
							msg.format("PropertyType <%d> is unknown.", idx.type.getType());
							throw UnknownPropertyTypeException(ExceptInfo( func,
											                               msg, 
																		   __FILE__, __LINE__));
						}
					}	// switch
				}	// if
			}	// for
		}	// if
		else
		{
			itIdxEnd = m_pCacheController->m_Index.end();

			for (itIdx = m_pCacheController->m_Index.begin(); itIdx != itIdxEnd; ++itIdx)
			{
				switch ( itIdx->type.getType() )
				{
                case INT64:
                    yIt.setInt64(itIdx->property, m_pCacheController->m_Resultset.getInt64(itIdx->resultset));
                    break;

				case INT32:
				case INTDATE:
				case INTTIME:
					yIt.setInt32( itIdx->property, 
								  m_pCacheController->m_Resultset.getInt32(itIdx->resultset) ); 
					break;
				
				case INT16:
					yIt.setInt16(itIdx->property, 
								 m_pCacheController->m_Resultset.getInt16(itIdx->resultset));
					break;
				
				case FLOAT64:
					yIt.setFloat64(itIdx->property, 
								   m_pCacheController->m_Resultset.getFloat64(itIdx->resultset));
					break;
				
				case FLOAT32:
					yIt.setFloat32(itIdx->property, 
								   m_pCacheController->m_Resultset.getFloat32(itIdx->resultset));
					break;
				
				case DECIMAL:
					yIt.setDecimal(itIdx->property, 
						           m_pCacheController->m_Resultset.getDecimal(itIdx->resultset));
					break;
				
				case STRING:
					yIt.setString(itIdx->property, 
						          m_pCacheController->m_Resultset.getString(itIdx->resultset));
					break;
				
				case DATETIME:
					yIt.setDateTime(itIdx->property, 
						            m_pCacheController->m_Resultset.getDateTime(itIdx->resultset));
					break;

				case TIMESPAN:
					yIt.setTimeSpan(itIdx->property, 
						        m_pCacheController->m_Resultset.getTimeSpan(itIdx->resultset)); 
					break;

				case DATE:
					yIt.setDate(itIdx->property, 
						        m_pCacheController->m_Resultset.getDate(itIdx->resultset));
					break;
				
				case TIME:
					yIt.setTime(itIdx->property, 
						        m_pCacheController->m_Resultset.getTime(itIdx->resultset)); 
					break;
				
				default:
					{
						VarString msg;
						msg.format("The PropertyType <%d> is unknown.", itIdx->type.getType());
						throw UnknownPropertyTypeException(ExceptInfo( func, 
											                           msg, 
																	   __FILE__, __LINE__));
					}
				}	// switch
			}	// for
		}	// if
	}
	
	return true;
}
////////////////////////////////////////////////////////////////////////////////////////////////
//! \retval always false
//! \throw  no-throw
////////////////////////////////////////////////////////////////////////////////////////////////
bool CachingPolicy::mustReplicateMethodTerminated() const
{
	return false;
}

////////////////////////////////////////////////////////////////////////////////////////////////
//! \throw  no-throw
//!
////////////////////////////////////////////////////////////////////////////////////////////////
property::PropertyTable_YIterator CachingPolicy::prepareForReplicate()
{
    return m_pCacheController->m_PropertyTableRef.insert( FOR_CLEAN ); 
}

////////////////////////////////////////////////////////////////////////////////////////////////
//! \param	replicated is used in successor classes
//! \throw  no-throw
//!
////////////////////////////////////////////////////////////////////////////////////////////////
void CachingPolicy::checkFetchAll( bool )
{ 
}

////////////////////////////////////////////////////////////////////////////////////////////////
//! \return the numeric value of the end()-Iterator
//! \throw  no-throw
//!
////////////////////////////////////////////////////////////////////////////////////////////////
Int32 CachingPolicy::getIndex4EmptyResult() const
{
	return ITERATOR_ENDPOS;
}

////////////////////////////////////////////////////////////////////////////////////////////////
//! \return alyways 0
//! \throw  no-throw
//!
////////////////////////////////////////////////////////////////////////////////////////////////
Int32 CachingPolicy::getIndex4HitResult() const
{
	return 0;
}

////////////////////////////////////////////////////////////////////////////////////////////////
/*! \return the row number */
//!	\param	yit is used in successor classes
//! \throw  no-throw
////////////////////////////////////////////////////////////////////////////////////////////////
Int32 CachingPolicy::getIndex4FilterResult( property::PropertyTable_YIterator &) const
{
	return ITERATOR_ENDPOS;
}

//---------------------------------------------------------------------------------------------
} // aspect
} // db
} // basar
