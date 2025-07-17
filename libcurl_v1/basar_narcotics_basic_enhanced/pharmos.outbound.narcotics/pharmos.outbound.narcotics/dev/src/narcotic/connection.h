//----------------------------------------------------------------------------
/*! \file
 *  \brief  connection class for narcotics
 *  \author Thomas Hörath
 *  \date   22.02.2006
 */
//----------------------------------------------------------------------------
#ifndef GUARD_CONNECTION_NARCOTICS_H
#define GUARD_CONNECTION_NARCOTICS_H

//----------------------------------------------------------------------------
//includes
//----------------------------------------------------------------------------
#include <libbasardbsql.h>

//----------------------------------------------------------------------------
//namespaces
//----------------------------------------------------------------------------
namespace narcotics 
{

//---------------------------------------------------------------------------
/*! \brief connection-class
	\n throws BasarExceptions
	\n final class */
class Connection
{

public:	
	/*! \brief std-destructor
		\n no-throw */
	virtual ~Connection();
	
	/*! \brief std-constructor
		\n no-throw */
	Connection();

	void initConnection();

	/*! \brief start main connection
		\n no-throw */
	void connect();
	
	/*! \brief disconnect open connections
		\n BasarException */
	void disconnect();

	/*! \brief get the current connection
		\n no-throw */
	basar::db::aspect::ConnectionRef getCurrentConnection();

private:
	basar::db::sql::DatabaseInfo					m_DbInfo;		//!< info to active connection
	basar::db::aspect::ConnectionRef				m_ActivConn;	//!< reference to active connection

	/*! \brief set connection infos to m_DbInfo
		\n no-throw */
	void setDbInfo();
};

} //namespaces narcotics

#endif //GUARD_CONNECTION_NARCOTICS_H