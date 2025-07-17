//----------------------------------------------------------------------------
/*! \file
 *  \brief  print data class for manually created contribution voucher
 *  \author Benjamin Endlicher
 *  \date   04.05.2010
 */
//----------------------------------------------------------------------------

//-------------------------------------------------------------------------------------------------//
// includes
//-------------------------------------------------------------------------------------------------//
#include "commonheader.h"
#include "manualcontributionvoucherprintdata.h"

//-------------------------------------------------------------------------------------------------//
// namespaces
//-------------------------------------------------------------------------------------------------//
namespace narcotics 
{

//------------------------------------------------------------------------------
/*!	\throw	no-throw */
ManualContributionVoucherPrintData::ManualContributionVoucherPrintData ( const basar::VarString & rPsFile, const basar::VarString & rFormName, 
																		 const basar::db::aspect::AccessorPropertyTableRef articleData,
																		 const basar::db::aspect::AccessorPropertyTable_YIterator branchData,
																		 const basar::db::aspect::AccessorPropertyTable_YIterator customerData )
					 : m_PsFile( rPsFile ),
					   m_FormName( rFormName ),
					   m_ArticleData( articleData ),
					   m_BranchIt( branchData ),
					   m_CustomerIt( customerData ),
					   m_DataSent( false ),
					   m_DateLineSent( false ),
					   m_ArticleDataSent( false ),
					   m_BranchCustomerDataSent( false ),
					   m_RowCounter( 0 ),
					   m_MAXIMUM_NUMBER_OF_PRINTABLE_LINES( 4 )
{
	m_ArticleIt = m_ArticleData.begin();
}

//------------------------------------------------------------------------------
/*!	\throw	no-throw */
ManualContributionVoucherPrintData::~ManualContributionVoucherPrintData()
{
}

//------------------------------------------------------------------------------
/*!	\throw	no-throw */
basar::I18nString ManualContributionVoucherPrintData::getArticleDataLines()
{
		
	//print one data line
	basar::I18nString articledataline;
	bool lastRealLine = false;
	
	// set article dataline for printing
	if( !m_ArticleIt.isEnd() )
	{
		// split packageunit
		basar::I18nString packUnity;
		basar::I18nString extUnity;
		splitUnity(m_ArticleIt.getString("packageunit"), packUnity, extUnity);

		articledataline.format("%07ld%s%05ld%s%-4.4s%s%s%s%s%s",
								m_ArticleIt.getInt32(constants::ARTICLENO),
								"\n",
								m_ArticleIt.getInt32("qtytransaction"),
								"\n",
								packUnity.c_str(),
								"\n",
								extUnity.c_str(),
								"\n",
								m_ArticleIt.getString("articlename").c_str(),
								"\n");
		lastRealLine = true;
		++m_ArticleIt;
		++m_RowCounter;
	}

	if( m_ArticleIt.isEnd() && ( m_ArticleData.size() < static_cast<unsigned>( m_MAXIMUM_NUMBER_OF_PRINTABLE_LINES ) ) && !lastRealLine )
	{
		if( m_RowCounter < m_MAXIMUM_NUMBER_OF_PRINTABLE_LINES )
		{
			articledataline.format("%s%s%s%s%s%s%s%s%s%s",
									" ",
									"\n",
									" ",
									"\n",
									" ",
									"\n",
									" ",
									"\n",
									" ",
									"\n");
			++m_RowCounter;
		}
	}

	if( m_RowCounter == m_MAXIMUM_NUMBER_OF_PRINTABLE_LINES ){
		articledataline.append("\n");
	}

	return articledataline;
}

//------------------------------------------------------------------------------
/*!	\throw	no-throw */
basar::I18nString ManualContributionVoucherPrintData::getBranchCustomerDataLine()
{
		
	//print one data line
	basar::I18nString branchcustomerdataline;

		// get branch data
	basar::I18nString branchInfo;

	branchInfo.format("%07ld%s%s%s%s%s%s%s%s",
						m_BranchIt.getInt32("bgano"),
						"\n",
						QApplication::translate("ManualContributionVoucherPrintData", "PHOENIX Pharmahandel GmbH & Co. KG").toLocal8Bit().data(),
						"\n",
						m_BranchIt.getString("street").c_str(),
						"\n",
						m_BranchIt.getString("postcode").c_str(),
						" ",
						m_BranchIt.getString("location").c_str()	
						);

	branchcustomerdataline.format("%07ld%s%s%s%s%s%s%s%s%s%s%s",
					m_CustomerIt.getInt32("customerno"),
					"\n",
					m_CustomerIt.getString("customername").c_str(),
					"\n",
					m_CustomerIt.getString("street").c_str(),
					"\n",
					m_CustomerIt.getString("postcode").c_str(),
					" ",
					m_CustomerIt.getString("customerlocation").c_str(),
					"\n\n",
					branchInfo.c_str(),
					"\n"
					);

	return branchcustomerdataline;
}

//------------------------------------------------------------------------------
/*!	\throw	no-throw */
basar::I18nString ManualContributionVoucherPrintData::getDateLine()
{	
	//print one data line
	basar::I18nString dateline;

	// get current date
	// new date format needed because of ssuk upgrade
	basar::I18nString dtTemp = basar::DateTime::getCurrent().toString("%02d%02m%y");

	dateline.format("%-6.6s%s",
					dtTemp.c_str(),
					"\n");

	return dateline;
}

//------------------------------------------------------------------------------
/*!	\throw	no-throw */
void ManualContributionVoucherPrintData::onSendData( basar::cmnutil::CollPrintRawInput & collPrintRawInput )
{
	static const basar::Int32 BUFLEN = 1024;
	char   buffer[BUFLEN];
	const char * pBuf;
	const char * pBufEnd;
	basar::Int32  i;
	bool branchCustomerDataJustSent;

	if( !m_ArticleDataSent && m_DateLineSent )
	{
		if( m_RowCounter < m_MAXIMUM_NUMBER_OF_PRINTABLE_LINES ){
			strcpy( buffer, getArticleDataLines().c_str() );
		}
		else{
			m_ArticleDataSent = true;
		}
	}
	branchCustomerDataJustSent = false;

	if( !m_BranchCustomerDataSent && m_ArticleDataSent )
	{
		m_BranchCustomerDataSent = true;
		branchCustomerDataJustSent = true;
		strcpy( buffer, getBranchCustomerDataLine().c_str() );
	}
	if( !m_DateLineSent )
	{
		m_DateLineSent = true;	
		strcpy( buffer, getDateLine().c_str() );
	}
	if( m_DateLineSent && 
		m_ArticleDataSent && 
		m_BranchCustomerDataSent && 
		!branchCustomerDataJustSent )
	{
		strcpy(buffer,"\0");
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
		// reset bools and set iterator to begin of data
		m_DateLineSent				= false;	
		m_BranchCustomerDataSent	= false;
		m_ArticleDataSent			= false;
		m_ArticleIt = m_ArticleData.begin();
	}
}


//------------------------------------------------------------------------------
/*!	\throw	no-throw 
	\return form name */
basar::VarString ManualContributionVoucherPrintData::getFormName() const
{
	return m_FormName;
}

//------------------------------------------------------------------------------
/*!	\throw	no-throw 
	\return ps-file name */
basar::VarString ManualContributionVoucherPrintData::getFormFileName() const
{
	return m_PsFile;
}

} //namespace narcotics