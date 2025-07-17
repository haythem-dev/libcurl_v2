//----------------------------------------------------------------------------
/*! \file
 *  \brief  print data class for manually created contribution voucher
 *  \author Benjamin Endlicher
 *  \date   04.05.2010
 */
//----------------------------------------------------------------------------
#ifndef GUARD_MANUALCONTRIBUTIONVOUCHERPRINTDATA_H
#define GUARD_MANUALCONTRIBUTIONVOUCHERPRINTDATA_H

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
class ManualContributionVoucherPrintData : public basar::cmnutil::IPrintDataProvider
{
public:
					/*! \brief constructor
						\n no-throw */
	ManualContributionVoucherPrintData		(	 const basar::VarString & rPsFile,										//!< name of postscript file
												 const basar::VarString & rFormName,									//!< name of form (to find in ps-file)
												 const basar::db::aspect::AccessorPropertyTableRef articleData,			//!< iterator containing transaction data
												 const basar::db::aspect::AccessorPropertyTable_YIterator branchData,	//!< iterator containing branch data
												 const basar::db::aspect::AccessorPropertyTable_YIterator customerData	//!< iterator containing customer data
												 );

					/*! \brief destructor
						\n no-throw */
	~ManualContributionVoucherPrintData			();

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
	basar::I18nString				getArticleDataLines();

	basar::I18nString				getDateLine();

	basar::I18nString				getBranchCustomerDataLine();
	
	basar::VarString									m_PsFile;		//!< name of postscript file
	basar::VarString									m_FormName;		//!< name of form (to find in ps-file!)
	basar::Int32										m_RowCounter;	//!< member counts calls of getArticleDataLines
	basar::Int32										m_MAXIMUM_NUMBER_OF_PRINTABLE_LINES;
	
	basar::db::aspect::AccessorPropertyTableRef			m_ArticleData;  //!< propTab containing article data
	basar::db::aspect::AccessorPropertyTable_YIterator	m_ArticleIt;	//!< iterator containing article data
	basar::db::aspect::AccessorPropertyTable_YIterator	m_BranchIt;		//!< iterator containing branch data
	basar::db::aspect::AccessorPropertyTable_YIterator	m_CustomerIt;	//!< iterator containing Customer data

	bool							m_DataSent;
	bool							m_DateLineSent;
	bool							m_ArticleDataSent;
	bool							m_BranchCustomerDataSent;
};

} //namespaces narcotics

#endif //GUARD_MANUALCONTRIBUTIONVOUCHERPRINTDATA_H