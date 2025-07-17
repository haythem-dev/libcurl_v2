//----------------------------------------------------------------------------
/*! \file
 *  \brief  print data class for dissolve receipt data
 *  \author Thomas Hörath
 *  \date   29.06.2006
 */
//----------------------------------------------------------------------------
#ifndef GUARD_DISSOLVERECEIPTPRINTDATA_H
#define GUARD_DISSOLVERECEIPTPRINTDATA_H

//----------------------------------------------------------------------------
//namespaces
//----------------------------------------------------------------------------
namespace narcotics
{

//-------------------------------------------------------------------------------------------------//
// class declaration
//-------------------------------------------------------------------------------------------------//
/*! \brief  Helper class for printing dissolve receipt data out of given property table

	\n This is a final class.  
	\n throw no-throw */
//-------------------------------------------------------------------------------------------------//
class DissolveReceiptPrintData : public basar::cmnutil::IPrintDataProvider
{
public:
					/*! \brief constructor
						\n no-throw */
					DissolveReceiptPrintData(	 const basar::VarString & rPsFile,		//!< name of postscript file
												 const basar::VarString & rFormName,	//!< name of form (to find in ps-file)
												 const basar::db::aspect::AccessorPropertyTableRef printData,	//!< property table containing data to print
												 const basar::db::aspect::AccessorPropertyTable_YIterator yitBranch, //!< YIteraor containing branch infos to print
												 const basar::db::aspect::AccessorPropertyTable_YIterator yitDate //!< YIteraor containing date infos to print
												 );

					/*! \brief destructor
						\n no-throw */
					~DissolveReceiptPrintData			();

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
	/*! \brief return header data
		\n no-throw */
	basar::I18nString				getHeaderLines		();
	
	/*! \brief return next data line
		\n no-throw */
	basar::I18nString				getDataLine			();

	basar::Int32								m_Counter;			//!< declares current position of order
	
	basar::VarString							m_PsFile;			//!< name of postscript file
	basar::VarString							m_FormName;			//!< name of form (to find in ps-file!)
	basar::db::aspect::AccessorPropertyTableRef m_DisReceiptData;	//!< property table containing data to print
	basar::db::aspect::AccessorPropertyTable_YIterator m_DataIterator;	//!< iterator containing data to print
	basar::db::aspect::AccessorPropertyTable_YIterator m_BranchIt;	//!< iterator containing branch info
	basar::db::aspect::AccessorPropertyTable_YIterator m_DateIt;	//!< iterator containing date info

	bool										m_HeaderDataSent;	//!< has header data been sent
	bool										m_ArticleDataSent;	//!< has article data been sent
	bool										m_Logged;			//!< has logging already been executed
	basar::cmnutil::DateTime					m_Today;			//!< get current date
};

} //namespace narcotics

#endif	//GUARD_DISSOLVERECEIPTPRINTDATA_H