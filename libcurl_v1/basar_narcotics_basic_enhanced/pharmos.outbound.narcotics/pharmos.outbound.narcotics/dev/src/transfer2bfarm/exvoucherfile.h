#ifndef EXVOUCHERFILE_GUARD
#define EXVOUCHERFILE_GUARD

//--------------------------------------------------------------------------------------------------//
// namespace section
//--------------------------------------------------------------------------------------------------//

namespace narcotics {
namespace exceptions {

//--------------------------------------------------------------------------------------------------//

class ExVoucherFile : public basar::cmnutil::BasarRuntimeException
{
public:
	ExVoucherFile(const basar::ExceptInfo& sInfo);
	virtual ~ExVoucherFile();
};

//--------------------------------------------------------------------------------------------------//

} //namespace exceptions
} //namespace narcotics

//--------------------------------------------------------------------------------------------------//

#endif //EXVOUCHERFILE_GUARD

//--------------------------------------------------------------------------------------------------//
