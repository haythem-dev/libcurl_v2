//----------------------------------------------------------------------------
/*! \file
 *  \brief  transformer for Decimal types
 *  \author Michael Eichenlaub
 *  \date   15.04.2010
 */
//----------------------------------------------------------------------------
#include "libbasarguitie_transformer.h"

#include "loggerpool.h"
#include "libbasarguitie_propertytable.h"
#include "libbasardbaspect_accessorpropertytable_yiterator.h"

//----------------------------------------------------------------------------
namespace basar {
namespace gui {
namespace tie {

//----------------------------------------------------------------------------
/*! \throw  no-throw */
DecimalTransformer::~DecimalTransformer() 
{
}

//----------------------------------------------------------------------------
/*! \throw  no-throw */
DecimalTransformer::DecimalTransformer (const VarString & accPropTblCol,
									    const Int16       decPlaces	   )
									   : m_AccPropTblCol (accPropTblCol),
									     m_GuiPropTblCol (accPropTblCol),
									     m_DecPlaces     (decPlaces    )
{
}

//----------------------------------------------------------------------------
/*! \throw  no-throw */
DecimalTransformer::DecimalTransformer (const VarString & accPropTblCol,
										const VarString & guiPropTblCol,
									    const Int16       decPlaces	   )
									   : m_AccPropTblCol (accPropTblCol),
									     m_GuiPropTblCol (guiPropTblCol),
									     m_DecPlaces     (decPlaces    )
{
}

//----------------------------------------------------------------------------
/*! \throw	no-throw */
void DecimalTransformer::RightToLeft (GuiPropertyTable_YIterator guiIt,
									  const AccPropTblYIterator  accIt)
{
	guiIt.setString(m_GuiPropTblCol, 
		            accIt.getDecimal(m_AccPropTblCol).toString(m_DecPlaces));

	if (LoggerPool::transformer().isEnabledFor(log4cplus::DEBUG_LOG_LEVEL))
	{
		VarString msg;
		msg.format("DecimalTransformer: accessor property <%s> value <%s> transformed to "
			       "gui property <%s> value <%s>",
				    m_AccPropTblCol.c_str(),
					accIt.getDecimal(m_AccPropTblCol).toString().c_str(),
					m_GuiPropTblCol.c_str(), 
					accIt.getDecimal(m_AccPropTblCol).toString(m_DecPlaces).c_str());
		LoggerPool::transformer().forcedLog(log4cplus::DEBUG_LOG_LEVEL, 
				                                msg, 
												__FILE__, __LINE__);
	}
}

//----------------------------------------------------------------------------
/*! \throw  no-throw */ 
void DecimalTransformer::LeftToRight(const GuiPropertyTable_YIterator guiIt,
									 AccPropTblYIterator              accIt)
{
	Decimal dec(guiIt.getString(m_GuiPropTblCol));
	accIt.setDecimal(m_AccPropTblCol, dec);

	if (LoggerPool::transformer().isEnabledFor(log4cplus::DEBUG_LOG_LEVEL))
	{
		VarString msg;
		msg.format("DecimalTransformer: gui property <%s> value <%s> transformed to "
			       "accessor property <%s> value <%s>",
				    m_GuiPropTblCol.c_str(),
					guiIt.getString(m_GuiPropTblCol).c_str(),
					m_AccPropTblCol.c_str(), 
					dec.toString(m_DecPlaces).c_str());
		LoggerPool::transformer().forcedLog(log4cplus::DEBUG_LOG_LEVEL, 
				                                msg, 
												__FILE__, __LINE__);
	}
}

//----------------------------------------------------------------------------
/*! \throw  no-throw
	\return pointer to created transformer */
Transformer::TransformSharedPtr DecimalTransformer::create()
{
	return TransformSharedPtr (new DecimalTransformer(*this));
}

//----------------------------------------------------------------------------
} // tie
} // gui
} // basar
