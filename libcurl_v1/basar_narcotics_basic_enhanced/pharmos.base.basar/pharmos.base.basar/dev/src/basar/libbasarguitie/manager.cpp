//----------------------------------------------------------------------------
/*! \file
 *  \brief  all functions implementation manager guitie
 *  \author Roland Kiefert
 *  \date   20.09.2005
 */
//----------------------------------------------------------------------------

#include "libbasarguitie_manager.h"

#include "libbasarguitie_functions.h"
#include "libbasarguitie_matcher.h"
#include "libbasarguitie_propertytable.h"
#include "managerimpl.h"
#include "libbasardbaspect_accessorpropertytable.h"
#include "libbasardbaspect_accessorpropertytable_yiterator.h"

//----------------------------------------------------------------------------
namespace basar {

//------------------------------------------------------------------------------
namespace cmnutil {

//------------------------------------------------------------------------------
using gui::tie::ManagerBase;

/*!	specialised method */
//! \return shared_ptr to ManagerBase instance
template <>
typename Singleton<ManagerBase>::InstancePtr & Singleton<ManagerBase>::inst()
{
    // avoid init order fiasco
    gui::tie::init();

    static InstancePtr s_Inst;
    return s_Inst;
}

//------------------------------------------------------------------------------
}	// namespace

//------------------------------------------------------------------------------
namespace gui {
namespace tie {

//----------------------------------------------------------------------------
/*!	\throw no-throw 
	\return reference to the created matcher */
MatcherRef ManagerBase::createMatcher(const QWidget                            * qWidget, 
									  const db::aspect::AccessorPropertyTableRef accessorTable,
									  const WidgetPolicyEnum                     eWidgetPolicy /* = basar::gui::tie::WITH_ALL_CTRLS */)
{
	return getImpl()->createMatcher(qWidget, accessorTable, eWidgetPolicy);
}

//----------------------------------------------------------------------------
/*!	\throw no-throw 
	\return reference to the created matcher */
MatcherRef ManagerBase::createMatcher( const GuiPropertyTableRef                  guiPropertyTable, 
									   const db::aspect::AccessorPropertyTableRef accessorTable )
{
	return getImpl()->createMatcher(guiPropertyTable, accessorTable);
}

//----------------------------------------------------------------------------
/*!	\throw no-throw 
	\warning only for single-line widgets ! */
void ManagerBase::MatchFromIterator(	const QWidget                                   * qWidget, 
										const db::aspect::AccessorPropertyTable_YIterator accIt, 
										const Int32                                       policy        /* = CONVERTTYPE | INSERT | SKIP */    ,
										const WidgetPolicyEnum                            eWidgetPolicy /* = basar::gui::tie::WITH_ALL_CTRLS */)
{	
	getImpl()->MatchFromIterator(qWidget, accIt, policy, eWidgetPolicy);
}

//----------------------------------------------------------------------------
/*!	\throw no-throw 
	\warning only for single-line widgets ! */
void ManagerBase::MatchToIterator(	const QWidget                                   * qWidget, 
									const db::aspect::AccessorPropertyTable_YIterator accIt, 
									const Int32                                       policy        /* = CONVERTTYPE | INSERT | SKIP */    ,
									const WidgetPolicyEnum                            eWidgetPolicy /* = basar::gui::tie::WITH_ALL_CTRLS */)
{
	getImpl()->MatchToIterator(qWidget, accIt, policy, eWidgetPolicy);
}

//----------------------------------------------------------------------------
/*!	\throw  NullReferenceException
	\return pointer to manager implementation */
const boost::shared_ptr<ManagerImpl>& ManagerBase::getImpl() const
{
	if (m_pManager.get() == 0)
	{
		ExceptInfo sInfo("basar.gui.tie.ManagerBase.getImpl", 
						 "Reference of ManagerImpl - no pointer to impl-class", 
						 __FILE__, __LINE__);
		throw NullReferenceException(sInfo);
	}

	return m_pManager;
}

//----------------------------------------------------------------------------
/*! \return reference to exception handler manager
	\throw  no-throw */
ExceptionHandlerManager & ManagerBase::getExceptionHandlerManager()
 {
   return getImpl()->getExceptionHandlerManager( );
} 

//----------------------------------------------------------------------------
/*!	\throw  no-throw */
ManagerBase::ManagerBase() : m_pManager(ManagerImpl::create())
{
}

//----------------------------------------------------------------------------
} // tie
} // gui
} // basar
