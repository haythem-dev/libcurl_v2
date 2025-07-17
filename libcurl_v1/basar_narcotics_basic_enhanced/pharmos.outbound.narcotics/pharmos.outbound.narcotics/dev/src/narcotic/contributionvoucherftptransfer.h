#ifndef CONTRIBUTIONVOUCHERFTPTRANSFER_H_GUARD
#define CONTRIBUTIONVOUCHERFTPTRANSFER_H_GUARD

//-------------------------------------------------------------------------------------------------------------------------//
// include section
//-------------------------------------------------------------------------------------------------------------------------//
#include "commonheader.h"
#include "contributionvoucherprintdata.h"
#include "narcoticsftpfiletransfer.h"

#pragma warning (push) // to avoid warnings from QT always put QT-includes between push - pop
#pragma warning(disable: 4127 4244 4311 4312 4481 4512 4800)
#include <QtNetwork/QFtp>
#pragma warning (pop)

//-------------------------------------------------------------------------------------------------------------------------//
// namespace section
//-------------------------------------------------------------------------------------------------------------------------//
namespace narcotics
{

class ContributionVoucherFTPTransfer : public NarcoticsFTPFileTransfer
{
Q_OBJECT

public:
	virtual bool storeFile(	basar::cmnutil::IPrintDataProvider& printData, 
							const basar::Int32 bgano, 
							const basar::Int32 contributionvoucherno,
							const basar::VarString startDate,
							const FileCreationBehaviour& fileCreationBehaviour = Preserve
							);
							
	virtual bool cancelOrder(	const basar::Int32 bgano, 
								const basar::Int32 contributionvoucherno,
								const basar::VarString startDate
							);	
	
	virtual bool checkFileExists(	const basar::Int32 bgano, 
								    const basar::Int32 contributionvoucherno,
								    const basar::VarString startDate    );
	 
    ContributionVoucherFTPTransfer(	basar::VarString hostname,
									basar::VarString username, 
									basar::VarString password,
									basar::VarString saveDirectory,
									basar::VarString localTmp,
									basar::Int32 port = 21);

	virtual ~ContributionVoucherFTPTransfer(void);
	
	virtual QString generateFilePath(	const basar::Int32 bgano, 
										const basar::Int32 contributionvoucherno,
										const basar::VarString startDate,
										const FileCreationBehaviour& fileCreationBehaviour = Preserve);

protected:
	virtual QString generateDirectory(	const basar::Int32 bgano, 
										const basar::VarString startDate );	
	
	virtual QString generateFileName(	QString &directoryName, 
										const basar::Int32 &contributionVoucherNo,
										const FileCreationBehaviour& fileCreationBehaviour = Preserve );
										
	virtual void reorganizeFileNames( QString dir );
	
public:
	ContributionVoucherFTPTransfer( const ContributionVoucherFTPTransfer& );
	ContributionVoucherFTPTransfer& operator=( const ContributionVoucherFTPTransfer& );	
};

} //namespace narcotics

#endif //CONTRIBUTIONVOUCHERFTPTRANSFER_H_GUARD