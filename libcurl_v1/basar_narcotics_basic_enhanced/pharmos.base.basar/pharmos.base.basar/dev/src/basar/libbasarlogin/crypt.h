//----------------------------------------------------------------------------
/*! \file
 *  \brief  use case declarations for crypting passwords
 *  \author Thomas Hörath
 *  \date   20.12.2005
 */
//----------------------------------------------------------------------------

#ifndef GUARD_CRYPT_H
#define GUARD_CRYPT_H

//----------------------------------------------------------------------------
#include "libbasar_definitions.h"

//----------------------------------------------------------------------------
namespace basar {
namespace login	{

//----------------------------------------------------------------------------
//! namespace for common utilities in basar::login
namespace util {

/*! \brief	encrypt 1st parameter
	\n		no-throw */
void    encryptPW       (basar::ConstString InDecrypted,		
                        char       *		OutEncrypted,		
						const basar::Int32  Length			
						);

/*! \brief	decrypt 1st parameter
	\n		no-throw */
void    decryptPW      (char * const	   InEncrypted,	
                        char       *       OutDecrypted,
                        const basar::Int32 Length		
						);

//----------------------------------------------------------------------------
} // util
} // login
} // basar

//----------------------------------------------------------------------------
#endif	// GUARD
