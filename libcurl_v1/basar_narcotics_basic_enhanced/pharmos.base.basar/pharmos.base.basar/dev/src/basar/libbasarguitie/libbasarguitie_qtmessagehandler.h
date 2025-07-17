 /*
  * @file                                            
  * @author Anke Klink                      
  * @date 2008                            
  */    

                                         
#ifndef  GUARD__LIBBASARGUITIE_QTMESSAGEHANDLER_H__ 
#define  GUARD__LIBBASARGUITIE_QTMESSAGEHANDLER_H__ 

//------------------------------------------------------------------------------
#ifndef LIBBASARGUITIE_API
	#ifdef _WIN32
		#ifdef LIBBASARGUITIE_EXPORTS
			#define LIBBASARGUITIE_API __declspec(dllexport)        //!< dll exported
		#else
			#define LIBBASARGUITIE_API __declspec(dllimport)        //!< dll imported
		#endif
	#else
		#define LIBBASARGUITIE_API
	#endif
#endif	// LIBBASARGUITIE_API

//------------------------------------------------------------------------------
//forward declarations
enum QtMsgType;
class QMessageLogContext;
class QString;

//------------------------------------------------------------------------------
namespace basar
{
namespace gui
{
namespace tie
{

//------------------------------------------------------------------------------
/*!
    * \brief Function: basarQTMessageHandler  writes QTMessages to the Logger
    *        named "basar.gui.tie.QTMessageHandler" 
    */
void LIBBASARGUITIE_API basarQTMessageHandler( QtMsgType t, const QMessageLogContext& context, const QString& logmsg );

//------------------------------------------------------------------------------
}
}
}

//------------------------------------------------------------------------------
#endif 

// ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~  ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~
