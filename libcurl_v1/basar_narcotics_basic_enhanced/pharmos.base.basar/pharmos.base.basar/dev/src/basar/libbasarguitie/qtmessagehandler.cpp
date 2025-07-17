/*
* @file                                            
* @author Anke Klink                      
* @date 2008                            
*/    

//------------------------------------------------------------------------------
#include "libbasarguitie_qtmessagehandler.h"

#include "libbasar_definitions.h"
#include "libbasarcmnutil_bstring.h"
#include "loggerpool.h"

#include <QtCore/QtGlobal>
#include <QtCore/QMessageLogContext>

#pragma warning (push)
#pragma warning (disable: 4127)
#include <QtCore/QString>
#pragma warning (pop)


//------------------------------------------------------------------------------
namespace basar
{
namespace gui
{
namespace tie
{

//------------------------------------------------------------------------------
/*!
    * Function basarQTMessageHandler writes QT-Messages to the Logger
    * named "basar.gui.tie.QTMessageHandler" 
    * the MessageHandler has to activated with the line:
    * qInstallMessageHandler( basar::gui::tie::basarQTMessageHandler);
    * That line should be part of each main Programm that uses QT !
	* The function interface is defined by QT.
	* Interface from Qt 5 doc:
	* void myMessageOutput(QtMsgType type, const QMessageLogContext &context, const QString &msg)
	* @param type       This enum describes the messages that was sent to a message 
	*          handler (f.ex. QtWarningMsg), see QT-Dokumentation 
	* @param context    context of the message
	* @param logmsg     The text of the message
    */
void basarQTMessageHandler( QtMsgType type, const QMessageLogContext& context, const QString& logmsg )
{
    log4cplus::LogLevel loglevel = log4cplus::INFO_LOG_LEVEL;
    VarString loglevelname = "QTMessage(Unknown Level) ";
    switch (type)
    {
        case QtDebugMsg:
            {
                loglevel = log4cplus::DEBUG_LOG_LEVEL;
                loglevelname = "QTMessage(Debug) ";
            }
            break;

        case QtWarningMsg:
            {
                loglevel = log4cplus::WARN_LOG_LEVEL;
                loglevelname = "QTMessage(Warning) ";  
            }
            break;
        case QtCriticalMsg:
            {
                loglevel = log4cplus::ERROR_LOG_LEVEL;
                loglevelname = "QTMessage(CriticalMsg or SystemMsg) : ";  
            }
            break;
        case QtFatalMsg:
            {
                loglevel = log4cplus::FATAL_LOG_LEVEL;
                loglevelname = "QTMessage(FatalMsg) ";  
            }
            break;
        default:
            {
                loglevel = log4cplus::INFO_LOG_LEVEL;
                loglevelname = "QTMessage(Unknown) ";
            }
            break;
    }

    if (LoggerPool::qtMessageHandler().isEnabledFor(loglevel))
    {
        VarString msg;
		msg.format("%s : %s - in function %s",  loglevelname.c_str() , logmsg.toLatin1().constData(), context.function);
		LoggerPool::qtMessageHandler().forcedLog(loglevel, msg, context.file, context.line);
    }

    // re-throw Fatal Message:
    if (QtFatalMsg == type)
    {   
        QtMessageHandler h = qInstallMessageHandler(0 );
        if (h != basarQTMessageHandler)
        {
            qInstallMessageHandler(h);
        }
        else
        {
			qFatal(logmsg.toLatin1().constData());
        }    
    }
}

//------------------------------------------------------------------------------
}
}
}
