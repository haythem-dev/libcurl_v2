#ifndef ARCHIVESVC_GUARD
#define ARCHIVESVC_GUARD

//------------------------------------------------------------------------------
//includes
//------------------------------------------------------------------------------
#include "metadatawriterptr.h"
#include "bgafolderptr.h"
#include "bfarmservice.h"

//------------------------------------------------------------------------------
//namespaces
//------------------------------------------------------------------------------
namespace narcotics{
	namespace bgafolder{
		class BGAFolder;
	}
}

namespace narcotics{
	namespace services{
		class ServiceConfiguration;
	}
}

namespace narcotics
{
namespace services
{

//------------------------------------------------------------------------------
class ArchiveSVC : public BfarmService
{
	
public:
	void archiveBGAFolder(narcotics::bgafolder::BGAFolderPtr folder);

	ArchiveSVC(	boost::shared_ptr<narcotics::bgabranch::BGABranchRepo> bgaBranchRepo,
					boost::shared_ptr<narcotics::contributionvoucherentry::ContributionVoucherEntryRepo> voucherRepo,
					boost::shared_ptr<narcotics::bgafolder::BGAFolderRepo> bgaFolderRepo,
					boost::shared_ptr<narcotics::datecalc::StartEndDateCalculator> dateCalc,
					narcotics::services::ServiceConfiguration config,
					utils::MetaDataWriterPtr metaDataWriter
					);

	virtual ~ArchiveSVC();

protected:	
	
	utils::MetaDataWriterPtr m_MetaDataWriter;	
};

} //namespace services
} //namespace narcotics

#endif //ARCHIVESVC_GUARD