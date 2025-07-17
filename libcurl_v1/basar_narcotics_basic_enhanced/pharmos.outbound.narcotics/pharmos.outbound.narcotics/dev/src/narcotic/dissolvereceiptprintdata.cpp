//-------------------------------------------------------------------------------------------------//
/*! \file
 *  \brief  implemtation of dissolve receipt print data
 *  \author	Thomas Hörath
 *  \date   29.06.2006
 */
//-------------------------------------------------------------------------------------------------//

//-------------------------------------------------------------------------------------------------//
// includes
//-------------------------------------------------------------------------------------------------//
#include "commonheader.h"
#include "dissolvereceiptprintdata.h"
#include "loggerpool.h"

#define DISREC_DELIM		"|"
#define DISREC_NEWLINE		"\n"

//-------------------------------------------------------------------------------------------------//
// namespaces
//-------------------------------------------------------------------------------------------------//
namespace narcotics {

//------------------------------------------------------------------------------
/*!	\throw	no-throw */
DissolveReceiptPrintData::DissolveReceiptPrintData( const basar::VarString & rPsFile, 
													const basar::VarString & rFormName, 
													const basar::db::aspect::AccessorPropertyTableRef printData,
													const basar::db::aspect::AccessorPropertyTable_YIterator yitBranch,
													const basar::db::aspect::AccessorPropertyTable_YIterator yitDate
													)
					 : m_PsFile( rPsFile ),
					   m_FormName( rFormName ),
					   m_DisReceiptData( printData ),
					   m_DataIterator( m_DisReceiptData.begin() ),
					   m_BranchIt( yitBranch ),
					   m_HeaderDataSent( false ),
					   m_ArticleDataSent( false ),
					   m_DateIt( yitDate ),
					   m_Counter( 0 )
{
}

//------------------------------------------------------------------------------
/*!	\throw	no-throw */
DissolveReceiptPrintData::~DissolveReceiptPrintData()
{
}

//------------------------------------------------------------------------------
/*!	\throw	no-throw */
basar::I18nString DissolveReceiptPrintData::getHeaderLines()
{
	basar::VarString headerLines;
	basar::I18nString reportCompanyName;

	basar::I18nString branchnotxt;
	branchnotxt.itos(basar::login::Manager::getInstance().getAreaID());
	if (!basar::login::Manager::getInstance().getParaValue(branchnotxt,UADM_REPORT_COMPANY_NAME, reportCompanyName))
	{
		if (!basar::login::Manager::getInstance().getParaValue(UADM_REPORT_COMPANY_NAME, reportCompanyName))
			reportCompanyName = "PHOENIX Pharmahandel GmbH & Co. KG";
	}
	
	if( m_DateIt.contains( constants::FROMDATE ) ){
		m_Today.setDate( m_DateIt.getString( constants::FROMDATE ).stoi() );
	}
	if( !m_BranchIt.isNull() && !m_BranchIt.isEnd() )
	{
		if( m_BranchIt.getInt16( constants::BRANCHNO ) == 21 ) // branchno for branch Hageda-Stumpf (21)
		{
			headerLines.format("%s%s%s%s%s%s%s%ld%s%s%s%s%s%s%s%s%s%s%s%s%s%s%s%s%s",
						"h01 ",
						basar::login::Manager::getInstance().getLanguageID().c_str(),
						DISREC_NEWLINE,
						"h02 Vernichtungsdatum ",
						m_Today.toStrDate().c_str(),
						DISREC_NEWLINE,
						"h03 BtM-Nummer ",
						m_BranchIt.getInt32("bgano"),
						DISREC_NEWLINE,
						"h04 ",
						QApplication::translate("DissolveReceiptPrintData", reportCompanyName.c_str()).toLocal8Bit().data(),
						DISREC_NEWLINE,
						"h05",
						DISREC_NEWLINE,
						"h06 ",
						QApplication::translate("DissolveReceiptPrintData", "Narcotics-Department").toLocal8Bit().data(),
						DISREC_NEWLINE,
						"h07 ",
						m_BranchIt.getString("street").c_str(),
						DISREC_NEWLINE,
						"h08 ",
						m_BranchIt.getString("postcode").c_str(),
						" ",
						m_BranchIt.getString("location").c_str(),
						DISREC_NEWLINE
						);	
		}
		else 
		{
			// print header lines using new print form
			headerLines.format("%s%s%s%s%s%s%s%ld%s%s%s%s%s%s%s%s%s%s%s%s%s%s%s%s%s%s%s",
						"h01 ",
						basar::login::Manager::getInstance().getLanguageID().c_str(),
						DISREC_NEWLINE,
						"h02 Vernichtungsdatum ",
						m_Today.toStrDate().c_str(),
						DISREC_NEWLINE,
						"h03 BtM-Nummer ",
						m_BranchIt.getInt32("bgano"),
						DISREC_NEWLINE,
						"h04 ",
						QApplication::translate("DissolveReceiptPrintData", reportCompanyName.c_str()).toLocal8Bit().data(),
						DISREC_NEWLINE,
						"h05 ",
						QApplication::translate("DissolveReceiptPrintData", "Depot ").toLocal8Bit().data(),
						m_BranchIt.getString("location_long").c_str(),
						DISREC_NEWLINE,
						"h06 ",
						QApplication::translate("DissolveReceiptPrintData", "Narcotics-Department").toLocal8Bit().data(),
						DISREC_NEWLINE,
						"h07 ",
						m_BranchIt.getString("street").c_str(),
						DISREC_NEWLINE,
						"h08 ",
						m_BranchIt.getString("postcode").c_str(),
						" ",
						m_BranchIt.getString("location").c_str(),
						DISREC_NEWLINE
						);	
		}
	}
	else // no branch data found!
	{			
		// print header lines using new print form
		headerLines.format("%s%s%s%s%s%s%s%ld%s%s%s%s%s%s%s%s%s%s%s%s%s%s",
						"h01 ",
						basar::login::Manager::getInstance().getLanguageID().c_str(),
						DISREC_NEWLINE,
						"h02 Vernichtungsdatum ",
						m_Today.toStrDate().c_str(),
						DISREC_NEWLINE,
						"h03 BtM-Nummer ",
						basar::login::Manager::getInstance().getAreaID(),
						DISREC_NEWLINE,
						"h04 ",
						QApplication::translate("DissolveReceiptPrintData", reportCompanyName.c_str()).toLocal8Bit().data(),
						DISREC_NEWLINE,
						"h05 ",
						QApplication::translate("DissolveReceiptPrintData", "Depot ").toLocal8Bit().data(),
						DISREC_NEWLINE,
						"h06 ",
						QApplication::translate("DissolveReceiptPrintData", "Narcotics-Department").toLocal8Bit().data(),
						DISREC_NEWLINE,
						"h07",
						DISREC_NEWLINE,
						"h08",
						DISREC_NEWLINE
						);
	}

    return static_cast<basar::I18nString>(headerLines);
}

//------------------------------------------------------------------------------
/*!	\throw	no-throw */
basar::I18nString DissolveReceiptPrintData::getDataLine()
{
	//print one data line
	basar::VarString dataline;

	basar::VarString packUnity;
	basar::VarString extUnity;

	splitUnity( m_DataIterator.getString("packageunit"), packUnity, extUnity );
	
	// expand packUnity and extUnity to 6 signs if they're shorter
	packUnity.trim();
	extUnity.trim();
	
	while( packUnity.length() < 6 ){
		packUnity += " ";
	}
	
	while( extUnity.length() < 6 ){
		extUnity += " ";
	}

	m_DataIterator.getString(constants::ARTICLECODE).stoi();
	
	// new dataline format
	dataline.format("%s%s%08ld%s%s%5ld%s%s%s%s%s%s%s%s%s%s",
								"p01 ",
								"PZN ",
								m_DataIterator.getString(constants::ARTICLECODE).stoi(),
								//m_DataIterator.getInt32(constants::ARTICLENO),
								" ",
								"Anzahl ",
								m_DataIterator.getInt32("qtytransaction"),
								" ",
								"Packungseinheit ",
								packUnity.c_str(),
								" ",
								"Maßeinheit ",
								extUnity.c_str(),
								" ",
								"Bezeichnung ",
								m_DataIterator.getString("articlename").c_str(),
								DISREC_NEWLINE
								);
	m_DataIterator++;
	m_Counter++;

	// end of printing order as dissolve receipt already contains maximum number of bookings
	if( m_Counter % constants::BOOKINGS_PER_FORM == 0 )
	{
		// parameter for defining end of data page in planet press
		dataline.append("#endofpage#\n");
	}

	if( m_DataIterator.isEnd() ){
		dataline.append("EODF\n"); // end of data file (delim erased)
	}
	
    return static_cast<basar::I18nString>(dataline);
}

//------------------------------------------------------------------------------
/*!	\throw	no-throw */
void DissolveReceiptPrintData::onSendData( basar::cmnutil::CollPrintRawInput & collPrintRawInput )
{
	static const basar::Int32 BUFLEN = 1024;
	char   buffer[BUFLEN];
	const char * pBuf;
	const char * pBufEnd;
	basar::Int32  i;

	basar::I18nString dataline;
	
	if( !m_HeaderDataSent )
	{
		basar::I18nString headerline = getHeaderLines();
		dataline = headerline;
		m_HeaderDataSent = true;		
		strcpy(buffer, headerline.c_str());
	}
	else
	{
		if( !m_DataIterator.isEnd() )
		{
			basar::I18nString positions = getDataLine();
			dataline += positions;
			strcpy(buffer, positions.c_str());
		}
		else{
			strcpy(buffer,"\0");
		}
	}

	size_t readbytes = strlen (buffer);

	for( i = 0, pBuf = buffer, pBufEnd = (buffer + readbytes); 
		pBuf < pBufEnd; 
		++pBuf )
	{
		collPrintRawInput.push(*pBuf);
	}

	if( 0 == readbytes )
	{
		// reset data for print preview save pdf
		m_HeaderDataSent = false;
		m_DataIterator = m_DisReceiptData.begin();
		m_Counter = 0;
	}
}

//------------------------------------------------------------------------------
/*!	\throw	no-throw 
	\return form name */
basar::VarString DissolveReceiptPrintData::getFormName() const
{
	return m_FormName;
}

//------------------------------------------------------------------------------
/*!	\throw	no-throw 
	\return ps-file name */
basar::VarString DissolveReceiptPrintData::getFormFileName() const
{
	return m_PsFile;
}

} //namespace narcotics