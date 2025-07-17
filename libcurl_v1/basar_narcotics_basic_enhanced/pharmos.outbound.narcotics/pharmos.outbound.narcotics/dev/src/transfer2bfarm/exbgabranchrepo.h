#ifndef EXBGABRANCHREPO_GUARD
#define EXBGABRANCHREPO_GUARD

//--------------------------------------------------------------------------------------------------//
// namespace section
//--------------------------------------------------------------------------------------------------//
namespace narcotics
{
namespace exceptions
{

//--------------------------------------------------------------------------------------------------//
class ExBGABranchRepo : public basar::cmnutil::BasarRuntimeException
{
public:
	ExBGABranchRepo(const basar::ExceptInfo& sInfo);
	virtual ~ExBGABranchRepo(void);
};

} //namespace exceptions
} //namespace narcotics

#endif //EXBGABRANCHREPO_GUARD