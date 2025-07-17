/*------------------------------------------------------------------------------*/
/*! \file
 *  \brief  ESQL/C exception handling
 *  \author Michael Eichenlaub
 *  \date   05.08.2005
 */
/*------------------------------------------------------------------------------*/

#include "esqlc.h"

#include <string.h>
#include <assert.h>

#ifdef __GNUC__
#pragma GCC diagnostic ignored "-Wmissing-field-initializers"
#endif

/*------------------------------------------------------------------------------*/
/*!         no DB server traffic
    \throw  no-throw */
void infxClearSqlState()
{
	SQLSTATE[0] = '0';	/* clear previous state --> set to InfxSqlStateEnum::SUCCESS */
	SQLSTATE[1] = '0';	
}

/*------------------------------------------------------------------------------*/
/*!         no DB server traffic
    \throw  no-throw
    \return Informix SQLSTATE exception condition */
enum InfxSqlStateEnum infxGetSqlState()
{
    if ('0' == SQLSTATE[0])
    {
	    switch (SQLSTATE[1])
        {
        case '0': 
            return SQL_SUCCESS;
            
        case '1':
            return SQL_WARNING;
            
        case '2':
            return SQL_NOTFOUND;
            
        default:
            break;
        }
    }

    return SQL_ERROR;
}

/*------------------------------------------------------------------------------*/
/*!         no DB server traffic
    \throw  no-throw
    \return number of diagnostic area's exceptions for most-recently executed SQL statement */
int infxGetCountDiagExcepts ()
{
	EXEC SQL BEGIN DECLARE SECTION;
	int count;
	EXEC SQL END DECLARE SECTION;
	
	EXEC SQL get diagnostics :count = NUMBER;
	
	return count;
}

/*------------------------------------------------------------------------------*/
/*!         no DB server traffic
    \throw  no-throw
    \return number of rows affected by statement */
int infxGetCountDiagRows ()
{
	EXEC SQL BEGIN DECLARE SECTION;
	int count;
	EXEC SQL END DECLARE SECTION;

	EXEC SQL get diagnostics :count = ROW_COUNT;
	
	return count;
}

/*------------------------------------------------------------------------------*/
/*!         no DB server traffic
    \throw  no-throw
    \retval   0 failure or successful non-insert statement or successful insert statement w/o serial column
    \retval !=0 serial value generated for inserted row */
bigint infxGetSerialValue ()
{
	/* if successful then use generated serial value */
	if (sqlca.sqlcode == 0)
	{
		/* check if set: serial, if not set: serial8 */
		if (sqlca.sqlerrd[1])
		{
			return (bigint)sqlca.sqlerrd[1];
		}
		else
		{
			ifx_int8_t serial8;
			bigint bigserial;
			ifx_getserial8(&serial8);

			bigserial = convint8tobigint(&serial8);

			/*try for bigserial*/
			if (0 == bigserial)
			{
				ifx_getbigserial(&bigserial);
			}
			return bigserial;
		}
	}

	return (bigint)0;
}

/*------------------------------------------------------------------------------*/
/*!        no DB server traffic
    \throw  no-throw 
    \return main and additional error codes */
struct InfxErrorCodes infxGetErrorCodes ()
{
	struct InfxErrorCodes errors = {0, 0};
	
	/* if error code is given then use ISAM error code */
	if (sqlca.sqlcode < 0)
	{
		errors.main = sqlca.sqlcode;
		errors.isam = sqlca.sqlerrd[1];  
	}
	
	return errors;
}

/*------------------------------------------------------------------------------*/
/*!        no DB server traffic
    \throw no-throw */
void infxGetFirstDiagExcept 
(
	/*! \cond x */	EXEC SQL BEGIN DECLARE SECTION;	/*! \endcond */
	/*! \cond x */	PARAMETER						/*! \endcond */		char  dbServer[],		
	/*! \cond x */	PARAMETER						/*! \endcond */		char  ConnId  [],		
	/*! \cond x */	PARAMETER						/*! \endcond */		char  msgTxt  [],		
	/*! \cond x */	PARAMETER						/*! \endcond */		char  sqlState[],		
	/*! \cond x */	PARAMETER						/*! \endcond */		int * pSqlcode		
	/*! \cond x */	EXEC SQL END DECLARE SECTION;	/*! \endcond */
)
{
	EXEC SQL BEGIN DECLARE SECTION;
    mint msgLen = 0;
	EXEC SQL END DECLARE SECTION;


	EXEC SQL get diagnostics exception 1
							: dbServer = SERVER_NAME      ,
							: ConnId   = CONNECTION_NAME  ,
							: msgTxt   = MESSAGE_TEXT     ,
							: msgLen   = MESSAGE_LENGTH   ,
							: sqlState = RETURNED_SQLSTATE,
							:*pSqlcode = INFORMIX_SQLCODE ;
			
	assert(strlen(dbServer) < LEN_MESSAGE_TEXT);
	assert(strlen(ConnId)   < LEN_CONNECT_NAME);
	assert(msgLen           < LEN_MESSAGE_TEXT);
	assert(strlen(msgTxt)   < LEN_MESSAGE_TEXT);
	assert(strlen(sqlState) < LEN_SQLSTATE);
}

/*------------------------------------------------------------------------------*/
/*!        no DB server traffic
    \throw no-throw */
void infxGetNextDiagExcept 
(
	/*! \cond x */	EXEC SQL BEGIN DECLARE SECTION;	/*! \endcond */
	/*! \cond x */	PARAMETER						/*! \endcond */		int   except    ,	
	/*! \cond x */	PARAMETER						/*! \endcond */		char  msgTxt  [],		
	/*! \cond x */	PARAMETER						/*! \endcond */		char  sqlState[],		
	/*! \cond x */	PARAMETER						/*! \endcond */		int * pSqlCode		
	/*! \cond x */	EXEC SQL END DECLARE SECTION;	/*! \endcond */
)
{
	EXEC SQL BEGIN DECLARE SECTION;
    mint msgLen;
	EXEC SQL END DECLARE SECTION;

	EXEC SQL get diagnostics exception :except
							: msgTxt   = MESSAGE_TEXT     ,
							: msgLen   = MESSAGE_LENGTH   ,
							: sqlState = RETURNED_SQLSTATE,
							:*pSqlCode = INFORMIX_SQLCODE;
							
	assert(msgLen           < LEN_MESSAGE_TEXT);
	assert(strlen(msgTxt)   < LEN_MESSAGE_TEXT);
	assert(strlen(sqlState) < LEN_SQLSTATE);
}

/*------------------------------------------------------------------------------*/
#ifdef DUMP_ENABLE
#include <stdio.h>

int infxDumpException()
{
EXEC SQL BEGIN DECLARE SECTION;
    int  exception_count;
    int  row_count;
    char overflow		[  2];
    
    char class_id		[255];
    char subclass_id	[255];
    char message		[255];
    int  messlen;
    char sqlstate_code	[  6];
    int  sqlcode;
    char servername		[255];
    char connid			[255];
    int  i;
EXEC SQL END DECLARE SECTION;

    printf("=== sql exception handling =====================================================\n");
    printf("\n");
    printf("SQLSTATE: %s\n", SQLSTATE);
    printf("SQLCODE : %d\n", SQLCODE);
    printf("\n");
 
    EXEC SQL get diagnostics :exception_count = NUMBER, 
                             :overflow        = MORE, 
                             :row_count       = ROW_COUNT;
    
    printf("EXCEPTIONS: Number: %d\t", exception_count);
    printf("More? %s\t"              , overflow       );
    printf("Row Count: %d\n"         , row_count      );

    for (i = 1; i <= exception_count; i++)
    {
        EXEC SQL get diagnostics exception :i
											:sqlstate_code = RETURNED_SQLSTATE,
											:sqlcode       = INFORMIX_SQLCODE,
											:class_id      = CLASS_ORIGIN,
											:subclass_id   = SUBCLASS_ORIGIN,
											:message       = MESSAGE_TEXT,
											:messlen       = MESSAGE_LENGTH,
											:servername    = SERVER_NAME,
											:connid        = CONNECTION_NAME;
											
	    assert(strlen(sqlstate_code) < sizeof(sqlstate_code));
	    assert(strlen(class_id)      < sizeof(class_id     ));
	    assert(strlen(subclass_id)   < sizeof(subclass_id  ));
	    assert(messlen               < sizeof(message      ));
	    assert(strlen(message)       < sizeof(message      ));
	    assert(strlen(servername)    < sizeof(servername   ));
	    assert(strlen(connid)        < sizeof(connid       ));
											
        printf("- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - \n");
        printf("EXCEPTION %d:\n", i);
        printf("    SQLSTATE       : %s\n", sqlstate_code);
        printf("    SQLCODE        : %d\n", sqlcode);
        printf("    MESSAGE TEXT   : %s\n", message);
        printf("    CLASS ORIGIN   : %s\n",class_id);
        printf("    SUBCLASS ORIGIN: %s\n", subclass_id);
        printf("    SERVER NAME    : %s\n", servername);
        printf("    CONNECTION_NAME: %s\n", connid);
    }

    printf("--------------------------------------------------------------------------------\n");
    
    printf("SQLCODE       : %d\n", SQLCODE);
    printf("\n");
    
    printf("sqlca.sqlcode : %d\n", sqlca.sqlcode);
    
    assert(strlen(sqlca.sqlerrm) < 72);
    printf("sqlca.sqlerrm : '%s'\n", sqlca.sqlerrm);

	printf("sqlca.sqlerrp : '");
    for (i = 0; i < 8; ++i)
    {
		printf("%c", sqlca.sqlerrp[i]);
    }
    printf("'\n");

	printf("sqlca.sqlerrd : ");
    for (i = 0; i < 6; ++i)
    {
		printf("[%d]=%d; ", i, sqlca.sqlerrd[i]);
    }
    printf("\n");

    printf("sqlca.sqlwarn : '%c%c%c%c%c%c%c%c'\n", 
            sqlca.sqlwarn.sqlwarn0,
            sqlca.sqlwarn.sqlwarn1,
            sqlca.sqlwarn.sqlwarn2,
            sqlca.sqlwarn.sqlwarn3,
            sqlca.sqlwarn.sqlwarn4,
            sqlca.sqlwarn.sqlwarn5,
            sqlca.sqlwarn.sqlwarn6,
            sqlca.sqlwarn.sqlwarn7);
            
	return 0;            
}
#endif
