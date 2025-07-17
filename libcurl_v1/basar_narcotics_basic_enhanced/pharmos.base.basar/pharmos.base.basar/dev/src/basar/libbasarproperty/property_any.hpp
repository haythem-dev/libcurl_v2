//----------------------------------------------------------------------------//
/*! \file 
 *  \brief  template for basar any (modified boost:any)
 *  \author Bischof Bjoern
 *  \date   05.10.2006
 */
//----------------------------------------------------------------------------//

#ifndef GUARD_PROPERTY_ANY_H
#define GUARD_PROPERTY_ANY_H

#include <assert.h>
#include <boost/static_assert.hpp>
#include <libbasarcmnutil_i18nstring.h>
//----------------------------------------------------------------------------//
namespace basar		
{
namespace property	
{

//------------------------------------------------------------------------------------------------------//
/*! \brief	class combines the idea of the boost::any (dynamic allocation) and an union (static allocation)
			to store the value of a single property/attribute.

	This is a final class.
	Class throws exceptions. */
//------------------------------------------------------------------------------------------------------//
class any
{
public: 
	/*! \brief	default c'tor with initialization of the boolean member variable m_isPrimitive 
			    indicating whether a primitive datatype is on or a more complex user defined datatype. 
		\n no-throw.*/
	any () : m_isPrimitive (false)
	{
        //as we can only assign one variable in union, we'll make sure that assignee (float64) is at least one of the largest
        BOOST_STATIC_ASSERT_MSG( sizeof(content.complex_val) <= sizeof(content.float64_val), "property::any relies on the fact that 64bit float is at least as large as void*" );
	}
	
	//! \brief overloaded c'tor ready for use of any user defined datatypes. \n no-throw.
	template <typename ValueType>
	any (const ValueType & value) : m_isPrimitive (false)
	{
		content.complex_val = new holder<ValueType>(value);
	}

	//! \brief overloaded c'tor with datatype Int16. \n no-throw.
	any (basar::Int16 value) : m_isPrimitive (true)
	{
		content.int16_val = value;
	}

	//! \brief overloaded c'tor with datatype Int32. \n no-throw.
	any (basar::Int32 value) : m_isPrimitive (true)
	{
		content.int32_val = value;
	}

    //! \brief overloaded c'tor with datatype Int64. \n no-throw.
    any(basar::Int64 value) : m_isPrimitive(true)
    {
        content.int64_val = value;
    }

	//! \brief overloaded c'tor with datatype Float32. \n no-throw.
	any (basar::Float32 value) : m_isPrimitive (true)
	{
		content.float32_val = value;
	}

	//! \brief overloaded c'tor with datatype Float64. \n no-throw.
	any (basar::Float64 value) : m_isPrimitive (true)
	{
		content.float64_val = value;
	}

	/*! \brief	copy c'tor with a simple copy of the value member variable m_isPrimitive and 
				a differentiation if the union must be copied or the complex object (user defined datatype). 
		\n no-throw.*/
	any (const any & rhs) : m_isPrimitive (rhs.m_isPrimitive)
	{
		if ( m_isPrimitive )
			content	= rhs.content;
		else
			content.complex_val = rhs.content.complex_val ? rhs.content.complex_val->clone() : 0; 
	}

	//! \brief d'tor deleting the dynamically allocated. \n no-throw.
	~any ()
    {
		clean();
	}

	//! \brief	indicates if the set member (integral value member or pointer) of the union is not empty. \n no-throw
	bool empty () const
    {
		if( !m_isPrimitive )
        {
			return !content.complex_val;
        }
        else
        {
            return content.float64_val == 0.0;
        }

		return true;
    }

	/*! \brief	assign operator with any	\n no-throw.*/
	any & operator = (const any & rhs)
	{
		clean();
		m_isPrimitive = rhs.m_isPrimitive;

		if ( m_isPrimitive )
			content	= rhs.content;
		else
			content.complex_val = rhs.content.complex_val ? rhs.content.complex_val->clone() : 0;

		return *this;
	}

	/*! \brief	template based assign operator with non integral datatype. 
				The passed value is set to the concerning pointer based inner union member. 
		\n no-throw.*/
	template <typename ValueType>
	any & operator = (const ValueType & rhs)
	{
		clean();
		m_isPrimitive       = false;
		content.complex_val = new holder<ValueType> (rhs);

		return *this;
	}

	/*! \brief	template based assign operator with non integral datatype. 
				The passed value is set to the concerning pointer based inner union member. 
		\n no-throw.*/
	template <typename ValueType>
	any & operator = (const ValueType* rhs)
	{
		clean();
		m_isPrimitive       = false;
		content.complex_val = new holder<ValueType> (*rhs);

		return *this;	
	}
	


	/*! \brief	overloaded assign operator with datatype Int16. 
				The passed value is set to the concerning inner union member. 
		\n no-throw.*/
	any & operator = (const basar::Int16& rhs)
	{
		clean();
		m_isPrimitive     = true;
		content.int16_val = rhs;

		return *this;
	}

	/*! \brief	overloaded assign operator with datatype Int32.
				The passed value is set to the concerning inner union member. 
		\n no-throw.*/
	any & operator = (const basar::Int32& rhs)
	{
		clean();
		m_isPrimitive     = true;
		content.int32_val = rhs;

		return *this;
	}

    /*! \brief	overloaded assign operator with datatype Int64.
    The passed value is set to the concerning inner union member.
    \n no-throw.*/
    any & operator = (const basar::Int64& rhs)
    {
        clean();
        m_isPrimitive = true;
        content.int64_val = rhs;

        return *this;
    }

	/*! \brief	overloaded assign operator with datatype Float32.
				The passed value is set to the concerning inner union member. 
		\n no-throw.*/
	any & operator = (const basar::Float32& rhs)
	{
		clean();
		m_isPrimitive       = true;
		content.float32_val = rhs;

		return *this;
	}

	/*! \brief	overloaded assign operator with datatype Float64.
				The passed value is set to the concerning inner union member. 
		\n no-throw.*/
	any & operator = (const basar::Float64& rhs)
	{
		clean();
		m_isPrimitive       = true;
		content.float64_val = rhs;

		return *this;
	}


	//! \brief	returns the value of the inner union member of datatype Int16. \n no-throw.
	const basar::Int16& getInt16()  const
	{
		assert(m_isPrimitive);
		return content.int16_val;
	}

	//! \brief	returns the value of the inner union member of datatype Int32. \n no-throw.
	const basar::Int32& getInt32()  const
	{
		assert(m_isPrimitive);
		return content.int32_val;
	}

    const basar::Int64& getInt64() const
    {
        assert(m_isPrimitive);
        return content.int64_val;
    }

	//! \brief	returns the value of the inner union member of datatype Float32. \n no-throw.
	const basar::Float32& getFloat32()  const
	{
		assert(m_isPrimitive);
		return content.float32_val;
	}

	//! \brief	returns the value of the inner union member of datatype Float64. \n no-throw.
	const basar::Float64& getFloat64()  const
	{
		assert(m_isPrimitive);
		return content.float64_val;
	}
    
private:
	//! deleting dynamically allocated member \n no-throw
	void clean()
	{
		if ( !m_isPrimitive )
        {
			delete content.complex_val;
        }
        content.float64_val = 0.0;
	}


public:
	//-----------------------------------------------//
	// types (public so any_cast can be non-friend)
	//-----------------------------------------------//

    //! \brief	base class of the object holding the value as a pointer in the inner union member
	class placeholder
    {
    public:
		//! \brief	d'tor
		virtual ~placeholder(){}

		//! \brief	clone()-method (design prototype)
        virtual placeholder * clone() const = 0;
    };

	//! \brief	successor class of the class holding the value as a pointer member. \n no-throw
    template<typename ValueType>
    class holder : public placeholder
    {
    public:
        //! \brief	overloaded c'tor. The passed object is stored as a value member.
		holder( const ValueType& value ) : held( value ) {}

        //! \brief overloaded c'tor for constructing a string from recordset
        explicit holder( char const* value ) : held( value ) {}

		//! \brief	concrete overwritten clone()-method
        virtual placeholder* clone() const
        {
            return new holder( held );
        }

		ValueType held;		//!< value to hold
    };

	/*! \brief	structure of the union holding integral values and a pointer member.
				a pointer as an inner member of an union is only possible, 
				if the class has an compiler generated (default) copy c'tor. */
	union Content 
	{
		basar::Int16			int16_val;		//!< small integer value
		basar::Int32			int32_val;		//!< integer value
        basar::Int64            int64_val;      //!< 64bit int value
		basar::Float32			float32_val;	//!< float value
		basar::Float64			float64_val;	//!< double value

		placeholder*			complex_val;	//!< placeholder for complex datatype

        Content() : float64_val(0.0) {};
	};

	//-------------------------------------------------------//
	// representation (public so any_cast can be non-friend)
	//-------------------------------------------------------//

	bool	m_isPrimitive;		//!< bool member indicates if a value member of an integral datatype is stored in the inner union member.
	Content content;			//!< inner union member
};

//-------------------------------------------------------------------------------------------------//
// cast operation
//-------------------------------------------------------------------------------------------------//
//! \brief	global cast method to get the pointer value as out of an any object.
template <typename ValueType>
ValueType * any_cast( any * operand )
{
    return &static_cast<any::holder<ValueType> *>( operand->content.complex_val )->held;
}

template <>
inline any & any::operator = (char const* rhs)
{
	clean();
	m_isPrimitive       = false;
	content.complex_val = new holder<basar::cmnutil::I18nString> (rhs);

	return *this;
}

//-------------------------------------------------------------------------------------------------//
}	// end namespace property
}	// end namespace basar


//-------------------------------------------------------------------------------------------------//
// Copyright Kevlin Henney, 2000, 2001, 2002. All rights reserved.
//
// Permission to use, copy, modify, and distribute this software for any
// purpose is hereby granted without fee, provided that this copyright and
// permissions notice appear in all copies and derivatives.
//
// This software is provided "as is" without express or implied warranty.

//-------------------------------------------------------------------------------------------------//
#endif // GUARD_PROPERTY_ANY_H
