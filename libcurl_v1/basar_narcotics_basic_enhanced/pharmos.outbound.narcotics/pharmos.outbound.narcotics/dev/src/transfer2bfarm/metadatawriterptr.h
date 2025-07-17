#ifndef METADATAWRITERPTR_GUARD
#define METADATAWRITERPTR_GUARD

//------------------------------------------------------------------------------
//includes
//------------------------------------------------------------------------------
#include <boost/shared_ptr.hpp>

//------------------------------------------------------------------------------
//namespaces
//------------------------------------------------------------------------------
namespace utils 
{
	class MetaDataWriter;
	typedef boost::shared_ptr< MetaDataWriter > MetaDataWriterPtr;

} //namespace utils 

#endif //METADATAWRITERPTR_GUARD