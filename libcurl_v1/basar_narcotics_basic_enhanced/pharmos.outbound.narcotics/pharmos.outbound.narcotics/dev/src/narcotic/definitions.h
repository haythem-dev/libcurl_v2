//------------------------------------------------------------------------------
/*! \file
 *  \brief  common defines
 *  \author Thomas Hörath
 *  \date   17.05.2006
 */
//------------------------------------------------------------------------------
#ifndef GUARD_NARCOTICS_DEFINITIONS_H
#define GUARD_NARCOTICS_DEFINITIONS_H


//------------------------------------------------------------------------------
#define UADM_HOST						"HOST"		//!< parameter in useradministration to read server
#define UADM_DB							"DB"		//!< parameter in useradministration to read database
#define UADM_DOKU						"DOKU"		//!< parameter in useradministration to read documentation link
#define UADM_NEW_NARCOTIC_PROCESS		"NEW_NARCOTIC_PROCESS"
#define UADM_NARCOTICSPRINTER			"NARCOTICSPRINTER"
#define UADM_START_INTERNAL_BOOKINGS_NO	"START_INTERNAL_BOOKINGS_NO"
#define UADM_STARTDATE					"STARTDATE"
#define UADM_CONTRIBUTIONVOUCHERDIR		"CONTRIBUTIONVOUCHERDIR"
#define UADM_RELPRINTFOLDERPATH			"RELPRINTFOLDERPATH"
#define UADM_FTP_CITRIX_HOST			"FTP_CITRIX_SHARE_HOSTNAME"
#define UADM_FTP_CITRIX_USER			"FTP_CITRIX_SHARE_USER"
#define UADM_FTP_CITRIX_PW				"FTP_CITRIX_SHARE_PW"
#define UADM_FTP_LOCAL_TMP				"FTP_LOCAL_TMP"
#define UADM_BI_HTTP_ADDRESS			"BI_HTTP_ADDRESS"
#define UADM_REPORT_COMPANY_NAME		"REPORT_COMPANY_NAME"

#define UADM_MODULE_MAIN	1			//!< moduleid in useradministration where all  userrights are defined in
#define UADM_RIGHT_INCOME	"INCOME"	//!< name of userright in useradministration to work with incoming goods
#define UADM_RIGHT_GENERAL	"GENERAL"	//!< name of userright in useradministration to do everything except "protocol" and "incoming"
#define UADM_RIGHT_PROTOCOL	"PROTOCOL"	//!< name of userright in useradministration to work on protocol (only for masters!)

//------------------------------------------------------------------------------
namespace TransactionType
{
	basar::ConstString CUSTOMER_ORDER				= "a";	// Abverkauf 
	basar::ConstString GOODSIN_DELIVERY				= "b";	// Wareneingang 
	basar::ConstString CUSTOMER_RETURNS				= "c";	// Retoure
	basar::ConstString SUPPLIER_RETURNS				= "d";	// Abbuchung (Lieferant)
	basar::ConstString STOCK_DIFFERENCE_INCREASE	= "x";	// Bestandsdifferenz plus
	basar::ConstString STOCK_DIFFERENCE_DECREASE	= "y";	// Bestandsdifferenz minus 
	basar::ConstString DESTRUCTION					= "z";	// Vernichtung
}

//------------------------------------------------------------------------------

//! \brief collection of property tables for each narcotic article (key) containing bookings for printing
typedef std::map < basar::Int32, basar::db::aspect::AccessorPropertyTableRef >	CollOfNarcArtBookings;

//! \brief collection of property tables for each narcotic booking (catalogid = key) containing logs for printing
typedef std::map < basar::Int32, basar::db::aspect::AccessorPropertyTableRef >	CollOfBookingLogs;

#endif //GUARD_NARCOTICS_DEFINITIONS_H