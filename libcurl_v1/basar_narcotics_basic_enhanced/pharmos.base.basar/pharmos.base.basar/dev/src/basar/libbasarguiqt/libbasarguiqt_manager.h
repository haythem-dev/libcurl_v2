//---------------------------------------------------------------------------------------//
/*! \file
 *  \brief  public interface manager
 *  \author Roland Kiefert
 *  \date   20.12.2005
 */
//---------------------------------------------------------------------------------------//
#ifndef GUARD_LIBBASARGUIQT_MANAGER_H
#define GUARD_LIBBASARGUIQT_MANAGER_H

//----------------------------------------------------------------------------------------
#ifndef LIBBASARGUIQT_API
	#ifdef _WIN32
		#ifdef LIBBASARGUIQT_EXPORTS
			#define LIBBASARGUIQT_API __declspec(dllexport)	//!< dll exported
		#else
			#define LIBBASARGUIQT_API __declspec(dllimport)	//!< dll imported
		#endif 
	#else 
		#define LIBBASARGUIQT_API
	#endif 
#endif 

//-----------------------------------------------------------------------------------------
#include <boost/shared_ptr.hpp>
#include "libbasarcmnutil_pattern.h"
#include "libbasarcmnutil_bstring.h"

//---------------------------------------------------------------------------------------//
namespace basar {

struct DefinitionStruct
{
	VarString	copyright;				//!< copyright string from the company
	VarString	version;				//!< version string from the main program
	VarString	programDescription;		//!< description from a program
};

struct InitStruct
{
	VarString	dokulink;				//!< link from the program documentation
    VarString   dokulinktext;           //!< text which is displayed instead of dokulink (if present) 
	VarString	host;					//!< host name where the database is
	VarString	database;				//!< name from the database
};

struct DllStruct
{
	VarString	name;					//!< name of the dll
	VarString	pkgVersion;				//!< package version from the dll
};

//---------------------------------------------------------------------------------------//
namespace gui {
namespace qt {

//---------------------------------------------------------------------------------------//
class ManagerImpl;

/*! \brief  Base class for manager.

	This is a final class Singleton */
//---------------------------------------------------------------------------------------//
// class implementation ManagerBase
//---------------------------------------------------------------------------------------//
class ManagerBase
{
	friend class basar::cmnutil::Singleton<ManagerBase>; //!< friend for calling create-methods

public:
	/*! \brief The method inits a DefinitionStruct of a Manager before main will start
 		\n     NullReferenceException */
	LIBBASARGUIQT_API 
	void				init		(	const DefinitionStruct & defInfo	//!< definition information
									); 
	
	/*! \brief The method inits an InitStruct of a Manager after Main has started
 		\n     NullReferenceException */
	LIBBASARGUIQT_API 
	void				init		(	const InitStruct & initInfo			//!< init information
									); 

	/*! \brief The method inits a DllStruct for each dll.
 		\n     NullReferenceException */
	LIBBASARGUIQT_API 
	void				initDll		(	const DllStruct & dllInfo			//!< dll informaion
									); 

	/*! \brief The method returns the DefinitionStruct. 
 		\n     NullReferenceException */
	LIBBASARGUIQT_API 
	DefinitionStruct	getDef	(); 

	/*! \brief The method returns the InitStruct. 
 		\n     NullReferenceException */
	LIBBASARGUIQT_API 
	InitStruct			getInit	(); 

	/*! \brief The method gets the first DllStruct.
		\return true, if a dll was found
	    \return otherwise returns false
 		\n     no-throw */
	LIBBASARGUIQT_API 
	bool				getFirstDll	(	DllStruct & dllInfo						//!< dll informaion
									); 

	/*! \brief The method gets the next DllStruct.
	    \return true, if a dll was found
	    \return otherwise returns false
 		\n     no-throw */
	LIBBASARGUIQT_API 
	bool				getNextDll	(	DllStruct & dllInfo						//!< dll informaion
									); 

private:
	/*! \brief standard-constructor 
 		\n     no-throw */
	LIBBASARGUIQT_API					ManagerBase();
	
	/*! \brief The method returns the pointer to a manager. 
 		\n     NullReferenceException */
	const boost::shared_ptr<basar::gui::qt::ManagerImpl>&	getImpl() const;

	boost::shared_ptr<basar::gui::qt::ManagerImpl>			m_pManager; //!< pointer to manageraccessMethod
};

/*! \brief  derived class from ManagerBase.

	This is a final class. Singleton 
 *  \note  class is necessary for dll exporting Singleton\<ManagerBase\> and 
 *         instantiating Manager::m_Instance */
//-----------------------------------------------------------------------------------------------------//
// class Manager
//-----------------------------------------------------------------------------------------------------//
class LIBBASARGUIQT_API Manager : public basar::cmnutil::Singleton<ManagerBase>
{
private:
									/*! \brief standard constructor 
 										\n     no-throw */
									Manager    		();
									/*! \brief copy constructor 
 										\n     no-throw */
									Manager    		(const Manager     & r);
									/*! \brief assign operator 
 									\n     no-throw */
	Manager &    					operator =		(const Manager     & r);
};

} // qt
} // gui
} // basar

//---------------------------------------------------------------------------------------//
#endif	// GUARD
