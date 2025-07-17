//----------------------------------------------------------------------------
/*! \file
 *  \brief  transformer for RadioButton
 *  \author Michael Eichenlaub
 *  \date   18.11.2005
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
//! init static constant for radio button state on
//!
ConstString RadioButtonTransformer::s_STATEON   = "TRANSFORMER_ON";

//! init static constant for radio button state off
//!
ConstString RadioButtonTransformer::s_STATEOFF  = "TRANSFORMER_OFF";

//----------------------------------------------------------------------------
/*! \throw  no-throw */
RadioButtonTransformer::~RadioButtonTransformer() 
{
}
	 
//----------------------------------------------------------------------------
//! \throw  no-throw
//! 
RadioButtonTransformer::RadioButtonTransformer( const ParamList & rCorValRes   ,
											    const VarString & accPropTblCol)
											  : m_AccPropTblCol(accPropTblCol),
											    m_CorValRes    (rCorValRes)
{
}

//----------------------------------------------------------------------------
//! \throw  UnknownPropertyTypeException
//! 
void RadioButtonTransformer::RightToLeft(GuiPropertyTable_YIterator guiIt,
										 const AccPropTblYIterator  accIt)
{
	I18nString                    rightVal; 
	basar::property::PropertyType accType = accIt.getType(m_AccPropTblCol);

	if (!Helper::getStringFromAcc(accIt, accIt.getIndex(m_AccPropTblCol), rightVal, accType))
	{
		if (rightVal == "")
			return;

		basar::VarString msg;
		msg.format("accessor type <%d> for getting string of property <%s> is unknown", 
			       accType, 
				   m_AccPropTblCol.c_str());
		ExceptInfo sInfo("basar.gui.tie.RadioButtonTransformer.RightToLeft()", 
			             msg, __FILE__, __LINE__);
		throw UnknownPropertyTypeException(sInfo);
	}


	// reset all defined columns to off state
	for (cmnutil::ParameterList_Iterator it = m_CorValRes.begin(); 
		 it != m_CorValRes.end(); 
		 ++it)
	{
		guiIt.setString(it->second, s_STATEOFF);
	}

	I18nString leftVal = m_CorValRes.getValue(rightVal);
	guiIt.setString(leftVal, s_STATEON);

	if (LoggerPool::transformer().isEnabledFor(log4cplus::DEBUG_LOG_LEVEL))
	{
		VarString msg;
		msg.format("RadioButtonTransformer: accessor property <%s> value <%s> transformed to "
			       "gui property <%s> value <%s>",
				    m_AccPropTblCol.c_str(),
					rightVal.c_str(),
					leftVal.c_str(), 
					s_STATEON);
		LoggerPool::transformer().forcedLog(log4cplus::DEBUG_LOG_LEVEL, msg, __FILE__, __LINE__);
	}
}

//----------------------------------------------------------------------------
//! \throw  TypecastFailedException
//! 
void RadioButtonTransformer::LeftToRight(const GuiPropertyTable_YIterator guiIt,
										 AccPropTblYIterator              accIt )
{
	VarString                       errorMsg;
	property::PropertyType          accType;
	cmnutil::ParameterList_Iterator it;

	for (it = m_CorValRes.begin(); it != m_CorValRes.end(); ++it)
	{
		if (guiIt.isValueExist(it->second))
		{	
			if (guiIt.getString(it->second) == s_STATEON)
			{
				accType = accIt.getType(m_AccPropTblCol);

				if (!Helper::setStringToAcc(accIt, accIt.getIndex(m_AccPropTblCol), it->first, accType, errorMsg))
				{
					if (errorMsg != "")
					{
						ExceptInfo sInfo("basar.gui.tie.RadioButtonTransformer.LeftToRight()", 
							             errorMsg, 
										 __FILE__, __LINE__);
						throw TypecastFailedException(sInfo);
					}
				}

				break;
			}
		}
	}

	if (LoggerPool::transformer().isEnabledFor(log4cplus::DEBUG_LOG_LEVEL))
	{
		VarString msg;
		msg.format("RadioButtonTransformer: gui property <%s> value <%s> transformed to "
			       "accessor property <%s> value <%s>",
				    it->second.c_str(),
					s_STATEON,
					m_AccPropTblCol.c_str(), 
					it->first.c_str());
		LoggerPool::transformer().forcedLog(log4cplus::DEBUG_LOG_LEVEL, 
			                                    msg,
												__FILE__, __LINE__);
	}
}

//----------------------------------------------------------------------------
//! \throw  no-throw
//! \return pointer to created transformer
Transformer::TransformSharedPtr RadioButtonTransformer::create()
{
	return TransformSharedPtr (new RadioButtonTransformer(*this));
}

//----------------------------------------------------------------------------
} // tie
} // gui
} // basar
