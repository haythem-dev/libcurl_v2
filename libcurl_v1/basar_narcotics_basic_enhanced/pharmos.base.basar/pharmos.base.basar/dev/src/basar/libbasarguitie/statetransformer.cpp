//----------------------------------------------------------------------------
/*! \file
 *  \brief  transformer for mapping between DB state flags and display values
 *  \author Michael Eichenlaub
 *  \date   15.04.2010
 */
//----------------------------------------------------------------------------
#include "libbasarguitie_transformer.h"

#include "loggerpool.h"
#include "libbasarproperty_propertytype.h"
#include "helper.h"
#include "libbasarguitie_propertytable.h"
#include "libbasarguitie_exceptions.h"
#include "libbasardbaspect_accessorpropertytable_yiterator.h"

//----------------------------------------------------------------------------
namespace basar {
namespace gui {
namespace tie {

//----------------------------------------------------------------------------
/*! \throw  no-throw */
StateTransformer::~StateTransformer() 
{
}

//----------------------------------------------------------------------------
/*! \throw  no-throw */
StateTransformer::StateTransformer (const ParamList & valMap,
									const VarString & accPropTblCol,
									const VarString & accPropTblValDef,	
								    const VarString & guiPropTblValDef)
								   : m_ValMap          (valMap),
								     m_AccPropTblCol   (accPropTblCol),
									 m_GuiPropTblCol   (accPropTblCol),
									 m_AccPropTblValDef(accPropTblValDef),
									 m_GuiPropTblValDef(guiPropTblValDef)
{
}

//----------------------------------------------------------------------------
/*! \throw  no-throw */
StateTransformer::StateTransformer (const ParamList & valMap,
									const VarString & accPropTblCol,
									const VarString & guiPropTblCol,
									const VarString & accPropTblValDef,	
								    const VarString & guiPropTblValDef)
								   : m_ValMap          (valMap),
								     m_AccPropTblCol   (accPropTblCol),
								     m_GuiPropTblCol   (guiPropTblCol),
									 m_AccPropTblValDef(accPropTblValDef),
									 m_GuiPropTblValDef(guiPropTblValDef)
{
}

//----------------------------------------------------------------------------
/*! \throw  UnknownPropertyTypeException */
void StateTransformer::RightToLeft (GuiPropertyTable_YIterator guiIt,
									const AccPropTblYIterator  accIt)
{
	I18nString             rightVal; 
	property::PropertyType accType = accIt.getType(m_AccPropTblCol);

	if (! Helper::getStringFromAcc(accIt, accIt.getIndex(m_AccPropTblCol), rightVal, accType))
	{
		if (rightVal == "")
			return;

		basar::VarString msg;
		msg.format("accessor type <%d> for getting string of property <%s> is unknown.", 
				    accType, 
					m_AccPropTblCol.c_str());
		ExceptInfo sInfo("basar.gui.tie.StateTransformer.RightToLeft()",
				         msg, 
						 __FILE__, __LINE__);
		throw UnknownPropertyTypeException(sInfo);
	}

	I18nString leftVal = m_ValMap.isKeyContained(rightVal) 
		               ? m_ValMap.getValue(rightVal)       
					   : m_GuiPropTblValDef;

	guiIt.setString(m_GuiPropTblCol, leftVal);

	if (LoggerPool::transformer().isEnabledFor(log4cplus::DEBUG_LOG_LEVEL))
	{
		VarString msg;
		msg.format("StateTransformer: accessor property <%s> value <%s> transformed to "
			       "gui property <%s> value <%s>",
				    m_AccPropTblCol.c_str(),
					rightVal.c_str(),
					m_GuiPropTblCol.c_str(), 
					leftVal.c_str());
		LoggerPool::transformer().forcedLog(log4cplus::DEBUG_LOG_LEVEL, 
				                                msg, 
												__FILE__, __LINE__);
	}
}

//----------------------------------------------------------------------------
/*! \throw  TypecastFailedException */ 
void StateTransformer::LeftToRight(const GuiPropertyTable_YIterator guiIt,
								   AccPropTblYIterator              accIt)
{
	I18nString leftVal;

	if (guiIt.isValueExist(m_GuiPropTblCol))
		leftVal = guiIt.getString(m_GuiPropTblCol);

	I18nString rightVal = m_AccPropTblValDef; 

	for (cmnutil::ParameterList_Iterator it = m_ValMap.begin(); 
		 it != m_ValMap.end(); 
		 ++it)
	{
		if (it->second == leftVal)
		{
			rightVal = it->first;
			break;
		}
	}

	VarString              errorMsg;
	property::PropertyType accType = accIt.getType(m_AccPropTblCol);

	if (!Helper::setStringToAcc(accIt, 
		                        accIt.getIndex(m_AccPropTblCol), 
								rightVal, 
								accType, 
								errorMsg))
	{
		if (errorMsg != "")
		{
			ExceptInfo sInfo("basar.gui.tie.StateTransformer.LeftToRight()", 
				             errorMsg, 
							 __FILE__, __LINE__);
			throw TypecastFailedException(sInfo);
		}
	}

	if (LoggerPool::transformer().isEnabledFor(log4cplus::DEBUG_LOG_LEVEL))
	{
		VarString msg;
		msg.format("StateTransformer: gui property <%s> value <%s> transformed to "
			       "accessor property <%s> value <%s>",
				    m_GuiPropTblCol.c_str(),
					leftVal.c_str(),
					m_AccPropTblCol.c_str(), 
					rightVal.c_str());
		LoggerPool::transformer().forcedLog(log4cplus::DEBUG_LOG_LEVEL, 
				                                msg, 
												__FILE__, __LINE__);
	}
}

//----------------------------------------------------------------------------
/*! \throw  no-throw
	\return pointer to created transformer */
Transformer::TransformSharedPtr StateTransformer::create()
{
	return TransformSharedPtr (new StateTransformer(*this));
}

//----------------------------------------------------------------------------
} // tie
} // gui
} // basar
