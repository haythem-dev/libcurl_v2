/*------------------------------------------------------------------------------*/
/*! \file
 *  \brief  ESQL/C profiling
 *  \author Michael Eichenlaub
 *  \date   06.09.2005
 */
/*------------------------------------------------------------------------------*/

#ifndef GUARD_PROFILING_H
#define GUARD_PROFILING_H

/*------------------------------------------------------------------------------*/
#ifdef __cplusplus
extern "C" 
{
#endif

/*------------------------------------------------------------------------------*/
/*! \ingroup DIFF_WIN_UNIX */ 
#ifdef _WIN32

#include <time.h>

/*------------------------------------------------------------------------------*/

extern clock_t				csdkFetchTotalTime;		/*!< summarized fetch time within Informix client SDK for one prepared statement	*/
extern clock_t				startTime;				/*!< start measure time																*/
extern clock_t				stopTime;				/*!< stop measure time																*/

/*------------------------------------------------------------------------------*/
#else	/* _WIN32 */

/*------------------------------------------------------------------------------*/

extern struct timespec		csdkFetchTotalTime;		/*!< summarized fetch time within Informix client SDK for one prepared statement	*/
extern struct timespec		startTime;				/*!< start measure time																*/
extern struct timespec		stopTime;				/*!< stop measure time																*/

/*------------------------------------------------------------------------------*/
#endif	/* _WIN32 */

/*------------------------------------------------------------------------------*/

void				resetCsdkFetchTotalTime	();		/*!< initialize fetch total time			\n no-throw	*/
void				sumCsdkFetchTotalTime	();		/*!< add measure time to fetch total time	\n no-throw	*/
int					getCsdkFetchTotalTime	();		/*!< get fetch total time in ms				\n no-throw	*/

void				setStartTime			();		/*!< save start measure time				\n no-throw	*/
void				setStopTime				();		/*!< save stop measure time					\n no-throw	*/

/*------------------------------------------------------------------------------*/
#ifdef __cplusplus
}
#endif

/*------------------------------------------------------------------------------*/

#endif	/* GUARD */

