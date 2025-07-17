//------------------------------------------------------------------------------
/*! \file
 *  \brief  common header files include
 *  \author Thomas Hörath
 *  \date   10.02.2006
 */
//------------------------------------------------------------------------------

#ifndef GUARD_NARCOTICS_COMMONHEADER_H
#define GUARD_NARCOTICS_COMMONHEADER_H

//------------------------------------------------------------------------------
// displays stage of precompiling, so not fulfilled precompile conditions can be detected
#ifdef WIN32
	#ifndef PRECOMPILEDHDR__MESSAGE__COMMONHEADER_H
		#pragma message("    precompiling " __FILE__ " ...")
		#define PRECOMPILEDHDR__MESSAGE__COMMONHEADER_H
	#endif	
#endif

//------------------------------------------------------------------------------
//includes
//------------------------------------------------------------------------------

#include <windows.h>
#include <shellapi.h>
#include <sstream>
#include <iomanip>

#include <libbasarcmnutil.h>
#include <libbasardbsql.h>
#include <libbasarproperty.h>
#include <libbasardbaspect.h>
#include <libbasarappl.h>
#include <libbasarguitie.h>
#include <libbasarguiqt.h>
#include <libbasarlogin.h>

#ifdef WIN32
	#pragma warning (push)
	#pragma warning(disable: 4127 4244 4311 4312 4481 4512 4800)
#endif
	#include <QtWidgets/QApplication>
	#include <QtGui/QIntValidator>
	#include <QtWidgets/QGridLayout>
	#include <QtGui/QKeyEvent>
	#include <QtWidgets/QLineEdit>
	#include <QtWidgets/QComboBox>	
	#include <QtGui/QFocusEvent>
	#include <QtWidgets/QMessageBox>
	#include <QtWidgets/QPushButton>
	
	#include <QtCore/QEvent>
	#include <QtCore/QDir>
	#include <QtCore/QDirIterator>
	#include <QtCore/QTimer>
	#include <QtCore/QDate>
	#include <QtCore/QMutex>
	#include <QtCore/QMutexLocker>
	#include <QtCore/QObject>
#ifdef WIN32
	#pragma warning (pop)
#endif


#include "definitions.h"
#include "constants.h"

//------------------------------------------------------------------------------

#define PROCESS_EVENT_RETURN( result, parentWidget )  \
            if( basar::appl::HANDLER_NOT_FOUND == result.ret || basar::appl::HANDLER_INFO == result.ret ){ \
                basar::gui::tie::infoMsgBox( parentWidget, result.message.c_str() );                       \
            } else if( basar::appl::HANDLER_ERROR == result.ret ){                                         \
                basar::gui::tie::criticalMsgBox( parentWidget, result.message.c_str() );                   \
            }

#ifndef NDEBUG
#define LOG_YIT( fun, logger, yit )                                                                         \
            {                                                                                                   \
                std::stringstream ss; ss << fun << ": ";                                                        \
                if( yit.isNull() ){                                                                             \
                    ss << "YIterator is Null()!";                                                               \
                } else if( yit.isEnd() ){                                                                       \
                    ss << "YIterator is End()!";                                                                \
                } else {                                                                                        \
                    ss << std::endl;                                                                            \
                    basar::db::aspect::AccessorPropertyTableRef propTab = yit.getPropertyTable();               \
                    basar::db::aspect::PropertyDescriptionListRef descriptList =                                \
                        propTab.getPropertyDescriptionList();                                                   \
                    basar::db::aspect::AccessorPropertyTable_XIterator xit = yit.begin();                       \
                    basar::VarString value = "";                                                               \
                    for( int i = 1; !xit.isEnd(); ++xit, ++i ){                                                 \
                        value = "";                                                                             \
                        if( basar::SS_UNSET != xit.getState().getState() ){                                     \
                            value = xit.getSQLString();                                                         \
                        }                                                                                       \
                        ss << std::left << std::setw(2) << i << ". " << xit.getName() << "(>" << value << "<, " \
                           << xit.getState().toString() << ")" << std::endl;                                    \
                    }                                                                                           \
                }                                                                                               \
                BLOG_DEBUG( logger, ss.str().c_str() );                                                         \
            }
    #define LOG_PROPTAB( propTab, logger, fun )                            \
            {                                                              \
                if( propTab.isNull() ){                                    \
                    std::stringstream ss; ss << fun << ": ";               \
                    ss << "PropertyTable is Null()!";                      \
                    BLOG_DEBUG( logger, ss.str().c_str() );                \
                } else {                                                   \
                    basar::db::aspect::AccessorPropertyTable_YIterator yit = propTab.begin(); \
                    for(; !yit.isEnd(); ++yit ){                           \
                        LOG_YIT( yit, logger, fun )                        \
                    }                                                      \
                }                                                          \
            }
#else
    #define LOG_YIT( fun, logger, yit )
    #define LOG_PROPTAB( propTab, logger, fun )
#endif

/*----------------------------------------------------------------------------*/
namespace narcotics
{
	/*! \brief return readable mapped transaction type
		\n no-throw */
	basar::VarString	transactionType	( basar::VarString type		//!< transaction type of ksc ( a-c, x-z )
								);

	/*! \brief return readable mapped changing type (from protocol)
		\n no-throw */
	basar::VarString	changingType	( basar::VarString type		//!< changing type of ksc ( u / r / d )
								);

	/*! \brief get pack unit + unit extension out of unity (packageunit)
		\n no-throw */
	void				splitUnity		( basar::VarString unity,		//!< unity (packageunit) which will be splitted
										  basar::VarString& rPack,		//!< ref containing pack unit at the end
										  basar::VarString& rExtension	//!< ref containing unit extension at the end
								);

	basar::I18nString	removeLeadingZeros( basar::I18nString originalVal );

	QDate				basarDateToQDate( const basar::Date& date );

} //namespace narcotics

#endif //GUARD_NARCOTICS_COMMONHEADER_H