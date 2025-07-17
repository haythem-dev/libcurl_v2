#ifndef GUARD_NARCOTICSPROTOCOL_H
#define GUARD_NARCOTICSPROTOCOL_H
//-------------------------------------------------------------------------------------------------//
/*! \file
 *  \brief  main interface for use case class narcotics protocol
 *  \author Kerstin Reuther
 *  \date   23.03.2006
 */
//-------------------------------------------------------------------------------------------------//
//includes
//-------------------------------------------------------------------------------------------------//

#include "usecasecontroller.h"

//-------------------------------------------------------------------------------------------------//
// namespace
//-------------------------------------------------------------------------------------------------//
namespace narcotics 
{
namespace useCase 
{

//-------------------------------------------------------------------------------------------------//
// class declaration
//-------------------------------------------------------------------------------------------------//
/*! \brief  Use Case class for use case register narcotics protocol.

	\n This is a final class.  
	\n throws no-throw */
//-------------------------------------------------------------------------------------------------//
class NarcoticsProtocolUC : public UseCaseController
{
public:
	/*! \brief The constructor for the use case narcotics protocol
		\n no-throw */
	NarcoticsProtocolUC();

	/*! \brief The destructor for the use case narcotics protocol
		\n no-throw */
	~NarcoticsProtocolUC();

	/*! \brief This method starts the use case narcotics protocol
		\n no-throw */
	basar::appl::EventReturnType run();

	/*! \brief std method; has to be the first call in run() if events should be caught;
		\n no-throw */
	void registerEventHandlers( const bool forSave	//!< register = true / deregister = false
										);

	SYSTEM_EVENT_HANDLER_DECLARATION( SearchProtocol )
	SYSTEM_EVENT_HANDLER_DECLARATION( SearchCatalog )

	/*! \brief This method manages to send protocol data to print preview
		\n no-throw */
	SYSTEM_EVENT_HANDLER_DECLARATION( PrintProtocol )
	
private:
	NarcoticsProtocolUC	( const NarcoticsProtocolUC& ); // forbidden
	NarcoticsProtocolUC& operator=( const NarcoticsProtocolUC& ); // forbidden

};

} //namespace useCase
} //namespace narcotics

#endif //GUARD_NARCOTICSPROTOCOL_H