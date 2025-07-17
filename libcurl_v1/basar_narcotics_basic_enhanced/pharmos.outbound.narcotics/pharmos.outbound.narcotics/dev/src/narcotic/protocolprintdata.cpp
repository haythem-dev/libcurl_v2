//-------------------------------------------------------------------------------------------------//
/*! \file
 *  \brief  implemtation of protocol print data
 *  \author	Thomas Hörath
 *  \date   23.06.2006
 */
//-------------------------------------------------------------------------------------------------//

//-------------------------------------------------------------------------------------------------//
// includes
//-------------------------------------------------------------------------------------------------//
#include "commonheader.h"
#include "protocolprintdata.h"

#define PROTPRINT_DELIM		"|"
#define NEW_PROT_LINE		"\n"
#define LINES_PER_PROTPAGE	33

//-------------------------------------------------------------------------------------------------//
// namespaces
//-------------------------------------------------------------------------------------------------//
namespace narcotics
{

//------------------------------------------------------------------------------
/*!	\throw	no-throw */
ProtocolPrintData::ProtocolPrintData (  const basar::VarString & rPsFile, const basar::VarString & rFormName, 
										const basar::db::aspect::AccessorPropertyTableRef protocolData, 
										const CollOfBookingLogs* const pCollLogs, const basar::VarString & rPeriod )					 
					 : m_PsFile( rPsFile ),
					   m_FormName( rFormName ),
					   m_ChangedBookings( protocolData ),
					   m_ItBookings( m_ChangedBookings.begin() ),
					   m_pCollLogs( pCollLogs ),
					   m_currentLogs( m_pCollLogs->find( m_ItBookings.getInt32( constants::CATALOGID ) )->second ),
					   m_ItLogs( m_currentLogs.begin() ),
					   m_SendHeaderLines( true ),
					   m_currentCatalogId( -1 ),
					   m_LineNo( 0 ),
					   m_Period( rPeriod )
{
}

//------------------------------------------------------------------------------
/*!	\throw	no-throw */
ProtocolPrintData::~ProtocolPrintData()
{
}

//------------------------------------------------------------------------------
/*!	\throw	no-throw */
basar::I18nString ProtocolPrintData::getHeaderLines()
{
	// print header lines
	basar::I18nString headerLines;
	
	headerLines.format("%s%s%s%s%s%s%s%s%s%s%s%s%s%s%s%s%s%s%s%s%s%s%s%s%s%s%s%s%s%s%s",

						basar::login::Manager::getInstance().getLanguageID().c_str(),
						NEW_PROT_LINE,
						QApplication::translate("ProtocolPrintData", "Narcotics - Logged changed bookings\n").toLocal8Bit().data(),
						basar::login::Manager::getInstance().getAreaName().c_str(),
						NEW_PROT_LINE,
						QApplication::translate("ProtocolPrintData", "page\n").toLocal8Bit().data(),
						QApplication::translate("ProtocolPrintData", "of\n").toLocal8Bit().data(),
						QDate::currentDate().toString("dddd, ").toLocal8Bit().constData(),
						NEW_PROT_LINE,
						QDate::currentDate().toString("dd.MM.yyyy").toLocal8Bit().constData(),
						NEW_PROT_LINE,
						QApplication::translate("ProtocolPrintData", "period: ").toLocal8Bit().data(),
						m_Period.c_str(),
						NEW_PROT_LINE,
						NEW_PROT_LINE,
																		
						QApplication::translate("ProtocolPrintData", "ART\n").toLocal8Bit().data(),
						QApplication::translate("ProtocolPrintData", "DATE\n").toLocal8Bit().data(),
						QApplication::translate("ProtocolPrintData", "TIME\n").toLocal8Bit().data(),
						QApplication::translate("ProtocolPrintData", "C-S-NO\n").toLocal8Bit().data(),
						QApplication::translate("ProtocolPrintData", "TT\n").toLocal8Bit().data(),
						QApplication::translate("ProtocolPrintData", "ORDERNO\n").toLocal8Bit().data(),
						QApplication::translate("ProtocolPrintData", "QTY-TR\n").toLocal8Bit().data(),
						QApplication::translate("ProtocolPrintData", "QTY-STK\n").toLocal8Bit().data(),
						NEW_PROT_LINE,
						
						QApplication::translate("ProtocolPrintData", "date\n").toLocal8Bit().data(),
						QApplication::translate("ProtocolPrintData", "time\n").toLocal8Bit().data(),
						QApplication::translate("ProtocolPrintData", "agent\n").toLocal8Bit().data(),
						QApplication::translate("ProtocolPrintData", "type\n").toLocal8Bit().data(),
						QApplication::translate("ProtocolPrintData", "attribute\n").toLocal8Bit().data(),
						QApplication::translate("ProtocolPrintData", "old value\n").toLocal8Bit().data(),
						QApplication::translate("ProtocolPrintData", "new value\n").toLocal8Bit().data(),
						NEW_PROT_LINE						
						);
    
	return headerLines;
}

//------------------------------------------------------------------------------
/*!	\throw	no-throw */
basar::I18nString ProtocolPrintData::getGroupHeaderLine( basar::I18nString lineType )
{
	basar::I18nString dataline;

	basar::DateTime dtTemp;
	dtTemp.setDate( m_ItBookings.getInt32(constants::TRANSACTIONDATE) );
	dtTemp.setTime( m_ItBookings.getInt32(constants::TRANSACTIONTIME) * 1000 );
		
	dataline.format("%s%1.1s%-8.8s%1.1s%-10.10s%1.1s%-8.8s%1.1s%9ld%1.1s%-3.3s%1.1s%6ld%1.1s%6ld%1.1s%6ld",
				lineType.c_str(),			
				PROTPRINT_DELIM,
				m_ItBookings.getString(constants::ARTICLECODE).c_str(),
				PROTPRINT_DELIM,
				dtTemp.toStrDate().c_str(),
				PROTPRINT_DELIM,
				dtTemp.toStrTimeShort().c_str(),
				PROTPRINT_DELIM,
				m_ItBookings.getInt32("customersupplierno"),
				PROTPRINT_DELIM,
				transactionType(m_ItBookings.getString(constants::TRANSACTIONTYPE)).c_str(),
				PROTPRINT_DELIM,
				m_ItBookings.getInt32(constants::PURCHASEORDERNO),
				PROTPRINT_DELIM,
				m_ItBookings.getInt32("qtytransaction"),
				PROTPRINT_DELIM,
				m_ItBookings.getInt32("qtyinstock")					
				);
	
	return dataline;	
}

//------------------------------------------------------------------------------
/*!	\throw	no-throw */
basar::I18nString ProtocolPrintData::getDataLine()
{
	//print one data line
	basar::I18nString dataline;
		
	basar::I18nString lineType;
	
	//print group header again on next page!
	++m_LineNo;

	if( m_LineNo > LINES_PER_PROTPAGE )
	{
		if( !m_ItLogs.isEnd() )
		{
			if( m_LineNo == LINES_PER_PROTPAGE + 1 &&
				m_currentCatalogId == m_ItLogs.getInt32( constants::CATALOGID ))
			{
				lineType = NEW_PROT_LINE;
				lineType.append( NEW_PROT_LINE );
				lineType.append( "$$$" );
				
				dataline = getGroupHeaderLine( lineType );
				
				dataline.append( PROTPRINT_DELIM );
				dataline.append( NEW_PROT_LINE );

				m_LineNo = 1;
						
				return dataline;
			}
		}
		
		m_LineNo = 1;
	}

	if( !m_ItLogs.isEnd() && m_currentCatalogId == m_ItBookings.getInt32( constants::CATALOGID ) )
	{
		lineType = "   ";

		basar::DateTime	dtTemp;
		dtTemp.setDate( m_ItLogs.getInt32("protocoldate") );
		dtTemp.setTime( m_ItLogs.getInt32("protocoltime") );
		
		dataline.format("%3.3s%1.1s%-40.40s%1.1s%-10.10s%1.1s%-8.8s%1.1s%-20.20s%1.1s%-30.30s%1.1s%-15.15s%1.1s%-15.15s",
						lineType.c_str(),			
						PROTPRINT_DELIM,
						m_ItLogs.getString("username").c_str(),
						PROTPRINT_DELIM,
						dtTemp.toStrDate().c_str(),
						PROTPRINT_DELIM,
						dtTemp.toStrTimeShort().c_str(),
						PROTPRINT_DELIM,
						changingType(m_ItLogs.getString("activitytype")).c_str(),
						PROTPRINT_DELIM,
						m_ItLogs.getString("changedattribute").c_str(),
						PROTPRINT_DELIM,
						m_ItLogs.getString("valueold").c_str(),
						PROTPRINT_DELIM,
						m_ItLogs.getString("valuenew").c_str()				
						);
		++m_ItLogs;
	}
	else
	{
		++m_LineNo;
		
		if( m_LineNo != LINES_PER_PROTPAGE + 1 )
		{
			if( m_currentCatalogId != -1 ){
				++m_ItBookings;
			}
			
			if( m_ItBookings.isEnd() && m_ItLogs.isEnd() )
			{
				dataline.append("EOF\n"); // end of file
				return dataline;
			}

			m_currentCatalogId = m_ItBookings.getInt32( constants::CATALOGID );
			m_currentLogs = m_pCollLogs->find( m_currentCatalogId )->second;
			m_ItLogs = m_currentLogs.begin();
			
			if( m_LineNo != LINES_PER_PROTPAGE )
			{
				++m_LineNo;
				lineType = NEW_PROT_LINE;
				lineType.append( NEW_PROT_LINE );
				lineType.append( "$$$" );
				dataline = getGroupHeaderLine( lineType );
			}
			else{
				dataline = NEW_PROT_LINE;
			}
		}
	}
								
	dataline.append( PROTPRINT_DELIM );
	dataline.append( NEW_PROT_LINE );
		
	if( m_ItBookings.isEnd() && m_ItLogs.isEnd() ){
		dataline.append( "EOF\n" ); // end of file
	}

	return dataline;
}

//------------------------------------------------------------------------------
/*!	\throw	no-throw */
void ProtocolPrintData::onSendData( basar::cmnutil::CollPrintRawInput& collPrintRawInput )
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
		if( !m_ItBookings.isEnd() ){
			strcpy( buffer, getDataLine().c_str() );
		}
		else{
			strcpy( buffer,"\0" );
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
		m_SendHeaderLines	= true;
		m_currentCatalogId	= -1;
		m_ItBookings		= m_ChangedBookings.begin();
		m_currentLogs		= m_pCollLogs->find( m_ItBookings.getInt32( constants::CATALOGID ) )->second;
		m_ItLogs			= m_currentLogs.begin();
		m_LineNo			= 0;
	}
}

//------------------------------------------------------------------------------
/*!	\throw	no-throw 
	\return form name */
basar::VarString ProtocolPrintData::getFormName() const
{
	return m_FormName;
}

//------------------------------------------------------------------------------
/*!	\throw	no-throw 
	\return ps-file name */
basar::VarString ProtocolPrintData::getFormFileName() const
{
	return m_PsFile;
}

} //namespace narcotics