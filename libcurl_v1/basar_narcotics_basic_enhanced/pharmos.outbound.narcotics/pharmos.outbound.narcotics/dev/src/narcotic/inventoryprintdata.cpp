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
#include "inventoryprintdata.h"

#define INV_DELIM		"|"
#define STOR_LOC_DELIM	"-"
#define NEW_LINE		"\n"

//-------------------------------------------------------------------------------------------------//
// namespaces
//-------------------------------------------------------------------------------------------------//
namespace narcotics
{

//------------------------------------------------------------------------------
/*!	\throw	no-throw */
InventoryPrintData::InventoryPrintData ( const basar::VarString & rPsFile, const basar::VarString & rFormName, 
										 const basar::db::aspect::AccessorPropertyTableRef inventoryData,
										 const bool isCountList)
					 : m_PsFile( rPsFile ),
					   m_FormName( rFormName ),
					   m_InventoryData( inventoryData ),
					   m_DataIterator( m_InventoryData.begin() ),
					   m_SendHeaderLines( true ),
					   m_IsCountList( isCountList )
{
}

//------------------------------------------------------------------------------
/*!	\throw	no-throw */
InventoryPrintData::~InventoryPrintData()
{
}

//------------------------------------------------------------------------------
/*!	\throw	no-throw */
basar::I18nString InventoryPrintData::getHeaderLines()
{
	basar::I18nString header;
	if( m_IsCountList ){
		header = QApplication::translate("InventoryPrintData", "Perpetual Stocktaking - Narcotics Count-List\n").toLocal8Bit().data();
	}
	else{
		header = QApplication::translate("InventoryPrintData", "Perpetual Stocktaking - Narcotics Stock-List\n").toLocal8Bit().data();
	}

	basar::VarString param;
	if( m_IsCountList ){
		param = "Z\n\n";
	}
	else{
		param = "B\n\n";
	}
		
	// print header lines
	basar::I18nString headerLines;
	headerLines.format("%s%s%s%s%s%s%s%s%s%s%s%s%s%s%s%s%s%s",

						basar::login::Manager::getInstance().getLanguageID().c_str(),
						NEW_LINE,
						header.c_str(),
						basar::login::Manager::getInstance().getAreaName().c_str(),
						NEW_LINE,
						QApplication::translate("InventoryPrintData", "page\n").toLocal8Bit().data(),
						QApplication::translate("InventoryPrintData", "of\n").toLocal8Bit().data(),
						QDate::currentDate().toString("dddd, ").toLocal8Bit().constData(),
						NEW_LINE,
						QDate::currentDate().toString("dd.MM.yyyy").toLocal8Bit().constData(),
						NEW_LINE,
						param.c_str(),
	
						QApplication::translate("InventoryPrintData", "storage location\n").toLocal8Bit().data(),
						QApplication::translate("InventoryPrintData", "articleno\n").toLocal8Bit().data(),
						QApplication::translate("InventoryPrintData", "unity\n").toLocal8Bit().data(),
						QApplication::translate("InventoryPrintData", "article name\n").toLocal8Bit().data(),
						QApplication::translate("InventoryPrintData", "form\n").toLocal8Bit().data(),
						QApplication::translate("InventoryPrintData", "stock\n\n").toLocal8Bit().data()
						);
    
	return headerLines;
}

//------------------------------------------------------------------------------
/*!	\throw	no-throw */
basar::I18nString InventoryPrintData::getDataLine()
{
	//print one data line
	basar::I18nString dataline;
	
	basar::I18nString storLocFormatted;
	basar::I18nString storLoc = m_DataIterator.getString("storagelocation");
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
	
	dataline.format("%s%s%08s%s%s%s%s%s%s%s%ld",
					storLocFormatted.c_str(),
					INV_DELIM,
					m_DataIterator.getString(constants::ARTICLECODE).c_str(),
					INV_DELIM,
					m_DataIterator.getString("packageunit").c_str(),
					INV_DELIM,
					m_DataIterator.getString("articlename").c_str(),
					INV_DELIM,
					m_DataIterator.getString("pharmaform").c_str(),
					INV_DELIM,
					m_DataIterator.getInt32("qtyinstock")
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

	return dataline;
}

//------------------------------------------------------------------------------
/*!	\throw	no-throw */
void InventoryPrintData::onSendData( basar::cmnutil::CollPrintRawInput & collPrintRawInput )
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
			strcpy( buffer, getDataLine().c_str() );
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
		m_DataIterator = m_InventoryData.begin();
	}
}

//------------------------------------------------------------------------------
/*!	\throw	no-throw 
	\return form name */
basar::VarString InventoryPrintData::getFormName() const
{
	return m_FormName;
}

//------------------------------------------------------------------------------
/*!	\throw	no-throw 
	\return ps-file name */
basar::VarString InventoryPrintData::getFormFileName() const
{
	return m_PsFile;
}

} //namespace narcotics