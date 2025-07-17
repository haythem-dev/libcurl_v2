//---------------------------------------------------------------------------
//include section
//---------------------------------------------------------------------------
#include "commonheader.h"
#include "postscriptfilesource.h"
#include "loggerpool.h"

//---------------------------------------------------------------------------
//namespace section
//---------------------------------------------------------------------------
namespace narcotic
{
namespace dataAccess
{

//---------------------------------------------------------------------------
bool PostScriptFileSource::doesCacheContainString( const basar::VarString& searchedString ) const
{	
	if( m_UsernameCache.empty() ){
		return false;
	}
	
	QString searchedQString(QString::fromLocal8Bit(searchedString.c_str()));
	
	std::set<QString>::iterator it = m_UsernameCache.find(searchedQString);

	if( it == m_UsernameCache.end() ){	
		return false;
	}
	else{
		return true;
	}
}

//---------------------------------------------------------------------------
bool PostScriptFileSource::doesLineContainString( const QString& line, const QString& searchedString ) const
{
	if( line.contains(searchedString,Qt::CaseInsensitive) ){
		return true;
	}

	return false;
}

//---------------------------------------------------------------------------
bool PostScriptFileSource::doesFileContainString( const basar::VarString& searchedString ) const
{
	if( !m_File.open(QIODevice::ReadOnly | QIODevice::Text) )
	{
		basar::cmnutil::ExceptionInfoStruct excInfo;
		excInfo.context = "PostScriptFileSource::PostScriptFileSource(const basar::VarString& path):m_File(QString(path.c_str()))";
		excInfo .file =__FILE__;
		excInfo.line=__LINE__;
		excInfo.reason = basar::VarString("could not open file for reading: ").append(m_File.fileName().toLocal8Bit());
		
		throw basar::cmnutil::BasarRuntimeException(excInfo);		
	}

	QString searchedQString(QString::fromLocal8Bit(searchedString.c_str()));

	QTextStream in(&m_File);

     while( !in.atEnd() ) 
	 {
         QString line = in.readLine();
         
		 if( doesLineContainString(line,searchedQString) )
		 {
			 m_UsernameCache.insert(searchedQString);
			 m_File.close();
			 return true;
		 }
     }

	m_File.close();
	return false;
}

//---------------------------------------------------------------------------
bool PostScriptFileSource::doesContainBitmapForUserName( basar::VarString userName ) const
{
	basar::VarString signatureName = userName.append(".bmp");

	if( narcotics::LoggerPool::loggerNarcotics.isEnabledFor(log4cplus::DEBUG_LOG_LEVEL) )
	{
		basar::VarString msg;
		msg.format("PostScriptFileSource::doesContainBitmapForUserName(basar::VarString userName) const: Searching for entry %s",signatureName.c_str());

		narcotics::LoggerPool::loggerNarcotics.forcedLog(log4cplus::DEBUG_LOG_LEVEL, msg, __FILE__, __LINE__);
	}

	return doesContainString(signatureName);
}

//---------------------------------------------------------------------------
bool PostScriptFileSource::doesContainString( const basar::VarString& searchedString ) const
{	
	if( doesCacheContainString(searchedString) || doesFileContainString(searchedString) )
	{
		if( narcotics::LoggerPool::loggerNarcotics.isEnabledFor(log4cplus::TRACE_LOG_LEVEL) )
		{
			basar::VarString msg;
			msg.format("doesContainString(const basar::VarString& searchedString) const: Username found %s",searchedString.c_str());

			narcotics::LoggerPool::loggerNarcotics.forcedLog(log4cplus::TRACE_LOG_LEVEL, msg, __FILE__, __LINE__);
		}		

		return true;
	}
	
	if( narcotics::LoggerPool::loggerNarcotics.isEnabledFor(log4cplus::TRACE_LOG_LEVEL) )
	{
		basar::VarString msg;
		msg.format("doesContainString(const basar::VarString& searchedString) const: Username not found %s",searchedString.c_str());

		narcotics::LoggerPool::loggerNarcotics.forcedLog(log4cplus::TRACE_LOG_LEVEL, msg, __FILE__, __LINE__);
	}

	return false;
}

//---------------------------------------------------------------------------
PostScriptFileSource::PostScriptFileSource( const basar::VarString& path):m_File(QString(QString::fromLocal8Bit(path.c_str())) )
{
	if( !m_File.exists() )
	{
		basar::cmnutil::ExceptionInfoStruct excInfo;
		excInfo.context = "PostScriptFileSource::PostScriptFileSource(const basar::VarString& path):m_File(QString(path.c_str()))";
		excInfo .file =__FILE__;
		excInfo.line=__LINE__;
		excInfo.reason = basar::VarString("file does not exist!: ").append(path);
		
		throw basar::cmnutil::BasarInvalidParameterException(excInfo);		
	}
}

//---------------------------------------------------------------------------
PostScriptFileSource::~PostScriptFileSource()
{
}

} //namespace dataAccess
} //namespace narcotic