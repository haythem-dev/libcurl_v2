#ifndef METADATA_GUARD
#define METADATA_GUARD

//------------------------------------------------------------------------------
//namespace
//------------------------------------------------------------------------------

namespace narcotics {
namespace archiving {

//------------------------------------------------------------------------------

class MetaData
{
public:
	virtual QString toString()=0;
	MetaData();
	virtual ~MetaData(void);
};

//------------------------------------------------------------------------------

} //namespace archiving
} //namespace narcotics

//------------------------------------------------------------------------------

#endif //METADATA_GUARD

//------------------------------------------------------------------------------
