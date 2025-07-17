//----------------------------------------------------------------------------
/*! \file
 *  \brief  transformer for current combobox
 *  \author Roland Kiefert
 *  \date   28.12.2005
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
CurrentListTransformer::~CurrentListTransformer() 
{
}
	 
//----------------------------------------------------------------------------
//! \throw  no-throw
//! 
CurrentListTransformer::CurrentListTransformer (const VarString & guiPropTblCol,	
											    const VarString & accPropTblCol)
											   : m_GuiPropTblCol(guiPropTblCol),
											     m_AccPropTblCol(accPropTblCol)
{
	m_AccPropTbl.reset();
}

//----------------------------------------------------------------------------
//! \throw  no-throw
//! 
CurrentListTransformer::CurrentListTransformer(const VarString   & guiPropTblCol,	
											   const VarString   & accPropTblCol,
											   const VarString   & rKey,
											   const VarString   & rValue,
											   const AccPropTblRef AccPropTable)
											   : m_GuiPropTblCol(guiPropTblCol),
												 m_AccPropTblCol(accPropTblCol),
												 m_KeyProperty  (rKey),
												 m_ValueProperty(rValue),
												 m_AccPropTbl   (AccPropTable)
{
}

//----------------------------------------------------------------------------
//! \throw  UnknownPropertyTypeException
//! 
void CurrentListTransformer::RightToLeft(GuiPropertyTable_YIterator guiIt,
										 const AccPropTblYIterator  accIt)
{
	I18nString             rightVal; 
	property::PropertyType accType = accIt.getType(m_AccPropTblCol);

	if ( !Helper::getStringFromAcc(accIt, accIt.getIndex(m_AccPropTblCol), rightVal, accType))
	{
		if (rightVal.empty())
			return;
		basar::VarString msg;
		msg.format("accessor type <%d> for getting string of property <%s> is unknown.", 
				    accType, 
					m_AccPropTblCol.c_str());
		ExceptInfo sInfo("basar.gui.tie.CurrentListTransformer.RightToLeft()", 
				            msg, __FILE__, __LINE__);
		throw UnknownPropertyTypeException(sInfo);
	}

	I18nString leftVal;

	if (m_AccPropTbl.isNull())
	{
		leftVal = rightVal;
	}
	else
	{
		AccPropTblYIterator    it;
		property::PropertyType accType2;

		for (it = m_AccPropTbl.begin(); it != m_AccPropTbl.end(); ++it)
		{
			accType2 = it.getType(m_KeyProperty);

			if (!Helper::getStringFromAcc(it, it.getIndex(m_KeyProperty), leftVal, accType2))
			{
				if (!(leftVal.empty()))
				{
					basar::VarString msg;
					msg.format("accessor type <%d> for getting string of property <%s> is unknown.", 
						       accType2, 
							   m_KeyProperty.c_str());
					ExceptInfo sInfo("basar.gui.tie.CurrentListTransformer.RightToLeft()", 
						             msg, 
									 __FILE__, __LINE__);
					throw UnknownPropertyTypeException(sInfo);
				}
			}

			if (leftVal == rightVal)
			{
				accType2 = it.getType(m_ValueProperty);

				if (!Helper::getStringFromAcc(it, it.getIndex(m_ValueProperty), leftVal, accType2))
				{
					if (leftVal.empty())
						return;

					basar::VarString msg;
					msg.format("accessor type <%d> for getting string of property <%s> is unknown.", 
						       accType2, 
							   m_ValueProperty.c_str());
					ExceptInfo sInfo("basar.gui.tie.CurrentListTransformer.RightToLeft()", 
						              msg, 
									  __FILE__, __LINE__);
					throw UnknownPropertyTypeException(sInfo);
				}

				break;
			}
		}

		if (it == m_AccPropTbl.end())
			return;
	}

	guiIt.setCurrentString(m_GuiPropTblCol, leftVal);

	if (LoggerPool::transformer().isEnabledFor(log4cplus::DEBUG_LOG_LEVEL))
	{
		VarString msg;
		msg.format("CurrentListTransformer: accessor property <%s> value <%s> transformed to "
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
//! \throw  TypecastFailedException, UnknownPropertyTypeException
//! 
void CurrentListTransformer::LeftToRight(const GuiPropertyTable_YIterator guiIt,
										 AccPropTblYIterator              accIt)
{
	I18nString leftVal  = guiIt.getCurrentString(m_GuiPropTblCol);
	I18nString rightVal;

	if (m_AccPropTbl.isNull())
	{
		rightVal = leftVal;
	}
	else
	{
		AccPropTblYIterator    it;
		property::PropertyType accType;

		for (it = m_AccPropTbl.begin(); it != m_AccPropTbl.end(); ++it)
		{
			accType = it.getType(m_ValueProperty);

			if (!Helper::getStringFromAcc(it, it.getIndex(m_ValueProperty), rightVal, accType))
			{
				if (!(rightVal.empty()))
				{
					basar::VarString msg;
					msg.format("accessor type <%d> for getting string of property <%s> is unknown.", 
						       accType, 
							   m_ValueProperty.c_str());
					ExceptInfo sInfo("basar.gui.tie.CurrentListTransformer.LeftToRight()", 
						             msg, 
									 __FILE__, __LINE__);
					throw UnknownPropertyTypeException(sInfo);
				}
			}

			if (leftVal == rightVal)
			{
				accType = it.getType(m_KeyProperty);

				if (!Helper::getStringFromAcc(it, it.getIndex(m_KeyProperty), rightVal, accType))
				{
					if (rightVal.empty())
						return;

					basar::VarString msg;
					msg.format("accessor type <%d> for getting string of property <%s> is unknown.", 
						       accType, 
							   m_KeyProperty.c_str());
					ExceptInfo sInfo("basar.gui.tie.CurrentListTransformer.LeftToRight()", 
						             msg,
									 __FILE__, __LINE__);
					throw UnknownPropertyTypeException(sInfo);
				}

				break;
			}
		}

		if (it == m_AccPropTbl.end())
			return;
	}

	VarString              errorMsg;
	property::PropertyType accType = accIt.getType(m_AccPropTblCol);

	if (!Helper::setStringToAcc(accIt, accIt.getIndex(m_AccPropTblCol), rightVal, accType, errorMsg))
	{
		if (!(errorMsg.empty()))
		{
			ExceptInfo sInfo("basar.gui.tie.CurrentListTransformer.LeftToRight()", 
				             errorMsg, 
							 __FILE__, __LINE__);
			throw TypecastFailedException(sInfo);
		}
	}

	if (LoggerPool::transformer().isEnabledFor(log4cplus::DEBUG_LOG_LEVEL))
	{
		VarString msg;
		msg.format("CurrentListTransformer: gui property <%s> value <%s> transformed to "
			       "accessor property <%s> value <%s>",
				    m_GuiPropTblCol.c_str(),
					leftVal.c_str(),
					m_AccPropTblCol.c_str(), 
					rightVal.c_str());
		LoggerPool::transformer().forcedLog(log4cplus::DEBUG_LOG_LEVEL, msg, __FILE__, __LINE__);
	}
}

//----------------------------------------------------------------------------
//! \throw  no-throw
//! \return pointer to created transformer
Transformer::TransformSharedPtr CurrentListTransformer::create()
{
	return TransformSharedPtr (new CurrentListTransformer(*this));
}

//----------------------------------------------------------------------------
} // tie
} // gui
} // basar
