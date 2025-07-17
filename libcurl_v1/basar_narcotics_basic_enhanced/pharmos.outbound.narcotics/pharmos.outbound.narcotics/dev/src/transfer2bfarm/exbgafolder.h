#ifndef EXBGAFOLDER_GUARD
#define EXBGAFOLDER_GUARD

//--------------------------------------------------------------------------------------------------//
// namespace section
//--------------------------------------------------------------------------------------------------//
namespace narcotics
{
namespace exceptions
{

//--------------------------------------------------------------------------------------------------//
class ExBGAFolder : public basar::cmnutil::BasarRuntimeException
{
public:
	ExBGAFolder(const basar::ExceptInfo& sInfo);
	virtual ~ExBGAFolder(void);
};

} //namespace exceptions
} //namespace narcotics

#endif //EXBGAFOLDER_GUARD