//----------------------------------------------------------------------------
/*! \file
 *  \brief  transformer for basar::TimeSpan type
 *  \author Michael Eichenlaub
 *  \date   03.05.2010
 */
//----------------------------------------------------------------------------
#include "libbasarguitie_transformer.h"

#pragma warning(push)
#pragma warning(disable: 4512)
#include <QtCore/QDateTime>
#pragma warning(pop)


#include "loggerpool.h"
#include "libbasarguitie_propertytable.h"
#include "libbasardbaspect_accessorpropertytable_yiterator.h"

//----------------------------------------------------------------------------
namespace basar {
namespace gui {
namespace tie {

//----------------------------------------------------------------------------
/*! \throw  no-throw */
TimeSpanTransformer::~TimeSpanTransformer() 
{
}

//----------------------------------------------------------------------------
/*! \throw  no-throw */
TimeSpanTransformer::TimeSpanTransformer( const VarString & accPropTblCol,
									      const VarString & format	     )
									    : m_AccPropTblCol (accPropTblCol),
									      m_GuiPropTblCol (accPropTblCol),
									      m_Format        (format       )
{
}

//----------------------------------------------------------------------------
/*! \throw  no-throw */
TimeSpanTransformer::TimeSpanTransformer( const VarString & accPropTblCol,
										  const VarString & guiPropTblCol,
									      const VarString & format	     )
									    : m_AccPropTblCol (accPropTblCol),
									      m_GuiPropTblCol (guiPropTblCol),
									      m_Format        (format       )
{
}

//----------------------------------------------------------------------------
/*! \throw	no-throw */
void TimeSpanTransformer::RightToLeft (GuiPropertyTable_YIterator guiIt,
									  const AccPropTblYIterator  accIt)
{
	TimeSpan   ts(accIt.getTimeSpan(m_AccPropTblCol));
	I18nString tsStr;

	if (m_Format.empty())
		tsStr = ts.toNormalizedString();
	else
	{
		tsStr = ts.toFormattedString(m_Format.c_str());
	}

	guiIt.setString(m_GuiPropTblCol, tsStr);

	if (LoggerPool::transformer().isEnabledFor(log4cplus::DEBUG_LOG_LEVEL))
	{
		VarString msg;
		msg.format("TimeSpanTransformer: accessor property <%s> value <%s> transformed to "
			       "gui property <%s> value <%s>",
				    m_AccPropTblCol.c_str(),
					accIt.getTimeSpan(m_AccPropTblCol).toNormalizedString().c_str(),
					m_GuiPropTblCol.c_str(), 
					tsStr.c_str());
		LoggerPool::transformer().forcedLog(log4cplus::DEBUG_LOG_LEVEL, 
				                                msg, 
												__FILE__, __LINE__);
	}
}

//----------------------------------------------------------------------------
/*! \throw  no-throw */ 
void TimeSpanTransformer::LeftToRight(const GuiPropertyTable_YIterator guiIt,
									 AccPropTblYIterator              accIt)
{
	TimeSpan   ts;
	I18nString tsStr = guiIt.getString(m_GuiPropTblCol);

	if (m_Format.empty())
		ts.fromNormalizedString(tsStr);
	else
	{
		ts.fromString(m_Format.c_str(), tsStr);
	}

	accIt.setTimeSpan(m_AccPropTblCol, ts);

	if (LoggerPool::transformer().isEnabledFor(log4cplus::DEBUG_LOG_LEVEL))
	{
		VarString msg;
		msg.format("TimeSpanTransformer: gui property <%s> value <%s> transformed to "
			       "accessor property <%s> value <%s>",
				    m_GuiPropTblCol.c_str(),
					tsStr.c_str(),
					m_AccPropTblCol.c_str(), 
					ts.toNormalizedString().c_str());
		LoggerPool::transformer().forcedLog(log4cplus::DEBUG_LOG_LEVEL, 
				                                msg, 
												__FILE__, __LINE__);
	}
}

//----------------------------------------------------------------------------
/*! \throw  no-throw
	\return pointer to created transformer */
Transformer::TransformSharedPtr TimeSpanTransformer::create()
{
	return TransformSharedPtr (new TimeSpanTransformer(*this));
}

//----------------------------------------------------------------------------
} // tie
} // gui
} // basar
