//----------------------------------------------------------------------------
/*! \file
 *  \brief  transformer for basar::Time type
 *  \author Michael Eichenlaub
 *  \date   03.05.2010
 */
//----------------------------------------------------------------------------
#include "libbasarguitie_transformer.h"

#ifdef _WIN32
#pragma warning (push)
#pragma warning (disable: 4512) //'QtPrivate::QSlotObjectBase' : assignment operator could not be generated
#endif
#include <QtCore/QTime>
#ifdef _WIN32
#pragma warning (pop)
#endif

#include "loggerpool.h"
#include "libbasarguitie_propertytable.h"
#include "libbasardbaspect_accessorpropertytable_yiterator.h"

//----------------------------------------------------------------------------
namespace basar {
namespace gui {
namespace tie {

//----------------------------------------------------------------------------
/*! \throw  no-throw */
TimeTransformer::~TimeTransformer() 
{
}

//----------------------------------------------------------------------------
/*! \throw  no-throw */
TimeTransformer::TimeTransformer( const VarString & accPropTblCol,
								  const VarString & format	     )
								: m_AccPropTblCol (accPropTblCol),
								  m_GuiPropTblCol (accPropTblCol),
								  m_Format        (format       )
{
}

//----------------------------------------------------------------------------
/*! \throw  no-throw */
TimeTransformer::TimeTransformer( const VarString & accPropTblCol,
								  const VarString & guiPropTblCol,
								  const VarString & format	     )
								: m_AccPropTblCol (accPropTblCol),
								  m_GuiPropTblCol (guiPropTblCol),
								  m_Format        (format       )
{
}

//----------------------------------------------------------------------------
/*! \throw	no-throw */
void TimeTransformer::RightToLeft (GuiPropertyTable_YIterator guiIt,
								   const AccPropTblYIterator  accIt)
{
	Time   dt(accIt.getTime(m_AccPropTblCol));
	I18nString dtStr;

	if (m_Format.empty())
		dtStr = dt.toStrTime();
	else
	{
		QTime qdt(QTime(dt.getHour  (),
						dt.getMinute(),
						dt.getSec   (),
						dt.getMSec  ()));

		dtStr = qdt.toString( QString::fromLatin1(m_Format.c_str()) ).toStdString();
	}

	guiIt.setString(m_GuiPropTblCol, dtStr);

	if (LoggerPool::transformer().isEnabledFor(log4cplus::DEBUG_LOG_LEVEL))
	{
		VarString msg;
		msg.format("TimeTransformer: accessor property <%s> value <%s> transformed to "
			       "gui property <%s> value <%s>",
				    m_AccPropTblCol.c_str(),
					accIt.getTime(m_AccPropTblCol).toStrTime().c_str(),
					m_GuiPropTblCol.c_str(), 
					dtStr.c_str());
		LoggerPool::transformer().forcedLog(log4cplus::DEBUG_LOG_LEVEL, 
				                                msg, 
												__FILE__, __LINE__);
	}
}

//----------------------------------------------------------------------------
/*! \throw  no-throw */ 
void TimeTransformer::LeftToRight(const GuiPropertyTable_YIterator guiIt,
								  AccPropTblYIterator              accIt)
{
	Time       dt;
	I18nString dtStr = guiIt.getString(m_GuiPropTblCol);

	if (m_Format.empty())
		dt.fromStrTime(dtStr);
	else
	{

		QTime qdt = QTime::fromString( QString::fromLatin1(dtStr.c_str()), QString::fromLatin1(m_Format.c_str()) );
		dt.setInvalidDate();
		dt.setTime       (qdt.hour  (),
			              qdt.minute(),
				          qdt.second(),
				          qdt.msec  ());
	}

	accIt.setTime(m_AccPropTblCol, dt);

	if (LoggerPool::transformer().isEnabledFor(log4cplus::DEBUG_LOG_LEVEL))
	{
		VarString msg;
		msg.format("TimeTransformer: gui property <%s> value <%s> transformed to "
			       "accessor property <%s> value <%s>",
				    m_GuiPropTblCol.c_str(),
					dtStr.c_str(),
					m_AccPropTblCol.c_str(), 
					dt.toStrTime().c_str());
		LoggerPool::transformer().forcedLog(log4cplus::DEBUG_LOG_LEVEL, 
				                                msg, 
												__FILE__, __LINE__);
	}
}

//----------------------------------------------------------------------------
/*! \throw  no-throw
	\return pointer to created transformer */
Transformer::TransformSharedPtr TimeTransformer::create()
{
	return TransformSharedPtr (new TimeTransformer(*this));
}

//----------------------------------------------------------------------------
} // tie
} // gui
} // basar
