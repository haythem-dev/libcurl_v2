//-------------------------------------------------------------------------------------------------//
/*! \file	cicsoldvoucherdm_definitions.h
 *  \brief  global definitions for CicsOldVoucherDM
 *  \author Patrick Samson
 *  \date   03.07.08
 */
//-------------------------------------------------------------------------------------------------//
#ifndef GUARD_CICSOLDVOUCHERDM_DEFINITIONS_H
#define GUARD_CICSOLDVOUCHERDM_DEFINITIONS_H

#include <libbasar_definitions.h>
#include <libbasarproperty_propertydescription.h>
#include <libbasarproperty_definitions.h>

#ifdef DIFFERENCE
	#undef DIFFERENCE
#endif

//----------------------------------------------------------------------------
namespace domMod
{
    namespace dm_cicsoldvoucher
    {
		//static basar::ConstString STR_FALSE									= "False";								//!< string representation of false
		//static basar::ConstString STR_TRUE									= "True";								//!< string representation of true

	}
}

namespace lit
{
/*! 
	\brief Namespace containing all necessary defintions for DM CicsOldVoucher<BR>
		(fieldnames used for matching / accessing properties + defined values)
*/
// length of property names must not exceed 18 characters (see definition of VSAM database table)
namespace dm_cicsoldvoucher
{
	CONST_PROPERTY_DESCRIPTION(ACQUISITION_DATE,						"acquisitiondate",		basar::INTDATE);
	CONST_PROPERTY_DESCRIPTION(ACQUISITION_USER_ID,						"acquisitionuserid",	basar::STRING);
    CONST_PROPERTY_DESCRIPTION(BASE_TYPE_CODE,							"basetypecode",			basar::STRING);
    CONST_PROPERTY_DESCRIPTION(BRANCH_NO,								"branchno",				basar::INT16);
	CONST_PROPERTY_DESCRIPTION(CASH_DISCOUNT_SAP_IN_PERCENT,			"cashdiscntsapperc",	basar::DECIMAL);
	CONST_PROPERTY_DESCRIPTION(CURRENCY_CODE_OLD,						"currencycodeold",		basar::STRING);
	CONST_PROPERTY_DESCRIPTION(DELIVERY_COSTS_FREIGHT,					"delcostsfreight",		basar::DECIMAL);
	CONST_PROPERTY_DESCRIPTION(DELIVERY_COSTS_EXPRESS,					"delcostsexpress",		basar::DECIMAL);
	CONST_PROPERTY_DESCRIPTION(DIFFERENCE,								"difference",			basar::DECIMAL);
	CONST_PROPERTY_DESCRIPTION(DISCOUNT_DAYS,							"discountdays",			basar::INT16);
	CONST_PROPERTY_DESCRIPTION(DISCOUNT_VALUE_FULL_TAX,					"discountvalfulltax",	basar::DECIMAL);
	CONST_PROPERTY_DESCRIPTION(DISCOUNT_VALUE_REDUCED_TAX1,				"discountvalredtax1",	basar::DECIMAL);
	CONST_PROPERTY_DESCRIPTION(DISCOUNT_VALUE_REDUCED_TAX2,				"discountvalredtax2",	basar::DECIMAL);
	CONST_PROPERTY_DESCRIPTION(DOCUMENT_CODE,							"documentcode",			basar::STRING);
	CONST_PROPERTY_DESCRIPTION(DOCUMENT_DATE,							"documentdate",			basar::INTDATE);
	CONST_PROPERTY_DESCRIPTION(DOCUMENT_FINAL_VALUE,					"documentfinalvalue",	basar::DECIMAL);
	CONST_PROPERTY_DESCRIPTION(FULL_TAX_VALUE,							"fulltaxvalue",			basar::DECIMAL);
	CONST_PROPERTY_DESCRIPTION(GOODS_IN_STATE,							"goodsinstate",			basar::STRING);
	CONST_PROPERTY_DESCRIPTION(GOODS_VALUE_CALCULATED,					"goodsvalcalculated",	basar::DECIMAL);
	CONST_PROPERTY_DESCRIPTION(GOODS_VALUE_FULL_TAX,					"goodsvaluefulltax",	basar::DECIMAL);
	CONST_PROPERTY_DESCRIPTION(GOODS_VALUE_NO_TAX,						"goodsvaluenotax",		basar::DECIMAL);
	CONST_PROPERTY_DESCRIPTION(GOODS_VALUE_REDUCED_TAX1,				"goodsvalueredtax1",	basar::DECIMAL);
	CONST_PROPERTY_DESCRIPTION(GOODS_VALUE_REDUCED_TAX2,				"goodsvalueredtax2",	basar::DECIMAL);
	CONST_PROPERTY_DESCRIPTION(MODIFICATION_DATE,						"modificationdate",		basar::INTDATE);
	CONST_PROPERTY_DESCRIPTION(MODIFICATION_USER_ID,					"modificationuserid",	basar::STRING);
	CONST_PROPERTY_DESCRIPTION(OLDEST_POSTING_DATE,						"oldestpostingdate",	basar::INTDATE);
	CONST_PROPERTY_DESCRIPTION(ORDER_TYPE,								"ordertype",			basar::STRING);
	CONST_PROPERTY_DESCRIPTION(ORDER_VALUE,								"ordervalue",			basar::DECIMAL);
	CONST_PROPERTY_DESCRIPTION(OTHER_REFUND_VALUE_FULL_TAX,				"otherrefundfulltax",	basar::DECIMAL);
	CONST_PROPERTY_DESCRIPTION(OTHER_REFUND_VALUE_REDUCED_TAX1,			"otherrefundredtax1",	basar::DECIMAL);
	CONST_PROPERTY_DESCRIPTION(OTHER_REFUND_VALUE_REDUCED_TAX2,			"otherrefundredtax2",	basar::DECIMAL);
	CONST_PROPERTY_DESCRIPTION(PACKAGE_REFUND_VALUE,					"packagerefundvalue",	basar::DECIMAL);
	CONST_PROPERTY_DESCRIPTION(PAYMENT_KEY,								"paymentkey",			basar::STRING);
	CONST_PROPERTY_DESCRIPTION(PAYMENT_LOCKED_CFLAG,					"paymentlockedcflag",	basar::STRING);
	CONST_PROPERTY_DESCRIPTION(POST_EDITING_DATE,						"posteditingdate",		basar::INTDATE);
	CONST_PROPERTY_DESCRIPTION(POST_EDITING_USER_ID,					"posteditinguserid",	basar::STRING);
	CONST_PROPERTY_DESCRIPTION(PURCHASE_ORDER_DATE,						"purchaseorderdate",	basar::INTDATE);
	CONST_PROPERTY_DESCRIPTION(PURCHASE_ORDER_NO_1,						"purchaseorderno1",		basar::STRING);
	CONST_PROPERTY_DESCRIPTION(PURCHASE_ORDER_NO_2,						"purchaseorderno2",		basar::INT32);
	CONST_PROPERTY_DESCRIPTION(PURCHASE_ORDER_NO_3,						"purchaseorderno3",		basar::INT32);
	CONST_PROPERTY_DESCRIPTION(PURCHASE_ORDER_NO_4,						"purchaseorderno4",		basar::INT32);
	CONST_PROPERTY_DESCRIPTION(PURCHASE_ORDER_NO_5,						"purchaseorderno5",		basar::INT32);
	CONST_PROPERTY_DESCRIPTION(PURCHASE_ORDER_NO_6,						"purchaseorderno6",		basar::INT32);
	CONST_PROPERTY_DESCRIPTION(PURCHASE_ORDER_NO_7,						"purchaseorderno7",		basar::INT32);
	CONST_PROPERTY_DESCRIPTION(PURCHASE_ORDER_NO_8,						"purchaseorderno8",		basar::INT32);
	CONST_PROPERTY_DESCRIPTION(PURCHASE_ORDER_NO_9,						"purchaseorderno9",		basar::INT32);
	CONST_PROPERTY_DESCRIPTION(PURCHASE_ORDER_NO_10,					"purchaseorderno10",	basar::INT32);
	CONST_PROPERTY_DESCRIPTION(PURCHASE_ORDER_NO_11,					"purchaseorderno11",	basar::INT32);
	CONST_PROPERTY_DESCRIPTION(PURCHASE_ORDER_NO_12,					"purchaseorderno12",	basar::INT32);
	CONST_PROPERTY_DESCRIPTION(PURCHASE_ORDER_NO_13,					"purchaseorderno13",	basar::INT32);
	CONST_PROPERTY_DESCRIPTION(PURCHASE_ORDER_NO_14,					"purchaseorderno14",	basar::INT32);
	CONST_PROPERTY_DESCRIPTION(PURCHASE_ORDER_NO_15,					"purchaseorderno15",	basar::INT32);
	CONST_PROPERTY_DESCRIPTION(PURCHASE_ORDER_NO_16,					"purchaseorderno16",	basar::INT32);
	CONST_PROPERTY_DESCRIPTION(PURCHASE_ORDER_TYPE,						"purchaseordertype",	basar::STRING);
	CONST_PROPERTY_DESCRIPTION(REBATE_IN_KIND_CURRENT_PRICE,			"rebatekindcurpric",	basar::DECIMAL);
	CONST_PROPERTY_DESCRIPTION(REBATE_IN_KIND_ORDER_PRICE,				"rebatekindordpric",	basar::DECIMAL);
	CONST_PROPERTY_DESCRIPTION(REDUCED_TAX1_VALUE,						"reducedtax1value",		basar::DECIMAL);
	CONST_PROPERTY_DESCRIPTION(REDUCED_TAX2_VALUE,						"reducedtax2value",		basar::DECIMAL);
	CONST_PROPERTY_DESCRIPTION(SAP_ACCOUNT_NO_OTHER_REFUND_FULL_TAX,	"sapacntnofulltax",		basar::INT32);
	CONST_PROPERTY_DESCRIPTION(SAP_ACCOUNT_NO_OTHER_REFUND_REDUCED_TAX1,"sapacntnoredtax1",		basar::INT32);
	CONST_PROPERTY_DESCRIPTION(SAP_ACCOUNT_NO_OTHER_REFUND_REDUCED_TAX2,"sapacntnoredtax2",		basar::INT32);
	CONST_PROPERTY_DESCRIPTION(SAP_SUPPLIER_NO,							"sapsupplierno",		basar::INT32);
	CONST_PROPERTY_DESCRIPTION(SUPPLIER_NO,								"supplierno",			basar::INT32);
	CONST_PROPERTY_DESCRIPTION(TAX_KEY_CODE,							"taxkeycode",			basar::STRING);
	CONST_PROPERTY_DESCRIPTION(TOTAL_DIFFERENCE_ALL_VOUCHERS,			"totdiffallvouchers",	basar::DECIMAL);
	CONST_PROPERTY_DESCRIPTION(TRANSFER_TO_SAP_DATE,					"transfertosapdate",	basar::INTDATE);
	CONST_PROPERTY_DESCRIPTION(USER_ID_CANCEL,							"useridcancel",			basar::STRING);
	CONST_PROPERTY_DESCRIPTION(USER_ID_PAYMENT_LOCK,					"useridpaymentlock",	basar::STRING);
	CONST_PROPERTY_DESCRIPTION(VALUE_DATE,								"valuedate",			basar::INTDATE);
	CONST_PROPERTY_DESCRIPTION(VALUE_GOODS_DELIVERED_CURRENT_PRICE,		"valgoodsdelcurpric",	basar::DECIMAL);
	CONST_PROPERTY_DESCRIPTION(VALUE_GOODS_DELIVERED_ORDER_PRICE,		"valgoodsdelordpric",	basar::DECIMAL);
	CONST_PROPERTY_DESCRIPTION(VOUCHER_CODE,							"vouchercode",			basar::STRING);
    CONST_PROPERTY_DESCRIPTION(VOUCHER_NO,								"voucherno",			basar::INT32);
    CONST_PROPERTY_DESCRIPTION(VOUCHER_NO_PREDECESSOR,					"vouchernopredecess",	basar::INT32);
    CONST_PROPERTY_DESCRIPTION(VOUCHER_NO_SUCCESSOR,					"vouchernosuccessor",	basar::INT32);
	CONST_PROPERTY_DESCRIPTION(VOUCHER_TEXT,							"vouchertext",			basar::STRING);
	CONST_PROPERTY_DESCRIPTION(VOUCHER_STATE,							"voucherstate",			basar::STRING);

} // end namespace dm_cicsoldvoucher
} // end namespace lit


//----------------------------------------------------------------------------
#endif //GUARD_CICSOLDVOUCHERDM_DEFINITIONS_H

