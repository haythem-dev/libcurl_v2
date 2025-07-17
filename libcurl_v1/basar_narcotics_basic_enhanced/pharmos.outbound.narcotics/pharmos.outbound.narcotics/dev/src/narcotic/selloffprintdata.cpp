//-------------------------------------------------------------------------------------------------//
/*! \file
 *  \brief  implemtation of sell off print data
 *  \author	Thomas Hörath
 *  \date   25.07.2006
 */
//-------------------------------------------------------------------------------------------------//

//-------------------------------------------------------------------------------------------------//
// includes
//-------------------------------------------------------------------------------------------------//
#include "commonheader.h"
#include "selloffprintdata.h"

//-------------------------------------------------------------------------------------------------//
// namespaces
//-------------------------------------------------------------------------------------------------//
namespace narcotics
{

//------------------------------------------------------------------------------
/*!	\throw	no-throw */
SellOffPrintData::SellOffPrintData ( const basar::VarString & rPsFile, const basar::VarString & rFormName, 
									 const basar::db::aspect::AccessorPropertyTable_YIterator transactData,
									 const basar::db::aspect::AccessorPropertyTable_YIterator branchData,
									 const basar::db::aspect::AccessorPropertyTable_YIterator custsupplData )
					 : m_PsFile( rPsFile ),
					   m_FormName( rFormName ),
					   m_TransactIt( transactData ),
					   m_BranchIt( branchData ),
					   m_CustSupplIterator( custsupplData ),
					   m_DataSent( false )
{
}

//------------------------------------------------------------------------------
/*!	\throw	no-throw */
SellOffPrintData::~SellOffPrintData()
{
}

//------------------------------------------------------------------------------
/*!	\throw	no-throw */
basar::I18nString SellOffPrintData::getDataLines()
{
	//print one data line
	basar::I18nString dataline;
	
	basar::I18nString custSupplInfo;
	
	if( m_TransactIt.getString(constants::TRANSACTIONTYPE) == TransactionType::CUSTOMER_ORDER )
	{
		custSupplInfo.format("%07ld%s%s%s%s%s%s%s%s",
							m_CustSupplIterator.getInt32("customerno"),
							"\n",
							m_CustSupplIterator.getString("customername").c_str(),
							"\n",
							m_CustSupplIterator.getString("street").c_str(),
							"\n",
							m_CustSupplIterator.getString("postcode").c_str(),
							" ",
							m_CustSupplIterator.getString("customerlocation").c_str()	
							);
	}
	else
	{
		custSupplInfo.format("%07ld%s%s%s%s%s%s%s%s",
							m_CustSupplIterator.getInt32("narcoticssupplierno"),
							"\n",
							m_CustSupplIterator.getString("suppliername").c_str(),
							"\n",
							m_CustSupplIterator.getString("street").c_str(),
							"\n",
							m_CustSupplIterator.getString("postcode").c_str(),
							" ",
							m_CustSupplIterator.getString("supplierlocation").c_str()	
							);
	}

	basar::I18nString packUnity;
	basar::I18nString extUnity;
	splitUnity( m_TransactIt.getString("packageunit"), packUnity, extUnity );
	
	basar::DateTime	dtTemp;
	dtTemp.setDate( m_TransactIt.getInt32( constants::TRANSACTIONDATE ) );
	basar::I18nString dateFormatted = dtTemp.toString( "%02d%02m%y" );
	
	dataline.format("%-6.6s%s%07ld%s%05ld%s%-4.4s%s%s%s%s%s%07ld%s%s%s%s%s%s%s%s%s%s%s",
					dateFormatted.c_str(),
					"\n",
					m_TransactIt.getInt32( constants::ARTICLENO ),
					"\n",
					m_TransactIt.getInt32( "qtytransaction" ),
					"\n",
					packUnity.c_str(),
					"\n",
					extUnity.c_str(),
					"\n",
					m_TransactIt.getString( "articlename" ).c_str(),
					"\n\n",
					m_BranchIt.getInt32( "bgano" ),
					"\n",
					QApplication::translate( "SellOffPrintData", "PHOENIX Pharmahandel GmbH & Co. KG" ).toLocal8Bit().data(),
					"\n",
					m_BranchIt.getString( "street" ).c_str(),
					"\n",
					m_BranchIt.getString( "postcode" ).c_str(),
					" ",
					m_BranchIt.getString( "location" ).c_str(),
					"\n\n",
					custSupplInfo.c_str(),
					"\n"					
					);

	return dataline;
}

//------------------------------------------------------------------------------
/*!	\throw	no-throw */
void SellOffPrintData::onSendData( basar::cmnutil::CollPrintRawInput & collPrintRawInput )
{
	static const basar::Int32 BUFLEN = 1024;
	char   buffer[BUFLEN];
	const char * pBuf;
	const char * pBufEnd;
	basar::Int32  i;
	
	if( !m_DataSent )
	{
		strcpy( buffer, getDataLines().c_str() );
		m_DataSent = true;
	}
	else{
		strcpy( buffer, "\0" );
	}

	size_t readbytes = strlen( buffer );

	for( i = 0, pBuf = buffer, pBufEnd = ( buffer + readbytes ); 
		pBuf < pBufEnd; 
		pBuf++ )
	{
		collPrintRawInput.push( *pBuf );
	}

	if( 0 == readbytes ){
		m_DataSent = false;
	}
}

//------------------------------------------------------------------------------
/*!	\throw	no-throw 
	\return form name */
basar::VarString SellOffPrintData::getFormName() const
{
	return m_FormName;
}

//------------------------------------------------------------------------------
/*!	\throw	no-throw 
	\return ps-file name */
basar::VarString SellOffPrintData::getFormFileName() const
{
	return m_PsFile;
}

} //namespace narcotics