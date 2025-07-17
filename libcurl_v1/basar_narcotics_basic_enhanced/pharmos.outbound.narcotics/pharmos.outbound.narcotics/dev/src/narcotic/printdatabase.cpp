//----------------------------------------------------------------------------
/*! \file
 *  \brief  print data class contribution voucher
 *  \author Harald Bayer
 *  \date   17.03.2011
 */
//----------------------------------------------------------------------------

//-------------------------------------------------------------------------------------------------//
// includes
//-------------------------------------------------------------------------------------------------//
#include "commonheader.h"
#include "printdatabase.h"
#include "loggerpool.h"

//-------------------------------------------------------------------------------------------------//
// namespaces
//-------------------------------------------------------------------------------------------------//
namespace narcotics
{

//-------------------------------------------------------------------------------------------------//
using basar::cmnutil::PrintTools;

//------------------------------------------------------------------------------
/*!	\throw	no-throw */
PrintDataBase::PrintDataBase (	const basar::db::aspect::AccessorPropertyTableRef articleData,
								const basar::db::aspect::AccessorPropertyTable_YIterator branchData,
								const basar::db::aspect::AccessorPropertyTable_YIterator customerData,
								const basar::db::aspect::AccessorPropertyTable_YIterator signatureLnk,
								basar::Int32 beginOfOrder,
								basar::Int32 endOfOrder															
								)
					 : m_ArticleData( articleData ),
					   m_BranchIt( branchData ),
					   m_CustomerIt( customerData ),
					   m_SignatureLnkIt( signatureLnk ),
					   m_Counter( beginOfOrder ),
					   m_EndOfOrder( endOfOrder ),					   
					   m_HeaderDataSent( false ),
					   m_FooterDataSent( false ),
					   m_ArticleDataSent( false ),
					   m_Logged( false )
{
	m_ArticleIt = m_ArticleData.begin();
	// get starting position of iterator
	for( int i = 0; i < beginOfOrder; i++ ){
		m_ArticleIt++;
	}
}

//------------------------------------------------------------------------------
/*!	\throw	no-throw */
PrintDataBase::~PrintDataBase()
{
}

//------------------------------------------------------------------------------
/*!	\throw	no-throw */
basar::VarString PrintDataBase::getArticleDataLines()
{
	// print one data line
	basar::VarString articledataline;
	
	// set article dataline for printing
	if( !m_ArticleIt.isEnd() )
	{
		// split packageunit
		basar::VarString packUnity;
		basar::VarString extUnity;
		
		splitUnity( m_ArticleIt.getString("packageunit"), packUnity, extUnity );

		// expand packUnity and extUnity to 6 signs if they're shorter
		packUnity.trim();
		extUnity.trim();
		
		while( packUnity.length() < 6 ){
			packUnity += " ";
		}
		
		while( extUnity.length() < 6 ){
			extUnity += " ";
		}
		
		basar::VarString articleCode = m_ArticleIt.getString( "articlecode" );
		
		std::stringstream stream;

		stream << std::setfill('0') << std::setw(8) << articleCode;

		articleCode = stream.str().c_str();
		
		articledataline.format( "%s%s%s%s%s%5ld%s%s%s%s%s%s%s%s%s%s",
								"p01 ",
								"PZN ",
								articleCode.c_str(),
								" ",
								"Anzahl ",
								m_ArticleIt.getInt32("qtytransaction"),
								" ",
								"Packungseinheit ",
								packUnity.c_str(),
								" ",
								"Maßeinheit ",
								extUnity.c_str(),
								" ",
								"Bezeichnung ",
								m_ArticleIt.getString("articlename").c_str(),
								"\n" );
		m_ArticleIt++;
		m_Counter++;
	}

	if( m_Counter == m_EndOfOrder ){
		m_ArticleDataSent = true;
	}

	return articledataline;
}

//------------------------------------------------------------------------------
/*!	\throw	no-throw */
basar::VarString PrintDataBase::getBranchDataLine()
{
	
	//EXTREM UNSAUBER!! Wenn namensaenderung -> Codeaenderung noetig
	//TODO: Anpassen!! evtl synonym von zpps1 rfiliale nach zpos1? klaeren mit Herrn Kiefert
	basar::VarString branchData;	
	basar::I18nString reportCompanyName;

	basar::I18nString branchnotxt;
	branchnotxt.itos(basar::login::Manager::getInstance().getAreaID());
	if (!basar::login::Manager::getInstance().getParaValue(branchnotxt,UADM_REPORT_COMPANY_NAME, reportCompanyName))
	{
		if (!basar::login::Manager::getInstance().getParaValue(UADM_REPORT_COMPANY_NAME, reportCompanyName))
			reportCompanyName = "PHOENIX Pharmahandel GmbH & Co. KG";
	}
	
	if ( !m_BranchIt.isNull() && !m_BranchIt.isEnd() )
	{
		//print header lines
		if (m_BranchIt.getInt16("branchno") != 21)
		{
			branchData.format(	"%s%s%s%s%s%s%s%s%s%s",
								"h05 ",
								reportCompanyName.c_str(),
								"\n",
								"h06 ",
								m_BranchIt.getString("street").c_str(),
								"\n",
								"h07 ",
								m_BranchIt.getString("postcode").c_str(),
								" ",
								m_BranchIt.getString("location").c_str()
							);		
		}
		else
		{
			branchData.format(	"%s%s%s%s%s%s%s%s%s%s",
								"h05 ",
								reportCompanyName.c_str(),
								"\n",
								"h06 ",
								m_BranchIt.getString("street").c_str(),
								"\n",
								"h07 ",
								m_BranchIt.getString("postcode").c_str(),
								" ",
								m_BranchIt.getString("location").c_str()
							);			
		}
	}

	return branchData;
}

//------------------------------------------------------------------------------
/*!	\throw	no-throw */
basar::VarString PrintDataBase::getCustomerDataLine()
{
	// get branch data
	basar::VarString customerData;
	basar::VarString customerSupplierName = "";
	basar::VarString customerSupplierLocation = "";
	
	if( m_CustomerIt.contains( "customersuppliername" ) && 
		!(m_CustomerIt.getState( "customersuppliername" ) == basar::SS_UNSET) )
	{
		customerSupplierName = m_CustomerIt.getString("customersuppliername");
	}
	else if( m_CustomerIt.contains( "customername" ) && 
		!(m_CustomerIt.getState( "customername" ) == basar::SS_UNSET) )
	{
		customerSupplierName = m_CustomerIt.getString("customername");
	}
    else if( m_CustomerIt.contains( "suppliername" ) &&
        !(m_CustomerIt.getState("suppliername") == basar::SS_UNSET) )
    {
        customerSupplierName = m_CustomerIt.getString("suppliername");
    }

	if( m_CustomerIt.contains( "customersupplierlocation" ) && 
		!(m_CustomerIt.getState( "customersupplierlocation" ) == basar::SS_UNSET) )
	{
		customerSupplierLocation = m_CustomerIt.getString("customersupplierlocation");
	}
	else if( m_CustomerIt.contains( "customerlocation" ) && 
		!(m_CustomerIt.getState( "customerlocation" ) == basar::SS_UNSET) )
	{
		customerSupplierLocation = m_CustomerIt.getString("customerlocation");
	}
    else if( m_CustomerIt.contains( "supplierlocation" ) && 
		!(m_CustomerIt.getState( "supplierlocation" ) == basar::SS_UNSET) )
	{
		customerSupplierLocation = m_CustomerIt.getString("supplierlocation");
	}

	customerData.format(	"%s%s%s%s%s%s%s%s%s%s",
							"f04 ",
							customerSupplierName.c_str(),
							"\n",
							"f05 ",
							m_CustomerIt.getString("street").c_str(),
							"\n",
							"f06 ",
							m_CustomerIt.getString("postcode").c_str(),
							" ",
							customerSupplierLocation.c_str()
						);
	return customerData;
}

//------------------------------------------------------------------------------
/*!	\throw	no-throw */
basar::VarString PrintDataBase::getPostPrintingInformation()
 {
	 return getGlobalHeaderLines();
 }

//------------------------------------------------------------------------------
/*!	\throw	no-throw */
basar::VarString PrintDataBase::getGlobalHeaderLines()
{
	// logging
	if( LoggerPool::loggerUseCases.isEnabledFor(log4cplus::INFO_LOG_LEVEL) )
	{
		basar::VarString msg;
		msg.format("Printing %s < %ld > by user < %s > ...", 
			getTitleLine().c_str(), m_ArticleIt.getInt32("contributionvoucherno"), m_SignatureLnkIt.getString("username").c_str() );
		LoggerPool::loggerUseCases.log(log4cplus::INFO_LOG_LEVEL, msg, __FILE__, __LINE__);
	}

	basar::VarString headerData;

	basar::VarString contributionvouchernotxt;
	contributionvouchernotxt.itos( m_ArticleIt.getInt32("contributionvoucherno") );

	basar::VarString transactiondatetxt;
	transactiondatetxt.itos( m_ArticleIt.getInt32(constants::TRANSACTIONDATE) );

	basar::VarString bganotxt;
	bganotxt.itos( m_BranchIt.getInt32("bgano") );

	headerData = getTitleLine().c_str();
	headerData += "\n";
	headerData += "h02 Abgabebeleg-Nummer ";
	headerData += contributionvouchernotxt;
	headerData += "\n";
	headerData += "h03 Abgabedatum ";
	headerData += transactiondatetxt;
	headerData += "\n";
	headerData += "h04 BTM-Nummer ";
	headerData += bganotxt;
	headerData += "\n";
	headerData += getBranchDataLine().c_str();
	headerData += "\n";
	headerData += "h08 Unterschrift Bearbeiter ";	
	headerData += m_SignatureLnkIt.getString("username");
	headerData += "\n";
	if( printTour() )
	{
		headerData += "h10 Tour ";
		headerData += m_ArticleIt.getString(constants::TOURID);
		headerData += "\n";
	}
	
	if( printDeliveryNoteNumber() )
	{
		basar::VarString deliveryNoteNumber;
		deliveryNoteNumber.itos(m_ArticleIt.getInt32(constants::PURCHASEORDERNO));

		//pad to length 6
		if( deliveryNoteNumber.length() < 6 )
		{
			std::stringstream ss;
			ss << std::setw(6) << deliveryNoteNumber;
			deliveryNoteNumber = ss.str();
		}

		headerData += "h12 Lieferscheinnummer ";
		headerData += deliveryNoteNumber;
		headerData += "\n";
	}

	if( !m_SignatureLnkIt.isNull() && m_SignatureLnkIt.isContainedAndSet("signaturelnk") )
	{		
		headerData += "h11 Signaturlink ";
		headerData += m_SignatureLnkIt.getString("signaturelnk");
		headerData += "\n";	
	}	

	// logging
	if( LoggerPool::loggerUseCases.isEnabledFor(log4cplus::TRACE_LOG_LEVEL) )
	{
		basar::VarString msg;
		msg.format("\n"
				   "  Header data \n%s",
				   headerData.c_str());
		LoggerPool::loggerUseCases.log(log4cplus::TRACE_LOG_LEVEL, msg, __FILE__, __LINE__);
	}

	return headerData;
}

//------------------------------------------------------------------------------
/*!	\throw	no-throw */
basar::VarString PrintDataBase::getGlobalFooterLines()
{
	basar::VarString footerData;
	basar::Int32 customerSupplierNo = 0;
	basar::VarString rectificationNote = "";

	if( m_CustomerIt.contains( "customerno" ) && 
		!(m_CustomerIt.getState( "customerno" ) == basar::SS_UNSET) )
	{
		customerSupplierNo = m_CustomerIt.getInt32("customerno");
	}
	else if( m_CustomerIt.contains( "customersupplierno" ) && 
		!(m_CustomerIt.getState( "customersupplierno" ) == basar::SS_UNSET) )
	{
		customerSupplierNo = m_CustomerIt.getInt32("customersupplierno");
	}
    else if( m_CustomerIt.contains( "narcoticssupplierno" ) &&
        !(m_CustomerIt.getState( "narcoticssupplierno" ) == basar::SS_UNSET) )
    {
        customerSupplierNo = m_CustomerIt.getInt32("narcoticssupplierno");
    }
	if (m_ArticleData.begin().isContainedAndSet(constants::RECTIFICATIONTEXT))
	{
		rectificationNote = m_ArticleData.begin().getString(constants::RECTIFICATIONTEXT);
	}
	if (m_ArticleData.begin().isContainedAndSet(constants::RECTIFICATIONCOMMENT))
	{
		rectificationNote.append(" " + m_ArticleData.begin().getString(constants::RECTIFICATIONCOMMENT));
	}

	footerData.format(	"%s%07ld%s%s%s%s%s%s%s%s%s%s",
						"f01 BTM-Nummer Erwerber ",
						customerSupplierNo,
						"\n",
						"f02 Empfangsdatum ",
						"\n",
						"f03 ", 
						rectificationNote.c_str(),
						"\n",
						getCustomerDataLine().c_str(),
						"\n",
						"f08 Unterschrift Erwerber Willi Mustermann",
						"\n"
					);

	return footerData;
}

//------------------------------------------------------------------------------
/*!	\throw	no-throw */
basar::VarString PrintDataBase::getData()
{
	basar::VarString data;
	
	// add header data
	if( !m_HeaderDataSent )
	{
		data = getGlobalHeaderLines();
		m_HeaderDataSent = true;
	}

	// add article data
	while( !m_ArticleIt.isEnd() && !m_ArticleDataSent )
	{
		data += getArticleDataLines();
	}

	// add footer data
	if( m_ArticleDataSent && !m_FooterDataSent )
	{
		data += getGlobalFooterLines();
		m_FooterDataSent = true;
	}

	if( m_HeaderDataSent&& m_ArticleDataSent && m_FooterDataSent && !m_Logged )
	{
		// logging
		if( LoggerPool::loggerUseCases.isEnabledFor(log4cplus::TRACE_LOG_LEVEL) )
		{
			basar::VarString msg;
			msg.format("\n"
						"  Header data \n%s",
						data.c_str());
			LoggerPool::loggerUseCases.log(log4cplus::TRACE_LOG_LEVEL, msg, __FILE__, __LINE__);
		}
		m_Logged = true;
	}

	return data;
}

//------------------------------------------------------------------------------
/*!	\throw	no-throw */
void PrintDataBase::onSendData( basar::cmnutil::CollPrintRawInput & collPrintRawInput )
{
	basar::VarString data = getData();
	
	if( LoggerPool::loggerUseCases.isEnabledFor(log4cplus::TRACE_LOG_LEVEL) ){		
		LoggerPool::loggerUseCases.forcedLog(log4cplus::TRACE_LOG_LEVEL, data.c_str(), __FILE__, __LINE__);
	}

	PrintTools::insertStr2Coll( data, collPrintRawInput ); 
}

} //namespace narcotics