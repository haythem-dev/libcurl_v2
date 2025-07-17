//----------------------------------------------------------------------------
/*! \file
 *  \brief  print data class contribution voucher
 *  \author Harald Bayer
 *  \date   17.03.2011
 */
//----------------------------------------------------------------------------
#ifndef GUARD_PRINTDATABASE_H
#define GUARD_PRINTDATABASE_H

//----------------------------------------------------------------------------
//namespaces
//----------------------------------------------------------------------------
namespace narcotics
{

//-------------------------------------------------------------------------------------------------//
// class declaration
//-------------------------------------------------------------------------------------------------//
/*! \brief  Helper class for printing contribution voucher out of given YIterators

	\n This is a final class.  
	\n throw no-throw */	
//-------------------------------------------------------------------------------------------------//

class PrintDataBase : public basar::cmnutil::IPrintDataProvider
{
public:
					/*! \brief constructor
						\n no-throw */
	PrintDataBase	  ( const basar::db::aspect::AccessorPropertyTableRef articleData,			//!< iterator containing transaction data
						const basar::db::aspect::AccessorPropertyTable_YIterator branchData,	//!< iterator containing branch data
						const basar::db::aspect::AccessorPropertyTable_YIterator customerData,	//!< iterator containing customer data
						const basar::db::aspect::AccessorPropertyTable_YIterator signaturelnk,	//!< iterator containing link to digital user signature
						basar::Int32 beginOfOrder,												//!< int declaring first booking of an order
						basar::Int32 endOfOrder 												//!< int declaring last booking of an order									
						);	

					/*! \brief destructor
						\n no-throw */
	~PrintDataBase			();

	/*! \brief overwritten call-back function to send inventory data to queue of print class to add to formular
		\n no-throw */
	virtual void	onSendData					( basar::cmnutil::CollPrintRawInput & collPrintRawInput	//!< data collection
												);

	virtual basar::VarString getPostPrintingInformation();

protected:
	/*! \brief overwritten call-back function to send form name to printdataprovider
		\n no-throw */
	virtual	basar::VarString		getFormName			() const = 0;	
		
	/*! \brief overwritten call-back function to send ps-filename to printdataprovider
		\n no-throw */
	virtual	basar::VarString		getFormFileName		() const = 0;

	/*! \brief return complete data for contribution voucher; only called once!
		\n no-throw */
	virtual basar::VarString		getArticleDataLines();

	virtual basar::VarString    	getBranchDataLine();

	virtual basar::VarString		getCustomerDataLine();

	/*! \brief return the title line (Abgabemeldung, Lieferscheindoppel, usw)
		\n no-throw */
	virtual basar::VarString		getTitleLine() = 0;

	/*! \brief does tourid have to be printed (only needed on Empfangsbestätigung, Lieferschein)
		\n no-throw */
	virtual bool					printTour() = 0;

	virtual bool					printDeliveryNoteNumber() = 0;

	virtual basar::VarString		getGlobalHeaderLines();

	virtual basar::VarString		getGlobalFooterLines();

	virtual basar::VarString		getData();
		
	basar::Int32										m_Counter;		//!< declares current position of order
	basar::Int32										m_EndOfOrder;	//!< declares last position of an order	
																		//!< Empfangsbestätigung & Lieferschein shall be created
	basar::db::aspect::AccessorPropertyTableRef			m_ArticleData;  //!< propTab containing article data
	basar::db::aspect::AccessorPropertyTable_YIterator	m_ArticleIt;	//!< iterator containing article data
	basar::db::aspect::AccessorPropertyTable_YIterator	m_BranchIt;		//!< iterator containing branch data
	basar::db::aspect::AccessorPropertyTable_YIterator	m_CustomerIt;	//!< iterator containing Customer data
	basar::db::aspect::AccessorPropertyTable_YIterator	m_SignatureLnkIt;	//!< iterator link to user signature

	bool							m_HeaderDataSent;
	bool							m_FooterDataSent;
	bool							m_ArticleDataSent;
	bool							m_Logged;
};

} //namespace narcotics

#endif	//GUARD_PRINTDATABASE_H