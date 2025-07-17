#ifndef USECASECONTROLLER_GUARD
#define USECASECONTROLLER_GUARD

//------------------------------------------------------------------------------
//namespace
//------------------------------------------------------------------------------
namespace narcotics {
	class NarcoticsAlertingApplBase;
}

//------------------------------------------------------------------------------

namespace narcotics {
namespace usecase {

//------------------------------------------------------------------------------

class UseCaseController
{
public:
	UseCaseController();
	
	virtual ~UseCaseController();

	/*! \brief return a reference to the application manager
		\n no-throw */

	narcotics::NarcoticsAlertingApplBase& getManager();
	
	/*! \brief pure virtual run; has to be overwritten by each use case
		\n no-throw */
	virtual basar::appl::EventReturnType run() = 0;
};

//------------------------------------------------------------------------------

} //namespace usecase
} //namespace narcotics

//------------------------------------------------------------------------------

#endif //USECASECONTROLLER_GUARD

//------------------------------------------------------------------------------
