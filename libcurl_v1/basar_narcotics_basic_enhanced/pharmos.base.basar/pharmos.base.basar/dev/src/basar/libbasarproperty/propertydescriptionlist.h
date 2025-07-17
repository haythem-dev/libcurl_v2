#ifndef GUARD_PROPERTYDESCRIPTIONLIST_H
#define GUARD_PROPERTYDESCRIPTIONLIST_H 
//--------------------------------------------------------------------------------------------//
/*! \file
 *  \brief  class represents the column name and the datatype of the database query
 *  \author Bischof Björn 
 *  \date   25.07.2005
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
#include "libbasarcmnutil_exceptions.h"
#include "libbasarproperty_definitions.h"
#include "libbasarproperty_propertydescriptionlist_iterator.h"

//--------------------------------------------------------------------------------------------//
// namespaces
//--------------------------------------------------------------------------------------------//
namespace basar
{
namespace property
{
//--------------------------------------------------------------------------------------------//
// forward declarations
//--------------------------------------------------------------------------------------------//
class  PropertyDescription;
struct PropertyDescriptionVector;
class  PropertyType;

//--------------------------------------------------------------------------------------------//
//  class declaration
//--------------------------------------------------------------------------------------------//
/*! \brief  class PropertyDescriptionList holds a container 
			with std::pair<ColumnName, PropertyType>.
            It can also be used to hold ColumnNames only with 
			the default value 'UNKNOWN' for the PropertyType.
            
			This is a final class.     
			Class throws the follwing exceptions: 
			class UnknownColumnNameException and class OutOfRangeException. */
class PropertyDescriptionList
{	
	friend class PropertyDescriptionList_Iterator; //!< friend iterator
public:
    //--------------------------------------------------------------------------------------------//
	// static section
	//--------------------------------------------------------------------------------------------//
    //! \brief simple factory method (static method) to create a new object of this class \n no-throw
	LIBBASARPROPERTY_API static PdlSharedPtr	create();
	
	
public:
	//--------------------------------------------------------------------------------------------//
	// object setcion
	//--------------------------------------------------------------------------------------------//
    //! \brief destructor which cleans up the std::vector<>-member attribute  \n no-throw
    ~PropertyDescriptionList();
	
	/*! \brief builds all datatypes and columnnames from one successional string 
		\n throw InvalidParameterException, UnknownPropertyNameException */
	void								construct( const I18nString& rValuePairs );

    /*! \brief builds all datatypes and columnnames from one successional string 
		\n throw InvalidParameterException, UnknownPropertyNameException */
    void			                    construct( const PropertyDescriptionVector& rValuePairs );
	
    /*! \brief	possibility to pass a ColumnName that is then appended to the container. 
				Internally a dflt PropertyType. \n PropertyNameAlreadyExistsException */

    void								push_back( const ColumnName& rName );
	/*! \brief	possibility to pass a ColumnName and a PropertyType as separated arguments that are then appended
				to the container. \n PropertyNameAlreadyExistsException */
    void								push_back( const ColumnName& rName, 
												   const PropertyType& propType );

     /*! \brief possibility to pass a PropertyDescription
        \n PropertyNameAlreadyExistsException */
    void                                push_back( const PropertyDescription & pd);

    /*! \brief possibility to pass a std::pair<ColumnName, PropertyType> as one successional string
        \n PropertyNameAlreadyExistsException */
    void								push_back( const ColNamePropTypePair& rPair );
    

	/*! \brief checks if the passed ColumnName matches with one contained. \n no-throw*/
	bool								contains( const ColumnName& rName ) const; 
    /*! \brief checks if the passed ColumnName matches with one contained.
        The second parameter needn't be passed - in this case a default object value 
        PropertyType is set with the default value PropertyType::UNKNOWN
        \n no-throw*/
	bool								contains( const ColumnName& rName, 
										          const PropertyType& propType ) const; 
	/*! \brief	checks if the passed std::pair<ColumnName, PropertyType> matches with one contained.
				\n no-throw*/
    bool								contains( const ColNamePropTypePair& ) const;                           
    

    /*! \brief returns the number of elements (columnnames). \n no-throw*/
	NumberColumns						size() const;
	/*! \brief	returns the index of a given ColumName. \n UnknownColumnNameException */
	ColumnIndex					        getIndex( const ColumnName& rColName ) const;  
    /*! \brief	returns the columnname of a given index. \n OutOfRangeIndexException */
    const ColumnName&					getName( const ColumnIndex index )const;     
    /*! \brief	returns the propertytype (internal: SupportedTypeEnum) of a given index. 
				\n OutOfRangeIndexException */
	const PropertyType&					getType( const ColumnIndex index ) const;    
    /*! \brief	returns the propertytype (internal: SupportedTypeEnum) of a given columnname. 
				\n UnknownPropertyNameException */
    const PropertyType&					getType( const ColumnName& rName ) const;    
    /*! \brief	calls double parameterized constructor with the this-pointer and 
				the begin()-method of the member attribut (std::vector<>) 
				\n no-throw */
	PropertyDescriptionList_Iterator	begin();// const; 
    /*! \brief	calls double parameterized constructor with the this-pointer and 
				the end()-method of the member attribut (std::vector<>) 
				\n no-throw */
	PropertyDescriptionList_Iterator    end();// const;

    //! make copies itself and returns a shared pointer
	PdlSharedPtr						deepCopy();

	/*! \brief	iterates all column names and corresponding property types
				printing them via std::cout by default. Could be overwritten by the std::ostream parameter.
				\n     no-throw */
	void								toStream( std::ostream& strm = std::cout ) const;

     /*! \brief writes the data to a string (in the same format as the construction string)
     */
	VarString							toString(  ) const;

private:
	//--------------------------------------------------------------------------------------------//
	// private methods
	//--------------------------------------------------------------------------------------------//
	/*! \brief	returns a const shared pointer of this \n no-throw */
    const PdlSharedPtr					thisPtr() const;
	/*! \brief	returns a shared pointer of this \n no-throw */
	PdlSharedPtr						thisPtr();
	/*! \brief	calls clear-method of the member attribut (std::vector<>) \n no-throw */
	void								clear();
	/*! \brief	registers a new pointer of this class to weak pointer of this class \n no-throw */
	void								setThisPtr( PdlWeakPtr p );

    
	/*! \brief	privatize the default constructor with no special action.
				It is not allowed to construct a simple default object. 
				Always a PropertyDescriptionListRef-object has to be passed.
				\n no-throw */
	PropertyDescriptionList();

	//--------------------------------------------------------------------------------------------//
	// private members
	//--------------------------------------------------------------------------------------------//
	//! stores a weak pointer of itself 
    PdlWeakPtr							m_this_as_weak_ptr;
	//! member attribute representing the container of std::pairs<ColumnName, PropertyType>
    ColNamePropTypeColl					m_ColNamePropTypeColl;

	basar::I18nString					m_ConstructString;	//!< string with pairs given at construction to create list out of them
	

}; // END class PropertyDescriptionList

} // END namespace property
} // END namespace basar


#endif // GUARD_PROPERTYDESCRIPTIONLIST_H
