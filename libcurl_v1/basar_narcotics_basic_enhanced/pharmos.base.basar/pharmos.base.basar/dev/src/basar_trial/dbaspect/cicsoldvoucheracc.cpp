//----------------------------------------------------------------------------
/*! \file	cicsoldvoucheracc.cpp
 *  \brief  string builder definition for CicsOldVoucher-accessor + registration of CicsOldVoucher-accessor
 *  \author Samson, Patrick
 *  \date   25.07.08
 */
//----------------------------------------------------------------------------

// following include directive must be the first include !
// this cpp file must be compiled for all(!) configurations with settings:
//	   /Yu"cmnaccessor.h" /Fp$(IntDir)/cmnaccessor.pch
#include "cmnaccessor.h"

#include "cicsoldvoucheracc.h"
#include "cicsoldvoucherdm_definitions.h"

using namespace lit::dm_cicsoldvoucher;

//----------------------------------------------------------------------------
namespace acc_cicsoldvoucher
{

//-------------------------------------------------------------------------------------------//
// SELECTs
//-------------------------------------------------------------------------------------------//

BUILDER_DEFINITION(SelectAll)
	DONT_CHECK_PROPERTIES( SelectAll )
	//! build sql statement
	void SelectAll::buildSQLString()
	{
		basar::I18nString sqlStatement;
		sqlStatement.append("SELECT ");
		sqlStatement.append(	"RPR_FIL2 AS " +				BRANCH_NO									.getName() + ", ");
		sqlStatement.append(	"RPR_HNR AS " +					SUPPLIER_NO									.getName() + ", ");
		sqlStatement.append(	"RPR_BLNR2 AS " +				VOUCHER_NO									.getName() + ", ");
		sqlStatement.append(	"RPR_SAPHNR AS " +				SAP_SUPPLIER_NO								.getName() + ", ");
		sqlStatement.append(	"RPR_KZRG AS " +				BASE_TYPE_CODE								.getName() + ", ");
		sqlStatement.append(	"RPR_BNR AS " + 				PURCHASE_ORDER_NO_1							.getName() + ", ");
		sqlStatement.append(	"RPR_BESDAT AS " + 				PURCHASE_ORDER_DATE							.getName() + ", ");
		sqlStatement.append(	"RPR_RENR || RPR_RENR2 AS " +	DOCUMENT_CODE								.getName() + ", ");
		sqlStatement.append(	"RPR_REDAT AS " + 				DOCUMENT_DATE								.getName() + ", ");
		sqlStatement.append(	"RPR_STBID AS " + 				USER_ID_CANCEL								.getName() + ", ");
		sqlStatement.append(	"RPR_VADAT AS " + 				VALUE_DATE									.getName() + ", ");
		sqlStatement.append(	"RPR_RBETR AS " + 				DOCUMENT_FINAL_VALUE						.getName() + ", ");
		sqlStatement.append(	"RPR_RWAWE AS " + 				GOODS_VALUE_CALCULATED						.getName() + ", ");
		sqlStatement.append(	"RPR_RDIFF AS " + 				DIFFERENCE									.getName() + ", ");
		sqlStatement.append(	"RPR_WAWE1 AS " + 				GOODS_VALUE_FULL_TAX						.getName() + ", ");
		sqlStatement.append(	"RPR_BZFRA AS " + 				DELIVERY_COSTS_FREIGHT						.getName() + ", ");
		sqlStatement.append(	"RPR_WAWE2 AS " + 				GOODS_VALUE_REDUCED_TAX1					.getName() + ", ");
		sqlStatement.append(	"RPR_BZEXP AS " + 				DELIVERY_COSTS_EXPRESS						.getName() + ", ");
		sqlStatement.append(	"RPR_WAWEO AS " + 				GOODS_VALUE_NO_TAX							.getName() + ", ");
		sqlStatement.append(	"RPR_MWSW1 AS " + 				FULL_TAX_VALUE								.getName() + ", ");
		sqlStatement.append(	"RPR_MWSW2 AS " + 				REDUCED_TAX1_VALUE							.getName() + ", ");
		sqlStatement.append(	"RPR_RABA1 AS " + 				DISCOUNT_VALUE_FULL_TAX						.getName() + ", ");
		sqlStatement.append(	"RPR_RABA2 AS " + 				DISCOUNT_VALUE_REDUCED_TAX1					.getName() + ", ");
		sqlStatement.append(	"RPR_VVERP AS " + 				PACKAGE_REFUND_VALUE						.getName() + ", ");
		sqlStatement.append(	"RPR_VSON1 AS " + 				OTHER_REFUND_VALUE_FULL_TAX					.getName() + ", ");
		sqlStatement.append(	"RPR_ZSPKZ AS " + 				PAYMENT_LOCKED_CFLAG						.getName() + ", ");
		sqlStatement.append(	"RPR_RDIFG AS " + 				TOTAL_DIFFERENCE_ALL_VOUCHERS				.getName() + ", ");
		sqlStatement.append(	"RPR_ZSPID AS " + 				USER_ID_PAYMENT_LOCK						.getName() + ", ");
		sqlStatement.append(	"RPR_VSON2 AS " + 				OTHER_REFUND_VALUE_REDUCED_TAX1				.getName() + ", ");
		sqlStatement.append(	"RPR_SKVS2 AS " + 				SAP_ACCOUNT_NO_OTHER_REFUND_REDUCED_TAX1	.getName() + ", ");
		sqlStatement.append(	"RPR_STAG AS " + 				DISCOUNT_DAYS								.getName() + ", ");
		sqlStatement.append(	"RPR_SPROZ AS " + 				CASH_DISCOUNT_SAP_IN_PERCENT				.getName() + ", ");
		sqlStatement.append(	"RPR_ZSCHL AS " + 				PAYMENT_KEY									.getName() + ", ");
		sqlStatement.append(	"RPR_RKZ AS " + 				VOUCHER_CODE								.getName() + ", ");
		sqlStatement.append(	"RPR_RTEXT AS " + 				VOUCHER_TEXT								.getName() + ", ");
		sqlStatement.append(	"RPR_RSTAT AS " + 				VOUCHER_STATE								.getName() + ", ");
		sqlStatement.append(	"RPR_ERFDA AS " + 				ACQUISITION_DATE							.getName() + ", ");
		sqlStatement.append(	"RPR_AENDA AS " + 				MODIFICATION_DATE							.getName() + ", ");
		sqlStatement.append(	"RPR_NACHDA AS " + 				POST_EDITING_DATE							.getName() + ", ");
		sqlStatement.append(	"RPR_SKVS1 AS " + 				SAP_ACCOUNT_NO_OTHER_REFUND_FULL_TAX		.getName() + ", ");
		sqlStatement.append(	"RPR_SAPDA AS " + 				TRANSFER_TO_SAP_DATE						.getName() + ", ");
		sqlStatement.append(	"RPR_ERFBID AS " + 				ACQUISITION_USER_ID							.getName() + ", ");
		sqlStatement.append(	"RPR_AENBID AS " + 				MODIFICATION_USER_ID						.getName() + ", ");
		sqlStatement.append(	"RPR_NACBID AS " + 				POST_EDITING_USER_ID						.getName() + ", ");
		sqlStatement.append(	"RPR_XBELNR AS " + 				VOUCHER_NO_SUCCESSOR						.getName() + ", ");
		sqlStatement.append(	"RPR_RBELNR AS " + 				VOUCHER_NO_PREDECESSOR						.getName() + ", ");
		sqlStatement.append(	"RPR_WBNR_001 AS " + 			PURCHASE_ORDER_NO_2							.getName() + ", ");
		sqlStatement.append(	"RPR_WBNR_002 AS " + 			PURCHASE_ORDER_NO_3							.getName() + ", ");
		sqlStatement.append(	"RPR_WBNR_003 AS " + 			PURCHASE_ORDER_NO_4							.getName() + ", ");
		sqlStatement.append(	"RPR_WBNR_004 AS " + 			PURCHASE_ORDER_NO_5							.getName() + ", ");
		sqlStatement.append(	"RPR_WBNR_005 AS " + 			PURCHASE_ORDER_NO_6							.getName() + ", ");
		sqlStatement.append(	"RPR_WBNR_006 AS " + 			PURCHASE_ORDER_NO_7							.getName() + ", ");
		sqlStatement.append(	"RPR_WBNR_007 AS " + 			PURCHASE_ORDER_NO_8							.getName() + ", ");
		sqlStatement.append(	"RPR_WBNR_008 AS " + 			PURCHASE_ORDER_NO_9							.getName() + ", ");
		sqlStatement.append(	"RPR_WBNR_009 AS " + 			PURCHASE_ORDER_NO_10						.getName() + ", ");
		sqlStatement.append(	"RPR_WBNR_010 AS " + 			PURCHASE_ORDER_NO_11						.getName() + ", ");
		sqlStatement.append(	"RPR_WBNR_011 AS " + 			PURCHASE_ORDER_NO_12						.getName() + ", ");
		sqlStatement.append(	"RPR_WBNR_012 AS " + 			PURCHASE_ORDER_NO_13						.getName() + ", ");
		sqlStatement.append(	"RPR_WBNR_013 AS " + 			PURCHASE_ORDER_NO_14						.getName() + ", ");
		sqlStatement.append(	"RPR_WBNR_014 AS " + 			PURCHASE_ORDER_NO_15						.getName() + ", ");
		sqlStatement.append(	"RPR_WBNR_015 AS " + 			PURCHASE_ORDER_NO_16						.getName() + ", ");
		sqlStatement.append(	"RPR_WEDAT AS " + 				OLDEST_POSTING_DATE							.getName() + ", ");
		sqlStatement.append(	"RPR_BWERT AS " + 				ORDER_VALUE									.getName() + ", ");
		sqlStatement.append(	"RPR_LWERT AS " + 				VALUE_GOODS_DELIVERED_CURRENT_PRICE			.getName() + ", ");
		sqlStatement.append(	"RPR_LWBPR AS " + 				VALUE_GOODS_DELIVERED_ORDER_PRICE			.getName() + ", ");
		sqlStatement.append(	"RPR_NRWE AS " + 				REBATE_IN_KIND_CURRENT_PRICE				.getName() + ", ");
		sqlStatement.append(	"RPR_NWBPR AS " + 				REBATE_IN_KIND_ORDER_PRICE					.getName() + ", ");
		sqlStatement.append(	"RPR_WSTAT AS " + 				GOODS_IN_STATE								.getName() + ", ");
		sqlStatement.append(	"RPR_AART AS " + 				PURCHASE_ORDER_TYPE							.getName() + ", ");
		sqlStatement.append(	"RPR_SSKZ AS " + 				TAX_KEY_CODE								.getName() + ", ");
		sqlStatement.append(	"RPR_BA AS " + 					ORDER_TYPE									.getName() + ", ");
		sqlStatement.append(	"RPR_WAEKZ AS " + 				CURRENCY_CODE_OLD							.getName() + ", ");
		sqlStatement.append(	"RPR_WAWE3 AS " + 				GOODS_VALUE_REDUCED_TAX2					.getName() + ", ");
		sqlStatement.append(	"RPR_MWSW3 AS " + 				REDUCED_TAX2_VALUE							.getName() + ", ");
		sqlStatement.append(	"RPR_RABA3 AS " + 				DISCOUNT_VALUE_REDUCED_TAX2					.getName() + ", ");
		sqlStatement.append(	"RPR_VSON3 AS " + 				OTHER_REFUND_VALUE_REDUCED_TAX2				.getName() + ", ");
		sqlStatement.append(	"RPR_SKVS3 AS " + 				SAP_ACCOUNT_NO_OTHER_REFUND_REDUCED_TAX2	.getName() + " ");

		sqlStatement.append("FROM VSAM.DKRPR ");
		resolve(sqlStatement);
	}
	//--- for documenting purposes only! ---
	//! \ingroup SQLSTMT
	//! select-statement to select all DKFBK joined with DKFBP data rows
	basar::ConstString SelectAll::s_SqlStmt = 
		"SELECT "
			"RPR_FIL2 "			
			"RPR_HNR "				
			"RPR_BLNR2 "			
			"RPR_SAPHNR "			
			"RPR_KZRG "			
			"RPR_BNR " 			
			"RPR_BESDAT " 			
			"RPR_RENR || RPR_RENR2 " 
			"RPR_REDAT " 			
			"RPR_STBID " 			
			"RPR_VADAT " 			
			"RPR_RBETR " 			
			"RPR_RWAWE " 			
			"RPR_RDIFF " 			
			"RPR_WAWE1 " 			
			"RPR_BZFRA " 			
			"RPR_WAWE2 " 			
			"RPR_BZEXP " 			
			"RPR_WAWEO " 			
			"RPR_MWSW1 " 			
			"RPR_MWSW2 " 			
			"RPR_RABA1 " 			
			"RPR_RABA2 " 			
			"RPR_VVERP " 			
			"RPR_VSON1 " 			
			"RPR_ZSPKZ " 			
			"RPR_RDIFG " 			
			"RPR_ZSPID " 			
			"RPR_VSON2 " 			
			"RPR_SKVS2 " 			
			"RPR_STAG " 			
			"RPR_SPROZ " 			
			"RPR_ZSCHL " 			
			"RPR_RKZ " 			
			"RPR_RTEXT " 			
			"RPR_RSTAT " 			
			"RPR_ERFDA " 			
			"RPR_AENDA " 			
			"RPR_NACHDA " 			
			"RPR_SKVS1 " 			
			"RPR_SAPDA " 			
			"RPR_ERFBID " 			
			"RPR_AENBID " 			
			"RPR_NACBID " 			
			"RPR_XBELNR " 			
			"RPR_RBELNR " 			
			"RPR_WBNR1 " 			
			"RPR_WBNR2 " 			
			"RPR_WBNR3 " 			
			"RPR_WBNR4 " 			
			"RPR_WBNR5 " 			
			"RPR_WBNR6 " 			
			"RPR_WBNR7 " 			
			"RPR_WBNR8 " 			
			"RPR_WBNR9 " 			
			"RPR_WBNR10 " 			
			"RPR_WBNR11 " 			
			"RPR_WBNR12 " 			
			"RPR_WBNR13 " 			
			"RPR_WBNR14 " 			
			"RPR_WBNR15 " 			
			"RPR_WEDAT " 			
			"RPR_BWERT " 			
			"RPR_LWERT " 			
			"RPR_LWBPR " 			
			"RPR_NRWE " 			
			"RPR_NWBPR " 			
			"RPR_WSTAT " 	
			"RPR_AART "
			"RPR_SSKZ " 			
			"RPR_BA " 				
			"RPR_WAEKZ " 			
			"RPR_WAWE3 " 			
			"RPR_MWSW3 " 			
			"RPR_RABA3 " 			
			"RPR_VSON3 " 			
			"RPR_SKVS3 " 			
		"FROM VSAM.DKRPR ";

//-------------------------------------------------------------------------------------------//
// INSERTs/UPDATEs
//-------------------------------------------------------------------------------------------//

//-------------------------------------------------------------------------------------------//
// DELETEs
//-------------------------------------------------------------------------------------------//

//-------------------------------------------------------------------------------------------//
// ACCESSOR DEFINITION
//-------------------------------------------------------------------------------------------//
BEGIN_ACCESSOR_DEFINITION("AccCicsOldVoucher")

	BEGIN_PROPERTY_DESCRIPTION_LIST
		PROPERTY_DESCRIPTION_LIST_ADD(BRANCH_NO);
		PROPERTY_DESCRIPTION_LIST_ADD(SUPPLIER_NO);
		PROPERTY_DESCRIPTION_LIST_ADD(VOUCHER_NO);
		PROPERTY_DESCRIPTION_LIST_ADD(SAP_SUPPLIER_NO);
		PROPERTY_DESCRIPTION_LIST_ADD(BASE_TYPE_CODE);
		PROPERTY_DESCRIPTION_LIST_ADD(PURCHASE_ORDER_NO_1);
		PROPERTY_DESCRIPTION_LIST_ADD(PURCHASE_ORDER_DATE);
		PROPERTY_DESCRIPTION_LIST_ADD(DOCUMENT_CODE);
		PROPERTY_DESCRIPTION_LIST_ADD(DOCUMENT_DATE);
		PROPERTY_DESCRIPTION_LIST_ADD(USER_ID_CANCEL);
		PROPERTY_DESCRIPTION_LIST_ADD(VALUE_DATE);
		PROPERTY_DESCRIPTION_LIST_ADD(DOCUMENT_FINAL_VALUE);
		PROPERTY_DESCRIPTION_LIST_ADD(GOODS_VALUE_CALCULATED);
		PROPERTY_DESCRIPTION_LIST_ADD(DIFFERENCE);
		PROPERTY_DESCRIPTION_LIST_ADD(GOODS_VALUE_FULL_TAX);
		PROPERTY_DESCRIPTION_LIST_ADD(DELIVERY_COSTS_FREIGHT);
		PROPERTY_DESCRIPTION_LIST_ADD(GOODS_VALUE_REDUCED_TAX1);
		PROPERTY_DESCRIPTION_LIST_ADD(DELIVERY_COSTS_EXPRESS);
		PROPERTY_DESCRIPTION_LIST_ADD(GOODS_VALUE_NO_TAX);
		PROPERTY_DESCRIPTION_LIST_ADD(FULL_TAX_VALUE);
		PROPERTY_DESCRIPTION_LIST_ADD(REDUCED_TAX1_VALUE);
		PROPERTY_DESCRIPTION_LIST_ADD(DISCOUNT_VALUE_FULL_TAX);
		PROPERTY_DESCRIPTION_LIST_ADD(DISCOUNT_VALUE_REDUCED_TAX1);
		PROPERTY_DESCRIPTION_LIST_ADD(PACKAGE_REFUND_VALUE);
		PROPERTY_DESCRIPTION_LIST_ADD(OTHER_REFUND_VALUE_FULL_TAX);
		PROPERTY_DESCRIPTION_LIST_ADD(PAYMENT_LOCKED_CFLAG);
		PROPERTY_DESCRIPTION_LIST_ADD(TOTAL_DIFFERENCE_ALL_VOUCHERS);
		PROPERTY_DESCRIPTION_LIST_ADD(USER_ID_PAYMENT_LOCK);
		PROPERTY_DESCRIPTION_LIST_ADD(OTHER_REFUND_VALUE_REDUCED_TAX1);
		PROPERTY_DESCRIPTION_LIST_ADD(SAP_ACCOUNT_NO_OTHER_REFUND_REDUCED_TAX1);
		PROPERTY_DESCRIPTION_LIST_ADD(DISCOUNT_DAYS);
		PROPERTY_DESCRIPTION_LIST_ADD(CASH_DISCOUNT_SAP_IN_PERCENT);
		PROPERTY_DESCRIPTION_LIST_ADD(PAYMENT_KEY);
		PROPERTY_DESCRIPTION_LIST_ADD(VOUCHER_CODE);
		PROPERTY_DESCRIPTION_LIST_ADD(VOUCHER_TEXT);
		PROPERTY_DESCRIPTION_LIST_ADD(VOUCHER_STATE);
		PROPERTY_DESCRIPTION_LIST_ADD(ACQUISITION_DATE);
		PROPERTY_DESCRIPTION_LIST_ADD(MODIFICATION_DATE);
		PROPERTY_DESCRIPTION_LIST_ADD(POST_EDITING_DATE);
		PROPERTY_DESCRIPTION_LIST_ADD(SAP_ACCOUNT_NO_OTHER_REFUND_FULL_TAX);
		PROPERTY_DESCRIPTION_LIST_ADD(TRANSFER_TO_SAP_DATE);
		PROPERTY_DESCRIPTION_LIST_ADD(ACQUISITION_USER_ID);
		PROPERTY_DESCRIPTION_LIST_ADD(MODIFICATION_USER_ID);
		PROPERTY_DESCRIPTION_LIST_ADD(POST_EDITING_USER_ID);
		PROPERTY_DESCRIPTION_LIST_ADD(VOUCHER_NO_SUCCESSOR);
		PROPERTY_DESCRIPTION_LIST_ADD(VOUCHER_NO_PREDECESSOR);
		PROPERTY_DESCRIPTION_LIST_ADD(PURCHASE_ORDER_NO_2);
		PROPERTY_DESCRIPTION_LIST_ADD(PURCHASE_ORDER_NO_3);
		PROPERTY_DESCRIPTION_LIST_ADD(PURCHASE_ORDER_NO_4);
		PROPERTY_DESCRIPTION_LIST_ADD(PURCHASE_ORDER_NO_5);
		PROPERTY_DESCRIPTION_LIST_ADD(PURCHASE_ORDER_NO_6);
		PROPERTY_DESCRIPTION_LIST_ADD(PURCHASE_ORDER_NO_7);
		PROPERTY_DESCRIPTION_LIST_ADD(PURCHASE_ORDER_NO_8);
		PROPERTY_DESCRIPTION_LIST_ADD(PURCHASE_ORDER_NO_9);
		PROPERTY_DESCRIPTION_LIST_ADD(PURCHASE_ORDER_NO_10);
		PROPERTY_DESCRIPTION_LIST_ADD(PURCHASE_ORDER_NO_11);
		PROPERTY_DESCRIPTION_LIST_ADD(PURCHASE_ORDER_NO_12);
		PROPERTY_DESCRIPTION_LIST_ADD(PURCHASE_ORDER_NO_13);
		PROPERTY_DESCRIPTION_LIST_ADD(PURCHASE_ORDER_NO_14);
		PROPERTY_DESCRIPTION_LIST_ADD(PURCHASE_ORDER_NO_15);
		PROPERTY_DESCRIPTION_LIST_ADD(PURCHASE_ORDER_NO_16);
		PROPERTY_DESCRIPTION_LIST_ADD(OLDEST_POSTING_DATE);
		PROPERTY_DESCRIPTION_LIST_ADD(ORDER_VALUE);
		PROPERTY_DESCRIPTION_LIST_ADD(VALUE_GOODS_DELIVERED_CURRENT_PRICE);
		PROPERTY_DESCRIPTION_LIST_ADD(VALUE_GOODS_DELIVERED_ORDER_PRICE);
		PROPERTY_DESCRIPTION_LIST_ADD(REBATE_IN_KIND_CURRENT_PRICE);
		PROPERTY_DESCRIPTION_LIST_ADD(REBATE_IN_KIND_ORDER_PRICE);
		PROPERTY_DESCRIPTION_LIST_ADD(GOODS_IN_STATE);
		PROPERTY_DESCRIPTION_LIST_ADD(PURCHASE_ORDER_TYPE);
		PROPERTY_DESCRIPTION_LIST_ADD(TAX_KEY_CODE);
		PROPERTY_DESCRIPTION_LIST_ADD(ORDER_TYPE);
		PROPERTY_DESCRIPTION_LIST_ADD(CURRENCY_CODE_OLD);
		PROPERTY_DESCRIPTION_LIST_ADD(GOODS_VALUE_REDUCED_TAX2);
		PROPERTY_DESCRIPTION_LIST_ADD(REDUCED_TAX2_VALUE);
		PROPERTY_DESCRIPTION_LIST_ADD(DISCOUNT_VALUE_REDUCED_TAX2);
		PROPERTY_DESCRIPTION_LIST_ADD(OTHER_REFUND_VALUE_REDUCED_TAX2);
		PROPERTY_DESCRIPTION_LIST_ADD(SAP_ACCOUNT_NO_OTHER_REFUND_REDUCED_TAX2);
	END_PROPERTY_DESCRIPTION_LIST

	ACCESS_METHOD("selectAll")
		SQL_BUILDER_CREATE(SelectAll)
		SQL_BUILDER_PUSH_BACK(SelectAll)

END_ACCESSOR_DEFINITION
				
//----------------------------------------------------------------------------
}//acc_cicsoldvoucher

