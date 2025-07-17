//-------------------------------------------------------------------------------------------------//
/*! \file
 *  \brief  implemtation of transaction article print data
 *  \author	Thomas Hörath
 *  \date   20.07.2006
 */
//-------------------------------------------------------------------------------------------------//

//-------------------------------------------------------------------------------------------------//
// includes
//-------------------------------------------------------------------------------------------------//
#include "commonheader.h"
#include "transactarticleprintdata.h"

#define TRANSART_DELIM		"|"
#define TRANSART_NEWLINE	"\n"
#define STOR_LOC_DELIM		"-"

//-------------------------------------------------------------------------------------------------//
// namespaces
//-------------------------------------------------------------------------------------------------//
namespace narcotics
{

//------------------------------------------------------------------------------
/*!	\throw	no-throw */
TransactArticlePrintData::TransactArticlePrintData ( const basar::VarString & rPsFile, 
													 const basar::VarString & rFormName, 
													 const basar::db::aspect::AccessorPropertyTableRef printData,
													 const basar::db::aspect::AccessorPropertyTable_YIterator yitArticle)
					 : m_PsFile( rPsFile ),
					   m_FormName( rFormName ),
					   m_CustSupplData( printData ),
					   m_DataIterator( m_CustSupplData.begin() ),
					   m_ArticleIt( yitArticle ),
					   m_SendHeaderLines( true )
{
}

//------------------------------------------------------------------------------
/*!	\throw	no-throw */
TransactArticlePrintData::~TransactArticlePrintData()
{
}

//------------------------------------------------------------------------------
/*!	\throw	no-throw */
basar::I18nString TransactArticlePrintData::getHeaderLines()
{
	basar::I18nString headerLines;

	basar::I18nString storLocFormatted;
	basar::I18nString storLoc = m_ArticleIt.getString("storagelocation");
	if( storLoc.length() >= 8 )
	{
		storLocFormatted.format("%s%s%s%s%s%s%s",
						storLoc.substr(0,2).c_str(),
						STOR_LOC_DELIM,
						storLoc.substr(2,2).c_str(), 
						STOR_LOC_DELIM,
						storLoc.substr(4,2).c_str(), 
						STOR_LOC_DELIM,
						storLoc.substr(6,2).c_str()
						);								
	}
	else{
		storLocFormatted = storLoc;
	}
	
	//print header lines
	headerLines.format("%s%s%s%s%s%s%s%s%s%s%s%s%s%s%s%s%s%s%s%s%s%s%s%s%s%s%s%s%s%s%s%s%s%s%s%s%s%s%s%s%s%s%s%s",

						basar::login::Manager::getInstance().getLanguageID().c_str(),
						TRANSART_NEWLINE,
						QApplication::translate("TransactArticlePrintData", "customers / suppliers to one article").toLocal8Bit().data(),
						TRANSART_NEWLINE,
						QApplication::translate("TransactArticlePrintData", "depot ").toLocal8Bit().data(),
						basar::login::Manager::getInstance().getAreaName().c_str(),
						TRANSART_NEWLINE,
						QApplication::translate("TransactArticlePrintData", "page").toLocal8Bit().data(),
						TRANSART_NEWLINE,
						QApplication::translate("TransactArticlePrintData", "of").toLocal8Bit().data(),
						TRANSART_NEWLINE,
						QDate::currentDate().toString("dddd, ").toLocal8Bit().constData(),
						TRANSART_NEWLINE,
						QDate::currentDate().toString("dd.MM.yyyy").toLocal8Bit().constData(),
						TRANSART_NEWLINE,
						TRANSART_NEWLINE,
						QApplication::translate("TransactArticlePrintData", "DATE").toLocal8Bit().data(),
						TRANSART_NEWLINE,
						QApplication::translate("TransactArticlePrintData", "C-S-No").toLocal8Bit().data(),
						TRANSART_NEWLINE,
						QApplication::translate("TransactArticlePrintData", "Name").toLocal8Bit().data(),
						TRANSART_NEWLINE,
						QApplication::translate("TransactArticlePrintData", "TT").toLocal8Bit().data(),
						TRANSART_NEWLINE,
						QApplication::translate("TransactArticlePrintData", "OrdNo").toLocal8Bit().data(),
						TRANSART_NEWLINE,
						QApplication::translate("TransactArticlePrintData", "Amount").toLocal8Bit().data(),
						TRANSART_NEWLINE,
						QApplication::translate("TransactArticlePrintData", "SD").toLocal8Bit().data(),
						TRANSART_NEWLINE,
						QApplication::translate("TransactArticlePrintData", "Callback").toLocal8Bit().data(),
						TRANSART_NEWLINE,
						TRANSART_NEWLINE,
						m_ArticleIt.getString(constants::ARTICLECODE).c_str(),
						TRANSART_NEWLINE,
						m_ArticleIt.getString("packageunit").c_str(),
						TRANSART_NEWLINE,
						m_ArticleIt.getString("articlename").c_str(),
						TRANSART_NEWLINE,
						m_ArticleIt.getString("pharmaform").c_str(),
						TRANSART_NEWLINE,
						storLocFormatted.c_str(),
						TRANSART_NEWLINE,
						TRANSART_NEWLINE						
						);

	return headerLines;
}

//------------------------------------------------------------------------------
/*!	\throw	no-throw */
basar::I18nString TransactArticlePrintData::getDataLine()
{
	//print one data line
	basar::I18nString dataline;
	
	basar::DateTime		dtTemp;
	dtTemp.setDate( m_DataIterator.getInt32( constants::TRANSACTIONDATE ) );
		
	dataline.format("%s%s%ld%s%s%s%s%s%ld%s%ld%s%s%s%s",
					dtTemp.toStrDate().c_str(),
					TRANSART_DELIM,
					m_DataIterator.getInt32("customersupplierno"),
					TRANSART_DELIM,
					m_DataIterator.getString("customersuppliername").c_str(),
					TRANSART_DELIM,
					transactionType(m_DataIterator.getString(constants::TRANSACTIONTYPE)).c_str(),
					TRANSART_DELIM,
					m_DataIterator.getInt32(constants::PURCHASEORDERNO),
					TRANSART_DELIM,
					m_DataIterator.getInt32("qtytransaction"),
					TRANSART_DELIM,
					m_DataIterator.getString("quickdialing").c_str(),
					TRANSART_DELIM,
					m_DataIterator.getString("telno").c_str()
					);
	
	++m_DataIterator;

	if( m_DataIterator.isEnd() ){
		dataline.append("\nEODF\n"); // end of data file
	}
	else
	{
		dataline.append( TRANSART_DELIM );
		dataline.append( TRANSART_NEWLINE );
	}

	return dataline;
}

//------------------------------------------------------------------------------
/*!	\throw	no-throw */
void TransactArticlePrintData::onSendData( basar::cmnutil::CollPrintRawInput& collPrintRawInput )
{
	static const basar::Int32 BUFLEN = 1024;
	char   buffer[BUFLEN];
	const char * pBuf;
	const char * pBufEnd;
	basar::Int32  i;
	
	if( m_SendHeaderLines )
	{
		m_SendHeaderLines = false;		
		strcpy(buffer, getHeaderLines().c_str());
	}
	else
	{
		if( !m_DataIterator.isEnd() ){
			strcpy(buffer, getDataLine().c_str());
		}
		else{
			strcpy(buffer,"\0");
		}
	}

	size_t readbytes = strlen (buffer);

	for( i = 0, pBuf = buffer, pBufEnd = (buffer + readbytes); 
		pBuf < pBufEnd; 
		pBuf++ )
	{
		collPrintRawInput.push(*pBuf);
	}

	if( 0 == readbytes )
	{
		// set iterator to begin of data
		m_SendHeaderLines = true;
		m_DataIterator = m_CustSupplData.begin();
	}
}

//------------------------------------------------------------------------------
/*!	\throw	no-throw 
	\return form name */
basar::VarString TransactArticlePrintData::getFormName() const
{
	return m_FormName;
}

//------------------------------------------------------------------------------
/*!	\throw	no-throw 
	\return ps-file name */
basar::VarString TransactArticlePrintData::getFormFileName() const
{
	return m_PsFile;
}

} //namespace narcotics