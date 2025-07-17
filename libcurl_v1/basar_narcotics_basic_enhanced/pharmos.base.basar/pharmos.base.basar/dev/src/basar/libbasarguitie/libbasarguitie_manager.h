//----------------------------------------------------------------------------
/*! \file
 *  \brief  public interface manager
 *  \author Roland Kiefert
 *  \date   18.07.2005
 */
//----------------------------------------------------------------------------
#ifndef GUARD_LIBBASARGUITIE_MANAGER_H
#define GUARD_LIBBASARGUITIE_MANAGER_H

//----------------------------------------------------------------------------
#ifndef LIBBASARGUITIE_API
	#ifdef _WIN32
		#ifdef LIBBASARGUITIE_EXPORTS
			#define LIBBASARGUITIE_API __declspec(dllexport)        //!< dll exported
		#else
			#define LIBBASARGUITIE_API __declspec(dllimport)        //!< dll imported
		#endif
	#else
		#define LIBBASARGUITIE_API
	#endif
#endif	// LIBBASARGUITIE_API

//----------------------------------------------------------------------------
#include "libbasarcmnutil_pattern.h"
#include "libbasarguitie_definitions.h"

//---------------------------------------------------------------------------------------//
class QWidget;

//---------------------------------------------------------------------------------------//
namespace basar  {
namespace db	 {
namespace aspect {
	class AccessorPropertyTableRef;
	class AccessorPropertyTable_YIterator;
} // aspect
} // db

//---------------------------------------------------------------------------------------//
namespace gui {
namespace tie {

//---------------------------------------------------------------------------------------//
class MatcherRef;
class GuiPropertyTableRef;
class ManagerImpl;
class ExceptionHandlerManager;

//---------------------------------------------------------------------------------------//
/*! \brief  Base class for guitie-manager.

	This is a final class. Singleton */
class ManagerBase
{
	friend class basar::cmnutil::Singleton<ManagerBase>; //!< friend singleton
public:
	/*! \brief The method returns a reference to created matcher 
 		\n     NullReferenceException 
		\param qWidget       QWidget e.g.: dialog, panel or view
		\param accessorTable accessor property table 
		\param eWidgetPolicy policy that determines which kind of controls will be used as properties */
	LIBBASARGUITIE_API MatcherRef createMatcher		(const QWidget                            * qWidget, 
													 const db::aspect::AccessorPropertyTableRef accessorTable,
													 const WidgetPolicyEnum                     eWidgetPolicy = WITH_ALL_CTRLS);

	/*! \brief The method returns a reference to the created matcher. 
 		\n     NullReferenceException 
		\param guiPropertyTable qt property table
		\param accessorTable    accessor property table */
	LIBBASARGUITIE_API MatcherRef createMatcher		(const GuiPropertyTableRef                  guiPropertyTable, 
													 const db::aspect::AccessorPropertyTableRef accessorTable); 

	/*! \brief The method matches from an accessor property table to a QWidget
 		\n     NullReferenceException 	
		\param qWidget       QWidget e.g.: dialog, panel or view
		\param accIt         iterator to AccessorPropertyTable where data come from
		\param policy        matcher policy 
		\param eWidgetPolicy policy that determines which kind of controls will be used as properties */
	LIBBASARGUITIE_API void       MatchFromIterator	(const QWidget                                   * qWidget, 
													 const db::aspect::AccessorPropertyTable_YIterator accIt, 
													 const Int32                                       policy        = CONVERTTYPE | INSERT | SKIP,
													 const WidgetPolicyEnum                            eWidgetPolicy = WITH_ALL_CTRLS );

	/*! \brief The method matches from a QWidget to an accessor property table
 		\n     NullReferenceException 
		\param qWidget       QWidget e.g.: dialog, panel or view
		\param accIt         iterator to AccessorPropertyTable where data come from
		\param policy        matcher policy
		\param eWidgetPolicy policy that determines which kind of controls will be used as properties */
	LIBBASARGUITIE_API void		  MatchToIterator	(const QWidget                                   * qWidget, 
													 const db::aspect::AccessorPropertyTable_YIterator accIt, 
													 const Int32                                       policy        = CONVERTTYPE | INSERT | SKIP,
													 const WidgetPolicyEnum                            eWidgetPolicy = WITH_ALL_CTRLS );

	/*! \brief get reference to exception handler manager
 		\n     no-throw */
    LIBBASARGUITIE_API ExceptionHandlerManager &	getExceptionHandlerManager	();

private:
	/*! \brief default constructor 
 		\n     no-throw */
	LIBBASARGUITIE_API ManagerBase();
	
	/*! \brief The method returns the pointer to a manager implementation. 
 		\n     NullReferenceException */
	const boost::shared_ptr<ManagerImpl> &  getImpl() const;
	
	boost::shared_ptr<ManagerImpl>			m_pManager;		//!< pointer to managerMatcher
};

//---------------------------------------------------------------------------------------//
/*! \brief  derived class from ManagerBase of guitie.

	This is a final class. Singleton 
 *  \note  class is necessary for dll exporting Singleton\<ManagerBase\> and 
 *         instantiating Manager::m_Instance */
class LIBBASARGUITIE_API Manager : public basar::cmnutil::Singleton<ManagerBase>
{
private:
				/*! \brief standard constructor 
 					\n     no-throw */
				Manager    	();
				/*! \brief copy constructor 
 					\n     no-throw */
				Manager    	(const Manager & r);
				/*! \brief assign operator 
 				\n     no-throw */
	Manager &   operator =	(const Manager & r);
};

//---------------------------------------------------------------------------------------//
} // tie
} // gui
} // basar

//---------------------------------------------------------------------------------------//
#endif	// GUARD
