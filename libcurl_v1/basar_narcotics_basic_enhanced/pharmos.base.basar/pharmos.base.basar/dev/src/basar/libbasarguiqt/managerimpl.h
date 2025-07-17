//----------------------------------------------------------------------------
/*! \file
 *  \brief  internal interface manager
 *  \author Roland Kiefert
 *  \date   20.12.2005
 */
//----------------------------------------------------------------------------
#ifndef GUARD_LIBBASARGUIQT_MANAGERIMPL_H
#define GUARD_LIBBASARGUIQT_MANAGERIMPL_H

//----------------------------------------------------------------------------
#include <list>
#include <boost/weak_ptr.hpp>
#include <boost/shared_ptr.hpp>
#include "libbasarguiqt_manager.h"

//----------------------------------------------------------------------------
namespace basar {
namespace gui {
namespace qt {

//----------------------------------------------------------------------------
/*! \brief  implementation of manager.

	This is a final class.*/
class ManagerImpl
{
public:
	typedef boost::shared_ptr< ManagerImpl >    ManagerImplSharedPtr;		//!< shared pointer to AccessMethod

	/*! \brief The method inits a DefinitionStruct of a Manager before main will start
 		\n     no-throw */
	void				init		(	const DefinitionStruct & defInfo	//!< definition information
									); 
	
	/*! \brief The method inits an InitStruct of a Manager after Main has started
 		\n     no-throw */
	void				init		(	const InitStruct & initInfo			//!< init information
									); 

	/*! \brief The method inits a DllStruct for each dll.
 		\n     no-throw */
	void				initDll		(	const DllStruct & dllInfo			//!< dll informaion
									); 

	/*! \brief The method returns the DefinitionStruct. 
 		\n     no-throw */
	DefinitionStruct	getDef		(); 

	/*! \brief The method returns the InitStruct. 
 		\n     no-throw */
	InitStruct			getInit		(); 
	
	/*! \brief The method gets the first DllStruct.
 		\n     no-throw */
	bool				getFirstDll	(	DllStruct & dllInfo						//!< dll informaion
									); 

	/*! \brief The method gets the next DllStruct.
 		\n     no-throw */
	bool				getNextDll	(	DllStruct & dllInfo						//!< dll informaion
									); 

	/*! \brief The method returns shared pointer to this by casting weak -> shared
		\n no-throw */
	ManagerImplSharedPtr		getThis	();
	
	/*! \brief The method creates a ManagerImpl. 
 		\n     no-throw */
	static ManagerImplSharedPtr create	();
	
	/*! \brief destructor
		\n	   no-throw */
	~ManagerImpl();
	
private:
	/*! "weak" reference to object (itself) that's already managed by a shared_ptr (ManagerImpl), not visible from the outside,
		initialized at static create */
	typedef boost::weak_ptr<ManagerImpl>	ManagerImplWeakPtr;	//!< weak pointer to ManagerImpl

	/*! \brief The method sets member weak pointer
		\n no-throw */
	void	setWeakPtr		(ManagerImplWeakPtr weakPtr	);		

	ManagerImplWeakPtr				m_WeakPtr;	//!< weak pointer to this, needed for common reference group 
	DefinitionStruct				m_Def;		//!< definition information from the program
	InitStruct						m_Init;		//!< init information from the program
	std::list<DllStruct>			m_Dlls;		//!< dll informations
	std::list<DllStruct>::iterator	m_it;		//!< iterator from the list
	bool							m_DllIsGet;	//!< the first dll is read and it is not finished	
};

//----------------------------------------------------------------------------
} // qt
} // gui
} // basar

//----------------------------------------------------------------------------
#endif // GUARD
