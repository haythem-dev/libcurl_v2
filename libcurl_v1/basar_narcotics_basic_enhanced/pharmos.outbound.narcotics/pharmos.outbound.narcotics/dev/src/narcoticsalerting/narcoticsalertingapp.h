#ifndef NARCOTICSALERTING_APPL_H
#define NARCOTICSALERTING_APPL_H

//--------------------------------------------------------------------------------------------------//
// include section
//--------------------------------------------------------------------------------------------------//
#include "../transfer2bfarm/iconfiguration.h"
#include "../transfer2bfarm/infxconnection.h"
#include "usecasecontroler.h"
#include "../transfer2bfarm/serviceconfiguration.h"

//--------------------------------------------------------------------------------------------------//
// namespace
//--------------------------------------------------------------------------------------------------//
namespace narcotics
{

//--------------------------------------------------------------------------------------------------//
// class 
//--------------------------------------------------------------------------------------------------//
class NarcoticsAlertingApplBase
{
//--------------------------------------------------------------------------------------------------//
// static definition section
//--------------------------------------------------------------------------------------------------//
public:
    
	void init(const basar::VarString contributionvoucherdir, basar::Int32 startDate=0, basar::Int32 endtDate=0);

	boost::shared_ptr<narcotics::configuration::IConfiguration> getConfiguration();

	basar::appl::EventReturnStruct run();

	void shutdown();

	void clear();

public:
	NarcoticsAlertingApplBase();
	virtual ~NarcoticsAlertingApplBase();

private:
		void establishConnections();

		void closeConnections();

		void initLoginManager();

		narcotics::connection::InfxConnection m_firstDBConnection;

		boost::shared_ptr<narcotics::configuration::IConfiguration> m_configuration;

		boost::shared_ptr<narcotics::usecase::UseCaseController> m_crossCheckUC;

		bool m_Crosscheck;

		narcotics::services::ServiceConfiguration m_serviceConfig;
};

//--------------------------------------------------------------------------------------------------//

class NarcoticsAlertingAppl : public basar::cmnutil::Singleton<NarcoticsAlertingApplBase> 
{
	friend class dummy;			//!< prevents pedantic compiler warning

private:
	/*! \brief standard constructor \n \throw no-throw */
	NarcoticsAlertingAppl();
	/*! \brief copy constructor \n \throw no-throw */
	NarcoticsAlertingAppl( const NarcoticsAlertingAppl & r );
	/*! \brief assign operator \n \throw no-throw */
	NarcoticsAlertingAppl & operator = ( const NarcoticsAlertingAppl & r );	
};

//--------------------------------------------------------------------------------------------------//

} //namespace narcotics

//--------------------------------------------------------------------------------------------------//

#endif //NARCOTICSALERTING_APPL_H

//--------------------------------------------------------------------------------------------------//
