//------------------------------------------------------------------------------
/*! \file
 *  \brief  own defined return type for domain module methods having to return a general error code, 
 *  \		the number of affected rows OR own defined error codesfiles include
 *  \author Benjamin Endlicher
 *  \date   30.03.2010
 */
//------------------------------------------------------------------------------

#ifndef GUARD_NARCOTICS_EXECUTERETURNTYPE_H
#define GUARD_NARCOTICS_EXECUTERETURNTYPE_H

//------------------------------------------------------------------------------
// include section
//------------------------------------------------------------------------------
#include "libbasardbaspect_definitions.h"

//------------------------------------------------------------------------------
// namespace section
//------------------------------------------------------------------------------
namespace narcotics	
{

//------------------------------------------------------------------------------
class ExecuteReturnType
{

public:

	enum StatusInformation
	{
		OK_BY_SELECT_BUT_NOTHING_SAVED_BY_MANIPULATION	= 0,
		BASAR_EXCEPTION_THROWN							= -1,
		LOCK_ERROR										= -2,
		ATTRIBUTES_UNSET								= -3,
		CURRENT_DATASET_UNEQUAL_DB_DATASET				= -4,
		NEGATIVE_QTY_IN_STOCK							= -5,
		INVALID_VALUE									= -6,
		OUTDATED_DATE									= -7,
		ARTICLE_LOCKED									= -99,
		NO_MAPPING_POSSIBLE								= -100 // needed if an informix error code cannot be
															   // mapped to a manually defined one
	};

	/*! \brief std constructor 
		\n	   param: no param
 		\n     no-throw */
	ExecuteReturnType();

	/*! \brief constructor 
		\n	   param: ExecuteResultInfo
 		\n     no-throw */
	ExecuteReturnType( basar::db::aspect::ExecuteResultInfo resInfo );

	/*! \brief constructor 
		\n	   param: StatusInformation
 		\n     no-throw */
	ExecuteReturnType( ExecuteReturnType::StatusInformation statusInfo );
	
	/*! \brief returns error code of m_resInfo if it has been set
		\n no-throw */
	StatusInformation getError() const;

	/*! \brief returns true if an error has occurred while executing an sql statement
		\n no-throw */
	bool hasError() const;

	/*! \brief returns number of affected rows for a successful execution of a sql statement
		\n no-throw */
	basar::db::aspect::ExecuteResultInfo::RowNumber getAffectedRows() const;

private:
	basar::db::aspect::ExecuteResultInfo								m_resInfo;
	StatusInformation													m_statusInfo;

	bool																m_realExecute;	//	 needed to decide whether a real execute 
																						//	 has been fired or a manually defined error 
																						//	 code has to be transferred
	/*! \brief maps return value for executing an sql statement to corresponding status information enum
		\n no-throw */
	StatusInformation	executeReturnMapping ( basar::db::aspect::ExecuteErrorEnum errorEnum );	//!< param: errorEnum of execute-statement

}; //class declaration

} //namespace narcotics

#endif //GUARD_NARCOTICS_EXECUTERETURNTYPE_H