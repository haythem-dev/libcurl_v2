//----------------------------------------------------------------------------------------//
/*! \file
 *  \brief  dedicated namespace for constants used in different sub modules 
			as view connectors, use cases and domain modules
 *  \author Bjoern Bischof
 *  \date   03.11.2006
 */
//----------------------------------------------------------------------------------------//
#ifndef GUARD_CONSTANTS_H
#define GUARD_CONSTANTS_H

//----------------------------------------------------------------------------------------//
// namespace
//----------------------------------------------------------------------------------------//
namespace constants
{
	// booking catalog
	basar::ConstString COMPANYNAME					= "name";				//! filiale.name
	basar::ConstString STREET						= "street";
	basar::ConstString POSTCODE						= "postcode";
	basar::ConstString LOCATION						= "location";			//! filiale.ort
	basar::ConstString LOCATION_LONG				= "location_long";		//! filiale.standort_lang
	basar::ConstString BRANCHNO						= "branchno";
	basar::ConstString CATALOGID					= "catalogid";	
	basar::ConstString ARTICLENO					= "articleno";
	basar::ConstString ARTICLECODE					= "articlecode";
	basar::ConstString PURCHASEORDERNO				= "purchaseorderno";
	basar::ConstString FROMDATE						= "fromdate";
	basar::ConstString TODATE						= "todate";
	basar::ConstString FROMTIME						= "fromtime";
	basar::ConstString TOTIME						= "totime";
	basar::ConstString TRANSACTIONTYPE				= "transactiontype";
	basar::ConstString NEWTRANSACTIONTYPE			= "newtransactiontype";
	basar::ConstString TRANSACTIONTIME				= "transactiontime";
	basar::ConstString TRANSACTIONDATE				= "transactiondate";
    basar::ConstString ORIGINALTRANSACTIONDATE      = "originalTransactionDate";
    basar::ConstString ORIGINALTRANSACTIONTIME      = "originalTransactionTime";
	basar::ConstString NEWTRANSACTIONDATE			= "newTransactionDate";
    basar::ConstString NEWTRANSACTIONTIME			= "newTransactionTime";
	basar::ConstString DELETEDFLAG					= "deletedflag";
	basar::ConstString PROCESSEDFLAG				= "processedflag";
	basar::ConstString INTERNAL_ACCOUNTS			= "internal_accounts";
	basar::ConstString CUSTOMERSUPPLIERNO			= "customersupplierno";
	basar::ConstString SUPPLIERNO                   = "supplierno";
	basar::ConstString CUSTOMER_SUPPLIER_NAME		= "customersuppliername";
    basar::ConstString CUSTOMER_SUPPLIER_STREET		= "customersupplierstreet";
	basar::ConstString CUSTOMER_SUPPLIER_CIP		= "customersuppliercip";
	basar::ConstString CUSTOMER_SUPPLIER_LOCATION	= "customersupplierlocation";
	basar::ConstString QTYTRANSACTION				= "qtytransaction";
	basar::ConstString ORIGINALQTYTRANSACTION		= "originalqtytransaction";
	basar::ConstString QTYINSTOCK					= "qtyinstock";
	basar::ConstString ISPROCESSED					= "1";
	basar::ConstString ISNOTPROCESSED				= "0";
	basar::ConstString ISDELETED					= "1";
	basar::ConstString ISNOTDELETED					= "0";
	basar::ConstString INITIAL_INVENTORY_TAG		= "u";
	basar::ConstString BGANO						= "bgano";
	basar::ConstString CUSTOMERNO					= "customerno";
	basar::ConstString CUSTOMERNAME					= "customername";
	basar::ConstString CUSTOMERLOCATION				= "customerlocation";
	// booking catalog -> insert narcotic specific data into narctransactioncatalog
	basar::ConstString CONCENTRATION				= "concentration";
	basar::ConstString NARCGROUP					= "narcgroup";
	basar::ConstString NARCNAME						= "narcname";
	basar::ConstString NARCUNIT						= "narcunit";
	basar::ConstString NARCPOTENCY					= "narcpotency";
	basar::ConstString NARCFACTOR					= "narcfactor";
	// set constants for private member m_ChangingState of bookingcatalog
	basar::ConstString CREATE						= "create";
	basar::ConstString EDIT							= "edit";
	basar::ConstString ONLY_QTY_CHANGED				= "onlyqtychanged";
	
	basar::ConstString SELECTION					= "selection";
	basar::ConstString SELECTION_ON					= "1";
	basar::ConstString SELECTION_OFF				= "0";

	// narcotic category
	basar::ConstString GROUP						= "group";
	basar::ConstString NARCOTICNAME					= "name";

	// order overview
	basar::ConstString PHARMAFORM					= "pharmaform";
	basar::ConstString PACKAGEUNIT					= "packageunit";
	basar::ConstString ARTICLENAME					= "articlename";
	basar::ConstString SHOW_LOCKED_CUST				= "showAlsoLockedCust";

	// customer mapping
	basar::ConstString CUSTOMERNO_EXTERN			= "customernoextern";
	basar::ConstString NAME_EXTERN					= "name_extern";
	basar::ConstString STREET_EXTERN				= "street_extern";
	basar::ConstString CIP_EXTERN					= "cip_extern";
	basar::ConstString LOCATION_EXTERN				= "location_extern";
	basar::ConstString CUSTOMERNO_INTERN			= "customernointern";
	basar::ConstString NAME_INTERN					= "name_intern";
	basar::ConstString STREET_INTERN				= "street_intern";
	basar::ConstString CIP_INTERN					= "cip_intern";
	basar::ConstString LOCATION_INTERN				= "location_intern";
	basar::ConstString PRINTED_ADRESS				= "printed_adress";
	
	basar::ConstString SWITCHADDRESSFLAG			= "switchaddressflag";

	// booking catalog (sub dlg)
	basar::ConstString NARCOTICSSUPPLIERNO			= "narcoticssupplierno";
	basar::ConstString SUPPLIERNAME					= "suppliername";
	basar::ConstString SUPPLIERLOCATION				= "supplierlocation"; 
	basar::ConstString SUPPLIERCIP					= "suppliercip";
	const basar::Int16 MAXSIZEOLDDELIVERYRECEIPT	= 3;

	// login
	basar::ConstString USERNAME						= "username";
	basar::ConstString CHANGEDATTRIBUTE				= "changedattribute";

	basar::ConstString PROTOCOLDATE					= "protocoldate";
	basar::ConstString PROTOCOLTIME					= "protocoltime";
	basar::ConstString ACTIVITYTYPE					= "activitytype";
	basar::ConstString VALUEOLD						= "valueold";
	basar::ConstString VALUENEW						= "valuenew";

	// contribution voucher
	basar::ConstString CONTRIBUTIONVOUCHERNO		= "contributionvoucherno";
	basar::ConstString ACKNOWLEDGEMENTSTATUS		= "acknowledgementstatus";
    basar::ConstString PRINTED_DATE                 = "printeddate";
    basar::ConstString PRINTED_TIME                 = "printedtime";
	basar::ConstString FORMER_ACKNOWLEDGEMENTSTATUS = "formeracknowledgementstatus";
	basar::ConstString SIGNATURELNK					= "signaturelnk";
	basar::ConstString TELNO						= "telno";
	basar::ConstString QUICKDIALING					= "quickdialing";
	basar::ConstString FAXNO						= "faxno";
	basar::ConstString RECTIFICATIONREASON			= "rectificationreason";
	basar::ConstString RECTIFICATIONTEXT			= "rectificationtext";		// text is defined by the system
	basar::ConstString RECTIFICATIONCOMMENT			= "rectificationcomment";	// additional user comments

	basar::ConstString CONTRIBUTIONVOUCHERBASEFILENAME	= "Abgabemeldung.ps";
	basar::ConstString TOURID						= "tourid";
    basar::ConstString ORDERBYTOUR					= "orderbytour";
    basar::ConstString SCANNED_CONTRIBUTION_VOUCHER_NO = "ScannedContributionVoucherNo";

	// print
	basar::ConstString PRINTFOLDERNAME				= "print";
	basar::ConstString PRINTLOCK					= "PrintLock";
	basar::ConstString PRINTLOCK_TABLE_NAME			= "narclock";
	const basar::Int16 PRINTTHREADS_PER_BRANCH		= 1;
	const basar::Int16 PRINTLOCK_BUSINESSOBJECT_ID	= 42;

	// enum to declare different varieties for acknowledgementstatus 
	enum AcknowledgementStatus
	{
		INVALID                     = -1,
        INSERTED					= 0,
		FOR_PRINT					= 1,
		PRINTED						= 2,
		ACKNOWLEDGEMENT_RECEIVED	= 3,
		CHANGED_ACKNOWLEDGEMENT		= 4,
		DELIVERY_NOTE_COPY_CREATED	= 5,
		IN_PRINT					= 6,
        NOT_PRINTABLE               = 7
	};

    //defines tasks done by Resolve printing errors dialog
    enum ResolvePrintingErrorsTasks
    {
        NONE                            = 0,
        PRINTED_CONTRIBUTION_VOUCHER    = 1,
        PRINTED_DELIVERY_NOTE           = 2,
        PRINTED_BOTH_DOCUMENTS          = 3
    };

    //defines possible results from scanning a contribution voucher
    enum ContributionVoucherScanResults
    {
        INVALID_SCAN_RESULT     = -1,
        MARKED_AS_RECEIVED      = 0,
        VOUCHER_ALREADY_MARKED  = 1,
        VOUCHER_NOT_FOUND_IN_DB = 2
    };

	enum DeliveryNoteCopyReason
	{
		THEFT_LOSS				= 0,
		REFUSAL_OF_ACCEPTANCE	= 1,
		QUANTITY_CORRECTION		= 2
	};

	basar::ConstString ABGABE  = "Abgabemeldung";
	basar::ConstString EMPFANG = "Empfangsbestätigung";
	basar::ConstString LIEFER  = "Lieferscheindoppel";

	const basar::Int16 BOOKINGS_PER_FORM = 6;		//!< constant for maximum number of printable lines per form
	const basar::Int16 SERIAL_LENGTH_VOUCHER = 4;	//!< constant for length of serial within name of contribution vouchers
	const basar::Int16 SERIAL_LENGTH_FOLDER = 3;	//!< constant for length of serial week within name of contribution voucher folders

	basar::ConstString TRANSFERLOCK_TABLE_NAME			= "narclock";
	basar::ConstString TRANSFER_BUSINESS_TYPE			= "transfer_booking";
	const basar::Int32 TRANSFER_MAX_LOCKS_PER_BOOKING	= 1;

	//date formats
	static const basar::ConstString QT_DATE_FORMAT        = "yyyy-MM-dd"; 
	static const basar::ConstString BASAR_DATE_FORMAT     = "yyyy-mm-dd";

	//defines maximum search date one year in the future
	const basar::Date  MAXIMUM_SEARCH_DATE( basar::Date().getCurrent().getYear() + 1,
											basar::Date().getCurrent().getMonth(),
											basar::Date().getCurrent().getDay()		);
	const basar::Date  MINIMUM_SEARCH_DATE( 2000, 1, 1);
}

#endif //GUARD_CONSTANTS_H