#ifndef STARTENDDATECALCULATOR_GUARD
#define STARTENDDATECALCULATOR_GUARD

//------------------------------------------------------------------------------
//includes
//------------------------------------------------------------------------------
#include "dateinfo.h"

//------------------------------------------------------------------------------
//namespace
//------------------------------------------------------------------------------
namespace narcotics
{
namespace datecalc
{

//------------------------------------------------------------------------------
class StartEndDateCalculator
{
public:
	virtual DateInfo calculateStartEndDate(basar::Date startDate, basar::Int32 weeksSinceStart); 		

	StartEndDateCalculator(void);
	~StartEndDateCalculator(void);
};

} //namespace datecalc
} //namespace narcotics

#endif //STARTENDDATECALCULATOR_GUARD