//----------------------------------------------------------------------------
/*! \file
 *  \brief  print data class for protocol
 *  \author Thomas Hörath
 *  \date   23.06.2006
 */
//----------------------------------------------------------------------------
#ifndef GUARD_PROTOCOLPRINTDATA_H
#define GUARD_PROTOCOLPRINTDATA_H

//----------------------------------------------------------------------------
//namespaces
//----------------------------------------------------------------------------
namespace narcotics
{

//-------------------------------------------------------------------------------------------------//
// class declaration
//-------------------------------------------------------------------------------------------------//
/*! \brief  Helper class for printing narcotics protocol out of given property table

	\n This is a final class.  
	\n throw no-throw */
//-------------------------------------------------------------------------------------------------//
class ProtocolPrintData : public basar::cmnutil::IPrintDataProvider
{
public:
					/*! \brief constructor
						\n no-throw */
					ProtocolPrintData		(	 const basar::VarString & rPsFile,		//!< name of postscript file
												 const basar::VarString & rFormName,	//!< name of form (to find in ps-file)
												 const basar::db::aspect::AccessorPropertyTableRef protocolData,	//!< property table containing data to print
												 const CollOfBookingLogs* const pCollLogs,	//!< collection of logs for each booking in protocolData
												 const basar::VarString & rPeriod		//!< string containing period filter
												 );

					/*! \brief destructor
						\n no-throw */
					~ProtocolPrintData			();

	/*! \brief overwritten call-back function to send inventory data to queue of print class to add to formular
		\n no-throw */
	virtual void	onSendData					( basar::cmnutil::CollPrintRawInput & collPrintRawInput	//!< data collection
												);
protected:
	/*! \brief overwritten call-back function to send form name to printdataprovider
		\n no-throw */
	virtual	basar::VarString		getFormName			() const;
	
	/*! \brief overwritten call-back function to send ps-filename to printdataprovider
		\n no-throw */
	virtual	basar::VarString		getFormFileName		() const;

private:
	ProtocolPrintData	( const ProtocolPrintData& ); // forbidden
	ProtocolPrintData& operator=			( const ProtocolPrintData& ); // forbidden

	/*! \brief return header data
		\n no-throw */
	basar::I18nString				getHeaderLines		();
	
	/*! \brief return next data line
		\n no-throw */
	basar::I18nString				getDataLine			();

	/*! \brief return next data line
		\n no-throw */
	basar::I18nString				getGroupHeaderLine	( basar::I18nString	lineType	//!< type of line to identify in ps-file
														 );

	basar::VarString							m_PsFile;			//!< name of postscript file
	basar::VarString							m_FormName;			//!< name of form (to find in ps-file!)
	basar::VarString							m_Period;			//!< period for which protocol is printed
	basar::db::aspect::AccessorPropertyTableRef m_ChangedBookings;	//!< property table containing all bookings where logs are found to
	basar::db::aspect::AccessorPropertyTable_YIterator m_ItBookings;//!< iterator on changed bookings list

	const CollOfBookingLogs * const				m_pCollLogs;		//!< collection of property tables containing logs for all changed bookings
	basar::db::aspect::AccessorPropertyTableRef m_currentLogs;		//!< property table containing logs for current booking
	basar::db::aspect::AccessorPropertyTable_YIterator m_ItLogs;	//!< iterator on current logs list

	bool										m_SendHeaderLines;	//!< should header lines be send?
	basar::Int32								m_currentCatalogId;	//!< articleno to which transactions are sent at the moment (to know when to print a new group header!)
	basar::Int32								m_LineNo;			//!< counter of current line on print page!
};

} //namespaces narcotics

#endif //GUARD_PROTOCOLPRINTDATA_H