//----------------------------------------------------------------------------
/*! \file
 *  \brief  print data class for narcotics book
 *  \author Thomas Hörath
 *  \date   26.05.2006
 */
//----------------------------------------------------------------------------
#ifndef GUARD_NARCBOOKPRINTDATA_H
#define GUARD_NARCBOOKPRINTDATA_H

//----------------------------------------------------------------------------
//namespaces
//----------------------------------------------------------------------------
namespace narcotics
{

//-------------------------------------------------------------------------------------------------//
// class declaration
//-------------------------------------------------------------------------------------------------//
/*! \brief  Helper class for printing narcotics book out of given property table

	\n This is a final class.  
	\n throw no-throw */
//-------------------------------------------------------------------------------------------------//
class NarcBookPrintData : public basar::cmnutil::IPrintDataProvider
{
public:
					/*! \brief constructor
						\n no-throw */
					NarcBookPrintData		(	 const basar::VarString & rPsFile,		//!< name of postscript file
												 const basar::VarString & rFormName,	//!< name of form (to find in ps-file)
												 const basar::db::aspect::AccessorPropertyTableRef narcBookData,	//!< property table containing data to print
												 const CollOfNarcArtBookings* const pCollBookings	//!< collection of bookings for each article in narcBookData
												 );

					/*! \brief destructor
						\n no-throw */
					~NarcBookPrintData			();

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
	NarcBookPrintData	( const NarcBookPrintData& ); // forbidden
	NarcBookPrintData& operator=			( const NarcBookPrintData& ); // forbidden

	/*! \brief return header data
		\n no-throw */
	basar::I18nString				getHeaderLines		();
	
	/*! \brief return next data line
		\n no-throw */
	basar::I18nString				getDataLine			();

	/*! \brief return next data line
		\n no-throw */
	basar::I18nString				getGroupHeaderLine	( basar::I18nString	lineType,	//!< type of line to identify in ps-file
														  basar::Int32		balance		//!< value of balance to print
														);

	
	basar::VarString							m_PsFile;			//!< name of postscript file
	basar::VarString							m_FormName;			//!< name of form (to find in ps-file!)
	basar::db::aspect::AccessorPropertyTableRef m_NarcArticles;		//!< property table containing all narcotic articles to print
	basar::db::aspect::AccessorPropertyTable_YIterator m_ItArticles;//!< iterator on article list

	//! \todo	raw pointer
	const CollOfNarcArtBookings * const			m_pCollBookings;	//!< collection of property tables containing bookings for all articles
	basar::db::aspect::AccessorPropertyTableRef m_currentBookings;	//!< property table containing bookings for current article
	basar::db::aspect::AccessorPropertyTable_YIterator m_ItBookings;//!< iterator on current bookings list

	bool										m_SendHeaderLines;	//!< should header lines be send?
	basar::Int32								m_currentArticleNo;	//!< articleno to which transactions are sent at the moment (to know when to print a new group header!)
	basar::Int32								m_LineNo;			//!< counter of current line on print page!
};

} //namespaces narcotics

#endif //GUARD_NARCBOOKPRINTDATA_H