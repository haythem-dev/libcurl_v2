//----------------------------------------------------------------------------
/*! \file
 *  \brief  build SQL - statements list
 *  \author Roland Kiefert
 *  \date   15.07.2005
 */
//----------------------------------------------------------------------------

#include "libbasardbaspect_sqlstringbuilder.h"
#include "accessmethod.h"

//--------------------------------------------------------------------------------------------------
namespace basar {
namespace db {
namespace aspect {

//--------------------------------------------------------------------------------------------------
/*!	\throw no-throw 
	\param Method a shared pointer to an accessmethod */
SQLStringBuilderListRef::SQLStringBuilderListRef(boost::shared_ptr<AccessMethod> Method) : m_Method(Method)
{
}

//--------------------------------------------------------------------------------------------------
/*!	\throw no-throw 
	\return the first iterator of the list */
SQLStringBuilderList_Iterator SQLStringBuilderListRef::begin()
{
	SQLStringBuilderList_Iterator ret (m_Method, m_Method->begin());
	return ret;
}

//--------------------------------------------------------------------------------------------------
/*!	\throw no-throw 
	\return the last iterator of the list */
SQLStringBuilderList_Iterator SQLStringBuilderListRef::end()
{
	SQLStringBuilderList_Iterator ret (m_Method, m_Method->end());
	return ret;
}

//--------------------------------------------------------------------------------------------------
/*!	\throw no-throw 
	\return the size of the list */
SQLStringBuilderListSize SQLStringBuilderListRef::size() const
{
	return m_Method->size();
}

//--------------------------------------------------------------------------------------------------
} // aspect
} // db
} // basar
