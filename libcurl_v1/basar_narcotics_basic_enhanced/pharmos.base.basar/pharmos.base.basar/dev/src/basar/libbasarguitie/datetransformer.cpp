//----------------------------------------------------------------------------
/*! \file
 *  \brief  transformer for basar::Date type
 *  \author Michael Eichenlaub
 *  \date   03.05.2010
 */
//----------------------------------------------------------------------------

#include "libbasarguitie_transformer.h"

#pragma warning (push)
#pragma warning (disable: 4512)
#include <QtCore/QDate>
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
DateTransformer::~DateTransformer() 
{
}

//----------------------------------------------------------------------------
/*! \throw  no-throw */
DateTransformer::DateTransformer( const VarString & accPropTblCol,
								  const VarString & format	     )
								: m_AccPropTblCol (accPropTblCol),
								  m_GuiPropTblCol (accPropTblCol),
								  m_Format        (format       )
{
}

//----------------------------------------------------------------------------
/*! \throw  no-throw */
DateTransformer::DateTransformer( const VarString & accPropTblCol,
								  const VarString & guiPropTblCol,
								  const VarString & format	     )
								: m_AccPropTblCol (accPropTblCol),
								  m_GuiPropTblCol (guiPropTblCol),
								  m_Format        (format       )
{
}

//----------------------------------------------------------------------------
/*! \throw	no-throw */
void DateTransformer::RightToLeft (GuiPropertyTable_YIterator guiIt,
								   const AccPropTblYIterator  accIt)
{
	Date       dt(accIt.getDate(m_AccPropTblCol));
	I18nString dtStr;

	if (m_Format.empty())
		dtStr = dt.toStrDate();
	else
	{
		QDate qdt(dt.getYear(), dt.getMonth(), dt.getDay());
		dtStr = qdt.toString(QString::fromLatin1(m_Format.c_str())).toStdString();
	}

	guiIt.setString(m_GuiPropTblCol, dtStr);

	if (LoggerPool::transformer().isEnabledFor(log4cplus::DEBUG_LOG_LEVEL))
	{
		VarString msg;
		msg.format("DateTransformer: accessor property <%s> value <%s> transformed to "
			       "gui property <%s> value <%s>",
				    m_AccPropTblCol.c_str(),
					accIt.getDate(m_AccPropTblCol).toStrDate().c_str(),
					m_GuiPropTblCol.c_str(), 
					dtStr.c_str());
		LoggerPool::transformer().forcedLog(log4cplus::DEBUG_LOG_LEVEL, 
				                                msg, 
												__FILE__, __LINE__);
	}
}

//----------------------------------------------------------------------------
/*! \throw  no-throw */ 
void DateTransformer::LeftToRight(const GuiPropertyTable_YIterator guiIt,
								  AccPropTblYIterator              accIt)
{
	Date       dt;
	I18nString dtStr = guiIt.getString(m_GuiPropTblCol);

	if (m_Format.empty())
		dt.fromStrDate(dtStr);
	else
	{
		QDate qdt = QDate::fromString(QString::fromLocal8Bit(dtStr.c_str()), QString::fromLatin1(m_Format.c_str()));
		dt = Date(qdt.year(), qdt.month(), qdt.day());
	}

	accIt.setDate(m_AccPropTblCol, dt);

	if (LoggerPool::transformer().isEnabledFor(log4cplus::DEBUG_LOG_LEVEL))
	{
		VarString msg;
		msg.format("DateTransformer: gui property <%s> value <%s> transformed to "
			       "accessor property <%s> value <%s>",
				    m_GuiPropTblCol.c_str(),
					dtStr.c_str(),
					m_AccPropTblCol.c_str(), 
					dt.toStrDate().c_str());
		LoggerPool::transformer().forcedLog(log4cplus::DEBUG_LOG_LEVEL, 
				                                msg, 
												__FILE__, __LINE__);
	}
}

//----------------------------------------------------------------------------
/*! \throw  no-throw
	\return pointer to created transformer */
Transformer::TransformSharedPtr DateTransformer::create()
{
	return TransformSharedPtr (new DateTransformer(*this));
}

//----------------------------------------------------------------------------
} // tie
} // gui
} // basar
