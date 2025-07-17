//----------------------------------------------------------------------------
/*! \file
 *  \brief  transformer for CheckBox
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
//! init static constant for check box state checked
//!
ConstString CheckBoxTransformer::s_STATEON   = "TRANSFORMER_ON";

//! init static constant for check box state partially checked
//!
ConstString CheckBoxTransformer::s_STATEPART = "TRANSFORMER_PART";

//! init static constant for check box state unchecked
//!
ConstString CheckBoxTransformer::s_STATEOFF  = "TRANSFORMER_OFF";

//----------------------------------------------------------------------------
//! \throw  no-throw
//! 
CheckBoxTransformer::CheckBoxTransformer ( const VarString & accPropTblCol,
		                                   const VarString & rOn  ,
										   const VarString & rOff ,
										   EvaluateEnum      eEval /* = EVAL_STATE_OFF */)
										 : m_TriState     (false),
										   m_eEvalMethod  (eEval),
										   m_AccPropTblCol(accPropTblCol),
										   m_GuiPropTblCol(accPropTblCol),
										   m_On		      (rOn),
										   m_Part	      (""),
										   m_Off          (rOff)

{
}

//----------------------------------------------------------------------------
//! \throw  no-throw
//! 
CheckBoxTransformer::CheckBoxTransformer ( const VarString & guiPropTblCol,
		                                   const VarString & rOn  ,
										   const VarString & rOff ,
										   EvaluateEnum      eEval,
										   const VarString & accPropTblCol)
										 : m_TriState     (false),
										   m_eEvalMethod  (eEval),
										   m_AccPropTblCol(accPropTblCol),
										   m_GuiPropTblCol(guiPropTblCol),
										   m_On		      (rOn),
										   m_Part	      (""),
										   m_Off          (rOff)

{
}

//----------------------------------------------------------------------------
//! \throw  no-throw
//! 
CheckBoxTransformer::CheckBoxTransformer ( const VarString & accPropTblCol,
		                                   const VarString & rOn  ,
                                           const VarString & rPart,
										   const VarString & rOff ,
										   EvaluateEnum      eEval /* = EVAL_STATE_OFF */)
										 : m_TriState     (true),
										   m_eEvalMethod  (eEval),
										   m_AccPropTblCol(accPropTblCol),
										   m_GuiPropTblCol(accPropTblCol),
										   m_On		      (rOn),
										   m_Part	      (rPart),
										   m_Off          (rOff)
{
}

//----------------------------------------------------------------------------
//! \throw  no-throw
//! 
CheckBoxTransformer::CheckBoxTransformer ( const VarString & guiPropTblCol,
		                                   const VarString & rOn  ,
                                           const VarString & rPart,
										   const VarString & rOff ,
										   EvaluateEnum      eEval,
										   const VarString & accPropTblCol)
										 : m_TriState     (true),
										   m_eEvalMethod  (eEval),
										   m_AccPropTblCol(accPropTblCol),
										   m_GuiPropTblCol(guiPropTblCol),
										   m_On		   (rOn  ),
										   m_Part	   (rPart),
										   m_Off       (rOff )
{
}

//----------------------------------------------------------------------------
//! \throw  no-throw
//! 
CheckBoxTransformer::~CheckBoxTransformer() 
{
}

//----------------------------------------------------------------------------
//! \throw  UnknownPropertyTypeException
//! 
void CheckBoxTransformer::RightToLeft(GuiPropertyTable_YIterator guiIt,
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
		ExceptInfo sInfo("basar.gui.tie.CheckBoxTransformer.RightToLeft()",
				         msg, 
						 __FILE__, __LINE__);
		throw UnknownPropertyTypeException(sInfo);
	}

	VarString  leftVal;

	switch (m_eEvalMethod)
	{
	case EVAL_STATE_ON:
		if (rightVal == m_Off)
			leftVal = s_STATEOFF;
		else
		{
			if ( m_TriState && (rightVal == m_Part) )	// regard condition order!
				leftVal = s_STATEPART;
			else
				leftVal = s_STATEON;
		}

		break;

	case EVAL_STATE_PART:
		if (rightVal == m_Off)
			leftVal = s_STATEOFF;
		else
		{
			if ( !m_TriState && (rightVal == m_On) )	// regard condition order!
				leftVal = s_STATEON;
			else
				leftVal = s_STATEPART;
		}

		break;

	case EVAL_STATE_OFF:
	default:
		if (m_TriState && (rightVal == m_Part))
			leftVal = s_STATEPART;
		else
		{
			if ( rightVal == m_On )	// regard condition order!
				leftVal = s_STATEON;
			else
				leftVal = s_STATEOFF;
		}

		break;
	}

	guiIt.setString(m_GuiPropTblCol, leftVal);

	if (LoggerPool::transformer().isEnabledFor(log4cplus::DEBUG_LOG_LEVEL))
	{
		VarString msg;
		msg.format("CheckBoxTransformer: accessor property <%s> value <%s> transformed to "
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
//! \throw  TypecastFailedException
//! 
void CheckBoxTransformer::LeftToRight(const GuiPropertyTable_YIterator guiIt,
									  AccPropTblYIterator              accIt )
{
	VarString  leftVal;
	I18nString rightVal = m_Off;

	if (guiIt.isValueExist(m_GuiPropTblCol))
	{	
		leftVal = guiIt.getString(m_GuiPropTblCol);

		if (s_STATEON == leftVal)
			rightVal = m_On;
		else
		{
			if ( m_TriState && (s_STATEPART == leftVal) )
				rightVal = m_Part;
		}
	}

	VarString              errorMsg;
	property::PropertyType accType = accIt.getType(m_AccPropTblCol);

	if (!Helper::setStringToAcc(accIt, accIt.getIndex(m_AccPropTblCol), rightVal, accType, errorMsg))
	{
		if (errorMsg != "")
		{
			ExceptInfo sInfo("basar.gui.tie.CheckBoxTransformer.LeftToRight()", 
				             errorMsg, 
							 __FILE__, __LINE__);
			throw TypecastFailedException(sInfo);
		}
	}

	if (LoggerPool::transformer().isEnabledFor(log4cplus::DEBUG_LOG_LEVEL))
	{
		VarString msg;
		msg.format("CheckBoxTransformer: gui property <%s> value <%s> transformed to "
			       "accessor property <%s> value <%s>",
				    m_GuiPropTblCol.c_str(),
					leftVal.c_str(),
					m_AccPropTblCol.c_str(), 
					rightVal.c_str());
		LoggerPool::transformer().forcedLog(log4cplus::DEBUG_LOG_LEVEL, 
			                                    msg.c_str(), 
												__FILE__, __LINE__);
	}
}

//----------------------------------------------------------------------------
//! \throw  no-throw
//! \return pointer to created transformer
Transformer::TransformSharedPtr CheckBoxTransformer::create()
{
	return TransformSharedPtr (new CheckBoxTransformer(*this));
}

//----------------------------------------------------------------------------
} // tie
} // gui
} // basar
