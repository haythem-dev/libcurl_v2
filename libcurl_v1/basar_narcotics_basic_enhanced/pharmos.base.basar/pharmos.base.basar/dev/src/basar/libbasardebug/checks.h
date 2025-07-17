//----------------------------------------------------------------------------
/*! \file
 *  \brief  class for debug checks
 *  \n       - check memory leaks        
 *  \author Michael Eichenlaub
 *  \date   10.10.2005
 */
//----------------------------------------------------------------------------

#ifndef GUARD_CHECKS_H
#define GUARD_CHECKS_H

/*! \ingroup DIFF_WIN_UNIX */ 
#ifdef _WIN32
#include <crtdbg.h>
#endif

//----------------------------------------------------------------------------
namespace basar
{
namespace debug
{

//---------------------------------------------------------------------------
//! class for debug checks \n final class
class Checks
{
public:

	//! Windows heap state snapshot (only used for Windows!)
	struct MemState
	{
		int blocks;		//!< number of blocks
		int bytes;		//!< total bytes allocated in blocks
	};

	Checks	();							//!< constructor								\n no-throw
   ~Checks	();							//!< at destructing last checks are executed	\n no-throw

	bool			m_flagCheck;		//!< switch on/off checking
	bool			m_flagIgnoreSet;	//!< is initialized?
	MemState		m_Ignore;			//!< snapshot deviation to ignore

private:

/*! \ingroup DIFF_WIN_UNIX */ 
#ifdef _WIN32
	_CrtMemState	m_start;			//!< memory snapshot at begin
	_CrtMemState	m_stop;				//!< memory snapshot at end
#endif
};

//----------------------------------------------------------------------------
//! \brief get check object (lazy instantiation)
//! 
//!        init static variable, should be first constructed object in process. 
//!        So it also is last destructed object and last measure point in process
//! \return reference to static(!) check instance
Checks & check	();

//----------------------------------------------------------------------------
}	// namespace debug
}	// namespace basar

//----------------------------------------------------------------------------
#endif

