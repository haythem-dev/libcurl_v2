#ifndef GUARD_PROPERTYTABLE_XITERATOR_H
#define GUARD_PROPERTYTABLE_XITERATOR_H
//----------------------------------------------------------------------------
/*! \file
 *  \brief  class PropertyTable_XIterator represents an iterator pointing 
			to a single propertyinternal (every column)
 *  \author Bischof Björn 
 *  \date   23.08.2005
 */
//----------------------------------------------------------------------------

//----------------------------------------------------------------------------------------------------------//
// dll imports and exports
//----------------------------------------------------------------------------------------------------------//
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

//----------------------------------------------------------------------------------------------------------//
#include "libbasarproperty_propertystateset.h"
#include "libbasarproperty_definitions.h"

//----------------------------------------------------------------------------------------------------------//
// namespaces
//----------------------------------------------------------------------------------------------------------//
namespace basar
{
namespace property
{

//----------------------------------------------------------------------------------------------------------//
// forward declaration
//----------------------------------------------------------------------------------------------------------//
class PropertyType;
class PropertyState;

//----------------------------------------------------------------------------------------------------------//
// class declaration
//----------------------------------------------------------------------------------------------------------//
//////////////////////////////////////////////////////////////////////////////////////////////////////////////
/*! \brief  class PropertyTable_XIterator represents an iterator pointing 
			to a single propertyinternal (every column)
                        
    This is a final class */
class PropertyTable_XIterator
{
	friend class PropertyTable_YIterator;

public:
	//--------------------------------------------------------------------------------------------//
	// typedef section
	//--------------------------------------------------------------------------------------------//
	typedef CollPropertyInternalRow::difference_type Difference_Type; //!< collection type of property internal row


	//----------------------------------------------------------------------------------------//
	// c'tors
	//----------------------------------------------------------------------------------------//
	/*! \brief	default constructor with no special action. \n no-throw */
	LIBBASARPROPERTY_API PropertyTable_XIterator();
    /*! \todo	this parameter 'PairInsertTypeCollPropertyInternalColumn' is a bald pointer. 
				It collides with our claim to a safe memory management. 
				But in this context it is intention. \n no-throw */

	/*! \brief	overloaded c'tor. It differs from the default constructor insofar as the passed 
				propertystateset will be set. The default constructor will set the 
				propertystateset member to the value 'SS_UNKNOWN'. \n no-throw */
	LIBBASARPROPERTY_API PropertyTable_XIterator( const PropertyStateSet & rhs );

	/*! \brief	copy constructor with special manner. The inner filter 'PropertyStateSet m_Pss'
				is copied only if the filter is set, that means if the filter doesn't have the states
				'SS_UNKNOWN' or 'SS_UNSET'. \n no-throw */
	LIBBASARPROPERTY_API PropertyTable_XIterator( const PropertyTable_XIterator & rhs );

	/*! \brief	implicit copy constructor expanded with a passed propertystateset object.
				This propertystateset object will be set to the private propertystateset member. 
				\n no-throw */
	LIBBASARPROPERTY_API PropertyTable_XIterator( const PropertyTable_XIterator & rhs, 
												  const PropertyStateSet        & pss );

	/*! \brief	overloaded constructor with the class which aggregates the container as shared pointer, 
				the type of the insert and the pure/real iterator of the container.  \n no-throw */
	LIBBASARPROPERTY_API PropertyTable_XIterator(	PropertyTableSharedPtr                     pt, 
													PairInsertTypeCollPropertyInternalColumn * p, 
													CollPropertyInternalColumnIter             it );

	//----------------------------------------------------------------------------------------//
	// operators
	//----------------------------------------------------------------------------------------//
	/*! \brief	checks the equality of this in comparison with the passed xiterator  
		\n InvalidIteratorException, DifferentContainersIteratorException */
	LIBBASARPROPERTY_API bool						operator ==	( const PropertyTable_XIterator & xit ) const;
	/*! \brief	checks the unequality of this in comparison with the passed xiterator  \n no-throw */
	LIBBASARPROPERTY_API bool						operator !=	( const PropertyTable_XIterator & rhp ) const;
	/*! \brief	switches forward the iterator (preincrement)  \n OutOfRangeIteratorException */
	LIBBASARPROPERTY_API PropertyTable_XIterator&	operator ++	();
    /*! \brief	switches forward the iterator (postincrement)  \n OutOfRangeIteratorException */
	LIBBASARPROPERTY_API PropertyTable_XIterator	operator ++	( int i );
	/*! \brief	assignment operator with special manner. The inner filter 'PropertyStateSet m_Pss'
				is copied only if the filter is set, that means if the filter doesn't have the states
				'SS_UNKNOWN' or 'SS_UNSET'. \n no-throw */
	LIBBASARPROPERTY_API PropertyTable_XIterator&	operator =	( const PropertyTable_XIterator & rhs );

	//----------------------------------------------------------------------------------------//
	// get- and set-methods
	//----------------------------------------------------------------------------------------//
	/*! \brief	sets the passed argument to the corresponding member \n no-throw */
	//LIBBASARPROPERTY_API void						setPropertyStateSet( const PropertyStateSet pss );
	/*! \brief	returns the name of the column with the help of the 
				belonging propertydescriptionlist \n no-throw */
	LIBBASARPROPERTY_API const ColumnName&			getName() const;
    /*! \brief	returns the datatype with the help of the 
				belonging propertydescriptionlist \n no-throw */
	LIBBASARPROPERTY_API const PropertyType&		getType() const;
    /*! \brief	returns the state  \n no-throw */
	LIBBASARPROPERTY_API const PropertyState&		getState() const;
    /*! \brief	returns the position of the iterator  \n no-throw */
	LIBBASARPROPERTY_API ColumnIndex			    getIndex() const;
    /*! \brief	checks if the iterator points to an object 
				which has a state contained in the passed argument pss by calling isInStateInternal \n no-throw */
	LIBBASARPROPERTY_API bool						isInState( const PropertyStateSet& pss ) const;
	/*! \brief	returns the states the iterator is traversing.  \n no-throw */
    LIBBASARPROPERTY_API const PropertyStateSet&	getFilter() const;
	/*! \brief	sets the states the iterator is traversing.  \n no-throw */
    LIBBASARPROPERTY_API void						setFilter( const PropertyStateSet& pss );
	/*! \brief	lets the iterator jump to another iterator with a given distance.  \n no-throw */
	LIBBASARPROPERTY_API PropertyTable_XIterator	skip( PropertyTable_XIterator::Difference_Type difftype 
														) const;
	/*! \brief	returns the value as a string with simple inverted commas 
				that can be used in SQL-strings.  \n FatalErrorException */
    LIBBASARPROPERTY_API const SQLString			getSQLString() const;
    
	/*! \brief	sets the passed state to the corresponding method.  \n InvalidParameterException */
	LIBBASARPROPERTY_API void						setPropertyState( const SupportedStateEnum eState );



	//-----------------//
	// Int
	//-----------------//
    /*! \brief	returns the value the iterator points to as an INT16.    
		\n WrongPropertyTypeException, WrongOperationException */
    LIBBASARPROPERTY_API const Int16&		        getInt16() const;
	/*! \brief	sets the value the iterator points to as an INT16.  \n WrongPropertyTypeException */
    LIBBASARPROPERTY_API void						setInt16( const Int16& value );
	/*! \brief	returns the value the iterator points to as an INT32.    
		\n WrongPropertyTypeException, WrongOperationException */
    LIBBASARPROPERTY_API const Int32&		        getInt32() const;
    
	/*! \brief	sets the value the iterator points to as an INT32.  \n WrongPropertyTypeException */
    LIBBASARPROPERTY_API void						setInt32( const Int32& value );

    /*! \brief	returns the value the iterator points to as an INT32.
    \n WrongPropertyTypeException, WrongOperationException */
    LIBBASARPROPERTY_API const Int64&		        getInt64() const;

    /*! \brief	sets the value the iterator points to as an INT32.  \n WrongPropertyTypeException */
    LIBBASARPROPERTY_API void						setInt64(const Int64& value);
	
	//-----------------//
	// Float
	//-----------------//	
	/*! \brief	returns the value the iterator points to as a FLOAT32.  
		\n WrongPropertyTypeException, WrongOperationException */
	LIBBASARPROPERTY_API const Float32&			    getFloat32() const;
	/*! \brief	sets the value the iterator points to as a FLOAT32.  \n WrongPropertyTypeException */
	LIBBASARPROPERTY_API void						setFloat32( const Float32& value );
	/*! \brief	returns the value the iterator points to as a FLOAT64.    
		\n WrongPropertyTypeException, WrongOperationException */
	LIBBASARPROPERTY_API const Float64&			    getFloat64() const;
	/*! \brief	sets the value the iterator points to as a FLOAT64. \n WrongPropertyTypeException  */
	LIBBASARPROPERTY_API void						setFloat64( const Float64& value );
	
	//-----------------//
	// Decimal
	//-----------------//	
	/*! \brief	sets the value the iterator points to as a Decimal. \n WrongPropertyTypeException  */
	LIBBASARPROPERTY_API void						setDecimal( const Decimal& value );
	/*! \brief	returns the value the iterator points to as a Decimal.  
		\n WrongPropertyTypeException, WrongOperationException */
	LIBBASARPROPERTY_API const Decimal&				getDecimal() const;

	//-----------------//
	// DateTime
	//-----------------//	
	/*! \brief	sets the value the iterator points to as a DateTime. \n WrongPropertyTypeException  */
	LIBBASARPROPERTY_API void						setDateTime( const DateTime& value );
	/*! \brief	returns the value the iterator points to as a DateTime.  
				\n WrongPropertyTypeException, WrongOperationException */
	LIBBASARPROPERTY_API const DateTime&			getDateTime() const;

	//-----------------//
	// TimeSpan
	//-----------------//	
	/*! \brief	sets the value the iterator points to as a TimeSpan. \n WrongPropertyTypeException  */
	LIBBASARPROPERTY_API void						setTimeSpan( const TimeSpan& value );
	/*! \brief	returns the value the iterator points to as a TimeSpan.  
				\n WrongPropertyTypeException, WrongOperationException */
	LIBBASARPROPERTY_API const TimeSpan&			getTimeSpan() const;

	//-----------------//
	// Date
	//-----------------//	
	/*! \brief	sets the value the iterator points to as a DateTime with time 0. 
				\n WrongPropertyTypeException  */
	LIBBASARPROPERTY_API void						setDate( const Date& value );
	/*! \brief	returns the value the iterator points to as a DateTime with time 0.  
				\n WrongPropertyTypeException, WrongOperationException */
	LIBBASARPROPERTY_API const Date&				getDate() const;

	//-----------------//
	// Time
	//-----------------//	
	/*! \brief	sets the value the iterator points to as a DateTime with time 0. 
				\n WrongPropertyTypeException  */
	LIBBASARPROPERTY_API void						setTime( const Time& value );
	/*! \brief	returns the value the iterator points to as a DateTime with time 0.  
				\n WrongPropertyTypeException, WrongOperationException */
	LIBBASARPROPERTY_API const Time&				getTime() const;

    //-----------------//
    // Binary
    //-----------------//	
    /*! \brief	sets the value the iterator points to as a Binary
    \n WrongPropertyTypeException  */
    LIBBASARPROPERTY_API void						setBinary(const Binary& value);
    /*! \brief	returns the value the iterator points to as a Binary
    \n WrongPropertyTypeException, WrongOperationException */
    LIBBASARPROPERTY_API const Binary&				getBinary() const;

	//-----------------//
	// String
	//-----------------//	
	/*! \brief	sets the value the iterator points to as a STRING. \n WrongPropertyTypeException  */
    LIBBASARPROPERTY_API void						setString( const I18nString & value );

	/*! \brief	sets the value the iterator points to as a STRING. Overload for delaying copy \n WrongPropertyTypeException  */
    LIBBASARPROPERTY_API void						setString( ConstBuffer value );
    
	/*! \brief	returns the value the iterator points to as a STRING.   
		\n WrongPropertyTypeException, WrongOperationException */
    LIBBASARPROPERTY_API const I18nString&			getString() const;
	
	/*! \brief is Null Value */
	LIBBASARPROPERTY_API bool isNull() const;

	/*! \brief resets to Null value */
	LIBBASARPROPERTY_API void reset();

	/*! \brief checks if iterator points to begin() of the inner container */
    LIBBASARPROPERTY_API bool isBegin() const;

	/*! \brief checks if iterator points to end() of the inner container */
    LIBBASARPROPERTY_API bool isEnd() const;

private:
	//--------------------------------------------------------------------------------------------//
	// private methods
	//--------------------------------------------------------------------------------------------//

	/*! \brief	privateized operation doing the same as skip, 
				but skip is easier to read/understand for the client.  \n no-throw */
		PropertyTable_XIterator						operator+( PropertyTable_XIterator::Difference_Type difftype ) const;
	/*! \brief	private help method to test the current state 
				to switch to an new valid state.  \n no-throw */
	void            								determineState();
	/*! \brief	checks if an access happened to a default constructed iterator 
				and throws an exception in this case. \n InvalidIteratorException  */
	void    										checkInitialization() const;
	/*! \brief	gets the shared pointer member of the implementation.
				This method exists because with every access to this member
				there is a check (via internal call of the method 
				'checkInitialization()') if the iterator is initialized. 
				This is the non-const version.  \n InvalidIteratorException */
	PropertyTableSharedPtr&							getPt();
	/*! \brief	gets the shared pointer member of the implementation.
				This method exists because with every access to this member
				There is a check (via internal call of the method 
				'checkInitialization()') if the iterator is initialized. 
				This is the const version.  \n InvalidIteratorException */
	const PropertyTableSharedPtr&					getPt() const;
	/*! \brief	gets the pure iterator member of the implementation container.
				This method exists because with every access to this member
				there is a check (via internal call of the method 
				'checkInitialization()') if the iterator is initialized. 
				This is the non-const version.  \n InvalidIteratorException */
    CollPropertyInternalColumnIter&					getIter();
	/*! \brief	gets the pure iterator member of the implementation container.
				This method exists because with every access to this member
				there is a check (via internal call of the method 
				'checkInitialization()') if the iterator is initialized. 
				This is the const version.  \n InvalidIteratorException */
	const CollPropertyInternalColumnIter&			getIter() const;
	/*! \brief	gets the value the pure iterator is pointing to. 
				This method exists because with every access to member
				there is a check (via internal call of the method 
				'checkInitialization()') if the iterator is initialized 
				and if the iterator is pointing to an valid object.
				This is the const version.  \n InvalidIteratorException */
	PairInsertTypeCollPropertyInternalColumn*		getInsertTypeColl();
	/*! \brief	gets the value the pure iterator is pointing to. 
				This method exists because with every access to member
				there is a check (via internal call of the method 
				'checkInitialization()') if the iterator is initialized 
				and if the iterator is pointing to an valid object.
				This is the const version.  \n InvalidIteratorException */
	const PairInsertTypeCollPropertyInternalColumn* getInsertTypeColl() const;

	/*! \brief	private help method to assign the members of the passed object 
				of type PropertyTable_XIterator. It is used in the copy constructor 
				and the assignment operator. \n no-throw */
	void											assign( const PropertyTable_XIterator& rhs );
	
	/*! \brief	returns the datatype with the help of the 
				belonging propertydescriptionlist \n this function does not check iterators validity \n no-throw */
	const PropertyType&		                        getTypeInternal() const;
	
	/*! \brief	returns the position of the iterator  \n no-throw */
	ColumnIndex                                     getIndexInteral() const;
	
    /*! \brief	checks if the iterator points to an object 
			which has a state contained in the passed argument pss \n this function does not check iterators validity \n no-throw */
    bool			                                isInStateInternal( const PropertyStateSet& pss ) const;
    
    /*! \brief	returns the state  \n no-throw */
    const PropertyState&		                    getStateInternal() const;
    
    bool                                            isEqualInternal(const PropertyTable_XIterator& xit) const;
    
    PropertyTable_XIterator&                        preincrementInteral();

	//--------------------------------------------------------------------------------------------//
	// private methods
	//--------------------------------------------------------------------------------------------//
	/*! \brief	stores the shared pointer to the implementation object.*/
    PropertyTableSharedPtr							m_Pt;
	/*! \brief	stores the type of the insert and the actual propertyinternal (value and state) */
	//! \todo this member is a bald pointer. It collides with our claim to a safe memory management.
    PairInsertTypeCollPropertyInternalColumn *		m_pInsertTypeCollColumn;
	/*! \brief	stores the pure/real stl-iterator of the corresponding container */
    CollPropertyInternalColumnIter					m_It;
    //! keep the criterion defining the current valid state/s which it is allowed to iterate over
    PropertyStateSet								m_Pss;   
    
    //caching, columns don't change
    CollPropertyInternalColumn::const_iterator      m_itPropertyInternalColumnEnd;

}; // END class PropertyTable_XIterator

} // END namespace property
} // END namespace basar


#endif // GUARD_PROPERTYTABLE_XITERATOR_H
