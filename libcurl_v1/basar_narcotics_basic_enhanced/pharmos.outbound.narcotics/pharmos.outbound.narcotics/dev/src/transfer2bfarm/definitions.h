#ifndef DEFINITIONS_GUARD
#define DEFINITIONS_GUARD

//------------------------------------------------------------------------------
//namespaces
//------------------------------------------------------------------------------

namespace narcotics {
namespace constants {

//------------------------------------------------------------------------------

	basar::ConstString UADM_DB1_HOST				= "DB1_HOST";
	basar::ConstString UADM_DB1_DB					= "DB1_DB";

	basar::ConstString UADM_CONTRIBUTIONVOUCHERDIR	= "CONTRIBUTIONVOUCHERDIR";
	basar::ConstString UADM_SAVEDIR					= "SAVEDIR";
	basar::ConstString UADM_VOUCHERSTOOMUCHDIR		= "VOUCHERSTOOMUCHDIR";
	basar::ConstString UADM_ARCHIVINGDIR			= "ARCHIVINGDIR";
	basar::ConstString UADM_METADATAFILENAME		= "METADATAFILENAME";
	basar::ConstString UADM_KOFAXPATH				= "KOFAXPATH";
	basar::ConstString UADM_BFARM_IP				= "BFARM_IP";
	basar::ConstString UADM_BFARM_USER				= "BFARM_USER";

	basar::ConstString UADM_STARTDATE				= "STARTDATE";

	
	CONST_PROPERTY_DESCRIPTION( BRANCHNO, "filialnr", basar::INT16 )	
	CONST_PROPERTY_DESCRIPTION( PURCHASEORDERNO, "purchaseorderno", basar::INT32 )	
	CONST_PROPERTY_DESCRIPTION( SUBSCRIBER, "username", basar::STRING )
	CONST_PROPERTY_DESCRIPTION( PRINTEDDATE, "printeddate", basar::INT32 )

	CONST_PROPERTY_DESCRIPTION( CUSTOMERSUPPLIERNO, "customersupplierno", basar::INT32 )

	CONST_PROPERTY_DESCRIPTION( BGANO, "bganr_ek", basar::INT32 )
	CONST_PROPERTY_DESCRIPTION( VOUCHERNO, "contributionvoucherno", basar::INT32 )
	CONST_PROPERTY_DESCRIPTION( PROCESSSTARTDATE, "startdate", basar::INT32 )
	CONST_PROPERTY_DESCRIPTION( PROCESSENDDATE, "enddate", basar::INT32 )
	CONST_PROPERTY_DESCRIPTION( TRANSACTIONDATE, "transactiondate", basar::INT32 )


	CONST_PROPERTY_DESCRIPTION( STARTDATE, "startdate", basar::STRING )	
	CONST_PROPERTY_DESCRIPTION( BRANCHNOPARAMETER, "filialnr", basar::STRING )
	
}

//------------------------------------------------------------------------------

namespace definitions
{
	basar::ConstString VOUCHERFILEREGEXP = "[0-9]{12}.pdf";
	basar::ConstString DELIVERYNOTECOPYREGEXP = "LD[0-9]{12}.pdf";
	basar::ConstString ALLVOUCHERS = "(LD){0,1}[0-9]{12}.pdf";

	basar::ConstString BGAFOLDERDELIVERYNOTECOPYREGEXP = "LD[0-9]{10}";
	basar::ConstString BGAFOLDERALLENTRIES = "(LD){0,1}[0-9]{10}";
	basar::ConstString BGAFOLDERREGEXP =  "[0-9]{10}";
	basar::ConstString ARCHIVEFOLDERREGEXP = "vz_[0-9]{2}";
	
	//cold archiving
	basar::ConstString BRANCHNO = "branchno";
	basar::ConstString DOCUMENTTYPE = "type";
	basar::ConstString CONTRIBUTIONVOUCHER = "AB";
	basar::ConstString DELIVERYNOTECOPY = "LD";
	basar::ConstString FILETYPE = "filetype";
	basar::ConstString FILENAME = "filename";
    basar::ConstString CONTRIBUTIONVOUCHERNO = "contributionvoucherno";
	basar::ConstString PRINTEDDATE = "printeddate";
	basar::ConstString TRANSACTIONDATE = "transactiondate";
	basar::ConstString PURCHASEORDERNO = "purchaseorderno";
	basar::ConstString CUSTOMERSUPPLIERNO = "customersupplierno";
	basar::ConstString SUBSCRIBER = "subscriber";


//------------------------------------------------------------------------------

} //namespace constants
} //namespace narcotics

//------------------------------------------------------------------------------

#endif //DEFINITIONS_GUARD

//------------------------------------------------------------------------------
