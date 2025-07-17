#ifndef GUARD_PROPERTYDESCRIPTIONLISTREF_H
#define GUARD_PROPERTYDESCRIPTIONLISTREF_H
//--------------------------------------------------------------------------------------------//
/*! \file
 *  \brief  This class PropertyDescriptionListRef is a special proxy (smart reference) 
			to the class PropertyDescriptionList.
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
class  PropertyDescriptionList_Iterator;
class  PropertyDescription;
struct PropertyDescriptionVector;

//----------------------------------------------------------------------------
//! \brief dump PropertyDescriptionListRef object's content to debug output
//!
//! \note  use in Visual Studio's "Command Window - Immediate" following input (OBJECT_ADDRESS e.g.: 0x0012fa94) :
//! \code  {,,libbasarproperty.dll} basar::property::dumpPropDescList ( OBJECT_ADDRESS )
//! \endcode
LIBBASARPROPERTY_API void dumpPropDescList	(size_t addr	//!< object address
											);

//--------------------------------------------------------------------------------------------//
// class declaration
//--------------------------------------------------------------------------------------------//
///////////////////////////////////////////////////////////////////////////////////
/*! \brief  This class PropertyDescriptionListRef is a special proxy (smart reference) 
			to the class PropertyDescriptionList.

			Class PropertyDescriptionListRef holds a smart (shared) pointer to its 
			implementation (class PropertyDescriptionList). 
			So the reference-class can be copied as many times as necessary without any doubts about 
			memory allocation or leaks.
            Class throws all the exceptions the implementation throws. */
class PropertyDescriptionListRef
{	
public:
    //! \brief standard constructor with no special action \n no-throw
	LIBBASARPROPERTY_API PropertyDescriptionListRef();
    
    /*! \brief	overloaded constructor with a shared pointer to the implementation object 
				\n no-throw */
	LIBBASARPROPERTY_API PropertyDescriptionListRef( const PdlSharedPtr pImpl );
    
	//! checks if the implementation is instantiated
    LIBBASARPROPERTY_API bool			isNull() const;	
    //! delegates the operation to the shared pointer and sets it to null
	LIBBASARPROPERTY_API void			reset();

	//! builds the datatypes and columnnames from one successional string
	LIBBASARPROPERTY_API void			construct( const VarString& rValuePairs );

    	//! builds the datatypes and columnnames from one successional string
	LIBBASARPROPERTY_API void			construct( const PropertyDescriptionVector& rValuePairs );

  
    //! adds the new column with the given state 
	LIBBASARPROPERTY_API void			push_back(	const ColumnName& rName,  //!< name
													const PropertyType pType  //!< type
                                                    );
    //! adds the new column with the given state in the form of a pair
    LIBBASARPROPERTY_API void			push_back( 
        const ColNamePropTypePair& pair //!< name and type
        );

    //! adds the new column with the given state in the form of a pair
    LIBBASARPROPERTY_API void			push_back( 
        const PropertyDescription & desc //!< name and type definition
        );

    //! returns the number of the columns
	LIBBASARPROPERTY_API 
	NumberColumns					    size() const;

    //! \brief checks if the passed ColumnName matches with one contained.
    //    \n no-throw
	LIBBASARPROPERTY_API bool			contains( const ColumnName& rName ) const; 
    /*! \brief	checks if the passed ColumnName matches with one contained.
				The second parameter needn't be passed - in this case a default object value 
				PropertyType is set with the default value PropertyType::UNKNOWN
				\n no-throw */
	LIBBASARPROPERTY_API bool			contains(	const ColumnName& rName,  //!< name
													const PropertyType& rPropType //!< type
                                                    ) const; 

    /*! \brief	checks if the passed std::pair<ColumnName, PropertyType> matches with one contained.
				\n no-throw */
    LIBBASARPROPERTY_API bool			contains( 
        const ColNamePropTypePair& pair //!< name and type
        ) const;                           

    //! \brief returns index of a given ColumName \n UnknownColumnNameException
	LIBBASARPROPERTY_API 
	ColumnIndex					        getIndex( const ColumnName& name ) const;  

    //! \brief returns columnname of a given index \n OutOfRangeException
    LIBBASARPROPERTY_API 
	const ColumnName&					getName( const ColumnIndex index )const;     

    //! \brief returns propertytype of a given index \n OutOfRangeException
	LIBBASARPROPERTY_API 
	const PropertyType&					getType( const ColumnIndex index ) const;    

    /*! \brief	returns propertytype of a given ColumnName \n OutOfRangeException
				\n throws UnknownColumnNameException */
    LIBBASARPROPERTY_API 
	const PropertyType&					getType( const ColumnName& name ) const;    

    /*! \brief	calls double parameterized constructor with the this-pointer and 
				the begin()-method of the member attribut (std::vector<>) \n no-throw */
	LIBBASARPROPERTY_API 
	PropertyDescriptionList_Iterator	begin(); 

    /*! \brief	calls double parameterized constructor with the this-pointer and 
				the end()-method of the member attribut (std::vector<>) \n no-throw */
	LIBBASARPROPERTY_API 
	PropertyDescriptionList_Iterator	end();

	/*! \brief	iterates all column names and corresponding property types
				printing via std::cout by default. Could be overwritten by the std::ostream parameter.
				\n     no-throw */
	LIBBASARPROPERTY_API 
	void								toStream( std::ostream& strm = std::cout ) const;

    /*! \brief writes the data to a string (in the same 
     *         format as the construction string)
     *         f.ex. "string col1;int col2;"
     * \n     no-throw
     */
    LIBBASARPROPERTY_API      
    VarString							toString() const;

private:
	//--------------------------------------------------------------------------------------------//
	// private methods
	//--------------------------------------------------------------------------------------------//
	//! checks and returns shared pointer to implementation object \n NullReferenceException
	const PdlSharedPtr&					getImpl() const;
    //! makes a copy of the impl class and returns a reference \n no-throw
	PropertyDescriptionListRef			deepCopy();
	
	//--------------------------------------------------------------------------------------------//
	// private members
	//--------------------------------------------------------------------------------------------//
	//! stores the implementation object in the form of a shared pointer
	PdlSharedPtr						m_pImpl;

}; // END class PropertyDescriptionListRef


} // END namespace property
} // END namespace basar

#endif // GUARD_PROPERTYDESCRIPTIONLISTREF_H
