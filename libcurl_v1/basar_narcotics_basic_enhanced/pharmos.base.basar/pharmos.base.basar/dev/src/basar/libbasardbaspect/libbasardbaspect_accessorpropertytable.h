#ifndef GUARD_ACCESSORPROPERTYTABLEREF_H
#define GUARD_ACCESSORPROPERTYTABLEREF_H
//----------------------------------------------------------------------------
/*! \file
 *  \brief  external interface of AccessorPropertyTableRef
 *  \author Roland Kiefert 
 *  \date   18.08.2005
 */
//----------------------------------------------------------------------------

//----------------------------------------------------------------------------------//
// dll exports und imports
//----------------------------------------------------------------------------------//
#ifndef LIBBASARDBASPECT_API
	#ifdef _WIN32
		#ifdef LIBBASARDBASPECT_EXPORTS
			#define LIBBASARDBASPECT_API __declspec(dllexport)	//!< dll exported
		#else
			#define LIBBASARDBASPECT_API __declspec(dllimport)	//!< dll imported
		#endif 
	#else 
		#define LIBBASARDBASPECT_API
	#endif 
#endif 

//----------------------------------------------------------------------------------//
#include "libbasardbaspect_definitions.h"

//----------------------------------------------------------------------------------//
// namespaces
//----------------------------------------------------------------------------------//
namespace basar  {
namespace db     {
	namespace sql
	{
		class StatementRef;
		class ResultsetRef;
		class PreparedStatementRef;
	}
namespace aspect {

//----------------------------------------------------------------------------------//
// forward declarations
//----------------------------------------------------------------------------------//
class AccessorPropertyTable_YIterator;

//----------------------------------------------------------------------------
//! \brief dump maximal maxRows of AccessorPropertyTableRef variable's property table rows to debug output
//!
//! \note  use in Visual Studio's "Command Window - Immediate" following input (OBJECT_ADDRESS e.g.: 0x0012fa94) :
//! \code  {,,libbasardbaspect.dll} basar::db::aspect::dumpAPTRefRows ( OBJECT_ADDRESS, maxRows )
//! \endcode
LIBBASARDBASPECT_API void dumpAPTRefRows(size_t addr	//!< object address
    , int maxRows
);

//----------------------------------------------------------------------------
//! \brief dump max 100 rows AccessorPropertyTableRef variable's property table rows to debug output
//!
//! \note  use in Visual Studio's "Command Window - Immediate" following input (OBJECT_ADDRESS e.g.: 0x0012fa94) :
//! \code  {,,libbasardbaspect.dll} basar::db::aspect::dumpAPTRef ( OBJECT_ADDRESS )
//! \endcode
LIBBASARDBASPECT_API void dumpAPTRef(size_t addr	//!< object address
);


//----------------------------------------------------------------------------------//
// class declaration AccessorPropertyTableRef
//----------------------------------------------------------------------------------//
//////////////////////////////////////////////////////////////////////////////////////
/*! \brief  class AccessorPropertyTableRef holds a resultset and a propertytable.
            Class replicates between resultset and propertytable, 
			so that read rows from resultset are set to propertytable.
            
			This is a final class.     
			Class throws the follwing exceptions: 
			class InvalidValueException, 
			class UnknownPropertyTypeException, 
			class NullReferenceException. */
//////////////////////////////////////////////////////////////////////////////////////
class AccessorPropertyTableRef
{
	friend class Accessor; //!< friend accessor
public:
	/*! \brief returning reference to PropertyDescriptionList
		\n no-throw */
	LIBBASARDBASPECT_API 
	const PropertyDescriptionListRef				getPropertyDescriptionList() const;

	/*! \brief returning the current caching policy. \n no-throw */
	LIBBASARDBASPECT_API
	CachingPolicyEnum							    getCachingPolicy() const;

	/*! \brief returning actual size of propertytable */
	LIBBASARDBASPECT_API 
	NumberRows								        size() const;

    /*! \brief returning number of read / fetched Rows from resultset*/
    LIBBASARDBASPECT_API
    NumberRows                                      getNumberOfFetchedRows() const;

	
	/*! \brief checking if there are no results
		\n no-throw */
	LIBBASARDBASPECT_API 
	bool										    empty() const;

    /*! \brief checking if there are no results
	\n no-throw */
	LIBBASARDBASPECT_API 
	bool										    isEmpty() const;
	
	/*! \brief returning size limit of reserved space where the results are saved
		\n no-throw */
	LIBBASARDBASPECT_API 
	MaxSizeRows								        max_size() const;
	
	/*! \brief clearing propertytable and setting resultsetref to 0
		\n no-throw */
	LIBBASARDBASPECT_API 
	void											clear();

	/*! \brief returning iterator to first row with specific state
		\n no-throw */
	LIBBASARDBASPECT_API 
	AccessorPropertyTable_YIterator					begin( const PropertyStateSet & state ) const;
	
	/*! \brief returning iterator to first row
		\n no-throw */
	LIBBASARDBASPECT_API 
	AccessorPropertyTable_YIterator					begin	() const;
	
	/*! \brief returning iterator to row after last
		\n no-throw */
	LIBBASARDBASPECT_API 
	AccessorPropertyTable_YIterator					end		() const;

    /* ! \brief returning iterator pointing to "at" row
        \n throw OutOfRangeIteratorException, InvalidMethodCallException
    */
    LIBBASARDBASPECT_API
    AccessorPropertyTable_YIterator                 at( const basar::Int32 index );

	/*! \brief setting new empty row to propertytable and returning iterator to this row
		\n no-throw */
	LIBBASARDBASPECT_API 
	AccessorPropertyTable_YIterator					insert( const InsertTypeEnum eInsertTypeEnum ); 
												
	/*! \brief setting row of propertytable to state SS_DELETE
		\n no-throw */
	LIBBASARDBASPECT_API 
	void											markForDelete( AccessorPropertyTable_YIterator yIt );

	/*! \brief setting row of propertytable to state SS_UNKNOWN
		\n no-throw */
	LIBBASARDBASPECT_API 
	void											setStateUnknown( AccessorPropertyTable_YIterator yIt );

	/*! \brief deleting row from propertytable
		\n no-throw */
	LIBBASARDBASPECT_API 
	AccessorPropertyTable_YIterator					erase( AccessorPropertyTable_YIterator yIt );

	/*! \brief deleting rows between yItBegin and yItEnd from propertytable
		\n no-throw */
	LIBBASARDBASPECT_API 
	AccessorPropertyTable_YIterator					erase( AccessorPropertyTable_YIterator yItBegin, 
												           AccessorPropertyTable_YIterator yItEnd );

    /*! \brief deleting rows having the state state (i.e. SS_DELETED )
        \n no-throw */
    LIBBASARDBASPECT_API
    void                                            erase( const PropertyStateSet& state );

	/*! \brief creating the pointer of the class CacheController. 
 		\n     no-throw */
	LIBBASARDBASPECT_API												
	AccessorPropertyTableRef(boost::shared_ptr<CacheController> pCacheController);
	
	/*! \brief creating an empty reference to CacheController. 
 		\n     no-throw */
	LIBBASARDBASPECT_API							AccessorPropertyTableRef();
	
	/*! \brief checking if shared pointer to implementation class is null 
		\n no-throw */
	LIBBASARDBASPECT_API 
	bool										    isNull() const;
	
	/*! \brief setting the sharedpointer to null.
		\n no-throw */
	LIBBASARDBASPECT_API 
	void											reset();

    /*!\brief 
     * checks the AccessorPropertyTableRef are equal (same Impl)
    */
    LIBBASARDBASPECT_API bool operator==(const AccessorPropertyTableRef & a) const;

    /*! \brief	write first numRows of property table to stream
    \n		no-throw */
    LIBBASARDBASPECT_API void				toStream(std::ostream& strm = std::cout, int maxRows = 10) const;

private:
	//-------------------------//
	// private methods
	//-------------------------//

	//--------------------------------------------------------------------------//
	// snapshot methods
	//--------------------------------------------------------------------------//
	//! \brief creating snapshot of propertytable.		\n no-throw 
	const PropertyTableSnapshotRef					createSnapshot() const;
	//! \brief resetting propertytable to given snapshot.	\n no-throw 
	void											resetToSnapshot( const PropertyTableSnapshotRef snapShot );
	//! \brief creating an internal snapshot and storing it in a member. \n no-throw 
	bool										    createInternalSnapshot();
	//! \brief resetting internal snapshot to stored member. \n no-throw 
	void											resetInternalSnapshot();
	//! \brief clearing the internal snapshot. \n no-throw 
	void											clearInternalSnapshot();
	/*! \brief	turning flag on (true) or off (false) to decide if the internal snapshot must 
				be created or not. \n NullReferenceException */
	void											switchCreateInternalSnapshot( const bool toCreate );


	/*! \brief setting states of propertytable to clear. \n no-throw */
	AccessorPropertyTable_YIterator					executePerformed( AccessorPropertyTable_YIterator yit );

	/*! \brief setting the statement. \n no-throw */
	void											setStatement( const db::sql::StatementRef Statement );

	/*! \brief setting the resultset. \n no-throw */
	void											setResultset( const db::sql::ResultsetRef Resultset );

	/*! \brief returning the pointer to a CacheController. 
 		\n     NullReferenceException */
	const boost::shared_ptr<CacheController>&		getImpl() const;
	
	//-------------------------//
	// private members
	//-------------------------//
	//! \brief pointer to CacheController
	boost::shared_ptr<CacheController>				m_pCacheController; 
};	// end class AccessorPropertyTableRef

/*--------------------------------------------------------------------------------------------------*/
} // aspect
} // db
} // basar

/*--------------------------------------------------------------------------------------------------*/
#endif
