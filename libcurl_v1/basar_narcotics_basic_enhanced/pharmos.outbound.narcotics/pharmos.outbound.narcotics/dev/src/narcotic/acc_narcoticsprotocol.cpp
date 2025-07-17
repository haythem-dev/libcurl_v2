//----------------------------------------------------------------------------------------------
/*! \file
 *  \brief  string builder definition for protocol-accessor + registration of protocol-accessor
 *  \brief  string builder definition for catalog-accessor + registration of catalog-accessor
 *  \author Kerstin Reuther
 *  \date   21.03.2006
 */
//----------------------------------------------------------------------------------------------
//includes
//----------------------------------------------------------------------------------------------
#include "commonheader.h"
#include "loggerpool.h"
#include "acc_narcoticsprotocol.h"

//----------------------------------------------------------------------------------------------
//namespaces
//----------------------------------------------------------------------------------------------
namespace acc_narcoticsprotocol
{

//---------------------------------------------------------------------------
BUILDER_DEFINITION(SelPro)

	ENSURE_PROPERTIES_ARE_SET(SelPro,"branchno;")
	
	void SelPro::buildSQLString()
	{
		basar::VarString res;
		res.format("SELECT catalogid, protocolid, username, protocoldate, protocoltime, activitytype, "
							"changedattribute, valueold, valuenew "
					"FROM narcprotocol "
					"WHERE catalogid = #%s# ", constants::CATALOGID);
		resolve(res);

		if( !(getPropertyList().getState(constants::CHANGEDATTRIBUTE) == basar::SS_UNSET) )
		{
			basar::VarString resatt;
			resatt.format("AND changedattribute = '#%s#' ", constants::CHANGEDATTRIBUTE);
			resolve(resatt);
		}

		if( !(getPropertyList().getState(constants::USERNAME) == basar::SS_UNSET) )
		{
			basar::VarString resusrname;
			resusrname.format("AND username = '#%s#' ", constants::USERNAME);
			resolve(resusrname);
		}

		resolve("ORDER BY protocolid");
	}

//-----------------------------------------------------------------------------------------------------
BUILDER_DEFINITION(InsProUpdate)

	ENSURE_PROPERTIES_ARE_SET_AND_ONE_IN_STATE(InsProUpdate,"catalogid; username; protocoldate; "
												"protocoltime; activitytype; changedattribute; valueold; "
												"valuenew;", "INSERT")

	SQL_PATTERN(InsProUpdate,
	            "INSERT INTO narcprotocol (catalogid, username, protocoldate, protocoltime, activitytype, "
				"changedattribute, valueold, valuenew) VALUES (#catalogid#, '#username#', #protocoldate#, "
				"#protocoltime#, '#activitytype#', '#changedattribute#', '#valueold#', '#valuenew#')")

//-----------------------------------------------------------------------------------------------------
BUILDER_DEFINITION(InsProDelete)

	ENSURE_PROPERTIES_ARE_SET_AND_ONE_IN_STATE(InsProDelete,"catalogid; username; protocoldate; "
												"protocoltime; activitytype;", "INSERT")

	SQL_PATTERN(InsProDelete,
	            "INSERT INTO narcprotocol (catalogid, username, protocoldate, protocoltime, activitytype) "
				"VALUES (#catalogid#, '#username#', #protocoldate#, #protocoltime#, '#activitytype#')")

//-----------------------------------------------------------------------------------------------------
BUILDER_DEFINITION(InsProRestore)

	ENSURE_PROPERTIES_ARE_SET_AND_ONE_IN_STATE(InsProRestore,"catalogid; username; protocoldate; "
												"protocoltime; activitytype;", "INSERT")

	SQL_PATTERN(InsProRestore,
	            "INSERT INTO narcprotocol (catalogid, username, protocoldate, protocoltime, activitytype) "
				"VALUES (#catalogid#, '#username#', #protocoldate#, #protocoltime#, '#activitytype#')")

//---------------------------------------------------------------------------
BEGIN_ACCESSOR_DEFINITION("NarcoticsProtocol")

	PROPERTY_DESCRIPTION_LIST("int32 protocolid; int32 catalogid; string username; int32 protocoldate; "
								"int32 protocoltime; string activitytype; string changedattribute; string valueold; "
								"string valuenew;")				

	ACCESS_METHOD("SelProtocol")
		SQL_BUILDER_CREATE(SelPro)
		SQL_BUILDER_PUSH_BACK(SelPro)

	ACCESS_METHOD("InsProtocolUpdate")
		SQL_BUILDER_CREATE(InsProUpdate)
		SQL_BUILDER_PUSH_BACK(InsProUpdate)

	ACCESS_METHOD("InsProtocolDelete")
		SQL_BUILDER_CREATE(InsProDelete)
		SQL_BUILDER_PUSH_BACK(InsProDelete)

	ACCESS_METHOD("InsProtocolRestore")
		SQL_BUILDER_CREATE(InsProRestore)
		SQL_BUILDER_PUSH_BACK(InsProRestore)

END_ACCESSOR_DEFINITION
} //namespace acc_narcoticsprotocol

//---------------------------------------------------------------------------
namespace acc_narcoticstransactiondata
{

//---------------------------------------------------------------------------
BUILDER_DEFINITION(SelTCat)

	ENSURE_PROPERTIES_ARE_SET(SelTCat,"branchno;")
	
	void SelTCat::buildSQLString()
	{
		resolve("SELECT DISTINCT(c.catalogid), branchno, c.articleno, a.article_code AS articlecode, transactiondate, transactiontime, "
				"customersupplierno, transactiontype, purchaseorderno, qtytransaction, qtyinstock "
				"FROM narctransactioncatalog c, narcprotocol p, articlecodes a "
				"WHERE branchno = #branchno# AND c.catalogid = p.catalogid AND c.articleno=a.articleno AND a.preferred_flag=1 ");

		if( !(getPropertyList().getState(constants::TODATE) == basar::SS_UNSET || getPropertyList().getState(constants::FROMDATE) == basar::SS_UNSET) )
		{
			basar::VarString res;
			res.format("AND transactiondate BETWEEN #%s# AND #%s# ", constants::FROMDATE, constants::TODATE);
			resolve(res);
		}
		else 
		{
			if( !(getPropertyList().getState(constants::TODATE) == basar::SS_UNSET) )
			{
				basar::VarString res;
				res.format("AND transactiondate <= #%s# ", constants::TODATE);
				resolve(res);
			}
			else if( !(getPropertyList().getState(constants::FROMDATE) == basar::SS_UNSET) )
			{
				basar::VarString res;
				res.format("AND transactiondate >= #%s# ", constants::FROMDATE);
				resolve(res);
			}
		}

		if( !(getPropertyList().getState(constants::ARTICLENO) == basar::SS_UNSET) )
		{
			basar::VarString res;
			res.format("AND c.articleno = #%s# ", constants::ARTICLENO);
			resolve(res);
		}

		if( !(getPropertyList().getState(constants::ARTICLECODE) == basar::SS_UNSET) )
		{
			basar::VarString res;
			res.format("AND a.article_code = '#%s#' ", constants::ARTICLECODE);
			resolve(res);
		}

		if( !(getPropertyList().getState(constants::CHANGEDATTRIBUTE) == basar::SS_UNSET) )
		{
			basar::VarString res;
			res.format("AND changedattribute = '#%s#' ", constants::CHANGEDATTRIBUTE);
			resolve(res);
		}

		if( !(getPropertyList().getState(constants::USERNAME) == basar::SS_UNSET) )
		{
			basar::VarString res;
			res.format("AND p.username = '#%s#' ", constants::USERNAME);
			resolve(res);
		}

		resolve("ORDER BY catalogid");
		BLOG_TRACE( narcotics::LoggerPool::loggerDomModules, getSQLString() );
	}

//---------------------------------------------------------------------------
BEGIN_ACCESSOR_DEFINITION("NarcoticsTransactionData")

	PROPERTY_DESCRIPTION_LIST("int32 catalogid; int16 branchno; int32 articleno; string articlecode; int32 transactiondate; int32 transactiontime; "
								"int32 customersupplierno; string transactiontype; int32 purchaseorderno; "
								"int32 qtytransaction; int32 qtyinstock;")

	
	ACCESS_METHOD("SelTransactionCatalog")
		SQL_BUILDER_CREATE(SelTCat)
		SQL_BUILDER_PUSH_BACK(SelTCat)


END_ACCESSOR_DEFINITION

} //namespace acc_narcoticsTransactionData