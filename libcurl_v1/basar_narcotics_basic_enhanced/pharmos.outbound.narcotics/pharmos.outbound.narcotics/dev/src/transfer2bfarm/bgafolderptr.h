#ifndef BGAFOLDERPTR_GUARD
#define BGAFOLDERPTR_GUARD

//------------------------------------------------------------------------------
//includes
//------------------------------------------------------------------------------
#include <boost/shared_ptr.hpp>

//------------------------------------------------------------------------------
//namespaces
//------------------------------------------------------------------------------
namespace narcotics
{
namespace bgafolder
{

	class BGAFolder;
	typedef boost::shared_ptr< BGAFolder > BGAFolderPtr;

} //namespace bgabranch
} //namespace narcotics

#endif //BGAFOLDERPTR_GUARD