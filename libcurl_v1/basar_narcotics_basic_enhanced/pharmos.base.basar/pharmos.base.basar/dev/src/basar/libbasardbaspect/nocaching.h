#ifndef GUARD_NO_CACHING_H
#define GUARD_NO_CACHING_H

//---------------------------------------------------------------------------------------------//
/*! \file
 *  \brief  internal implementation for the caching policy 'no caching'
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
/*! \brief  class CachingPolicy holds a AccessorPropertyTable_YIterator.
			Depending on the concrete caching policy the caching controller, propertyTable_YIterator
			and the current row have to be updated.
            
			This is a final class.     
			Class throws the follwing exceptions: 
			class ??? */
//---------------------------------------------------------------------------------------------//
// class NoCaching
//---------------------------------------------------------------------------------------------//
class NoCaching : public CachingPolicy
{
public:
	NoCaching(CacheController * cacheController  );

	virtual ~NoCaching();

	//--------------------------------------------------//
	// accessorpropertytable_yiterator specific methods
	//--------------------------------------------------//
	/*! \brief	checks equality of the passed accessorPropertyTable-YIterator itself 
				with its begin()-Iterator of the internal CacheController. \n no-throw */
	virtual bool							    isBegin(				const AccessorPropertyTable_YIterator& r) const;

	/*! \brief	checks equality of the passed accessorPropertyTable-YIterator itself 
				with its end()-Iterator of the internal CacheController. \n no-throw */
	virtual bool							    isEnd(					const AccessorPropertyTable_YIterator& r) const;

	/*! \brief	the internal row number is updated dflt. implementation checks 
				if the passed accessorPropertyTable-Yiterator points to the end of its internal cachecontroller. \n no-throw */
	virtual bool								updateRowNumber(		const bool replicated, AccessorPropertyTable_YIterator& r );

	//! \brief	returns the begin()-Iterator of the passed accessorPropertyTable-Yiterator . \n throw XIteratorNotAvailableException
	virtual AccessorPropertyTable_XIterator		begin(					const AccessorPropertyTable_YIterator& r) const;

	//! \brief	returns the end()-Iterator of the passed accessorPropertyTable-Yiterator . \n throw XIteratorNotAvailableException
	virtual AccessorPropertyTable_XIterator		end(					const AccessorPropertyTable_YIterator& r) const;

	/*! \brief	returns the begin()-Iterator of the passed accessorPropertyTable-Yiterator 
				filtered by the passed propertyStateSet. \n throw XIteratorNotAvailableException */
	virtual AccessorPropertyTable_XIterator		begin(					const PropertyStateSet          & state, 
																		AccessorPropertyTable_YIterator & r );
	
	/*! \brief	returns the filter (propertyStateSet) of the internal propertyTable yiterator 
				of the passed accessorPropertyTable-Yiterator. \n throw NoFilterAvailableException */
	virtual const PropertyStateSet&				getFilter(				const AccessorPropertyTable_YIterator& r) const;

	/*! \brief	returns the state(propertyStateSet) of the internal propertyTable yiterator 
				of the passed accessorPropertyTable-Yiterator. \n throw NoStateAvailableException*/
	virtual const PropertyStateSet				getState(				const AccessorPropertyTable_YIterator& r) const;


	/*! \brief	checks if the passed accessorPropertyTable-Yiterator has a attribut named by the passed colName 
				with the given datatype having a state being in the passed propertyStateSet.  \n throw NoStateAvailableException*/
	virtual bool							    contains(				const ColumnName & rColName, 
																		const PropertyType pt, 
																		const PropertyStateSet pss, 
																		const AccessorPropertyTable_YIterator& r ) const;


	/*! \brief	checks if the accessorPropertyTable-Yiterator has a attribut colName with the given datatype 
				passed as a pair having a state being in the passed propertyStateSet.  \n throw NoStateAvailableException*/
	virtual bool							    contains(				const std::pair<ColumnName, PropertyType> & p,
																		const                                       PropertyStateSet pss, 
																		const AccessorPropertyTable_YIterator     & r ) const;


	//! \brief	returns the value of the attribute named by the passed colName of the accessorPropertyTable-YIterator.  \n no-throw
	virtual const Int32&					    getInt32(				const ColumnName& rColName, 
																		const AccessorPropertyTable_YIterator& r ) const;
	
	//! \brief	returns the value of the attribute identified by the passed index of the accessorPropertyTable-YIterator.  \n no-throw
	virtual const Int32&    				    getInt32(				const ColumnIndex                      index, 
																		const AccessorPropertyTable_YIterator& r ) const;
	
	//! \brief	returns the value of the attribute named by the passed colName of the accessorPropertyTable-YIterator.  \n no-throw
	virtual const Int16&					    getInt16(				const ColumnName & rColName, 
																		const AccessorPropertyTable_YIterator& r ) const;
	
	//! \brief	returns the value of the attribute identified by the passed index of the accessorPropertyTable-YIterator.  \n no-throw
	virtual const Int16&					    getInt16(				const ColumnIndex                      index, 
																		const AccessorPropertyTable_YIterator& r ) const;
	
	/*! \brief	returns the value of the attribute named by the passed colName of the accessorPropertyTable-YIterator 
				prepared in a form for using it in a sql-statement.  \n throw FatalErrorException, NotImplementedYetException*/
	virtual const SQLString						getSQLString(			const ColumnName& rColName, 
																		const AccessorPropertyTable_YIterator& r ) const;
	
	//! \brief	returns the value of the attribute named by the passed colName of the accessorPropertyTable-YIterator.  \n no-throw
	virtual ConstBuffer							getConstString(			const ColumnName& rColName, 
																		const AccessorPropertyTable_YIterator& r ) const;
	
	//! \brief	returns the value of the attribute identified by the passed index of the accessorPropertyTable-YIterator.  \n no-throw
	virtual ConstBuffer							getConstString(			const ColumnIndex                      index, 
																		const AccessorPropertyTable_YIterator& r ) const;
	
	//! \brief	returns the value of the attribute named by the passed colName of the accessorPropertyTable-YIterator.  \n no-throw
	virtual const I18nString&					getString(				const ColumnName& rColName, 
																		const AccessorPropertyTable_YIterator& r ) const;
	
	//! \brief	returns the value of the attribute identified by the passed index of the accessorPropertyTable-YIterator.  \n no-throw
	virtual const I18nString&					getString(				const ColumnIndex                      index, 
																		const AccessorPropertyTable_YIterator& r ) const;
	
	//! \brief	returns the value of the attribute named by the passed colName of the accessorPropertyTable-YIterator.  \n no-throw
	virtual const Float64&				        getFloat64(				const ColumnName & rColName, 
																		const AccessorPropertyTable_YIterator& r ) const;
	
		//! \brief	returns the value of the attribute identified by the passed index of the accessorPropertyTable-YIterator.  \n no-throw
	virtual const Float64&				        getFloat64(				const ColumnIndex                      index, 
																		const AccessorPropertyTable_YIterator& r ) const;
	
	//! \brief	returns the value of the attribute named by the passed colName of the accessorPropertyTable-YIterator.  \n no-throw
	virtual const Float32&				        getFloat32(				const ColumnName & rColName, 
																		const AccessorPropertyTable_YIterator& r ) const;
	
	//! \brief	returns the value of the attribute identified by the passed index of the accessorPropertyTable-YIterator.  \n no-throw
	virtual const Float32&				        getFloat32(				const ColumnIndex                      index, 
																		const AccessorPropertyTable_YIterator& r ) const;
	
	//! \brief	returns the value of the attribute named by the passed colName of the accessorPropertyTable-YIterator.  \n no-throw
	virtual const DateTime&						getDateTime(			const ColumnName& rColName, 
																		const AccessorPropertyTable_YIterator& r ) const;
	
	//! \brief	returns the value of the attribute identified by the passed index of the accessorPropertyTable-YIterator.  \n no-throw
	virtual const DateTime&						getDateTime(			const ColumnIndex                      index, 
																		const AccessorPropertyTable_YIterator& r ) const;
	
	//! \brief	returns the value of the attribute named by the passed colName of the accessorPropertyTable-YIterator.  \n no-throw
	virtual const Date&							getDate(				const ColumnName& rColName, 
																		const AccessorPropertyTable_YIterator& r ) const;
	
	//! \brief	returns the value of the attribute identified by the passed index of the accessorPropertyTable-YIterator.  \n no-throw
	virtual const Date&							getDate(				const ColumnIndex                      index, 
																		const AccessorPropertyTable_YIterator& r  ) const;
	
	//! \brief	returns the value of the attribute named by the passed colName of the accessorPropertyTable-YIterator.  \n no-throw
	virtual const Time&							getTime(				const ColumnName& rColName, 
																		const AccessorPropertyTable_YIterator& r ) const;
	
	//! \brief	returns the value of the attribute identified by the passed index of the accessorPropertyTable-YIterator.  \n no-throw
	virtual const Time&							getTime(				const ColumnIndex                      index, 
																		const AccessorPropertyTable_YIterator& r  ) const;
	
	//! \brief	returns the value of the attribute named by the passed colName of the accessorPropertyTable-YIterator.  \n no-throw
	virtual const Decimal&						getDecimal(				const ColumnName & rColName, 
																		const AccessorPropertyTable_YIterator& r ) const;
	
	//! \brief	returns the value of the attribute identified by the passed index of the accessorPropertyTable-YIterator.  \n no-throw
	virtual const Decimal&						getDecimal(				const ColumnIndex                      index, 
																		const AccessorPropertyTable_YIterator& r ) const;

	//! \brief	checks if it is allowed to set a value. \n throw NoSetAllowedException.
	virtual void								checkIsAllowedToSet(	) const;
	
	//! \brief	checks if it is allowed to get a value. \n throw NoGetAllowedException.
	virtual void								checkIsAllowedToGet(	const AccessorPropertyTable_YIterator& r) const;

	//--------------------------------------------------//
	// cachecontroller specific methods
	//--------------------------------------------------//
	/*! \brief	the passed boolean value controls if the internal snapshot is switched on or off. 
				It will be interpreted with transaction.
		\n		throw InvalidValueException */
	virtual void								switchCreateInternalSnapshot( bool toCreate );

	/*! \brief	sets the State 'SS_DELETE' to every xiterator of the passed accessorPropertyTable-YIterator . 
		\n		throw InvalidMethodCallException */
	virtual void								markForDelete( AccessorPropertyTable_YIterator& yit );

    virtual AccessorPropertyTable_YIterator atRow( const basar::Int32 row );
	
protected:
	//-----------------------------//
	// protected methods
	//-----------------------------//
		/*! \brief	checks if call for replicate has to be skipped \n no-throw */
	virtual bool								mustReplicateMethodTerminated() const;
	
	/*! \brief	by default it returns 0. \n throw RowNoLongerAvailableException */
	virtual Int32								getIndex4HitResult() const;
	
	/*! \brief	checks if the filtered begin() is allowed. It depends of the current caching policy. 
				\n throw NoFilteredBeginAllowedException */
	virtual void								checkFilteredBeginAllowed() const;
};


} // aspect
} // db
} // basar


#endif // GUARD_NO_CACHING_H
