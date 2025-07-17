//----------------------------------------------------------------------------
/*! \file
 *  \brief  basar configuration by xml file
 *  \author Michael Eichenlaub
 *  \date   03.02.2010
 */
//----------------------------------------------------------------------------

#include "libbasarcmnutil_config.h"
#include "libbasarcmnutil_bstring.h"
#include "libbasarcmnutil_functions.h"
#include "loggerpool.h"

//! don't use deprecated Boost functions
#define BOOST_SYSTEM_NO_DEPRECATED 
//! don't use deprecated Boost functions
#define BOOST_FILESYSTEM_NO_DEPRECATED

#ifdef __GNUG__
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wtype-limits"
#endif

#include <boost/property_tree/ptree.hpp>

#ifdef __GNUG__
#pragma GCC diagnostic pop
#endif

#include <boost/property_tree/xml_parser.hpp>
#include <boost/filesystem.hpp>

/*----------------------------------------------------------------------------*/
namespace basar		{
namespace cmnutil	{

/*----------------------------------------------------------------------------*/
/*!	specialised method */
//! \return shared_ptr to ConfigBase instance
template <>
typename Singleton<ConfigBase>::InstancePtr & Singleton<ConfigBase>::inst()
{
    // avoid init order fiasco
    init();

    static InstancePtr s_inst;	
    return s_inst;
}

/*----------------------------------------------------------------------------*/
// pimpl: hide property_tree because of compiler warnings, 
// if included in header file
class PropTree : public boost::property_tree::ptree
{
};

/*----------------------------------------------------------------------------*/
//! reads xml config file
//! \throw  no-throw
ConfigBase::ConfigBase() : m_PropColl((new PropTree))
{
	VarString configpath;

	if ( ! getConfigFile(configpath) )
		return;

	readConfig(configpath);
}

/*----------------------------------------------------------------------------*/
//! \return retrieving success
//! \throw  no-throw
bool ConfigBase::getValue(const VarString & key, VarString & val) const
{

	const boost::optional<VarString> ret = m_PropColl->get_optional<VarString>(key);

	if ( ! ret )
		return false;

	val = ret.get();

	return true;
}

/*----------------------------------------------------------------------------*/
//! \throw no-throw
//!
void ConfigBase::streamNodes(const PropTree  & pt, 
							 std::ostream    & stream, 
							 const VarString & path)
{
	using boost::property_tree::ptree;

	ptree::const_iterator itEnd = pt.end();

	for (ptree::const_iterator it = pt.begin(); it != itEnd; ++it)
	{
		VarString extpath = path;

		if ( ! path.empty() )
			extpath += ".";
		extpath += it->first;

		stream	<< "config key: <" << extpath << ">";

		if ( ! it->second.data().empty() )
			stream << ", config value: <" << it->second.data() << ">";

		stream << std::endl;

		
		if ( ! it->second.empty() )		// node with further children
		{
			const PropTree * pPTree = static_cast<const PropTree *>(&(it->second));

			streamNodes(*pPTree, stream, extpath);
		}
	}
}

/*----------------------------------------------------------------------------*/
//! \throw no-throw
//!
void ConfigBase::toStream(std::ostream & stream /* = std::cout */) const
{
	streamNodes(*m_PropColl, stream, "");
}

/*----------------------------------------------------------------------------*/
//! search xml config file (first searched in executable's, then in working directory)
//! \return searching success
//! \throw  no-throw
bool ConfigBase::getConfigFile(VarString & configpath)
{
	using boost::filesystem::path;
	using boost::filesystem::exists;
	using boost::filesystem::current_path;
    using boost::filesystem::canonical;

	path searchpath[] = 
	{
		path(getExecDir().c_str()),	// executable's path
		path(current_path())		// current working directory
/*! \ingroup DIFF_WIN_UNIX */ 
#ifdef _AIX
        , canonical(path(getLibDir().c_str())) // basar so directory
#endif
	};

	ConstString dirtype[] = 
	{
		"executable's",
		"current working"
/*! \ingroup DIFF_WIN_UNIX */ 
#ifdef _AIX
        , "basar's"
#endif
	};

	ConstString	filename = "basar.config.xml";

	for (size_t i = 0; i < sizeof(searchpath)/sizeof(searchpath[0]); ++i)
	{
		searchpath[i] /= filename;

		if ( exists(searchpath[i]) )
		{
			if (LoggerPool::config().isEnabledFor(log4cplus::DEBUG_LOG_LEVEL))
			{
				VarString msg;
				msg.format("config file <%s> found in %s directory ",
							searchpath[i].string().c_str(),
							dirtype[i]);
				LoggerPool::config().forcedLog(log4cplus::DEBUG_LOG_LEVEL, msg, __FILE__, __LINE__);
			}

			configpath = searchpath[i].string();
			return true;
		}
	}

	if (LoggerPool::config().isEnabledFor(log4cplus::DEBUG_LOG_LEVEL))
	{
		VarString msg;
		msg.format("config file <%s> found neither in %s (%s) nor in %s (%s) directory ",
					filename,
					dirtype[0], searchpath[0].string().c_str(),
					dirtype[1], searchpath[1].string().c_str());
		LoggerPool::config().forcedLog(log4cplus::DEBUG_LOG_LEVEL, msg, __FILE__, __LINE__);
	}

	configpath.clear();
	return false;
}

/*----------------------------------------------------------------------------*/
//! \throw no-throw
//!
void ConfigBase::readConfig(const VarString & configpath)
{
	boost::property_tree::ptree * pPt = m_PropColl.get();

	boost::property_tree::read_xml (configpath, 
									*pPt,
									boost::property_tree::xml_parser::trim_whitespace | 
									boost::property_tree::xml_parser::no_comments       );
}

//----------------------------------------------------------------------------
}	// namespace cmnutil
}	// namespace basar
