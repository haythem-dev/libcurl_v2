//----------------------------------------------------------------------------
/*! \file
 *  \brief  accessor definition list
 *  \author Roland Kiefert
 *  \date   19.07.2005
 */
//----------------------------------------------------------------------------

#include "libbasardbaspect_accessor.h"
#include "managerimpl.h"

//--------------------------------------------------------------------------------------------------
namespace basar {
namespace db {
namespace aspect {

///////////////////////////////////////////////////////////////////////////////////////////////
/*!	\throw no-throw 
	\param Manager a reference to the father class Manager */
///////////////////////////////////////////////////////////////////////////////////////////////
AccessorDefinitionListRef::AccessorDefinitionListRef( const boost::shared_ptr<ManagerImpl> Manager ) 
:m_Manager( Manager )
{
}
///////////////////////////////////////////////////////////////////////////////////////////////
/*! \retval	true if the implementation object hasn't been instantiated yet 
	\return otherwise returns false
	\throw no-throw */
///////////////////////////////////////////////////////////////////////////////////////////////
bool AccessorDefinitionListRef::isNull() const
{
	return( m_Manager.get() == 0 );
}
///////////////////////////////////////////////////////////////////////////////////////////////
//!	\return const shared pointer to implementation object
//! \throw  NullReferenceException 
///////////////////////////////////////////////////////////////////////////////////////////////
const boost::shared_ptr<ManagerImpl>& AccessorDefinitionListRef::getImpl() const
{
	basar::ConstString fun = "basar.db.aspect.AccessorDefinitionListRef.getImpl()";

	if( isNull() ){
		throw NullReferenceException( 
				ExceptInfo( fun, "m_Manager.get() == 0",__FILE__, __LINE__) );
    }
	return m_Manager;
}
///////////////////////////////////////////////////////////////////////////////////////////////
/*! \throw	no-throw */
///////////////////////////////////////////////////////////////////////////////////////////////
void AccessorDefinitionListRef::reset()
{
	m_Manager.reset();
}
///////////////////////////////////////////////////////////////////////////////////////////////
/*!	\throw no-throw 
	\return the first iterator of the list */
///////////////////////////////////////////////////////////////////////////////////////////////
AccessorDefinitionList_Iterator AccessorDefinitionListRef::begin()
{
	AccessorDefinitionList_Iterator ret (m_Manager, m_Manager->beginDefinition());
	return ret;
}

/*!	\throw no-throw 
	\return the last iterator of the list */
AccessorDefinitionList_Iterator AccessorDefinitionListRef::end()
{
	AccessorDefinitionList_Iterator ret (m_Manager, m_Manager->endDefinition());
	return ret;
}

/*!	\throw no-throw 
	\return the size of the list */
AccessorListSize AccessorDefinitionListRef::size() const 
{
	return m_Manager->sizeDefinition();
}

} // aspect
} // db
} // basar
