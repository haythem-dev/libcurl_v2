//----------------------------------------------------------------------------
/*! \file
 *  \brief  public library interface: basar configuration
 *  \author Michael Eichenlaub
 *  \date   03.02.2010
 */
//----------------------------------------------------------------------------

#ifndef GUARD_LIBBASARCMNUTIL_CONFIG_H
#define GUARD_LIBBASARCMNUTIL_CONFIG_H

//------------------------------------------------------------------------------
#ifndef LIBBASARCMNUTIL_API
	#ifdef _WIN32
		#ifdef LIBBASARCMNUTIL_EXPORTS
			#define LIBBASARCMNUTIL_API __declspec(dllexport)	//!< dll exported
		#else
			#define LIBBASARCMNUTIL_API __declspec(dllimport)	//!< dll imported
		#endif 
	#else 
		#define LIBBASARCMNUTIL_API
	#endif
#endif

//------------------------------------------------------------------------------
#include "libbasarcmnutil_pattern.h"
#include "libbasar_definitions.h"
#include <iostream>
#include <boost/shared_ptr.hpp>

//----------------------------------------------------------------------------
namespace basar		{
namespace cmnutil	{

//----------------------------------------------------------------------------
class PropTree;

//----------------------------------------------------------------------------
//! \brief basar configuration by optional xml file "basar.config.xml"
//! \n     search order: executable directory, current working directory
class ConfigBase
{
	friend class Singleton < ConfigBase > ;		//!< creator class

public:

	//! \brief search xml config file and returns path
	LIBBASARCMNUTIL_API
	static bool	getConfigFile	(VarString & configpath					//!< found config file path
								);

	//! \brief get config value for given key (case sensitive) \n no-throw
	LIBBASARCMNUTIL_API 
	bool		getValue		(const VarString & key,					//!< config key
								 VarString       & val					//!< found config value
								) const;
	//! \brief iterate over all configuration values, print to std::cout by default \n no-throw
	LIBBASARCMNUTIL_API 
	void		toStream		(std::ostream & stream = std::cout		//!< output stream
								) const;

private:

	typedef boost::shared_ptr <PropTree> PropTreePtr;			//!< hides boost internal tree

	//! \brief recursive method for walking the property tree and stream node properties \n no-throw
	static void		streamNodes	(const PropTree  & pt,		//!< property tree node
								 std::ostream    & stream,	//!< output stream for printing
								 const VarString & path		//!< current node path; root node starts with empty path
								);

	//! \brief constructed by singleton \n no-throw
					ConfigBase  ();						
					ConfigBase  (const ConfigBase &);	//!< forbidden
	ConfigBase &	operator =	(const ConfigBase &);	//!< forbidden

	//! \brief read xml config file and build key, value tree \n no-throw
	void			readConfig	(const VarString & configpath		//! config file path
								);

	PropTreePtr				m_PropColl;		//!< property container for storing hierarchical key/values 
};

//------------------------------------------------------------------------------
//! \brief singleton wrapper for ConfigBase \n final class
//! \note  class is necessary for dll exporting Singleton\<ConfigBase\> and 
//!        instantiating Config::m_Instance
class LIBBASARCMNUTIL_API Config : public Singleton<ConfigBase>
{
private:
				Config    	();					//!< forbidden
				Config    	(const Config &);	//!< forbidden
	Config &	operator =	(const Config &);	//!< forbidden
};


//-----------------------------------------------------------------------------
}	// namespace cmnutil
}	// namespace basar

//-----------------------------------------------------------------------------
#endif	// GUARD
