//------------------------------------------------------------------------------
//includes
//------------------------------------------------------------------------------
#include "commonheader.h"
#include "bgafolderrepo.h"
#include "bgafolder.h"
#include "definitions.h"
#include "exbgafolder.h"
#include "loggerpool.h"

//------------------------------------------------------------------------------
//namespace
//------------------------------------------------------------------------------
namespace narcotics {
namespace bgafolder {

//------------------------------------------------------------------------------

BGAFolderRepo::BGAFolderRepo() :
	m_deliveryNoteCopiesIncluded(false)
{
}

//------------------------------------------------------------------------------

BGAFolderRepo::~BGAFolderRepo()
{
}

//------------------------------------------------------------------------------

void BGAFolderRepo::shutdown()
{
}

//------------------------------------------------------------------------------

void BGAFolderRepo::includeDeliveryNoteCopies(bool include)
{
	m_deliveryNoteCopiesIncluded = include;
}

//------------------------------------------------------------------------------
std::map<basar::VarString, BGAFolderPtr> BGAFolderRepo::getAllBGAFoldersFromDir(basar::VarString dir)
{
	BLOG_TRACE( LoggerPool::getLoggerTransferBfarm(), "BGAFolderRepo::getAllBGAFoldersFromDir(basar::VarString dir)" );  

	using narcotics::bgafolder::BGAFolder;

	QDir directory(QString::fromLocal8Bit(dir.c_str()));

	std::map<basar::VarString, BGAFolderPtr> bgaFolders;

	if(!directory.exists())
	{
		basar::VarString msg = "Path does not exist: ";
		msg.append(dir);

		throw narcotics::exceptions::ExBGAFolder(basar::ExceptInfo("BGAFolderRepo::getAllBGAFoldersFromDir(const basar::VarString& dir)",msg,__FILE__,__LINE__));
	}
	
	QFileInfoList entries = directory.entryInfoList(QDir::AllDirs | QDir::NoDotAndDotDot);
	
	QRegExp regexp;

	if(m_deliveryNoteCopiesIncluded)
	{
		regexp.setPattern(definitions::BGAFOLDERALLENTRIES);
	}
	else
	{
		regexp.setPattern(definitions::BGAFOLDERREGEXP);
	}
	
	for(int i=0;i<entries.size();i++)
	{
		if(entries.at(i).isDir() && regexp.exactMatch(entries.at(i).fileName()))
		{
			BGAFolderPtr folder(new BGAFolder(entries.at(i).absoluteFilePath().toStdString()));
			bgaFolders[folder->dirName()] = folder;
		}
	}
	return bgaFolders;
}

//------------------------------------------------------------------------------

} //namespace bgabranch
} //namespace narcotics