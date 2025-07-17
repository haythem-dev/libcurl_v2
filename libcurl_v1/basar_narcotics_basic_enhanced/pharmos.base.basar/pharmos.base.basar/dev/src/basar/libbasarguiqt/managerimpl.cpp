//----------------------------------------------------------------------------
/*! \file
 *  \brief  all functions implemented of ManagerImpl
 *  \author Roland Kiefert
 *  \date   20.12.2005
 */
//----------------------------------------------------------------------------

#include "managerimpl.h"

//----------------------------------------------------------------------------
namespace basar {
namespace gui {
namespace qt {


//----------------------------------------------------------------------------
/*!	\throw  no-throw */
void ManagerImpl::init(const DefinitionStruct & defInfo)
{
	m_Def = defInfo;
}

//----------------------------------------------------------------------------
/*!	\throw  no-throw */
void ManagerImpl::init(const InitStruct & initInfo)
{
	m_Init = initInfo;
}

//----------------------------------------------------------------------------
/*!	\throw  no-throw */
void ManagerImpl::initDll(const DllStruct & dllInfo)
{
	m_Dlls.push_back(dllInfo);
}

//----------------------------------------------------------------------------
/*!	\throw no-throw 
	\return Struct with definition information */
DefinitionStruct ManagerImpl::getDef()
{
	return m_Def;
}

//----------------------------------------------------------------------------
/*!	\throw no-throw 
	\return struct with init information */
InitStruct ManagerImpl::getInit()
{
	return m_Init;
}

//----------------------------------------------------------------------------
/*!	\throw	no-throw 
	\return true, if a dll was found
	\return otherwise returns false */
bool ManagerImpl::getFirstDll(DllStruct & dllInfo)
{
	m_it = m_Dlls.begin();
	m_DllIsGet = m_it != m_Dlls.end();
	if (m_DllIsGet)
		dllInfo = *m_it;
	return m_DllIsGet;
}

//----------------------------------------------------------------------------
/*!	\throw	no-throw 
	\return true, if a dll was found
	\return otherwise returns false
*/
bool ManagerImpl::getNextDll(DllStruct & dllInfo)
{
	if (m_DllIsGet)
	{
		m_it++;
		m_DllIsGet = m_it != m_Dlls.end();
		if (m_DllIsGet)
			dllInfo = *m_it;
	}
	return m_DllIsGet;
}

//----------------------------------------------------------------------------
/*!	\throw  no-throw
	\return pointer to AccessMethod itself */
ManagerImpl::ManagerImplSharedPtr ManagerImpl::create()
{
	ManagerImplSharedPtr t = ManagerImplSharedPtr(new ManagerImpl);
	t->setWeakPtr(t);
	return t;
}

//----------------------------------------------------------------------------
/*!	\throw  no-throw */
ManagerImpl::~ManagerImpl()
{
}

//----------------------------------------------------------------------------
/*! \return (only one group of) AccessMethod shared pointer 
	\throw  no-throw */
ManagerImpl::ManagerImplSharedPtr ManagerImpl::getThis()
{
	return ManagerImplSharedPtr(m_WeakPtr);
}

//----------------------------------------------------------------------------
/*! \throw  no-throw
	\param weakPtr weak pointer to the class ManagerImpl */
void ManagerImpl::setWeakPtr(ManagerImplWeakPtr weakPtr)
{
	m_WeakPtr = weakPtr;
}

//----------------------------------------------------------------------------
} // qt
} // gui
} // basar
