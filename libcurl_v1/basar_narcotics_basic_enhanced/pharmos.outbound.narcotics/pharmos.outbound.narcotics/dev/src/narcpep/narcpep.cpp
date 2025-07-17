//----------------------------------------------------------------------------
//
//----------------------------------------------------------------------------

#include "libbasarcmnutil.h"
#include "libbasardbsql.h"

//----------------------------------------------------------------------------
//
//----------------------------------------------------------------------------

basar::db::sql::PreparedStatementRef	stmtSelCatalog;
basar::db::sql::PreparedStatementRef	stmtSelProtocol;
basar::db::sql::PreparedStatementRef	stmtInsCatalog;
basar::db::sql::PreparedStatementRef	stmtInsProtocol;

//----------------------------------------------------------------------------
//
//----------------------------------------------------------------------------

void resetPreparedStatementsNARC()
{
	stmtSelCatalog.close();
	stmtSelCatalog.reset();

	stmtSelProtocol.close();
	stmtSelProtocol.reset();

	stmtInsCatalog.close();
	stmtInsCatalog.reset();

	stmtInsProtocol.close();
	stmtInsProtocol.reset();
}

//----------------------------------------------------------------------------
//
//----------------------------------------------------------------------------

void createPreparedStatementsNARC(basar::db::sql::ConnectionRef dbConn, const basar::Int16 branchNo, const basar::VarString& fromDB, const basar::VarString& toDB, const basar::Int32 fromDate)
{
	basar::VarString sql;
	
	sql.format(
		"select "
			"catalogid, "
			"branchno, "
			"articleno, "
			"transactiondate, "
			"transactiontime, "
			"customersupplierno, "
			"transactiontype, "
			"purchaseorderno, "
			"qtytransaction, "
			"qtyinstock, "
			"deletedflag, "
			"processedflag, "
			"customersuppliername, "
			"customersupplierstreet, "
			"customersuppliercity, "
			"invoiceno, "
			"narcgroup, "
			"narcname, "
			"narcpotency, "
			"narcunit, "
			"narcfactor, "
			"contributionvoucherno, "
			"username, "
			"printeddate, "
			"acknowledgementstatus, "
			"tourid, "
			"printedtime "
		"from %s:narctransactioncatalog "
		"where branchno = %d ", fromDB.c_str(), branchNo);
	if (fromDate > 0)
	{
		basar::VarString tmp;
		tmp.format("and transactiondate >= %d ", fromDate);
		sql.append(tmp);
	}
	sql.append("order by catalogid");
	LOG4CPLUS_INFO(log4cplus::Logger::getRoot(), "SELECT STMT:");
	LOG4CPLUS_INFO(log4cplus::Logger::getRoot(), sql);
	stmtSelCatalog = dbConn.createPreparedStatement(sql, basar::db::sql::CURS_HOLD_OVER_COMMIT);

	sql.format(
		"select "
			"protocolid, catalogid, username, protocoldate, protocoltime, activitytype, changedattribute, valueold, valuenew "
		"from %s:narcprotocol "
		"where catalogid=?", fromDB.c_str());
	stmtSelProtocol = dbConn.createPreparedStatement(sql, basar::db::sql::CURS_HOLD_OVER_COMMIT);

	sql.format(
		"insert into %s:narctransactioncatalog ("
			"branchno, "
			"articleno, "
			"transactiondate, "
			"transactiontime, "
			"customersupplierno, "
			"transactiontype, "
			"purchaseorderno, "
			"qtytransaction, "
			"qtyinstock, "
			"deletedflag, "
			"processedflag, "
			"customersuppliername, "
			"customersupplierstreet, "
			"customersuppliercity, "
			"invoiceno, "
			"narcgroup, "
			"narcname, "
			"narcpotency, "
			"narcunit, "
			"narcfactor, "
			"contributionvoucherno, "
			"username, "
			"printeddate, "
			"acknowledgementstatus, "
			"tourid, "
			"printedtime )"
		"values "
			"( "
			" ?, ?, ?, ?, ?, ?, ?, ?, ?, ?,"
			" ?, ?, ?, ?, ?, ?, ?, ?, ?, ?,"
			" ?, ?, ?, ?, ?, ?)", toDB.c_str());
	stmtInsCatalog = dbConn.createPreparedStatement(sql);

	sql.format(
		"insert into %s:narcprotocol "
			"(catalogid, username, protocoldate, protocoltime, activitytype, changedattribute, valueold, valuenew) "
		"values "
			"(?, ?, ?, ?, ?, ?, ?, ?)", toDB.c_str());
	stmtInsProtocol = dbConn.createPreparedStatement(sql);
}

//----------------------------------------------------------------------------
//
//----------------------------------------------------------------------------

basar::db::sql::ExecuteReturnInfo insertCatalog(basar::db::sql::ResultsetRef resultSetCatalog)
{
	// first insert catalog and get new serial id
	static basar::Int16		branchno				= 0;
	static basar::Int32		articleno				= 0;
	static basar::Int32		transactiondate			= 0;
	static basar::Int32		transactiontime			= 0;
	static basar::Int32		customersupplierno		= 0;
	static basar::VarString	transactiontype			= basar::VarString();
	static basar::Int32		purchaseorderno			= 0;
	static basar::Int32		qtytransaction			= 0;
	static basar::Int32		qtyinstock				= 0;
	static basar::VarString	deletedflag				= basar::VarString();
	static basar::VarString	processedflag			= basar::VarString();
	static basar::VarString	customersuppliername	= basar::VarString();
	static basar::VarString	customersupplierstreet	= basar::VarString();
	static basar::VarString	customersuppliercity	= basar::VarString();
	static basar::Int32		invoiceno				= 0;
	static basar::VarString	narcgroup				= basar::VarString();
	static basar::VarString	narcname				= basar::VarString();
	static basar::Decimal	narcpotency				= basar::Decimal(0);
	static basar::VarString	narcunit				= basar::VarString();
	static basar::Decimal	narcfactor				= basar::Decimal(0);
	static basar::Int32		contributionvoucherno	= 0;
	static basar::VarString	username				= basar::VarString();
	static basar::Int32		printeddate				= 0;
	static basar::Int16		acknowledgementstatus	= 0;
	static basar::VarString	tourid					= basar::VarString();
	static basar::Int32		printedtime				= 0;

	branchno				= resultSetCatalog.getInt16("branchno");
	articleno				= resultSetCatalog.getInt32("articleno");
	transactiondate			= resultSetCatalog.getInt32("transactiondate");
	transactiontime			= resultSetCatalog.getInt32("transactiontime");
	customersupplierno		= resultSetCatalog.getInt32("customersupplierno");
	transactiontype			= resultSetCatalog.getString("transactiontype");
	purchaseorderno			= resultSetCatalog.getInt32("purchaseorderno");
	qtytransaction			= resultSetCatalog.getInt32("qtytransaction");
	qtyinstock				= resultSetCatalog.getInt32("qtyinstock");
	deletedflag				= resultSetCatalog.getString("deletedflag");
	processedflag			= resultSetCatalog.getString("processedflag");
	customersuppliername	= resultSetCatalog.getString("customersuppliername");
	customersupplierstreet	= resultSetCatalog.getString("customersupplierstreet");
	customersuppliercity	= resultSetCatalog.getString("customersuppliercity");
	invoiceno				= resultSetCatalog.getInt32("invoiceno");
	narcgroup				= resultSetCatalog.getString("narcgroup");
	narcname				= resultSetCatalog.getString("narcname");
	narcpotency				= resultSetCatalog.getDecimal("narcpotency");
	narcunit				= resultSetCatalog.getString("narcunit");
	narcfactor				= resultSetCatalog.getDecimal("narcfactor");
	contributionvoucherno	= resultSetCatalog.getInt32("contributionvoucherno");
	username				= resultSetCatalog.getString("username");
	printeddate				= resultSetCatalog.getInt32("printeddate");
	acknowledgementstatus	= resultSetCatalog.getInt16("acknowledgementstatus");
	tourid					= resultSetCatalog.getString("tourid");
	printedtime				= resultSetCatalog.getInt32("printedtime");

// workaround BASAR
	if (transactiontype.empty())		transactiontype = " ";
	if (deletedflag.empty())			deletedflag = " ";
	if (processedflag.empty())			processedflag = " ";
	if (customersuppliername.empty())	customersuppliername = " ";
	if (customersupplierstreet.empty())	customersupplierstreet = " ";
	if (customersuppliercity.empty())	customersuppliercity = " ";
	if (narcgroup.empty())				narcgroup = " ";
	if (narcname.empty())				narcname = " ";
	if (narcunit.empty())				narcunit = " ";
	if (username.empty())				username = " ";
	if (tourid.empty())					tourid = " ";
// workaround BASAR

	stmtInsCatalog.setInt16(0,		branchno);
	stmtInsCatalog.setInt32(1,		articleno);
	stmtInsCatalog.setInt32(2,		transactiondate);
	stmtInsCatalog.setInt32(3,		transactiontime);
	stmtInsCatalog.setInt32(4,		customersupplierno);
	stmtInsCatalog.setString(5,		transactiontype.c_str());
	stmtInsCatalog.setInt32(6,		purchaseorderno);
	stmtInsCatalog.setInt32(7,		qtytransaction);
	stmtInsCatalog.setInt32(8,		qtyinstock);
	stmtInsCatalog.setString(9,		deletedflag.c_str());
	stmtInsCatalog.setString(10,	processedflag.c_str());
	stmtInsCatalog.setString(11,	customersuppliername.c_str());
	stmtInsCatalog.setString(12,	customersupplierstreet.c_str());
	stmtInsCatalog.setString(13,	customersuppliercity.c_str());
	stmtInsCatalog.setInt32(14,		invoiceno);
	stmtInsCatalog.setString(15,	narcgroup.c_str());
	stmtInsCatalog.setString(16,	narcname.c_str());
	stmtInsCatalog.setDecimal(17,	narcpotency);
	stmtInsCatalog.setString(18,	narcunit.c_str());
	stmtInsCatalog.setDecimal(19,	narcfactor);
	stmtInsCatalog.setInt32(20,		contributionvoucherno);
	stmtInsCatalog.setString(21,	username.c_str());
	stmtInsCatalog.setInt32(22,		printeddate);
	stmtInsCatalog.setInt16(23,		acknowledgementstatus);
	stmtInsCatalog.setString(24,	tourid.c_str());
	stmtInsCatalog.setInt32(25,		printedtime);

	basar::db::sql::ExecuteReturnInfo retInfo = stmtInsCatalog.execute();

	return retInfo;
}

//----------------------------------------------------------------------------
//
//----------------------------------------------------------------------------

basar::db::sql::ExecuteReturnInfo insertProtocol(basar::Int32 newCatalogID, basar::db::sql::ResultsetRef resultSetProtocol)
{
	static basar::Int32		protocoldate		= 0;
	static basar::Int32		protocoltime		= 0;
	static basar::VarString	username			= basar::VarString();
	static basar::VarString	activitytype		= basar::VarString();
	static basar::VarString	changedattribute	= basar::VarString();
	static basar::VarString	valueold			= basar::VarString();
	static basar::VarString	valuenew			= basar::VarString();

	protocoldate		= resultSetProtocol.getInt32("protocoldate");
	protocoltime		= resultSetProtocol.getInt32("protocoltime");
	username			= resultSetProtocol.getString("username");
	activitytype		= resultSetProtocol.getString("activitytype");
	changedattribute	= resultSetProtocol.getString("changedattribute");
	valueold			= resultSetProtocol.getString("valueold");
	valuenew			= resultSetProtocol.getString("valuenew");

// workaround BASAR
	if (username.empty())			username = " ";
	if (activitytype.empty())		activitytype = " ";
	if (changedattribute.empty())	changedattribute = " ";
	if (valueold.empty())			valueold = " ";
	if (valuenew.empty())			valuenew = " ";
// workaround BASAR

	stmtInsProtocol.setInt32(0,		newCatalogID);
	stmtInsProtocol.setString(1,	username.c_str());
	stmtInsProtocol.setInt32(2,		protocoldate);
	stmtInsProtocol.setInt32(3,		protocoltime);
	stmtInsProtocol.setString(4,	activitytype.c_str());
	stmtInsProtocol.setString(5,	changedattribute.c_str());
	stmtInsProtocol.setString(6,	valueold.c_str());
	stmtInsProtocol.setString(7,	valuenew.c_str());

	basar::db::sql::ExecuteReturnInfo retInfo = stmtInsProtocol.execute();

	return retInfo;
}

//----------------------------------------------------------------------------
//
//----------------------------------------------------------------------------

int narcPEP(basar::db::sql::ConnectionRef dbConn, const basar::Int16 branchNo, const basar::VarString& fromDB, const basar::VarString& toDB, const basar::Int32 fromDate)
{
	basar::Int32 retval = 0;
	try
	{
		basar::db::sql::ResultsetRef		resultSetCatalog;
		basar::db::sql::ResultsetRef		resultSetProtocol;
		basar::db::sql::ExecuteReturnInfo	retInfo;

		createPreparedStatementsNARC(dbConn, branchNo, fromDB, toDB, fromDate);

//		stmtSelCatalog.setInt16(0, branchNo);

		resultSetCatalog = stmtSelCatalog.executeQuery();
		long counter=0;
		while (resultSetCatalog.next())
		{
			retInfo = insertCatalog(resultSetCatalog);
			if (retInfo.hasError())
			{
				std::stringstream ss;
				ss << "Error while inserting catalogid" << resultSetCatalog.getInt32("catalogid");
				LOG4CPLUS_ERROR(log4cplus::Logger::getRoot(), ss.str());
				retval = -1;
				break;
			}
			if (retInfo.m_lastSerial.begin() == retInfo.m_lastSerial.end())
			{
				std::stringstream ss;
				ss << "Error no last serial for " << resultSetCatalog.getInt32("catalogid");
				LOG4CPLUS_ERROR(log4cplus::Logger::getRoot(), ss.str());
				retval = -1;
				break;
			}      
			basar::Int32 newCatalogID = static_cast<basar::Int32>(*retInfo.m_lastSerial.begin());

			stmtSelProtocol.setInt32(0, resultSetCatalog.getInt32("catalogid"));
			resultSetProtocol = stmtSelProtocol.executeQuery();
			while (resultSetProtocol.next())
			{
				retInfo = insertProtocol(newCatalogID, resultSetProtocol);
				if (retInfo.hasError())
				{
					std::stringstream ss;
					ss << "Error while inserting protocol entry for " << newCatalogID;
					LOG4CPLUS_ERROR(log4cplus::Logger::getRoot(), ss.str());
					retval = -1;
					break;
				}
			}
			if (retval == -1)
			{
				break;
			}

			counter++;
			if ((counter % 10000) == 0)
			{
				std::stringstream ss;
				ss << "Es sind " << counter << " Datensätze übertragen";
				LOG4CPLUS_INFO(log4cplus::Logger::getRoot(), ss.str());
			}
		}
		resetPreparedStatementsNARC();

		std::stringstream ss;
		ss << "Es sind " << counter << " Datensätze übertragen";
		LOG4CPLUS_INFO(log4cplus::Logger::getRoot(), ss.str());
	}
	catch (basar::Exception ex)
	{
		LOG4CPLUS_ERROR(log4cplus::Logger::getRoot(), ex.what());
		return -1;
	}
	catch (bool ex)
	{
		std::stringstream ss;
		ss << "catched a bool ;) " << ex << std::endl;
		LOG4CPLUS_ERROR(log4cplus::Logger::getRoot(), ss.str());
		return -1;
	}
	return retval;
}
