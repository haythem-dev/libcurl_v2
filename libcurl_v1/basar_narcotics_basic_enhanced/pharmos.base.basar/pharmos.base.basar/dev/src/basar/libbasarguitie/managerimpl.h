//----------------------------------------------------------------------------
/*! \file
 *  \brief  internal interface manager
 *  \author Roland Kiefert
 *  \date   20.09.2005
 */
//----------------------------------------------------------------------------
#ifndef GUARD_MANAGERGUITIE_H
#define GUARD_MANAGERGUITIE_H

//----------------------------------------------------------------------------
#include <boost/shared_ptr.hpp>
#include <boost/weak_ptr.hpp>
#include "libbasar_definitions.h"
#include "libbasarguitie_exceptionhandlermanager.h"

//----------------------------------------------------------------------------
class QWidget;

namespace basar	{
	namespace db {
		namespace aspect {
			class AccessorPropertyTableRef;
			class AccessorPropertyTable_YIterator;
		}
	}

//----------------------------------------------------------------------------
namespace gui		{
namespace tie		{

//----------------------------------------------------------------------------
enum  WidgetPolicyEnum;
class MatcherRef;
class GuiPropertyTableRef;

//----------------------------------------------------------------------------

/*! \brief  implementation of manager guitie.

	This is a final class.*/
class ManagerImpl
{
public:
	typedef boost::shared_ptr< ManagerImpl >    ManagerImplSharedPtr;	//!< shared pointer to ManagerImpl

	/*! \brief The method returns a reference to the created matcher. 
 		\n     no-throws */
	MatcherRef				  createMatcher		(const QWidget                            * qWidget, 
												 const db::aspect::AccessorPropertyTableRef accessorTable, 
												 const WidgetPolicyEnum                     eWidgetPolicy);

	/*! \brief The method returns a reference to the created matcher. 
 		\n     no-throws */
	MatcherRef				  createMatcher		(const GuiPropertyTableRef                  guiPropertyTable, 
												 const db::aspect::AccessorPropertyTableRef accessorTable );

	/*! \brief The method matches from an accessor property table to a Qwidget
 		\n     no-throws */
	void					  MatchFromIterator	(const QWidget                                   * qWidget, 
												 const db::aspect::AccessorPropertyTable_YIterator accIt, 
												 const Int32                                       policy, 
												 const WidgetPolicyEnum                            eWidgetPolicy);

	/*! \brief The method matches from a Qwidget to an accessor property table
 		\n     no-throws */
	void					  MatchToIterator	(const QWidget                                   * qWidget, 
												 const db::aspect::AccessorPropertyTable_YIterator accIt, 
												 const Int32                                       policy, 
												 const WidgetPolicyEnum                            eWidgetPolicy);
	
	/*! \brief get reference to exception handler manager
 		\n     no-throw */
    ExceptionHandlerManager   & getExceptionHandlerManager() ;

	/*! \brief The method returns the shared pointer to this by casting weak -> shared
		\n no-throw */
	ManagerImplSharedPtr	    getThis();
	
	/*! \brief The method creates a ManagerImpl. 
 		\n     no-throw */
	static ManagerImplSharedPtr create();
	
	/*! \brief destructor
		\n	   no-throw */
	~ManagerImpl();
	
private:
	/*! "weak" reference to ManagerImpl (this self) that's already managed by a shared_ptr (ManagerImpl), not visible from the outside,
		initialized at static create */
	typedef boost::weak_ptr<ManagerImpl>	ManagerImplWeakPtr;

	/*! \brief The method sets member weak pointer
		\n no-throw */
	void	setWeakPtr		(ManagerImplWeakPtr weakPtr	);		

	ManagerImplWeakPtr		m_WeakPtr;					//!< weak pointer to this, needed for common reference group 
	ExceptionHandlerManager m_exceptionHandlerManager;	//!< exception handler manager instance
};

//----------------------------------------------------------------------------
} // tie
} // gui
} // basar

//----------------------------------------------------------------------------
#endif
