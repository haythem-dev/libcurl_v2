#ifndef INFXCONNECTION_GUARD
#define INFXCONNECTION_GUARD

//--------------------------------------------------------------------------------------------------//
// include section
//--------------------------------------------------------------------------------------------------//

//--------------------------------------------------------------------------------------------------//
// namespace section
//--------------------------------------------------------------------------------------------------//
namespace narcotics
{
namespace connection
{

//--------------------------------------------------------------------------------------------------//
class InfxConnection
{
public:
	InfxConnection();
	virtual ~InfxConnection();

	void initConnection( const basar::db::sql::DatabaseInfo& dbInfo);

	/*! \brief start main connection
		\n no-throw */
	void connect();
	
	/*! \brief disconnect open connections
		\n BasarException */
	void disconnect();

	/*! \brief get the current connection
		\n no-throw */
	basar::db::aspect::ConnectionRef getCurrentConnection();

	bool isOpen();

private:
	basar::db::sql::DatabaseInfo		m_DbInfo;		//!< info to active connection
	basar::db::aspect::ConnectionRef	m_ActivConn;	//!< reference to active connection
};

} //namespace connection
} //namespace narcotics

#endif //INFXCONNECTION_GUARD
