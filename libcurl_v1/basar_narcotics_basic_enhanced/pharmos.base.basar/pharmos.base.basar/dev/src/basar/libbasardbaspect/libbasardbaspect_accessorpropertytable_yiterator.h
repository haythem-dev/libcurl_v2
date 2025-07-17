#ifndef GUARD_ACCESSORPROPERTYTABLE_YITERATOR_H
#define GUARD_ACCESSORPROPERTYTABLE_YITERATOR_H
//----------------------------------------------------------------------------
/*! \file
 *  \brief  external interface of AccessorPropertyTable_YIterator
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
#include "libbasarproperty_propertytable_yiterator.h"

//----------------------------------------------------------------------------------//
// namespaces
//----------------------------------------------------------------------------------//
namespace basar  {
namespace db     {
namespace aspect {

//----------------------------------------------------------------------------
class AccessorPropertyTableRef;

//----------------------------------------------------------------------------
//! \brief dump AccessorPropertyTable_YIterator variable's content to debug output
//!
//! \note  use in Visual Studio's "Command Window - Immediate" following input (OBJECT_ADDRESS e.g.: 0x0012fa94) :
//! \code  {,,libbasardbaspect.dll} basar::db::aspect::dumpAPTYIt ( OBJECT_ADDRESS )
//! \endcode
LIBBASARDBASPECT_API void dumpAPTYIt	(size_t addr	//!< object address
										);

//----------------------------------------------------------------------------------------------------------------//
// class declaration AccessorPropertyTable_YIterator
//----------------------------------------------------------------------------------------------------------------//
//////////////////////////////////////////////////////////////////////////////////////
/*! \brief  class YIterator holds the yiterator of a propertytable, 
			a pointer to it's cachecontroller and a row-number.
            
		This is a final class.     
		no-throw */
//////////////////////////////////////////////////////////////////////////////////////
class AccessorPropertyTable_YIterator
{
	friend class CacheController;
	friend class CachingPolicy;
	friend class FullCaching;
	friend class NoCaching;
	friend class OnDemandCaching;
	friend class SingleRowCaching;
	
public:
	//-----------------------------------------------//
	// c'tors
	//-----------------------------------------------//
	/*! \brief  standard constructor. \n no-throw */
	LIBBASARDBASPECT_API AccessorPropertyTable_YIterator();
	
	/*!	\brief overloaded constructor that creates the give PropertyStateSet for the YIterator. \n no-throw */
	LIBBASARDBASPECT_API AccessorPropertyTable_YIterator( const basar::db::aspect::PropertyStateSet& state );

	/*!	\brief copy constructor that creates YIterator out of an existing YIterator. \n no-throw */
	LIBBASARDBASPECT_API AccessorPropertyTable_YIterator( const AccessorPropertyTable_YIterator& rhs, 
														  const PropertyStateSet& pss ); 

	/*!	\brief	creates a unbounded copy of only the values (not of the states) of this yiterator.
				The datatype 'basar::UNKNOWN' is not copied, because it has not been 
				implemented in BASAR yet. If this method is invoked on an uninitialized yiterator
				an exception is thrown. 
		\n		no-throw */
	LIBBASARDBASPECT_API 
	AccessorPropertyTable_YIterator						cloneToUnboundedPropertyRow();

	/*!	\brief	method compares all attribute values of the passed YIterator with this.\n
				The states will not regarded at all for comparison, because the method 
				cloneToUnboundedPropertyRow() copies only attribute values with the states 
				unequal of SS_UNSET. \n
				Passed YIterator's attributes not existing in the original YIterator (this) 
				will return false. So it is regarded as unequal. \n
				One YIterator's contained, but not set attributes that are contained and set in
				other YIterator will return false; regarded as unequal. \n
				Attributes in passed YIterator and original YIterator (this) don't need to have 
				same order. \n
				The datatype 'basar::UNKNOWN' could not be regarded in the comparisons. \n
				YIterators may not be end().
		\n		throw InvalidIteratorException */
	LIBBASARDBASPECT_API 
	std::pair< bool, cmnutil::ParameterList >	compareSetValues( const AccessorPropertyTable_YIterator rhs,
																  const cmnutil::ParameterList& pl
																	= cmnutil::ParameterList() );

	/*!	\brief	All attribute values (only values not states) existing in the passed yiterator are copied to 
				the original yiterator. Before the copy it is checked if the passed yiterator attribute 
				exists in the original yiterator (this). The states of the original yiterator's attributes 
				could be changed by setting the values.\n no-throw */
	LIBBASARDBASPECT_API void							match( AccessorPropertyTable_YIterator rhs ); 

	/*!	\brief	returns const CacheController reference in the form of a shared pointer.
        \n		no-throw */
	LIBBASARDBASPECT_API const CacheControllerSharedPtr	getCacheControllerSharedPtr() const; 

    /*!	\brief	returns const AccessorPropertyTableRef reference in the form of a shared pointer.
        \n		no-throw */
    LIBBASARDBASPECT_API const  AccessorPropertyTableRef getPropertyTable() const; 


	//-----------------------------------------------//
	// operator methods
	//-----------------------------------------------//
	/*!	\brief checks if two iterators are equal
		\n no-throw */
	LIBBASARDBASPECT_API 
	bool											    operator==( const AccessorPropertyTable_YIterator& rhp ) const;
	
	/*!	\brief checks if two iterators are not equal
		\n no-throw */
	LIBBASARDBASPECT_API 
	bool											    operator !=( const AccessorPropertyTable_YIterator& rhp) const;
	
    /*!	\brief	sets iterator to next row
		\n		throw OutOfRangeIteratorException */
	LIBBASARDBASPECT_API 
	AccessorPropertyTable_YIterator						operator ++();
    
	/*!	\brief  sets iterator to next row
		\n		throw OutOfRangeIteratorException */
	LIBBASARDBASPECT_API 
	AccessorPropertyTable_YIterator						operator ++(int);
	
    /*!	\brief	sets iterator to previous row
		\n		throw OutOfRangeIteratorException */
	LIBBASARDBASPECT_API 
	AccessorPropertyTable_YIterator						operator --();
    
	/*!	\brief	sets iterator to previous row
		\n		throw OutOfRangeIteratorException */
	LIBBASARDBASPECT_API 
	AccessorPropertyTable_YIterator						operator --(int);
		
	/*!	\brief sets xiterator on first column of current row
		\n no-throw */
	LIBBASARDBASPECT_API 
	AccessorPropertyTable_XIterator						begin() const;

	/*!	\brief	sets xiterator on first column of current row with given state. 
		\n		no-throw */
	LIBBASARDBASPECT_API 
	AccessorPropertyTable_XIterator						begin( const PropertyStateSet& state );

	/*!	\brief sets xiterator on last column of current row
		\n no-throw */
	LIBBASARDBASPECT_API 
	AccessorPropertyTable_XIterator						end	() const;
	
	/*! \brief checks if iterator points to begin() of the inner container
		\n     no-throw */
	LIBBASARDBASPECT_API bool						    isBegin			() const;

	/*! \brief checks if iterator points to end() of the inner container
		\n     no-throw */
	LIBBASARDBASPECT_API bool						    isEnd			() const;

	/*! \brief returns the index of the iterator
		\n     no-throw */
	LIBBASARDBASPECT_API Int32					        getRowNumber	() const;

	/*! \brief clears the values of iterator
		\n no-throw */
	LIBBASARDBASPECT_API 
	void												clear();	
	
	//-----------------------------------------------//
	// filter- and type-methods
	//-----------------------------------------------//
	/*! \brief	returns the filter (member of PropertyStateSet). \n throw NoFilterAvailableException */
	LIBBASARDBASPECT_API 
	const PropertyStateSet&								getFilter() const;
	
    /*!	\brief gets data type of iterator.\n no-throw */
	LIBBASARDBASPECT_API 
	InsertTypeEnum								        getInsertType( ) const;

    /*!	\brief gets state of iterator. \n throw NoStateAvailableException */
	LIBBASARDBASPECT_API 
	const PropertyStateSet								getState( ) const;

	/*!	\brief gets data type of given column.\n no-throw */
	LIBBASARDBASPECT_API 
	const PropertyType&									getType( const ColumnName & rColName ) const;

    LIBBASARDBASPECT_API
    const PropertyType&                                 getType( const ColumnIndex index ) const;

	/*! \brief	gets the index from the rColName.\n		no-throw */
	LIBBASARDBASPECT_API ColumnIndex			        getIndex( const ColumnName& rColName ) const;


	/*!	\brief gets state of given column. \n no-throw */
	LIBBASARDBASPECT_API 
	const PropertyState&								getState( const ColumnName& rColName ) const;

	/*!	\brief gets state of given index. \n no-throw */
	LIBBASARDBASPECT_API 
	const PropertyState&								getState( const ColumnIndex Index ) const;

	/*! \brief	sets the passed state to the xiterator determined by the passed column name.
		\n		no-throw */
	LIBBASARDBASPECT_API 
	void												setPropertyState( const ColumnName& rColName, 
																	      const SupportedStateEnum eState );


	//-----------------------------------------------//
	// contains-methods
	//-----------------------------------------------//
	/*!	\brief checks if propertytable has a column called colName
		\n no-throw */
	LIBBASARDBASPECT_API bool						    contains(	const ColumnName & colName ) const;
	
	/*!	\brief checks if propertytable has a column called colName with a specific type
		\n no-throw */
	LIBBASARDBASPECT_API bool						    contains(	const ColumnName & colName, 
																	const PropertyType pt ) const;
	
	/*!	\brief checks if propertytable has a column called colName with a specific type (as pair)
		\n no-throw */
	LIBBASARDBASPECT_API bool						    contains(	const std::pair<ColumnName, PropertyType>& p )	const;
	
	/*!	\brief checks if propertytable has a column called colName with a specific type and a row with a specific state
		\n throw NoStateAvailableException. */
	LIBBASARDBASPECT_API bool						    contains(	const ColumnName & colName, 
																	const PropertyType pt, 
																	const PropertyStateSet pss )	const;
	
	/*!	\brief checks if propertytable has a column called colName with a specific type (as pair) and a row with a specific state
		\n throw NoStateAvailableException.*/
	LIBBASARDBASPECT_API bool						    contains(	const std::pair<ColumnName, PropertyType>& p, 
																	const PropertyStateSet pss ) const;

	//-----------------------------------------------//
	// set- and get-methods
	//-----------------------------------------------//
	//-------------//
	// Int16
	//-------------//
	/*! \brief	sets the value of a certain column (via index) as a 2 byte integer 
		\n no-throw */
    LIBBASARDBASPECT_API void							setInt16( const ColumnIndex index, const Int16 value );

	/*!	\brief sets the value of a column as an int16
		\n no-throw */
	LIBBASARDBASPECT_API void							setInt16( const ColumnName & rColName, const Int16 value );	
	
	/*!	\brief gets value of a column of the type int16
		\n no-throw */
	LIBBASARDBASPECT_API const Int16&			        getInt16( const ColumnName & rColName ) const;
		
	/*!	\brief gets value of a column of the type int16
		\n no-throw */
	LIBBASARDBASPECT_API const Int16&			        getInt16( const ColumnIndex index ) const;
	
	//-------------//
	// Int32
	//-------------//
    /*! \brief	sets the value of a certain column (via index) as a 4 byte integer 
		\n		no-throw */
	LIBBASARDBASPECT_API void							setInt32( const ColumnIndex index, const Int32 value );

	/*!	\brief	sets value of a column as an int32
		\n		no-throw */
	LIBBASARDBASPECT_API void							setInt32( const ColumnName & rColName, const Int32 value );

	/*!	\brief gets value of a column of the type int32
		\n no-throw */
	LIBBASARDBASPECT_API const Int32&                   getInt32( const ColumnName & rColName ) const;
	
	/*!	\brief gets value of a column of the type int32
		\n no-throw */
	LIBBASARDBASPECT_API const Int32&   		        getInt32( const ColumnIndex index ) const;

    //-------------//
    // Int64
    //-------------//
    /*! \brief	sets the value of a certain column (via index) as a 8 byte integer
    \n		no-throw */
    LIBBASARDBASPECT_API void							setInt64(const ColumnIndex index, const Int64 value);

    /*!	\brief	sets value of a column as an int64
    \n		no-throw */
    LIBBASARDBASPECT_API void							setInt64(const ColumnName & rColName, const Int64 value);

    /*!	\brief gets value of a column of the type int64
    \n no-throw */
    LIBBASARDBASPECT_API const Int64&                   getInt64(const ColumnName & rColName) const;

    /*!	\brief gets value of a column of the type int32
    \n no-throw */
    LIBBASARDBASPECT_API const Int64&   		        getInt64(const ColumnIndex index) const;

	//-------------//
	// string
	//-------------//
	/*! \brief	sets the value of a certain column (via index) as a string  
		\n		no-throw */
	LIBBASARDBASPECT_API void							setString( const ColumnIndex index, const I18nString& value );

	/*!	\brief sets value of column as a string
		\n no-throw */
	LIBBASARDBASPECT_API void							setString(	const ColumnName& rColName, 
																	const I18nString & value );
	/*!	\brief gets value of a column of type conststring
		\n no-throw */
	LIBBASARDBASPECT_API ConstBuffer					getConstString( const ColumnName & rColName ) const;

	/*!	\brief gets value of a column of type conststring
		\n no-throw */
	LIBBASARDBASPECT_API ConstBuffer					getConstString( const ColumnIndex index ) const;

	/*!	\brief gets value of a column of type string
		\n no-throw */
	LIBBASARDBASPECT_API const I18nString&				getString( const ColumnName & rColName ) const;
	
	/*!	\brief gets value of a column of type string
		\n no-throw */
	LIBBASARDBASPECT_API const I18nString&				getString( const ColumnIndex index ) const;
	
	/*!	\brief gets value of a column of type sqlstring
		\n no-throw */
	LIBBASARDBASPECT_API const SQLString				getSQLString( const ColumnName & rColName ) const;


	//------------------//
	// float32
	//------------------//
	/*! \brief	sets the value of a certain column (via index) as a 4 byte float 
		\n		no-throw */
	LIBBASARDBASPECT_API void							setFloat32(		const ColumnIndex index, const Float32 value );	
	/*!	\brief sets value of column as a float32
		\n no-throw */
	LIBBASARDBASPECT_API void							setFloat32(		const ColumnName & rColName, const Float32 value );
	/*!	\brief gets value of a column of type float32
		\n no-throw */
	LIBBASARDBASPECT_API const Float32&  			    getFloat32(		const ColumnName & ColName ) const;
	/*!	\brief gets value of a column of type float32
		\n no-throw */
	LIBBASARDBASPECT_API const Float32&				    getFloat32(		const ColumnIndex index ) const;


	//------------------//
	// float64
	//------------------//
	/*! \brief	sets the value of a certain column (via index) as a 8 byte float 
		\n		no-throw */
	LIBBASARDBASPECT_API void							setFloat64		(const ColumnIndex index, const Float64 value );	
	/*!	\brief sets value of a column as a float64
		\n no-throw */
	LIBBASARDBASPECT_API void							setFloat64		(const ColumnName & rColName, const Float64 value );
	/*!	\brief gets value of a column of type float64
		\n no-throw */
	LIBBASARDBASPECT_API const Float64&				    getFloat64		(const ColumnName & rColName ) const;
	/*!	\brief gets value of a column of type float64
		\n no-throw */
	LIBBASARDBASPECT_API const Float64&				    getFloat64		(const ColumnIndex index ) const;

	
	//------------------//
	// decimal
	//------------------//
	/*!	\brief gets value of a column of type decimal
		\n no-throw */
	LIBBASARDBASPECT_API const Decimal&					getDecimal		( const ColumnName & rColName ) const;
	
	/*!	\brief gets value of a column of type decimal
		\n no-throw */
	LIBBASARDBASPECT_API const Decimal&					getDecimal		( const ColumnIndex index ) const;

	/*! \brief	sets the value of a certain column (via index) as a Decimal 
		\n		no-throw */
	LIBBASARDBASPECT_API void							setDecimal		( const ColumnIndex index, const Decimal value );	

	/*!	\brief sets value of column as a decimal
		\n     no-throw */
	LIBBASARDBASPECT_API void							setDecimal		( const ColumnName & rColName, const Decimal value );
	

	//------------------//
	// datetime
	//------------------//
	/*! \brief	sets the value of a certain column (via index) as a DateTime 
		\n		no-throw */
	LIBBASARDBASPECT_API void							setDateTime		( const ColumnIndex index, 
																		  const DateTime  & value );	
	/*!	\brief sets value of column as a DateTime
		\n no-throw */
	LIBBASARDBASPECT_API void							setDateTime		( const ColumnName& rColName, 
																		  const DateTime& value );
	/*!	\brief gets value of column of type DateTime */
	LIBBASARDBASPECT_API const DateTime&				getDateTime		( const ColumnName& rColName ) const;
	/*!	\brief gets value of column of type DateTime
		\n no-throw */
	LIBBASARDBASPECT_API const DateTime&				getDateTime		( const ColumnIndex index ) const;

	//------------------//
	// timespan
	//------------------//
	/*! \brief	sets the value of a certain column (via index) as a TimeSpan 
		\n		no-throw */
	LIBBASARDBASPECT_API void							setTimeSpan		( const ColumnIndex index, 
																		  const TimeSpan  & value );	
	/*!	\brief sets value of column as a TimeSpan
		\n no-throw */
	LIBBASARDBASPECT_API void							setTimeSpan		( const ColumnName& rColName, 
																		  const TimeSpan& value );
	/*!	\brief gets value of column of type TimeSpan */
	LIBBASARDBASPECT_API const TimeSpan&				getTimeSpan		( const ColumnName& rColName ) const;
	/*!	\brief gets value of column of type TimeSpan
		\n no-throw */
	LIBBASARDBASPECT_API const TimeSpan&				getTimeSpan		( const ColumnIndex index ) const;

	//------------------//
	// date
	//------------------//
	/*! \brief	sets the value of a certain column (via index) as a DateTime 
		\n		no-throw */
	LIBBASARDBASPECT_API void							setDate		( const ColumnIndex index, const Date& value );	
	/*!	\brief sets value of column as a DateTime
		\n no-throw */
	LIBBASARDBASPECT_API void							setDate		( const ColumnName& rColName, const Date& value );
	/*!	\brief gets value of column of type DateTime */
	LIBBASARDBASPECT_API const Date&					getDate		( const ColumnName& rColName ) const;
	/*!	\brief gets value of column of type DateTime
		\n no-throw */
	LIBBASARDBASPECT_API const Date&					getDate		( const ColumnIndex index ) const;

	//------------------//
	// time
	//------------------//
	/*! \brief	sets the value of a certain column (via index) as a DateTime 
		\n		no-throw */
	LIBBASARDBASPECT_API void							setTime		( const ColumnIndex index, const Time& value );	
	/*!	\brief sets value of column as a DateTime
		\n no-throw */
	LIBBASARDBASPECT_API void							setTime		( const ColumnName& rColName, const Time& value );
	/*!	\brief gets value of column of type DateTime */
	LIBBASARDBASPECT_API const Time&					getTime		( const ColumnName& rColName ) const;
	/*!	\brief gets value of column of type DateTime
		\n no-throw */
	LIBBASARDBASPECT_API const Time&					getTime		( const ColumnIndex index ) const;

    //------------------//
    // binary
    //------------------//
    /*! \brief	sets the value of a certain column (via index) as a Binary
    \n		no-throw */
    LIBBASARDBASPECT_API void							setBinary(const ColumnIndex index, const Binary& value);
    /*!	\brief sets value of column as a Binary
    \n no-throw */
    LIBBASARDBASPECT_API void							setBinary(const ColumnName& rColName, const Binary& value);
    /*!	\brief gets value of column of type Binary */
    LIBBASARDBASPECT_API const Binary&					getBinary(const ColumnName& rColName) const;
    /*!	\brief gets value of column of type Binary
    \n no-throw */
    LIBBASARDBASPECT_API const Binary&					getBinary(const ColumnIndex index) const;

	//-----------------------------------------------//
	// state-methods
	//-----------------------------------------------//
	/*! \brief	returns true, if all given properties (rPropertyNames) are set in this yiterator
		\n		no-throw */
	LIBBASARDBASPECT_API bool	arePropertiesSet ( const PropertyName& rPropertyNames 
												 ) const;
	
    /*! \brief	returns true, if all given properties (rParaList) are set in this yiterator
		\n		no-throw */
	LIBBASARDBASPECT_API bool	arePropertiesSet ( const cmnutil::ParameterList& rParaList 
												 ) const;

    /*! \brief returns true, if all the given properties (rPropertyNames) are set in this yiterator
                and at least one of those properties is in one of the given states (pss)
        \n		no-throw */
	LIBBASARDBASPECT_API bool	arePropertiesSetAndOneMustBeInState (const PropertyName     & rPropertyNames, 
																	 const PropertyStateSet & pss
																	) const;	

    /*! \brief returns true, if all the given properties (rPropertyNames) are set in this yiterator
                and at least one of those properties is in one of the given states (pss)
        \n		no-throw */
	LIBBASARDBASPECT_API bool	arePropertiesSetAndOneMustBeInState (const cmnutil::ParameterList & rPropertyNames, 
																	 const PropertyStateSet       & pss
																	) const;	 

    /*! \brief	returns true, if all of the given properties (rPropertyNames) 
                are set in this yiterator and all in one of the given states (pss)
        \n		no-throw */
	LIBBASARDBASPECT_API bool	arePropertiesSetAndAllInState (const PropertyName     & rPropertyNames, 
															   const PropertyStateSet & pss
															  ) const;	

     /*! \brief	returns true, if all of the given properties (rPropertyNames) 
                are set in this yiterator and all in one of the given states (pss)
         \n		no-throw */
	LIBBASARDBASPECT_API bool	arePropertiesSetAndAllInState (const cmnutil::ParameterList & rPropertyNames,
															   const PropertyStateSet       & pss
															  ) const;	

	/*! \brief	returns true, if all given properties in the 1st parameter ( plShouldBeSet)
                are set and at least one of the given properties in the 
				2nd parameter (plOneMustBeInState) is in one of the given states (pss)
        \n		no-throw */
	LIBBASARDBASPECT_API bool	are1stPropertiesSetAnd2ndOneInState(const cmnutil::ParameterList & plShouldBeSet, 
																	const cmnutil::ParameterList & plOneMustBeInState, 
																	const PropertyStateSet       & pss 
																   ) const;
	
    /*! \brief	returns true, if all given properties in the 1st parameter ( propertyNamesShouldBeSet)
                are set and at least one of the given properties in the 
				2nd parameter (propertyNamesOneMustBeInState) is in one of the given states (pss)
		\n		no-throw */
    LIBBASARDBASPECT_API bool	are1stPropertiesSetAnd2ndOneInState(const PropertyName     & propertyNamesShouldBeSet, 
																	const PropertyName     & propertyNamesOneMustBeInState, 
																	const PropertyStateSet & pss 
																   ) const;
   
     /*! \brief returns true, if at least one of the given properties (rPropertyNames) in this 
                yiterator is in one of the given states (pss)
         \n		no-throw */
	LIBBASARDBASPECT_API bool	isOnePropertyInState(const cmnutil::ParameterList & rPropertyNames, 
													 const PropertyStateSet       & pss
													) const;	
	
    /*! \brief returns true, if all of the given properties (rParaList), that are 
               listed in this YIterator are in one of the given states (pss). 
               If none of the parameters are listed in the YIterator the return 
               value is acceptNoProperty 
        \n	   no-throw */
   LIBBASARDBASPECT_API bool	areKnownPropertiesInState (const cmnutil::ParameterList & rParaList,		//<! paramter List
														   const PropertyStateSet       & pss,				//<! parameterstate
														   bool                           acceptNoProperty	//<! if no parameter is listed in the YIterator the return value is acceptNoProperty 
														  ) const;

   /*! \brief returns true, if all of the given properties (rParaList), that are 
               set in this YIterator are in one of the given states (pss). 
               If none of the parameter are set in the YIterator the return 
               value is acceptNoProperty 
       \n	   no-throw */
   LIBBASARDBASPECT_API bool	areKnownAndSetPropertiesInState	(const cmnutil::ParameterList & rParaList,			//<! paramter List
																 const PropertyStateSet       & pss,				//<! parameterstate
																 bool                           acceptNoProperty    //<! if no parameter is listed in the YIterator the return value is acceptNoProperty 
																) const;

	/*! \brief	returns true, if given property is not in unset state and yiterator is not at the end 
		\n		no-throw */
	LIBBASARDBASPECT_API bool	isNotEndAndSet (const PropertyName & rPropertyName 
		                                       ) const;
	
	/*! \brief	returns true, if yiterator contains given property and property is not in unset state 
		\n		no-throw */
	LIBBASARDBASPECT_API bool	isContainedAndSet (const PropertyName & rPropertyName 
		                                          ) const;
	

	//-----------------------------------------------//
	//-----------------------------------------------//

	/*! \brief	whole line to stream
		\n		no-throw */
	LIBBASARDBASPECT_API void				toStream( std::ostream& strm = std::cout ) const;

	/*! \brief	checks if yiterator is initialized					 
		\n	    no-throw */
	LIBBASARDBASPECT_API bool			    isNull(  ) const;

	/*! \brief	returning size of propertytable after resultset has been read completely
		\n		no-throw */
	LIBBASARDBASPECT_API 
	NumberRows						        size() const;


    /*! \brief returns the number of columns in this Y-Iterator 
		\n	   no-throw */
    LIBBASARDBASPECT_API    ColumnIndex		getNumberOfColumns() const;

	/*! \brief returns the name of one column 
		\n	   no-throw  */
    LIBBASARDBASPECT_API    ColumnName      getName(ColumnIndex index) const;

    /*! \brief changing the state of row represented by this in propertytable to SS_DELETE
        \n no-throw*/
    LIBBASARDBASPECT_API    void            markForDelete();

private:
	//------------------//
	// private methods
	//------------------//
	/*!	\brief	constrution of YIterator; pointer to cachecontroller, yiterator of propertytable and 
				row-number (default = -1) needed. 
		\n		no-throw */
	AccessorPropertyTable_YIterator( boost::shared_ptr< CacheController > pCache, 
									 property::PropertyTable_YIterator    it, 
									 const Int32                          row = -1 );

	/*! \brief	checks if the iterator is valid. 
		\n		throw InvalidIteratorException */
	void	checkValid	() const;

	/*! \brief check if XIterator and YIterator have equal value for given attribute name
	    \n     no-throw */
	bool	cmpVal		(SupportedTypeEnum                         eType,	//!< datatype
						 const VarString                         & colname,	//!< attribute name
						 const property::PropertyTable_XIterator & xit,		//!< iterator to compared value
						 const AccessorPropertyTable_YIterator   & yit		//!< iterator to compared value
						) const;

	//------------------//
	// private members
	//------------------//
	mutable CacheControllerSharedPtr	m_pCacheController;	//!< member: shared pointer to own cachecontroller
	property::PropertyTable_YIterator	m_PtYit;			//!< member: yiterator of propertytable
	Int32								m_RowNumber;		//!< member: row-number to save the current position
};

/*--------------------------------------------------------------------------------------------------*/
} // aspect
} // db
} // basar

/*--------------------------------------------------------------------------------------------------*/
#endif
