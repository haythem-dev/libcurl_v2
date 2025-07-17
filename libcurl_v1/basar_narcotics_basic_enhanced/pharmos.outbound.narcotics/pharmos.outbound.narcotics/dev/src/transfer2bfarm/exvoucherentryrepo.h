#ifndef EXVOUCHERENTRYREPO_GUARD
#define EXVOUCHERENTRYREPO_GUARD

//--------------------------------------------------------------------------------------------------//
// namespace section
//--------------------------------------------------------------------------------------------------//

namespace narcotics {
namespace exceptions {

//--------------------------------------------------------------------------------------------------//

class ExVoucherEntryRepo : public basar::cmnutil::BasarRuntimeException
{
public:
	ExVoucherEntryRepo(const basar::ExceptInfo& sInfo);
	virtual ~ExVoucherEntryRepo();
};

//--------------------------------------------------------------------------------------------------//

} //namespace exceptions
} //namespace narcotics

//--------------------------------------------------------------------------------------------------//

#endif //EXVOUCHERENTRYREPO_GUARD

//--------------------------------------------------------------------------------------------------//
