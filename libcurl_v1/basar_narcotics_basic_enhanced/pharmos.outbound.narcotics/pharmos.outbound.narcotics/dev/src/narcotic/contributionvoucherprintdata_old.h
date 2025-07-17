//----------------------------------------------------------------------------
/*! \file
 *  \brief  print data class contribution voucher
 *  \author Benjamin Endlicher
 *  \date   09.09.2010
 */
//----------------------------------------------------------------------------
#ifndef GUARD_CONTRIBUTIONVOUCHERPRINTDATA_H
#define GUARD_CONTRIBUTIONVOUCHERPRINTDATA_H

//-------------------------------------------------------------------------------------------------//
// defines
//-------------------------------------------------------------------------------------------------//
//#define	UADM_PRINT_SRV	"PRINTSERVER"	//!< parameter in useradministration to read print-server (for ps-print)
//#define	UADM_PR_NAME	"PRINTERNAME"	//!< parameter in useradministration to read name of printer (for ps-print via lpr)

//----------------------------------------------------------------------------
namespace narcotics {

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
PrintDataBase	   (const basar::VarString & rPsFile,										//!< name of postscript file
									const basar::VarString & rFormName,										//!< name of form (to find in ps-file)
									const basar::db::aspect::AccessorPropertyTableRef articleData,			//!< iterator containing transaction data
									const basar::db::aspect::AccessorPropertyTable_YIterator branchData,	//!< iterator containing branch data
									const basar::db::aspect::AccessorPropertyTable_YIterator customerData,	//!< iterator containing customer data
									const basar::db::aspect::AccessorPropertyTable_YIterator signaturelnk,	//!< iterator containing link to digital user signature
									basar::Int32 beginOfOrder,												//!< int declaring first booking of an order
									basar::Int32 endOfOrder,												//!< int declaring last booking of an order
									basar::VarString form																//!< bool defines if Empfangsbestätigung shall be printed or Abgabemeldung created as pdf
									);	

					/*! \brief destructor
						\n no-throw */
	~PrintDataBase			();

	/*! \brief overwritten call-back function to send inventory data to queue of print class to add to formular
		\n no-throw */
	virtual void	onSendData					(basar::cmnutil::CollPrintRawInput & collPrintRawInput	//!< data collection
												);
protected:
	/*! \brief overwritten call-back function to send form name to printdataprovider
		\n no-throw */
	virtual	basar::VarString		getFormName			() const;
	
	/*! \brief overwritten call-back function to send ps-filename to printdataprovider
		\n no-throw */
	virtual	basar::VarString		getFormFileName		() const;

private:
    PrintDataBase( const PrintDataBase& );
    PrintDataBase& operator=( const PrintDataBase& );

	/*! \brief return complete data for contribution voucher; only called once!
		\n no-throw */
	basar::VarString				getArticleDataLines();

	basar::VarString				getBranchDataLine();

	basar::VarString				getCustomerDataLine();

	basar::VarString				getTitleLine();

	basar::VarString				getGlobalHeaderLines();

	basar::VarString				getGlobalFooterLines();

	basar::VarString				getData();

	/*! \brief get pack unit + unit extension out of unity (packageunit)
		\n no-throw */
	//void							splitUnity	(basar::VarString unity,		//!< unity (packageunit) which will be splitted
	//											basar::VarString & rPack,		//!< ref containing pack unit at the end
	//											basar::VarString & rExtension	//!< ref containing unit extension at the end
	//											);

	basar::VarString									m_PsFile;		//!< name of postscript file
	basar::VarString									m_FormName;		//!< name of form (to find in ps-file!)
	basar::Int32										m_Counter;		//!< declares current position of order
	basar::Int32										m_EndOfOrder;	//!< declares last position of an order
	basar::VarString									m_Form;		//!< bool declares if Abgabemeldung, Lieferscheindoppel or 
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

//----------------------------------------------------------------------------
} // namespace narcotics

//----------------------------------------------------------------------------
#endif	// GUARD
