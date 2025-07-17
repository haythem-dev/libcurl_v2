//-------------------------------------------------------------------------------------------------------------------------//
// include section
//-------------------------------------------------------------------------------------------------------------------------//
#include "commonheader.h"
#include "deliverynotecopyftptransfer.h"

//-------------------------------------------------------------------------------------------------------------------------//
// namespace section
//-------------------------------------------------------------------------------------------------------------------------//
namespace narcotics
{

//----------------------------------------------------------------------------
//ATTENTION: filecreationbehaviour is ignored here(not nice, but fast)
QString DeliveryNoteCopyFTPTransfer::generateFilePath(	const basar::Int32 bgano, 
														const basar::Int32 contributionvoucherno,
														const basar::VarString startDate,
														const FileCreationBehaviour& fileCreationBehaviour )
{
	fileCreationBehaviour;

	QString dir = ContributionVoucherFTPTransfer::generateDirectory( bgano, startDate );
	
	QString ld = "LD";

	dir = ld.append(dir);

	if( !doesDirectoryExist(dir) )
	{
		m_Ftp.mkdir(dir);
		m_TimeWasteLoop.exec();
        
        setPermissions("777", dir);
	}	
	
	QString fileName = ContributionVoucherFTPTransfer::generateFileName( dir, contributionvoucherno );
	
	QString path = dir + "LD" + fileName;
	
 	return path;
}
														
//----------------------------------------------------------------------------
DeliveryNoteCopyFTPTransfer::DeliveryNoteCopyFTPTransfer(	basar::VarString hostname,
															basar::VarString username, 
															basar::VarString password,
															basar::VarString saveDirectory,
															basar::VarString localTmp,
															basar::Int32 port)														
															:ContributionVoucherFTPTransfer(	hostname,
																								username,
																								password,
																								saveDirectory,
																								localTmp,
																								port)
{
}

//----------------------------------------------------------------------------
DeliveryNoteCopyFTPTransfer::~DeliveryNoteCopyFTPTransfer(void)
{
}

} //namespace narcotics