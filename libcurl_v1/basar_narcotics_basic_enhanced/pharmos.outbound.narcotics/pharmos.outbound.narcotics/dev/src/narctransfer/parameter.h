//----------------------------------------------------------------------------
/*! \file
 *  \brief  application parameter
 *  \author Michael Eichenlaub
 *  \date   27.02.2006
 */
//----------------------------------------------------------------------------

#ifndef GUARD_PARAMETER_H
#define GUARD_PARAMETER_H

//------------------------------------------------------------------------------
//! application parameters \n final class
class ParameterImpl
{
	friend class basar::cmnutil::Singleton<ParameterImpl>;	//!< singleton class

public:
	//! init object with command line arguments	\n no-throw
	void				init			(basar::Int32   argC,	//!< argument number
									     char* argV[]	//!< argument values
										);

	const basar::VarString	getProcessName	() const;		//!< process name
	basar::Int16			getBranchNo		() const;		//!< branchNo

private:
						ParameterImpl	();
						ParameterImpl	(const ParameterImpl & rCopy);	//!< forbidden
	ParameterImpl &		operator =		(const ParameterImpl & rCopy);	//!< forbidden

	//! evaluate commandline arguments	\n rethrow basar::cmnutil::CmdLineTokenizer exceptions
	void				evalCmdLine		(basar::Int32   argC,	//!< argument number
									     char* argV[]	//!< argument values
										);

	basar::VarString	m_ProcessName;				//!< process name
	basar::Int16		m_BranchNo;					//!< branch no
};

//------------------------------------------------------------------------------------------------------------//
typedef basar::cmnutil::Singleton<ParameterImpl> Parameter;		//!< parameter evaluation singleton

//---------------------------------------------------------------------------------------//

#endif //GUARD_PARAMETER_H
