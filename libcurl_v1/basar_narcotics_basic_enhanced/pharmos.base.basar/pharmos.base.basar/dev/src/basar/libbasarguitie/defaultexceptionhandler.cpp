/*
* @file                                            
* @author Anke Klink                      
* @date 2009                            
*/      

#include "libbasarguitie_defaultexceptionhandler.h" 

#include "libbasarcmnutil_exceptions.h"

#pragma warning (push)
#pragma warning (disable: 4127)
#include <QtCore/QString>
#pragma warning (pop)

#pragma warning (push)
#pragma warning (disable: 4127 4231 4251 4512 4800)
#include <QtWidgets/QMessagebox>
#pragma warning (pop)

//----------------------------------------------------------------------------
namespace basar
{
namespace gui 
{
namespace tie
{

// ----------------------------------------------------------
DefaultExceptionHandler::DefaultExceptionHandler( )
{

}
// ----------------------------------------------------------
DefaultExceptionHandler::~DefaultExceptionHandler( )
{

}
// ----------------------------------------------------------
QString DefaultExceptionHandler::getInfo( const ExceptionFoundInfo & found , bool ok)
{
	QString helper;
	helper.setNum(found.line);
	VarString msg;
	msg.append(" \n");
	msg.append("found in \n");
	msg.append(found.context.c_str());
	msg.append(" \n");
	msg.append("file     : ");
	msg.append(found.file.c_str());
	msg.append(" \n");
	msg.append("line     : "); 
	msg.append(helper.toLatin1().constData());
	msg.append(" \n");
	msg.append("returns ");
	msg.append((ok)?"true":"false");
	return QString::fromLatin1(msg.c_str());
}
// ----------------------------------------------------------
void DefaultExceptionHandler::show(const QString & msg, bool ok)
{ 
	if (ok)
	{
		QMessageBox::warning(0,  QString::fromLatin1("Exception - Warning") , msg);
	}
	else
	{
		QMessageBox::critical(0,  QString::fromLatin1("Exception - Error") , msg);
	}
}

// ----------------------------------------------------------
bool DefaultExceptionHandler::handleException(
	const basar::Exception & ex , 
	const ExceptionFoundInfo & found, 
	bool defretval)  
{ 
	bool retval = defretval; 
	QString msg;
	msg.append(QString::fromLatin1("Exception : \n"));
	msg.append(QString::fromLatin1(ex.what().c_str()));
	msg.append(getInfo(found, retval));

	show(msg, retval); 
	return retval;
}

// ----------------------------------------------------------

bool DefaultExceptionHandler::handleException(
	const std::exception& ex, 
	const ExceptionFoundInfo &  found,
	bool defretval)  
{
	bool retval = defretval; 
	QString msg;
	msg.append(QString::fromLocal8Bit("Exception : \n"));
	msg.append(QString::fromLocal8Bit(ex.what()));
	msg.append(getInfo(found, retval));

	show(msg, retval); 
	return retval;
}
// ----------------------------------------------------------

bool DefaultExceptionHandler::handleUnknownException(
	const ExceptionFoundInfo & found  , 
	bool defretval)   
{
	bool retval = defretval; 
	QString msg;
	msg.append(QString::fromLatin1(" Unknown Exception : \n")); 
	msg.append(getInfo(found, retval));
	show(msg, retval);
	return retval;
}

// ----------------------------------------------------------
}
}
}
