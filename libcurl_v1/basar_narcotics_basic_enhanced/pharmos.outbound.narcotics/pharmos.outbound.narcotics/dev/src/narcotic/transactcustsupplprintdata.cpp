//-------------------------------------------------------------------------------------------------//
/*! \file
 *  \brief  implemtation of transaction cust / suppl print data
 *  \author	Thomas Hörath
 *  \date   20.07.2006
 */
//-------------------------------------------------------------------------------------------------//

//-------------------------------------------------------------------------------------------------//
// includes
//-------------------------------------------------------------------------------------------------//
#include "commonheader.h"
#include "transactcustsupplprintdata.h"

#define TRANSCS_DELIM		"|"
#define TRANSCS_NEWLINE		"\n"

//-------------------------------------------------------------------------------------------------//
// namespaces
//-------------------------------------------------------------------------------------------------//
namespace narcotics 
{

//------------------------------------------------------------------------------
/*!	\throw	no-throw */
TransactCustSupplPrintData::TransactCustSupplPrintData ( const basar::VarString & rPsFile, 
														 const basar::VarString & rFormName, 
														 const basar::db::aspect::AccessorPropertyTableRef printData,
														 const basar::db::aspect::AccessorPropertyTable_YIterator yitCustSuppl)
					 : m_PsFile( rPsFile ),
					   m_FormName( rFormName ),
					   m_ArticleData( printData ),
					   m_DataIterator( m_ArticleData.begin() ),
					   m_CustSupplIt( yitCustSuppl ),
					   m_SendHeaderLines( true )
{
}

//------------------------------------------------------------------------------
/*!	\throw	no-throw */
TransactCustSupplPrintData::~TransactCustSupplPrintData()
{
}

//------------------------------------------------------------------------------
/*!	\throw	no-throw */
basar::I18nString TransactCustSupplPrintData::getHeaderLines()
{
	basar::I18nString headerLines;

	// difference between customer / supplier / branch!
	basar::I18nString details;

	if( m_DataIterator.getString(constants::TRANSACTIONTYPE) == TransactionType::CUSTOMER_ORDER ||
		m_DataIterator.getString(constants::TRANSACTIONTYPE) == TransactionType::CUSTOMER_RETURNS )
	{
		details.format("%ld%s%s%s%s%s%s%s%s",
						m_CustSupplIt.getInt32("customerno"),
						TRANSCS_NEWLINE,
						m_CustSupplIt.getString("customername").c_str(),
						TRANSCS_NEWLINE,
						QApplication::translate("TransactCustSupplPrintData", "Quick Dialing: ").toLocal8Bit().data(),
						m_CustSupplIt.getString("quickdialing").c_str(),
						TRANSCS_NEWLINE,
						QApplication::translate("TransactCustSupplPrintData", "Callback: ").toLocal8Bit().data(),
						m_CustSupplIt.getString("telno").c_str()            			
						);
	}
	else if( m_DataIterator.getString(constants::TRANSACTIONTYPE) == TransactionType::GOODSIN_DELIVERY ||
			 m_DataIterator.getString(constants::TRANSACTIONTYPE) == TransactionType::SUPPLIER_RETURNS )
	{
		details.format("%ld%s%s%s%s%s%s%s%s",
						m_CustSupplIt.getInt32("narcoticssupplierno"),
						TRANSCS_NEWLINE,
						m_CustSupplIt.getString("suppliername").c_str(),
						TRANSCS_NEWLINE,
						QApplication::translate("TransactCustSupplPrintData", "Quick Dialing: ").toLocal8Bit().data(),
						"NN",
						TRANSCS_NEWLINE,
						QApplication::translate("TransactCustSupplPrintData", "Callback: ").toLocal8Bit().data(),
						"NN"           			
						);
	}
	else
	{
		details.format("%ld%s%s%s%s%s%s%s%s",
            			m_CustSupplIt.getInt32("bgano"),
						TRANSCS_NEWLINE,
						m_CustSupplIt.getString("name").c_str(),
						TRANSCS_NEWLINE,
						QApplication::translate("TransactCustSupplPrintData", "Quick Dialing: ").toLocal8Bit().data(),
						"NN",
						TRANSCS_NEWLINE,
						QApplication::translate("TransactCustSupplPrintData", "Callback: ").toLocal8Bit().data(),
						m_CustSupplIt.getString("telno").c_str()
						);
	}

	//print header lines
	headerLines.format("%s%s%s%s%s%s%s%s%s%s%s%s%s%s%s%s%s%s%s%s%s%s%s%s%s%s%s%s%s%s%s%s%s%s%s%s",

						basar::login::Manager::getInstance().getLanguageID().c_str(),
						TRANSCS_NEWLINE,
						QApplication::translate("TransactCustSupplPrintData", "articles to one customer / supplier").toLocal8Bit().data(),
						TRANSCS_NEWLINE,
						QApplication::translate("TransactCustSupplPrintData", "depot ").toLocal8Bit().data(),
						basar::login::Manager::getInstance().getAreaName().c_str(),
						TRANSCS_NEWLINE,
						QApplication::translate("TransactCustSupplPrintData", "page").toLocal8Bit().data(),
						TRANSCS_NEWLINE,
						QApplication::translate("TransactCustSupplPrintData", "of").toLocal8Bit().data(),
						TRANSCS_NEWLINE,
						QDate::currentDate().toString("dddd, ").toLocal8Bit().constData(),
						TRANSCS_NEWLINE,
						QDate::currentDate().toString("dd.MM.yyyy").toLocal8Bit().constData(),
						TRANSCS_NEWLINE,
						TRANSCS_NEWLINE,
						QApplication::translate("TransactCustSupplPrintData", "DATE").toLocal8Bit().data(),
						TRANSCS_NEWLINE,
						QApplication::translate("TransactCustSupplPrintData", "ARTNO").toLocal8Bit().data(),
						TRANSCS_NEWLINE,
						QApplication::translate("TransactCustSupplPrintData", "UNITY").toLocal8Bit().data(),
						TRANSCS_NEWLINE,
						QApplication::translate("TransactCustSupplPrintData", "ARTICLENAME").toLocal8Bit().data(),
						TRANSCS_NEWLINE,
						QApplication::translate("TransactCustSupplPrintData", "FORM").toLocal8Bit().data(),
						TRANSCS_NEWLINE,
						QApplication::translate("TransactCustSupplPrintData", "TT").toLocal8Bit().data(),
						TRANSCS_NEWLINE,
						QApplication::translate("TransactCustSupplPrintData", "ORDERNO").toLocal8Bit().data(),
						TRANSCS_NEWLINE,
						QApplication::translate("TransactCustSupplPrintData", "AMOUNT").toLocal8Bit().data(),
						TRANSCS_NEWLINE,
						TRANSCS_NEWLINE,
						details.c_str(),
						TRANSCS_NEWLINE,
						TRANSCS_NEWLINE	
						);
    
	return headerLines;
}

//------------------------------------------------------------------------------
/*!	\throw	no-throw */
basar::I18nString TransactCustSupplPrintData::getDataLine()
{
	//print one data line
	basar::I18nString dataline;
	
	basar::DateTime	dtTemp;
	dtTemp.setDate( m_DataIterator.getInt32( constants::TRANSACTIONDATE ) );
		
	dataline.format("%s%s%08ld%s%s%s%s%s%s%s%s%s%ld%s%ld",
					dtTemp.toStrDate().c_str(),
					TRANSCS_DELIM,
					m_DataIterator.getString(constants::ARTICLECODE).stoi(),
					TRANSCS_DELIM,
					m_DataIterator.getString("packageunit").c_str(),
					TRANSCS_DELIM,
					m_DataIterator.getString("articlename").c_str(),
					TRANSCS_DELIM,
					m_DataIterator.getString("pharmaform").c_str(),
					TRANSCS_DELIM,
					transactionType(m_DataIterator.getString(constants::TRANSACTIONTYPE)).c_str(),
					TRANSCS_DELIM,
					m_DataIterator.getInt32(constants::PURCHASEORDERNO),
					TRANSCS_DELIM,
					m_DataIterator.getInt32("qtytransaction")
					);
	
	++m_DataIterator;

	if( m_DataIterator.isEnd() ){
		dataline.append("\nEODF\n"); // end of data file
	}
	else
	{
		dataline.append( TRANSCS_DELIM );
		dataline.append( TRANSCS_NEWLINE );
	}

	return dataline;
}

//------------------------------------------------------------------------------
/*!	\throw	no-throw */
void TransactCustSupplPrintData::onSendData( basar::cmnutil::CollPrintRawInput & collPrintRawInput )
{
	static const basar::Int32 BUFLEN = 1024;
	char   buffer[BUFLEN];
	const char * pBuf;
	const char * pBufEnd;
	basar::Int32  i;
	
	if( m_SendHeaderLines )
	{
		m_SendHeaderLines = false;		
		strcpy( buffer, getHeaderLines().c_str() );
	}
	else
	{
		if( !m_DataIterator.isEnd() ){
			strcpy( buffer, getDataLine().c_str() );
		}
		else{
			strcpy(buffer,"\0");
		}
	}

	size_t readbytes = strlen( buffer );

	for( i = 0, pBuf = buffer, pBufEnd = ( buffer + readbytes ); 
		pBuf < pBufEnd; 
		pBuf++ )
	{
		collPrintRawInput.push( *pBuf );
	}

	if( 0 == readbytes )
	{
		// set iterator to begin of data
		m_SendHeaderLines = true;
		m_DataIterator = m_ArticleData.begin();
	}
}

//------------------------------------------------------------------------------
/*!	\throw	no-throw 
	\return form name */
basar::VarString TransactCustSupplPrintData::getFormName() const
{
	return m_FormName;
}

//------------------------------------------------------------------------------
/*!	\throw	no-throw 
	\return ps-file name */
basar::VarString TransactCustSupplPrintData::getFormFileName() const
{
	return m_PsFile;
}

} //namespace narcotics