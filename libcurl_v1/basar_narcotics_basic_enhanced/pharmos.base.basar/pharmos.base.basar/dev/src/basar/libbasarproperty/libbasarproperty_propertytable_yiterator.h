#ifndef GUARD_PROPERTYTABLE_YITERATOR_H
#define GUARD_PROPERTYTABLE_YITERATOR_H
//--------------------------------------------------------------------------------------------//
/*! \file
 *  \brief  class PropertyTable_YIterator represents an iterator pointing 
			to a collection of propertyinternals (whole row)
 *  \author Bischof Björn 
 *  \date   23.08.2005
 */
//--------------------------------------------------------------------------------------------//

//--------------------------------------------------------------------------------------------//
// dll imports and exports
//--------------------------------------------------------------------------------------------//
#ifndef LIBBASARPROPERTY_API
	#ifdef _WIN32
		#ifdef LIBBASARPROPERTY_EXPORTS
			#define LIBBASARPROPERTY_API __declspec(dllexport)	//!< dll exported
		#else
			#define LIBBASARPROPERTY_API __declspec(dllimport)	//!< dll imported
		#endif 
	#else 
		#define LIBBASARPROPERTY_API
	#endif
#endif

//--------------------------------------------------------------------------------------------//
// includes
//--------------------------------------------------------------------------------------------//
#include "libbasarproperty_propertystateset.h"
#include "libbasarproperty_definitions.h"

//--------------------------------------------------------------------------------------------//
// namespaces
//--------------------------------------------------------------------------------------------//
namespace basar
{
namespace property
{

//--------------------------------------------------------------------------------------------//
// forward declaration
//--------------------------------------------------------------------------------------------//
class PropertyTable_XIterator;


//////////////////////////////////////////////////////////////////////////////////////////////////////////////
/*! \brief  class PropertyTable_YIterator represents an iterator pointing 
			to a collection of property internals (whole row)
                        
    This is a final class */
//////////////////////////////////////////////////////////////////////////////////////////////////////////////
// class declaration
//////////////////////////////////////////////////////////////////////////////////////////////////////////////
class PropertyTable_YIterator
{
    typedef CollPropertyInternalRow::difference_type Difference_Type; //!< collection type of property internal row
	//--------------------------------------------------------------------------------------------//
	// friend section
	//--------------------------------------------------------------------------------------------//
	friend class PropertyTable;
	friend class PropertyTableYIteratorSnapshot;

public:

	//--------------------------------------------------------------------------------------------//

	/*! \brief	default constructor with no special action. \n no-throw */
	LIBBASARPROPERTY_API PropertyTable_YIterator();
	
	/*! \brief	overloaded constructor. It differs from the default constructor insofar as the passed 
				propertystateset will be set. The default constructor will set the 
				propertystateset member to the value 'SS_UNKNOWN'. \n no-throw */
	LIBBASARPROPERTY_API PropertyTable_YIterator( const PropertyStateSet & rhs );

	/*! \brief	copy constructor with special manner. The inner filter 'PropertyStateSet m_Pss'
				is copied only if the filter is set, that means if the filter doesn't have the states
				'SS_UNKNOWN' or 'SS_UNSET'. \n no-throw */
	LIBBASARPROPERTY_API PropertyTable_YIterator( const PropertyTable_YIterator & rhs );

	/*! \brief	implicit copy constructor expanded with a passed propertystateset object.
				This propertystateset object will be set to the private propertystateset member. 
				\n no-throw */
	LIBBASARPROPERTY_API PropertyTable_YIterator( const PropertyTable_YIterator & rhs, 
												  const PropertyStateSet        & pss );

	/*! \brief	overloaded constructor with the class which aggregates the container as shared pointer, 
				and the pure/real iterator of the container.  \n no-throw */
	LIBBASARPROPERTY_API PropertyTable_YIterator( const PropertyTableSharedPtr      pt, 
												  const CollPropertyInternalRowIter it );

	//--------------------------------------------------------------------------------------------------------------//
	// operators
	//--------------------------------------------------------------------------------------------------------------//
	/*! \brief	assignment operator with special manner. The inner filter 'PropertyStateSet m_Pss'
				is copied only if the filter is set, that means if the filter doesn't have the states
				'SS_UNKNOWN' or 'SS_UNSET'. \n no-throw */
	LIBBASARPROPERTY_API 
	PropertyTable_YIterator &	operator =  ( const PropertyTable_YIterator& rhs );

	/*! \brief	checks the equality of this in comparison with the passed yiterator  
		\n InvalidIteratorException, DifferentContainersIteratorException */
    LIBBASARPROPERTY_API bool	operator == ( const PropertyTable_YIterator& rhp ) const;
	/*! \brief	checks the unequality of this in comparison with the passed yiterators  \n no-throw */
    LIBBASARPROPERTY_API bool	operator != ( const PropertyTable_YIterator& rhp ) const;
	/*! \brief	switches forward the iterator (preincrement)  \n OutOfRangeIteratorException */
	LIBBASARPROPERTY_API 
	PropertyTable_YIterator&	operator ++ ();
	/*! \brief	switches forward the iterator (postincrement)  \n OutOfRangeIteratorException */
    LIBBASARPROPERTY_API 
	PropertyTable_YIterator		operator ++ ( int i );
	/*! \brief	switches backward the iterator (preincrement)  \n OutOfRangeIteratorException */
	LIBBASARPROPERTY_API 
	PropertyTable_YIterator&	operator -- ();
	/*! \brief	switches backward the iterator (postincrement)  \n OutOfRangeIteratorException */
    LIBBASARPROPERTY_API 
	PropertyTable_YIterator		operator -- ( int i );
	/*! \brief	returns an iterator pointing to the very first element in the container.  \n InvalidIteratorException */
	LIBBASARPROPERTY_API 
	PropertyTable_XIterator		begin       () const;
	/*! \brief	returns an iterator pointing to the first element with the passed state in pss.  \n no-throw */
    LIBBASARPROPERTY_API 
	PropertyTable_XIterator		begin       ( const PropertyStateSet& pss );
	
	/*! \brief	returns an iterator pointing to the element position diff in the container.  \n InvalidIteratorException */
	LIBBASARPROPERTY_API
	PropertyTable_XIterator     at          (Difference_Type diff) const;
	
    /*! \brief	returns an iterator pointing directly past the last element in the container.  \n no-throw */
	LIBBASARPROPERTY_API 
	PropertyTable_XIterator		end         () const;

	/*! \brief checks if iterator points to begin() of the inner container
		\n     no-throw */
	LIBBASARPROPERTY_API bool	isBegin     () const;
	/*! \brief checks if iterator points to end() of the inner container
		\n     no-throw */
	LIBBASARPROPERTY_API bool	isEnd       () const;

    /*! \brief returns the PropertyTable_XIterator at columnindex
		\n     no-throw */
    LIBBASARPROPERTY_API 
	PropertyTable_XIterator		getXIterator(const ColumnIndex columnindex) const;

    /*! \brief returns the PropertyTable_XIterator at column columnname
		\n     no-throw */
    LIBBASARPROPERTY_API 
	PropertyTable_XIterator		getXIterator(const VarString & columnname) const;

	/**
	 * \brief number of Columns
	 */
	LIBBASARPROPERTY_API 
	NumberColumns				size		() const;
	
    //--------------------------------------------------------------------------------------------------------------//
	// contains-methods
	//--------------------------------------------------------------------------------------------------------------//
	/*! \brief	checks if the passed column name (PropertyInternal) is in the row  \n no-throw */
	LIBBASARPROPERTY_API bool	contains( const ColumnName   & colName ) const;
	/*! \brief	checks if the column name with the passed state(PropertyInternal) is in the row  \n no-throw */
	LIBBASARPROPERTY_API bool	contains( const ColumnName   & colName, 
									      const PropertyType & pt ) const;
	/*! \brief	checks if the column name with the passed state (PropertyInternal) is in the row 
				in the form of a pair  \n no-throw */
	LIBBASARPROPERTY_API bool	contains( const std::pair<ColumnName, PropertyType> & p ) const;
	/*! \brief	checks if the column name with the passed state (PropertyInternal) exists
				in the passed set of states (PropertyStateSet)  \n no-throw */
	LIBBASARPROPERTY_API bool	contains( const ColumnName       & colName, 
									      const PropertyType     & pt, 
									      const PropertyStateSet & pss ) const;
	/*! \brief	checks if the column name with the passed state (PropertyInternal) exists
				in the passed set of states (PropertyStateSet) - 
				columnname and its state is passed as a pair.  \n no-throw */
	LIBBASARPROPERTY_API bool	contains( const std::pair<ColumnName, PropertyType> & p, 
									      const PropertyStateSet                    & pss ) const;


	//--------------------------------------------------------------------------------------------------------------//
	// filter- and type-methods
	//--------------------------------------------------------------------------------------------------------------//
	/*! \brief	returns number of steps caused by filtered iteration.  \n no-throw */
	LIBBASARPROPERTY_API 
	UInt32				        getFilteredStep	() const;
	
	/*! \brief	returns the filter (member of PropertyStateSet).  \n no-throw */
    LIBBASARPROPERTY_API 
	const PropertyStateSet &	getFilter		() const;
	/*! \brief	returns the type of a certain XIterator (pointing to a column), 
				identified via a column name in the context of an YIterator.  \n UnknownColumnNameException */
    LIBBASARPROPERTY_API 
	const PropertyType&			getType			(  const ColumnName & rColName ) const;

    LIBBASARPROPERTY_API
    const PropertyType&         getType         ( const ColumnIndex index ) const;

	/*! \brief	returns the state of a certain XIterator (pointing to a column), 
				identified via a column name in the context of an YIterator.  \n UnknownColumnNameException */
    LIBBASARPROPERTY_API 
	const PropertyState&		getState		( const ColumnName & rColName ) const;
	
	/*! \brief	returns the state of a certain XIterator (pointing to a column), 
				identified via a column index in the context of an YIterator.  \n UnknownColumnNameException */
    LIBBASARPROPERTY_API 
	const PropertyState&		getState		( const ColumnIndex index ) const;
		
	/*! \brief	returns the states (serveral states possible) of 
				the whole row (collection of PropertyInternals).  \n no-throw */
	LIBBASARPROPERTY_API 
	const PropertyStateSet		getState		() const;

	/*! \brief	checks if the state/states of the complete row is/are equal to one of the passed ones  \n no-throw */
	LIBBASARPROPERTY_API bool	isInState		( const PropertyStateSet & pss ) const;
	/*! \brief	checks if the state of a ceratin column (the passed columnname identifies an certain column)
				is equal to one of the passed ones  \n no-throw */
	LIBBASARPROPERTY_API bool	isInState		( const ColumnName       & rColName, 
												  const PropertyStateSet & pss) const;
	/*! \brief	sets the passed state to the xiterator determined by the passed column name.\n throws basar exceptions */
	LIBBASARPROPERTY_API 
	void						setPropertyState( const ColumnName       & rColName, 
												  const SupportedStateEnum eState );

	/*! \brief	sets the passed state to the xiterator determined by the passed column name.\n throws basar exceptions */
	LIBBASARPROPERTY_API 
	void						setPropertyState( const ColumnIndex      & rColName, 
												  const SupportedStateEnum eState );



	//--------------------------------------------------------------------------------------------------------------//
	// higher level
	//--------------------------------------------------------------------------------------------------------------//
	/*! \brief	returns true, if given property is not in unset state and yiterator is not at the end 
				\n no-throw */
	LIBBASARPROPERTY_API bool	isNotEndAndSet		( const ColumnName & rColName ) const;
	
	/*! \brief	returns true, if yiterator contains given property and property is not in unset state 
				\n no-throw */
	LIBBASARPROPERTY_API bool	isContainedAndSet	( const ColumnName & rColName ) const;

	//-----------//
	// INT
	//-----------//
	/*! \brief	returns the value of a certain column (via columnname) as a 2 byte integer  
		\n WrongPropertyTypeException, WrongOperationException, FatalErrorException */
	LIBBASARPROPERTY_API const Int16&   getInt16	( const ColumnName& rColName ) const;
	/*! \brief	returns the value of a certain column (via index) as a 2 byte integer  
		\n WrongPropertyTypeException, WrongOperationException, FatalErrorException */
	LIBBASARPROPERTY_API const Int16&   getInt16	( const ColumnIndex index ) const;
    /*! \brief	sets the value of a certain column (via columnname) as a 2 byte integer  
		\n no-throw */
	LIBBASARPROPERTY_API void   setInt16	( const ColumnName & rColName, 
											  const Int16        value );
	/*! \brief	sets the value of a certain column (via index) as a 2 byte integer
			\n no-throw */
    LIBBASARPROPERTY_API void   setInt16	( const ColumnIndex index, 
											  const Int16       value );
	/*! \brief	returns the value of a certain column as a 4 byte integer
		\n WrongPropertyTypeException, WrongOperationException, FatalErrorException */
	LIBBASARPROPERTY_API const Int32&  getInt32	( const ColumnName& rColName ) const;
	/*! \brief	returns the value of a certain column (via index) as a 4 byte integer
		\n WrongPropertyTypeException, WrongOperationException, FatalErrorException */
	LIBBASARPROPERTY_API const Int32&  getInt32	( const ColumnIndex index ) const;

    /*! \brief	sets the value of a certain column (via columnname) as a 4 byte integer  \n no-throw */
	LIBBASARPROPERTY_API void	setInt32	( const ColumnName & rColName, 
											  const Int32        value );
    /*! \brief	sets the value of a certain column (via index) as a 4 byte integer  \n no-throw */
	LIBBASARPROPERTY_API void	setInt32	( const ColumnIndex index, 
											  const Int32       value );
    /*! \brief	returns the value of a certain column as a 8 byte integer
    \n WrongPropertyTypeException, WrongOperationException, FatalErrorException */
    LIBBASARPROPERTY_API const Int64&  getInt64(const ColumnName& rColName) const;
    /*! \brief	returns the value of a certain column (via index) as a 8 byte integer
    \n WrongPropertyTypeException, WrongOperationException, FatalErrorException */
    LIBBASARPROPERTY_API const Int64&  getInt64(const ColumnIndex index) const;

    /*! \brief	sets the value of a certain column (via columnname) as a 8 byte integer  \n no-throw */
    LIBBASARPROPERTY_API void	setInt64(const ColumnName & rColName,
                                         const Int64        value);
    /*! \brief	sets the value of a certain column (via index) as a 8 byte integer  \n no-throw */
    LIBBASARPROPERTY_API void	setInt64(const ColumnIndex index,
                                         const Int64       value);
	//-----------//
	// FLOAT
	//-----------//
	/*! \brief	returns the value of a certain column (via columnname) as a 4 byte float
		\n WrongPropertyTypeException, WrongOperationException, FatalErrorException */
	LIBBASARPROPERTY_API const Float32&    getFloat32	( const ColumnName & rColName ) const;
	/*! \brief	returns the value of a certain column (via index) as a 4 byte float
		\n WrongPropertyTypeException, WrongOperationException, FatalErrorException */
	LIBBASARPROPERTY_API const Float32&	getFloat32	( const ColumnIndex index ) const;
    /*! \brief	sets the value of a certain column (via columnname) as a 4 byte float  \n no-throw */
	LIBBASARPROPERTY_API void		setFloat32	( const ColumnName & rColName, 
													  const Float32      value );	
	/*! \brief	sets the value of a certain column (via index) as a 4 byte float  \n no-throw */
	LIBBASARPROPERTY_API void	    setFloat32	( const ColumnIndex index, 
													  const Float32     value );	
	/*! \brief	returns the value of a certain column (via columnname) as an 8 byte float
		\n WrongPropertyTypeException, WrongOperationException, FatalErrorException */
	LIBBASARPROPERTY_API const Float64&	getFloat64	( const ColumnName & rColName ) const;
	/*! \brief	returns the value of a certain column (via index) as an 8 byte float
		\n WrongPropertyTypeException, WrongOperationException, FatalErrorException */
	LIBBASARPROPERTY_API const Float64&	getFloat64	( const ColumnIndex index ) const;
    /*! \brief	sets the value of a certain column (via columnname) as an 8 byte float  \n no-throw */
	LIBBASARPROPERTY_API void		setFloat64	( const ColumnName & rColName, 
													  const Float64      value );	
	/*! \brief	sets the value of a certain column (via index) as an 8 byte float  \n no-throw */
	LIBBASARPROPERTY_API void		setFloat64	( const ColumnIndex index, 
												  const Float64     value );	
		
	//-----------//
	// DECIMAL
	//-----------//
	/*! \brief	returns the value of a certain column (via columnname) as a Decimal
		\n WrongPropertyTypeException, WrongOperationException, FatalErrorException */
	LIBBASARPROPERTY_API 
	const Decimal &					getDecimal	( const ColumnName & rColName ) const;
	/*! \brief	returns the value of a certain column (via index) as a Decimal
		\n WrongPropertyTypeException, WrongOperationException, FatalErrorException */
	LIBBASARPROPERTY_API 
	const Decimal &					getDecimal	( const ColumnIndex index ) const;
    /*! \brief	sets the value of a certain column (via columnname) as a Decimal  \n no-throw */
	LIBBASARPROPERTY_API void		setDecimal	( const ColumnName & rColName, 
												  const Decimal    & value   );	
	/*! \brief	sets the value of a certain column (via index) as a Decimal  \n no-throw */
	LIBBASARPROPERTY_API void		setDecimal	( const ColumnIndex index, 
												  const Decimal   & value );	

	//-----------//
	// STRING
	//-----------//
	/*! \brief	returns the value of a certain column (via columnname) as a string
		\n WrongPropertyTypeException, WrongOperationException, FatalErrorException */
	LIBBASARPROPERTY_API 
	const I18nString &				getString	( const ColumnName & rColName ) const;
	/*! \brief	returns the value of a certain column (via index) as a string
		\n WrongPropertyTypeException, WrongOperationException, FatalErrorException */
	LIBBASARPROPERTY_API 
	const I18nString &				getString	( const ColumnIndex index ) const;
    /*! \brief	sets the value of a certain column (via columnname) as a string  \n no-throw */
    
	LIBBASARPROPERTY_API void		setString	( const ColumnName & rColName, 
												  const I18nString & value );
	/*! \brief	sets the value of a certain column (via index) as a string   \n no-throw */
	LIBBASARPROPERTY_API void		setString	( const ColumnIndex  index, 
												  const I18nString & value );
	
	/*! \brief	sets the value of a certain column (via columnname) as a string - Overload for delaying copy \n no-throw */											  
	LIBBASARPROPERTY_API void		setString	( const ColumnName & rColName, 
												  ConstBuffer value );
	/*! \brief	sets the value of a certain column (via index) as a string - Overload for delaying copy  \n no-throw */
	LIBBASARPROPERTY_API void		setString	( const ColumnIndex  index, 
												  ConstBuffer value );
												  
	/*! \brief	returns the value of a certain column (via columnname) as a string 
				prepared for the use in a sql statement  \n WrongPropertyTypeException */
	LIBBASARPROPERTY_API 
	const VarString					getSQLString( const ColumnName & rColName ) const;
	
	
	//-----------//
	// DATETIME
	//-----------//
	/*! \brief	returns the value of a certain column (via columnname) as DateTime
				\n WrongPropertyTypeException, WrongOperationException, FatalErrorException */
	LIBBASARPROPERTY_API const DateTime &	getDateTime	( const ColumnName & rColName ) const;
	/*! \brief	returns the value of a certain column (via index) as DateTime
				\n WrongPropertyTypeException, WrongOperationException, FatalErrorException */
	LIBBASARPROPERTY_API const DateTime &	getDateTime	( const ColumnIndex index ) const;
    /*! \brief	sets the value of a certain column (via columnname) as DateTime  \n no-throw */
	LIBBASARPROPERTY_API void				setDateTime	( const ColumnName & rColName, 
														  const DateTime   & value );	
	/*! \brief	sets the value of a certain column (via index) as DatTime \n no-throw */
	LIBBASARPROPERTY_API void				setDateTime	( const ColumnIndex index, 
														  const DateTime  & value );	

    //-----------//
	// TIMESPAN
	//-----------//
	/*! \brief	returns the value of a certain column (via columnname) as TimeSpan
				\n WrongPropertyTypeException, WrongOperationException, FatalErrorException */
	LIBBASARPROPERTY_API const TimeSpan &	getTimeSpan	( const ColumnName & rColName ) const;
	/*! \brief	returns the value of a certain column (via index) as TimeSpan
				\n WrongPropertyTypeException, WrongOperationException, FatalErrorException */
	LIBBASARPROPERTY_API const TimeSpan &	getTimeSpan	( const ColumnIndex index ) const;
    /*! \brief	sets the value of a certain column (via columnname) as TimeSpan  \n no-throw */
	LIBBASARPROPERTY_API void				setTimeSpan	( const ColumnName & rColName, 
														  const TimeSpan   & value );	
	/*! \brief	sets the value of a certain column (via index) as TimeSpan \n no-throw */
	LIBBASARPROPERTY_API void				setTimeSpan	( const ColumnIndex index, 
														  const TimeSpan  & value );	

	//-----------//
	// DATE
	//-----------//
	/*! \brief	returns the value of a certain column (via columnname) as DateTime with time 0.
				\n WrongPropertyTypeException, WrongOperationException, FatalErrorException */
	LIBBASARPROPERTY_API const Date &	getDate	( const ColumnName & rColName ) const;
	/*! \brief	returns the value of a certain column (via index) as DateTime with time 0.
				\n WrongPropertyTypeException, WrongOperationException, FatalErrorException */
	LIBBASARPROPERTY_API const Date &	getDate	( const ColumnIndex index ) const;
    /*! \brief	sets the value of a certain column (via columnname) as DateTime with time 0.
				\n no-throw */
	LIBBASARPROPERTY_API void			setDate	( const ColumnName & rColName, 
												  const Date       & value );	
	/*! \brief	sets the value of a certain column (via index) as DateTime with time 0.
				\n no-throw */
	LIBBASARPROPERTY_API void			setDate	( const ColumnIndex index, 
												  const Date      & value );	
	

	//-----------//
	// TIME
	//-----------//
	/*! \brief	returns the value of a certain column (via columnname) as DateTime with date 0.
				\n WrongPropertyTypeException, WrongOperationException, FatalErrorException */
	LIBBASARPROPERTY_API const Time &	getTime	( const ColumnName & rColName ) const;
	/*! \brief	returns the value of a certain column (via index) as DateTime with date 0.
				\n WrongPropertyTypeException, WrongOperationException, FatalErrorException */
	LIBBASARPROPERTY_API const Time &	getTime	( const ColumnIndex index ) const;
    /*! \brief	sets the value of a certain column (via columnname) as DateTime with date 0.
				\n no-throw */
	LIBBASARPROPERTY_API void			setTime	( const ColumnName & rColName, 
												  const Time       & value );	
	/*! \brief	sets the value of a certain column (via index) as DateTime with date 0.
				\n no-throw */
	LIBBASARPROPERTY_API void			setTime	( const ColumnIndex index, 
												  const Time      & value );
    //-----------//
    // BINARY
    //-----------//
    /*! \brief	returns the value of a certain column (via columnname) as Binary.
    \n WrongPropertyTypeException, WrongOperationException, FatalErrorException */
    LIBBASARPROPERTY_API const Binary &	getBinary(const ColumnName & rColName) const;
    /*! \brief	returns the value of a certain column (via index) as Binary.
    \n WrongPropertyTypeException, WrongOperationException, FatalErrorException */
    LIBBASARPROPERTY_API const Binary &	getBinary(const ColumnIndex index) const;
    /*! \brief	sets the value of a certain column (via columnname) as Binary
    \n no-throw */
    LIBBASARPROPERTY_API void			setBinary(const ColumnName & rColName,
                                                const Binary       & value);
    /*! \brief	sets the value of a certain column (via index) as Binary
    \n no-throw */
    LIBBASARPROPERTY_API void			setBinary(const ColumnIndex index,
                                                const Binary      & value);
	
	/*! \brief	returns the position of the given column.
				\n UnknownColumnNameException, WrongTypeException */
	LIBBASARPROPERTY_API ColumnIndex	getIndex		( const ColumnName& rColName ) const;
	/*! \brief	returns the type used with inserting the row  \n no-throw */
	LIBBASARPROPERTY_API InsertTypeEnum	getInsertType	() const;

	/*! \brief	iterates all column names and corresponding property types
				printing them via std::cout by default. Could be overwritten by the std::ostream parameter.
				\n     no-throw */
	LIBBASARPROPERTY_API void					toStream		( std::ostream & strm = std::cout ) const;
	
	/*! \brief	checks if property table shared pointer is initialized  \n no-throw */
	LIBBASARPROPERTY_API bool				    isNull			() const;

	/*! \brief clear and reset propertytable member \n no-throw */
    LIBBASARPROPERTY_API void					reset			();

	/*! \brief clear and reset propertytable member \n no-throw */
	LIBBASARPROPERTY_API void					clear			();

    /*! returns the name of the column */
    LIBBASARPROPERTY_API ColumnName				getName			(ColumnIndex index	//!< index
																) const;
    /*! resets row state to SS_UNSET */
    LIBBASARPROPERTY_API void                   resetRowState();

protected:


private:
	//--------------------------------------------------------------------------------------------//
	// private methods
	//--------------------------------------------------------------------------------------------//

	/*! \brief	sets the passed state to every single column (PropertyInternals)
				so that the complete row has a defined state \n no-throw */
	void				setState		( const SupportedStateEnum eState );

    void                setInsertType( InsertTypeEnum type );

	/*! \brief	checks if the iterator is accessed with PropertyTable initialization before 
	    \n      NullReferenceException */
	void				checkPropTbl	() const; 
	
	/*! \brief	private help method to assign the members of the passed object 
				of type PropertyTable_YIterator. It is used in the copy constructor 
				and the assignment operator. \n no-throw */
	void				assign			( const PropertyTable_YIterator & rhs );


	//--------------------------------------------------------------------------------------------//
	// private members
	//--------------------------------------------------------------------------------------------//
	
	PropertyTableSharedPtr		m_Pt;							//!< shared pointer to the container
	CollPropertyInternalRowIter	m_It;							//!< iterator to pair of inserttype and PropertyInternals
	PropertyStateSet			m_Pss;							//!< holds the possible states for filtered iteration
	UInt32						m_FilteredStep;					//!< holds the possible states for filtered iteration
}; // END class

//--------------------------------------------------------------------------------------------//
} // END namespace property
} // END namespace basar


//--------------------------------------------------------------------------------------------//
#endif // GUARD_PROPERTYTABLE_YITERATOR_H
