//----------------------------------------------------------------------------
/*! \file
 *  \brief  print data class contribution voucher
 *  \author Benjamin Endlicher
 *  \date   09.09.2010
 */
//----------------------------------------------------------------------------

//-------------------------------------------------------------------------------------------------//
// includes
//-------------------------------------------------------------------------------------------------//
#include "commonheader.h"
#include "contributionvoucherprintdata.h"
#include "loggerpool.h"

//-------------------------------------------------------------------------------------------------//
// namespaces
//-------------------------------------------------------------------------------------------------//
namespace narcotics {

//-------------------------------------------------------------------------------------------------//
using basar::cmnutil::PrintTools;

//------------------------------------------------------------------------------
/*!	\throw	no-throw */
PrintDataBase::PrintDataBase (const basar::VarString & rPsFile, const basar::VarString & rFormName, 
															const basar::db::aspect::AccessorPropertyTableRef articleData,
															const basar::db::aspect::AccessorPropertyTable_YIterator branchData,
															const basar::db::aspect::AccessorPropertyTable_YIterator customerData,
															const basar::db::aspect::AccessorPropertyTable_YIterator signatureLnk,
															basar::Int32 beginOfOrder,
															basar::Int32 endOfOrder,
															basar::VarString form
														   )
					 : m_PsFile  (rPsFile),
					   m_FormName(rFormName),
					   m_ArticleData(articleData),
					   m_BranchIt(branchData),
					   m_CustomerIt(customerData),
					   m_SignatureLnkIt( signatureLnk ),
					   m_Counter( beginOfOrder ),
					   m_EndOfOrder( endOfOrder ),
					   m_Form( form ),
					   m_HeaderDataSent(false),
					   m_FooterDataSent( false ),
					   m_ArticleDataSent( false ),
					   m_Logged( false )
{
	m_ArticleIt = m_ArticleData.begin();
	// get starting position of iterator
	for( int i = 0; i < beginOfOrder; i++ )
	{
		m_ArticleIt++;
	}
}

//------------------------------------------------------------------------------
/*!	\throw	no-throw */
PrintDataBase::~ContributionVoucherPrintData()
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
		splitUnity(m_ArticleIt.getString("packageunit"), packUnity, extUnity);

		// expand packUnity and extUnity to 6 signs if they're shorter
		packUnity.trim();
		extUnity.trim();
		while( packUnity.length() < 6 )
		{
			packUnity += " ";
		}
		while( extUnity.length() < 6 )
		{
			extUnity += " ";
		}
		articledataline.format("%s%s%07ld%s%s%5ld%s%s%s%s%s%s%s%s%s%s",
								"p01 ",
								"PZN ",
								m_ArticleIt.getInt32("articleno"),
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
								"\n");
		m_ArticleIt++;
		m_Counter++;
	}

	if( m_Counter == m_EndOfOrder )
	{
		m_ArticleDataSent = true;
	}

	return articledataline;
}

//------------------------------------------------------------------------------
/*!	\throw	no-throw */
basar::VarString PrintDataBase::getBranchDataLine()
{
	// get branch data
	basar::VarString branchData;	
	branchData.format("%s%s%s%s%s%s%s%s%s",
						"h05 "
						"PHOENIX Pharmahandel GmbH & Co. KG",
						"\n",
						"h06 ",
						m_BranchIt.getString("street").c_str(),
						"\n",
						"h07 ",
						m_BranchIt.getString("postcode").c_str(),
						" ",
						m_BranchIt.getString("location").c_str()
						);
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

	customerData.format("%s%s%s%s%s%s%s%s%s%s",
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
basar::VarString PrintDataBase::getTitleLine()
{
	// header Abgabemeldung
	basar::VarString titleA = "h01 Abgabemeldung";
	// header Lieferscheindoppel
	basar::VarString titleL = "h01 Lieferscheindoppel";
	// header Empfangsbestätigung
	basar::VarString titleE = "h01 Empfangsbestätigung";

	if( m_Form == constants::ABGABE )
	{
		return titleA;
	}
	else if( m_Form == constants::LIEFER )
	{
		return titleL;
	}
	else if( m_Form == constants::EMPFANG )
	{
		return titleE;
	}
	return basar::VarString();
}
//------------------------------------------------------------------------------
/*!	\throw	no-throw */
basar::VarString PrintDataBase::getGlobalHeaderLines()
{
	basar::VarString headerData;

	basar::VarString contributionvouchernotxt;
	contributionvouchernotxt.itos( m_ArticleIt.getInt32("contributionvoucherno") );

	basar::VarString transactiondatetxt;
	transactiondatetxt.itos( m_ArticleIt.getInt32("transactiondate") );

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
	headerData += "h11 Signaturlink ";
	headerData += m_SignatureLnkIt.getString("signaturelnk");
	headerData += "\n";

	// logging
	if( LoggerPool::loggerUseCases.isEnabledFor(log4cplus::INFO_LOG_LEVEL) ){
		basar::VarString msg;
		msg.format("\n"
				   "  Header data \n%s",
				   headerData.c_str());
		LoggerPool::loggerUseCases.forcedLog(log4cplus::INFO_LOG_LEVEL, msg, __FILE__, __LINE__);
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
	if( m_ArticleData.begin().contains( "rectificationnote" ) && 
		!(m_ArticleData.begin().getState( "rectificationnote" ) == basar::SS_UNSET) )
	{
		rectificationNote = m_ArticleData.begin().getString("rectificationnote");
	}

	footerData.format("%s%07ld%s%s%s%s%s%s%s%s%s%s",
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
		if( LoggerPool::loggerUseCases.isEnabledFor(log4cplus::INFO_LOG_LEVEL) ){
			basar::VarString msg;
			msg.format("\n"
						"  Header data \n%s",
						data.c_str());
			LoggerPool::loggerUseCases.forcedLog(log4cplus::INFO_LOG_LEVEL, msg, __FILE__, __LINE__);
		}
		m_Logged = true;
	}
	return data;
}

//------------------------------------------------------------------------------
/*!	\throw	no-throw */
void PrintDataBase::onSendData(basar::cmnutil::CollPrintRawInput & collPrintRawInput)
{
	PrintTools::insertStr2Coll(getData(), collPrintRawInput); 
}

//------------------------------------------------------------------------------
/*!	\throw	no-throw 
	\return form name */
basar::VarString PrintDataBase::getFormName() const
{
	return m_FormName;
}

//------------------------------------------------------------------------------
/*!	\throw	no-throw 
	\return ps-file name */
basar::VarString PrintDataBase::getFormFileName() const
{
	return m_PsFile;
}

//------------------------------------------------------------------------------
/*!	\throw	no-throw */
//void ContributionVoucherPrintData::splitUnity(basar::VarString unity, basar::VarString & rPack, basar::VarString & rExtension	)
//{
//	unity.trim(basar::cmnutil::BString::FIRST_END);
//
//	basar::UInt32 counter	= 0;
//	size_t length			= unity.length();
//	char sign;
//
//	while ( counter < length )
//	{
//		sign = unity.at(counter);
//		
//		if ( isdigit(sign) || sign == ',' || sign == '.' || sign == 'x' || sign == 'X' || sign == ' ')
//			rPack += sign;
//		else
//			break;
//		
//		++counter;
//	}
//	rExtension = unity.substr(counter, length-counter);
//}

//----------------------------------------------------------------------------
}	// namespace narcotics
