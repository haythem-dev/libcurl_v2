#ifndef TRANSFERTOBFARMUC_GUARD
#define TRANSFERTOBFARMUC_GUARD


namespace narcotics{
	namespace bgafolder {
		class BGAFolderRepo;
	}
	namespace bgabranch {
		class BGABranchRepo;
	}
}
//------------------------------------------------------------------------------
//includes
//------------------------------------------------------------------------------
#include "usecasecontroler.h"

//------------------------------------------------------------------------------
//namespace
//------------------------------------------------------------------------------
namespace narcotics {
namespace usecase {

//------------------------------------------------------------------------------
class TransferToBfarmUC : public UseCaseController
{
public:

	TransferToBfarmUC(
		boost::shared_ptr<narcotics::bgafolder::BGAFolderRepo> folderRepo,
		boost::shared_ptr<narcotics::bgabranch::BGABranchRepo> branchRepo, 
		narcotics::services::ServiceConfiguration config);
	virtual ~TransferToBfarmUC();

	basar::appl::EventReturnStruct run();
private:
	basar::appl::EventReturnStruct renameDeliveryNoteCopyFolders();
	basar::appl::EventReturnStruct zipFolders();
	basar::appl::EventReturnStruct backupZippedFiles();
	basar::appl::EventReturnStruct sendZippedFiles();
	basar::appl::EventReturnStruct deleteZippedFiles();

	boost::shared_ptr<narcotics::bgafolder::BGAFolderRepo>		m_folderRepo;
	boost::shared_ptr<narcotics::bgabranch::BGABranchRepo>		m_branchRepo;
	narcotics::services::ServiceConfiguration					m_config;
	std::list<basar::VarString>									m_zippedFiles;
};

//------------------------------------------------------------------------------

} //namespace usecase
} //namespace narcotics

//------------------------------------------------------------------------------

#endif //TRANSFERTOBFARMUC_GUARD

//------------------------------------------------------------------------------
