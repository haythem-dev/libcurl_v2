//----------------------------------------------------------------------------
/*! \file
 *  \brief  transformer for integer: leading zeroes
 *  \author Michael Eichenlaub
 *  \date   20.04.2010
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
IntTransformer::~IntTransformer() 
{
}

//----------------------------------------------------------------------------
/*! \throw  no-throw */
IntTransformer::IntTransformer (const VarString & accPropTblCol,
								const Int32       length)
							   : m_AccPropTblCol(accPropTblCol),
								 m_GuiPropTblCol(accPropTblCol),
								 m_Length       (length       )
{
}

//----------------------------------------------------------------------------
/*! \throw  no-throw */
IntTransformer::IntTransformer (const VarString & accPropTblCol,
								const VarString & guiPropTblCol,
								const Int32       length)
							   : m_AccPropTblCol(accPropTblCol),
								 m_GuiPropTblCol(guiPropTblCol),
							     m_Length       (length       )
{
}

//----------------------------------------------------------------------------
/*! \throw  UnknownPropertyTypeException */
void IntTransformer::RightToLeft (GuiPropertyTable_YIterator guiIt,
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
		ExceptInfo sInfo("basar.gui.tie.IntTransformer.RightToLeft()",
				         msg, 
						 __FILE__, __LINE__);
		throw UnknownPropertyTypeException(sInfo);
	}

	I18nString leftVal;
	leftVal.format("%0*s", m_Length, rightVal.c_str());

	guiIt.setString(m_GuiPropTblCol, leftVal);

	if (LoggerPool::transformer().isEnabledFor(log4cplus::DEBUG_LOG_LEVEL))
	{
		VarString msg;
		msg.format("IntTransformer: accessor property <%s> value <%s> transformed to "
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
void IntTransformer::LeftToRight(const GuiPropertyTable_YIterator guiIt,
								 AccPropTblYIterator              accIt)
{
	I18nString leftVal;

	if (guiIt.isValueExist(m_GuiPropTblCol))
		leftVal = guiIt.getString(m_GuiPropTblCol);

	VarString              errorMsg;
	property::PropertyType accType = accIt.getType(m_AccPropTblCol);

	if (!Helper::setStringToAcc(accIt, 
		                        accIt.getIndex(m_AccPropTblCol), 
								leftVal, 
								accType, 
								errorMsg))
	{
		if (errorMsg != "")
		{
			ExceptInfo sInfo("basar.gui.tie.IntTransformer.LeftToRight()", 
				             errorMsg, 
							 __FILE__, __LINE__);
			throw TypecastFailedException(sInfo);
		}
	}

	if (LoggerPool::transformer().isEnabledFor(log4cplus::DEBUG_LOG_LEVEL))
	{
		I18nString tmp;
		Helper::getStringFromAcc(accIt, accIt.getIndex(m_AccPropTblCol), tmp, accType);

		VarString msg;
		msg.format("IntTransformer: gui property <%s> value <%s> transformed to "
			       "accessor property <%s> value <%s>",
				    m_GuiPropTblCol.c_str(),
					leftVal.c_str(),
					m_AccPropTblCol.c_str(), 
					tmp.c_str());
		LoggerPool::transformer().forcedLog(log4cplus::DEBUG_LOG_LEVEL, 
				                                msg, 
												__FILE__, __LINE__);
	}
}

//----------------------------------------------------------------------------
/*! \throw  no-throw
	\return pointer to created transformer */
Transformer::TransformSharedPtr IntTransformer::create()
{
	return TransformSharedPtr (new IntTransformer(*this));
}

//----------------------------------------------------------------------------
} // tie
} // gui
} // basar
