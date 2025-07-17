//----------------------------------------------------------------------------
/*! \file
 *  \brief  print data class for contribution voucher
 *  \author Thomas Hörath
 *  \date   25.07.2006
 */
//----------------------------------------------------------------------------
#ifndef GUARD_SELLOFFPRINTDATA_H
#define GUARD_SELLOFFPRINTDATA_H

//-------------------------------------------------------------------------------------------------//
// namespaces
//-------------------------------------------------------------------------------------------------//
namespace narcotics
{

//-------------------------------------------------------------------------------------------------//
// class declaration
//-------------------------------------------------------------------------------------------------//
/*! \brief  Helper class for printing contribution voucher out of given YIterators

	\n This is a final class.  
	\n throw no-throw */
//-------------------------------------------------------------------------------------------------//
class SellOffPrintData : public basar::cmnutil::IPrintDataProvider
{
public:
					/*! \brief constructor
						\n no-throw */
					SellOffPrintData		(	 const basar::VarString & rPsFile,		//!< name of postscript file
												 const basar::VarString & rFormName,	//!< name of form (to find in ps-file)
												 const basar::db::aspect::AccessorPropertyTable_YIterator transactData,	//!< iterator containing transaction data
												 const basar::db::aspect::AccessorPropertyTable_YIterator branchData,	//!< iterator containing branch data
												 const basar::db::aspect::AccessorPropertyTable_YIterator custsupplData	//!< iterator containing customer / supplier data
												 );

					/*! \brief destructor
						\n no-throw */
					~SellOffPrintData			();

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
	/*! \brief return complete data for contribution voucher; only called once!
		\n no-throw */
	basar::I18nString				getDataLines		();
	
	basar::VarString									m_PsFile;		//!< name of postscript file
	basar::VarString									m_FormName;		//!< name of form (to find in ps-file!)
	
	basar::db::aspect::AccessorPropertyTable_YIterator	m_TransactIt;	//!< iterator containing transaction data
	basar::db::aspect::AccessorPropertyTable_YIterator	m_BranchIt;		//!< iterator containing branch data
	basar::db::aspect::AccessorPropertyTable_YIterator	m_CustSupplIterator;	//!< iterator containing Customer / supplier data

	bool							m_DataSent;
};

} //namespace narcotics

#endif //GUARD_SELLOFFPRINTDATA_H