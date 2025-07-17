/*------------------------------------------------------------------------------*/
/*! \file
 *  \brief  ESQL/C connect functions
 *  \author Michael Eichenlaub
 *  \date   02.08.2005
 */
/*------------------------------------------------------------------------------*/

#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "esqlc.h"

#ifdef __GNUC__
#pragma GCC diagnostic ignored "-Wmissing-field-initializers"
#endif

/*------------------------------------------------------------------------------*/
/*! length of host variable      */
#define HOST_VAR_LEN		128					
/*! length of statement variable */
#define STMT_VAR_LEN		 64					

/*------------------------------------------------------------------------------*/
/*! \throw no-throw 
*   \return enum indicating success or failure
*/
enum InfxRetCode infxPutEnv(char * env)
{
	int ret;
	
	assert(env        != 0);
	assert(strlen(env) > 0);
	
/*! \ingroup DIFF_WIN_UNIX */ 
#ifdef _WIN32
	ret = ifx_putenv(env);
#else
	ret = putenv(env);
#endif

	return (ret == 0) ? RETCODE_OK : RETCODE_ERROR;
}

/*------------------------------------------------------------------------------*/
/*! \throw no-throw 
*   \return const char pointer to string value if set, empty string otherwise
*/
const char * infxGetEnv (const char * const name)
{
	static const char * const empty = "";
	
	const char * pValue;
	
	assert(name        != 0);
	assert(strlen(name) > 0);
	
/*! \ingroup DIFF_WIN_UNIX */ 
#ifdef _WIN32
	pValue = ifx_getenv((char *)name);
#else
	pValue = getenv(name);
#endif

	return pValue ? pValue : empty;
}

/*------------------------------------------------------------------------------*/
/*! \throw no-throw 
*/
void infxConnectDb (const char * const pConn       ,
					const char * const pConnId     ,
				    const int          flagTransact)
{
	EXEC SQL begin declare section;
	char h_Conn  [HOST_VAR_LEN];
	char h_ConnId[HOST_VAR_LEN];
	EXEC SQL end declare section;
	
	assert(strlen(pConn  ) < HOST_VAR_LEN);
	assert(strlen(pConnId) < HOST_VAR_LEN);
	assert(strlen(pConn  ) > 0);
	assert(strlen(pConnId) > 0);
	
	strcpy(h_Conn   , pConn );
	strcpy(h_ConnId, pConnId);
	
	if (flagTransact)
		EXEC SQL connect to :h_Conn
                         as :h_ConnId with concurrent transaction;
	else
		EXEC SQL connect to :h_Conn
			             as :h_ConnId;
}

/*------------------------------------------------------------------------------*/
/*! \throw no-throw 
*/
void infxConnectDbUser (const char * const pConn       ,
						const char * const pConnId     ,
						const char * const pUser       ,
						const char * const pPasswd     ,
				        const int          flagTransact)
{
	EXEC SQL begin declare section;
	char h_Conn  [HOST_VAR_LEN];
	char h_ConnId[HOST_VAR_LEN];
	char h_User  [HOST_VAR_LEN];
	char h_Passwd[HOST_VAR_LEN];
	EXEC SQL end declare section;
	
	assert(strlen(pConn  ) < HOST_VAR_LEN);
	assert(strlen(pConnId) < HOST_VAR_LEN);
	assert(strlen(pConn  ) > 0);
	assert(strlen(pConnId) > 0);
	assert(strlen(pUser  ) < HOST_VAR_LEN);
	assert(strlen(pPasswd) < HOST_VAR_LEN);
	
	strcpy(h_Conn  , pConn  );
	strcpy(h_ConnId, pConnId);
	strcpy(h_User  , pUser  );
	strcpy(h_Passwd, pPasswd);
	
	if (flagTransact)
		EXEC SQL connect to    :h_Conn
						 as    :h_ConnId
						 user  :h_User 
						 using :h_Passwd with concurrent transaction;
	else
		EXEC SQL connect to    :h_Conn
						 as    :h_ConnId
						 user  :h_User 
						 using :h_Passwd;
}

/*------------------------------------------------------------------------------*/
/*! \throw no-throw 
*/
void infxDisconnectDb (const char * const pConnId)
{
	EXEC SQL begin declare section;
	char h_ConnId[HOST_VAR_LEN];
	EXEC SQL end declare section;
	
	assert(strlen(pConnId) < HOST_VAR_LEN);
	assert(strlen(pConnId) > 0);
	
	strcpy(h_ConnId, pConnId);

	EXEC SQL disconnect :h_ConnId;
}

/*------------------------------------------------------------------------------*/
/*! no DB server traffic
    \throw no-throw 
*/
void infxSetCurrentDb (const char * const pConnId)
{
	EXEC SQL begin declare section;
	char h_ConnId[HOST_VAR_LEN];
	EXEC SQL end declare section;
	
	assert(strlen(pConnId) < HOST_VAR_LEN);
	assert(strlen(pConnId) > 0);
	
	strcpy(h_ConnId, pConnId);

	EXEC SQL set connection :h_ConnId;
}

/*------------------------------------------------------------------------------*/
/*! \throw    no-throw
 */ 
void infxBeginwork()
{
	EXEC SQL begin work;
}

/*------------------------------------------------------------------------------*/
/*! \throw    no-throw
 */ 
void infxCommitwork()
{
	EXEC SQL commit work;
}

/*------------------------------------------------------------------------------*/
/*! \throw    no-throw
 */ 
void infxRollbackwork()
{
	EXEC SQL rollback work;
}

/*------------------------------------------------------------------------------*/
/*! \throw no-throw */
void infxSetIsoLevel(enum InfxIsoLevelEnum eLevel,
                     int                   flagLocks)
{
	switch (eLevel)
	{
	case DIRTYREAD:
		if (flagLocks)
			EXEC SQL set isolation to dirty read retain update locks;
		else
			EXEC SQL set isolation to dirty read;
		break;
		
	case DIRTYREAD_WITHWARNING:
		if (flagLocks)
			EXEC SQL set isolation to dirty read with warning retain update locks;
		else
			EXEC SQL set isolation to dirty read with warning;
		break;
		
	case COMITTEDREAD:
		if (flagLocks)
			EXEC SQL set isolation to committed read retain update locks;
		else
			EXEC SQL set isolation to committed read;
		break;
		
	/* workaround: CSDK 2.90 doesn't know "last committed" */		
    case COMITTEDREAD_LASTCOMMITTED:		
		if (flagLocks)
			EXEC SQL execute immediate 'set isolation to committed read last committed retain update locks';
		else
			EXEC SQL execute immediate 'set isolation to committed read last committed';
		break;
		
	case CURSORSTABILITY:
		if (flagLocks)
			EXEC SQL set isolation to cursor stability retain update locks;
		else
			EXEC SQL set isolation to cursor stability;
		break;
		
	case REPEATABLEREAD:
		EXEC SQL set isolation to repeatable read;
		break;
	}
}

/*------------------------------------------------------------------------------*/
/*! \throw no-throw 
*/
void infxSetLockMode(enum InfxLockModeEnum eMode,
					 unsigned int          secs )
{
    EXEC SQL BEGIN DECLARE SECTION;
    char stmtlock[STMT_VAR_LEN]; 
    EXEC SQL END   DECLARE SECTION;
    
	switch (eMode)
	{
	case NOTWAIT:
		EXEC SQL set lock mode to not wait;
		break;
		
	case WAIT:
		EXEC SQL set lock mode to wait;
		break;
		
	case WAITSECS:
		sprintf(stmtlock, "set lock mode to wait %d", secs);
		
		assert(strlen(stmtlock) < STMT_VAR_LEN);
	
		EXEC SQL execute immediate :stmtlock;
		break;
	}
}

/*------------------------------------------------------------------------------*/
/*! \throw    no-throw
 */ 
void infxSetFetBufSize (int size)
{
	FetBufSize = size;
}

/*------------------------------------------------------------------------------*/
/*! \throw    no-throw
*   \return size of fetch buffer
 */ 
int infxGetFetBufSize()
{
	return FetBufSize;
}

/*------------------------------------------------------------------------------*/

