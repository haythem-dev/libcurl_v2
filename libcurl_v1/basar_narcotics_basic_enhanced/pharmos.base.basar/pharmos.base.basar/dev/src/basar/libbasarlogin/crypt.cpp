//----------------------------------------------------------------------------
/*! \file
 *  \brief  encryption implementation
 *  \author	Thomas Hörath
 *  \date   20.12.2005
 */
//----------------------------------------------------------------------------

#include "crypt.h"

#include <ctype.h>
#include "libbasarcmnutil_bstring.h"
#include "loggerpool.h"

//----------------------------------------------------------------------------
namespace basar {
namespace login {

//----------------------------------------------------------------------------
namespace util {

/* NOCHAR's will not be encrypted, if encrypting results in NOCHAR, the original sign will be taken as encryption */
/* because of SQL, NOCHAR's are not accepted as encryptions */
#define NOCHAR_1  127	//!< ascii 127 is not encrypted
#define NOCHAR_2  '"'	//!< " is not encrypted
#define NOCHAR_3  '\''	//!< ' is not encrypted


static int  perm[] = { 2, 7, 1, 5, 3, 0, 4, 6 };		//!< int-array of permutations

#define PERM_ANZ ( sizeof( perm ) / sizeof( int ) )		//!< number of permutations

static char offs[] =
  { 0X1C, 0X08, 0X1B, 0X05, 0X17, 0X0D, 0X13, 0X09 };	//!< char-array of offset

#define OFFS_ANZ ( sizeof( offs ) / sizeof( char ) )	//!< number of offsets

//----------------------------------------------------------------------------
/*!	\throw  no-throw 
	\param InDecrypted	input  = decrypted
	\param OutEncrypted result = encrypted
	\param Length		length of result */
void encryptPW     (basar::ConstString InDecrypted, 
                    char       *       OutEncrypted, 
					const basar::Int32 Length)
{
	basar::Int32 i;
	basar::Int32 j;

	for ( i = 0; i < Length; ++i )
	{
		j = perm[i%PERM_ANZ];

		if ( InDecrypted[j] == NOCHAR_1 ||
		  	 InDecrypted[j] == NOCHAR_2 ||
			 InDecrypted[j] == NOCHAR_3   )
		{
			OutEncrypted[i] = InDecrypted[j];
		}
		else
		{
			OutEncrypted[i] = ( InDecrypted[j] ^ offs[i%OFFS_ANZ] ) & (char) 0X7F;

			if ( OutEncrypted[i] == NOCHAR_1 ||
			 	 OutEncrypted[i] == NOCHAR_2 ||
				 OutEncrypted[i] == NOCHAR_3   )
			{
				OutEncrypted[i] = InDecrypted[j];
			}
		}
	}

	for ( i = 0; i < Length; i++ )
	{
		if ( isupper(OutEncrypted[i]) && OutEncrypted[i] & (char) 0X01 )
			OutEncrypted[i] = (char) tolower( OutEncrypted[i] );
		else 
		{
			if ( islower(OutEncrypted[i]) && OutEncrypted[i] & (char) 0X01 )
				OutEncrypted[i] = (char) toupper( OutEncrypted[i] );
		}
	}

	OutEncrypted[Length] = '\0';

	if (LoggerPool::loginCrypt().isEnabledFor(log4cplus::DEBUG_LOG_LEVEL))
	{
		VarString msg;
		msg.format("<%s> crypted to <%s>.", InDecrypted, OutEncrypted);
		LoggerPool::loginCrypt().forcedLog(log4cplus::DEBUG_LOG_LEVEL, msg, __FILE__, __LINE__);
	}
}

//----------------------------------------------------------------------------
/*!	\throw  no-throw 
	\param InEncrypted	input  = encrypted
	\param OutDecrypted result = decrypted
	\param Length	    length of result */
void decryptPW   (char * const InEncrypted, 
				  char       * OutDecrypted, 
				  const basar::Int32 Length)
{
	basar::Int32 i;
    basar::Int32 j;

    for ( i = 0; i < Length; ++i )
	{
      if ( isupper(InEncrypted[i]) && InEncrypted[i] & (char) 0X01 )
         InEncrypted[i] = (char) tolower( InEncrypted[i] );
      else 
	  {
		  if ( islower(InEncrypted[i]) && InEncrypted[i] & (char) 0X01 )
			InEncrypted[i] = (char) toupper( InEncrypted[i] );
	  }
	}

    for ( i = 0; i < Length; i++ )
    {
        j = perm[i%PERM_ANZ];

        if ( InEncrypted[i] == NOCHAR_1 ||
             InEncrypted[i] == NOCHAR_2 ||
             InEncrypted[i] == NOCHAR_3   )
        {
			OutDecrypted[j] = InEncrypted[i];
        }
        else
        {
            OutDecrypted[j] = ( InEncrypted[i] ^ offs[i%OFFS_ANZ] ) & (char) 0X7F;

            if ( OutDecrypted[j] == NOCHAR_1 ||
                 OutDecrypted[j] == NOCHAR_2 ||
                 OutDecrypted[j] == NOCHAR_3   )
			{
				OutDecrypted[j] = InEncrypted[i];
			}
        }
    }

    OutDecrypted[Length] = '\0';

	if (LoggerPool::loginCrypt().isEnabledFor(log4cplus::DEBUG_LOG_LEVEL))
	{
		VarString msg;
		msg.format("<%s> decrypted to <%s>.", InEncrypted, OutDecrypted);
		LoggerPool::loginCrypt().forcedLog(log4cplus::DEBUG_LOG_LEVEL, msg, __FILE__, __LINE__);
	}
}

//----------------------------------------------------------------------------
}//util
}//login
}//basar
