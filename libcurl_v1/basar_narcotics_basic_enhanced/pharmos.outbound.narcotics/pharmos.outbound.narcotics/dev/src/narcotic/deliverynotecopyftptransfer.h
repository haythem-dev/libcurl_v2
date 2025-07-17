#ifndef DELIVERYNOTECOPYFTPTRANSFER_H_GUARD
#define DELIVERYNOTECOPYFTPTRANSFER_H_GUARD

//-------------------------------------------------------------------------------------------------------------------------//
// include section
//-------------------------------------------------------------------------------------------------------------------------//
#include "contributionvoucherftptransfer.h"

//-------------------------------------------------------------------------------------------------------------------------//
// namespace section
//-------------------------------------------------------------------------------------------------------------------------//
namespace narcotics
{

//----------------------------------------------------------------------------
class DeliveryNoteCopyFTPTransfer :
	public ContributionVoucherFTPTransfer
{
public:
	DeliveryNoteCopyFTPTransfer(	basar::VarString hostname,
									basar::VarString username, 
									basar::VarString password,
									basar::VarString saveDirectory,
									basar::VarString localTmp,
									basar::Int32 port = 21);
	virtual ~DeliveryNoteCopyFTPTransfer(void);
	
protected:
	//ATTENTION: filecreationbehaviour is ignored here(not nice, but fast)
	virtual QString generateFilePath(	const basar::Int32 bgano, 
										const basar::Int32 contributionvoucherno,
										const basar::VarString startDate,
										const FileCreationBehaviour& fileCreationBehaviour = Preserve );
};

} //namespace narcotics

#endif //DELIVERYNOTECOPYFTPTRANSFER_H_GUARD