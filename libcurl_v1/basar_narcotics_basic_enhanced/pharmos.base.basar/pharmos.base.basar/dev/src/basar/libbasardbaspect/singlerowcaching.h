#ifndef GUARD_SINGLE_ROW_CACHING_H
#define GUARD_SINGLE_ROW_CACHING_H

//---------------------------------------------------------------------------------------------//
/*! \file
 *  \brief  internal implementation for the caching policy 'single row caching'
 *  \author Bjoern Bischof
 *  \date   10.10.2006
 */
//---------------------------------------------------------------------------------------------//

//---------------------------------------------------------------------------------------------//
// inclues
//---------------------------------------------------------------------------------------------//
#include "cachingpolicy.h"


//---------------------------------------------------------------------------------------------//
// namespaces
//---------------------------------------------------------------------------------------------//
namespace basar  {
namespace db	 {
namespace aspect {

//---------------------------------------------------------------------------------------------//
/*! \brief  class SingleRowCaching holds a AccessorPropertyTable_YIterator.
			Depending on the concrete caching policy the caching controller, propertyTable_YIterator
			and the current row have to be updated.
            
			This is a final class.     
			Class throws the follwing exceptions: 
			class ??? */
//---------------------------------------------------------------------------------------------//
// class SingleRowCaching
//---------------------------------------------------------------------------------------------//
class SingleRowCaching : public CachingPolicy
{
public:
	SingleRowCaching(CacheController* cacheController  );

	virtual ~SingleRowCaching();

	//--------------------------------------------------//
	// accessorpropertytable_yiterator specific methods
	//--------------------------------------------------//

	/*! \brief	checks equality of the passed accessorPropertyTable-YIterator itself 
				with its begin()-Iterator of the internal CacheController. \n no-throw */
	virtual bool							    isBegin(					const AccessorPropertyTable_YIterator& r ) const;

	/*! \brief	checks equality of the passed accessorPropertyTable-YIterator itself 
				with its end()-Iterator of the internal CacheController. \n no-throw */
	virtual bool							    isEnd(						const AccessorPropertyTable_YIterator& r ) const;


	/*! \brief	the internal row number is updated dflt. implementation checks 
				if the passed accessorPropertyTable-Yiterator points to the end of its internal cachecontroller. 
				\n throw UnknownPropertyTypeException */
	virtual bool								updateRowNumber(			const bool replicated, AccessorPropertyTable_YIterator& r );

	//! \brief	dflt. implementation does nothing. \n no-throw 
	virtual void								preIncrement(				AccessorPropertyTable_YIterator& r );

	/*! \brief	returns the filter (propertyStateSet) of the internal propertyTable yiterator 
				of the passed accessorPropertyTable-Yiterator. \n throw NoFilterAvailableException*/
	virtual const PropertyStateSet&				getFilter(					const AccessorPropertyTable_YIterator& r ) const;

	
	/*! \brief	returns the state(propertyStateSet) of the internal propertyTable 
				yiterator of the passed accessorPropertyTable-Yiterator. \n throw NoStateAvailableException*/
	virtual const PropertyStateSet				getState(					const AccessorPropertyTable_YIterator& r ) const;

	/*! \brief	checks if the passed accessorPropertyTable-Yiterator has a attribut named by 
				the passed colName with the given datatype having a state being in the passed propertyStateSet.  
				\n throw NoStateAvailableException*/
	virtual bool							    contains(					const ColumnName & rColName, 
																			const PropertyType pt, 
																			const PropertyStateSet pss, 
																			const AccessorPropertyTable_YIterator& r ) const;

	/*! \brief	checks if the accessorPropertyTable-Yiterator has a attribut colName 
				with the given datatype passed as a pair having a state being in the passed propertyStateSet.  
				\n throw NoStateAvailableException*/
	virtual bool							    contains(					const std::pair<ColumnName, PropertyType> & p,
																			const PropertyStateSet                      pss, 
																			const AccessorPropertyTable_YIterator     & r ) const;

	//! \brief	dflt. implementation does nothing.  \n throw RowNoLongerAvailableException
	virtual void								checkIsRowLongerAvailable(	const AccessorPropertyTable_YIterator& r ) const;


	//--------------------------------------------------//
	// cachecontroller specific methods
	//--------------------------------------------------//
	/*! \brief	the passed boolean value controls if the internal snapshot is switched on or off. 
				It will be interpreted with transaction..  \n throw InvalidValueException*/
	virtual void								switchCreateInternalSnapshot( bool toCreate );

    virtual AccessorPropertyTable_YIterator atRow( const basar::Int32 row );

protected:
	//-------------------------------------------------------------------//
	// virtual methods
	//-------------------------------------------------------------------//
	
	/*! \brief	returns the current row. \n no-throw */
	virtual property::PropertyTable_YIterator								prepareForReplicate();
	
	/*! \brief	returns the value for the current row. \n throw RowNoLongerAvailableException */
	virtual Int32								getIndex4HitResult() const;
	
	/*! \brief	 returns always an exception, because filtered begin() is not allowed at all.
				\n throw NoFilteredBeginAllowedException */
	virtual void								checkFilteredBeginAllowed() const;
};


} // aspect
} // db
} // basar


#endif // GUARD_SINGLE_ROW_CACHING_H
