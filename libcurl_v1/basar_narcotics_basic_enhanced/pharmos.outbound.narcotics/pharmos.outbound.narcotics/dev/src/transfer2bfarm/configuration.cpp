//--------------------------------------------------------------------------------------------------//
// include section
//--------------------------------------------------------------------------------------------------//
#include "commonheader.h"
#include "configuration.h"
#include "definitions.h"
#include "exconfiguration.h"
#include "loggerpool.h"

//--------------------------------------------------------------------------------------------------//
// namespace section
//--------------------------------------------------------------------------------------------------//
namespace narcotics {
namespace configuration {

//--------------------------------------------------------------------------------------------------//

Configuration::Configuration()
{
}

//--------------------------------------------------------------------------------------------------//

Configuration::~Configuration()
{
}

//--------------------------------------------------------------------------------------------------//

const basar::VarString Configuration::getArchivingDirectory()
{
	return m_archivingDirectory;
}

//--------------------------------------------------------------------------------------------------//
const basar::VarString Configuration::getArchivingMetaDataFileName()
{
	return m_archivingMetaDataFileName;
}

//--------------------------------------------------------------------------------------------------//
const basar::VarString Configuration::getArchivingKofaxPath()
{
	return m_archivingKofaxPath;
}

//--------------------------------------------------------------------------------------------------//
const basar::db::sql::DatabaseInfo Configuration::getFirstDBInfo()
{
	basar::db::sql::DatabaseInfo info;
	info.setInfx(m_db1host,m_db1db);
	
	return info;
}

//--------------------------------------------------------------------------------------------------//
void Configuration::setWorkingDirectory(const basar::VarString wdir)
{
	m_workingDirectory = wdir;
}

//--------------------------------------------------------------------------------------------------//
const basar::VarString Configuration::getWorkingDirectory()
{
	return m_workingDirectory;
}

//--------------------------------------------------------------------------------------------------//
const basar::VarString Configuration::getSaveDirectory()
{
	return m_saveDirectory;
}

//--------------------------------------------------------------------------------------------------//
const basar::VarString Configuration::getVouchersTooMuchDirectory()
{
	return m_vouchersTooMuchDirectory;
}

//--------------------------------------------------------------------------------------------------//
void Configuration::loadConfiguration()
{
	basar::login::Manager::getInstance().getParaValue(constants::UADM_DB1_HOST,					m_db1host);
	basar::login::Manager::getInstance().getParaValue(constants::UADM_DB1_DB,					m_db1db);

	basar::login::Manager::getInstance().getParaValue(constants::UADM_CONTRIBUTIONVOUCHERDIR,	m_workingDirectory);
	basar::login::Manager::getInstance().getParaValue(constants::UADM_SAVEDIR,					m_saveDirectory);
	basar::login::Manager::getInstance().getParaValue(constants::UADM_VOUCHERSTOOMUCHDIR,		m_vouchersTooMuchDirectory);

	basar::login::Manager::getInstance().getParaValue(constants::UADM_BFARM_IP,					m_BfarmIP);
	basar::login::Manager::getInstance().getParaValue(constants::UADM_BFARM_USER, 				m_BfarmUser);

	basar::login::Manager::getInstance().getParaValue(constants::UADM_ARCHIVINGDIR,				m_archivingDirectory);
	basar::login::Manager::getInstance().getParaValue(constants::UADM_METADATAFILENAME, 		m_archivingMetaDataFileName);
	basar::login::Manager::getInstance().getParaValue(constants::UADM_KOFAXPATH, 				m_archivingKofaxPath);
}

//--------------------------------------------------------------------------------------------------//

void Configuration::checkConfigValue(basar::ConstString key, const basar::I18nString& val) const
{
	if (val.empty())
	{
		basar::VarString msg;
		msg.format("config %s not set", key);
		BLOG_ERROR(LoggerPool::getLoggerTransferBfarm(), msg);
		throw narcotics::exceptions::ExConfiguration(basar::ExceptInfo("Configuration::checkConfigValue()",msg,__FILE__,__LINE__));
	}
}

//--------------------------------------------------------------------------------------------------//
const basar::VarString Configuration::getBfarmIP()
{
	return m_BfarmIP;
}

//--------------------------------------------------------------------------------------------------//
const basar::VarString Configuration::getBfarmUser()
{
	return m_BfarmUser;
}

//--------------------------------------------------------------------------------------------------//
void Configuration::checkConfig()
{
	checkConfigValue(constants::UADM_DB1_HOST,					m_db1host);
	checkConfigValue(constants::UADM_DB1_DB,					m_db1db);

	checkConfigValue(constants::UADM_CONTRIBUTIONVOUCHERDIR,	m_workingDirectory);
	checkConfigValue(constants::UADM_SAVEDIR,					m_saveDirectory);
	checkConfigValue(constants::UADM_VOUCHERSTOOMUCHDIR,		m_vouchersTooMuchDirectory);

	checkConfigValue(constants::UADM_BFARM_IP,					m_BfarmIP);
	checkConfigValue(constants::UADM_BFARM_USER, 				m_BfarmUser);

	checkConfigValue(constants::UADM_ARCHIVINGDIR,				m_archivingDirectory);
	checkConfigValue(constants::UADM_METADATAFILENAME, 			m_archivingMetaDataFileName);
	checkConfigValue(constants::UADM_KOFAXPATH, 				m_archivingKofaxPath);
}

//--------------------------------------------------------------------------------------------------//
void Configuration::init()
{
    loadConfiguration();	
	checkConfig();
}

//--------------------------------------------------------------------------------------------------//

} //namespace configuration
} //namespace narcotics

//--------------------------------------------------------------------------------------------------//

