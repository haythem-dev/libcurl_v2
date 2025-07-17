/** $Id$
 *
 * @file sqldiagnosticscics.cpp

 * @brief ...
 * @author Anke Klink

 * @date 2007
 */
//----------------------------------------------------------------------------

#include "sqldiagnosticscics.h"
#include "cicsdefinitions.h"

//----------------------------------------------------------------------------
namespace basar
{
namespace db
{
namespace sql
{

// ------------------------------------------------------------------------------------------------
SqlDiagSharedPtr SqlDiagnosticsCics::create()
{
	return SqlDiagSharedPtr(new SqlDiagnosticsCics());
}

// ------------------------------------------------------------------------------------------------
SqlDiagnosticsCics::~SqlDiagnosticsCics	()
{
}

// ------------------------------------------------------------------------------------------------
SqlDiagnosticsCics::SqlDiagnosticsCics	( )
{
}

// ------------------------------------------------------------------------------------------------
void SqlDiagnosticsCics::init()
{
    CicsErrorInfo error = CicsDefinitions::getLastCicsError();

    if (error.m_errorCode != 0)
    {
		DiagnosticInfo	diagInfo;
		diagInfo.msgTxt   = error.m_errorMessage  ;
		diagInfo.sqlState = "ERROR";
		diagInfo.sqlCode  = error.m_errorCode ;
	
		m_CollDiagInfo.push_back(diagInfo);
		m_CountExcepts++;
    }
}

// ------------------------------------------------------------------------------------------------

}	// namespace
}
}

// ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~  ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~
