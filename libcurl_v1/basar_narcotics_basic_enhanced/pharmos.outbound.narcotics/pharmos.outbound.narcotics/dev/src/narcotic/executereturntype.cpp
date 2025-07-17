//------------------------------------------------------------------------------
/*! \file
 *  \brief  own defined return type for domain module methods having to return a general error code, 
 *  \		the number of affected rows OR own defined error codesfiles include
 *  \author Benjamin Endlicher
 *  \date   30.03.2010
 */
//------------------------------------------------------------------------------

//-------------------------------------------------------------------------------------------------//
// includes
//-------------------------------------------------------------------------------------------------//
#include "commonheader.h"
#include "executereturntype.h"

//-------------------------------------------------------------------------------------------------//
// namespace
//-------------------------------------------------------------------------------------------------//
namespace narcotics
{
//---------------------------------------------------------------------------
//! \brief	std constructor for ExecuteReturnType 
//! \param	no param
//! \throw  no-throw
ExecuteReturnType::ExecuteReturnType()
: m_realExecute( false )
{
// m_resInfo is automatically set to SUCCESS by default because of corresponding basar implementation
	m_statusInfo = ExecuteReturnType::OK_BY_SELECT_BUT_NOTHING_SAVED_BY_MANIPULATION;
}

//---------------------------------------------------------------------------
//! \brief	constructor for ExecuteReturnType 
//! \param	ExecuteResultInfo
//! \throw  no-throw
ExecuteReturnType::ExecuteReturnType( basar::db::aspect::ExecuteResultInfo resInfo )
{
	m_resInfo = resInfo;
	m_statusInfo = executeReturnMapping ( resInfo.getError() ); // returns SUCCESS if no error has occurred
	m_realExecute = true;
}

//---------------------------------------------------------------------------
//! \brief	constructor for ExecuteReturnType 
//! \param	ExecuteResultInfo
//! \throw  no-throw
ExecuteReturnType::ExecuteReturnType( StatusInformation statusInfo )
{
	m_statusInfo  = statusInfo;
	m_realExecute = false;
}

//---------------------------------------------------------------------------
	/*! \brief returns error code of m_resInfo if it has been set
		\n no-throw */
//---------------------------------------------------------------------------
ExecuteReturnType::StatusInformation ExecuteReturnType::getError() const
{
	return m_statusInfo;
}

//---------------------------------------------------------------------------
	/*! \brief returns true if an error has occurred while executing an sql statement
		\n no-throw */
//---------------------------------------------------------------------------
bool ExecuteReturnType::hasError() const
{
	if ( m_realExecute )
		return m_resInfo.hasError();
	else if ( m_statusInfo < 0 )
		return true; // if no real execute has been transmitted, a manual error code with a value smaller than 0 might have 
					 // been transferred
	return false;
}

//---------------------------------------------------------------------------
	/*! \brief returns number of affected rows for a successful execution of a sql statement
		\n no-throw */
//---------------------------------------------------------------------------
basar::db::aspect::ExecuteResultInfo::RowNumber ExecuteReturnType::getAffectedRows() const
{
// only if a sql statement has been executed successfully, its Affected rows can be given out
//	assert ( m_realExecute && !hasError() );
	return m_resInfo.getAffectedRows();
}

//---------------------------------------------------------------------------
	/*! \brief maps return value for executing an sql statement to corresponding status information enum
		\n no-throw */
//---------------------------------------------------------------------------
ExecuteReturnType::StatusInformation ExecuteReturnType::executeReturnMapping ( basar::db::aspect::ExecuteErrorEnum errorEnum )//!< param: errorEnum of execute-statement
{
// errorEnum is definitely matching one of the following expressions if it is called
// ToDo: prevent warning from being shown
	switch ( errorEnum )
	{
		case basar::db::aspect::SUCCESS					:	return 	ExecuteReturnType::OK_BY_SELECT_BUT_NOTHING_SAVED_BY_MANIPULATION;
// the following two code lines both refer to case -1, therefore - as each switch-case-block can only include every case once - the case -1 has to be hardcoded here
		case -1											:	return	ExecuteReturnType::BASAR_EXCEPTION_THROWN;
		case basar::db::aspect::NOT_EXECUTABLE			:	return	ExecuteReturnType::LOCK_ERROR;
		case basar::db::aspect::SQL_ERROR_NONSPECIFIC	:	return	ExecuteReturnType::ATTRIBUTES_UNSET;
		case basar::db::aspect::SQL_ERROR_DUPL_INSERT	:	return	ExecuteReturnType::CURRENT_DATASET_UNEQUAL_DB_DATASET;
		default											:	return	ExecuteReturnType::NO_MAPPING_POSSIBLE;
	}
}

} //namespace narcotics