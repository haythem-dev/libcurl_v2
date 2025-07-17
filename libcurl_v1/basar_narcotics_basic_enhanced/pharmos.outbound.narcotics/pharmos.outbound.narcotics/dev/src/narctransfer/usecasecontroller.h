//---------------------------------------------------------------------------//
/*! \file usecasecontroller.h
 *  \brief abstract interface of base use case controller
 *  \author 
 *  \date 31.08.2009
 *  \version 00.00.00.01 first version
 */
//-----------------------------------------------------------------------//

#ifndef GUARD_USECASECONTROLLER_H
#define GUARD_USECASECONTROLLER_H

//---------------------------------------------------------------------------//
// namespace section
//---------------------------------------------------------------------------//
namespace narcotics
{	
    class NarcTransferApplBase; // forward declaration

namespace useCase  
{

//---------------------------------------------------------------------------//
/*! \brief Classname UseCaseController - base class for use cases
 *  \author 
 *  \date 19.11.2008
 */ //-----------------------------------------------------------------------//
class UseCaseController
{
public:
	/*! \brief std-constructor  
		\n no-throw */
	UseCaseController();
	
	/*! \brief std-destructor  
		\n no-throw */
	virtual ~UseCaseController();

	/*! \brief This method returns a reference to the application controller
		\n no-throw */
	NarcTransferApplBase& getManager();
	
	/*! \brief This method starts the use case - each use case must have this method / overwrite it (pure virtual)
		\n no-throw */
	virtual basar::appl::EventReturnType run(  
		basar::appl::IEventSource& rSource,						//!< source where event is thrown
		basar::db::aspect::AccessorPropertyTable_YIterator yitFromEvSrc
    ) = 0;

	/*! \brief std method; has to be the first call in run() if events should be caught;
		\n no-throw */
	virtual void registerEventHandlers(const bool forSave	//!< register = true / deregister = false
										);
private:
	/*! \brief copy constructor  \n     no-throw */
	UseCaseController                           ( const UseCaseController     & r );
    /*! \brief assign operator  \n     no-throw */
	UseCaseController &     operator =			( const UseCaseController     & r );
};

} //namespace useCase
} //namespace narcotics

#endif //GUARD_USECASECONTROLLER_H