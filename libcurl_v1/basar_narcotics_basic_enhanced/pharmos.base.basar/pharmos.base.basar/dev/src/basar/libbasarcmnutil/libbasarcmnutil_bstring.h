//----------------------------------------------------------------------------
/*! \file
 *  \brief  basic string class definitions
 *  \author Thomas Hörath
 *  \date   21.07.2005
 */
//----------------------------------------------------------------------------

#ifndef GUARD_LIBBASARCMNUTIL_BSTRING_H
#define GUARD_LIBBASARCMNUTIL_BSTRING_H

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
#include <stdarg.h>
#include <string>
#include <vector>
#include "libbasar_definitions.h"

//----------------------------------------------------------------------------
namespace basar {
namespace cmnutil	{

/*! \brief  typedef to std::string */
typedef std::string LIBBASARCMNUTIL_API CharString;

/*! \brief  Vector of strings */
typedef std::vector<BString>    CollBString;

//----------------------------------------------------------------------------
/*! \brief Basic string functionality.

	Basic class for I18nString.
	BString is an extension to std::basic_string<char> and should be used for 
	technical purpose. Synonym is VarString. I18nString is useful when locale behaviour is needed.
	\n functionality from former CBString that was not adopted:
	\n	-constructors for numeric datatypes
	\n	-typecast and assignment operators
	\n	-cast methods from and to numeric datatypes
	\n	-tidy() */
class BString : public CharString
{
public:
	/*! \brief Standard-Constructor of the BString-class	\n no-throw */
	LIBBASARCMNUTIL_API				BString			();
	/*! \brief overwritten constructor of the BString-class	\n no-throw */
	LIBBASARCMNUTIL_API explicit	BString			( const value_type   ch		//!< character to be copied in string being contructed
													);
	/*! \brief overwritten constructor of the BString-class \n no-throw */
	LIBBASARCMNUTIL_API				BString			( const value_type * pStr	//!< null terminated string to be copied in string being contructed
													);
	/*! \brief overwritten constructor of the BString-class \n no-throw */
	LIBBASARCMNUTIL_API				BString			( const CharString & rStr	//!< CharString to be copied in string being contructed
													);
	/*! \brief overwritten constructor of the BString-class \n no-throw */
	LIBBASARCMNUTIL_API				BString			( const BString    & rStr	//!< BString to be copied in string being contructed
													);
	/*! \brief overwritten constructor of the BString-class \n no-throw */
	LIBBASARCMNUTIL_API				BString			( const BString    & rStr,	//!< BString to be copied in string being contructed
													  const size_type	 off,	//!< index in BString that is the first to be used to initialize string being constructed
													  const size_type	 count	//!< number of characters to be initialized
													);
	/*! \brief overwritten constructor of BString-class		\n no-throw */
	LIBBASARCMNUTIL_API				BString			( const_iterator     first,	//!< input iterator addressing the first character in range of a string
													  const_iterator     last	//!< input iterator addressing the position one past the last character in the range of a string
													);
	/*! \brief virtual(!) d'tor								\n no-throw */
	LIBBASARCMNUTIL_API virtual	   ~BString			();

	/*! \brief returning string from the right side of given char		\n no-throw */
	LIBBASARCMNUTIL_API const BString	right	( const value_type ch			//!< character from which the right side is taken to create the new string
												) const;
	/*! \brief returning string from the right side of given BString	\n no-throw */
	LIBBASARCMNUTIL_API const BString	right	( const BString & rStr			//!< substring from which the right side is taken to create the new string  
												) const;
	/*! \brief returning string from the left side of given char 		\n no-throw */
	LIBBASARCMNUTIL_API const BString	left	( const value_type ch			//!< character from which the left side is taken to create the new string
												) const;
	/*! \brief returning string from the left side of given BString 	\n no-throw */
	LIBBASARCMNUTIL_API const BString	left	( const BString & rStr			//!< substring from which the left side is taken to create the new string  
												) const; 
	/*! \brief returning string from position Off with given length; overloaded from basic_string	\n no-throw */
	LIBBASARCMNUTIL_API	const BString	substr	( const size_type off,			//!< index locating element at position from which string copy is made
												  const size_type count = npos	//!< number of characters that are to be copied if they are present
												) const;

     
	/*! \brief checking if BString is completely numeric ( 123a would be false! ) */
	LIBBASARCMNUTIL_API bool				    isDigit() const;
	/*! \brief convert string to long */
	LIBBASARCMNUTIL_API BLong					stol   () const; 
	/*! \brief convert string to int */
	LIBBASARCMNUTIL_API basar::Int32			stoi   () const;
    /*! \brief convert string to Int64 */
    LIBBASARCMNUTIL_API basar::Int64            stoll  () const;
	/*! \brief convert string to double */
	LIBBASARCMNUTIL_API virtual basar::Float64	stof   () const; 
	/*! \brief convert int to string */
	LIBBASARCMNUTIL_API void					itos   (Int32   number  //!< number to convert to string
													   ); 
	/*! \brief convert long to string */
	LIBBASARCMNUTIL_API void					ltos   (Long32	number	//!< number to convert to string
													   ); 
    /*! \brief convert Int64 to string */
    LIBBASARCMNUTIL_API void                    lltos  (Int64 number //!< number to convert to string
                                                       );
	/*! \brief convert double to string */
	LIBBASARCMNUTIL_API virtual void   			ftos   (Float64 number,	//!< number to convert to string
                                                        Int32 precision = 6 //!< precision for conversion to string, default value same as set by std::basic_ios::init
													   ); 

	/*! \brief making all upper characters lower */ 
	LIBBASARCMNUTIL_API virtual void			lower  ();

    //! \brief returning all lower copy
    //! \return BString containing all lower characters
    LIBBASARCMNUTIL_API const BString           toLower() const;

	/*! \brief making all lower characters upper */
	LIBBASARCMNUTIL_API virtual void			upper  (); 

    //! \brief returning all upper copy
    //! \return BString containing all upper characters
    LIBBASARCMNUTIL_API const BString           toUpper() const;

	
 
	/*! \brief  position where BString should be trimmed */
	enum TrimPosEnum
    { 
        ALL			= 0x0010L, //!< erase all spaces
        FIRST		= 0x0020L, //!< erase spaces before first sign
        END			= 0x0040L, //!< erase spaces after last sign
		FIRST_END	= 0x0060L  //!< erase spaces before first and after last sign
    };  

	/*! \brief erasing space from BString (all,first,end --> TrimPosEnum)			\n no-throw */
	LIBBASARCMNUTIL_API void			trim				( TrimPosEnum	  eWhere = END	//!< position where to trim
															); 
	/*! \brief changing not printable char to space									\n no-throw */
	LIBBASARCMNUTIL_API virtual void	cleanchar			();
	/*! \brief reversing the order of the characters								\n no-throw */
	LIBBASARCMNUTIL_API void			reverse				( const size_type pos = 0		//!< position where reversing starts
															); 
	/*! \brief exchanging ALL given Char to Ptr										\n no-throw */
	LIBBASARCMNUTIL_API void			exchange			( const value_type ch,	//!< character to be exchanged 
															  ConstString      to	//!< replacing string
															); 
    
	/*! \brief exchanging ALL given From to To										\n no-throw */
	LIBBASARCMNUTIL_API void			exchange			( const BString & from,	//!< string to be exchanged 
															  const BString & to	//!< replacing string
															); 
	/*! \brief erasing all substrings from BString									\n no-throw */
	LIBBASARCMNUTIL_API void			remove				( const BString & str	//!< substring to be removed
															); 
    
	/*! \brief counting all Char in BString											\n no-throw */
	LIBBASARCMNUTIL_API size_type	    getCount			( const value_type ch	//!< counted character
															) const;

	/*! \brief formatting string by calling formatV()								\n no-throw */
	LIBBASARCMNUTIL_API void			format				( const value_type * pFormat,	//!< format string ( see printf() )
		                                                      ...							//!< optional arguments
															);
    /*! \brief substring to a delimiter, f.ex. string: "a b c".substrToDelimiter(" ", true) = "a" */
   LIBBASARCMNUTIL_API BString	substrToDelimiter			(
		  const BString & delimiter,					//!< delimiter string
		  bool            acceptNoDelimiter = true		//!< is it allowed that the string is not terminated by delimiter
		) const;

    /*! \brief substring to a delimiter and trim, f.ex. string: "a ; b ;c".substrToDelimiter(" ", ALL, true) = "a" */
   LIBBASARCMNUTIL_API BString	substrToDelimiterAndTrim	(
		  const BString & delimiter,					//!< delimiter string
		  TrimPosEnum     trim,							//!< trim parameter
		  bool            acceptNoDelimiter = true		//!< is it allowed that the string is not terminated by delimiter
		) const;

    /*! \brief substring to a delimiter, f.ex. string: "a b c".substrToDelimiter(5," ", true) = "c" */
   LIBBASARCMNUTIL_API BString	substrToDelimiter			(
		  const size_type off,							//!< position where substring starts
		  const BString & delimiter,					//!< delimiter string
		  bool            acceptNoDelimiter = true		//!< is it allowed that the string is not terminated by delimiter
		) const;

    /*! \brief substring to a delimiter and trim, f.ex. string: "a ;b ;c".substrToDelimiter(4,";", ALL, false) = "b" */
   LIBBASARCMNUTIL_API BString	substrToDelimiterAndTrim	(
		  const size_type off,							//!< position where substring starts
		  const BString & delimiter,					//!< delimiter string
		  TrimPosEnum     trim,							//!< trim parameter
		  bool            acceptNoDelimiter = true		//!< is it allowed that the string is not terminated by delimiter
		) const;

   /*!  \brief tokenize the string with delimiter */
   LIBBASARCMNUTIL_API CollBString	tokenize				(
		  const BString & delimiter,					//!< delimiter string
		  bool            acceptNoDelimiter  = true,	//!< is it allowed that the string is not terminated by delimiter
		  bool            removeEmptyStrings = false	//!< removes empty strings between two delimiter
		) const;


   /*!  \brief tokenize the string with delimiter */
   LIBBASARCMNUTIL_API CollBString	tokenizeAndTrim			(
		  const BString & delimiter,					//!< delimiter string
		  TrimPosEnum     trim,							//!< trim parameter
		  bool            acceptNoDelimiter  = true,	//!< is it allowed that the string is not terminated by delimiter
		  bool            removeEmptyStrings = false	//!< removes empty strings between two delimiter
		) const;

   /*!  \brief tokenize the string with delimiter */
   LIBBASARCMNUTIL_API void		tokenize					(
		  CollBString   & returnValue,					//!< return value, will be enhanced
		  const BString & delimiter,					//!< delimiter string
		  bool            acceptNoDelimiter  = true,	//!< is it allowed that the string is not terminated by delimiter
		  bool            removeEmptyStrings = false	//!< removes empty strings between two delimiter
       ) const;
  
   /*!  \brief tokenize the string with delimiter */
   LIBBASARCMNUTIL_API void		tokenizeAndTrim				(
		  CollBString   & returnValue,					//!< return value, will be enhanced
		  const BString & delimiter,					//!< delimiter string
		  TrimPosEnum     trim,							//!< trim parameter
		  bool            acceptNoDelimiter  = true,	//!< is it allowed that the string is not terminated by delimiter
		  bool            removeEmptyStrings = false	//!< removes empty strings between two delimiter
		) const;


protected:
    /*! \brief formatting string
        \n throw OutOfRangeIndexException */
	virtual void    formatV            (const value_type * pFormat,	//!< format string ( see printf() ) 
									    va_list            argList	//!< Pointer to list of arguments
									   );
	/*! \brief erasing ALL spaces from string */
	void            eraseAllSpace	   ();
	/*! \brief erasing leading spaces */
	void	        eraseFirstSpace	   ();
	/*! \brief erasing trailing spaces */
	void	        eraseEndSpace	   ();
	/*! \brief erasing leading and trailing spaces */
	void	        eraseFirstEndSpace ();
    /*! \brief erasing trailing zeros ('0')  */
    static void     eraseTrailingZeros (VarString & str //!< return value, will be removed of trailing zeroes
                                       );
};

//----------------------------------------------------------------------------
}	// namespace cmnutil
}	// namespace basar

//----------------------------------------------------------------------------
#endif	// GUARD
