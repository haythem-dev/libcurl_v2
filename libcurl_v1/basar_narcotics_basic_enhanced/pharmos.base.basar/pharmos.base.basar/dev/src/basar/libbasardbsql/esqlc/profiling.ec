/*------------------------------------------------------------------------------*/
/*! \file
 *  \brief  ESQL/C profiling
 *  \author Michael Eichenlaub
 *  \date   06.09.2005
 */
/*------------------------------------------------------------------------------*/


#include "profiling.h"

#include <time.h>

#ifndef _WIN32
#include <sys/time.h>
#endif

/*------------------------------------------------------------------------------*/
/*! \ingroup DIFF_WIN_UNIX */ 
#ifdef _WIN32

/*------------------------------------------------------------------------------*/
clock_t csdkFetchTotalTime  = 0;	/*!< init summarized fetch time 				*/

clock_t startTime			= 0;	/*!< init temporary start time for measuring	*/
clock_t stopTime			= 0;	/*!< init temporary stop time for measuring		*/

/*------------------------------------------------------------------------------*/
/*! \throw no-throw
 */
void resetCsdkFetchTotalTime()
{
	csdkFetchTotalTime = 0;
}

/*------------------------------------------------------------------------------*/
/*! \throw no-throw
 */
void sumCsdkFetchTotalTime	()
{
	csdkFetchTotalTime += (stopTime - startTime);
}

/*------------------------------------------------------------------------------*/
/*! \throw no-throw
 */
void setStartTime()
{
	startTime = clock();	
}

/*------------------------------------------------------------------------------*/
/*! \throw no-throw
 */
void setStopTime()
{
	stopTime = clock();	
}

/*------------------------------------------------------------------------------*/
/*! \return fetch total time in ms
    \throw no-throw */
int getCsdkFetchTotalTime()
{
	return csdkFetchTotalTime * 1000 / CLOCKS_PER_SEC;
}

/*------------------------------------------------------------------------------*/
#else	/* _WIN32 */

/*------------------------------------------------------------------------------*/
struct timespec	csdkFetchTotalTime	= { 0, 0 };	/*!< init summarized fetch time					*/

struct timespec	startTime			= { 0, 0 };	/*!< init temporary start time for measuring	*/
struct timespec	stopTime			= { 0, 0 };	/*!< init temporary stop time for measuring		*/

/*------------------------------------------------------------------------------*/
/*! \throw no-throw
 */
void resetCsdkFetchTotalTime()
{
	csdkFetchTotalTime.tv_sec  = 0;
	csdkFetchTotalTime.tv_nsec = 0;
}

/*------------------------------------------------------------------------------*/
/*! \throw no-throw
 */
void sumCsdkFetchTotalTime	()
{
	csdkFetchTotalTime.tv_sec  += (stopTime.tv_sec  - startTime.tv_sec );
	csdkFetchTotalTime.tv_nsec += (stopTime.tv_nsec - startTime.tv_nsec);
	
	if (1000000000 <= csdkFetchTotalTime.tv_nsec)
	{
		++csdkFetchTotalTime.tv_sec;  
		csdkFetchTotalTime.tv_nsec -= 1000000000;
	}
	else
	{
		if (0 > csdkFetchTotalTime.tv_nsec)
		{
			--csdkFetchTotalTime.tv_sec;  
			csdkFetchTotalTime.tv_nsec = 1000000000 + csdkFetchTotalTime.tv_nsec;
		}
	}
}

/*------------------------------------------------------------------------------*/
/*! \throw no-throw
 */
void setStartTime()
{
	clock_gettime(CLOCK_REALTIME, &startTime);
}

/*------------------------------------------------------------------------------*/
/*! \throw no-throw
 */
void setStopTime()
{
	clock_gettime(CLOCK_REALTIME, &stopTime);
}

/*------------------------------------------------------------------------------*/
/*! \return fetch total time in ms
    \throw no-throw */
int getCsdkFetchTotalTime()
{
	return (csdkFetchTotalTime.tv_sec * 1000) + (csdkFetchTotalTime.tv_nsec / 1000000);
}

/*------------------------------------------------------------------------------*/
#endif	/* _WIN32 */

