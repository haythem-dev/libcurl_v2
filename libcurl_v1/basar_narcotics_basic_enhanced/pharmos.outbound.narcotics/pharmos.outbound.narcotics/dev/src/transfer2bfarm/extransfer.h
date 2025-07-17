#ifndef EXTRANSFER_GUARD
#define EXTRANSFER_GUARD

//--------------------------------------------------------------------------------------------------//
// namespace section
//--------------------------------------------------------------------------------------------------//
namespace narcotics
{
namespace exceptions
{

//--------------------------------------------------------------------------------------------------//
class ExTransfer : public basar::cmnutil::BasarRuntimeException
{
public:
	ExTransfer(const basar::ExceptInfo& sInfo);
	virtual ~ExTransfer(void);
};

} //namespace exceptions
} //namespace narcotics

#endif //EXTRANSFER_GUARD