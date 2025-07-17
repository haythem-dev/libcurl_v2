//----------------------------------------------------------------------------
/*! \file
 *  \brief  transformer for property name
 *  \author Roland Kiefert
 *  \date   16.12.2005
 */
//----------------------------------------------------------------------------

#include "libbasarguitie_transformer.h"

#include "loggerpool.h"
#include "libbasarproperty_propertytype.h"
#include "helper.h"
#include "libbasarguitie_propertytable.h"
#include "libbasardbaspect_accessorpropertytable_yiterator.h"

//----------------------------------------------------------------------------
namespace basar {
namespace gui {
namespace tie {

//----------------------------------------------------------------------------
/*! \throw  no-throw */
PropertyNameTransformer::~PropertyNameTransformer() 
{
}
	 
//----------------------------------------------------------------------------
//! \throw  no-throw
//! 
PropertyNameTransformer::PropertyNameTransformer ( const ParamList & rCorProCon)
											     : m_CorAccessorGui(rCorProCon)
{
}

//----------------------------------------------------------------------------
//! \throw  no-throw
//! 
PropertyNameTransformer::PropertyNameTransformer(ConstString corProCon)	
{
	basar::cmnutil::ParameterList pl(corProCon);
	m_CorAccessorGui = pl;
}

//----------------------------------------------------------------------------
//! \throw  UnknownPropertyTypeException
//! 
void PropertyNameTransformer::RightToLeft(GuiPropertyTable_YIterator guiIt,
										  const AccPropTblYIterator  accIt)
{
	I18nString             temp;
	property::PropertyType accType;
	
	for (cmnutil::ParameterList_Iterator it = m_CorAccessorGui.begin(); 
		 it != m_CorAccessorGui.end(); 
		 ++it)
	{
		accType = accIt.getType(it->first);

		if ( !Helper::getStringFromAcc(accIt, accIt.getIndex(it->first), temp, accType))
		{
			if (temp == "")
				continue;

			basar::VarString msg;
			msg.format("accessor type <%d> for getting string of property <%s> is unknown", 
				       accType, 
					   it->first.c_str());
			ExceptInfo sInfo("basar.gui.tie.PropertyNameTransformer.RightToLeft()", 
				             msg, 
							 __FILE__, __LINE__);
			throw UnknownPropertyTypeException(sInfo);
		}

		guiIt.setString(guiIt.getIndex(it->second), temp);

		if (LoggerPool::transformer().isEnabledFor(log4cplus::DEBUG_LOG_LEVEL))
		{
			VarString msg;
			msg.format("PropertyNameTransformer: accessor property <%s> value <%s> transformed to "
					   "gui property <%s> value <%s>",
						it->first.c_str(),
						temp.c_str(),
						it->second.c_str(), 
						temp.c_str());
			LoggerPool::transformer().forcedLog(log4cplus::DEBUG_LOG_LEVEL, 
				                                    msg, 
													__FILE__, __LINE__);
		}
	}
}

//----------------------------------------------------------------------------
//! \throw  TypecastFailedException
//! 
void PropertyNameTransformer::LeftToRight(const GuiPropertyTable_YIterator guiIt,
										  AccPropTblYIterator              accIt)
{
	VarString              temp;
	property::PropertyType accType;
	
	for (cmnutil::ParameterList_Iterator it = m_CorAccessorGui.begin(); 
		 it != m_CorAccessorGui.end(); 
		 ++it)
	{
		if (guiIt.isValueExist(it->second))
		{	
			accType = accIt.getType(it->first);

			if ( !Helper::setStringToAcc(accIt, 
				                         accIt.getIndex(it->first), 
										 guiIt.getString(it->second), 
										 accType, 
										 temp))
			{
				if (temp == "")
					continue;

				basar::VarString msg;
				msg.format("accessor type <%d> for setting string of property <%s> is unknown", 
					        accType, 
							it->first.c_str());
				ExceptInfo sInfo("basar.gui.tie.PropertyNameTransformer.LeftToRight()", 
					             "The type is unknown", 
								 __FILE__, __LINE__);
				throw UnknownPropertyTypeException(sInfo);
			}
		}

		if (LoggerPool::transformer().isEnabledFor(log4cplus::DEBUG_LOG_LEVEL))
		{
			VarString msg;
			msg.format("PropertyNameTransformer: gui property <%s> value <%s> transformed to "
					   "accessor property <%s> value <%s>",
						it->second.c_str(),
						guiIt.getString(it->second).c_str(),
						it->first.c_str(), 
						guiIt.getString(it->second).c_str());
			LoggerPool::transformer().forcedLog(log4cplus::DEBUG_LOG_LEVEL, msg, __FILE__, __LINE__);
		}
	}
}

//----------------------------------------------------------------------------
//! \throw  no-throw
//! \return pointer to created transformer
Transformer::TransformSharedPtr PropertyNameTransformer::create()
{
	return TransformSharedPtr (new PropertyNameTransformer(*this));
}

//----------------------------------------------------------------------------
} // tie
} // gui
} // basar
