#ifndef EXCONFIGURATION_GUARD
#define EXCONFIGURATION_GUARD

//--------------------------------------------------------------------------------------------------//
// namespace section
//--------------------------------------------------------------------------------------------------//
namespace narcotics
{
namespace exceptions
{

//--------------------------------------------------------------------------------------------------//
class ExConfiguration : public basar::cmnutil::BasarRuntimeException
{
public:
	ExConfiguration(const basar::ExceptInfo& sInfo);
	virtual ~ExConfiguration(void);
};


} //namespace exceptions
} //namespace narcotics

#endif //EXCONFIGURATION_GUARD