//----------------------------------------------------------------------------
/*! \file
 *  \brief  SQL diagnostics area's info implementation class
 *  \author Michael Eichenlaub
 *  \date   11.08.2005
 */
//----------------------------------------------------------------------------

#include "sqldiagnosticsinfx.h"

#include <esqlc.h>

//----------------------------------------------------------------------------
namespace basar	{
namespace db	{
namespace sql	{

//----------------------------------------------------------------------------
//! \return	string with important descriptor data
//! \throw  no-throw
VarString SqlDiagnosticsInfx::getDescDump(::sqlda * m_pDesc)
{
	if (0 == m_pDesc)
		return "descriptor not valid";

	VarString dump;
	VarString help;
	Int32     items = infxGetDescCount(m_pDesc);


	help.format("        descriptor 0x%08x   items: %d", 
				m_pDesc,
				items);
	dump = help;

	for (Int32 i = 0; i < items; ++i)
	{
		help.format("\n           [%3d]: length    %-d"
					"\n                : type      %s"
			        "\n                : indicator %s"
					"\n                : data      <%s>"
					"\n                : name      <%s>",
					i,
					infxGetDescColLen   (m_pDesc, i),
					infxGetDescTypeName (m_pDesc, i),
					infxGetDescIndicator(m_pDesc, i),
					infxGetDescColData  (m_pDesc, i),
					infxGetDescColName  (m_pDesc, i));
		dump += help;
	}

	return dump;
}

//----------------------------------------------------------------------------
//! \return	shared pointer to sql diagnostics implementation object
//! \throw  no-throw
SqlDiagSharedPtr SqlDiagnosticsInfx::create ()
{
	return SqlDiagSharedPtr(new SqlDiagnosticsInfx);
}

//----------------------------------------------------------------------------
//! \throw  no-throw
//!
SqlDiagnosticsInfx::SqlDiagnosticsInfx()
{
	init();
}

//----------------------------------------------------------------------------
//! \throw  no-throw
//!
SqlDiagnosticsInfx::~SqlDiagnosticsInfx()
{
}

//----------------------------------------------------------------------------
//! \throw  no-throw
//!
void SqlDiagnosticsInfx::init()
{
	m_ItDiagInfo = m_CollDiagInfo.begin();

	m_CountExcepts = infxGetCountDiagExcepts(); 

	// is there any exception?
	if (0 >= m_CountExcepts)
	{
		assert(0);
		return;
	}	

	char			dbServer[LEN_SERVER_NAME ] = { 0 };
	char			connId  [LEN_CONNECT_NAME] = { 0 };

	char			msgTxt  [LEN_MESSAGE_TEXT] = { 0 };
	char			sqlState[LEN_SQLSTATE    ] = { 0 };
	Int32			sqlCode                    = 0;

	DiagnosticInfo	diagInfo;

	// walk through exceptions
	for (Int32 i = 1; i <= m_CountExcepts; ++i)
	{
		if (1 == i)
		{
			// first sql exception
			infxGetFirstDiagExcept (dbServer,
									connId  ,
									msgTxt  ,
									sqlState,
									& sqlCode);

			m_DbServer			= dbServer;
			m_ConnId			= connId;
		}
		else
		{
			infxGetNextDiagExcept  (i        ,
									msgTxt   ,
									sqlState ,
									& sqlCode);
		}

		diagInfo.msgTxt   = msgTxt  ;
		diagInfo.sqlState = sqlState;
		diagInfo.sqlCode  = sqlCode ;
	
		m_CollDiagInfo.push_back(diagInfo);
	}
}

//----------------------------------------------------------------------------
}	// namespace sql
}	// namespace db
}	// namespace basar
