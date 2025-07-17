#ifndef CONFIGURATION_GUARD
#define CONFIGURATION_GUARD

//--------------------------------------------------------------------------------------------------//
// include section
//--------------------------------------------------------------------------------------------------//
#include "iconfiguration.h"

//--------------------------------------------------------------------------------------------------//
// namespace section
//--------------------------------------------------------------------------------------------------//

namespace narcotics {
namespace configuration {
	
//--------------------------------------------------------------------------------------------------//

class Configuration : public IConfiguration
{
public:
	Configuration();
	virtual ~Configuration();

	virtual const basar::db::sql::DatabaseInfo getFirstDBInfo();

    virtual void                   setWorkingDirectory(const basar::VarString wdir); //!< for overwriting contributionvoucherdir by passing program arguments (basic loading into cold...)
	virtual const basar::VarString getWorkingDirectory();
	virtual const basar::VarString getSaveDirectory();
	virtual const basar::VarString getVouchersTooMuchDirectory();

	virtual const basar::VarString getArchivingDirectory();
	virtual const basar::VarString getArchivingMetaDataFileName();
	virtual const basar::VarString getArchivingKofaxPath();

	virtual const basar::VarString getBfarmIP();
	virtual const basar::VarString getBfarmUser();

	virtual void init();

private:
	void loadConfiguration();
	void checkConfig();
	void checkConfigValue(basar::ConstString key, const basar::I18nString& val) const;

	basar::I18nString m_db1host;
	basar::I18nString m_db1db;

	basar::I18nString m_workingDirectory;
	basar::I18nString m_saveDirectory;
	basar::I18nString m_vouchersTooMuchDirectory;

	basar::I18nString m_BfarmIP;
	basar::I18nString m_BfarmUser;

	basar::I18nString m_archivingDirectory;
	basar::I18nString m_archivingMetaDataFileName;
	basar::I18nString m_archivingKofaxPath;
};

//--------------------------------------------------------------------------------------------------//

} //namespace configuration
} //namespace narcotics

//--------------------------------------------------------------------------------------------------//

#endif //CONFIGURATION_GUARD

//--------------------------------------------------------------------------------------------------//
