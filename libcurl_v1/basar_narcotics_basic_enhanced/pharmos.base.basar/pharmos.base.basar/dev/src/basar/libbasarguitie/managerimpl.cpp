//----------------------------------------------------------------------------
/*! \file
 *  \brief  all function implementation manager 
 *  \author Roland Kiefert
 *  \date   20.09.2005
 */
//----------------------------------------------------------------------------

#include "managerimpl.h"

#include "guipropertytable.h"
#include "eventhandlercomposite.h"
#include "libbasarguitie_matcher.h"
#include "matcher.h"
#include "libbasarguitie_propertytable.h"
#include "loggerpool.h"
#include "libbasardbaspect_accessorpropertytable_yiterator.h"

//----------------------------------------------------------------------------
namespace basar {
namespace gui {
namespace tie {
	
/*!	\throw no-throw 
	\return reference to the created matcher 
	\param qWidget a QWidget for example dialog, panel or view
	\param accessorTable an accessor property table 
	\param eWidgetPolicy policy that determines which sort of controls will be used as properties */
MatcherRef ManagerImpl::createMatcher( const QWidget                            * qWidget, 
									   const db::aspect::AccessorPropertyTableRef accessorTable,
									   const WidgetPolicyEnum                     eWidgetPolicy )
{
	EventHandlerComposite    * eh        = new EventHandlerComposite;

	GuiPropertyTableRef        guiPropTbl (GuiPropertyTable::create(qWidget, eh, eWidgetPolicy));

	boost::shared_ptr<Matcher> pMatcher  = Matcher::create(guiPropTbl, accessorTable);

	MatcherRef t(pMatcher);
	guiPropTbl.setMatcher(pMatcher);

	return t;
}

/*!	\throw no-throw 
	\return reference to the created matcher 
	\param guiPropertyTable a qt property table
	\param accessorTable an accessor property table */
MatcherRef ManagerImpl::createMatcher( const GuiPropertyTableRef                  guiPropertyTable, 
									   const db::aspect::AccessorPropertyTableRef accessorTable )
{
	MatcherRef t(Matcher::create(guiPropertyTable,accessorTable));
	return t;
}

/*!	\throw no-throw 
	\param qWidget a QWidget for example dialog, panel or view
	\param  accIt Iterator to AccessorPropertyTable where data come from
	\param  policy the policy of the matcher 
	\param eWidgetPolicy policy that determines which sort of controls will be used as properties */
void ManagerImpl::MatchFromIterator(	const QWidget                                   * qWidget, 
										const db::aspect::AccessorPropertyTable_YIterator accIt, 
										const Int32                                       policy, 
										const WidgetPolicyEnum                            eWidgetPolicy )
{
	BLOG_TRACE(LoggerPool::managerImpl(), "matching from iterator to guiPropTab started.");

	EventHandlerComposite* eh = 0;
	GuiPropertyTableRef guiPropTbl(GuiPropertyTable::create(qWidget, eh, eWidgetPolicy));

	if (guiPropTbl.empty())
		guiPropTbl.insert();

	MatcherRef::RightToLeft(guiPropTbl.begin(),accIt,policy);

	BLOG_TRACE(LoggerPool::managerImpl(), "matching from iterator to guiPropTab finished.");
}

/*!	\throw no-throw 
	\param qWidget a QWidget for example dialog, panel or view
	\param  accIt Iterator to AccessorPropertyTable where data come from
	\param  policy the policy of the matcher 
	\param eWidgetPolicy policy that determines which sort of controls will be used as properties */
void ManagerImpl::MatchToIterator(	const QWidget                                   * qWidget, 
									const db::aspect::AccessorPropertyTable_YIterator accIt, 
									const Int32                                       policy, 
									const WidgetPolicyEnum                            eWidgetPolicy )
{
	BLOG_TRACE(LoggerPool::managerImpl(), "matching from guiPropTab to iterator started.");

	EventHandlerComposite* eh = 0;
	GuiPropertyTableRef guiPropTbl(GuiPropertyTable::create(qWidget, eh, eWidgetPolicy));

	if (guiPropTbl.empty())
		guiPropTbl.insert();

	MatcherRef::LeftToRight(guiPropTbl.begin(), accIt, policy);

	BLOG_TRACE(LoggerPool::managerImpl(), "matching from guiPropTab to iterator finished.");
}

/*!	\throw  no-throw
	\return pointer to ManagerImpl itself */
ManagerImpl::ManagerImplSharedPtr ManagerImpl::create()
{
	ManagerImplSharedPtr t = ManagerImplSharedPtr(new ManagerImpl);
	t->setWeakPtr(t);
	return t;
}

/*!	\throw  no-throw */
ManagerImpl::~ManagerImpl()
{
}

/*! \return (only one group of) ManagerImpl shared pointer 
	\throw  no-throw */
ManagerImpl::ManagerImplSharedPtr ManagerImpl::getThis()
{
	return ManagerImplSharedPtr(m_WeakPtr);
}

/*! \throw  no-throw
	\param weakPtr weak pointer to the class ManagerImpl */
void ManagerImpl::setWeakPtr(ManagerImplWeakPtr weakPtr)
{
	m_WeakPtr = weakPtr;
}

/*! \return reference to exception handler manager
	\throw  no-throw */
ExceptionHandlerManager & ManagerImpl::getExceptionHandlerManager()
{
	return m_exceptionHandlerManager;
} 

} // tie
} // gui
} // basar
