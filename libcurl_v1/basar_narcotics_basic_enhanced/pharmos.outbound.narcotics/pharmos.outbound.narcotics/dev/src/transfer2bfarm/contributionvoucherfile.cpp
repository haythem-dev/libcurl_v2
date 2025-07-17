//------------------------------------------------------------------------------
//includes
//------------------------------------------------------------------------------
#include "commonheader.h"
#include "contributionvoucherfile.h"
#include "exvoucherfile.h"
#include "definitions.h"
#include "loggerpool.h"

//------------------------------------------------------------------------------
//namespace
//------------------------------------------------------------------------------
namespace narcotics
{
namespace contributionvoucherfile
{

//------------------------------------------------------------------------------
QString ContributionVoucherFile::getFileType()
{
	if(m_type == CONTRIBUTIONVOUCHER)
		return definitions::CONTRIBUTIONVOUCHER;
	if(m_type == DELIVERYNOTECOPY)
		return definitions::DELIVERYNOTECOPY;

	return "";
}

//------------------------------------------------------------------------------
void ContributionVoucherFile::moveTo(QString newPath)
{
	BLOG_TRACE_METHOD( LoggerPool::getLoggerTransferBfarm(), " ContributionVoucherFile::moveTo(QString newPath)" ); 

	QFile file(this->filePath());

	QFile newFile(newPath);

	if(newFile.exists())
	{
		newFile.remove();
	}

	if(!file.copy(newPath))
	{
		
		basar::VarString msg = "copying failed for: ";
        msg.append(this->path().toStdString().c_str());
		msg.append("to: ");
		msg.append(newPath.toStdString().c_str());

		throw narcotics::exceptions::ExVoucherFile(basar::ExceptInfo("moveTo(QString newPath)",msg,__FILE__,__LINE__));
	}

	if(!file.remove())
	{
		basar::VarString msg = "removing file failed: ";
		msg.append(this->path().toStdString().c_str());
		
		throw narcotics::exceptions::ExVoucherFile(basar::ExceptInfo("moveTo(QString newPath)",msg,__FILE__,__LINE__));
	}

	this->setFile(newPath);
}

//------------------------------------------------------------------------------
basar::Int32 ContributionVoucherFile::getContributionVoucherNo()
{
	BLOG_TRACE( LoggerPool::getLoggerTransferBfarm(), this->fileName().toStdString() ); 
	
	QString fileName = this->fileName();

	QString voucherNo;

	if(this->m_type==DELIVERYNOTECOPY)
	{
		voucherNo=fileName.mid(2,8);
	}
	else if(this->m_type==CONTRIBUTIONVOUCHER)
	{
		voucherNo = fileName.left(8);
	}
	return voucherNo.toInt();
}

//------------------------------------------------------------------------------
basar::Int32 ContributionVoucherFile::getCounter()
{
	QString fileName = this->fileName();
	QString counter;

	if(this->m_type==DELIVERYNOTECOPY)
	{
		counter=fileName.mid(10,4);
	}
	else if(this->m_type==CONTRIBUTIONVOUCHER)
	{
		counter = fileName.mid(8,4);//    right(4);
	}

	return counter.toInt();
}

//------------------------------------------------------------------------------
void ContributionVoucherFile::setCounter(basar::Int32 counter)
{
	BLOG_TRACE( LoggerPool::getLoggerTransferBfarm(), "ContributionVoucherFile::setCounter(basar::Int32 counter)" );  

	basar::VarString msg;
	QString voucherNo;
	voucherNo.setNum(this->getContributionVoucherNo());

	QString strCounter;
	strCounter = strCounter.setNum(counter).rightJustified(4,'0');
	
	QString newPath = this->dir().absolutePath() + QDir::separator() + voucherNo + strCounter + ".pdf";

	QFileInfo newFileInfo(newPath);

	if(newFileInfo != *this)
	{
		QDir dir;
		
		msg.format("renaming %s to %s", this->absoluteFilePath().toStdString().c_str(), newPath.toStdString().c_str());
		BLOG_INFO(LoggerPool::getLoggerTransferBfarm(), msg);
		if(!dir.rename(this->absoluteFilePath(),newPath))
		{			
			msg = "rename failed for: ";
			msg.append(this->absoluteFilePath().toStdString().c_str());
			msg.append(" new path: ");
			msg.append(newPath.toStdString().c_str());
			BLOG_ERROR(LoggerPool::getLoggerTransferBfarm(), msg);
			throw narcotics::exceptions::ExVoucherFile(basar::ExceptInfo("ContributionVoucherFile::setCounter(basar::Int32 counter)",msg,__FILE__,__LINE__));
		}

		this->setFile(newPath);
	}
}
	
//------------------------------------------------------------------------------
ContributionVoucherFile::ContributionVoucherFile(QString filePath):QFileInfo(filePath)
{
	BLOG_TRACE( LoggerPool::getLoggerTransferBfarm(), "ContributionVoucherFile::ContributionVoucherFile(QString filePath):QFileInfo(filePath)" );  

	QRegExp voucherFileNamePattern(definitions::VOUCHERFILEREGEXP);
	QRegExp deliveryNoteCopyPattern(definitions::DELIVERYNOTECOPYREGEXP);

	if(voucherFileNamePattern.exactMatch(this->fileName()))
	{
		this->m_type = CONTRIBUTIONVOUCHER;
	}
	else if(deliveryNoteCopyPattern.exactMatch(this->fileName()))
	{	
		this->m_type = DELIVERYNOTECOPY;
	}
	else
	{
		basar::VarString msg = "file does not exists: ";
		msg.append(this->absoluteFilePath().toStdString().c_str() );
		
		throw narcotics::exceptions::ExVoucherFile(basar::ExceptInfo("ContributionVoucherFile::ContributionVoucherFile(QString filePath):QFileInfo(filePath)",msg,__FILE__,__LINE__));
	}

	if(!this->exists() )
	{
		basar::VarString msg = "file does not exists: ";
		msg.append(this->path().toStdString().c_str());
		
		throw narcotics::exceptions::ExVoucherFile(basar::ExceptInfo("ContributionVoucherFile::ContributionVoucherFile(QString filePath):QFileInfo(filePath)",msg,__FILE__,__LINE__));
	}
}

//------------------------------------------------------------------------------
ContributionVoucherFile::~ContributionVoucherFile(void)
{
}

} //namespace contributionvoucherentry
} //namespace narcotics
