#ifndef EXCROSSCHECKUC_GUARD
#define EXCROSSCHECKUC_GUARD

//--------------------------------------------------------------------------------------------------//
// namespace section
//--------------------------------------------------------------------------------------------------//
namespace narcotics
{
namespace exceptions
{

//--------------------------------------------------------------------------------------------------//
class ExCrossCheckUC : public basar::cmnutil::BasarRuntimeException
{
public:
	ExCrossCheckUC(const basar::ExceptInfo& sInfo);
	virtual ~ExCrossCheckUC(void);
};

} //namespace exceptions
} //namespace narcotics

#endif //EXCROSSCHECKUC_GUARD