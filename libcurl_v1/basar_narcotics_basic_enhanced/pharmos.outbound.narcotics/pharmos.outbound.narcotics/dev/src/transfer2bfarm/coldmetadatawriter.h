#ifndef COLDMETADATAWRITER_GUARD
#define COLDMETADATAWRITER_GUARD

//--------------------------------------------------------------------------------------------------//
// include section
//--------------------------------------------------------------------------------------------------//
#include "metadatawriter.h"
#include "metadata.h"
#include "serviceconfiguration.h"
#include "bgafolderptr.h"

//--------------------------------------------------------------------------------------------------//
// namespace section
//--------------------------------------------------------------------------------------------------//

namespace utils {

//--------------------------------------------------------------------------------------------------//

class ColdMetaDataWriter : public MetaDataWriter
{
public:
	ColdMetaDataWriter();
	ColdMetaDataWriter(narcotics::services::ServiceConfiguration config);
	virtual ~ColdMetaDataWriter();

	virtual void write(narcotics::archiving::MetaData& metaData, narcotics::bgafolder::BGAFolderPtr dir);

private:
	basar::VarString m_destFileName;
};

//--------------------------------------------------------------------------------------------------//

} //namespace utils

//--------------------------------------------------------------------------------------------------//

#endif //COLDMETADATAWRITER_GUARD

//--------------------------------------------------------------------------------------------------//
