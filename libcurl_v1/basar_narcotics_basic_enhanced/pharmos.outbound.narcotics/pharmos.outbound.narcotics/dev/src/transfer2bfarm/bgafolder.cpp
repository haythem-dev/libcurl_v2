//------------------------------------------------------------------------------
//includes
//------------------------------------------------------------------------------
#include "commonheader.h"
#include "bgafolder.h"
#include "definitions.h"
#include "bgabranch.h"
#include "exbgafolder.h"
#include "contributionvoucherfile.h"
#include "bgafolderptr.h"
#include "loggerpool.h"
#include <sstream>

//------------------------------------------------------------------------------
//namespace
//------------------------------------------------------------------------------

namespace narcotics {
namespace bgafolder {

//------------------------------------------------------------------------------

	using narcotics::bgabranch::BGABranch;

	//------------------------------------------------------------------------------
	BGAFolderPtr BGAFolder::copyTo(const basar::VarString& des)
	{
		if(false == m_Dir.isReadable())
		{
			basar::VarString msg = "source dir not readable: ";
			msg.append(absolutePath());
			throw narcotics::exceptions::ExBGAFolder(basar::ExceptInfo("BGAFolderPtr BGAFolder::copyTo(const QString& dest)",msg,__FILE__,__LINE__));
		}

		BGAFolderPtr destinationDir(new BGAFolder(des));
		destinationDir->m_bgaNo = this->m_bgaNo;
		destinationDir->m_weekCounter = this->m_weekCounter;

		if (false == destinationDir->exists())
		{
			if (m_Dir.mkpath(QString::fromLocal8Bit(destinationDir->absolutePath().c_str())) == false)
			{
				basar::VarString msg = "could not create destination dir: ";
				msg.append(destinationDir->absolutePath());
				throw narcotics::exceptions::ExBGAFolder(basar::ExceptInfo("BGAFolderPtr BGAFolder::copyTo(const QString& dest)",msg,__FILE__,__LINE__));
			}
		}		

		QFileInfoList entries = m_Dir.entryInfoList(QDir::AllEntries | QDir::NoDotAndDotDot);

		for(QList<QFileInfo>::iterator it = entries.begin(); it!=entries.end();++it)
		{
			QFileInfo& fileInfo = *it;

			if(fileInfo.isDir())
			{
				basar::VarString msg = "no sub dirs supported ";
				throw narcotics::exceptions::ExBGAFolder(basar::ExceptInfo("BGAFolderPtr BGAFolder::copyTo(const QString& dest)",msg,__FILE__,__LINE__));
			}

			if(fileInfo.isSymLink())
				continue;

			if(fileInfo.isFile() && fileInfo.isReadable())
			{
				QFile file(fileInfo.filePath());
				basar::VarString destinationPath = destinationDir->absoluteFilePath(fileInfo.fileName().toStdString());

				QFileInfo destInfo(QString::fromLocal8Bit(destinationPath.c_str()));
				//delete if file exists
				if(destInfo.exists())
				{
					QDir dir = destInfo.absoluteDir();
					dir.remove(destInfo.absoluteFilePath());
				}

				if (false == file.copy(QString::fromLocal8Bit(destinationPath.c_str())))
				{
					basar::VarString msg = "file copy failed from:";
					msg.append(fileInfo.absoluteFilePath().toStdString());
					msg.append(" to ");
					msg.append(destinationDir->absoluteFilePath(fileInfo.fileName().toStdString()));
					throw narcotics::exceptions::ExBGAFolder(basar::ExceptInfo("BGAFolder::copyTo()",msg,__FILE__,__LINE__));
				}
			}
		}
		
		return destinationDir;
	}

	//------------------------------------------------------------------------------
	void BGAFolder::renameTo(const basar::VarString& dir)
	{
		BLOG_TRACE( LoggerPool::getLoggerTransferBfarm(), "BGAFolder::renameTo()" );

		std::stringstream log;
		log << "old path: " << m_Dir.absolutePath().toStdString();
		BLOG_INFO( LoggerPool::getLoggerTransferBfarm(), log.str() );
		log.str("");
		log << "new path: " << dir;
		BLOG_INFO( LoggerPool::getLoggerTransferBfarm(), log.str() );
	
		
		m_Dir.rename(m_Dir.absolutePath(), dir.c_str());
	}

	//------------------------------------------------------------------------------
	void BGAFolder::reload()
	{
		loadFiles();
	}

	//------------------------------------------------------------------------------
	void BGAFolder::unloadFiles()
	{
		BLOG_TRACE( LoggerPool::getLoggerTransferBfarm(), "  BGAFolder::unloadFiles()" );

		m_files.clear();
		m_filesLoaded = false;
	}

	//------------------------------------------------------------------------------
	void BGAFolder::deleteDuplicateVoucherFiles()
	{
		BLOG_TRACE( LoggerPool::getLoggerTransferBfarm(), "BGAFolder::deleteDuplicateVoucherFiles()" );

		using contributionvoucherfile::ContributionVoucherFilePtr; 
		using contributionvoucherfile::ContributionVoucherFile; 

		m_files.clear();

		QStringList filters;
		filters << "*.pdf";

		QFileInfoList files = m_Dir.entryInfoList(filters, QDir::Files, QDir::Name|QDir::IgnoreCase);

		QRegExp voucherFilesRegExp(definitions::VOUCHERFILEREGEXP);

		std::map<basar::Int32,contributionvoucherfile::ContributionVoucherFilePtr>::iterator foundFileIt;

		for(int i=0;i<files.size();i++)
		{
			QString filename = files.at(i).fileName();
			if(voucherFilesRegExp.exactMatch(files.at(i).fileName()))
			{
				ContributionVoucherFilePtr file(new ContributionVoucherFile(files.at(i).absoluteFilePath () ));				

				foundFileIt = m_files.find(file->getContributionVoucherNo());

				if(foundFileIt != m_files.end())
				{
					if(!file->dir().remove(file->absoluteFilePath()))
					{
						basar::VarString msg = "could not delete file:";
						msg.append(file->absolutePath().toStdString().c_str());
						
						throw narcotics::exceptions::ExBGAFolder(basar::ExceptInfo("BGAFolder::deleteDuplicateVoucherFiles()",msg,__FILE__,__LINE__));
					}
				}
				else
				{
					m_files[file->getContributionVoucherNo()] = file;
				}
			}
		}

		m_filesLoaded = true;
	}

	//------------------------------------------------------------------------------
	void BGAFolder::loadFiles()
	{
		BLOG_TRACE( LoggerPool::getLoggerTransferBfarm(), "BGAFolder::loadFiles()" );

		using contributionvoucherfile::ContributionVoucherFilePtr; 
		using contributionvoucherfile::ContributionVoucherFile; 

		m_files.clear();

		QStringList filters;
		filters << "*.pdf";

		QFileInfoList files = m_Dir.entryInfoList(filters, QDir::Files, QDir::Name|QDir::IgnoreCase);

		QRegExp voucherFilesRegExp;
		
		if(!m_loadDeliveryNoteCopies)
		{
			voucherFilesRegExp.setPattern(definitions::VOUCHERFILEREGEXP);
		}
		else
		{
			voucherFilesRegExp.setPattern(definitions::ALLVOUCHERS );
		}

		for(int i=0;i<files.size();i++)
		{
			QString filename = files.at(i).fileName();
			if(voucherFilesRegExp.exactMatch(files.at(i).fileName()))
			{
				ContributionVoucherFilePtr file(new ContributionVoucherFile(files.at(i).absoluteFilePath () ));
				m_files[file->getContributionVoucherNo()] = file;
			}
		}
	}

	//------------------------------------------------------------------------------
	bool BGAFolder::containsFileWithNumber(basar::Int32 voucherNo)
	{
		BLOG_TRACE( LoggerPool::getLoggerTransferBfarm(), " BGAFolder::containsFileWithNumber(basar::Int32 voucherNo)" );

		std::map<basar::Int32,contributionvoucherfile::ContributionVoucherFilePtr> files = getVoucherFiles();
		
		std::map<basar::Int32,contributionvoucherfile::ContributionVoucherFilePtr>::iterator fileIt;

		fileIt = files.find(voucherNo);

		return fileIt != files.end();		
	}

	//------------------------------------------------------------------------------
	std::map<basar::Int32,contributionvoucherfile::ContributionVoucherFilePtr>& BGAFolder::getVoucherFiles()
	{
		BLOG_TRACE( LoggerPool::getLoggerTransferBfarm(), " BGAFolder::getVoucherFiles()" ); 

		if(!m_filesLoaded)
		{			
			loadFiles();
			m_filesLoaded=true;
		}

		return m_files;
	}

	//------------------------------------------------------------------------------
	basar::Int32 BGAFolder::getBGANo()
	{
		return m_bgaNo;
	}

	//------------------------------------------------------------------------------
	BGAFolder::FolderType BGAFolder::getFolderType()
	{
		return m_FolderType;
	}

	//------------------------------------------------------------------------------
	basar::Int32 BGAFolder::getWeeksSinceStartCounter()
	{
		return m_weekCounter;
	}

	//------------------------------------------------------------------------------
	const basar::VarString BGAFolder::absolutePath()
	{
		return m_Dir.absolutePath().toStdString();
	}

	//------------------------------------------------------------------------------
	bool BGAFolder::exists()
	{
		return m_Dir.exists();
	}

	//------------------------------------------------------------------------------
	const basar::VarString BGAFolder::dirName()
	{
		return m_Dir.dirName().toStdString();
	}

	const basar::VarString BGAFolder::absoluteFilePath(const basar::VarString fileName)
	{
		return m_Dir.absoluteFilePath(QString::fromLocal8Bit(fileName.c_str())).toStdString();
	}

	//------------------------------------------------------------------------------
	void BGAFolder::setBGABranch(bgabranch::BGABranchPtr bgaBranch)
	{
		m_bgaBranch = bgaBranch;
	}

	//------------------------------------------------------------------------------
	narcotics::bgabranch::BGABranchPtr BGAFolder::getBGABranch()
	{
		return m_bgaBranch;
	}

	//------------------------------------------------------------------------------
	bool BGAFolder::isValidContributionVoucherFolder()
	{
		BLOG_TRACE( LoggerPool::getLoggerTransferBfarm(), "BGAFolder::isValidContributionVoucherFolder()" ); 

		QRegExp regExp(definitions::BGAFOLDERREGEXP);

		QString dirName = m_Dir.dirName();
		if(regExp.exactMatch(dirName))
			return true;

		return false;
	}

	//------------------------------------------------------------------------------
	bool BGAFolder::isValidDeliveryNoteCopyFolder()
	{
		BLOG_TRACE( LoggerPool::getLoggerTransferBfarm(), "BGAFolder::isValidDeliveryNoteCopyFolder()" ); 

		QRegExp regExp(definitions::BGAFOLDERDELIVERYNOTECOPYREGEXP);

		QString dirName = m_Dir.dirName();
		if(regExp.exactMatch(dirName))
			return true;

		return false;
	}

	//------------------------------------------------------------------------------
	bool BGAFolder::isValidArchivingFolder()
	{
		BLOG_TRACE( LoggerPool::getLoggerTransferBfarm(), "BGAFolder::isValidArchivingFolder()" ); 

		QRegExp regExp(definitions::ARCHIVEFOLDERREGEXP);

		QString dirName = m_Dir.dirName();
		if(regExp.exactMatch(dirName))
			return true;

		return false;
	}

	//------------------------------------------------------------------------------
	BGAFolder::BGAFolder(basar::VarString path) : 
		m_Dir(QString::fromLocal8Bit(path.c_str())),
		m_weekCounter(-1),
		m_bgaNo(-1),
		m_filesLoaded(false),
		m_loadDeliveryNoteCopies(true),
		m_FolderType(BGAFolder::UNKNOWN)
	{
		BLOG_TRACE( LoggerPool::getLoggerTransferBfarm(), "BGAFolder::BGAFolder(QString path)" ); 

		if(isValidContributionVoucherFolder())
		{
			m_FolderType = BGAFolder::CONTRIBUTIONVOUCHER;
		}
		else if(isValidDeliveryNoteCopyFolder())
		{
			m_FolderType = BGAFolder::DELIVERYNOTECOPY;
		}
		else if (isValidArchivingFolder())
		{
			m_FolderType = BGAFolder::ARCHIVING;
		}
		else
		{				
			basar::VarString msg = "invalid folder ";
			msg.append(dirName());

			throw narcotics::exceptions::ExBGAFolder(basar::ExceptInfo("BGAFolder::BGAFolder()",msg,__FILE__,__LINE__));
		}

		if (m_FolderType == BGAFolder::CONTRIBUTIONVOUCHER || m_FolderType == BGAFolder::DELIVERYNOTECOPY)
		{
			bool ok=false;
			m_weekCounter = m_Dir.dirName().right(3).toInt(&ok,10);
			if(!ok)
			{				
				basar::VarString msg = "cannot extract weekcounter from: ";
				msg.append(dirName());
				throw narcotics::exceptions::ExBGAFolder(basar::ExceptInfo("BGAFolder::BGAFolder()",msg,__FILE__,__LINE__));
			}

			ok=false;

			if (m_FolderType == BGAFolder::CONTRIBUTIONVOUCHER)
			{
				m_bgaNo = m_Dir.dirName().left(7).toInt(&ok,10);
			}
			else if (m_FolderType == BGAFolder::DELIVERYNOTECOPY)
			{
				m_bgaNo = m_Dir.dirName().mid(2,7).toInt(&ok,10);				
			}
			else
			{
				basar::VarString msg = "unknown type ";
				msg.append(dirName());
				throw narcotics::exceptions::ExBGAFolder(basar::ExceptInfo("BGAFolder::BGAFolder()",msg,__FILE__,__LINE__));
			}

			if(!ok)
			{				
				basar::VarString msg = "cannot extract bgano from: ";
				msg.append(dirName());
				throw narcotics::exceptions::ExBGAFolder(basar::ExceptInfo("BGAFolder::BGAFolder()",msg,__FILE__,__LINE__));
			}			
		}
	}

//------------------------------------------------------------------------------

BGAFolder::~BGAFolder()
{	
}

//------------------------------------------------------------------------------

} //namespace bgafolder
} //namespace narcotics

//------------------------------------------------------------------------------
