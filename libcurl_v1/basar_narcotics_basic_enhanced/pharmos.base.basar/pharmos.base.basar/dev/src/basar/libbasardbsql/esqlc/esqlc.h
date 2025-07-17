/*------------------------------------------------------------------------------*/
/*! \file
 *  \brief  ESQL/C library interface
 *  \author Michael Eichenlaub
 *  \date   15.07.2005
 */
/*------------------------------------------------------------------------------*/

#ifndef GUARD_ESQLC_H
#define GUARD_ESQLC_H

#include <ifxtypes.h>

/*------------------------------------------------------------------------------*/
#ifdef __cplusplus
extern "C" 
{
#endif

/*##############################################################################*/
/*#  exception handling                                                        #*/
/*##############################################################################*/

/* --- define section ----------------------------------------------------*/

/*! length of SQLSTATE */
#define LEN_SQLSTATE		  6	

/*! length of exception message text */
#define LEN_MESSAGE_TEXT	8191			

/*! length of db server name */
#define LEN_SERVER_NAME		 255			

/*! length of connection name */
#define LEN_CONNECT_NAME	 255			


/* --- enum section ------------------------------------------------------*/

/*! \brief Informix SQLSTATE exception conditions */
enum InfxSqlStateEnum	
{
	SQL_SUCCESS,	/*!< SQL statement executed successfully									*/
	SQL_NOTFOUND,	/*!< SQL statement encountered NOT FOUND									*/
	SQL_WARNING,	/*!< SQL statement executed successfully, but encountered warning condition	*/
	SQL_ERROR		/*!< SQL statement resulted in runtime error								*/
};

/*! \brief informix error codes */
struct InfxErrorCodes
{
	int main;	/*!< SQL statement's success or main error code */
	int isam;	/*!< zero or an additional error code, called ISAM error code, that explains cause of main error */
};



/* --- function section --------------------------------------------------*/

/*! \brief clear SQLSTATE status variable \n C function */
void					infxClearSqlState		();

/*! \brief check SQLSTATE status variable \n C function */
enum InfxSqlStateEnum	infxGetSqlState			();

/*! \brief get number of diagnostic area's exceptions for most-recently executed SQL statement \n C function */
int						infxGetCountDiagExcepts	();
/*! \brief get number of processed rows for most-recently executed SQL statement \n C function */
int						infxGetCountDiagRows	();
/*! \brief get serial value generated for sucessfully inserted row	\n C function */
bigint					infxGetSerialValue		();
/*! \brief get informix error codes (main and additional) \n C function */
struct InfxErrorCodes	infxGetErrorCodes		();

/*! \brief get first exception info in diagnostic area for most-recently executed SQL statement \n C function */
void					infxGetFirstDiagExcept	(char  dbServer[],		/*!< database server name								*/
												 char  ConnId  [],		/*!< connection ID										*/
												 char  msgTxt  [],		/*!< message text to describe exception					*/
												 char  sqlState[],		/*!< SQLSTATE value										*/
												 int * pSqlCode		 	/*!< pointer to Informix-specific status code SQLCODE	*/
												);
/*! \brief get following exception info in diagnostic area for most-recently executed SQL statement \n C function */
void					infxGetNextDiagExcept	(int   except    ,		/*!< exception index (1, ..., n) in diagnostics area	*/
												 char  msgTxt  [],		/*!< message text to describe exception					*/
												 char  sqlState[],		/*!< SQLSTATE value										*/
												 int * pSqlCode		 	/*!< pointer to Informix-specific status code SQLCODE	*/
												);

/*##############################################################################*/
/*#  descriptor handling                                                       #*/
/*##############################################################################*/

/* --- enum section ------------------------------------------------------*/

/*! \brief simplified Informix SQL types */
enum InfxSqlTypeEnum	
{
	TYPE_INVALID,	/*!< type invalid/unknown					*/

	TYPE_STRING,	/*!< SQL type char, varchar, longvarchar	*/
	TYPE_DOUBLE,	/*!< SQL type float							*/
	TYPE_DECIMAL,	/*!< SQL type decimal, money				*/
	TYPE_FLOAT,		/*!< SQL type small float					*/
	TYPE_INT,		/*!< SQL type int, serial					*/
	TYPE_SHORT,		/*!< SQL type small int						*/
	TYPE_DATE,		/*!< SQL type date							*/
	TYPE_DATETIME,	/*!< SQL type datetime						*/
    TYPE_INTERVAL,	/*!< SQL type interval						*/
    TYPE_TEXT,      /*!< SQL type text                          */
    TYPE_BYTE,      /*!< SQL type byte                          */
    TYPE_INT8,      /*!< SQL type INT8, BIGINT                  */
	TYPE_UNKNOWN	/*!< SQL type unknown						*/
};

/*! \brief custom SQL types, overlay for Informix SQL types. see cmnutil classes */
enum InfxCustomTypeEnum		
{
	TYPE_CUSTOM_DECIMAL  = 0,	/*!< custom decimal  type => class cmnutil::Decimal  */
	TYPE_CUSTOM_DATE, 			/*!< custom date     type => class cmnutil::Date     */
	TYPE_CUSTOM_DATETIME, 		/*!< custom datetime type => class cmnutil::DateTime */
    TYPE_CUSTOM_INTERVAL,       /*!< custom interval type => class cmnutil::TimeInterval */

	TYPE_CUSTOM_LAST			/*!< total number of custom types                    */
};

enum InfxFreeLocBufferEnum
{
    FREE_LOC_BUFFER,
    DONT_FREE_LOC_BUFFER
};

/*! \brief array with custom SQL type size; initialized in basar::dbsql */
typedef const int	InfxCustomTypeSize[TYPE_CUSTOM_LAST];

/* --- forward section ---------------------------------------------------*/
struct sqlda;
struct ifx_int8_t;
/* --- function section --------------------------------------------------*/

/*! \brief convert ifx_int8_t value to bigint
\n     C function */
bigint           convint8tobigint   (const ifx_int8_t* pInt8 /*!< pointer to ifx_int8_t value */
                                                );

/*! \brief free previously allocated descriptor memory
    \n     C function */
void					infxFreeDesc			(struct sqlda * const pSqlda,		        /*!< sql descriptor area pointer */
                                                 enum InfxFreeLocBufferEnum freeLocBuffer   /*!< indicate if locBuffer should be freed */
												);

/*! \brief free previously allocated descriptor memory - windows
\n     C function */
void                    infxSqlFreeMem          (struct sqlda * const pSqlda,               /*!< sql descriptor area pointer */
                                                 enum InfxFreeLocBufferEnum freeLocBuffer   /*!< indicate if locBuffer should be freed */
                                                );
/*! \brief set BLOB locator entries to request buffer reallocation - required by CSDK after each fetch
\n     C function */
void                    infxSetDescAlloc        (struct sqlda * const pSqlda        /*!< sql descriptor area pointer */
                                                );

/*! \brief obtain information about prepared statement output before execution
    \n     C function */
struct sqlda *			infxDescribeStmtOut		(const char * pStmtId				/*!< unique statement ID */
												);

/*! \brief obtain information about prepared statement input ( = parameters in prepared statement) before execution
    \n     C function */
struct sqlda *			infxDescribeStmtIn		(const char * pStmtId				/*!< unique statement ID */
												);

/*! \brief free ( by infxDescribeStmt() ) implicitly allocated descriptor memory
\n     C function */
void					infxFreeImplicitDesc(struct sqlda * const pSqlda,		/*!< sql descriptor area pointer */
                                             enum InfxFreeLocBufferEnum freeLocBuffer /*!< indicate if locBuffer should be freed */
);

/*! \brief read from system descriptor area how many items are described
    \n     C function */
int						infxGetDescCount		(const struct sqlda * const pSqlda	/*!< sql descriptor area pointer */
												);

/*! \brief get Informix SQL column length
    \n     C function */
int						infxGetDescColLen		(const struct sqlda * const pSqlda,	/*!< sql descriptor area pointer               */
												 int                        item	/*!< descriptor (= column) index (0, ..., n-1) */
												);

/*! \brief read column name from descriptors
    \n     C function */
const char * 			infxGetDescColName		(const struct sqlda * const pSqlda,	/*!< sql descriptor area pointer				*/
												 int                        item	/*!< descriptor (= column) index (0, ..., n-1)	*/
												);

/*! \brief read column data from descriptors
    \n     C function */
const char *			infxGetDescColData		(const struct sqlda * const pSqlda,	/*!< sql descriptor area pointer				*/
												 int                        item	/*!< descriptor (= column) index (0, ..., n-1)	*/
												);

/*! \brief get Informix SQL column type - unused in basar
    \n     C function */
short					infxGetDescType			(const struct sqlda * const pSqlda,	/*!< sql descriptor area pointer				*/
												 int                        item    /*!< descriptor (= column) index (0, ..., n-1)	*/
												);

/*! \brief get Informix SQL column type name 
    \n     C function */
const char *			infxGetDescTypeName		(const struct sqlda * const pSqlda,	/*!< sql descriptor area pointer				*/
												 int                        item	/*!< descriptor (= column) index (0, ..., n-1)	*/
												);

/*! \brief get simplified Informix SQL column type
    \n     C function */
enum InfxSqlTypeEnum	infxGetDescSimpleType	(const struct sqlda * const pSqlda,	/*!< sql descriptor area pointer				*/
												 int                        item    /*!< descriptor (= column) index (0, ..., n-1)	*/
												);

/*! \brief get Informix SQL column indicator
    \n     C function */
const char *			infxGetDescIndicator	(const struct sqlda * const pSqlda,	/*!< sql descriptor area pointer				*/
												 int                        item	/*!< descriptor (= column) index (0, ..., n-1)	*/
												);

/*! \brief prepare output descriptors and get Informix SQL row length (aligned size)
    \n     C function */
int						infxPrepDescOut			(struct sqlda * const	pSqlda,		/*!< sql descriptor area pointer																*/
												 int					flagRaw		/*!< true: data types are treated as SQL types \n false: data types are converted to C types	*/
												);

/*! \brief prepare input descriptors
    \n     C function */
void					infxPrepDescIn			(struct sqlda * const	pSqlda,		/*!< sql descriptor area pointer																*/
												 int					flagRaw		/*!< true: data types are treated as SQL types \n false: data types are converted to C types	*/
												);
/*! \brief adjust descriptors with row buffer base address
    \n     C function */
void					infxAdjustDesc			(struct sqlda * const	pSqlda,		/*!< sql descriptor area pointer			*/
												 char  * const			pRow,		/*!< pointer to row buffer base				*/
												 short * const			pIndicator	/*!< pointer to indicator variables base	*/
                                                 );
/*! \brief adjust descriptors with row buffer base address for one item
    \n     C function */
void                    infxSetBufferToItemDesc	(struct sqlda * const pSqlda,       /*!< sql descriptor area pointer			*/
								                 char  * const        pRow,         /*!< pointer to row buffer base				*/
								                 short * const        pIndicator,   /*!< pointer to indicator variables base	*/
								                 int item                           /*!< position in description area which will be backed by row buffer */
                                                 );

/*! \brief read string value from descriptor
    \n     C function */
const char *			infxGetDescString		(struct sqlda * const	pSqlda,		/*!< sql descriptor area pointer				*/
												 int                    item		/*!< descriptor (= column) index (0, ..., n-1)	*/
												);

/*! \brief read int value from descriptor
    \n     C function */
const int *				infxGetDescInt   		(struct sqlda * const   pSqlda,		/*!< sql descriptor area pointer				*/
												 int                    item		/*!< descriptor (= column) index (0, ..., n-1)	*/
												);

/*! \brief read 8 byte int value from descriptor
\n     C function */
const bigint*    infxGetDescInt64        (struct sqlda * const pSqlda,       /*!< sql descriptor area pointer				*/
                                                 int item                           /*!< descriptor (= column) index (0, ..., n-1)	*/
                                                );

/*! \brief read short value from descriptor
    \n     C function */
const short *			infxGetDescShort   		(struct sqlda * const   pSqlda,		/*!< sql descriptor area pointer				*/
												 int                    item		/*!< descriptor (= column) index (0, ..., n-1)	*/
												);

/*! \brief read double value from descriptor
    \n     C function */
const double *			infxGetDescDouble		(struct sqlda * const   pSqlda,		/*!< sql descriptor area pointer				*/
												 int                    item		/*!< descriptor (= column) index (0, ..., n-1)	*/
												);

/*! \brief read double value from descriptor
    \n     C function */
const float *			infxGetDescFloat		(struct sqlda * const   pSqlda,		/*!< sql descriptor area pointer				*/
												 int                    item		/*!< descriptor (= column) index (0, ..., n-1)	*/
												);

/*! \brief read decimal value from descriptor
    \n     C function */
const struct decimal *	infxGetDescDecimal		(struct sqlda * const   pSqlda,		/*!< sql descriptor area pointer				*/
												 int                    item		/*!< descriptor (= column) index (0, ..., n-1)	*/
												);

/*! \brief read date value from descriptor
    \n     C function */
const int  *			infxGetDescDate			(struct sqlda * const   pSqlda,		/*!< sql descriptor area pointer				*/
												 int                    item		/*!< descriptor (= column) index (0, ..., n-1)	*/
												);

/*! \brief read datetime value from descriptor
    \n     C function */
const struct dtime *    infxGetDescDateTime		(struct sqlda * const   pSqlda,		/*!< sql descriptor area pointer				*/
												 int                    item		/*!< descriptor (= column) index (0, ..., n-1)	*/
												);

/*! \brief read interval value from descriptor
    \n     C function */
const struct intrvl *	infxGetDescTimeSpan	    (struct sqlda * const   pSqlda,		/*!< sql descriptor area pointer				*/
												 int                    item		/*!< descriptor (= column) index (0, ..., n-1)	*/
												);

/*! \brief set string value to descriptor
    \n     C function */
void					infxSetDescString		(struct sqlda *			pSqlda ,	/*!< sql descriptor area pointer				*/
												 int					item,		/*!< descriptor (= column) index (0, ..., n-1)	*/
												 const char * const		pValue		/*!< value to be set							*/
												);
/*! \brief set byte value to descriptor
\n     C function */
void                    infxSetDescByte         (struct sqlda *         pSqlda,     /*!< sql descriptor area pointer				*/
                                                 int                    item,       /*!< descriptor (= column) index (0, ..., n-1)	*/
                                                 const char * const     pValue,     /*!< value to be set							*/
                                                 unsigned int           length      /*!< length value to be set						*/
                                                );

/*! \brief set int value to descriptor
    \n     C function */
void					infxSetDescInt			(struct sqlda *			pSqlda ,	/*!< sql descriptor area pointer				*/
												 int					item,		/*!< descriptor (= column) index (0, ..., n-1)	*/
												 const int * const		pValue		/*!< value to be set							*/
												);

/*! \brief set 8 byte int value to descriptor
\n     C function */
void                    infxSetDescInt64        (struct sqlda* pSqlda,              /*!< sql descriptor area pointer				*/
                                                 int item,                          /*!< descriptor (= column) index (0, ..., n-1)	*/
                                                 const bigint * const pValue /*!< value to be set							*/
                                                );

/*! \brief set short value to descriptor
    \n     C function */
void					infxSetDescShort		(struct sqlda *			pSqlda ,	/*!< sql descriptor area pointer				*/
												 int					item,		/*!< descriptor (= column) index (0, ..., n-1)	*/
												 const short *	const	pValue		/*!< value to be set							*/
												);

/*! \brief set double value to descriptor
    \n     C function */
void					infxSetDescDouble		(struct sqlda *			pSqlda ,	/*!< sql descriptor area pointer					*/
												 int					item,		/*!< descriptor (= column) index (0, ..., n-1)		*/
												 const double * const	pValue		/*!< value to be set								*/
												);

/*! \brief set float value to descriptor
    \n     C function */
void					infxSetDescFloat		(struct sqlda *			pSqlda ,	/*!< sql descriptor area pointer					*/
												 int					item,		/*!< descriptor (= column) index (0, ..., n-1)		*/
												 const float *	const	pValue		/*!< value to be set								*/
												);

/*! \brief set decimal value to descriptor
    \n     C function */
void					infxSetDescDecimal		(struct sqlda *			pSqlda,		/*!< sql descriptor area pointer				*/
												 int					item,		/*!< descriptor (= column) index (0, ..., n-1)	*/
												 const struct decimal * const
                                                                        pValue		/*!< value to be set							*/
											 	);

/*! \brief set date value to descriptor
    \n     C function */
void					infxSetDescDate			(struct sqlda *			pSqlda,		/*!< sql descriptor area pointer					*/
												 int					item,		/*!< descriptor (= column) index (0, ..., n-1)		*/
												 const int * const		pValue		/*!< value to be set								*/
												);

/*! \brief set datetime value to descriptor
    \n     C function */
void					infxSetDescDateTime		(struct sqlda *			pSqlda,		/*!< sql descriptor area pointer					*/
												 int					item,		/*!< descriptor (= column) index (0, ..., n-1)		*/
												 const struct dtime * const	
                                                                        pValue		/*!< value to be set								*/
												);

/*! \brief set interval value to descriptor
    \n     C function */
void					infxSetDescTimeInterval	(struct sqlda *			pSqlda,		/*!< sql descriptor area pointer					*/
												 int					item,		/*!< descriptor (= column) index (0, ..., n-1)		*/
												 const struct intrvl * const
                                                                        pValue		/*!< value to be set								*/
												);

/*! \brief check if memory allocation for large object through CSDK succeeded
    \n     C function */
int                     infxCheckLocBufferAllocSucceeded( struct sqlda * pSqlda /*!< sql descriptor area pointer					*/
                        );

/*##############################################################################*/
/*#  connection handling                                                        */
/*##############################################################################*/

/* --- enum section ------------------------------------------------------*/

/*! \brief Informix transaction isolation level
    \see   basar::db::sql::ConnectionRef::IsolationLevelEnum */
enum InfxIsoLevelEnum
{
	DIRTYREAD					,	/*!< informix correspondent to ANSI_READUNCOMMITTED									*/
	DIRTYREAD_WITHWARNING		,	/*!< warning for uncommitted or phantom row											*/
	COMITTEDREAD				,	/*!< informix correspondent to ANSI_READCOMMITTED									*/
	COMITTEDREAD_LASTCOMMITTED	,	/*!< return most recently committed version of row, no lock wait					*/
	CURSORSTABILITY				,	/*!< not ANSI: fetched row doesn't change, exclusive lock can't be placed on it		*/
	REPEATABLEREAD					/*!< informix correspondent to ANSI_REPEATABLEREAD									*/
};

/*! \brief Informix lock modes
    \see   basar::db::sql::ConnectionRef::LockModeEnum */
enum InfxLockModeEnum
{
	NOTWAIT ,	/*!< database server ends the operation immediately and returns an error code		*/
	WAIT	,	/*!< database server suspends the process until the lock releases					*/
	WAITSECS	/*!< \brief database server suspends process until lock releases or until waiting 	*/
				/*!<        period ends. If lock remains after waiting period, database server 		*/
				/*!<		ends the operation and returns an error code							*/
};

/* --- function section --------------------------------------------------*/

/*! \brief Informix return code */
enum InfxRetCode	
{
	RETCODE_OK,			/*!< success */
	RETCODE_ERROR		/*!< failure */
};


/*! \brief set Informix environment variable	\n C function */
enum InfxRetCode	infxPutEnv			(char *             env					/*!< environment variable to set: NAME=value */
										);

/*! \brief set Informix environment variable	\n C function */
const char *      	infxGetEnv			(const char * const name				/*!< name of environment variable to get */
										);

/*! \brief connect to database via connect string \n C function */
void				infxConnectDb		(const char * const pConn       ,		/*!< connect string    (e.g. "db\@dbserver")                       */
										 const char * const pConnId     ,		/*!< unique connect ID (i.e. object address)                       */
										 const int          flagTransact		/*!< true : can switch to different connection while a transaction 
																				            is active in current connection.\n
																					 false: if within transaction, SET CONNECTION statement fails, 
																					        returning an error, and transaction in                 
																							current connection continues to be active              */
										);
/*! \brief connect to database via connect string and user \n C function */
void				infxConnectDbUser	(const char * const pConn       ,		/*!< connect string    (e.g. "db\@dbserver")                       */
										 const char * const pConnId     ,		/*!< unique connect ID (i.e. object address)                       */
										 const char * const pUser       ,		/*!< database user                                                 */
										 const char * const pPasswd     ,		/*!< database user's password                                      */
										 const int          flagTransact		/*!< true : can switch to different connection while a transaction 
										 													is active in current connection.\n
										  											 false: if within transaction, SET CONNECTION statement fails, 
																							returning an error, and transaction in                 
																							current connection continues to be active              */
										);
/*! \brief disconnect from database		\n C function */
void				infxDisconnectDb	(const char * const pConnId				/*!< unique connect name (i.e. object address) */
										);	
/*! \brief set connect to current			\n C function */
void				infxSetCurrentDb	(const char * const pConnId				/*!< unique connect name (i.e. object address) */
										);

/*! \brief start transaction				\n C function */
void				infxBeginwork		();								
/*! \brief commit changes in transaction	\n C function */
void				infxCommitwork		();								
/*! \brief undo changes in transaction		\n C function */
void				infxRollbackwork	();								

/*! \brief define degree of concurrency among processes that attempt to access the same rows simultaneously     \n C function */
void				infxSetIsoLevel		(enum InfxIsoLevelEnum eLevel,			/*!< Informix transaction isolation level                  */
										 int                   flagLocks		/*!< true: RETAIN UPDATE LOCKS for SELECT ... FOR UPDATE \n
																				     false: no retain                                      */
										);

/*! \brief define how the database server handles process that tries to access locked row or table    \n C function */
void				infxSetLockMode		(enum InfxLockModeEnum eMode,			/*!< mode for accessing locked rows or tables              */
										 unsigned int          secs				/*!< maximum number of seconds that process waits for lock 
																	     to be released before issuing an error                */
										);

/*! \brief set global fetch buffer size (default: 4kB; maximum depends on csdk version)    \n C function */
void				infxSetFetBufSize	(int size				/*!< new fetch buffer size in bytes	*/
										);

/*! \brief get global fetch buffer size     \n     C function */
int					infxGetFetBufSize	();


/*##############################################################################*/
/*  statement handling                                                          */
/*##############################################################################*/

/* --- function section --------------------------------------------------*/

/*! \brief execute dynamically a single simple SQL non-select statement
    \n     C function */
void	infxExecImm			(const char *         pStmt			/*!< SQL statement to perform (non-select)	*/
							);

/*! \brief  run a previously prepared single simple parameterised statement
    \n     C function */
void	infxExecUsing		(const char *         pStmtId,		/*!< unique statement ID				*/
							 struct sqlda * const pUsing		/*!< using sql descriptor area pointer	*/
							);

/*! \brief run a previously prepared statement
    \n     C function */
void	infxExecInto		(const char *         pStmtId,		/*!< unique statement ID				*/
							 struct sqlda * const pInto			/*!< into sql descriptor area pointer	*/
							);

/*! \brief run a previously prepared parameterised statement
    \n     C function */
void	infxExecIntoUsing	(const char *         pStmtId,		/*!< unique statement ID				*/
							 struct sqlda * const pInto,		/*!< into sql descriptor area pointer	*/
							 struct sqlda * const pUsing		/*!< using sql descriptor area pointer	*/
							);

/*! \brief assemble text of SQL statements at runtime (creating a prepared object) and make it executable
    \n     C function */
void	infxPrepStmt		(const char * pStmtId,		/*!< unique statement ID						*/
							 const char * pStmt			/*!<  SQL statement sto be prepared				*/
							);

/*! \brief release statement resources in application development tool and database server
    \n     C function */
void	infxFreeStmt		(const char * pStmtId		/*!< unique statement ID	*/
							);

/*##############################################################################*/
/*#  cursor handling                                                           #*/
/*##############################################################################*/

/* --- enum section ------------------------------------------------------*/

/*! \brief Informix cursor state */
enum InfxCursorState	
{
	CURSOR_WITHOUT_HOLD,	/*!< cursor declared without hold	*/
	CURSOR_WITH_HOLD		/*!< cursor declared with hold		*/
};

/* --- function section --------------------------------------------------*/

/*! \brief associate cursor with prepared statement
    \n     C function */
void	infxDeclCurs		(const char           * pCursorId,		/*!< unique cursor ID		*/
							 const char           * pStmtId,		/*!< unique statement ID	*/
							 enum InfxCursorState   eState			/*!< cursor state			*/
							);

/*! \brief activate cursor that DECLARE statement created
    \n     C function */
void	infxOpenCurs		(const char *		  pCursorId			/*!< unique cursor ID		*/
							);

/*! \brief activate cursor that DECLARE prepared statement created
    \n     C function */
void	infxOpenCursUsing	(const char *         pCursorId,		/*!< unique cursor ID					*/
							 struct sqlda * const pUsing			/*!< using sql descriptor area pointer	*/
							 );		

/*! \brief move cursor to new row in active set and retrieve row values from memory
    \n     C function */
void	infxFetchCursInto	(const char *         pCursorId,		/*!< unique cursor ID					*/
							 struct sqlda * const pInto				/*!< into sql descriptor area pointer	*/
							);

/*! \brief release resources that database server had allocated to the cursor
    \n     C function */
void	infxCloseCurs		(const char *		  pCursorId			/*!< unique cursor ID					*/
							);

/*! \brief release cursor resources in database server
    \n     C function */
void	infxFreeCurs		(const char *		  pCursorId			/*!< unique cursor ID					*/
							);

/*! \brief send single row into insert buffer
    \n     C function */
void	infxPutCurs			(const char *         pCursorId			/*!< unique cursor ID					*/
							);

/*! \brief send parameterised single row into insert buffer
    \n     C function */
void	infxPutCursUsing	(const char *         pCursorId,		/*!< unique cursor ID					*/
							 struct sqlda * const pUsing			/*!< using sql descriptor area pointer	*/
							);

/*! \brief clear the insert buffer and send the content to the database server
    \n     C function */
void	infxFlushCurs		(const char *		  pCursorId			/*!< unique cursor ID					*/
							);


/*------------------------------------------------------------------------------*/
#ifdef __cplusplus
}
#endif

/*------------------------------------------------------------------------------*/
#endif	/* GUARD */
