#ifndef BGAFOLDERREPO_GUARD
#define BGAFOLDERREPO_GUARD

//------------------------------------------------------------------------------
//includes
//------------------------------------------------------------------------------
#include "bgafolderptr.h"

//------------------------------------------------------------------------------
//namespace
//------------------------------------------------------------------------------

namespace narcotics {
namespace bgafolder {

//------------------------------------------------------------------------------
class BGAFolderRepo
{
public:
	BGAFolderRepo();
	virtual ~BGAFolderRepo();

	std::map<basar::VarString, BGAFolderPtr> getAllBGAFoldersFromDir(basar::VarString dir);
	
	void includeDeliveryNoteCopies(bool include);

	void shutdown();

private:
	bool m_deliveryNoteCopiesIncluded;
};

//------------------------------------------------------------------------------

} //namespace bgabranch
} //namespace narcotics

//------------------------------------------------------------------------------

#endif //BGAFOLDERREPO_GUARD

//------------------------------------------------------------------------------
