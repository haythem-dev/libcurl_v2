//----------------------------------------------------------------------------
/*! \file
 *  \brief  domain module for text specific handling for multilanguage
 *  \author Thomas Hörath
 *  \date   23.12.2005
 */
//----------------------------------------------------------------------------

#ifndef GUARD_TEXTMANAGEMENTDM_H
#define GUARD_TEXTMANAGEMENTDM_H

//----------------------------------------------------------------------------
#include "libbasar_definitions.h"
#include "libbasardbaspect_accessor.h"

//----------------------------------------------------------------------------
namespace basar
{
	namespace db
	{
		namespace aspect
		{
			class ConnectionRef;
			class AccessorPropertyTable_YIterator;
		}
	}
}

//----------------------------------------------------------------------------
namespace domMod {

//---------------------------------------------------------------------------
/*! \brief class for managing text specific access 
	\n final class
	\n throws no exceptions */
class TextManagement
{
public:	
	/*! \brief std-constructor
		\n no-throw */
	TextManagement();
	
	/*! \brief std-destructor
		\n no-throw */
	~TextManagement();

	/*! \brief initialize accessor instances
		\n no-throw */
	void init(const basar::db::aspect::ConnectionRef activeConn //!< reference to active connection
			 );
	
	/*! \brief reset accessor instances and contents of property tabels
		\n no-throw */
	void shutdown();

	
	/*! \brief set applicationid for selecting labels / messages
		\n no-throw */
	void setApplID		(const basar::Int16 applID		//!< applicationid to select pptext
						);
	
	/*! \brief set moduleid for selecting labels / messages
		\n no-throw */
	void setModID		(const basar::Int16 moduleID	//!< moduleid to select pptext
						);
	
	/*! \brief set languageid for selecting labels / messages
		\n no-throw */
	void setLanguageID	(const basar::I18nString& languageID	//!< languageid to select pptext
						);

// text
	/*! \brief select single text for given reftext
		\n no-throw */
	basar::db::aspect::AccessorPropertyTable_YIterator	findTextByRefText	(const basar::I18nString& rRefText //!< reftext to select label / message to
																			);
	
private:
	basar::db::aspect::AccessorInstanceRef m_TextACC;	//!< instance of text-accessor
		
	basar::Int16		m_ApplID;		//!< applicationid to select pptext
	basar::Int16		m_ModuleID;		//!< moduleid to select pptext
	basar::I18nString	m_LanguageID;	//!< languageid to select pptext
};

//----------------------------------------------------------------------------
} // domMod

//----------------------------------------------------------------------------
#endif	// GUARD
