//----------------------------------------------------------------------------
/*! \file
 *  \brief  class implementation of command line tokenizer
 *  \author Thomas Hörath
 *  \date   18.07.2005
 */
//----------------------------------------------------------------------------

#include "libbasarcmnutil_cmdline.h"

//----------------------------------------------------------------------------
namespace basar {
namespace cmnutil{

//----------------------------------------------------------------------------
/*! \throw  no-throw 
	\param sInfo struct with output-info */
CmdLineException::CmdLineException ( const ExceptInfo& sInfo )
                                   : basar::Exception(sInfo, "CmdLineException")
{
}

//----------------------------------------------------------------------------
/*! \ingroup DIFF_WIN_UNIX */ 
#ifdef _WIN32
	#define CHAR_SEPARATOR '\\' //!< separator in file-structure on windows
#else 
	#define CHAR_SEPARATOR '/' //!< separator in file-structure on unix
#endif

//----------------------------------------------------------------------------
/*!	\throw no-throw */
CmdLineTokenizer::CmdLineTokenizer() : m_ArgC  ( 0 ), m_ArgV( NULL )
{
}

//----------------------------------------------------------------------------
/*!	\throw no-throw  */
CmdLineTokenizer::~CmdLineTokenizer()
{
}

//----------------------------------------------------------------------------
/*!	\throw CmdLineException 
	\param argC number of arguments
	\param argV values of arguments

	this method has to be executed before any other function from this class can be used! */
void CmdLineTokenizer::setCmdLine( const Int32 argC, char ** argV )
{
	if( 0 == argC )
	{
		VarString errMsg;
		errMsg.format("no command line passed: argC = <%d>, argV = <0x%x>", argC, argV);
		ExceptInfo sInfo("basar.cmnutil.CmdLineTokenizer.setCmdLine", errMsg, __FILE__, __LINE__);

		throw CmdLineException(sInfo);
	}
	
	m_ArgC   = argC;
	m_ArgV   = argV;


	// determine program name
	VarString name			 = argV[0];
	VarString::size_type Pos = name.find_last_of(CHAR_SEPARATOR);

	if (VarString::npos == Pos)
		m_ProgName = name;
	else
	{
		if (Pos >= name.length() - 1)	// last char in string
		{
			VarString errMsg;
			errMsg.format("program name not correct: argC = <%d>, argV [0] = <%s>", argC, argV[0]);
			ExceptInfo sInfo("basar.cmnutil.CmdLineTokenizer.setCmdLine", errMsg, __FILE__, __LINE__);
		
			throw CmdLineException(sInfo);
		}

		m_ProgName = name.substr(Pos + 1, name.length() - (Pos + 1));
	}
}

//----------------------------------------------------------------------------
/*!	\throw no-throw 
	\return programname */
const VarString & CmdLineTokenizer::getProgName() const
{
	return m_ProgName;
}

//----------------------------------------------------------------------------
/*!	\throw no-throw 
	\param rParam name of parameter to search for
	\retval false if rParam is not found in cmdline 
	\retval true if rParam is found in cmdline */
bool CmdLineTokenizer::isParam(const VarString & rParam) const 
{
	return ( (VarString::npos == getPosOfParam(rParam)) ? false : true );
}

//----------------------------------------------------------------------------
/*!	\throw InvalidParameterException 
	\param rParam name of argument to search for
	\param rValue value of argument which was searched */
void CmdLineTokenizer::getValOfParam(const VarString & rParam, VarString & rValue) const
{
	VarString::size_type pos = getPosOfParam(rParam);

	// not found
	if (pos == VarString::npos)
	{
		VarString errMsg;
		errMsg.format("parameter not found: <%s>", rParam.c_str());
		ExceptInfo sInfo("basar.cmnutil.CmdLineTokenizer.getValOfParam", errMsg, __FILE__, __LINE__);
		
		throw InvalidParameterException(sInfo);
	}

	// last position; not allowed
	if (static_cast<Int16>(pos) >= (m_ArgC - 1))
	{
		VarString errMsg;
		errMsg.format("m_ArgC %ld, parameter <%s> found at position <%ld>", 
					  m_ArgC, 
					  rParam.c_str(),
					  pos);
		ExceptInfo sInfo("basar.cmnutil.CmdLineTokenizer.getValOfParam", errMsg, __FILE__, __LINE__);

		throw InvalidParameterException(sInfo);
	}

	rValue = m_ArgV[pos + 1];
}

//----------------------------------------------------------------------------
/*!	\throw no-throw 
	\param rParam name of param to search for
	\return position of parameter in cmdline */
VarString::size_type CmdLineTokenizer::getPosOfParam( const VarString & rParam ) const
{
	for (Int16 i = 1; i < m_ArgC; ++i )
	{
		if (0 == rParam.compare(m_ArgV[i]))
			return i;
	}

	return VarString::npos;
}

//----------------------------------------------------------------------------
/*!	\throw no-throw 
	\return complete cmdline as string */
const VarString CmdLineTokenizer::getCmdLine() const
{
	VarString cmdLine = m_ArgV[0];

	for (Int16 i = 1; i < m_ArgC; ++i)
	{
		cmdLine.append(" ");
		cmdLine.append(m_ArgV[i]);
	}

	return cmdLine;
}

//----------------------------------------------------------------------------
}//cmnutil
}//basar
