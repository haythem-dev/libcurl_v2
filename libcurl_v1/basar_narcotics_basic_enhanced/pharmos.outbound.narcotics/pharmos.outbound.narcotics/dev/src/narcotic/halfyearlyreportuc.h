#ifndef GUARD_HALFYEARLYREPORT_H
#define GUARD_HALFYEARLYREPORT_H
//-------------------------------------------------------------------------------------------------//
/*! \file
 *  \brief  main interface for use case class half yearly reports
 *  \author Roland Kiefert
 *  \date   11.04.2006
 */
//-------------------------------------------------------------------------------------------------//
// includes
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
/*! \brief  Use Case class for use case half yearly reports.

	\n This is a final class.  
	\n throws no-throw */
//-------------------------------------------------------------------------------------------------//
class HalfYearlyReportUC : public UseCaseController
{
public:
	/*! \brief The constructor for the use case half yearly reports
		\n no-throw */
	HalfYearlyReportUC();
	/*! \brief The destructor for the use case half yearly reports
		\n no-throw */
	~HalfYearlyReportUC();

	/*! \brief This method start the use case half yearly reports
		\n no-throw */
	basar::appl::EventReturnType run(); 

	/*! \brief std method; has to be the first call in run() if events should be caught;
		\n no-throw */
	void registerEventHandlers( const bool forSave	//!< register = true / deregister = false
										);

	/*! \brief This method processes the event show the report C1,C2 or C4 in the use case half yearly reports
		\n no-throw */
	SYSTEM_EVENT_HANDLER_DECLARATION( ShowHalfYearlyReport )
	/*! \brief This method processes the event print the C1,C2 or C4 report in the use case half yearly reports
		\n no-throw */
	SYSTEM_EVENT_HANDLER_DECLARATION( PrintHalfYearlyReport )
	/*! \brief This method processes the event delete a line from the report C4 in the use case half yearly reports
		\n no-throw */
	SYSTEM_EVENT_HANDLER_DECLARATION( DeleteHalfYearlyReport )
	
private:
	HalfYearlyReportUC( const HalfYearlyReportUC& ); // forbidden
	HalfYearlyReportUC& operator=( const HalfYearlyReportUC& ); // forbidden
	viewConn::ReportTypeEnum m_reportType;
};

} //namespace useCase
} //namespace narcotics

#endif //GUARD_HALFYEARLYREPORT_H