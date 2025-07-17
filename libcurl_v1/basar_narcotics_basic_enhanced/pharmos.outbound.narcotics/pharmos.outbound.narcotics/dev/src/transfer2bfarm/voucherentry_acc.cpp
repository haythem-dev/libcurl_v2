//------------------------------------------------------------------------------
//includes
//------------------------------------------------------------------------------
#include "commonheader.h"
#include "voucherentry_acc.h"
#include "definitions.h"
#include "loggerpool.h"

//------------------------------------------------------------------------------
//namespace
//------------------------------------------------------------------------------
namespace narcotics
{
namespace voucherentry_acc
{

//------------------------------------------------------------------------------
BUILDER_DEFINITION( SelectContributionVoucherEntries )
ENSURE_PROPERTIES_ARE_SET( SelectContributionVoucherEntries,	narcotics::constants::BRANCHNO.toPropertyString() +
																narcotics::constants::PROCESSSTARTDATE.toPropertyString() + 
																narcotics::constants::PROCESSENDDATE.toPropertyString())

//------------------------------------------------------------------------------
void SelectContributionVoucherEntries::buildSQLString()
{
    basar::I18nString sqlStatement;

	sqlStatement.append(
		"SELECT DISTINCT "
			"contributionvoucherno AS "		+ narcotics::constants::VOUCHERNO.getName()				+ ", "
			"transactiondate AS "			+ narcotics::constants::TRANSACTIONDATE.getName()		+ ", "
			"username AS "					+ narcotics::constants::SUBSCRIBER.getName()			+ ", "
			"customersupplierno AS "		+ narcotics::constants::CUSTOMERSUPPLIERNO.getName()	+ ", "
			"purchaseorderno AS "			+ narcotics::constants::PURCHASEORDERNO.getName()		+ ", "
			"printeddate AS "				+ narcotics::constants::PRINTEDDATE.getName()			+ " "
		"FROM narctransactioncatalog "
		"WHERE "
			"branchno = "				+ narcotics::constants::BRANCHNO.toSQLReplacementString()			+ " "
			"AND transactiondate >= "	+ narcotics::constants::PROCESSSTARTDATE.toSQLReplacementString()	+ " "
			"AND transactiondate <= "	+ narcotics::constants::PROCESSENDDATE.toSQLReplacementString()		+ " "
			"AND contributionvoucherno > 0 "
			"AND acknowledgementstatus >= 2 "
			"AND transactiontype in ('a', 'd') "
			"AND ((deletedflag = 0) OR (deletedflag = 1 AND acknowledgementstatus = 5))");
    resolve( sqlStatement );
	BLOG_TRACE( LoggerPool::getLoggerTransferBfarm(), getSQLString() );
}
//------------------------------------------------------------------------------
BUILDER_DEFINITION( SelectDeliveryNoteCopyEntries )
ENSURE_PROPERTIES_ARE_SET( SelectDeliveryNoteCopyEntries,	narcotics::constants::BRANCHNO.toPropertyString() +
															narcotics::constants::PROCESSSTARTDATE.toPropertyString() + 
															narcotics::constants::PROCESSENDDATE.toPropertyString())

//------------------------------------------------------------------------------
void SelectDeliveryNoteCopyEntries::buildSQLString()
{
    basar::I18nString sqlStatement;

	sqlStatement.append(
		"SELECT DISTINCT "
			"contributionvoucherno AS "		+ narcotics::constants::VOUCHERNO.getName()				+ ", "
			"transactiondate AS "			+ narcotics::constants::TRANSACTIONDATE.getName()		+ ", "
			"username AS "					+ narcotics::constants::SUBSCRIBER.getName()			+ ", "
			"customersupplierno AS "		+ narcotics::constants::CUSTOMERSUPPLIERNO.getName()	+ ", "
			"purchaseorderno AS "			+ narcotics::constants::PURCHASEORDERNO.getName()		+ ", "
			"printeddate AS "				+ narcotics::constants::PRINTEDDATE.getName()			+ " "
		"FROM narctransactioncatalog "
		"WHERE "
			"branchno = "				+ narcotics::constants::BRANCHNO.toSQLReplacementString()			+ " "
			"AND transactiondate >= "	+ narcotics::constants::PROCESSSTARTDATE.toSQLReplacementString()	+ " "
			"AND transactiondate <= "	+ narcotics::constants::PROCESSENDDATE.toSQLReplacementString()		+ " "
			"AND contributionvoucherno > 0 "
			"AND acknowledgementstatus = 5"
		);
    resolve( sqlStatement );
	BLOG_TRACE( LoggerPool::getLoggerTransferBfarm(), getSQLString() );
}

//------------------------------------------------------------------------------
BEGIN_ACCESSOR_DEFINITION("AccVoucherEntries")

	BEGIN_PROPERTY_DESCRIPTION_LIST
		PROPERTY_DESCRIPTION_LIST_ADD(narcotics::constants::PROCESSSTARTDATE);
		PROPERTY_DESCRIPTION_LIST_ADD(narcotics::constants::PROCESSENDDATE);
		PROPERTY_DESCRIPTION_LIST_ADD(narcotics::constants::VOUCHERNO);
		PROPERTY_DESCRIPTION_LIST_ADD(narcotics::constants::BRANCHNO);
		PROPERTY_DESCRIPTION_LIST_ADD(narcotics::constants::TRANSACTIONDATE);
		PROPERTY_DESCRIPTION_LIST_ADD(narcotics::constants::PURCHASEORDERNO);
		PROPERTY_DESCRIPTION_LIST_ADD(narcotics::constants::SUBSCRIBER);
		PROPERTY_DESCRIPTION_LIST_ADD(narcotics::constants::PRINTEDDATE);
		PROPERTY_DESCRIPTION_LIST_ADD(narcotics::constants::CUSTOMERSUPPLIERNO);
	END_PROPERTY_DESCRIPTION_LIST

//------------------------------------------------------------------------------
ACCESS_METHOD("amSelectContributionVoucherEntries")
		SQL_BUILDER_CREATE(SelectContributionVoucherEntries)
		SQL_BUILDER_PUSH_BACK(SelectContributionVoucherEntries)

ACCESS_METHOD("amSelectDeliveryNoteCopyEntries")
		SQL_BUILDER_CREATE(SelectDeliveryNoteCopyEntries)
		SQL_BUILDER_PUSH_BACK(SelectDeliveryNoteCopyEntries)

//------------------------------------------------------------------------------
END_ACCESSOR_DEFINITION

} //namespace voucherentry_acc
} //namespace narcotics