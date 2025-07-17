//----------------------------------------------------------------------------
/*! \file
 *  \brief  standard library functions narcotics
 *  \author Thomas Hörath
 *  \date   10.02.2006
 */
//------------------------------------------------------------------------------
// include section
//------------------------------------------------------------------------------
#include "commonheader.h"
#include "version.h"
#include "constants.h"

//------------------------------------------------------------------------------
// namespace section
//------------------------------------------------------------------------------
namespace narcotics
{

//------------------------------------------------------------------------------
/*!	\throw	no-throw */
basar::VarString transactionType (basar::VarString type)
{
	basar::VarString transType;

	type.trim();
	type.lower();
	
	if( type == TransactionType::CUSTOMER_ORDER )
	{
		transType = QApplication::translate( "functions.cpp", "PS" ).toLocal8Bit().data();
	}
	else if( type == TransactionType::GOODSIN_DELIVERY )
	{
		transType = QApplication::translate( "functions.cpp", "WE" ).toLocal8Bit().data();
	}
	else if( type == TransactionType::CUSTOMER_RETURNS )
	{
		transType = QApplication::translate( "functions.cpp", "RK" ).toLocal8Bit().data();
	}
	else if( type == TransactionType::SUPPLIER_RETURNS )
	{
		transType = QApplication::translate( "functions.cpp", "PS" ).toLocal8Bit().data();
	}
	else if( type == TransactionType::STOCK_DIFFERENCE_INCREASE )
	{
		transType = QApplication::translate( "functions.cpp", "GZ" ).toLocal8Bit().data();
	}
	else if( type == TransactionType::STOCK_DIFFERENCE_DECREASE )
	{
		transType = QApplication::translate( "functions.cpp", "GA" ).toLocal8Bit().data();
	}
	else if( type == TransactionType::DESTRUCTION )
	{
		transType = QApplication::translate( "functions.cpp", "VN" ).toLocal8Bit().data();
	}
	else
	{
		transType = type;
	}

	return transType;
}

//------------------------------------------------------------------------------
/*!	\throw	no-throw */
basar::VarString changingType( basar::VarString type )
{
	basar::VarString changeType;

	type.trim();
	type.lower();
	
	if( type == "u" )
	{
		changeType = QApplication::translate( "functions.cpp", "Update" ).toLocal8Bit().data();
	}
	else if( type == "d" )
	{
		changeType = QApplication::translate( "functions.cpp", "Delete" ).toLocal8Bit().data();
	}
	else if( type == "r" )
	{
		changeType = QApplication::translate( "functions.cpp", "Restore" ).toLocal8Bit().data();
	}
	else
	{
		changeType = type;
	}

	return changeType;
}

//------------------------------------------------------------------------------
/*!	\throw	no-throw */
void splitUnity( basar::VarString unity, basar::VarString & rPack, basar::VarString & rExtension )
{
	unity.trim( basar::cmnutil::BString::FIRST_END );

	basar::UInt32 counter	= 0;
	size_t length			= unity.length();
	char sign;

	while( counter < length )
	{
		sign = unity.at( counter );
		
		if( isdigit(sign) || sign == ',' || sign == '.' || sign == 'x' || sign == 'X' || sign == ' ' ){
			rPack += sign;
		}
		else{
			break;
		}
		
		counter++;
	}

	rExtension = unity.substr( counter, length-counter );
}

//------------------------------------------------------------------------------
/*!	\throw	no-throw */
basar::I18nString removeLeadingZeros( basar::I18nString originalVal )
{
	originalVal.trim( basar::cmnutil::BString::ALL );
	basar::VarString::size_type firstNotZero = originalVal.find_first_not_of( '0' );
	
	if( firstNotZero == basar::VarString::npos ){
		return "";
	}
	
	originalVal=originalVal.substr( firstNotZero, originalVal.length() - firstNotZero );
	
	return originalVal;
}

//-------------------------------------------------------------------------------------------------//
/*!	\throw	no-throw */
QDate basarDateToQDate( const basar::Date& date )
{
    QDate result;

    if( date.isValid() )
    {
		basar::I18nString d = date.toString( constants::BASAR_DATE_FORMAT );
		result = QDate::fromString( QString::fromLocal8Bit(d.c_str()), constants::QT_DATE_FORMAT );
    }

    return ( result );
}

//-------------------------------------------------------------------------------------------------//

}	//namespace narcotics