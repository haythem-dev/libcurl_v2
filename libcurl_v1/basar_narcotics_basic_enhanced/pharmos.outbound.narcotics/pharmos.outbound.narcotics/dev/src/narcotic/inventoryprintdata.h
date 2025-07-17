//----------------------------------------------------------------------------
/*! \file
 *  \brief  print data class for inventory data
 *  \author Thomas Hörath
 *  \date   17.05.2006
 */
//----------------------------------------------------------------------------
#ifndef GUARD_INVENTORYPRINTDATA_H
#define GUARD_INVENTORYPRINTDATA_H

//----------------------------------------------------------------------------
//namespaces
//----------------------------------------------------------------------------
namespace narcotics
{

//-------------------------------------------------------------------------------------------------//
// class declaration
//-------------------------------------------------------------------------------------------------//
/*! \brief  Helper class for printing inventory data out of given property table

	\n This is a final class.  
	\n throw no-throw */
//-------------------------------------------------------------------------------------------------//
class InventoryPrintData : public basar::cmnutil::IPrintDataProvider
{
public:
					/*! \brief constructor
						\n no-throw */
					InventoryPrintData		(	 const basar::VarString & rPsFile,		//!< name of postscript file
												 const basar::VarString & rFormName,	//!< name of form (to find in ps-file)
												 const basar::db::aspect::AccessorPropertyTableRef inventoryData,	//!< property table containing data to print
												 const bool isCountList					//!< send data for count or stock list?
												 );

					/*! \brief destructor
						\n no-throw */
					~InventoryPrintData			();

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
	
	basar::VarString							m_PsFile;			//!< name of postscript file
	basar::VarString							m_FormName;			//!< name of form (to find in ps-file!)
	basar::db::aspect::AccessorPropertyTableRef m_InventoryData;	//!< property table containing data to print
	basar::db::aspect::AccessorPropertyTable_YIterator m_DataIterator;	//!< iterator containing data to print

	bool										m_SendHeaderLines;	//!< should header lines be send?
	bool										m_IsCountList;		//!< send data for count or stock list?
};

} //namespace narcotics

#endif //GUARD_INVENTORYPRINTDATA_H