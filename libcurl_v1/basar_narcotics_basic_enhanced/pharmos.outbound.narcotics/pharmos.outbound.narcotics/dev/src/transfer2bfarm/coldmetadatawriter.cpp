//--------------------------------------------------------------------------------------------------//
// include section
//--------------------------------------------------------------------------------------------------//
#include "commonheader.h"
#include "bgafolder.h"
#include "exbgafolder.h"
#include "coldmetadatawriter.h"

//--------------------------------------------------------------------------------------------------//
// namespace section
//--------------------------------------------------------------------------------------------------//
namespace utils
{
	using narcotics::archiving::MetaData;

//--------------------------------------------------------------------------------------------------//
void ColdMetaDataWriter::write(MetaData& metaData, narcotics::bgafolder::BGAFolderPtr dir)
{
	if(false == dir->exists())
	{
		basar::VarString msg = "invalid folder: ";
		msg.append(dir->dirName());
		throw narcotics::exceptions::ExBGAFolder(basar::ExceptInfo("ColdMetaDataWriter::write",msg,__FILE__,__LINE__));
	}

	if(m_destFileName.empty())
	{
		basar::VarString msg = "no metadata filename provided ";
		throw narcotics::exceptions::ExBGAFolder(basar::ExceptInfo("ColdMetaDataWriter::write",msg,__FILE__,__LINE__));
	}

	basar::VarString metaDataFilename = dir->absolutePath() + QDir::separator().toLatin1() + m_destFileName;


	QFileInfo info(QString::fromLocal8Bit(metaDataFilename.c_str()));
	QFile file(info.absoluteFilePath());

	if(file.open(QFile::WriteOnly | QFile::Append))
	{
		QTextStream out(&file);
		out << metaData.toString();
		file.close();
	}
	else
	{
		basar::VarString msg = "could not open file for writing: ";
		msg.append(file.fileName().toStdString());
		throw narcotics::exceptions::ExBGAFolder(basar::ExceptInfo("ColdMetaDataWriter::write(MetaData& metaData, QDir& dir)",msg,__FILE__,__LINE__));
	}
}

//--------------------------------------------------------------------------------------------------//
ColdMetaDataWriter::ColdMetaDataWriter(narcotics::services::ServiceConfiguration config)
{	
	m_destFileName = config.archivingMetaDataFileName;
}

//--------------------------------------------------------------------------------------------------//
ColdMetaDataWriter::ColdMetaDataWriter()
{
}

//--------------------------------------------------------------------------------------------------//
ColdMetaDataWriter::~ColdMetaDataWriter()
{
}

} //namespace utils