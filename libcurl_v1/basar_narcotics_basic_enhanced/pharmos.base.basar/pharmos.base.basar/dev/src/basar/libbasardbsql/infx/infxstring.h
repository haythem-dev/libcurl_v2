//----------------------------------------------------------------------------
/*! \file
 *  \brief  string class regarding Informix server codepage (illegal characters)
 *  \author Michael Eichenlaub
 *  \date   03.07.2009
 */
//----------------------------------------------------------------------------

#ifndef GUARD_INFXSTRING_H
#define GUARD_INFXSTRING_H

//----------------------------------------------------------------------------
#include "libbasar_definitions.h"
#include "libbasarcmnutil_bstring.h"

//----------------------------------------------------------------------------
namespace basar	{
namespace db	{
namespace sql	{

//----------------------------------------------------------------------------
//! string class regarding Informix server codepage (illegal characters) \n derived class
class InfxString
{
public:
	//! c'tor: replaces forbidden characters with question marks; depends on chosen codepage	\n no-throw
					InfxString	(const VarString & rStr		//!< SQL select statement or value string with forbidden characters
								);
	//! return member as ConstBuffer	\n no-throw
	ConstBuffer		out			() const;

private:

	//! conversion table for mapping sql statemtents to DB server codepage (replacing illegal characters)
	typedef const UInt8	* ConvertTable;

	//! get appropriate conversion table according current basar codepage	\n no-throw
	ConvertTable	getConvTable() const;

	//! converts member VarString according to appropriate conversiontable \n no-throw
	void			convertVar	(ConvertTable cvttbl			//!< appropriate conversion table for codepage
								);

					InfxString	();                           	//!< forbidden
					InfxString	(const InfxString & rCopy);		//!< forbidden
	InfxString &   	operator =	(const InfxString & rCopy);		//!< forbidden

	VarString		m_StrVar;		//!< contains converted characters; init'd by VarString
};

//----------------------------------------------------------------------------
}	// namespace sql
}	// namespace db
}	// namespace basar

#endif	// GUARD
