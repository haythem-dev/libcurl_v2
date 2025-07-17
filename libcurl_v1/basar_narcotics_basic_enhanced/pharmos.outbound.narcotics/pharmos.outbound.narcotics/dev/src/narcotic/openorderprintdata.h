//----------------------------------------------------------------------------
/*! \file
 *  \brief  print data class for half year report 4
 *  \author Thomas Hörath
 *  \date   25.07.2006
 */
//----------------------------------------------------------------------------
#ifndef GUARD_OPENORDERPRINTDATA_H
#define GUARD_OPENORDERPRINTDATA_H

//----------------------------------------------------------------------------
//namespaces
//----------------------------------------------------------------------------
namespace narcotics
{

//-------------------------------------------------------------------------------------------------//
// class declaration
//-------------------------------------------------------------------------------------------------//
/*! \brief  Helper class for printing open orders out of given property table

	\n This is a final class.  
	\n throw no-throw */
//-------------------------------------------------------------------------------------------------//
class OpenOrderPrintData : public basar::cmnutil::IPrintDataProvider
{
public:
					/*! \brief constructor
						\n no-throw */
					OpenOrderPrintData	(const basar::VarString & rPsFile,		//!< name of postscript file
										 const basar::VarString & rFormName,	//!< name of form (to find in ps-file)
										 const basar::db::aspect::AccessorPropertyTableRef printData,	//!< property table containing data to print
										 const basar::db::aspect::AccessorPropertyTable_YIterator yitBranch, //!< iterator containing infos to branch
										 const basar::VarString & rPeriod		//!< string containing period filter
										);

					/*! \brief destructor
						\n no-throw */
					~OpenOrderPrintData			();

	/*! \brief overwritten call-back function to send open order data to queue of print class to add to formular
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
	
	basar::VarString							m_PsFile;			//!< name of postscript file
	basar::VarString							m_FormName;			//!< name of form (to find in ps-file!)
	basar::VarString							m_Period;			//!< period for which protocol is printed
	
	basar::db::aspect::AccessorPropertyTableRef			m_OpenOrderData;//!< property table containing data to print
	basar::db::aspect::AccessorPropertyTable_YIterator	m_DataIterator;	//!< iterator containing data to print
	basar::db::aspect::AccessorPropertyTable_YIterator	m_BranchIt;		//!< iterator containing branch info

	bool												m_SendHeaderLines;	//!< should header lines be send?
};

} //namespaces narcotics

#endif //GUARD_OPENORDERPRINTDATA_H