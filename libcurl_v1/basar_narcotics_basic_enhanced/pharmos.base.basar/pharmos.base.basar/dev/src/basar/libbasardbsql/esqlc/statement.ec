/*------------------------------------------------------------------------------*/
/*! \file
 *  \brief  ESQL/C statement handling
 *  \author Michael Eichenlaub
 *  \date   16.08.2005
 */
/*------------------------------------------------------------------------------*/

#include "esqlc.h"

/*------------------------------------------------------------------------------*/
/*!           performs functions of the PREPARE, EXECUTE, and FREE statements
    \throw    no-throw */
void infxExecImm	
(
	/*! \cond x */	EXEC SQL BEGIN DECLARE SECTION;	/*! \endcond */	
	/*! \cond x */	PARAMETER						/*! \endcond */		const char * pStmt
	/*! \cond x */	EXEC SQL END DECLARE SECTION;	/*! \endcond */
)
{
	EXEC SQL execute immediate :pStmt;
}

/*------------------------------------------------------------------------------*/
/*! \throw    no-throw
 */ 
void infxExecUsing 
(
	/*! \cond x */	EXEC SQL BEGIN DECLARE SECTION;	/*! \endcond */	
	/*! \cond x */	PARAMETER						/*! \endcond */		const char *         pStmtId,
	/*! \cond x */	EXEC SQL END DECLARE SECTION;	/*! \endcond */
																		struct sqlda * const pUsing  
)
{
	if (0 == pUsing)
		EXEC SQL execute :pStmtId;
	else
		EXEC SQL execute :pStmtId using descriptor pUsing;
}

/*------------------------------------------------------------------------------*/
/*! \throw    no-throw
 */ 
void infxExecInto
(
	/*! \cond x */	EXEC SQL BEGIN DECLARE SECTION;	/*! \endcond */	
	/*! \cond x */	PARAMETER						/*! \endcond */		const char *         pStmtId,
	/*! \cond x */	EXEC SQL END DECLARE SECTION;	/*! \endcond */
																		struct sqlda * const pInto
)
{
	EXEC SQL execute :pStmtId into descriptor pInto;
}


/*------------------------------------------------------------------------------*/
/*! \throw    no-throw
 */ 
void infxExecIntoUsing
(
	/*! \cond x */	EXEC SQL BEGIN DECLARE SECTION;	/*! \endcond */	
	/*! \cond x */	PARAMETER						/*! \endcond */		const char *         pStmtId,
	/*! \cond x */	EXEC SQL END DECLARE SECTION;	/*! \endcond */
																		struct sqlda * const pInto,
																		struct sqlda * const pUsing
)				    
{
	EXEC SQL execute :pStmtId into descriptor pInto using descriptor pUsing;
}

/*------------------------------------------------------------------------------*/
/*! \throw    no-throw
 */ 
void infxPrepStmt 
(
	/*! \cond x */	EXEC SQL BEGIN DECLARE SECTION;	/*! \endcond */	
	/*! \cond x */	PARAMETER						/*! \endcond */		const char * pStmtId,
	/*! \cond x */	PARAMETER						/*! \endcond */		const char * pStmt
	/*! \cond x */	EXEC SQL END DECLARE SECTION;	/*! \endcond */
)
{
	EXEC SQL prepare :pStmtId from :pStmt;
}

/*------------------------------------------------------------------------------*/
/*! \throw    no-throw
 */ 
void infxFreeStmt
(
	/*! \cond x */	EXEC SQL BEGIN DECLARE SECTION;	/*! \endcond */	
	/*! \cond x */	PARAMETER						/*! \endcond */		const char * pStmtId		
	/*! \cond x */	EXEC SQL END DECLARE SECTION;	/*! \endcond */
)
{
	EXEC SQL free :pStmtId;
}

