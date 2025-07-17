//----------------------------------------------------------------------------
/*! \file
 *  \brief  international string class definitions
 *  \author Thomas Hörath
 *  \date   04.10.2005
 */
//----------------------------------------------------------------------------

#ifndef GUARD_LIBBASARCMNUTIL_I18NSTRING_H
#define GUARD_LIBBASARCMNUTIL_I18NSTRING_H

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
#include "libbasarcmnutil_bstring.h"

//----------------------------------------------------------------------------
namespace basar     {
namespace cmnutil	{

//----------------------------------------------------------------------------
/*! \brief International string functionality.

	This is a final class.
	I18nString is an extension to BString (= VarString) that represents locale behaviour
	(e.g. upper/lower, convert to decimal).
	\throw no-throw */
class I18nString : public BString
{
public:
	/*! \brief c'tor */
	LIBBASARCMNUTIL_API				I18nString	();
	/*! \brief c'tor	\n no-throw */
	LIBBASARCMNUTIL_API explicit	I18nString	( const value_type   ch		//!< character to be copied in string being contructed
												);
	/*! \brief c'tor */
	LIBBASARCMNUTIL_API				I18nString	( const value_type * pStr	//!< null terminated string to be copied in string being contructed
												);
	/*! \brief c'tor */
	LIBBASARCMNUTIL_API				I18nString	( const CharString & rStr	//!< CharString to be copied in string being contructed
												);
	/*! \brief c'tor */
	LIBBASARCMNUTIL_API				I18nString	( const BString    & rStr	//!< BString to be copied in string being contructed
												);
	/*! \brief c'tor */
	LIBBASARCMNUTIL_API				I18nString	( const I18nString & rStr	//!< I18nString to be copied in string being contructed   
												);
	/*! \brief c'tor */
	LIBBASARCMNUTIL_API				I18nString	( const BString    & rStr,	//!< BString to be copied in string being contructed
												  const size_type	 off,	//!< index in BString that is the first to be used to initialize string being constructed
												  const size_type	 count	//!< number of characters to be initialized
												);
	/*! \brief c'tor */
	LIBBASARCMNUTIL_API				I18nString	( const I18nString & rStr,	//!< I18nString to be copied in string being contructed
												  const size_type	 off,	//!< index in I18nString that is the first to be used to initialize string being constructed
												  const size_type	 count	//!< number of characters to be initialized
												);
	/*! \brief c'tor */
	LIBBASARCMNUTIL_API				I18nString	( const_iterator     first,	//!< input iterator addressing the first character in range of a string
												  const_iterator     last	//!< input iterator addressing the position one past the last character in the range of a string
												);
	/*! \brief virtual(!) d'tor */
	LIBBASARCMNUTIL_API virtual	   ~I18nString	();


	/*! \brief convert string to double	regarding locale */
	LIBBASARCMNUTIL_API virtual basar::Float64	stof	() const; 
	/*! \brief convert double to string */
	LIBBASARCMNUTIL_API virtual void   			ftos	(Float64     number,	//!< number to convert to string
                                                         Int32 precision = 6    //!< precision for conversion to string, default value same as set by std::basic_ios::init
														); 
	/*! \brief making all upper characters lower regarding locale */ 
	LIBBASARCMNUTIL_API virtual void			lower	();

    //! \brief returning all lower copy regarding locale
    //! \return I18nString containing all lower characters
    LIBBASARCMNUTIL_API const I18nString        toLower() const;

    /*! \brief making all lower characters upper regarding locale */
	LIBBASARCMNUTIL_API virtual void			upper	(); 

    //! \brief returning all upper copy regarding locale
    //! \return I18nString containing all lower characters
    LIBBASARCMNUTIL_API const I18nString        toUpper() const;

    /*! \brief changing not printable char to space regarding locale */
	LIBBASARCMNUTIL_API virtual void			cleanchar	();

protected:
	/*! \brief formatting string regarding locale   \n throw OutOfRangeIndexException */
	virtual void    formatV    (const value_type * pFormat,		//!< format string ( see printf() ) 
								va_list            argList		//!< Pointer to list of arguments
							   );
private:
    //! \brief removes 1000s Sep from string, needed for compliance with unit tests 
    static void erase1000sSep  (VarString & str     //!< return value, will be removed of 1000s Sep 
                               );
    //! \brief determine whether character is printable in current locale
    static bool isprintable    (value_type character  //!< character to check
                               );
    //! \brief append flags in format string
    static void appFmtFlags    (const value_type * & src,    //!< evaluated format string
                                BString            & dest    //!< appended format string with flags
                               );
    //! \brief append and expand length in format string
    static void appFmtLength   (const value_type * & src,    //!< evaluated format string
                                BString            & dest,   //!< expanded format string with length
                                va_list            & argList //!< argument list
                               );
    //! \brief append and expand precision in format string
    static void appFmtPrec     (const value_type * & src,    //!< evaluated format string
                                BString            & dest,   //!< expanded format string with precision
                                va_list            & argList //!< argument list
                               );
    //! \brief append length subspecifier in format string
    static void appFmtSubspec  (const value_type * & src,    //!< evaluated format string
                                BString            & dest    //!< appended format string with length subspecifier
                               );
    //! \brief append type specifier in format string
    static void appFmtTypespec (const value_type   * src,    //!< evaluated format string
                                BString            & dest    //!< appended format string with type specifier
                               );

    //! \brief append current formatted value to string
    void        appFmtValue    (BString            & src,    //!< format string for current argument
                                va_list            & argList //!< argument list
                               );

    //! converts characters to upper/lower case 
    void        convert        (const bool up   //!< upper/lower consversion
                               );
};

//----------------------------------------------------------------------------
}	// namespace cmnutil
}	// namespace basar

//----------------------------------------------------------------------------
#endif	// GUARD

