//----------------------------------------------------------------------------
/*! \file
 *  \brief  generate serial week for contribution vouchers
 *  \author Benjamin Endlicher
 *  \date   23.09.2010
 */
//----------------------------------------------------------------------------

#ifndef GUARD_WEEKCALCULATOR_H
#define GUARD_WEEKCALCULATOR_H

//-----------------------------------------------------------------------------------------------//
// includes
//-----------------------------------------------------------------------------------------------//

//------------------------------------------------------------------------------
//! filename generator
class WeekCalculator
{
public:
	/*! \brief constructor for WeekCalculator
		\n no-throw */
	WeekCalculator();
	
	/*! \brief The destructor for WeekCalculator
		\n no-throw */
	~WeekCalculator();

	//! calculate serial week for contribution voucher
	basar::Int32 calculateSerialWeek( const basar::VarString& startdatetxt );

	//! return number of weeks for given year
	basar::Int32 getNoOfWeeks( basar::Int16 year );

	//! return number of weeks between two given years
	basar::Int32 getNoOfWeeksBetweenYears( basar::Int16 startYear, basar::Int16 endYear );

private:
	WeekCalculator		( const WeekCalculator& ); // forbidden
	WeekCalculator& operator=			( const WeekCalculator& ); // forbidden
};

//------------------------------------------------------------------------------------------------------------//
#endif//GUARD_WEEKCALCULATOR_H