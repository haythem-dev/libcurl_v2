#ifndef EXCROSSCHECKSVC_GUARD
#define EXCROSSCHECKSVC_GUARD

//--------------------------------------------------------------------------------------------------//
// namespace section
//--------------------------------------------------------------------------------------------------//
namespace narcotics
{
namespace exceptions
{

//--------------------------------------------------------------------------------------------------//
class ExCrossCheckSVC : public basar::cmnutil::BasarRuntimeException
{
public:
	ExCrossCheckSVC(const basar::ExceptInfo& sInfo);
	virtual ~ExCrossCheckSVC(void);
};

} //namespace exceptions
} //namespace narcotics

#endif //EXCROSSCHECKSVC_GUARD