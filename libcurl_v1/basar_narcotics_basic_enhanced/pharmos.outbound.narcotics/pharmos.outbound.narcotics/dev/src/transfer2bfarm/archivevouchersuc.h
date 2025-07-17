#ifndef ARCHIVEVOUCHERSUC_GUARD
#define ARCHIVEVOUCHERSUC_GUARD
//------------------------------------------------------------------------------
//includes
//------------------------------------------------------------------------------
#include "usecasecontroler.h"

//------------------------------------------------------------------------------
//namespaces
//------------------------------------------------------------------------------
namespace narcotics{
	namespace bgafolder{
		class BGAFolderRepo;
	}
	namespace configuration{
		class IConfiguration;
	}

	namespace services{
		class ArchiveSVC;
	}
}

namespace narcotics
{
namespace usecase
{

//------------------------------------------------------------------------------
class ArchiveVouchersUC : public UseCaseController
{
public:
	virtual basar::appl::EventReturnType run();

	//void setBGAFolderRepo(boost::shared_ptr<narcotics::bgafolder::BGAFolderRepo> repo);
	//
	//void setArchiveService(boost::shared_ptr<narcotics::services::ArchiveSVC> archiveSVC);
	

	ArchiveVouchersUC(boost::shared_ptr<narcotics::services::ArchiveSVC> archiveSVC,
					  boost::shared_ptr<narcotics::bgafolder::BGAFolderRepo> bgaFolderRepo);

	virtual ~ArchiveVouchersUC();

protected:
	boost::shared_ptr<narcotics::bgafolder::BGAFolderRepo> m_folderRepo;
	boost::shared_ptr<narcotics::services::ArchiveSVC> m_archiveSVC;
};

} //namespace usecase
} //namespace narcotics

#endif //ARCHIVEVOUCHERSUC_GUARD