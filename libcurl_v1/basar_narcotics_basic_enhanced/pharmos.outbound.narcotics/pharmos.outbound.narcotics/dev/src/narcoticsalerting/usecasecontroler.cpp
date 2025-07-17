//------------------------------------------------------------------------------
//includes
//------------------------------------------------------------------------------
#include "commonheader.h"
#include "usecasecontroler.h"
#include "narcoticsalertingapp.h"

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
narcotics::NarcoticsAlertingApplBase& UseCaseController::getManager()
{
	return NarcoticsAlertingAppl::getInstance();
}

} //namespace usecase
} //namespace narcotics