//----------------------------------------------------------------------------
/*! \file
 *  \brief  cmdline-tokenizer class definitions
 *  \author Thomas Hörath
 *  \date   15.07.2005
 */
//----------------------------------------------------------------------------

#ifndef GUARD_LIBBASARCMNUTIL_CMDLINE_H
#define GUARD_LIBBASARCMNUTIL_CMDLINE_H

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
#include "libbasarcmnutil_exceptions.h"

//----------------------------------------------------------------------------
namespace basar {
namespace cmnutil	{

//----------------------------------------------------------------------------
/*! \brief exception class for command line

	This is a final class. */
class CmdLineException : public BasarException
{
public:
	//! constructs exception object and initialises text description string
			LIBBASARCMNUTIL_API CmdLineException		(const ExceptInfo& sInfo);
private:
			CmdLineException		();							//!< forbidden
};

//----------------------------------------------------------------------------
/*! \brief Handling command line functionality.

	This is a final class.
	At first the method setCmdLine() has to be called.
	After this, working with the commandline is possible.
	Class throws a CmdLineException / InvalidParameterException, if an error occurs. */
class CmdLineTokenizer
{
public:
	/*! \brief Standard-Constructor of the CmdLineTokenizer-class 
		\n     no-throw */
	LIBBASARCMNUTIL_API			CmdLineTokenizer		();

	/*! \brief Standard-Destructor of the CmdLineTokenizer-class 
		\n     no-throw */
	LIBBASARCMNUTIL_API			~CmdLineTokenizer		();

	/*! \brief sets the members  m_ArgC, m_ArgV, m_ProgName
		\n     CmdLineException */
	LIBBASARCMNUTIL_API void	setCmdLine		( const Int32 argC, char ** argV );

	/*! \brief checks if given rParam is in CmdLine 
		\n     no-throw */
	LIBBASARCMNUTIL_API bool	isParam    		(const VarString & rParam) const;

	/*! \brief writes Value of rParam in 2nd Parameter rValue, to retrieve the parameters value 
		\n     InvalidParameterException */
	LIBBASARCMNUTIL_API void  	getValOfParam	(const VarString & rParam,
														VarString & rValue) const;


	/*! \brief returns complete CmdLine 
		\n     no-throw */
	LIBBASARCMNUTIL_API	const VarString			getCmdLine		() const;
	
	/*! \brief returns programm name 
		\n     no-throw */
	LIBBASARCMNUTIL_API	  const VarString	&	getProgName		() const;
	

protected:
	/*! \brief copy constructor 
		\n     no-throw */
	CmdLineTokenizer				(const CmdLineTokenizer & r);
	
	/*! \brief forbidden operator = 
		\n     no-throw */
	CmdLineTokenizer &	operator =	(const CmdLineTokenizer & r);


	/*! \brief returns position of given parameter
		\n     no-throw */
	LIBBASARCMNUTIL_API VarString::size_type    getPosOfParam	(const VarString & rParam) const;

	Int32						m_ArgC; //!< number of arguments in CmdLine
	char					**	m_ArgV;	//!< values of arguments in CmdLine

	VarString					m_ProgName; //!< programmname = 1st argument in CmdLine
};

//----------------------------------------------------------------------------
}	// namespace cmnutil
}	// namespace basar

//----------------------------------------------------------------------------
#endif	// GUARD
