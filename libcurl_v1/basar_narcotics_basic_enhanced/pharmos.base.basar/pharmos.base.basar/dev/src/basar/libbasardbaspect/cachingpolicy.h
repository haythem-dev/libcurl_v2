#ifndef GUARD_CACHING_POLICY_H
#define GUARD_CACHING_POLICY_H

//---------------------------------------------------------------------------------------------//
/*! \file
 *  \brief  internal interface for caching policy
 *  \author Bjoern Bischof
 *  \date   10.10.2006
 */
//---------------------------------------------------------------------------------------------//

//---------------------------------------------------------------------------------------------//
// includes
//---------------------------------------------------------------------------------------------//
#include "libbasardbaspect_definitions.h"

//---------------------------------------------------------------------------------------------//
// namespaces
//---------------------------------------------------------------------------------------------//
namespace basar  {
	namespace property
	{
		class PropertyTable_YIterator;
	}
namespace db	 {
namespace aspect {

//---------------------------------------------------------------------------------------------//
// forward declaration
//---------------------------------------------------------------------------------------------//
class AccessorPropertyTable_YIterator;

//---------------------------------------------------------------------------------------------//
/*! \brief	class CachingPolicy holds a AccessorPropertyTable_YIterator.
			Depending on the concrete caching policy the caching controller, 
			propertyTable_YIterator and the current row have to be updated.
	            
			final class. 
*/
//---------------------------------------------------------------------------------------------//
// class CachingPolicy
//---------------------------------------------------------------------------------------------//
class CachingPolicy
{
public:
	virtual ~CachingPolicy() = 0;
	
	//------------------------------------------------------------------------//
	// accessorpropertytable_yiterator specific methods
	//------------------------------------------------------------------------//
	/*! \brief	checks equality of the passed accessorPropertyTable-YIterator itself 
				with its begin()-Iterator of the internal CacheController. 
		\n		no-throw */
	virtual bool						    isBegin			(const AccessorPropertyTable_YIterator & r ) const;
	
	/*! \brief	checks equality of the passed accessorPropertyTable-YIterator itself 
				with its end()-Iterator of the internal CacheController. 
		\n		no-throw */
	virtual bool						    isEnd			(const AccessorPropertyTable_YIterator & r ) const; 
	
	//! \brief dflt. implementation does nothing. 
	//! \n     no-throw 
	virtual void							preIncrement	(AccessorPropertyTable_YIterator & r );
	
	//! \brief	dflt. implementation does nothing. \n no-throw 
	virtual void							postIncrement	(AccessorPropertyTable_YIterator & r );
	
	/*! \brief	the internal row number is updated dflt. implementation checks 
				if the passed accessorPropertyTable-Yiterator points to the end of its internal cachecontroller. 
		\n		throw UnknownPropertyTypeException */
	virtual bool							updateRowNumber	(const bool                        replicated,  
															 AccessorPropertyTable_YIterator & r  );
	
	//! \brief	returns the begin()-Iterator of the passed accessorPropertyTable-Yiterator . \n no-throw 
	virtual AccessorPropertyTable_XIterator	begin			(const AccessorPropertyTable_YIterator & r ) const;
	
	//! \brief	returns the end()-Iterator of the passed accessorPropertyTable-Yiterator . \n no-throw 
	virtual AccessorPropertyTable_XIterator	end				(const AccessorPropertyTable_YIterator & r ) const;

	/*! \brief	returns the begin()-Iterator of the passed accessorPropertyTable-Yiterator 
				filtered by the passed propertyStateSet. 
		\n		no-throw */
	virtual AccessorPropertyTable_XIterator	begin			(const PropertyStateSet          & state, 
															 AccessorPropertyTable_YIterator & r  );
	
	/*! \brief	returns the filter (propertyStateSet) of the internal propertyTable yiterator 
				of the passed accessorPropertyTable-Yiterator. 
		\n		no-throw */
	virtual const PropertyStateSet&			getFilter		(const AccessorPropertyTable_YIterator & r ) const;
	
	/*! \brief	returns the state(propertyStateSet) of the internal propertyTable 
				yiterator of the passed accessorPropertyTable-Yiterator. 
		\n		no-throw */
	virtual const PropertyStateSet			getState		(const AccessorPropertyTable_YIterator & r ) const;
	
	
	/*! \brief	checks if the passed accessorPropertyTable-Yiterator has a attribut named by 
				the passed colName with the given datatype having a state being in the passed propertyStateSet.  
		\n		no-throw */
	virtual bool						    contains		(const ColumnName                      & rColName, 
															 const PropertyType                      pt, 
															 const PropertyStateSet                  pss,  
															 const AccessorPropertyTable_YIterator & r  
															) const;
	
	/*! \brief	checks if the accessorPropertyTable-Yiterator has a attribut colName 
				with the given datatype passed as a pair having a state being in the passed propertyStateSet.  
		\n		no-throw */
	virtual bool						    contains		(const std::pair<ColumnName, PropertyType> & p, 
															 const PropertyStateSet                      pss,
															 const AccessorPropertyTable_YIterator     & r  
															) const;

    //! \brief	returns value of attribute named by passed colName of AccessorPropertyTable YIterator.  
    //! \n		no-throw
    virtual const Int64&	    getInt64(const ColumnName                      & rColName,
                                         const AccessorPropertyTable_YIterator & r
    ) const;

    //! \brief	returns value of attribute determinated by passed index of AccessorPropertyTable YIterator.  
    //  \n		no-throw
    virtual const Int64&	    getInt64(const ColumnIndex                       index,
                                         const AccessorPropertyTable_YIterator & r
    ) const;
	//! \brief	returns value of attribute named by passed colName of AccessorPropertyTable YIterator.  
	//! \n		no-throw
	virtual const Int32&	    getInt32		(const ColumnName                      & rColName, 
												 const AccessorPropertyTable_YIterator & r  
												) const;
	
	//! \brief	returns value of attribute determinated by passed index of AccessorPropertyTable YIterator.  
	//  \n		no-throw
	virtual const Int32&	    getInt32		(const ColumnIndex                       index, 
												 const AccessorPropertyTable_YIterator & r   
												) const;
	
	//! \brief	returns value of attribute named by passed colName of AccessorPropertyTable YIterator.  
	//! \n		no-throw
	virtual const Int16&	    getInt16		(const ColumnName                      & rColName, 
												 const AccessorPropertyTable_YIterator & r   
												) const;
	
	//! \brief	returns value of attribute determinated by passed index of AccessorPropertyTable YIterator.  
	//  \n		no-throw
	virtual const Int16&	    getInt16		(const ColumnIndex                       index, 
												 const AccessorPropertyTable_YIterator & r   
												) const;

	//! \brief	returns value of attribute named by passed colName of AccessorPropertyTable YIterator
	//!         prepared for being used in a SQL statement.  
	//! \n		no-throw
	virtual const SQLString		getSQLString	(const ColumnName                      & rColName, 
												 const AccessorPropertyTable_YIterator & r   
												) const;
	
	//! \brief	returns value of attribute named by passed colName of AccessorPropertyTable YIterator.  
	//! \n		no-throw
	virtual ConstBuffer			getConstString	(const ColumnName                      & rColName, 
												 const AccessorPropertyTable_YIterator & r   
												) const;
	
	//! \brief	returns value of attribute determinated by passed index of AccessorPropertyTable YIterator.  
	//  \n		no-throw
	virtual ConstBuffer 		getConstString	(const ColumnIndex                       index, 
												 const AccessorPropertyTable_YIterator & r   
												) const;
	
	//! \brief	returns value of attribute named by passed colName of AccessorPropertyTable YIterator.  
	//! \n		no-throw
	virtual const I18nString&	getString		(const ColumnName                      & rColName, 
												 const AccessorPropertyTable_YIterator & r   
												) const;
	
	//! \brief	returns value of attribute determinated by passed index of AccessorPropertyTable YIterator.  
	//  \n		no-throw
	virtual const I18nString&	getString		(const ColumnIndex                       index, 
												 const AccessorPropertyTable_YIterator & r   
												) const;
	
	//! \brief	returns value of attribute named by passed colName of AccessorPropertyTable YIterator.  
	//! \n		no-throw
	virtual const Float64&      getFloat64		(const ColumnName                      & rColName, 
												 const AccessorPropertyTable_YIterator & r   
												) const;
	
	//! \brief	returns value of attribute determinated by passed index of AccessorPropertyTable YIterator.  
	//  \n		no-throw
	virtual const Float64&      getFloat64		(const ColumnIndex                       index, 
												 const AccessorPropertyTable_YIterator & r   
												 ) const;
	
	//! \brief	returns value of attribute named by passed colName of AccessorPropertyTable YIterator.  
	//! \n		no-throw
	virtual const Float32&      getFloat32		(const ColumnName						& rColName, 
												 const AccessorPropertyTable_YIterator  & r   
												) const;
	
	//! \brief	returns value of attribute determinated by passed index of AccessorPropertyTable YIterator.  
	//  \n		no-throw
	virtual const Float32&      getFloat32		(const ColumnIndex                       index, 
												 const AccessorPropertyTable_YIterator & r   
												) const;
	
	//! \brief	returns value of attribute named by passed colName of AccessorPropertyTable YIterator.  
	//! \n		no-throw
	virtual const DateTime&		getDateTime		(const ColumnName                      & rColName, 
												 const AccessorPropertyTable_YIterator & r   
												) const;
	
	//! \brief	returns value of attribute determinated by passed index of AccessorPropertyTable YIterator.  
	//  \n		no-throw
	virtual const DateTime&		getDateTime		(const ColumnIndex                       index, 
												 const AccessorPropertyTable_YIterator & r   
												) const;

	//! \brief	returns value of attribute named by passed colName of AccessorPropertyTable YIterator.  
	//! \n		no-throw
	virtual const TimeSpan&		getTimeSpan		(const ColumnName                      & rColName, 
												 const AccessorPropertyTable_YIterator & r   
												) const;
	
	//! \brief	returns value of attribute determinated by passed index of AccessorPropertyTable YIterator.  
	//  \n		no-throw
	virtual const TimeSpan&		getTimeSpan		(const ColumnIndex                       index, 
												 const AccessorPropertyTable_YIterator & r   
												) const;

	//! \brief	returns value of attribute named by passed colName of AccessorPropertyTable YIterator.  
	//! \n		no-throw
	virtual const Date&			getDate			(const ColumnName                      & rColName, 
												 const AccessorPropertyTable_YIterator & r   
												) const;
	
	//! \brief	returns value of attribute determinated by passed index of AccessorPropertyTable YIterator.  
	//  \n		no-throw
	virtual const Date&			getDate			(const ColumnIndex                       index, 
												 const AccessorPropertyTable_YIterator & r   
												) const;
	
	//! \brief	returns value of attribute named by passed colName of AccessorPropertyTable YIterator.  
	//! \n		no-throw
	virtual const Time&			getTime			(const ColumnName                      & rColName, 
												 const AccessorPropertyTable_YIterator & r   
												) const;
	
	//! \brief	returns value of attribute determinated by passed index of AccessorPropertyTable YIterator.  
	//  \n		no-throw
	virtual const Time&			getTime			(const ColumnIndex                       index, 
												 const AccessorPropertyTable_YIterator & r   
												) const;

	//! \brief	returns value of attribute named by passed colName of AccessorPropertyTable YIterator.  
	//! \n		no-throw
	virtual const Decimal&		getDecimal		(const ColumnName                      & rColName, 
												 const AccessorPropertyTable_YIterator & r   
												) const;
	
	//! \brief	returns value of attribute determinated by passed index of AccessorPropertyTable YIterator.  
	//  \n		no-throw
	virtual const Decimal&		getDecimal		(const ColumnIndex                       index, 
												 const AccessorPropertyTable_YIterator & r   
												) const;
	//! \brief	returns value of attribute named by passed colName of AccessorPropertyTable YIterator.  
	//! \n		no-throw
	virtual const Binary&		getBinary		(const ColumnName                      & rColName, 
												 const AccessorPropertyTable_YIterator & r   
												) const;
	
	//! \brief	returns value of attribute determinated by passed index of AccessorPropertyTable YIterator.  
	//  \n		no-throw
	virtual const Binary&		getBinary		(const ColumnIndex                       index, 
												 const AccessorPropertyTable_YIterator & r   
												) const;

	//! \brief	dflt. implementation does nothing.  \n no-throw
	virtual void	checkIsAllowedToSet			() const;
	
	//! \brief	dflt. implementation does nothing.  \n no-throw
	virtual void	checkIsRowLongerAvailable	(const AccessorPropertyTable_YIterator & r) const;
	
	//! \brief	dflt. implementation does nothing.  \n no-throw
	virtual void	checkIsAllowedToGet			(const AccessorPropertyTable_YIterator & r  ) const;


	//--------------------------------------------------//
	// cachecontroller specific methods
	//--------------------------------------------------//
	/*! \brief	the passed boolean value controls if the internal snapshot is switched on or off. 
				It will be interpreted with transaction..  
		\n		no-throw */
	virtual void							switchCreateInternalSnapshot	(bool toCreate);

	//! \brief	returns the number of entries of the propertyTable of in the internal cacheController.  
	//! \n		no-throw
	virtual NumberRows				        size							() const;
	
	//! \brief	creates a snapshot of internal PropertyTable of CacheController. 
	//! \n		throw SnapshotNotAllowedToCreateException
	virtual const PropertyTableSnapshotRef	createSnapshot					() const;
	
	/*! \brief	sets state 'SS_UNKNOWN' to every xiterator of passed AccessorPropertyTable YIterator
		\n		throw InvalidMethodCallException */
	virtual void							setStateUnknown					(AccessorPropertyTable_YIterator & yit );

	//! \brief	sets the State 'SS_DELETE' to every xiterator of the passed accessorPropertyTable-YIterator 
	//! \n		no-throw
	virtual void							markForDelete					(AccessorPropertyTable_YIterator & yit);
	
	/*! \brief	erases range of AccessorPropertyTable YIterators from yItBegin (including) 
	            to yItEnd (excluding) of internal propertyTable of CacheController. 
		\n throw InvalidMethodCallException */
	virtual AccessorPropertyTable_YIterator	erase	(AccessorPropertyTable_YIterator & yItBegin, 
													 AccessorPropertyTable_YIterator & yItEnd 
													);
	
	/*! \brief	erases passed AccessorPropertyTable YIterator of internal propertyTable of CacheController. 
		\n throw InvalidMethodCallException */
	virtual AccessorPropertyTable_YIterator	erase	(AccessorPropertyTable_YIterator & aptYIt );

    /*! \brief	inserts a new row with type of given InsertTypeEnum in internal PropertyTable
		\n		throw InsertNotAllowedException */
	virtual AccessorPropertyTable_YIterator	insert	(InsertTypeEnum eInsertTypeEnum );

	//-----------------------------------------------------------------------------------------------------//
	// non virtual methods
	//-----------------------------------------------------------------------------------------------------//
	/*! \brief	returns an AccessorPropertyTable_YIterator pointing to the first entry of the properyTable 
	            or the resultset depending on the caching policy. 
		\n no-throw */
	AccessorPropertyTable_YIterator	beginRow	();
	
	/*! \brief	returns an filtered AccessorPropertyTable_YIterator pointing to the first entry of the 
	            properyTable or the resultset depending on the caching policy. 
		\n no-throw */
	AccessorPropertyTable_YIterator	beginRow	(const PropertyStateSet& state );
	
    virtual AccessorPropertyTable_YIterator atRow( const basar::Int32 row ) = 0;

	/*! \brief	copies the next row from the resultset to the propertyTable. \n no-throw */
	bool							replicate	(const UInt32 endPos );

protected:

	//------------------//
	// protected methods
	//------------------//
	//CachingPolicy( boost::shared_ptr< CacheController > cacheController );
	/*! \brief	protected c'tor to allow derived classes to make a instance. \n no-throw */
	CachingPolicy	(CacheController * cacheController);

	/*! \brief	checks if the filtered begin() is allowed. It depends of the current caching policy. \n no-throw */
	virtual void	checkFilteredBeginAllowed							() const;
	
	/*! \brief	checks if the replicate method has to be skipped. Dflt. implementation return false. \n no-throw */
	virtual bool	mustReplicateMethodTerminated						() const;
	
	/*! \brief	empty dflt. implementation. \n no-throw
	    \return property::PropertyTable_YIterator
	*/
	virtual property::PropertyTable_YIterator	prepareForReplicate									();
	
	/*! \brief	checks if all hits of the sql query has to be loaded in the propertyTable. It depends on the caching policy. \n no-throw */
	virtual void	checkFetchAll										(bool replicated);
	
	/*! \brief	by default it returns ITERATOR_ENDPOS (value -99) \n no-throw */
	virtual Int32	getIndex4EmptyResult	() const;
	
	/*! \brief	by default it returns 0 \n no-throw */
	virtual Int32	getIndex4HitResult		() const;
	
	/*! \brief	by default it returns ITERATOR_ENDPOS (value -99) \n no-throw */
	virtual Int32	getIndex4FilterResult	(property::PropertyTable_YIterator & yit) const;

	//------------------//
	// protected members
	//------------------//
	CacheController *	m_pCacheController;		//!< containing cachecontroller
};


//---------------------------------------------------------------------------------------------//
} // aspect
} // db
} // basar


//---------------------------------------------------------------------------------------------//
#endif // GUARD_CACHING_POLICY_H
