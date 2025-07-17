//----------------------------------------------------------------------------
/*! \file
 *  \brief  public library interface: classes for piped child process
 *  \author Michael Eichenlaub
 *  \date   17.05.2006
 */
//----------------------------------------------------------------------------

#ifndef GUARD_LIBBASARCMNUTIL_PIPEDPROCESS_H
#define GUARD_LIBBASARCMNUTIL_PIPEDPROCESS_H

//------------------------------------------------------------------------------
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
#include <boost/shared_ptr.hpp>
#include "libbasar_definitions.h"
#include "libbasarcmnutil_definitions.h"
#include "libbasarcmnutil_bstring.h"

//----------------------------------------------------------------------------
namespace basar		{
namespace cmnutil	{

//----------------------------------------------------------------------------
/*! \brief  provides piped process data. \n interface class */
class IProcessDataProvider
{
public:
	//! \brief virtual destructor
	virtual			   ~IProcessDataProvider	() {};

	/*! \brief callback method: let caller send standard input data \n pure virtual, must be implemented */
	virtual void		onSendInput				(CollStdIo & collInput	//!< collection for standard input data to piped process
												) = 0;
	//! \brief   callback method: let caller receive standard output data \n pure virtual, must be implemented
	virtual void		onReceiveOutput			(CollStdIo & collOutput	//!< collection for standard output data from piped process
												) = 0;
	//! \brief  callback method: let caller receive standard error data \n pure virtual, must be implemented
	virtual void		onReceiveError			(CollStdIo & collError	//!< collection for standard error data from piped process
												) = 0;
};

//----------------------------------------------------------------------------
class Pipe;		// forward

//----------------------------------------------------------------------------
//! \brief  child process with pipe to parent
//! final class
class PipedProcess
{
public:
	//! \brief constructor	\n no-throw
	LIBBASARCMNUTIL_API						PipedProcess		(const VarString      & command			//!< command executed by child process
																);
	//! \brief constructor	\n no-throw
	LIBBASARCMNUTIL_API						PipedProcess		(const VarString      & command,		//!< command executed by child process
																 IProcessDataProvider & rDataProvider	//!< provides child with input, receives child's output
																);
	//! \brief destructor	\n no-throw
	LIBBASARCMNUTIL_API					   ~PipedProcess		();

	
	LIBBASARCMNUTIL_API Int32				execute				();		//!< execute child process	\n no-throw


	LIBBASARCMNUTIL_API CollStdIo &			getInput			();		//!< get reference to child's stdinput collection	\n no-throw
	LIBBASARCMNUTIL_API CollStdIo &			getOutput			();		//!< get reference to child's stdoutput collection	\n no-throw
	LIBBASARCMNUTIL_API CollStdIo &			getError			();		//!< get reference to child's stderror collection	\n no-throw

private:
	
	typedef boost::shared_ptr < Pipe >	InternMemberSharedPtr;	//!< shared pointer	to internal object with pipe descriptors/handles


											PipedProcess		();                           			//!< forbidden
											PipedProcess		(const PipedProcess & rCopy);			//!< forbidden
	PipedProcess &    						operator =			(const PipedProcess & rCopy);			//!< forbidden


	
	void									createPipe			();			//!< create pipes		\n throw RuntimeException
	//! \brief close pipes				\n throw RuntimeException
	void									closePipe			();
	//! read from pipe into collection	\n throw RuntimeException
	bool									readPipe			(void      * pPipe,		//!< pipe to read from
		                                                         CollStdIo & rColl		//!< collection to write into
																);
	//! write from collection into pipe	\n throw RuntimeException
	bool									writePipe			(void      * pPipe,		//!< pipe to write into
		                                                         CollStdIo & rColl		//!< collection to read from
																);

	
	void									createProcess		();			//!< create child process				\n throw RuntimeException
	void									handleProcessData	();			//!< manage data transfer to/from child	\n throw RuntimeException
	Int32									closeProcess		() const;	//!< close child process				\n throw RuntimeException

	void									clearColls			();			//!< empty collections with child data	\n no-throw


	InternMemberSharedPtr					m_pIntern;				//!< shared pointer to internal pipe descriptors/handles
	IProcessDataProvider *                  m_pDataProvider;		//!< pointer to data provider (receives/sends child data)

	VarString								m_Command;				//!< command executed by child process

	CollStdIo								m_CollInput;			//!< collection with child's stdinput
	CollStdIo								m_CollOutput;			//!< collection with child's stdoutput
	CollStdIo								m_CollError;			//!< collection with child's stderror
};

//-----------------------------------------------------------------------------
}	// namespace cmnutil
}	// namespace basar

//-----------------------------------------------------------------------------
#endif	// GUARD
