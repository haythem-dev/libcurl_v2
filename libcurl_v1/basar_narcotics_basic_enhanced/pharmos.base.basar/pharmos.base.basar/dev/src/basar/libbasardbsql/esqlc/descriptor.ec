/*------------------------------------------------------------------------------*/
/*! \file
 *  \brief  ESQL/C descriptor handling (wrapper for sqlda/system descriptor-area)
 *  \author Michael Eichenlaub
 *  \date   30.08.2005
 */
/*------------------------------------------------------------------------------*/

/*! \cond x  */	
EXEC SQL include sqltypes;
EXEC SQL include locator;
EXEC SQL include int8;
/*! \endcond */

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <assert.h>

#include "esqlc.h"

extern int bigintcvifx_int8(const ifx_int8_t*, bigint*);

/*------------------------------------------------------------------------------*/
extern InfxCustomTypeSize customTypeSize;								/*!< global variable is initialized in file resultsetinfx.cpp */

/*------------------------------------------------------------------------------*/
static const char * const VALUE_NULLPTR			  = "pointer null";		/*!< signals null pointer 							*/

static const char * const VALUE_INDICATOR_NULL    = "value null";		/*!< indicator variable signals null value 			*/
static const char * const VALUE_INDICATOR_NOTNULL = "value not null";	/*!< indicator variable signals not null value 		*/
static const char * const VALUE_INDICATOR_INVALID = "value invalid";	/*!< indicator variable signals value not defined	*/

/*------------------------------------------------------------------------------*/
/*! \brief map Informix SQL types to simplified ones
    \n     no DB server traffic
    \return simplified Informix SQL type
    \throw  no-throw */
static enum InfxSqlTypeEnum getSimpleType(int type		/*!< column data type (see sqltypes.h) */
                                         )
{
	switch (type)
	{
	case SQLSMINT:
		return TYPE_SHORT;
		
	case SQLINT:
	case SQLSERIAL:
		return TYPE_INT;
		
	case SQLSMFLOAT:
		return TYPE_FLOAT;
		
	case SQLFLOAT:
		return TYPE_DOUBLE;

	case SQLDECIMAL:
	case SQLMONEY: 
		return TYPE_DECIMAL;
		
	case SQLCHAR:
	case SQLVCHAR:
	case SQLLVARCHAR:
	
	case CCHARTYPE:
	case CSTRINGTYPE:	
		return TYPE_STRING;
		
	case SQLDATE:
		return TYPE_DATE;
		
	case SQLDTIME:	
		return TYPE_DATETIME;
		
	case SQLINTERVAL:
		return TYPE_INTERVAL;
		
	case SQLUNKNOWN:
		return TYPE_UNKNOWN;
	
	case SQLTEXT:
		return TYPE_STRING;

	case SQLBYTES:
		return TYPE_BYTE;

	case SQLINFXBIGINT:
	case SQLBIGSERIAL:
	case SQLINT8:
	case SQLSERIAL8:
		return TYPE_INT8;

	default:
		break;
	}
	
	return TYPE_INVALID;
}

/*------------------------------------------------------------------------------*/
/*!        no DB server traffic
    \return bigint representation of ifx_int8_t value
    \throw no-throw */
bigint convint8tobigint(const ifx_int8_t* pInt8)
{
	bigint value = 0;

	bigintcvifx_int8(pInt8, &value );

	return value;
}

/*------------------------------------------------------------------------------*/
/*!        no DB server traffic
    \throw no-throw */
void infxFreeDesc (struct sqlda * const pSqlda, enum InfxFreeLocBufferEnum freeLocBuffer)	
{
	int i;
	ifx_loc_t* loc = NULL;

	if (pSqlda != 0)
	{
		for (i = 0; i < pSqlda->sqld; ++i) 
		{
			/*free simple BLOB locator structure and buffer*/
			if (CLOCATORTYPE == pSqlda->sqlvar[i].sqltype)
			{
				loc = (ifx_loc_t*)pSqlda->sqlvar[i].sqldata;
				if ( FREE_LOC_BUFFER == freeLocBuffer && 0 < loc->loc_bufsize )
				{
					free(loc->loc_buffer);
				}
				free(loc);
				loc = NULL;
			}
			free(pSqlda->sqlvar + i);
		}
		
		free(pSqlda);
	}
}

/*------------------------------------------------------------------------------*/
/*!        no DB server traffic
    \throw no-throw */
void infxSqlFreeMem(struct sqlda* const pSqlda, enum InfxFreeLocBufferEnum freeLocBuffer )
{
	int i;
	ifx_loc_t* loc = NULL;

	if (0 != pSqlda)
	{
		for (i = 0; i < pSqlda->sqld; ++i) 
		{
			/*free simple BLOB locator structure and buffer*/
			if (CLOCATORTYPE == pSqlda->sqlvar[i].sqltype)
			{
				loc = (ifx_loc_t*)pSqlda->sqlvar[i].sqldata;
				if ( FREE_LOC_BUFFER == freeLocBuffer && 0 < loc->loc_bufsize )
				{
					SqlFreeMem(loc->loc_buffer, LOC_BUFFER_FREE);
					loc->loc_buffer = NULL;
					loc->loc_bufsize = 0;
				}
				free(loc);
				loc = NULL;
			}
		}
	}
	SqlFreeMem(pSqlda, SQLDA_FREE);
}

/*------------------------------------------------------------------------------*/
/*!        no DB server traffic
    \throw no-throw */
void infxSetDescAlloc (struct sqlda * const pSqlda )
{
	int i;
	ifx_loc_t* loc = NULL;

	if ( 0 != pSqlda )
	{
		for (i = 0; i < pSqlda->sqld; ++i)
		{
			if (CLOCATORTYPE == pSqlda->sqlvar[i].sqltype)
			{
				loc = (ifx_loc_t*)pSqlda->sqlvar[i].sqldata;
				loc->loc_mflags = LOC_ALLOC;
			}
		}
	}
}

/*------------------------------------------------------------------------------*/
/*!        no DB server traffic
    \return pointer to sql descriptor area with input data (parameters in prepared statement)
    \throw no-throw */
struct sqlda * infxDescribeStmtIn
(
	/*! \cond x */	EXEC SQL BEGIN DECLARE SECTION;	/*! \endcond */	
	/*! \cond x */	PARAMETER						/*! \endcond */		const char * pStmtId
	/*! \cond x */	EXEC SQL END DECLARE SECTION;	/*! \endcond */
)
{
	struct sqlda * pSqlda = 0;
	
	EXEC SQL describe input :pStmtId into pSqlda;
	
	return pSqlda;
}

/*------------------------------------------------------------------------------*/
/*!        no DB server traffic
    \return pointer to sql descriptor area with output data (selected columns)
    \throw no-throw */
struct sqlda * infxDescribeStmtOut
(
	/*! \cond x */	EXEC SQL BEGIN DECLARE SECTION;	/*! \endcond */	
	/*! \cond x */	PARAMETER						/*! \endcond */		const char * pStmtId
	/*! \cond x */	EXEC SQL END DECLARE SECTION;	/*! \endcond */
)
{
	struct sqlda * pSqlda = 0;
	
	EXEC SQL describe output :pStmtId into pSqlda;
	
	return pSqlda;
}

/*------------------------------------------------------------------------------*/
/*!        no DB server traffic
    \throw no-throw */
void infxFreeImplicitDesc (struct sqlda * const pSqlda, enum InfxFreeLocBufferEnum freeLocBuffer)	
{
/*! \ingroup DIFF_WIN_UNIX */ 
#ifdef _WIN32
	infxSqlFreeMem(pSqlda, freeLocBuffer);
#else
	infxFreeDesc(pSqlda, freeLocBuffer);
#endif
}

/*------------------------------------------------------------------------------*/
/*!         no DB server traffic
    \return number of columns
    \throw  no-throw */
int infxGetDescCount (const struct sqlda * const pSqlda)
{
	if (0 == pSqlda)
		return 0;
		
	return pSqlda->sqld;
}

/*------------------------------------------------------------------------------*/
/*!         no DB server traffic
    \return Informix SQL column length
    \throw  no-throw */
int infxGetDescColLen (const struct sqlda * const pSqlda,
					   int                        item )
{
	if (0 == pSqlda)
		return 0;
	
	if ( CLOCATORTYPE == pSqlda->sqlvar[item].sqltype )
	{
		return ((ifx_loc_t*)pSqlda->sqlvar[item].sqldata)->loc_size;
	}

	return pSqlda->sqlvar[item].sqllen;
}

/*------------------------------------------------------------------------------*/
/*!         no DB server traffic
    \return Informix SQL column name
    \throw  no-throw */
const char * infxGetDescColName (const struct sqlda * const pSqlda,
							     int                        item  )
{
	if (0 == pSqlda)
		return VALUE_NULLPTR;
		
	if (0 == pSqlda->sqlvar[item].sqlname)
		return VALUE_NULLPTR;
		
	return pSqlda->sqlvar[item].sqlname;
}

/*------------------------------------------------------------------------------*/
#define COLDATE_BUFLEN	256 //!< size of data buffer

/*!         no DB server traffic
    \return Informix SQL column data
    \throw  no-throw */
const char * infxGetDescColData (const struct sqlda * const pSqlda,
							     int                        item  )
{
	static char        data[COLDATE_BUFLEN];
	const char * const pData  = pSqlda->sqlvar[item].sqldata;
	
	if (0 == pData)
		return VALUE_NULLPTR;
		
	switch (pSqlda->sqlvar[item].sqltype)
	{
	case SQLSMINT:
		sprintf(data, "%d", *(short *)pData);
		return data;

	case SQLINT:
	case SQLSERIAL:
		sprintf(data, "%d", *(int *)pData);
		return data;
		
	case SQLSMFLOAT:
		sprintf(data, "%f", *(float *)pData);
		return data;
		
	case SQLFLOAT:
		sprintf(data, "%f", *(double *)pData);
		return data;

	case SQLDECIMAL:
	case SQLMONEY: 
		dectoasc((struct decimal *)(pData), data, COLDATE_BUFLEN - 1, -1);
		return data;
		
	case SQLCHAR:
	case SQLVCHAR:
	case SQLLVARCHAR:
		return pData;
		
	case SQLDATE:
		rdatestr(*(int *)(pData), data);
		return data;
	
	case SQLINTERVAL:
		intoasc((struct intrvl *)(pData), data);
		return data;
		
	case SQLDTIME:	
		dttoasc((struct dtime *)(pData), data);
		return data;

	case SQLTEXT:
		return ((ifx_loc_t*)pData)->loc_buffer;

	case CLOCATORTYPE:
		if (SQLTEXT == ((ifx_loc_t*)pData)->loc_type)
		{
			return ((ifx_loc_t*)pData)->loc_buffer;
		}
		else if ( SQLBYTES == ((ifx_loc_t*)pData)->loc_type)
		{
			sprintf(data, "%s", "Binary data");
			return data;
		}

	case SQLINFXBIGINT:
	case SQLBIGSERIAL:
		sprintf(data, "%lld", *(long long int*)pData);
		return data;

	case SQLINT8:
	case SQLSERIAL8:
		sprintf(data, "%lld", (long long int)convint8tobigint( (ifx_int8_t*)pData ));
		return data;

	default:
		return pData;
	}
}

/*------------------------------------------------------------------------------*/
/*!         no DB server traffic
    \return simplified Informix SQL column type
    \throw  no-throw */
enum InfxSqlTypeEnum infxGetDescSimpleType (const struct sqlda * const pSqlda,
											int                        item  )
{
	if (0 == pSqlda)
		return TYPE_INVALID;
	
	/* Simple BLOB special handling */
	if ( CLOCATORTYPE == pSqlda->sqlvar[item].sqltype )
	{
		return getSimpleType(((ifx_loc_t*)pSqlda->sqlvar[item].sqldata)->loc_type);
	}

	return getSimpleType(pSqlda->sqlvar[item].sqltype);
}


/*------------------------------------------------------------------------------*/
/*!         no DB server traffic
    \return Informix SQL column type (see sqltypes.h)
    \throw  no-throw */
short infxGetDescType (const struct sqlda * const pSqlda,
					   int                        item  )
{
	return pSqlda->sqlvar[item].sqltype;
}


/*------------------------------------------------------------------------------*/
/*!         no DB server traffic
    \return Informix SQL column type name
    \throw  no-throw */
const char * infxGetDescTypeName (const struct sqlda * const pSqlda,
								  int                        item  )
{
	short type;
	
	if (0 == pSqlda)
		return VALUE_NULLPTR;
	
	type = pSqlda->sqlvar[item].sqltype;
	
	switch (type)
	{
	case CCHARTYPE:
		return "C char";
		
	case CSTRINGTYPE:
		return "C string";
	
	case CLOCATORTYPE:
		if ( SQLTEXT == ((ifx_loc_t*)pSqlda->sqlvar[item].sqldata)->loc_type )
		{
			return "string (TEXT)";
		}
		else if (SQLBYTES == ((ifx_loc_t*)pSqlda->sqlvar[item].sqldata)->loc_type)
		{
			return "binary (BYTE)";
		}
		

	default:
		return rtypname(type);
	}
}

/*------------------------------------------------------------------------------*/
/*!         no DB server traffic
    \return Informix SQL column indicator
    \throw  no-throw */
const char * infxGetDescIndicator (const struct sqlda * const pSqlda,
							       int                        item  )
{
	if (0 == pSqlda->sqlvar[item].sqlind)
		return VALUE_NULLPTR;
		
	switch (*(pSqlda->sqlvar[item].sqlind))
	{
	case -1:
		return VALUE_INDICATOR_NULL;
		
	case 0:
		return VALUE_INDICATOR_NOTNULL;
		
	default:
		return VALUE_INDICATOR_INVALID;
	}
}

/*------------------------------------------------------------------------------*/
/*!         no DB server traffic
    \return Informix SQL row length (aligned size)
    \throw  no-throw */
int infxPrepDescOut (struct sqlda * const pSqlda,
                     int                  flagRaw)
{
	int					   colSize;			/* single column size			*/
	mintptr				   offset  = 0;		/* position within row buffer	*/
	int                    i;				/* column index					*/
	struct sqlvar_struct * pDesc;			/* pointer to descriptor		*/
	ifx_loc_t *temp_loc;					/* pointer for loc_t structures */

	if (0 == pSqlda)
		return 0;
	
    for (i = 0, pDesc = pSqlda->sqlvar;
		 i < pSqlda->sqld; 
	     ++i, ++pDesc)
	{
		switch (pDesc->sqltype)
		{
		case SQLDATE:
			colSize = customTypeSize[TYPE_CUSTOM_DATE];
			break;
			
		case SQLDTIME:
			colSize = customTypeSize[TYPE_CUSTOM_DATETIME];
			break;
		
		case SQLINTERVAL:
			colSize = customTypeSize[TYPE_CUSTOM_INTERVAL];
			break;
			
		case SQLDECIMAL:
			colSize = customTypeSize[TYPE_CUSTOM_DECIMAL];
			break;
			
		case SQLCHAR:
			pDesc->sqltype = flagRaw ? CCHARTYPE : CSTRINGTYPE;
			pDesc->sqllen += 1;
			colSize        = rtypmsize(pDesc->sqltype, pDesc->sqllen);
			break;

		case SQLVCHAR:
		case SQLLVARCHAR:
			pDesc->sqltype = CSTRINGTYPE;
			pDesc->sqllen += 1;
			colSize        = rtypmsize(pDesc->sqltype, pDesc->sqllen);
			break;

		case SQLTEXT:
		case SQLBYTES:
			temp_loc = (ifx_loc_t *)malloc(sizeof(ifx_loc_t)); /* alloc BLOB locator structure */
			byfill( (char*)temp_loc, sizeof(ifx_loc_t), 0); /* init memory with 0 */
			temp_loc->loc_loctype = LOCMEMORY; /*store in memory*/
			temp_loc->loc_bufsize = -1; /* tell CSDK to handle buffer allocation */
			temp_loc->loc_oflags = 0; /* clear flag */
			temp_loc->loc_mflags = 0; /* clear flag */
			temp_loc->loc_type = pDesc->sqltype; /* copy type (needed by inserts) */
			pDesc->sqltype = CLOCATORTYPE;
			pDesc->sqldata = (char *)temp_loc;
			break;

		default:
			colSize = rtypmsize(pDesc->sqltype, pDesc->sqllen);
			break;
		}
		
		if ( CLOCATORTYPE != pDesc->sqltype )
		{
			offset         = rtypalign(offset, pDesc->sqltype);		/* get aligned boundary for column data	*/
			pDesc->sqldata = (char *)offset;						/* assign column data buffer offset (must be adjusted by row buffer base address)	*/
			offset        += colSize;								/* increment buffer position	*/
		}
	}
	
	return (int)offset;
}

/*------------------------------------------------------------------------------*/
/*!         no DB server traffic
    \throw  no-throw */
void infxPrepDescIn (struct sqlda * const pSqlda,
                    int                  flagRaw)
{
	int                    i;				/* column index				*/
	struct sqlvar_struct * pDesc;			/* pointer to descriptor	*/
	
	if (0 == pSqlda)
		return;
	
	for (i = 0, pDesc = pSqlda->sqlvar;
		i < pSqlda->sqld; 
		++i, ++pDesc)
	{
		switch (pDesc->sqltype)
		{
		case SQLCHAR:
			pDesc->sqltype = flagRaw ? CCHARTYPE : CSTRINGTYPE;
			break;
			
		case SQLVCHAR:
		case SQLLVARCHAR:
			pDesc->sqltype = CSTRINGTYPE;
			break;

		break;

		default:
			break;
		}
	}
}

/*------------------------------------------------------------------------------*/
/*!         no DB server traffic
    \throw  no-throw */
void infxAdjustDesc	(struct sqlda * const pSqlda,
					 char  * const        pRow,
					 short * const        pIndicator)
{
	int                    i;
	struct sqlvar_struct * pDesc;
	short                * pInd;
	
	if (0 == pSqlda)
		return;
	
	for (i = 0, pDesc = pSqlda->sqlvar, pInd = pIndicator; 
	     i < pSqlda->sqld; 
	     ++i, ++pDesc, ++pInd)
	{
		/* increment previously stored offset by start of row buffer */
		if ( CLOCATORTYPE != pDesc->sqltype && SQLUDTFIXED != pDesc->sqltype )
		{
			pDesc->sqldata = (mintptr)pDesc->sqldata + pRow;
		}
		
		/* assign column data indicator address */
		pDesc->sqlind = pInd;
	}
}

/*------------------------------------------------------------------------------*/
/*!         no DB server traffic
    \throw  no-throw */
void infxSetBufferToItemDesc	(struct sqlda * const pSqlda,
								char  * const        pRow,
								short * const        pIndicator,
								int item)
{
	/*select part of sqlda according to item*/
	struct sqlvar_struct * pDesc = pSqlda->sqlvar + item;
	

	/* increment previously stored offset by start of row buffer and do so only once */
	pDesc->sqldata = (mintptr)pDesc->sqldata + pRow;
		
	/* assign column data indicator address */
	pDesc->sqlind = pIndicator;

}

/*------------------------------------------------------------------------------*/
/*!         no DB server traffic
    \return column value
    \throw  no-throw */
const char * infxGetDescString (struct sqlda * const pSqlda,
							    int                  item  )
{
	struct sqlvar_struct * pDesc = pSqlda->sqlvar + item;
	ifx_loc_t* loc;
	static const char empty = '\0';

	/* check for blobs */
	if (CLOCATORTYPE == pDesc->sqltype)
	{
		loc = (ifx_loc_t*)pDesc->sqldata;

		/* honor null value or empty content*/
		/* as loc->loc_buffer will be allocated by ifx csdk there may or may not be memory */
		if (-1 == *(pDesc->sqlind) || 0 == loc->loc_bufsize)
		{
			return &empty;
		}

/*
		if (SQLTEXT == loc->loc_type)
		{
			loc->loc_buffer[loc->loc_bufsize] = '\0';
		}
*/
		return loc->loc_buffer;
	}
	else
	{
		if (-1 == *(pDesc->sqlind))
		{
			pDesc->sqldata[0] = empty;
		}

		/* check empty string if SQL null value */
		assert( (-1 != *(pDesc->sqlind)) || (empty == *(char * const)(pDesc->sqldata)) );
	
		return pDesc->sqldata;
	}
}

/*------------------------------------------------------------------------------*/
/*!         no DB server traffic
    \return column value
    \throw  no-throw */
const int * infxGetDescInt (struct sqlda * const pSqlda,
						    int                  item  )
{
	struct sqlvar_struct * pDesc = pSqlda->sqlvar + item;
	
	if (-1 == *(pDesc->sqlind))	/* null value */
		*(int * const)(pDesc->sqldata) = 0;

	return (const int * const)pDesc->sqldata;
}

/*------------------------------------------------------------------------------*/
/*!         no DB server traffic
    \return column value
    \throw  no-throw */
const bigint* infxGetDescInt64(struct sqlda * const pSqlda, int item)
{
	struct sqlvar_struct * pDesc = pSqlda->sqlvar + item;
	static bigint value; /* variable with non-stack address */

	if ( -1 == *(pDesc->sqlind)) /* nullvalue -> return 0*/
	{
		*(int * const)(pDesc->sqldata) = 0;
		return (const bigint* const)(pDesc->sqldata);
	}

	/*find out which type of 8 byte integer it is - test that it has to be a 8 byte int happens before call to this*/
	if (SQLINT8 == pDesc->sqltype || SQLSERIAL8 == pDesc->sqltype)
	{
		value = convint8tobigint((ifx_int8_t*)pDesc->sqldata);
		return (const bigint* const)&value;
	}
	else /*SQLBIGINT == pDesc->sqltype || SQLBIGSERIAL == pDesc->sqltype*/
	{
		return (const bigint* const)(pDesc->sqldata);
	}
}

/*------------------------------------------------------------------------------*/
/*!         no DB server traffic
    \return column value
    \throw  no-throw */
const short * infxGetDescShort (struct sqlda * const         pSqlda,
							    int                          item  )
{
	struct sqlvar_struct * pDesc = pSqlda->sqlvar + item;
	
	if (-1 == *(pDesc->sqlind))	/* null value */
		*(short * const)(pDesc->sqldata) = 0;

	return (const short * const)pDesc->sqldata;
}

/*------------------------------------------------------------------------------*/
/*!         no DB server traffic
    \return column value
    \throw  no-throw */
const double * infxGetDescDouble (struct sqlda * const pSqlda,
							      int                  item  )
{
	struct sqlvar_struct * pDesc = pSqlda->sqlvar + item;
	
	if (-1 == *(pDesc->sqlind))		/* null value */
		*(double * const)(pDesc->sqldata) = 0.0;

	return (const double * const)pDesc->sqldata;
}

/*------------------------------------------------------------------------------*/
/*!         no DB server traffic
    \return column value
    \throw  no-throw */
const float * infxGetDescFloat (struct sqlda * const pSqlda,
							    int                  item  )
{
	struct sqlvar_struct * pDesc = pSqlda->sqlvar + item;
	
	if (-1 == *(pDesc->sqlind))		/* null value */
		*(float * const)(pDesc->sqldata) = 0.0;

	return (const float * const)pDesc->sqldata;
}
	
/*------------------------------------------------------------------------------*/
/*!         no DB server traffic
    \return column value
    \throw  no-throw */
const struct decimal * infxGetDescDecimal (struct sqlda * const pSqlda,
								  int                  item  )
{
	struct sqlvar_struct * pDesc = pSqlda->sqlvar + item;
	
	if (-1 == *(pDesc->sqlind))		/* null value */
		deccvdbl(0.0, (struct decimal *)pDesc->sqldata);

	return (const struct decimal * const)pDesc->sqldata;
}

/*------------------------------------------------------------------------------*/
/*!         no DB server traffic
*   \throw  no-throw
*   \return const int pointer to date representation inside resultset
*/
const int * infxGetDescDate  (struct sqlda * const pSqlda,
							   int                  item  )
{
	struct sqlvar_struct * pDesc = pSqlda->sqlvar + item;

	if (-1 == *(pDesc->sqlind))		/* null value */
	{
		/* NULL = 9999-12-31 */
		short mdy[3] = { 12, 31, 9999 };		
		rmdyjul(mdy, (int *)pDesc->sqldata);
	}
	
	return (const int * const)pDesc->sqldata;
}

/*------------------------------------------------------------------------------*/
/*!         no DB server traffic
*   \throw  no-throw
*   \return const dtime pointer to datetime representation inside resultset
*/
const struct dtime * infxGetDescDateTime (struct sqlda * const pSqlda,
									 int                  item  )
{
	struct sqlvar_struct * pDesc = pSqlda->sqlvar + item;

	if (-1 == *(pDesc->sqlind))		/* null value */
	{
		const char * const DTNULL = "9999-12-31 23:59:59.999";
		
		((struct dtime *)pDesc->sqldata)->dt_qual = TU_DTENCODE(TU_YEAR, TU_F3);
		dtcvasc((char *)DTNULL, (struct dtime *)pDesc->sqldata);
	}
	
	return (const struct dtime * const)pDesc->sqldata;
}

/*------------------------------------------------------------------------------*/
/*!         no DB server traffic
*   \throw  no-throw
*   \return const intrvl pointer to interval representation inside resultset
*/
const struct intrvl * infxGetDescTimeSpan (struct sqlda * const pSqlda,
									 int                  item  )
{
	struct sqlvar_struct * pDesc = pSqlda->sqlvar + item;

	if (-1 == *(pDesc->sqlind))		/* null value */
	{
		const char * const DTNULL = "999999999 23:59:59.999";
		
		((struct intrvl *)pDesc->sqldata)->in_qual = TU_IENCODE(9, TU_DAY, TU_F3);
		incvasc((char *)DTNULL, (struct intrvl *)pDesc->sqldata);
	}
	
	return (const struct intrvl * const)pDesc->sqldata;
}

/*------------------------------------------------------------------------------*/
/*!         no DB server traffic
    \throw  no-throw */
void infxSetDescString (struct sqlda *     pSqlda,
						int                item  ,
						const char * const pValue)
{
	static char            SPACE[2]; /* static: providing non-stack address */
	int                    length = (int)strlen(pValue);
	struct sqlvar_struct * pDesc = pSqlda->sqlvar + item;
	ifx_loc_t*             loc;

    /* static buffer must be init'ed everytime, 
       is not const --> see sqlvar_struct.sqldata */
    strcpy(SPACE, " ");    
	
	if (SQLUNKNOWN == pDesc->sqltype)
	{
		pDesc->sqltype = CSTRINGTYPE;
	}

	/*handle TEXT BLOB (test if it's actually a TEXT happend already)*/
	if (CLOCATORTYPE == pDesc->sqltype)
	{
		loc = (ifx_loc_t*)pDesc->sqldata;
		loc->loc_buffer = (char *)pValue;
		/* length +1: also save terminating '\0' */
		loc->loc_size = length + 1;
		loc->loc_bufsize = length + 1;
	}
	else
	{
		if (0 == length)
		{
			/* single space inserted instead of empty string  */
			pDesc->sqllen  = rtypmsize(pDesc->sqltype, (mint)strlen(SPACE)); 
			pDesc->sqldata = SPACE;
		}
		else
		{ 
			pDesc->sqllen  = rtypmsize(pDesc->sqltype, length); 		
			pDesc->sqldata = (char *)pValue;
		}
	}
}

/*------------------------------------------------------------------------------*/
/*!         no DB server traffic
    \throw  no-throw */
void infxSetDescByte(struct sqlda * pSqlda, int item, const char * const pValue, unsigned int length)
{
	struct sqlvar_struct * pDesc = pSqlda->sqlvar + item;
	ifx_loc_t* loc;

	if (CLOCATORTYPE == pDesc->sqltype)
	{
		loc = (ifx_loc_t*)pDesc->sqldata;
		loc->loc_buffer = (char*)pValue;
		loc->loc_size =  length;
		loc->loc_bufsize = length;
	}
}

/*------------------------------------------------------------------------------*/
/*!         no DB server traffic
    \throw  no-throw */
void infxSetDescInt (struct sqlda *    pSqlda,
					 int               item  ,
					 const int * const pValue)
{
	struct sqlvar_struct * pDesc = pSqlda->sqlvar + item;

	if (SQLUNKNOWN == pDesc->sqltype)
	{
		pDesc->sqltype = SQLINT;
		pDesc->sqllen  = sizeof(int);
	}
	
	pDesc->sqldata = (char *)pValue;
}

/*------------------------------------------------------------------------------*/
/*!         no DB server traffic
    \throw  no-throw */
void infxSetDescInt64(struct sqlda* pSqlda, int item, const bigint * const pValue)
{
	struct sqlvar_struct * pDesc = pSqlda->sqlvar + item;
	static ifx_int8_t value; /* non-stack address */
	char buffer[21]; /* conversion buffer */

	if ( SQLINT8 == pDesc->sqltype || SQLSERIAL8 == pDesc->sqltype)
	{
		pDesc->sqllen = sizeof(ifx_int8_t);

		sprintf(buffer, "%lld", (long long int)*pValue);

		ifx_int8cvasc(buffer, (mint)strlen(buffer), &value);

		pDesc->sqldata = (char*)&value;
	}
	else  /* SQLBIGINT == pDesc->sqltype || SQLBIGSERIAL == pDesc->sqltype */
	{
		pDesc->sqllen = sizeof(bigint);

		pDesc->sqldata = (char*)pValue;
	}

}

/*------------------------------------------------------------------------------*/
/*!         no DB server traffic
    \throw  no-throw */
void infxSetDescShort (struct sqlda *      pSqlda ,
					   int                 item  ,
					   const short * const pValue)
{
	struct sqlvar_struct * pDesc = pSqlda->sqlvar + item;

	if (SQLUNKNOWN == pDesc->sqltype)
	{
		pDesc->sqltype = SQLSMINT;
		pDesc->sqllen  = sizeof(short);
	}
	
	pDesc->sqldata = (char *)pValue;
}

/*------------------------------------------------------------------------------*/
/*!         no DB server traffic
    \throw  no-throw */
void infxSetDescDouble (struct sqlda *       pSqlda ,
						int                  item  ,
						const double * const pValue)
{
	struct sqlvar_struct * pDesc = pSqlda->sqlvar + item;

	if (SQLUNKNOWN == pDesc->sqltype)
	{
		pDesc->sqltype = SQLFLOAT;
		pDesc->sqllen  = sizeof(double);
	}
	
	pDesc->sqldata = (char *)pValue;
}

/*------------------------------------------------------------------------------*/
/*!         no DB server traffic
    \throw  no-throw */
void infxSetDescFloat (struct sqlda *      pSqlda,
					   int                 item  ,
					   const float * const pValue)
{
	struct sqlvar_struct * pDesc = pSqlda->sqlvar + item;

	if (SQLUNKNOWN == pDesc->sqltype)
	{
		pDesc->sqltype = SQLSMFLOAT;
		pDesc->sqllen  = sizeof(float);
	}
	
	pDesc->sqldata = (char *)pValue;
}

/*------------------------------------------------------------------------------*/
/*!         no DB server traffic
    \throw  no-throw */
void infxSetDescDecimal (struct sqlda *               pSqlda,
						 int                          item  ,
						 const struct decimal * const pValue)
{
	struct sqlvar_struct * pDesc = pSqlda->sqlvar + item;

	if (SQLUNKNOWN == pDesc->sqltype)
	{
		pDesc->sqltype = SQLDECIMAL;
		pDesc->sqllen  = sizeof(struct decimal);
	}
	
	memcpy(pDesc->sqldata, pValue, sizeof(struct decimal));
}

/*------------------------------------------------------------------------------*/
/*!         no DB server traffic
    \throw  no-throw */
void infxSetDescDate (struct sqlda *    pSqlda,
					  int			    item  ,
					  const int * const pValue)
{
	struct sqlvar_struct * pDesc = pSqlda->sqlvar + item;
	
	if (SQLUNKNOWN == pDesc->sqltype)
	{
		pDesc->sqltype = SQLDATE;
		pDesc->sqllen  = sizeof(int);
	}
	
	memcpy(pDesc->sqldata, pValue, sizeof(int));
}

/*------------------------------------------------------------------------------*/
/*!         no DB server traffic
    \throw  no-throw */
void infxSetDescDateTime (struct sqlda *            pSqlda,
						 int                        item  ,
						 const struct dtime * const pValue)
{
	struct sqlvar_struct * pDesc = pSqlda->sqlvar + item;
	
	if (SQLUNKNOWN == pDesc->sqltype)
	{
		pDesc->sqltype = SQLDTIME;
		pDesc->sqllen  = sizeof(struct dtime);
	}
	
	memcpy(pDesc->sqldata, pValue, sizeof(struct dtime));

}

/*------------------------------------------------------------------------------*/
/*!         no DB server traffic
    \throw  no-throw */
void infxSetDescTimeInterval (struct sqlda *             pSqlda,
						     int                         item  ,
						     const struct intrvl * const pValue)
{
	struct sqlvar_struct * pDesc = pSqlda->sqlvar + item;
	
	if (SQLUNKNOWN == pDesc->sqltype)
	{
		pDesc->sqltype = SQLINTERVAL;
		pDesc->sqllen  = sizeof(struct intrvl);
	}
	
	memcpy(pDesc->sqldata, pValue, sizeof(struct intrvl));

}

/*------------------------------------------------------------------------------*/
/*!         no DB server traffic
    \return int indicating success (1: success, 0: failure)
    \throw  no-throw */
int infxCheckLocBufferAllocSucceeded( struct sqlda * pSqlda )
{
	int                    i;				/* column index				*/
	struct sqlvar_struct * pDesc;			/* pointer to descriptor	*/
	ifx_loc_t* loc;
	int ret = 1;

	if (0 == pSqlda)
		return ret;
	
	for (i = 0, pDesc = pSqlda->sqlvar;
		i < pSqlda->sqld; 
		++i, ++pDesc)
		{
			if (CLOCATORTYPE == pDesc->sqltype)
			{
				loc = (ifx_loc_t*)pDesc->sqldata;
				if (0 != loc->loc_status)
				{
					ret = 0;
					break;
				}
			}
		}

	return ret;
}
