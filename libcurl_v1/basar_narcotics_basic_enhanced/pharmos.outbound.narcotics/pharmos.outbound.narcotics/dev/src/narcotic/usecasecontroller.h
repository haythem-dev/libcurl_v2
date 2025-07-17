#ifndef GUARD_USECASECONTROLLER_H
#define GUARD_USECASECONTROLLER_H
//-------------------------------------------------------------------------------------------------//
/*! \file
 *  \brief  main interface for use a base use case controller
 *  \author Roland Kiefert
 *  \date   07.03.2006
 */
//-------------------------------------------------------------------------------------------------//

//-------------------------------------------------------------------------------------------------//
// namespace
//-------------------------------------------------------------------------------------------------//
namespace narcotics 
{
	class NarcoticsApplBase;
namespace useCase 
{

//-------------------------------------------------------------------------------------------------//
// class declaration
//-------------------------------------------------------------------------------------------------//
/*! \brief  Use Case class for base use case controller.

	\n This is a base class.  
	\n throws no-throw */
//-------------------------------------------------------------------------------------------------//
class UseCaseController
{
public:
	UseCaseController();
	virtual ~UseCaseController();

	/*! \brief return a reference to the application manager
		\n no-throw */
	NarcoticsApplBase& getManager();
	
	/*! \brief pure virtual run; has to be overwritten by each use case
		\n no-throw */
	virtual basar::appl::EventReturnType run() = 0;

	/*! \brief std method; has to be the first call in use cases run() if events should be caught;
		\n no-throw */
	virtual void registerEventHandlers( const bool forSave	//!< register = true / deregister = false
										) = 0;
};

} //namespace useCase
} //namespace narcotics

#endif //GUARD_USECASECONTROLLER_H