//----------------------------------------------------------------------------
/*! \file
 *  \brief  transformer for basar::DateTime type
 *  \author Michael Eichenlaub
 *  \date   03.05.2010
 */
//----------------------------------------------------------------------------
#include "libbasarguitie_transformer.h"

#pragma warning (push)
#pragma warning (disable: 4512)
#include <QtCore/QDateTime>
#pragma warning (pop)

#include "loggerpool.h"
#include "libbasarguitie_propertytable.h"
#include "libbasardbaspect_accessorpropertytable_yiterator.h"

//----------------------------------------------------------------------------
namespace basar {
namespace gui {
namespace tie {

//----------------------------------------------------------------------------
/*! \throw  no-throw */
DateTimeTransformer::~DateTimeTransformer() 
{
}

//----------------------------------------------------------------------------
/*! \throw  no-throw */
DateTimeTransformer::DateTimeTransformer( const VarString & accPropTblCol,
									      const VarString & format	     )
									    : m_AccPropTblCol (accPropTblCol),
									      m_GuiPropTblCol (accPropTblCol),
									      m_Format        (format       )
{
}

//----------------------------------------------------------------------------
/*! \throw  no-throw */
DateTimeTransformer::DateTimeTransformer( const VarString & accPropTblCol,
										  const VarString & guiPropTblCol,
									      const VarString & format	     )
									    : m_AccPropTblCol (accPropTblCol),
									      m_GuiPropTblCol (guiPropTblCol),
									      m_Format        (format       )
{
}

//----------------------------------------------------------------------------
/*! \throw	no-throw */
void DateTimeTransformer::RightToLeft (GuiPropertyTable_YIterator guiIt,
									  const AccPropTblYIterator  accIt)
{
	DateTime   dt(accIt.getDateTime(m_AccPropTblCol));
	I18nString dtStr;

	if (m_Format.empty())
		dtStr = dt.toStrDateTime();
	else
	{
		QDateTime qdt(QDate(dt.getYear (), 
			                dt.getMonth(), 
							dt.getDay  ()),
			          QTime(dt.getHour  (),
							dt.getMinute(),
							dt.getSec   (),
							dt.getMSec  ()));
		dtStr = qdt.toString(QString::fromLatin1(m_Format.c_str())).toStdString();
	}

	guiIt.setString(m_GuiPropTblCol, dtStr);

	if (LoggerPool::transformer().isEnabledFor(log4cplus::DEBUG_LOG_LEVEL))
	{
		VarString msg;
		msg.format("DateTimeTransformer: accessor property <%s> value <%s> transformed to "
			       "gui property <%s> value <%s>",
				    m_AccPropTblCol.c_str(),
					accIt.getDateTime(m_AccPropTblCol).toStrDateTime().c_str(),
					m_GuiPropTblCol.c_str(), 
					dtStr.c_str());
		LoggerPool::transformer().forcedLog(log4cplus::DEBUG_LOG_LEVEL, 
				                                msg, 
												__FILE__, __LINE__);
	}
}

//----------------------------------------------------------------------------
/*! \throw  no-throw */ 
void DateTimeTransformer::LeftToRight(const GuiPropertyTable_YIterator guiIt,
									 AccPropTblYIterator              accIt)
{
	DateTime   dt;
	I18nString dtStr = guiIt.getString(m_GuiPropTblCol);

	if (m_Format.empty())
		dt.fromStrDateTime(dtStr);
	else
	{
		QDateTime qdt = QDateTime::fromString(QString::fromLocal8Bit(dtStr.c_str()), QString::fromLatin1(m_Format.c_str()));
		QDate     qd  = qdt.date();
		QTime     qt  = qdt.time();
		dt.setDate(qd.year (), 
			       qd.month(), 
				   qd.day  ());
		dt.setTime(qt.hour  (),
			       qt.minute(),
				   qt.second(),
				   qt.msec  ());
	}

	accIt.setDateTime(m_AccPropTblCol, dt);

	if (LoggerPool::transformer().isEnabledFor(log4cplus::DEBUG_LOG_LEVEL))
	{
		VarString msg;
		msg.format("DateTimeTransformer: gui property <%s> value <%s> transformed to "
			       "accessor property <%s> value <%s>",
				    m_GuiPropTblCol.c_str(),
					dtStr.c_str(),
					m_AccPropTblCol.c_str(), 
					dt.toStrDateTime().c_str());
		LoggerPool::transformer().forcedLog(log4cplus::DEBUG_LOG_LEVEL, 
				                                msg, 
												__FILE__, __LINE__);
	}
}

//----------------------------------------------------------------------------
/*! \throw  no-throw
	\return pointer to created transformer */
Transformer::TransformSharedPtr DateTimeTransformer::create()
{
	return TransformSharedPtr (new DateTimeTransformer(*this));
}

//----------------------------------------------------------------------------
} // tie
} // gui
} // basar
