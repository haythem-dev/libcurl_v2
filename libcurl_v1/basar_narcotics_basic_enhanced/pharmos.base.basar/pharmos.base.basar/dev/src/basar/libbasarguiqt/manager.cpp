//----------------------------------------------------------------------------
/*! \file
 *  \brief  implementation of Manager and ManagerBase
 *  \author Roland Kiefert
 *  \date   20.12.2005
 */
//----------------------------------------------------------------------------

#include "managerimpl.h"

#include "libbasarcmnutil_exceptions.h"
#include "libbasarguiqt_functions.h"

//----------------------------------------------------------------------------
namespace basar   {

//------------------------------------------------------------------------------
namespace cmnutil {

//------------------------------------------------------------------------------
using gui::qt::ManagerBase;

/*!	specialised method */
//! \return shared_ptr to ManagerBase instance
template <>
typename Singleton<ManagerBase>::InstancePtr & Singleton<ManagerBase>::inst()
{
    // avoid init order fiasco
    gui::qt::init();

    static InstancePtr s_Inst; 
    return s_Inst;
}

//------------------------------------------------------------------------------
}	// namespace

//------------------------------------------------------------------------------
namespace gui {
namespace qt  {

/*!    \throw NullReferenceException */
void ManagerBase::init(const DefinitionStruct & defInfo)
{
	getImpl()->init(defInfo);
}
	
/*!    \throw NullReferenceException */
void ManagerBase::init(const InitStruct & initInfo)
{
	getImpl()->init(initInfo);
}

/*!    \throw NullReferenceException */
void ManagerBase::initDll(const DllStruct & dllInfo)
{
	getImpl()->initDll(dllInfo);
}

/*!	\throw NullReferenceException 
	\return the definition Information */
DefinitionStruct ManagerBase::getDef()
{
	return getImpl()->getDef();
}

/*!	\throw NullReferenceException 
	\return the init Information */
InitStruct ManagerBase::getInit()
{
	return getImpl()->getInit();
}

/*!	\throw NullReferenceException 
	\return true, if a dll was found
	\return otherwise returns false */
bool ManagerBase::getFirstDll(DllStruct & dllInfo)
{
	return getImpl()->getFirstDll(dllInfo);
}

/*!	\throw NullReferenceException 
	    \return true, if a dll was found
	    \return otherwise returns false
 */
bool ManagerBase::getNextDll(DllStruct & dllInfo)
{
	return getImpl()->getNextDll(dllInfo);
}

/*!	\throw  NullReferenceException
	\return pointer to Manager */
const boost::shared_ptr<basar::gui::qt::ManagerImpl>& ManagerBase::getImpl() const
{
	if (m_pManager.get() == 0)
	{
		ExceptInfo sInfo( "basar.gui.qt.ManagerBase.check", 
						  "Reference of ManagerImpl is no pointer to impl-class", 
						  __FILE__, __LINE__);
		throw NullReferenceException(sInfo);
	}
	return m_pManager;
}

/*!	\throw  no-throw */
ManagerBase::ManagerBase()
: m_pManager(ManagerImpl::create())
{
}

} // qt
} // gui
} // basar
