//-------------------------------------------------------------------------------------------------//
/*! \file
 *  \brief  implemtation of narcotics book print data
 *  \author	Thomas Hörath
 *  \date   26.05.2006
 */
//-------------------------------------------------------------------------------------------------//

//-------------------------------------------------------------------------------------------------//
// includes
//-------------------------------------------------------------------------------------------------//
#include "commonheader.h"
#include "narcbookprintdata.h"
#include "loggerpool.h"

#define NARCBOOK_DELIM	"|"
#define NEW_LINE		"\n"
#define LINES_PER_PAGE	22

//-------------------------------------------------------------------------------------------------//
// namespaces
//-------------------------------------------------------------------------------------------------//
namespace narcotics
{

//------------------------------------------------------------------------------
/*!	\throw	no-throw */
NarcBookPrintData::NarcBookPrintData (  const basar::VarString & rPsFile, const basar::VarString & rFormName,
										const basar::db::aspect::AccessorPropertyTableRef narcBookData,
										const CollOfNarcArtBookings* const pCollBookings )
					 : m_PsFile( rPsFile ),
					   m_FormName( rFormName ),
					   m_NarcArticles( narcBookData ),
					   m_ItArticles( m_NarcArticles.begin() ),
					   m_pCollBookings( pCollBookings ),
					   m_currentBookings( m_pCollBookings->find( m_ItArticles.getInt32( constants::ARTICLENO ) )->second ),
					   m_ItBookings( m_currentBookings.begin() ),
					   m_SendHeaderLines( true ),
					   m_currentArticleNo( -1 ),
					   m_LineNo( 0 )
{
}

//------------------------------------------------------------------------------
/*!	\throw	no-throw */
NarcBookPrintData::~NarcBookPrintData()
{
}

//------------------------------------------------------------------------------
/*!	\throw	no-throw */
basar::I18nString NarcBookPrintData::getHeaderLines()
{
	// print header lines
	basar::I18nString headerLines;

	headerLines.append( basar::login::Manager::getInstance().getLanguageID().c_str() );
	headerLines.append( NEW_LINE );
	headerLines.append( QApplication::translate("NarcBookPrintData", "Narcotics Booking Catalog").toLocal8Bit().data() );
	headerLines.append( NEW_LINE );
	headerLines.append( basar::login::Manager::getInstance().getAreaName().c_str() );
	headerLines.append( NEW_LINE );
	headerLines.append( QApplication::translate("NarcBookPrintData", "page").toLocal8Bit().data() );
	headerLines.append( NEW_LINE );
	headerLines.append( QApplication::translate("NarcBookPrintData", "of").toLocal8Bit().data() );
	headerLines.append( NEW_LINE );
	headerLines.append( QDate::currentDate().toString("dddd, ").toLocal8Bit().constData() );
	headerLines.append( NEW_LINE );
	headerLines.append( QDate::currentDate().toString("dd.MM.yyyy").toLocal8Bit().constData() );
	headerLines.append( NEW_LINE );
	headerLines.append( QApplication::translate("NarcBookPrintData", "signature narcotics-officer: ____________________").toLocal8Bit().data() );
	headerLines.append( NEW_LINE );
	headerLines.append( NEW_LINE );
	headerLines.append( QApplication::translate("NarcBookPrintData", "Art:").toLocal8Bit().data() );
	headerLines.append( NEW_LINE );
	headerLines.append( QApplication::translate("NarcBookPrintData", "balance:").toLocal8Bit().data() );
	headerLines.append( NEW_LINE );
	headerLines.append( NEW_LINE );
	headerLines.append( QApplication::translate("NarcBookPrintData", "date").toLocal8Bit().data() );
	headerLines.append( NEW_LINE );
	headerLines.append( QApplication::translate("NarcBookPrintData", "cvno").toLocal8Bit().data() );
	headerLines.append( NEW_LINE );
	headerLines.append( QApplication::translate("NarcBookPrintData", "c-s-no").toLocal8Bit().data() );
	headerLines.append( NEW_LINE );
	headerLines.append( QApplication::translate("NarcBookPrintData", "customer-supplier-name").toLocal8Bit().data() );
	headerLines.append( NEW_LINE );
	headerLines.append( QApplication::translate("NarcBookPrintData", "tt").toLocal8Bit().data() );
	headerLines.append( NEW_LINE );
	headerLines.append( QApplication::translate("NarcBookPrintData", "orderNo").toLocal8Bit().data() );
	headerLines.append( NEW_LINE );
	headerLines.append( QApplication::translate("NarcBookPrintData", "qtyTrans").toLocal8Bit().data() );
	headerLines.append( NEW_LINE );
	headerLines.append( QApplication::translate("NarcBookPrintData", "qtyStock").toLocal8Bit().data() );
	headerLines.append( NEW_LINE );

	return headerLines;
}

//------------------------------------------------------------------------------
/*!	\throw	no-throw */
basar::I18nString NarcBookPrintData::getGroupHeaderLine( basar::I18nString lineType, basar::Int32 balance )
{
	basar::I18nString dataline;

	dataline.format("%s%1.1s%-8.8s%1.1s%-9.9s%1.1s%-30.30s%1.1s%6ld",
				lineType.c_str(),
				NARCBOOK_DELIM,
				m_ItArticles.getString("articlecode").c_str(),
				NARCBOOK_DELIM,
				m_ItArticles.getString("packageunit").c_str(),
				NARCBOOK_DELIM,
				m_ItArticles.getString("articlename").c_str(),
				NARCBOOK_DELIM,
				balance
				);

	//BLOG_INFO(LoggerPool::loggerUseCases, m_ItArticles.getString("articlecode").c_str());
	//BLOG_INFO(LoggerPool::loggerUseCases, dataline.c_str());

	return dataline;
}

//------------------------------------------------------------------------------
/*!	\throw	no-throw */
basar::I18nString NarcBookPrintData::getDataLine()
{
	//print one data line
	basar::I18nString dataline;
	basar::I18nString lineType;

	//print group header again on next page!
	++m_LineNo;

	if( m_LineNo > LINES_PER_PAGE )
	{
		if( !m_ItBookings.isEnd() )
		{
			if( m_LineNo == LINES_PER_PAGE + 1 &&
				m_currentArticleNo == m_ItBookings.getInt32( constants::ARTICLENO ) )
			{
				//set balance for next page / same article
				if( !m_ItBookings.isBegin() )
				{
					--m_ItBookings;
					dataline = getGroupHeaderLine("$$$", m_ItBookings.getInt32("qtyinstock"));
					++m_ItBookings;
				}
				else{
					dataline = getGroupHeaderLine("$$$", m_ItArticles.getInt32("balance"));
				}


				dataline.append( NARCBOOK_DELIM );
				dataline.append( NEW_LINE );

				m_LineNo = 1;

				return dataline;
			}
		}
		else if( m_ItBookings.isBegin() && m_ItBookings.isEnd() )
		{
			if( m_LineNo == LINES_PER_PAGE + 1 )
			{
				dataline = getGroupHeaderLine("$$$", m_ItArticles.getInt32("balance"));

				dataline.append( NARCBOOK_DELIM );
				dataline.append( NEW_LINE );

				m_LineNo = 1;

				return dataline;
			}
		}

		m_LineNo = 1;
	}

	if( !m_ItBookings.isEnd() && m_currentArticleNo == m_ItArticles.getInt32( constants::ARTICLENO ) )
	{
		lineType = "   ";

		basar::DateTime		dtTemp;
		dtTemp.setDate( m_ItBookings.getInt32( constants::TRANSACTIONDATE ) );

		//! \todo FN OPL 438 "Anpassung des Monatsdrucks" -> Abgabebelegnummer!
		basar::VarString cvno;
		if( m_ItBookings.getInt32("contributionvoucherno") > 0 )
			cvno.format("%08d", m_ItBookings.getInt32("contributionvoucherno"));
		basar::VarString customersuppliername;
		customersuppliername = m_ItBookings.getString("customersuppliername") + " " +
			m_ItBookings.getString("customersupplierstreet") + " " +
			m_ItBookings.getString("customersuppliercip") + " " +
			m_ItBookings.getString("customersupplierlocation");

		// set prefix for quantity within the print version
		basar::VarString qtytransactionFormat;
		if (m_ItBookings.getString(constants::TRANSACTIONTYPE) == TransactionType::CUSTOMER_ORDER ||
			m_ItBookings.getString(constants::TRANSACTIONTYPE) == TransactionType::SUPPLIER_RETURNS ||
			m_ItBookings.getString(constants::TRANSACTIONTYPE) == TransactionType::DESTRUCTION ||
			m_ItBookings.getString(constants::TRANSACTIONTYPE) == TransactionType::STOCK_DIFFERENCE_DECREASE)
		{
			qtytransactionFormat.format("-%d", m_ItBookings.getInt32("qtytransaction"));
		}
		else
		{
			qtytransactionFormat.format("+%d", m_ItBookings.getInt32("qtytransaction"));
		}
		
		dataline.format("%3.3s%1.1s%10.10s%1.1s%9ld%1.1s%-110.110s%1.1s%-3.3s%1.1s%6ld%1.1s%6s%1.1s%6ld%1.1s%8.8s",
						lineType.c_str(),
						NARCBOOK_DELIM,
						dtTemp.toStrDate().c_str(),
						NARCBOOK_DELIM,
						m_ItBookings.getInt32("customersupplierno"),
						NARCBOOK_DELIM,
						customersuppliername.c_str(),
						NARCBOOK_DELIM,
						transactionType( m_ItBookings.getString(constants::TRANSACTIONTYPE) ).c_str(),
						NARCBOOK_DELIM,
						m_ItBookings.getInt32( constants::PURCHASEORDERNO ),
						NARCBOOK_DELIM,
						qtytransactionFormat.c_str(),
						NARCBOOK_DELIM,
						m_ItBookings.getInt32("qtyinstock"),
						NARCBOOK_DELIM,
						cvno.c_str());
		++m_ItBookings;
	}
	else
	{
		m_LineNo++;

		if( m_LineNo != LINES_PER_PAGE + 1 )
		{
			if( m_currentArticleNo != -1 ){
				++m_ItArticles;
			}

			if( m_ItArticles.isEnd() && m_ItBookings.isEnd() )
			{
				dataline.append("EOF\n"); // end of file
				return dataline;
			}

			m_currentArticleNo = m_ItArticles.getInt32( constants::ARTICLENO );
			m_currentBookings = m_pCollBookings->find( m_currentArticleNo )->second;
			m_ItBookings = m_currentBookings.begin();

			if( m_LineNo != LINES_PER_PAGE )
			{
				lineType = NEW_LINE;
				lineType.append("$$$");
				dataline = getGroupHeaderLine( lineType, m_ItArticles.getInt32("balance") );
			}
			else{
				dataline = NEW_LINE;
			}
		}
	}

	dataline.append( NARCBOOK_DELIM );
	dataline.append( NEW_LINE );

	if( m_ItArticles.isEnd() && m_ItBookings.isEnd() ){
		dataline.append("EOF\n"); // end of file
	}

	return dataline;
}

//------------------------------------------------------------------------------
/*!	\throw	no-throw */
void NarcBookPrintData::onSendData( basar::cmnutil::CollPrintRawInput & collPrintRawInput )
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
		if( !m_ItArticles.isEnd() ){
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
		m_SendHeaderLines	= true;
		m_currentArticleNo	= -1;
		m_ItArticles		= m_NarcArticles.begin();
		m_currentBookings	= m_pCollBookings->find( m_ItArticles.getInt32( constants::ARTICLENO ) )->second;
		m_ItBookings		= m_currentBookings.begin();
		m_LineNo			= 0;
	}
}

//------------------------------------------------------------------------------
/*!	\throw	no-throw
	\return form name */
basar::VarString NarcBookPrintData::getFormName() const
{
	return m_FormName;
}

//------------------------------------------------------------------------------
/*!	\throw	no-throw
	\return ps-file name */
basar::VarString NarcBookPrintData::getFormFileName() const
{
	return m_PsFile;
}

} //namespace narcotics