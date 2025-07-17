#ifndef ICONFIGURATION_GUARD
#define ICONFIGURATION_GUARD

//--------------------------------------------------------------------------------------------------//
// namespace section
//--------------------------------------------------------------------------------------------------//

namespace narcotics {
namespace configuration {

//--------------------------------------------------------------------------------------------------//
class IConfiguration
{
public:
	virtual const basar::db::sql::DatabaseInfo getFirstDBInfo()=0;

	virtual void                   setWorkingDirectory(const basar::VarString wdir)=0; //!< for overwriting contributionvoucherdir by passing program arguments (basic loading into cold...)
	virtual const basar::VarString getWorkingDirectory()=0;
	virtual const basar::VarString getSaveDirectory()=0;
	virtual const basar::VarString getVouchersTooMuchDirectory()=0;

	virtual const basar::VarString getArchivingDirectory()=0;
	virtual const basar::VarString getArchivingMetaDataFileName()=0;
	virtual const basar::VarString getArchivingKofaxPath()=0;

	virtual const basar::VarString getBfarmIP()=0;
	virtual const basar::VarString getBfarmUser()=0;
    
    virtual void init() = 0;
};

} //namespace configuration
} //namespace narcotics

#endif //ICONFIGURATION_GUARD