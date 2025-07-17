//----------------------------------------------------------------------------
/*! \file
 *  \brief  implemtation for all own transformer
 *  \author Roland Kiefert
 *  \date   21.03.2006
 */
//----------------------------------------------------------------------------

//-------------------------------------------------------------------------------------------------//
// includes
//-------------------------------------------------------------------------------------------------//
#include "commonheader.h"
#include "narcoticstransformer.h"

//-------------------------------------------------------------------------------------------------//
// namespaces
//-------------------------------------------------------------------------------------------------//
namespace narcotics {
namespace viewConn {

//----------------------------------------------------------------------------
/*! \throw  no-throw */
QuantityTransformer::~QuantityTransformer()
{
}

//----------------------------------------------------------------------------
//! \throw  no-throw
//!
QuantityTransformer::QuantityTransformer()
{
}

//----------------------------------------------------------------------------
//! \throw  no-throw
//!
void QuantityTransformer::RightToLeft(basar::gui::tie::GuiPropertyTable_YIterator				guiIt,
									  const basar::db::aspect::AccessorPropertyTable_YIterator	accIt)
{
	static basar::VarString name = QApplication::translate("QuantityTransformer","Quantity").toLocal8Bit().data();
	guiIt.setString(name,"0");
}

//----------------------------------------------------------------------------
//! \throw  no-throw
//!
void QuantityTransformer::LeftToRight(const basar::gui::tie::GuiPropertyTable_YIterator		guiIt,
									  basar::db::aspect::AccessorPropertyTable_YIterator	accIt)
{
}

//----------------------------------------------------------------------------
//! \throw  no-throw
//! \return pointer to created transformer
	boost::shared_ptr < basar::gui::tie::Transformer > QuantityTransformer::create()
{
	return boost::shared_ptr < basar::gui::tie::Transformer > (new QuantityTransformer(*this));
}

//----------------------------------------------------------------------------
/*! \throw  no-throw */
PrintedCustomerAdressTransformer::~PrintedCustomerAdressTransformer()
{
}

//----------------------------------------------------------------------------
//! \throw  no-throw
//!
PrintedCustomerAdressTransformer::PrintedCustomerAdressTransformer()
{
}

//----------------------------------------------------------------------------
//! \throw  no-throw
//!
void PrintedCustomerAdressTransformer::RightToLeft(basar::gui::tie::GuiPropertyTable_YIterator	guiIt,
									  const basar::db::aspect::AccessorPropertyTable_YIterator	accIt)
{
	basar::VarString adress;

	if (accIt.getInt16(constants::SWITCHADDRESSFLAG) == 0)
	{
		adress = accIt.getString(constants::STREET_EXTERN);
		adress += ", ";
		adress += accIt.getString(constants::CIP_EXTERN);
		adress += " ";
		adress += accIt.getString(constants::LOCATION_EXTERN);
	}
	else
	{
		adress = accIt.getString(constants::STREET_INTERN);
		adress += ", ";
		adress += accIt.getString(constants::CIP_INTERN);
		adress += " ";
		adress += accIt.getString(constants::LOCATION_INTERN);
	}

	guiIt.setString(QApplication::translate("CustomerMappingVC", "printed adress" ).toLocal8Bit().data(), adress);
}

//----------------------------------------------------------------------------
//! \throw  no-throw
//!
void PrintedCustomerAdressTransformer::LeftToRight(const basar::gui::tie::GuiPropertyTable_YIterator		guiIt,
									  basar::db::aspect::AccessorPropertyTable_YIterator	accIt)
{
	// not needed
}

//----------------------------------------------------------------------------
//! \throw  no-throw
//! \return pointer to created transformer
	boost::shared_ptr < basar::gui::tie::Transformer > PrintedCustomerAdressTransformer::create()
{
	return boost::shared_ptr < basar::gui::tie::Transformer > (new PrintedCustomerAdressTransformer(*this));
}

//----------------------------------------------------------------------------
/*! \throw  no-throw */
/*! \Variable Date Transformer for Table NarcTransactionCatalog */
VariableDateTransformer::~VariableDateTransformer()
{
}

//----------------------------------------------------------------------------
//! \throw  no-throw
//!
VariableDateTransformer::VariableDateTransformer(basar::I18nString propertyname, basar::I18nString fieldname)
{
	m_Propertyname = propertyname;
	m_Fieldname = fieldname;
}

//----------------------------------------------------------------------------
//! \throw  no-throw
//!
void VariableDateTransformer::RightToLeft(basar::gui::tie::GuiPropertyTable_YIterator				guiIt,
								  const basar::db::aspect::AccessorPropertyTable_YIterator	accIt)
{
    basar::DateTime date( accIt.getInt32(m_Propertyname), 0 );
    guiIt.setString( m_Fieldname, date.toStrDate().c_str() );
}

//----------------------------------------------------------------------------
//! \throw  no-throw
//!
void VariableDateTransformer::LeftToRight(const basar::gui::tie::GuiPropertyTable_YIterator		guiIt,
								  basar::db::aspect::AccessorPropertyTable_YIterator	accIt)
{
	basar::DateTime date;
	basar::VarString str = guiIt.getString(m_Fieldname);
	date.fromStrDate( str );
	accIt.setInt32(m_Propertyname, date.getDate());
}

//----------------------------------------------------------------------------
//! \throw  no-throw
//! \return pointer to created transformer
	boost::shared_ptr < basar::gui::tie::Transformer > VariableDateTransformer::create()
{
	return boost::shared_ptr < basar::gui::tie::Transformer > (new VariableDateTransformer(*this));
}

//----------------------------------------------------------------------------
/*! \throw  no-throw */
/*! \Time Transformer */
VariableTimeTransformer::~VariableTimeTransformer()
{
}

//----------------------------------------------------------------------------
//! \throw  no-throw
//!
VariableTimeTransformer::VariableTimeTransformer(basar::I18nString propertyname, basar::I18nString fieldname)
{
	m_Propertyname = propertyname;
	m_Fieldname = fieldname;
}

//----------------------------------------------------------------------------
//! \throw  no-throw
//!
void VariableTimeTransformer::RightToLeft(basar::gui::tie::GuiPropertyTable_YIterator				guiIt,
								  const basar::db::aspect::AccessorPropertyTable_YIterator	accIt)
{
	basar::DateTime date;
	date.setTime(accIt.getInt32(m_Propertyname)*1000); // milliseconds!
	guiIt.setString(m_Fieldname, date.toStrTimeShort().c_str());
}

//----------------------------------------------------------------------------
//! \throw  no-throw
//!
void VariableTimeTransformer::LeftToRight(const basar::gui::tie::GuiPropertyTable_YIterator		guiIt,
								  basar::db::aspect::AccessorPropertyTable_YIterator	accIt)
{
	basar::DateTime date;
	basar::VarString str = guiIt.getString(m_Fieldname);
	date.fromStrTimeShort(str);
	accIt.setInt32(m_Propertyname, date.getTime()/1000); // milliseconds!
}

//----------------------------------------------------------------------------
//! \throw  no-throw
//! \return pointer to created transformer
	boost::shared_ptr < basar::gui::tie::Transformer > VariableTimeTransformer::create()
{
	return boost::shared_ptr < basar::gui::tie::Transformer > (new VariableTimeTransformer(*this));
}

//--------------------------------------------------------------------------------------------------------//
// class SignTransformer
//--------------------------------------------------------------------------------------------------------//
//----------------------------------------------------------------------------
/*! \throw  no-throw */
SignTransformer::~SignTransformer()
{
}

//----------------------------------------------------------------------------
//! \throw  no-throw
//!
SignTransformer::SignTransformer()
{
}

//----------------------------------------------------------------------------
//! \throw  no-throw
//! \set prefix for quantity within the GUI
//!
void SignTransformer::RightToLeft(	basar::gui::tie::GuiPropertyTable_YIterator					guiIt,
									const basar::db::aspect::AccessorPropertyTable_YIterator	accIt)
{
	static basar::VarString nameValue
		= QApplication::translate( "SignTransformer","Quantity" ).toLocal8Bit().data();
	basar::VarString help;
	if( accIt.getString(constants::TRANSACTIONTYPE) == TransactionType::CUSTOMER_ORDER ||
		accIt.getString(constants::TRANSACTIONTYPE) == TransactionType::SUPPLIER_RETURNS ||
		accIt.getString(constants::TRANSACTIONTYPE) == TransactionType::DESTRUCTION ||
		accIt.getString(constants::TRANSACTIONTYPE) == TransactionType::STOCK_DIFFERENCE_DECREASE)
	{
		help.format("-%d",accIt.getInt32("qtytransaction"));
		guiIt.setString(nameValue,help);
	}

	else
	{
		help.format("+%d", accIt.getInt32("qtytransaction"));
		guiIt.setString(nameValue, help);
	}
}

//----------------------------------------------------------------------------
//! \throw  no-throw
//!
void SignTransformer::LeftToRight(const basar::gui::tie::GuiPropertyTable_YIterator		guiIt,
									  basar::db::aspect::AccessorPropertyTable_YIterator	accIt)
{
	static basar::VarString nameType = QApplication::translate("SignTransformer","Record Type").toLocal8Bit().data();
	static basar::VarString nameValue = QApplication::translate("SignTransformer","Quantity").toLocal8Bit().data();
	basar::Int32 help;
	if (guiIt.getString(nameType) == TransactionType::CUSTOMER_ORDER ||
		guiIt.getString(nameType) == TransactionType::DESTRUCTION ||
		guiIt.getString(nameType) == TransactionType::STOCK_DIFFERENCE_DECREASE)
	{
		help = atoi(guiIt.getString(nameValue).c_str()) * -1;
		accIt.setInt32("qtytransaction",help);
	}
}

//----------------------------------------------------------------------------
//! \throw  no-throw
//! \return pointer to created transformer
	boost::shared_ptr < basar::gui::tie::Transformer > SignTransformer::create()
{
	return boost::shared_ptr < basar::gui::tie::Transformer > (new SignTransformer(*this));
}

//--------------------------------------------------------------------------------------------------------//
// class ProtocolTransformer
//--------------------------------------------------------------------------------------------------------//
//----------------------------------------------------------------------------
/*! \throw  no-throw */
ProtocolTransformer::~ProtocolTransformer()
{
}

//----------------------------------------------------------------------------
//! \throw  no-throw
//!
ProtocolTransformer::ProtocolTransformer()
{
}

//----------------------------------------------------------------------------
//! \throw  no-throw
//!
void ProtocolTransformer::RightToLeft(	basar::gui::tie::GuiPropertyTable_YIterator					guiIt,
										const basar::db::aspect::AccessorPropertyTable_YIterator	accIt)
{
	basar::VarString transactiontype = accIt.getString( "activitytype" );
	basar::VarString attributeName;
	if( transactiontype == "U" ){
		// attention:	here it has to be used the string of the attribute as it is named in the dialog
		//				respectively the column of the tablewidget.
		// "Activity",
		guiIt.setString(	QApplication::translate( "ProtocolTransformer", "Activity" ).toLocal8Bit().data(),
							QApplication::translate( "ProtocolTransformer", "Update" ).toLocal8Bit().data() );

		basar::VarString changedattribute = accIt.getString( "changedattribute" );
		if( changedattribute == constants::TRANSACTIONDATE){
			attributeName = QApplication::translate( "ProtocolTransformer", "Date" ).toLocal8Bit().data();
		} else if( changedattribute == constants::TRANSACTIONTIME){
			attributeName = QApplication::translate( "ProtocolTransformer", "Time" ).toLocal8Bit().data();
		} else if( changedattribute == "customersupplierno"){
			attributeName = QApplication::translate( "ProtocolTransformer", "BgaNo" ).toLocal8Bit().data();
		} else if( changedattribute == constants::TRANSACTIONTYPE){
			attributeName = QApplication::translate( "ProtocolTransformer", "RT" ).toLocal8Bit().data();
		} else if( changedattribute == constants::PURCHASEORDERNO){
			attributeName = QApplication::translate( "ProtocolTransformer", "OrderNo" ).toLocal8Bit().data();
		} else if( changedattribute == "qtytransaction"){
			attributeName = QApplication::translate( "ProtocolTransformer", "Quantity" ).toLocal8Bit().data();
		} else if( changedattribute == constants::PROCESSEDFLAG){
			attributeName = QApplication::translate( "ProtocolTransformer", "Processed" ).toLocal8Bit().data();
		} else if( changedattribute == "customersuppliername"){
			attributeName = QApplication::translate( "ProtocolTransformer", "Name" ).toLocal8Bit().data();
		} else if( changedattribute == "customersupplierlocation"){
			attributeName = QApplication::translate( "ProtocolTransformer", "Location" ).toLocal8Bit().data();
		} else if( changedattribute == "invoiceno"){
			attributeName = QApplication::translate( "ProtocolTransformer", "InvoiceNo" ).toLocal8Bit().data();
		}

		// attention:	here it has to be used the string of the attribute as it is named in the dialog
		//				respectively the column of the tablewidget.
		//	"Attribute",
		guiIt.setString(	QApplication::translate( "ProtocolTransformer", "Attribute" ).toLocal8Bit().data(),
							QApplication::translate( "ProtocolTransformer", attributeName.c_str() ).toLocal8Bit().data() );

	} else if( transactiontype == "D" ){
		guiIt.setString(	QApplication::translate( "ProtocolTransformer", "Activity" ).toLocal8Bit().data(),
							QApplication::translate( "ProtocolTransformer", "Delete" ).toLocal8Bit().data() );
	} else if( transactiontype == "R" ){
		guiIt.setString(	QApplication::translate( "ProtocolTransformer", "Activity" ).toLocal8Bit().data(),
							QApplication::translate( "ProtocolTransformer", "Restore" ).toLocal8Bit().data() );
	}
}

//----------------------------------------------------------------------------
//! \throw  no-throw
//!
void ProtocolTransformer::LeftToRight( const basar::gui::tie::GuiPropertyTable_YIterator	guiIt,
									   basar::db::aspect::AccessorPropertyTable_YIterator	accIt )
{
	// it is not necessary to match back to the AccessorPropertyTable
}

//----------------------------------------------------------------------------
//! \throw  no-throw
//!
void ProtocolTransformer::LeftToRight( const basar::VarString attributeName, basar::db::aspect::AccessorPropertyTable_YIterator accIt)
{
	basar::VarString columnName;

	if (attributeName == QApplication::translate( "ProtocolTransformer", "Date" ).toLocal8Bit().data())
		columnName = constants::TRANSACTIONDATE;
	else if (attributeName == QApplication::translate( "ProtocolTransformer", "Time" ).toLocal8Bit().data())
		columnName = constants::TRANSACTIONTIME;
	else if (attributeName == QApplication::translate( "ProtocolTransformer", "BgaNo" ).toLocal8Bit().data())
		columnName = "customersupplierno";
	else if (attributeName == QApplication::translate( "ProtocolTransformer", "RT" ).toLocal8Bit().data())
		columnName = constants::TRANSACTIONTYPE;
	else if (attributeName == QApplication::translate( "ProtocolTransformer", "OrderNo" ).toLocal8Bit().data())
		columnName = constants::PURCHASEORDERNO;
	else if (attributeName == QApplication::translate( "ProtocolTransformer", "Quantity" ).toLocal8Bit().data())
		columnName = "qtytransaction";
	else if (attributeName == QApplication::translate( "ProtocolTransformer", "Processed" ).toLocal8Bit().data())
		columnName = constants::PROCESSEDFLAG;
	else if (attributeName == QApplication::translate( "ProtocolTransformer", "Name" ).toLocal8Bit().data())
		columnName = "customersuppliername";
	else if (attributeName == QApplication::translate( "ProtocolTransformer", "Location" ).toLocal8Bit().data())
		columnName = "customersupplierlocation";
	else if (attributeName == QApplication::translate( "ProtocolTransformer", "InvoiceNo" ).toLocal8Bit().data())
		columnName = "invoiceno";
	else
		columnName = attributeName;


	accIt.setString( constants::CHANGEDATTRIBUTE, columnName);
}

//----------------------------------------------------------------------------
//! \throw  no-throw
//! \return pointer to created transformer
	boost::shared_ptr < basar::gui::tie::Transformer > ProtocolTransformer::create()
{
	return boost::shared_ptr < basar::gui::tie::Transformer > (new ProtocolTransformer(*this));
}

} //namespace viewConn
} //namespace narcotics