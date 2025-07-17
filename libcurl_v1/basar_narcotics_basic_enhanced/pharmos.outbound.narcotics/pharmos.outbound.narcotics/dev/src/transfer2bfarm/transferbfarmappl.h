#ifndef TRANSFERBFARM_APPL_H
#define TRANSFERBFARM_APPL_H

//--------------------------------------------------------------------------------------------------//
// include section
//--------------------------------------------------------------------------------------------------//
#include "iconfiguration.h"
#include "infxconnection.h"
#include "usecasecontroler.h"
#include "serviceconfiguration.h"

//--------------------------------------------------------------------------------------------------//
// namespace
//--------------------------------------------------------------------------------------------------//
namespace narcotics
{

//--------------------------------------------------------------------------------------------------//
// class 
//--------------------------------------------------------------------------------------------------//
class TransferBfarmApplBase
{
//--------------------------------------------------------------------------------------------------//
// static definition section
//--------------------------------------------------------------------------------------------------//
public:
    
	void init(const bool crosscheck, const bool transfer, const bool archive, const basar::VarString contributionvoucherdir);

	boost::shared_ptr<narcotics::configuration::IConfiguration> getConfiguration();

	basar::appl::EventReturnStruct run();

	void shutdown();

	void clear();

public:
	TransferBfarmApplBase();
	virtual ~TransferBfarmApplBase();

private:
		void establishConnections();

		void closeConnections();

		void initLoginManager();

		narcotics::connection::InfxConnection m_firstDBConnection;

		boost::shared_ptr<narcotics::configuration::IConfiguration> m_configuration;

		boost::shared_ptr<narcotics::usecase::UseCaseController> m_crossCheckUC;
		boost::shared_ptr<narcotics::usecase::UseCaseController> m_transferToBfarmUC;	
		boost::shared_ptr<narcotics::usecase::UseCaseController> m_archiveUC;

		bool m_Crosscheck;
		bool m_Transfer;
		bool m_Archive;

		narcotics::services::ServiceConfiguration m_serviceConfig;
};

//--------------------------------------------------------------------------------------------------//

class TransferBfarmAppl : public basar::cmnutil::Singleton<TransferBfarmApplBase> 
{
	friend class dummy;			//!< prevents pedantic compiler warning

private:
	/*! \brief standard constructor \n \throw no-throw */
	TransferBfarmAppl();
	/*! \brief copy constructor \n \throw no-throw */
	TransferBfarmAppl( const TransferBfarmAppl & r );
	/*! \brief assign operator \n \throw no-throw */
	TransferBfarmAppl & operator = ( const TransferBfarmAppl & r );	
};

//--------------------------------------------------------------------------------------------------//

} //namespace narcotics

//--------------------------------------------------------------------------------------------------//

#endif //TRANSFERBFARM_APPL_H

//--------------------------------------------------------------------------------------------------//
