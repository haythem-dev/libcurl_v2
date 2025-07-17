/*------------------------------------------------------------------------------*/
/*! \file
 *  \brief  ESQL/C cursor handling
 *  \author Michael Eichenlaub
 *  \date   18.08.2005
 */
/*------------------------------------------------------------------------------*/

#include "profiling.h"

#include "esqlc.h"

/*------------------------------------------------------------------------------*/
/*!        no DB server traffic
    \throw no-throw */
void infxDeclCurs		
(
	/*! \cond x */	EXEC SQL BEGIN DECLARE SECTION;	/*! \endcond */	
	/*! \cond x */	PARAMETER						/*! \endcond */		const char         * pCursorId,	
	/*! \cond x */	PARAMETER						/*! \endcond */		const char         * pStmtId  ,	
	/*! \cond x */	EXEC SQL END DECLARE SECTION;	/*! \endcond */
																		enum InfxCursorState eState
)
{
	if (CURSOR_WITH_HOLD == eState)
		EXEC SQL declare :pCursorId cursor with hold for :pStmtId;
	else
		EXEC SQL declare :pCursorId cursor for :pStmtId;
}

/*------------------------------------------------------------------------------*/
/*! \throw    no-throw
 */ 
void infxOpenCurs		
(
	/*! \cond x */	EXEC SQL BEGIN DECLARE SECTION;	/*! \endcond */	
	/*! \cond x */	PARAMETER						/*! \endcond */		const char * pCursorId		
	/*! \cond x */	EXEC SQL END DECLARE SECTION;	/*! \endcond */
)
{
	EXEC SQL open :pCursorId;
	
#ifdef LIBBASARSQLINF_PROFILING
	resetCsdkFetchTotalTime();
#endif
}

/*------------------------------------------------------------------------------*/
/*! \throw    no-throw
 */ 
void infxOpenCursUsing 
(						 
	/*! \cond x */	EXEC SQL BEGIN DECLARE SECTION;	/*! \endcond */	
	/*! \cond x */	PARAMETER						/*! \endcond */		const char *         pCursorId,
	/*! \cond x */	EXEC SQL END DECLARE SECTION;	/*! \endcond */
																		struct sqlda * const pUsing
)
{
	EXEC SQL open :pCursorId using descriptor pUsing;
	
#ifdef LIBBASARSQLINF_PROFILING
	resetCsdkFetchTotalTime();
#endif
}

/*------------------------------------------------------------------------------*/
/*! \throw    no-throw
 */ 
void infxFetchCursInto
(						 
	/*! \cond x */	EXEC SQL BEGIN DECLARE SECTION;	/*! \endcond */	
	/*! \cond x */	PARAMETER						/*! \endcond */		const char *         pCursorId,
	/*! \cond x */	EXEC SQL END DECLARE SECTION;	/*! \endcond */
																		struct sqlda * const pInto
)
{
#ifdef LIBBASARSQLINF_PROFILING
	setStartTime();
#endif

	EXEC SQL fetch :pCursorId using descriptor pInto;

#ifdef LIBBASARSQLINF_PROFILING
	setStopTime();
	sumCsdkFetchTotalTime();
#endif
}

/*------------------------------------------------------------------------------*/
/*! \throw    no-throw
 */ 
void infxCloseCurs		
(
	/*! \cond x */	EXEC SQL BEGIN DECLARE SECTION;	/*! \endcond */	
	/*! \cond x */	PARAMETER						/*! \endcond */		const char * pCursorId	
	/*! \cond x */	EXEC SQL END DECLARE SECTION;	/*! \endcond */
)
{
	EXEC SQL close :pCursorId;
}

/*------------------------------------------------------------------------------*/
/*!        no DB server traffic
    \throw no-throw */
void infxFreeCurs		
(
	/*! \cond x */	EXEC SQL BEGIN DECLARE SECTION;	/*! \endcond */	
	/*! \cond x */	PARAMETER						/*! \endcond */		const char * pCursorId	
	/*! \cond x */	EXEC SQL END DECLARE SECTION;	/*! \endcond */
)
{
	EXEC SQL free :pCursorId;
}

/*------------------------------------------------------------------------------*/
/*! \throw    no-throw
 */ 
void infxPutCurs
(						 
	/*! \cond x */	EXEC SQL BEGIN DECLARE SECTION;	/*! \endcond */	
	/*! \cond x */	PARAMETER						/*! \endcond */		const char * pCursorId
	/*! \cond x */	EXEC SQL END DECLARE SECTION;	/*! \endcond */
)
{
	EXEC SQL put :pCursorId;
}

/*------------------------------------------------------------------------------*/
/*! \throw    no-throw
 */ 
void infxPutCursUsing
(						 
	/*! \cond x */	EXEC SQL BEGIN DECLARE SECTION;	/*! \endcond */	
	/*! \cond x */	PARAMETER						/*! \endcond */		const char *         pCursorId,
	/*! \cond x */	EXEC SQL END DECLARE SECTION;	/*! \endcond */
																		struct sqlda * const pUsing
)
{
	EXEC SQL put :pCursorId using descriptor pUsing;
}

/*------------------------------------------------------------------------------*/
/*! \throw    no-throw
 */ 
void infxFlushCurs
(
	/*! \cond x */	EXEC SQL BEGIN DECLARE SECTION;	/*! \endcond */	
	/*! \cond x */	PARAMETER						/*! \endcond */		const char * pCursorId	
	/*! \cond x */	EXEC SQL END DECLARE SECTION;	/*! \endcond */
)
{
	EXEC SQL flush :pCursorId;
}

/*------------------------------------------------------------------------------*/

