//----------------------------------------------------------------------------
/*! \file
 *  \brief  print data class for transactions of articles (1 article - n customer / supplier) data
 *  \author Thomas Hörath
 *  \date   20.07.2006
 */
//----------------------------------------------------------------------------
#ifndef GUARD_TRANSACTARTICLEPRINTDATA_H
#define GUARD_TRANSACTARTICLEPRINTDATA_H

//----------------------------------------------------------------------------
namespace narcotics {

//-------------------------------------------------------------------------------------------------//
// class declaration
//-------------------------------------------------------------------------------------------------//
/*! \brief  Helper class for printing dissolve receipt data out of given property table

	\n This is a final class.  
	\n throw no-throw */
//-------------------------------------------------------------------------------------------------//
class TransactArticlePrintData : public basar::cmnutil::IPrintDataProvider
{
public:
					/*! \brief constructor
						\n no-throw */
					TransactArticlePrintData(	 const basar::VarString & rPsFile,		//!< name of postscript file
												 const basar::VarString & rFormName,	//!< name of form (to find in ps-file)
												 const basar::db::aspect::AccessorPropertyTableRef printData,	//!< property table containing data (supplier / customer) to print
												 const basar::db::aspect::AccessorPropertyTable_YIterator yitArticle //!< YIterator containing article infos to print
												 );

					/*! \brief destructor
						\n no-throw */
					~TransactArticlePrintData			();

	/*! \brief overwritten call-back function to send inventory data to queue of print class to add to formular
		\n no-throw */
	virtual void					onSendData			( basar::cmnutil::CollPrintRawInput & collPrintRawInput	//!< data collection
														);
protected:
	/*! \brief overwritten call-back function to send form name to printdataprovider
		\n no-throw */
	virtual	basar::VarString		getFormName			() const;
	
	/*! \brief overwritten call-back function to send ps-filename to printdataprovider
		\n no-throw */
	virtual	basar::VarString		getFormFileName		() const;

private:
	/*! \brief return header data
		\n no-throw */
	basar::I18nString				getHeaderLines		();
	
	/*! \brief return next data line
		\n no-throw */
	basar::I18nString				getDataLine			();
	
	basar::VarString							m_PsFile;			//!< name of postscript file
	basar::VarString							m_FormName;			//!< name of form (to find in ps-file!)
	basar::db::aspect::AccessorPropertyTableRef m_CustSupplData;	//!< property table containing data to print
	basar::db::aspect::AccessorPropertyTable_YIterator m_DataIterator;	//!< iterator containing data to print
	basar::db::aspect::AccessorPropertyTable_YIterator m_ArticleIt;	//!< iterator containing article info

	bool										m_SendHeaderLines;	//!< should header lines be send?
};

} //namespaces narcotics

#endif //GUARD_TRANSACTARTICLEPRINTDATA_H