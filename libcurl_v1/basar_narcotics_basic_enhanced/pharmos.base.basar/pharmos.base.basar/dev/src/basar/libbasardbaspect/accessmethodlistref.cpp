//----------------------------------------------------------------------------
/*! \file
 *  \brief  accessor method list implementation
 *  \author Roland Kiefert
 *  \date   15.07.2005
 */
//----------------------------------------------------------------------------

#include "libbasardbaspect_accessmethod.h"
#include "libbasardbaspect_iaccessor.h"

//--------------------------------------------------------------------------------------------------
namespace basar  {
namespace db     {
namespace aspect {

//--------------------------------------------------------------------------------------------------
/*!	\throw no-throw 
\param Accessor a reference to the father class Accessor */
AccessMethodListRef::AccessMethodListRef( const boost::shared_ptr< IAccessor > Accessor) 
: m_Accessor(Accessor)
{
}

///////////////////////////////////////////////////////////////////////////////////////////////
/*! \retval	true if the implementation object hasn't been instantiated yet
	\return otherwise returns false
	\throw no-throw */
///////////////////////////////////////////////////////////////////////////////////////////////
bool AccessMethodListRef::isNull() const
{
	return( m_Accessor.get() == 0 );
}
///////////////////////////////////////////////////////////////////////////////////////////////
//!	\return const shared pointer to implementation object
//! \throw  NullReferenceException 
///////////////////////////////////////////////////////////////////////////////////////////////
const boost::shared_ptr<IAccessor>& AccessMethodListRef::getImpl() const
{
	basar::ConstString fun = "basar.db.aspect.AccessMethodListRef.getImpl()";

	if( isNull() ){
		throw NullReferenceException( 
				ExceptInfo( fun, "m_Accessor.get() == 0",__FILE__, __LINE__) );
    }
	return m_Accessor;
}
///////////////////////////////////////////////////////////////////////////////////////////////
/*!	\throw	no-throw */
///////////////////////////////////////////////////////////////////////////////////////////////
void AccessMethodListRef::reset()
{
	m_Accessor.reset();
}

//--------------------------------------------------------------------------------------------------
/*!	\throw no-throw 
	\return the first iterator of the list */
AccessMethodList_Iterator AccessMethodListRef::begin()
{
	AccessMethodList_Iterator ret (m_Accessor, m_Accessor->begin());
	return ret;
}

//--------------------------------------------------------------------------------------------------
/*!	\throw no-throw 
	\return the last iterator of the list */
AccessMethodList_Iterator AccessMethodListRef::end()
{
	AccessMethodList_Iterator ret (m_Accessor, m_Accessor->end());
	return ret;
}

//--------------------------------------------------------------------------------------------------
/*!	\throw no-throw 
	\return the size of the list */
AccessMethodListSize AccessMethodListRef::size()
{
	return m_Accessor->size();
}

//--------------------------------------------------------------------------------------------------
} // aspect
} // db
} // basar
