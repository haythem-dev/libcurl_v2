#ifndef GUARD_FULL_CACHING_H
#define GUARD_FULL_CACHING_H

//---------------------------------------------------------------------------------------------//
/*! \file
 *  \brief  internal implementation for the caching policy 'full caching'
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
/*! \brief  class FullCaching holds a AccessorPropertyTable_YIterator.
			Depending on the concrete caching policy the caching controller, propertyTable_YIterator
			and the current row have to be updated.
            
			This is a final class. */
//---------------------------------------------------------------------------------------------//
class FullCaching : public CachingPolicy
{
public:
	FullCaching( CacheController* cacheController  );

	virtual ~FullCaching();

	//! \brief	method to delegate the prae-++-operator on the passed AccessorPropertyTable_YIterator. \n no-throw 
	virtual void	preIncrement(AccessorPropertyTable_YIterator& r);
	
	//! \brief	method to delegate the post-++-operator on the passed AccessorPropertyTable_YIterator. \n no-throw 
	virtual void	postIncrement(AccessorPropertyTable_YIterator& r);

	//! \brief	the internal row number is upadted. \n no-throw 
	virtual bool	updateRowNumber( const bool replicated, AccessorPropertyTable_YIterator& r );
	
	//! \brief	creates a snapshot of internal PropertyTable of CacheController. 
	//! \n		no-throw
	virtual const PropertyTableSnapshotRef		createSnapshot() const;

	//! \brief	sets the State 'SS_UNKNOWN' to every xiterator of the passed AccessorPropertyTable YIterator . 
	//! \n		no-throw
	virtual void								setStateUnknown	(AccessorPropertyTable_YIterator & yit);
	
	/*! \brief	erases range of AccessorPropertyTable YIterators from yItBegin (including) 
	            to yItEnd (excluding) of internal propertyTable of CacheController. 
		\n		no-throw */
	virtual AccessorPropertyTable_YIterator		erase	(AccessorPropertyTable_YIterator & yItBegin, 
													     AccessorPropertyTable_YIterator & yItEnd );
	
	/*! \brief	erases passed AccessorPropertyTable YIterator of internal propertyTable of CacheController. 
		\n		no-throw */
	virtual AccessorPropertyTable_YIterator		erase	(AccessorPropertyTable_YIterator & aptYIt );

    virtual void                                erase   ( const PropertyStateSet& state );

	/*! \brief	inserts a new row with type of given InsertTypeEnum in internal PropertyTable  
		\n throw InsertNotAllowedException */
	virtual AccessorPropertyTable_YIterator		insert	(InsertTypeEnum eInsertTypeEnum);

    virtual AccessorPropertyTable_YIterator atRow( const basar::Int32 row );

protected:
	//--------------------------------------------------------//
	// virtual methods
	//--------------------------------------------------------//
	/*! \brief	all hits of the sql query has to be loaded in the propertyTable. \n no-throw */
	virtual void	checkFetchAll( bool replicated );
	
	/*! \brief	checks the new row number if there are no hits. \n no-throw */
	virtual Int32	getIndex4EmptyResult() const;
	
	/*! \brief	checks the new row number if there are hits. \n no-throw */
	virtual Int32	getIndex4HitResult() const;
	
	/*! \brief	checks the new row number if there are hits and a filtered begin() was invoked. \n no-throw */
	virtual Int32	getIndex4FilterResult(property::PropertyTable_YIterator & yit) const;
};

//---------------------------------------------------------------------------------------------//
} // aspect
} // db
} // basar

//---------------------------------------------------------------------------------------------//
#endif // GUARD_FULL_CACHING_H
