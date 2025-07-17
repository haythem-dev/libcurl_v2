//-------------------------------------------------------------------------------------------------//
/*! \file
 *  \brief  abstract interface of a base use case controller
 *  \author Thomas Hörath
 *  \date   15.03.2006
 */
//-------------------------------------------------------------------------------------------------//
#ifndef GUARD_LIBBASARLOGIN_USECASECONTROLLER_H
#define GUARD_LIBBASARLOGIN_USECASECONTROLLER_H

//-------------------------------------------------------------------------------------------------//
#include <boost/shared_ptr.hpp>

//-------------------------------------------------------------------------------------------------//
// namespace
//-------------------------------------------------------------------------------------------------//
namespace basar {
namespace login {
	
class LoginAppl; // forward declaration

//-------------------------------------------------------------------------------------------------//
namespace useCase {

//-------------------------------------------------------------------------------------------------//
// class declaration
//-------------------------------------------------------------------------------------------------//
/*! \brief  Use Case class to derive all use cases from.

	\n This is a base class.  
	\n throws no-throw */
//-------------------------------------------------------------------------------------------------//
class UseCaseController
{
public:
	/*! \brief default constructor
		\n no-throw */
	UseCaseController();
	
	/*! \brief default destructor
		\n no-throw */
	virtual ~UseCaseController();

	/*! \brief This method returns a reference to the application controller
		\n no-throw */
	const boost::shared_ptr<LoginAppl>& getAppl();
		
	/*! \brief This method starts the use case
		\n no-throw */
	virtual bool run() = 0;
};

//---------------------------------------------------------------------------
} // useCase
} // login
} // basar

#endif //GUARD
