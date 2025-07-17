//---------------------------------------------------------------------------//
/*! \file usecasecontroller.h
 *  \brief abstract interface of base use case controller
 *  \author 
 *  \date 31.08.2009
 *  \version 00.00.00.01 first version
 */ //-----------------------------------------------------------------------//

//---------------------------------------------------------------------------//
// include section
//---------------------------------------------------------------------------//
#include "commonheader.h"
#include "usecasecontroller.h"
#include "narctransferappl.h"

//---------------------------------------------------------------------------//
// namespace section
//---------------------------------------------------------------------------//
namespace narcotics
{
namespace useCase  
{

//---------------------------------------------------------------------------//
/*!	\throw no-throw */
UseCaseController::UseCaseController()
{
}

//---------------------------------------------------------------------------//
/*!	\throw no-throw */
UseCaseController::~UseCaseController()
{
}

//---------------------------------------------------------------------------//
/*!	\throw no-throw  
	\return const reference to application controller */
NarcTransferApplBase& UseCaseController::getManager()
{
	return NarcTransferAppl::getInstance();
}

//---------------------------------------------------------------------------//
//!	\throw no-throw  
void UseCaseController::registerEventHandlers( const bool )
{
}

} //namespace useCase
} //namespace narcotics