//----------------------------------------------------------------------------
/*! \file
 *  \brief  global definitions for narctransfer
 *  \author
 *  \date
 */
//----------------------------------------------------------------------------

#ifndef GUARD_NARCTRANSFER_DEFINITIONS_H
#define GUARD_NARCTRANSFER_DEFINITIONS_H

//----------------------------------------------------------------------------

namespace uadm {
// add global definitions here (enums, typedefs, ...)
	basar::ConstString UADM_HOST						= "HOST";	//!< parameter in useradministration to read server
	basar::ConstString UADM_DB							= "DB";		//!< parameter in useradministration to read database
	basar::ConstString UADM_NEW_NARCOTIC_PROCESS		= "NEW_NARCOTIC_PROCESS"; //!< parameter in useradministration to use electronic contribution voucher
	basar::ConstString UADM_MARK_GI_ITEM_AS_PROCESSED	= "MARK_GI_ITEM_AS_PROCESSED";
	basar::ConstString UADM_RELEVANT_COMPLETED_STATE	= "RELEVANT_COMPLETED_STATE";
} //namespace uadm

//----------------------------------------------------------------------------

namespace lit {
namespace narcotics {

	CONST_PROPERTY_DESCRIPTION(BRANCHNO,				"branchno",					basar::INT16)
	CONST_PROPERTY_DESCRIPTION(ARTICLENO,				"articleno",				basar::INT32)
	CONST_PROPERTY_DESCRIPTION(TRANSFERID,				"transferid",				basar::INT32)
	CONST_PROPERTY_DESCRIPTION(CATALOGID,				"catalogid",				basar::INT32)
	CONST_PROPERTY_DESCRIPTION(CONTRIBUTIONVOUCHERNO,	"contributionvoucherno",	basar::INT32)
	CONST_PROPERTY_DESCRIPTION(CUSTOMERSUPPLIERNO,		"customersupplierno",		basar::INT32)
	CONST_PROPERTY_DESCRIPTION(CUSTOMERSUPPLIERNAME,	"customersuppliername",		basar::STRING)
    CONST_PROPERTY_DESCRIPTION(CUSTOMERSUPPLIERSTREET,	"customersupplierstreet",	basar::STRING)
	CONST_PROPERTY_DESCRIPTION(CUSTOMERSUPPLIERCITY,	"customersuppliercity",		basar::STRING)
	CONST_PROPERTY_DESCRIPTION(CUSTOMERSUPPLIERCIP,		"customersuppliercip",		basar::STRING)
	CONST_PROPERTY_DESCRIPTION(DOCUMENTNO,				"documentno",				basar::INT32)
	CONST_PROPERTY_DESCRIPTION(DOCUMENTTYPE,			"documenttype",				basar::INT16)
	CONST_PROPERTY_DESCRIPTION(PURCHASEORDERNO,			"purchaseorderno",			basar::INT32)
	CONST_PROPERTY_DESCRIPTION(ORDERNO,					"orderno",					basar::INT32)
	CONST_PROPERTY_DESCRIPTION(DELETEDFLAG,				"deletedflag",				basar::STRING)
	CONST_PROPERTY_DESCRIPTION(PROCESSEDFLAG,			"processedflag",			basar::STRING)
	CONST_PROPERTY_DESCRIPTION(QTYINSTOCK,				"qtyinstock",				basar::INT32)
	CONST_PROPERTY_DESCRIPTION(QTYTRANSACTION,			"qtytransaction",			basar::INT32)
	CONST_PROPERTY_DESCRIPTION(TRANSACTIONDATE,			"transactiondate",			basar::INT32)
	CONST_PROPERTY_DESCRIPTION(TRANSACTIONTIME,			"transactiontime",			basar::INT32)
	CONST_PROPERTY_DESCRIPTION(TRANSACTIONTYPE,			"transactiontype",			basar::STRING)
	CONST_PROPERTY_DESCRIPTION(ACKNOWLEDGEMENTSTATUS,	"acknowledgementstatus",	basar::INT16)
	CONST_PROPERTY_DESCRIPTION(TOURID,					"tourid",					basar::STRING)
	CONST_PROPERTY_DESCRIPTION(WAREHOUSEORDERNO,		"warehouseorderno",			basar::INT32)

	CONST_PROPERTY_DESCRIPTION(NARCGROUP,				"narcgroup",				basar::STRING)
	CONST_PROPERTY_DESCRIPTION(NARCNAME,				"narcname",					basar::STRING)
	CONST_PROPERTY_DESCRIPTION(CONCENTRATION,			"concentration",			basar::DECIMAL)
	CONST_PROPERTY_DESCRIPTION(PACKAGEUNIT,				"packageunit",				basar::STRING)
	CONST_PROPERTY_DESCRIPTION(NARCFACTOR,				"narcfactor",				basar::DECIMAL)

	CONST_PROPERTY_DESCRIPTION(CUSTOMERNO_EXTERN,		"customernoextern",			basar::INT32)
	CONST_PROPERTY_DESCRIPTION(SWITCHADDRESSFLAG,		"switchaddressflag",		basar::INT16)
	CONST_PROPERTY_DESCRIPTION(COMPLETEDFLAG,			"completedflag",			basar::INT16)

	basar::ConstString SELLOFF = "a";
} //namespace narcotics
} //namespace lit

//----------------------------------------------------------------------------
namespace narcotics {

//---------------------------------------------------------------------------------------------------------//
// enum to declare different varieties for acknowledgementstatus
//---------------------------------------------------------------------------------------------------------//
enum AcknowledgementStatus
{
	AS_INSERTED					= 0,
	AS_FOR_PRINT				= 1,
	AS_PRINTED					= 2,
	AS_ACKNOWLEDGEMENT_RECEIVED	= 3,
	AS_CHANGED_ACKNOWLEDGEMENT	= 4,
	AS_DELIVERY_NOTE_CREATED	= 5
};

//---------------------------------------------------------------------------------------------------------//
// enum ProcessStateEnum
//---------------------------------------------------------------------------------------------------------//
enum ProcessStateEnum
{
	PS_UNINITIALZED = -1,
	PS_ERROR  = 0,
	PS_TO_INSERT,
	PS_TO_UPDATE
};

//---------------------------------------------------------------------------------------------------------//
// enum DocumentType
//---------------------------------------------------------------------------------------------------------//
enum DocumentType
{
	DT_EMPFANGSBESTAETIGUNG	= 1,
	DT_LIEFERSCHEINDOPPEL	= 2
};

//---------------------------------------------------------------------------------------------------------//
// constants for new locking mechanism
//---------------------------------------------------------------------------------------------------------//
basar::ConstString TRANSFERLOCK_TABLE_NAME			= "narclock";
basar::ConstString TRANSFER_BUSINESS_TYPE			= "transfer_booking";
const basar::Int32 TRANSFER_MAX_LOCKS_PER_BOOKING	= 1;

//----------------------------------------------------------------------------

} //namespace narcotics

//----------------------------------------------------------------------------

#endif	//GUARD_NARCTRANSFER_DEFINITIONS_H