//-------------------------------------------------------------------------------------------------//
/*! \file
 *  \brief  implemtation of open order print data
 *  \author	Thomas Hörath
 *  \date   23.06.2006
 */
//-------------------------------------------------------------------------------------------------//

//-------------------------------------------------------------------------------------------------//
// includes
//-------------------------------------------------------------------------------------------------//
#include "commonheader.h"
#include "openorderprintdata.h"

//-------------------------------------------------------------------------------------------------//
// namespaces
//-------------------------------------------------------------------------------------------------//
namespace narcotics
{

//------------------------------------------------------------------------------
/*!	\throw	no-throw */
OpenOrderPrintData::OpenOrderPrintData ( const basar::VarString & rPsFile, const basar::VarString & rFormName, 
										 const basar::db::aspect::AccessorPropertyTableRef printData, 
										 const basar::db::aspect::AccessorPropertyTable_YIterator yitBranch,
										 const basar::VarString & rPeriod )					 
					 : m_PsFile( rPsFile ),
					   m_FormName( rFormName ),
					   m_OpenOrderData( printData ),
					   m_DataIterator( m_OpenOrderData.begin() ),
					   m_BranchIt( yitBranch ),
					   m_SendHeaderLines( true ),
					   m_Period( rPeriod )
{
}

//------------------------------------------------------------------------------
/*!	\throw	no-throw */
OpenOrderPrintData::~OpenOrderPrintData()
{
}

//------------------------------------------------------------------------------
/*!	\throw	no-throw */
basar::I18nString OpenOrderPrintData::getHeaderLines()
{
	// print header lines
	basar::I18nString headerLines;
	
	headerLines.format("%s%s%s%s%ld%s%s%s",
					basar::login::Manager::getInstance().getLanguageID().c_str(),
					"\n",
					m_Period.c_str(),
					"\n",
					m_BranchIt.getInt32("bgano"),
					"\n",
					m_BranchIt.getString("location_long").c_str(),
					"\n\n"
					);

	return headerLines;
}

//------------------------------------------------------------------------------
/*!	\throw	no-throw */
basar::I18nString OpenOrderPrintData::getDataLine()
{
	//print one data line
	basar::I18nString dataline;

	basar::DateTime dtTemp;
	dtTemp.setDate( m_DataIterator.getInt32("orderdate") );
	
	dataline.format("%s%-10.10s%s%9.3f%s%-4.4s%s%-40.40s%s%-40.40s%10ld%s",
					"$|",
					dtTemp.toStrDate().c_str(),
					"|",
					m_DataIterator.getDecimal("qtyorder").toFloat64(),
					"|",
					m_DataIterator.getString("unity").c_str(),
					"|",
					m_DataIterator.getString("articlename").c_str(),
					"|",
					m_DataIterator.getString("suppliername").c_str(),
					m_DataIterator.getInt32("bgano"), 			
					"|\n"
					);

	++m_DataIterator;

	if( m_DataIterator.isEnd() ){
		dataline.append("EODF\n");
	}	
	
	return dataline;
}

//------------------------------------------------------------------------------
/*!	\throw	no-throw */
void OpenOrderPrintData::onSendData( basar::cmnutil::CollPrintRawInput & collPrintRawInput )
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

	size_t readbytes = strlen (buffer);

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
		m_DataIterator		= m_OpenOrderData.begin();
	}
}

//------------------------------------------------------------------------------
/*!	\throw	no-throw 
	\return form name */
basar::VarString OpenOrderPrintData::getFormName() const
{
	return m_FormName;
}

//------------------------------------------------------------------------------
/*!	\throw	no-throw 
	\return ps-file name */
basar::VarString OpenOrderPrintData::getFormFileName() const
{
	return m_PsFile;
}

} //namespace narcotics