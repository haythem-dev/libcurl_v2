//------------------------------------------------------------------------------
//includes
//------------------------------------------------------------------------------
#include "commonheader.h"
#include "usecasecontroler.h"
#include "transferbfarmappl.h"

//------------------------------------------------------------------------------
//namespace
//------------------------------------------------------------------------------
namespace narcotics
{
namespace usecase
{

//------------------------------------------------------------------------------
UseCaseController::UseCaseController()
{
}

//------------------------------------------------------------------------------
UseCaseController::~UseCaseController()
{
}

//------------------------------------------------------------------------------
narcotics::TransferBfarmApplBase& UseCaseController::getManager()
{
	return TransferBfarmAppl::getInstance();
}

} //namespace usecase
} //namespace narcotics