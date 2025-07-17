//-------------------------------------------------------------------------------------------------//
/*! \file
 *  \brief  implemtation of inventory print data
 *  \author	Thomas Hörath
 *  \date   17.05.2006
 */
//-------------------------------------------------------------------------------------------------//

//-------------------------------------------------------------------------------------------------//
// includes
//-------------------------------------------------------------------------------------------------//
#include "commonheader.h"
#include "loggerpool.h"
#include "acknowledgementlistprintdata.h"

#define INV_DELIM		"|"
#define NEW_LINE		"\n"

//-------------------------------------------------------------------------------------------------//
// namespaces
//-------------------------------------------------------------------------------------------------//
namespace narcotics {

//------------------------------------------------------------------------------
/*!	\throw	no-throw */
AcknowledgementListPrintData::AcknowledgementListPrintData( const basar::VarString & rPsFile, const basar::VarString & rFormName, 
										 const basar::db::aspect::AccessorPropertyTableRef acknowledgementListData )
					 : m_PsFile( rPsFile ),
					   m_FormName( rFormName ),
					   m_acknowledgementListData( acknowledgementListData ),
					   m_DataIterator( m_acknowledgementListData.begin() ),
					   m_SendHeaderLines( true )
{
}

//------------------------------------------------------------------------------
/*!	\throw	no-throw */
AcknowledgementListPrintData::~AcknowledgementListPrintData()
{
}

//------------------------------------------------------------------------------
/*!	\throw	no-throw */
basar::I18nString AcknowledgementListPrintData::getHeaderLines()
{
	basar::I18nString header;
	header = QApplication::translate("AcknowledgementListPrintData", "Data from NA17\n").toLocal8Bit().data();
		
	// print header lines
	basar::I18nString headerLines;
	headerLines.format("%s%s%s%s%s%s%s%s%s%s%s%s%s%s%s%s%s%s%s%s%s%s%s%s%s%s%s%s%s%s%s%s",
                        //document header lines
						basar::login::Manager::getInstance().getLanguageID().c_str(),
                        NEW_LINE,
						header.c_str(),
                        NEW_LINE,
						basar::login::Manager::getInstance().getAreaName().c_str(),
                        NEW_LINE,
						QApplication::translate("AcknowledgementListPrintData", "page").toLocal8Bit().data(),
                        NEW_LINE,
						QApplication::translate("AcknowledgementListPrintData", "of").toLocal8Bit().data(),
                        NEW_LINE,
						QDate::currentDate().toString("dddd, ").toLocal8Bit().constData(),
                        NEW_LINE,
						QDate::currentDate().toString("dd.MM.yyyy").toLocal8Bit().constData(),
                        NEW_LINE,
                        NEW_LINE,
                        //column header lines
						QApplication::translate("AcknowledgementListPrintData", "CustomerSupplier Name").toLocal8Bit().data(),
                        NEW_LINE,
						QApplication::translate("AcknowledgementListPrintData", "CustomerSupplier City").toLocal8Bit().data(),
                        NEW_LINE,
						QApplication::translate("AcknowledgementListPrintData", "PurchaseOrderNo").toLocal8Bit().data(),
                        NEW_LINE,
						QApplication::translate("AcknowledgementListPrintData", "Date").toLocal8Bit().data(),
                        NEW_LINE,
						QApplication::translate("AcknowledgementListPrintData", "Phone").toLocal8Bit().data(),
                        NEW_LINE,
						QApplication::translate("AcknowledgementListPrintData", "ContributionVoucherNo").toLocal8Bit().data(),
                        NEW_LINE,
						QApplication::translate("AcknowledgementListPrintData", "Status").toLocal8Bit().data(),
                        NEW_LINE,
						QApplication::translate("AcknowledgementListPrintData", "Tour").toLocal8Bit().data(),
                        NEW_LINE,
                        NEW_LINE
						);
    basar::VarString msg;

    msg.format("AcknowledgementListPrintData - Headerlines:\n>%s<\nHeaderlines End", headerLines.c_str());

    BLOG_TRACE(LoggerPool::loggerDebug, msg);
	return headerLines;
}

//------------------------------------------------------------------------------
/*!	\throw	no-throw */
basar::I18nString AcknowledgementListPrintData::getDataLine()
{
	//print one data line
	basar::I18nString dataline;
	
    basar::VarString telno_str = m_DataIterator.getString( constants::TELNO );
    basar::Date dateTemp( m_DataIterator.getInt32( constants::TRANSACTIONDATE ) );

    telno_str.remove(" ");

	basar::I18nString status;

	switch( m_DataIterator.getInt16( constants::ACKNOWLEDGEMENTSTATUS ) )
	{
		case constants::PRINTED:
            status = QApplication::translate("AcknowledgementStatusTransformer", "printed").toLocal8Bit().data();
            break;
        case constants::ACKNOWLEDGEMENT_RECEIVED:
            status = QApplication::translate("AcknowledgementStatusTransformer", "received").toLocal8Bit().data();
            break;
        case constants::CHANGED_ACKNOWLEDGEMENT:
            status = QApplication::translate("AcknowledgementStatusTransformer", "received").toLocal8Bit().data();
            break;
        case constants::DELIVERY_NOTE_COPY_CREATED:
            status = QApplication::translate("AcknowledgementStatusTransformer", "Delivery note copy created").toLocal8Bit().data();
            break;
        default:
            status = QString::number( m_DataIterator.getInt16( constants::ACKNOWLEDGEMENTSTATUS ) ).toLocal8Bit().data();
	}

    dataline.format("%s%s%s%s%d%s%s%s%s%s%d%s%s%s%s", 
        m_DataIterator.getString(constants::CUSTOMER_SUPPLIER_NAME).c_str(),
        INV_DELIM,
        m_DataIterator.getString(constants::CUSTOMER_SUPPLIER_LOCATION).c_str(),
        INV_DELIM,
        m_DataIterator.getInt32(constants::PURCHASEORDERNO),
        INV_DELIM,
        dateTemp.toStrDate().c_str(),
        INV_DELIM,
        telno_str.c_str(),
        INV_DELIM,
        m_DataIterator.getInt32(constants::CONTRIBUTIONVOUCHERNO),
		INV_DELIM,
        status.c_str(),
		INV_DELIM,
		m_DataIterator.getString(constants::TOURID).c_str()
        );
							
	++m_DataIterator;

	if( m_DataIterator.isEnd() ){
		dataline.append("\nEODF\n"); // end of data file
	}
	else
	{
		dataline.append( INV_DELIM );
		dataline.append( NEW_LINE );
	}

    basar::VarString msg;

    msg.format( "AcknowledgementListPrintData - DataLine:\n>%s<\nDataLine End", dataline.c_str() );

    BLOG_TRACE(LoggerPool::loggerDebug, msg);

	return dataline;
}

//------------------------------------------------------------------------------
/*!	\throw	no-throw */
void AcknowledgementListPrintData::onSendData( basar::cmnutil::CollPrintRawInput & collPrintRawInput )
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
		m_DataIterator = m_acknowledgementListData.begin();
	}
}

//------------------------------------------------------------------------------
/*!	\throw	no-throw 
	\return form name */
basar::VarString AcknowledgementListPrintData::getFormName() const
{
	return m_FormName;
}

//------------------------------------------------------------------------------
/*!	\throw	no-throw 
	\return ps-file name */
basar::VarString AcknowledgementListPrintData::getFormFileName() const
{
	return m_PsFile;
}

} //namespace narcotics