#ifndef EXSERVICE_GUARD
#define EXSERVICE_GUARD

//--------------------------------------------------------------------------------------------------//
// namespace section
//--------------------------------------------------------------------------------------------------//
namespace narcotics
{
namespace exceptions
{

//--------------------------------------------------------------------------------------------------//
class ExService : public basar::cmnutil::BasarRuntimeException
{
public:
	ExService(const basar::ExceptInfo& sInfo);
	virtual ~ExService(void);
};

} //namespace exceptions
} //namespace narcotics

#endif //EXSERVICE_GUARD