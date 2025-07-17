//-------------------------------------------------------------------------------------------------------------------------//
// include section
//-------------------------------------------------------------------------------------------------------------------------//
#include "commonheader.h"
#include "contributionvoucherftptransfer.h"
#include "loggerpool.h"
#include "weekcalculator.h"

//-------------------------------------------------------------------------------------------------------------------------//
// namespace section
//-------------------------------------------------------------------------------------------------------------------------//
namespace narcotics
{

//----------------------------------------------------------------------------
QString ContributionVoucherFTPTransfer::generateFilePath(	const basar::Int32 bgano, 
															const basar::Int32 contributionvoucherno,
															const basar::VarString startDate,
															const FileCreationBehaviour& fileCreationBehaviour /*= Preserve*/) 
{
	
	QString dir = generateDirectory( bgano,startDate );
	QString fileName = generateFileName( dir, contributionvoucherno, fileCreationBehaviour );
	
	return dir + fileName;	
}

//----------------------------------------------------------------------------
QString ContributionVoucherFTPTransfer::generateDirectory(	const basar::Int32 bgano, 
															const basar::VarString startDate )										
{
	basar::VarString directoryname;	
	basar::Int32 serialWeek;
	
	// get serial week
	WeekCalculator calc;
	serialWeek = calc.calculateSerialWeek(startDate);

	directoryname.format(	"%07d%0*d/",
							bgano,
							constants::SERIAL_LENGTH_FOLDER,
							serialWeek	);
	
	QString dir( QString::fromLocal8Bit(directoryname.c_str()) );
	
	if( !doesDirectoryExist("",dir) )
	{
		if( !createDirectory(dir) )
		{
			basar::cmnutil::ExceptionInfoStruct excInfo;
			excInfo.context = "NarcoticsFTPFileTransfer::generateDirectoryName";
			excInfo .file =__FILE__;
			excInfo.line=__LINE__;
			excInfo.reason = "Directory creation failed!!";
			
			throw basar::cmnutil::BasarRuntimeException(excInfo);	
		}
		
		setPermissions("777", dir);
	}
	
	return dir;
}

//----------------------------------------------------------------------------
QString ContributionVoucherFTPTransfer::generateFileName( QString &directoryName, const basar::Int32 &contributionVoucherNo, const FileCreationBehaviour& fileCreationBehaviour )
{	
	BLOG_TRACE_METHOD(narcotics::LoggerPool::loggerUseCases, "ContributionVoucherFTPTransfer::generateFileName");
	
	basar::VarString filename;
	basar::Int32 serialFileNo;	
	
	serialFileNo = 0;	
	
	m_foundDirectoryEntries.clear();

	m_Ftp.list(directoryName);

	m_TimeWasteLoop.exec();

	bool doesFileExist = false;

	std::stringstream ss;
	
	ss << contributionVoucherNo;

    QString contributionVoucherNoStr(QString::fromLocal8Bit(ss.str().c_str()));

    //no overwrite, always make a new file -> default case
    serialFileNo = m_foundDirectoryEntries.size() + 1;	

	if( fileCreationBehaviour == ContributionVoucherFTPTransfer::Overwrite )
	{
        //look for matching file, if found, use that filename, ie. set serialFileNo
	    //search for file

	    QList<QUrlInfo>::Iterator i;
        
        for( i = m_foundDirectoryEntries.begin(); i != m_foundDirectoryEntries.end(); ++i )
	    {
		    QString foundFilename = (*i).name();

		    if( foundFilename.contains(contributionVoucherNoStr) )
		    {
			    //extract serialno
			    QString serialString = i->name().mid(i->name().length()-constants::SERIAL_LENGTH_VOUCHER - 4,constants::SERIAL_LENGTH_VOUCHER);//?4 meaning extention, eg. '.pdf'?

			    doesFileExist = true;

			    serialFileNo = serialString.toInt();
			    break;
            }
	    }
    }

	if( serialFileNo == 0 )
	{
			basar::cmnutil::ExceptionInfoStruct excInfo;
			excInfo.context = "contributionvoucherftptransfer: generateFilename";
			excInfo.file =__FILE__;
			excInfo.line=__LINE__;
			excInfo.reason = "couldnot convert serial of filename!";

			//TODO: sollte auf dedizierte exception umgestellt werden
			throw basar::cmnutil::BasarRuntimeException(excInfo);	
	}

	filename.format("%d%0*d.pdf",
		contributionVoucherNo,
		constants::SERIAL_LENGTH_VOUCHER,
		serialFileNo);
	
	return QString(QString::fromLocal8Bit(filename.c_str()));
}

//----------------------------------------------------------------------------
void ContributionVoucherFTPTransfer::reorganizeFileNames( QString dir )
{
	QList<QUrlInfo> dirEntries = listDirectory( dir );
		
	if( dirEntries.size() > 0 )
	{			
		for( int i = 0 ; i < dirEntries.size(); i++ )
		{
			QUrlInfo urlInfo = dirEntries.at(i);
			QString name = urlInfo.name();
			
			if( urlInfo.isValid() )
			{
				QRegExp contrVoucherNoRegExp("\\d{12}.pdf");
				
				if( contrVoucherNoRegExp.exactMatch( urlInfo.name() ) )
				{							
					QString oldPathToFile = dir + "/" + urlInfo.name();
					
					QString newPathToFile = urlInfo.name().left(urlInfo.name().size()-8);
					
					newPathToFile = dir + "/" + newPathToFile + QString::number(i+1).rightJustified(4,'0') + ".pdf";
										
					m_Ftp.rename(oldPathToFile,newPathToFile);						
					m_TimeWasteLoop.exec();					
					
					setPermissions("777", newPathToFile);	

					BLOG_TRACE( LoggerPool::loggerUseCases, "Contributionvoucherftptransfer: reorganized file: " + newPathToFile.toStdString() );
				}
			}				
		}							
	}
}

//----------------------------------------------------------------------------
bool ContributionVoucherFTPTransfer::checkFileExists(	const basar::Int32 bgano, 
													    const basar::Int32 contributionvoucherno,
													    const basar::VarString startDate    )
{
    QMutexLocker locker( &m_FtpLock );
	
	bool retVal = false;
	
	if( establishFTPConnection() )
	{
		QString dir = generateDirectory( bgano, startDate );
		
	    QString contrVoucherNoStr;
	    contrVoucherNoStr = contrVoucherNoStr.setNum( contributionvoucherno );

        QRegExp contrVoucherNoRegExp( contrVoucherNoStr + "\\d{4}.pdf" );

		QList<QUrlInfo> dirEntries = listDirectory( dir );
		
		if( dirEntries.size() > 0 )
		{
			for( int i = 0 ; i < dirEntries.size(); i++ )
			{
				QUrlInfo urlInfo = dirEntries.at(i);
				QString name = urlInfo.name();
				
				if( urlInfo.isValid() )
				{
					if( urlInfo.name().endsWith(".pdf") )
					{
						if( contrVoucherNoRegExp.exactMatch( urlInfo.name() ) )
						{
							retVal = true;
                            break;
						}
					}
				}				
			}							
		}
		releaseFTPConnection();
	}
	
	return retVal;
}

//----------------------------------------------------------------------------
bool ContributionVoucherFTPTransfer::cancelOrder(	const basar::Int32 bgano, 
													const basar::Int32 contributionvoucherno,
													const basar::VarString startDate
												)
{	
	QMutexLocker locker(&m_FtpLock);
	
	bool retVal = false;
	
	if( establishFTPConnection() )
	{
		QString dir = generateDirectory( bgano, startDate );

        QString contrVoucherNoStr;
        contrVoucherNoStr = contrVoucherNoStr.setNum( contributionvoucherno );
        
        QRegExp contrVoucherNoRegExp(contrVoucherNoStr + "\\d{4}.pdf");	

		QList<QUrlInfo> dirEntries = listDirectory( dir );
		
		if( dirEntries.size() > 0 )
		{
			for( int i = 0 ; i < dirEntries.size(); i++ )
			{
				QUrlInfo urlInfo = dirEntries.at(i);
				QString name = urlInfo.name();
				
				if( urlInfo.isValid() )
				{
					if( urlInfo.name().endsWith(".pdf") )
					{					
						if( contrVoucherNoRegExp.exactMatch( urlInfo.name() ) )
						{
							QString pathToFile = dir + "/" + urlInfo.name();
							
							BLOG_TRACE( LoggerPool::loggerUseCases, "Contributionvoucherftptransfer: canceledOrder: " + pathToFile.toStdString() );

							deleteFile(pathToFile);
							
							reorganizeFileNames(dir);
						
							retVal = true;
                            break;
						}
					}
				}				
			}							
		}
		releaseFTPConnection();
	}
	
	return retVal;
}

//----------------------------------------------------------------------------
//bgano = nummer des abgebenden
bool ContributionVoucherFTPTransfer::storeFile(	basar::cmnutil::IPrintDataProvider& printData, 
												const basar::Int32 bgano, 
												const basar::Int32 contributionvoucherno,
												const basar::VarString startDate,
												const FileCreationBehaviour& fileCreationBehaviour /*= Preserve*/
												)
{
	bool retVal = false;
	try
	{
		QMutexLocker locker( &m_FtpLock );
		
		if( establishFTPConnection() )
		{
			QString dir = generateFilePath(bgano,contributionvoucherno,startDate, fileCreationBehaviour);	
			
			BLOG_TRACE( LoggerPool::loggerUseCases, "ContributionVoucherFTPTransfer::storeFile : directory " + dir.toStdString() );

			if( isValidPath(dir) )
			{							
				QStringList splittedPath = splitDir(dir);
				
				QString fileName = splittedPath.back();
												
				fileName.push_front(m_LocalTmp);
				
				basar::VarString basarFileName = fileName.toLocal8Bit();
				
				basar::cmnutil::InterpretPrint createAbgabe;
				
				createAbgabe.savePdf(printData, basarFileName);
				
				QFile file(fileName);

				BLOG_TRACE( LoggerPool::loggerUseCases, "ContributionVoucherFTPTransfer::storeFile :" + fileName.toStdString() );

				m_Ftp.put( &file, dir);											
				
				m_TimeWasteLoop.exec();	
				
				if( m_FtpErrorOccured )
				{
					releaseFTPConnection();
					return false;
				}
				
				//change mod to 666 for access of transfer_bfarm.sh
				setPermissions("666", dir);	
							
				file.remove();
				
				retVal = true;
				
				if( m_FtpErrorOccured ){				
					retVal = false;
				}
			}
			releaseFTPConnection();
		}
		return retVal;
	}
	catch(basar::cmnutil::BasarException ex)
	{
		BLOG_TRACE( LoggerPool::loggerUseCases, "ContributionVoucherFTPTransfer::storeFile :" + ex.what() );
		return false;
	}
}

//----------------------------------------------------------------------------
ContributionVoucherFTPTransfer::ContributionVoucherFTPTransfer(	basar::VarString hostname,
																basar::VarString username, 
																basar::VarString password,
																basar::VarString saveDirectory,
																basar::VarString localTmp,
																basar::Int32 port
																 )	: NarcoticsFTPFileTransfer(	hostname,
																								username,
																								password,
																								saveDirectory,
																								localTmp,
																								port)
																															
{
}

//----------------------------------------------------------------------------
ContributionVoucherFTPTransfer::ContributionVoucherFTPTransfer( const ContributionVoucherFTPTransfer& rhs )
: NarcoticsFTPFileTransfer(	rhs.m_Hostname.toLocal8Bit().data(),
							rhs.m_UserName.toLocal8Bit().data(),
							rhs.m_Password.toLocal8Bit().data(),
							rhs.m_SaveDirectory.toLocal8Bit().data(),
							rhs.m_LocalTmp.toLocal8Bit().data(),
							rhs.m_Port)
																										
{
}

//----------------------------------------------------------------------------
ContributionVoucherFTPTransfer& ContributionVoucherFTPTransfer::operator=( const ContributionVoucherFTPTransfer& )
{
	return *this;
}

//----------------------------------------------------------------------------

ContributionVoucherFTPTransfer::~ContributionVoucherFTPTransfer( void )
{	
}

} //namespace narcotics