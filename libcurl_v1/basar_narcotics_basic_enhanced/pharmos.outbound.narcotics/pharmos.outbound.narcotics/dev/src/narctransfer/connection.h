//----------------------------------------------------------------------------
/*! \file
 *  \brief  connection class for narctransfer
 *  \author 
 *  \date   
 */
//----------------------------------------------------------------------------

#ifndef GUARD_CONNECTION_H
#define GUARD_CONNECTION_H

//----------------------------------------------------------------------------
//namespaces
//----------------------------------------------------------------------------
namespace basar {
	namespace db {
		namespace aspect {
			class ConnectionRef;
		}
	}
}

//----------------------------------------------------------------------------
namespace narcotics
{

//----------------------------------------------------------------------------
using basar::db::aspect::ConnectionRef;

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

	/*! \brief start connecting
		\n no-throw */
	void connect();
	
	/*! \brief disconnect open connection
		\n BasarException */
	void disconnect();

	/*! \brief get the current connection
		\n no-throw */
	const ConnectionRef getCurrentConnection() const;

private:
	ConnectionRef::DatabaseInfo		m_DbInfo;		//!< info to active connection
	ConnectionRef					m_ActivConn;	//!< reference to active connection

	/*! \brief set connection infos to m_DbInfo
		\n no-throw */
	void setDbInfo();
};

} //namespace narcotics

#endif	//GUARD_CONNECTION_H