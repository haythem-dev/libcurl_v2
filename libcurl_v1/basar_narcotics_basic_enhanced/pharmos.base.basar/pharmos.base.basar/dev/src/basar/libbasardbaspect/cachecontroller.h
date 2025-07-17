#ifndef GUARD_CACHECONTROLLER_H
#define GUARD_CACHECONTROLLER_H
//----------------------------------------------------------------------------
/*! \file
 *  \brief  internal interface cache controller
 *  \author Roland Kiefert 
 *  \date   18.08.2005
 */
//----------------------------------------------------------------------------


//---------------------------------------------------------------------------------------------//
// dll exports and imports
//---------------------------------------------------------------------------------------------//
#ifdef _WIN32
    #ifdef LIBBASARDBASPECT_EXPORTS
        #define LIBBASARDBASPECT_API __declspec(dllexport)	//!< dll exported
	#else
        #define LIBBASARDBASPECT_API __declspec(dllimport)	//!< dll imported
    #endif 
#else 
    #define LIBBASARDBASPECT_API
#endif 

//---------------------------------------------------------------------------------------------//
// includes
//---------------------------------------------------------------------------------------------//
#include "libbasarproperty_propertytype.h"
#include "libbasarproperty_propertytableref.h"
#include "libbasarproperty_propertytablesnapshotref.h"
#include "libbasardbsql_statementref.h"
#include "libbasardbsql_resultsetref.h"

#include "libbasardbaspect_definitions.h"

//---------------------------------------------------------------------------------------------//
// namespaces
//---------------------------------------------------------------------------------------------//
namespace basar  {
namespace db     {
namespace aspect {

//---------------------------------------------------------------------------------------------//
// forward declaration
//---------------------------------------------------------------------------------------------//
class CachingPolicy;
class AccessorPropertyTable_YIterator;

//---------------------------------------------------------------------------------------------//
// #defines
//---------------------------------------------------------------------------------------------//
#define ITERATOR_ENDPOS    -99 //!< unique value to represent the end-iterator's value

//---------------------------------------------------------------------------------------------//
/*! \brief  class CacheController holds a resultset and a propertytable.
            class replicates between resultset and propertytable, 
			so that read rows from resultset are set to propertytable.
            
			This is a final class.     
			Class throws the follwing exceptions: 
			class InvalidValueException, 
			class UnknownPropertyTypeException. */
//---------------------------------------------------------------------------------------------//
class CacheController
{
	friend class CachingPolicy; 
	friend class FullCaching;
	friend class NoCaching; 
	friend class OnDemandCaching;
	friend class SingleRowCaching; 
	friend class AccessorPropertyTable_YIterator;

public:
	//------------------------------------------------------------------------------//
	// typedefs
	//------------------------------------------------------------------------------//
    typedef boost::shared_ptr< CacheController >	CacheControllerSharedPtr;	//!< shared pointer to cachecontroller

	//------------------------------------------------------------------------------//
	// static section
	//------------------------------------------------------------------------------//
	/*! \brief static creating of cachecontroller by calling the constructor
		\n no-throw */
	static CacheControllerSharedPtr create( const property::PropertyDescriptionListRef & r,
											const CachingPolicyEnum                      eCachePolicy = ON_DEMAND_CACHING );

public:
	//------------------------------------------------------------------------------//
	// object section
	//------------------------------------------------------------------------------//
    /*! \brief returning reference to PropertyDescriptionList
		\n no-throw */
	const PropertyDescriptionListRef	getPropertyDescriptionList	() const;

	/*! \brief returning the current caching policy. \n no-throw */
	CachingPolicyEnum				    getCachingPolicy			() const;

    /*! \brief returning shared pointer to this by casting weak -> shared
		\n no-throw */
	CacheControllerSharedPtr			getThis						() const;
    
	/*! \brief returning the aktual size of the propertytable  */
	NumberRows					        size						() const;

    /*! \brief returning the number of read / fetched Rows
		\n InvalidValueException */
	NumberRows					        getNumberOfFetchedRows		() const;
	
    /*! \brief true if propertytable is empty (in case of NO_CACHING resultset is checked if results are present)
	\n throw no-throw */
	bool								empty						() const;
	
	/*! \brief returning size limit of reserved space to save results to
		\n no-throw */
	MaxSizeRows					        max_size					() const;
	
	/*! \brief clearing propertytable and set resultsetref to 0
		\n no-throw */
	void								clear();

	/*! \brief returning iterator to first row with specific state
		\n no-throw */
	AccessorPropertyTable_YIterator		begin						( const PropertyStateSet & state );
	
	/*! \brief returning iterator to first row
		\n no-throw */
	AccessorPropertyTable_YIterator		begin						();
	
	/*! \brief return iterator to row after last
		\n no-throw */
	AccessorPropertyTable_YIterator		end							();

    AccessorPropertyTable_YIterator     at( const basar::Int32 index );

	/*! \brief setting new empty row to propertytable and returning iterator to this row
		\n no-throw */
	AccessorPropertyTable_YIterator		insert						( InsertTypeEnum eInsertTypeEnum ); 
												
	/*! \brief changing the state of row from propertytable to SS_DELETE
		\n no-throw */
	void								markForDelete				( AccessorPropertyTable_YIterator & yIt );

	/*! \brief changing the state of row from propertytable to SS_UNKNOWN
		\n no-throw */
	void								setStateUnknown				( AccessorPropertyTable_YIterator & yIt );

	/*! \brief deleting row from propertytable
		\n no-throw */
	AccessorPropertyTable_YIterator		erase						( AccessorPropertyTable_YIterator & yIt );

	/*! \brief deleting rows between yItBegin and yItEnd from propertytable
		\n no-throw */
	AccessorPropertyTable_YIterator		erase						( AccessorPropertyTable_YIterator & yItBegin, 
																	  AccessorPropertyTable_YIterator & yItEnd );

    void                                erase                       ( const PropertyStateSet& state );

	/*! \brief setting the statement
		\n no-throw */
	void								setStatement				( const db::sql::StatementRef Statement );

	/*! \brief setting the resultset
		\n no-throw */
	void								setResultset				( const db::sql::ResultsetRef Resultset );

	/*! \brief setting states of propertytable to clear
		\n no-throw */
	AccessorPropertyTable_YIterator		executePerformed			( AccessorPropertyTable_YIterator yit );
	
	//-------------------------------------------------------------------------------------------------------//
	// snapshot methods
	//-------------------------------------------------------------------------------------------------------//
	/*! \brief creating snapshot of propertytable
		\n no-throw */
	const PropertyTableSnapshotRef		createSnapshot					() const;
	//! \brief resetting propertytable to given snapshot	\n no-throw 
	void								resetToSnapshot					( const PropertyTableSnapshotRef snapShot );
	//! \brief creating an internal snapshot and stores it in a member. \n no-throw 
	bool								createInternalSnapshot			();
	//! \brief clearing the internal snapshot. \n no-throw 
	void								clearInternalSnapshot			();
	/*! \brief	resetting the internal snapshot (checks previously if the internal snapshot is allowed to be reset ). 
				\n no-throw */
	void								resetInternalSnapshot			();
	
	/*! \brief	turning flag on (true) or off (false) to decide if the internal snapshot must be created or not. 
				\n NullReferenceException */
	void								switchCreateInternalSnapshot	( bool toCreate );

private:
	/*! \brief	methods creates the appropriate caching policy if nesseccary and returns the shared pointer to it. \n no-throw */
	boost::shared_ptr<CachingPolicy>	giveCachingPolicy				() const;

	/*! \brief	construction of cachecontroller called by the static method 
				(create()[simple factory method]); setting PropertyDescriptionListRef to member
		\n no-throw */
	CacheController( const property::PropertyDescriptionListRef & propDescListRef, 
					 const CachingPolicyEnum                      eCachePolicy = ON_DEMAND_CACHING );

	/*! "weak" reference to object (itself) that's already managed by a shared_ptr (CacheController), 
		 not visible from the outside, initialized at static create */
	typedef boost::weak_ptr<CacheController>	CacheControllerWeakPtr; //!< weak pointer to cachecontroller
	/*! \brief setting member weak pointer
		\n no-throw */
	void								setWeakPtr						( CacheControllerWeakPtr weakPtr );		

	//-----------------------------------------------------------------------------------------------------------------//
	// private members
	//-----------------------------------------------------------------------------------------------------------------//
	CacheControllerWeakPtr				m_WeakPtr;			//!< weak pointer to this, needed for common reference group 
	property::PropertyTableRef			m_PropertyTableRef;	//!< reference to PropertyTable
	db::sql::ResultsetRef				m_Resultset;		//!< reference to Resultset
	db::sql::StatementRef				m_Statement;		//!< reference to Statement
	CachingPolicyEnum					m_CachePolicy;		//!< stores the caching policy

	//! \brief	holds the created snapshot with the current states and values
	property::PropertyTableSnapshotRef	m_SnapshotPTRef;
	
	//! \brief	flag indicating if the internal snapshot has to be created or not
	bool								m_createInternalSnapshot;
	

	//! \brief correlation between resultset and property
	struct IndexStruct
	{
		ColumnIndex				property;	//!< position of a property
		property::PropertyType	type;		//!< type of property
		UInt32					resultset;	//!< position in resultset
	};

	std::vector<IndexStruct>			m_Index;	//!< vector to IndexStruct
	boost::shared_ptr<CachingPolicy>	m_Caching;	//!< member for concrete instance of caching policy (strategy pattern)
};


//---------------------------------------------------------------------------------------------//
} // aspect
} // db
} // basar

//---------------------------------------------------------------------------------------------//
#endif // GUARD_CACHECONTROLLER_H
