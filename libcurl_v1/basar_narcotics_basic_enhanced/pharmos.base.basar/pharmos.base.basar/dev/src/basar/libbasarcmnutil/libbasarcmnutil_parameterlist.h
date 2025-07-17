//----------------------------------------------------------------------------
/*! \file
 *  \brief  parameter list class definitions
 *  \author Thomas Hörath
 *  \date   12.08.2005
 */
//----------------------------------------------------------------------------

#ifndef GUARD_LIBBASARCMNUTIL_PARAMETERLIST_H
#define GUARD_LIBBASARCMNUTIL_PARAMETERLIST_H

#ifndef LIBBASARCMNUTIL_API
	#ifdef _WIN32
		#ifdef LIBBASARCMNUTIL_EXPORTS
			#define LIBBASARCMNUTIL_API __declspec(dllexport)	//!< dll exported
		#else
			#define LIBBASARCMNUTIL_API __declspec(dllimport)	//!< dll imported
		#endif 
	#else 
		#define LIBBASARCMNUTIL_API
	#endif
#endif


//----------------------------------------------------------------------------
#include <map>
#include <iostream>
#include <boost/shared_ptr.hpp>
#include "libbasar_definitions.h"

//----------------------------------------------------------------------------
namespace basar {
namespace cmnutil	{

/*! \brief  key-value pair of ParameterList */
typedef I18nString											  ParamPair;    

/*! \brief  Key of ParameterList */
typedef I18nString                                            ParamName;    

/*! \brief  Value of ParameterList */
typedef I18nString                                            ParamValue;    

/*! \brief  Pair (key + value) of ParameterList */
typedef std::pair<const ParamName, ParamValue>				  ParamListPair;

/*! \brief  Map to store all parameter-pairs */
typedef std::map<ParamName, ParamValue>						  ColParamListMap;

/*! \brief  const-iterator on ColParamListMap*/
typedef ColParamListMap::const_iterator						  ColParamListMapConstIt;

/*! \brief  iterator on ColParamListMap*/
typedef ColParamListMap::iterator							  ColParamListMapIt;

/*! \brief  size of ColParamListMap*/
typedef ColParamListMap::size_type							  ParamListSize;

/*! \brief  shared pointer to ColParamListMap*/
typedef boost::shared_ptr<ColParamListMap>					  ColParamListMapSharedPtr;


//----------------------------------------------------------------------------
/*! \brief Iterator-class to iterate through ColParamListMap.

	This is a final class.
	Class throws OutOfRangeIteratorException.*/
class ParameterList_Iterator
{
	public:	
		/*! \brief Constructor of the Iterator-class 
			\n     no-throw */
		LIBBASARCMNUTIL_API ParameterList_Iterator	(ColParamListMapSharedPtr pCol, 
													 ColParamListMapIt it);
		
		/*! \brief standard-constructor 
			\n     no-throw */
		LIBBASARCMNUTIL_API ParameterList_Iterator						();

		/*! \brief checks if iterator points to begin() of the inner container
			\n     no-throw */
		LIBBASARCMNUTIL_API bool					isBegin			() const;

		/*! \brief checks if iterator points to end() of the inner container
			\n     no-throw */
		LIBBASARCMNUTIL_API bool					isEnd			() const;

		/*! \brief checks if there is any value set to given key
		\n     UnknownPropertyNameException */
		LIBBASARCMNUTIL_API bool					isValueEmpty	() const;

		/*! \brief checks if two iterators are equal 
			\n     no-throw */
		LIBBASARCMNUTIL_API bool					operator ==		(const ParameterList_Iterator& rhp) const;
		
		/*! \brief checks if two iterators are not equal 
			\n     no-throw */
		LIBBASARCMNUTIL_API bool					operator !=		(const ParameterList_Iterator& rhp) const;

		/*! \brief increases operator 
			\n     OutOfRangeIteratorException */
		LIBBASARCMNUTIL_API ParameterList_Iterator	operator ++		();
		
		/*! \brief increases operator
			\n     OutOfRangeIteratorException */
		LIBBASARCMNUTIL_API ParameterList_Iterator	operator ++		(int);

		/*! \brief returns reference to current iterator
			\n     OutOfRangeIteratorException */
		LIBBASARCMNUTIL_API ParamListPair &				operator*		();
		
		/*! \brief returns reference to current iterator
			\n     OutOfRangeIteratorException */
		LIBBASARCMNUTIL_API const ParamListPair &	operator*		() const;

		/*! \brief returns pointer to current iterator
			\n     OutOfRangeIteratorException */
		LIBBASARCMNUTIL_API ParamListPair *			operator->		();
		
		/*! \brief returns pointer to current iterator
			\n     OutOfRangeIteratorException */
		LIBBASARCMNUTIL_API const ParamListPair *	operator->		() const;

        //! \brief is  NULL (Null Pointer)?	\n no-throw
        LIBBASARCMNUTIL_API bool isNull() const;

		//! \brief resets the iterator	\n no-throw
        LIBBASARCMNUTIL_API void reset()  ;

	private:
		/*! \brief checks if the iterator is valid
 			\n     no-throw */
		void checkValid() const;

		ColParamListMapSharedPtr	m_ColParamListMap;	//!< shared pointer to map
		ColParamListMapIt			m_ColParamListMapIt;//!< iterator on ColParamListMap
};

//----------------------------------------------------------------------------
//! \brief dump ParameterList variable's content to debug output
//!
//! \note  use in Visual Studio's "Command Window - Immediate" following input (OBJECT_ADDRESS e.g.: 0x0012fa94) :
//! \code  {,,libbasarcmnutil.dll} basar::cmnutil::dumpParamList ( OBJECT_ADDRESS )
//! \endcode
LIBBASARCMNUTIL_API void dumpParamList	(size_t addr	//!< object address
										);

//----------------------------------------------------------------------------
#define SIGN_DELIM	';' //!< sign to declare the end of a key / value in rPairStr at construction 
#define SIGN_ASSIGN	'=' //!< sign to declare the assignment of a value in rPairStr to former key at construction 

//----------------------------------------------------------------------------
/*! \brief Parameter List functionality.

	This is a final class.
	Class throws UnknownPropertyNameException / OutOfRangeIteratorException / BasarNotTerminatedException	
	List of pairs (key,value) as simple value-based type. Key and value are both strings.
	Implemented as std::map.*/
class ParameterList 
{	
	friend class ParameterList_Iterator;	
   
public:	
	/*! \brief Constructor of the ParameterList-class 
		\n     no-throw */
	LIBBASARCMNUTIL_API ParameterList						();
	
	/*! \brief Copy-constructor of the ParameterList-class 
		\n     no-throw */
	LIBBASARCMNUTIL_API ParameterList						(const ParameterList& r);
	
	/*! \brief Constructor, creating list from string with format "key1=value1; key2; key3 = value3;"...
		\n     BasarNotTerminatedException */
	LIBBASARCMNUTIL_API ParameterList						(const ParamPair & rPairStr);
	
	/*! \brief assignment operator 
		\n     no-throw */
	LIBBASARCMNUTIL_API ParameterList&		operator =		(const ParameterList& r);
	
	/*! \brief insert new parameter in list, value = empty
		\n     no-throw */
	LIBBASARCMNUTIL_API void				push_back		(const ParamName& rKey);
	
	/*! \brief insert new parameter with associated value to list
		\n     no-throw */
	LIBBASARCMNUTIL_API void				push_back		(const ParamName& rKey, const ParamValue& rVal);
	
	/*! \brief insert new parameter-pair (key, value) to list
		\n     no-throw */
	LIBBASARCMNUTIL_API void				push_back		(const ParamListPair & rParamPair);
	
	/*! \brief get size of parameter-list
		\n     no-throw */
	LIBBASARCMNUTIL_API ParamListSize       size			() const;
	
	/*! \brief get value of given key stored in list
		\n     UnknownPropertyNameException */
	LIBBASARCMNUTIL_API const ParamValue	getValue		( const ParamName& rKey ) const;
	
	/*! \brief check if there is any value set to given key
		\n     UnknownPropertyNameException */
	LIBBASARCMNUTIL_API bool			    isValueEmpty	(const ParamName& rKey) const;

	/*! \brief check if list contains given key name
		\n     no-throw */
	LIBBASARCMNUTIL_API bool			    isKeyContained	(const ParamName& rKey) const;
			
    /*! \brief	returns true when the key is contained and a value is set.
                does not throw UnknownPropertyNameException
				\n     no-throw */
	LIBBASARCMNUTIL_API bool		        hasValue		(const ParamName& rKey ) const;

	/*! \brief set value of given key stored in list
		\n     UnknownPropertyNameException */
	LIBBASARCMNUTIL_API void				setValue		(const ParamName& rKey, const ParamValue& rVal);

	/*! \brief return iterator to first element in list
		\n     no-throw */
	LIBBASARCMNUTIL_API ParameterList_Iterator begin		() const;
	
	/*! \brief return iterator to last element in list
		\n     no-throw */
	LIBBASARCMNUTIL_API ParameterList_Iterator end			() const;

	/*! \brief remove keys + values from list, so that list is empty again.
		\n     no-throw */
	LIBBASARCMNUTIL_API void				clear			();
	
	/*! \brief	iterate all parameter names and corresponding parameter values, 
				print via std::cout by default. Could be overwritten by the std::ostream parameter.
				\n     no-throw */
	LIBBASARCMNUTIL_API void				toStream		( std::ostream& strm = std::cout ) const;


    /*! \brief  returns a String that can be used to initialize a new ParameterList
                \n     no-throw */
	LIBBASARCMNUTIL_API VarString			toString	    () const;

	/*! \brief  compares parameterlists			\n no-throw */
    LIBBASARCMNUTIL_API bool operator ==(const ParameterList &) const;

private:
	ColParamListMapSharedPtr m_ColParamListMap;			//!< shared pointer to parameterlist

	/*! \brief called from copy-constructor and operator= to insert iterator-value to list
		\n     no-throw */
	void assign										(const ParameterList& r);
};


//----------------------------------------------------------------------------
}	// namespace cmnutil
}	// namespace basar

//----------------------------------------------------------------------------
#endif	// GUARD
