#ifndef SERVICECONFIGURATION_GUARD
#define SERVICECONFIGURATION_GUARD

#include "infxconnection.h"

//------------------------------------------------------------------------------
//namespaces
//------------------------------------------------------------------------------
namespace narcotics
{
namespace services
{

//------------------------------------------------------------------------------
class ServiceConfiguration
{
public:
	
	narcotics::connection::InfxConnection firstDBConnection;
	bool			 transfer;
	basar::VarString workingDir;
	basar::VarString backupDir;
	basar::VarString vouchersTooMuchDir;
	basar::VarString archivingDir;
	basar::VarString archivingMetaDataFileName;
	basar::VarString archivingKofaxPath;
	basar::VarString BFARM_User;
	basar::VarString BFARM_IP;
};

} //namespace services
} //namespace narcotics

#endif //SERVICECONFIGURATION_GUARD