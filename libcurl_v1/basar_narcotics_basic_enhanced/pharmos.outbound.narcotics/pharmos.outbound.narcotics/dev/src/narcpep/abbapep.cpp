//----------------------------------------------------------------------------
//
//----------------------------------------------------------------------------

#include "libbasarcmnutil.h"
#include "libbasardbsql.h"

//----------------------------------------------------------------------------
//
//----------------------------------------------------------------------------

basar::db::sql::PreparedStatementRef	stmtSelOrderHead;
basar::db::sql::PreparedStatementRef	stmtSelOrderPos;
basar::db::sql::PreparedStatementRef	stmtSelOrderProposal;

basar::db::sql::PreparedStatementRef	stmtInsOrderHead;
basar::db::sql::PreparedStatementRef	stmtInsOrderPos;
basar::db::sql::PreparedStatementRef	stmtInsOrderProposal;
basar::db::sql::PreparedStatementRef	stmtInsOrderHeadProtocol;

basar::db::sql::PreparedStatementRef	stmtUpdBeskopfCscBatch;
basar::db::sql::PreparedStatementRef	stmtUpdPuBatchOrderPos;

//----------------------------------------------------------------------------
//
//----------------------------------------------------------------------------

void resetPreparedStatementsABBA()
{
	stmtSelOrderHead.close();
	stmtSelOrderHead.reset();

	stmtSelOrderPos.close();
	stmtSelOrderPos.reset();

	stmtSelOrderProposal.close();
	stmtSelOrderProposal.reset();

	stmtInsOrderHead.close();
	stmtInsOrderHead.reset();

	stmtInsOrderHeadProtocol.close();
	stmtInsOrderHeadProtocol.reset();

	stmtInsOrderPos.close();
	stmtInsOrderPos.reset();

	stmtInsOrderProposal.close();
	stmtInsOrderProposal.reset();

	stmtUpdBeskopfCscBatch.close();
	stmtUpdBeskopfCscBatch.reset();

	stmtUpdPuBatchOrderPos.close();
	stmtUpdPuBatchOrderPos.reset();
}

//----------------------------------------------------------------------------
//
//----------------------------------------------------------------------------

void createPreparedStatementsABBA(basar::db::sql::ConnectionRef dbConn, const basar::VarString& fromDB, const basar::VarString& toDB, const basar::VarString& inboundDB)
{
	basar::VarString sql;
	
	sql.format(
		"select "
			"branchno, "
			"orderno, "
			"status, "
			"ordertype, "
			"orderpickingtype, "
			"bookentrytype, "
			"orderdate, "
			"ordervalue, "
			"deferredpaymentdate, "
			"containers,narcoticsqty,totalweight,pharmacyno,headertext,deliveryrunreference,deliveryrun, "
			"readoutstatus,deliveryrunsequenceno,customergroup,salesmanno,imsareano,taxtype,orderacquisitiondate, "
			"datasourcetype,recordtype,branchnoorig,customertype,rekbno,mainwarehouseflag,edideliverydate,isdeliverynoteflag, "
			"isedicopyflag,islkmailflag,estorelflashflag,isestorelbulkorderflag,isinvoicedflag,splittedintoqty,reference,ordernocsc, "
			"ordernomf,manufacturerno,edideliverydatemf,pharmacynomf,purchaseorderno "
		"from %s:orderhead "
		"where branchno=? "
		"order by orderno", fromDB.c_str());
	stmtSelOrderHead = dbConn.createPreparedStatement(sql, basar::db::sql::CURS_HOLD_OVER_COMMIT);

	sql.format(
		"select "
			"branchno,orderno,posno,articleno,article_code,orderedqty,nonchargedqty,textfield,ibtrecordtype,selloffriseflag, "
			"customerorderno,customerposno,subsequentdeliveryqty,bookingflag,wholesalerpurchaseprice,subsequentdeliveryflag, "
			"customerordernoadd,estorelflashcode,lkmailshortageqty,discountpct,batch,expirydate "
		"from %s:orderpos "
		"where orderno=?", fromDB.c_str());
	stmtSelOrderPos = dbConn.createPreparedStatement(sql, basar::db::sql::CURS_HOLD_OVER_COMMIT);

	sql.format(
		"select "
			"branchno,articleno,orderproposalqty,ordernobatch,posnobatch,ordertypebatch,reasonid,orderproposaltime,transfertime, "
			"orderproposalprocessedtime,orderproposaldisclaimnote,orderproposalno,purchaseorderno,dateofpurchase,transfertimebi, "
			"expectedgoodsindate,completed "
		"from %s:orderproposal "
		"where ordernobatch=?", fromDB.c_str());
	stmtSelOrderProposal = dbConn.createPreparedStatement(sql, basar::db::sql::CURS_HOLD_OVER_COMMIT);

	sql.format(
		"insert into %s:orderhead ("
			"branchno, "
			"status, "
			"ordertype, "
			"orderpickingtype, "
			"bookentrytype, "
			"orderdate, "
			"ordervalue, "
			"deferredpaymentdate, "
			"containers,narcoticsqty,totalweight,pharmacyno,headertext,deliveryrunreference,deliveryrun, "
			"readoutstatus,deliveryrunsequenceno,customergroup,salesmanno,imsareano,taxtype,orderacquisitiondate, "
			"datasourcetype,recordtype,branchnoorig,customertype,rekbno,mainwarehouseflag,edideliverydate,isdeliverynoteflag, "
			"isedicopyflag,islkmailflag,estorelflashflag,isestorelbulkorderflag,isinvoicedflag,splittedintoqty,reference,ordernocsc, "
			"ordernomf,manufacturerno,edideliverydatemf,pharmacynomf,purchaseorderno "
			" )"
		"values "
			"( "
			" ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, "
			" ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, "
			" ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, "
			" ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, "
			" ?, ?, ?)", toDB.c_str());
	stmtInsOrderHead = dbConn.createPreparedStatement(sql);

	sql.format(
		"insert into %s:orderpos ("
			"branchno,orderno,posno,articleno,article_code,orderedqty,nonchargedqty,textfield,ibtrecordtype,selloffriseflag, "
			"customerorderno,customerposno,subsequentdeliveryqty,bookingflag,wholesalerpurchaseprice,subsequentdeliveryflag, "
			"customerordernoadd,estorelflashcode,lkmailshortageqty,discountpct,batch,expirydate "
			") "
		"values "
			"( "
			" ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, "
			" ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, "
			" ?, ? )", toDB.c_str());
	stmtInsOrderPos = dbConn.createPreparedStatement(sql);

	sql.format(
		"insert into %s:orderproposal ("
			"branchno,articleno,orderproposalqty,ordernobatch,posnobatch,ordertypebatch,reasonid,orderproposaltime,transfertime, "
			"orderproposalprocessedtime,orderproposaldisclaimnote,orderproposalno,purchaseorderno,dateofpurchase,transfertimebi, "
			"expectedgoodsindate,completed "
			") "
		"values "
			"( "
			" ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, "
			" ?, ?, ?, ?, ?, ?, ? )", toDB.c_str());
	stmtInsOrderProposal = dbConn.createPreparedStatement(sql);

	sql.format(
		"insert into %s:orderheadprotocol ("
			"branchno,orderno,activitytype,eventtype,processedby,changedattribute,oldvalue,newvalue "
			") "
		"values "
			"( "
			" ?, ?, ?, ?, ?, ?, ?, ? )", toDB.c_str());
	stmtInsOrderHeadProtocol = dbConn.createPreparedStatement(sql);

	sql.format(
		"update %s:beskopf_cscbatch set "
			"ordernocscbatch = ? "
		"where "
			"branchno = ? and "
			"ordernocscbatch = ? ", inboundDB.c_str());
	stmtUpdBeskopfCscBatch = dbConn.createPreparedStatement(sql);

	sql.format(
		"update %s:pubatchorderpos set "
			"ordernobatch = ? "
		"where "
			"branchno = ? and "
			"ordernobatch = ? ", inboundDB.c_str());
	stmtUpdPuBatchOrderPos = dbConn.createPreparedStatement(sql);
}

//----------------------------------------------------------------------------
//
//----------------------------------------------------------------------------

basar::db::sql::ExecuteReturnInfo insertOrderHead(basar::db::sql::ResultsetRef resultSetOrderHead)
{
	// first insert orderhead and get new serial id
	static basar::Int16			branchno				= 0;
//	static basar::Int32			orderno;
	static basar::VarString		status					= basar::VarString();
	static basar::VarString		ordertype				= basar::VarString();
	static basar::VarString		orderpickingtype		= basar::VarString();
	static basar::VarString		bookentrytype			= basar::VarString();
	static basar::Int32			orderdate				= 0;
	static basar::Int32			ordervalue				= 0;
	static basar::Int32			deferredpaymentdate		= 0;
	static basar::Int16			containers				= 0;
	static basar::Int16			narcoticsqty			= 0;
	static basar::Int32			totalweight				= 0;
	static basar::Int32			pharmacyno				= 0;
	static basar::VarString		headertext				= basar::VarString();
	static basar::VarString		deliveryrunreference	= basar::VarString();
	static basar::VarString		deliveryrun				= basar::VarString();
	static basar::VarString		readoutstatus			= basar::VarString();
	static basar::Int32			deliveryrunsequenceno	= 0;
	static basar::Int16			customergroup			= 0;
	static basar::Int32			salesmanno				= 0;
	static basar::Int32			imsareano				= 0;
	static basar::Int16			taxtype					= 0;
	static basar::Int32			orderacquisitiondate	= 0;
	static basar::VarString		datasourcetype			= basar::VarString();
	static basar::VarString		recordtype				= basar::VarString();
	static basar::Int16			branchnoorig			= 0;
	static basar::VarString		customertype			= basar::VarString();
	static basar::Int32			rekbno					= 0;
	static basar::VarString		mainwarehouseflag		= basar::VarString();
	static basar::Int32			edideliverydate			= 0;
	static basar::VarString		isdeliverynoteflag		= basar::VarString();
	static basar::VarString		isedicopyflag			= basar::VarString();
	static basar::VarString		islkmailflag			= basar::VarString();
	static basar::VarString		estorelflashflag		= basar::VarString();
	static basar::VarString		isestorelbulkorderflag	= basar::VarString();
	static basar::VarString		isinvoicedflag			= basar::VarString();
	static basar::Int16			splittedintoqty			= 0;
	static basar::VarString		reference				= basar::VarString();
	static basar::Int32			ordernocsc				= 0;
	static basar::VarString		ordernomf				= basar::VarString();
	static basar::Int32			manufacturerno			= 0;
	static basar::Int32			edideliverydatemf		= 0;
	static basar::VarString		pharmacynomf			= basar::VarString();
	static basar::Int32			purchaseorderno			= 0;

	branchno				= resultSetOrderHead.getInt16("branchno");
	status					= resultSetOrderHead.getString("status");
	ordertype				= resultSetOrderHead.getString("ordertype");
	orderpickingtype		= resultSetOrderHead.getString("orderpickingtype");
	bookentrytype			= resultSetOrderHead.getString("bookentrytype");
	orderdate				= resultSetOrderHead.getInt32("orderdate");
	ordervalue				= resultSetOrderHead.getInt32("ordervalue");
	deferredpaymentdate		= resultSetOrderHead.getInt32("deferredpaymentdate");
	containers				= resultSetOrderHead.getInt16("containers");
	narcoticsqty			= resultSetOrderHead.getInt16("narcoticsqty");
	totalweight				= resultSetOrderHead.getInt32("totalweight");
	pharmacyno				= resultSetOrderHead.getInt32("pharmacyno");
	headertext				= resultSetOrderHead.getString("headertext");
	deliveryrunreference	= resultSetOrderHead.getString("deliveryrunreference");
	deliveryrun				= resultSetOrderHead.getString("deliveryrun");
	readoutstatus			= resultSetOrderHead.getString("readoutstatus");
	deliveryrunsequenceno	= resultSetOrderHead.getInt32("deliveryrunsequenceno");
	customergroup			= resultSetOrderHead.getInt16("customergroup");
	salesmanno				= resultSetOrderHead.getInt32("salesmanno");
	imsareano				= resultSetOrderHead.getInt32("imsareano");
	taxtype					= resultSetOrderHead.getInt16("taxtype");
	orderacquisitiondate	= resultSetOrderHead.getInt32("orderacquisitiondate");
	datasourcetype			= resultSetOrderHead.getString("datasourcetype");
	recordtype				= resultSetOrderHead.getString("recordtype");
	branchnoorig			= resultSetOrderHead.getInt16("branchnoorig");
	customertype			= resultSetOrderHead.getString("customertype");
	rekbno					= resultSetOrderHead.getInt32("rekbno");
	mainwarehouseflag		= resultSetOrderHead.getString("mainwarehouseflag");
	edideliverydate			= resultSetOrderHead.getInt32("edideliverydate");
	isdeliverynoteflag		= resultSetOrderHead.getString("isdeliverynoteflag");
	isedicopyflag			= resultSetOrderHead.getString("isedicopyflag");
	islkmailflag			= resultSetOrderHead.getString("islkmailflag");
	estorelflashflag		= resultSetOrderHead.getString("estorelflashflag");
	isestorelbulkorderflag	= resultSetOrderHead.getString("isestorelbulkorderflag");
	isinvoicedflag			= resultSetOrderHead.getString("isinvoicedflag");
	splittedintoqty			= resultSetOrderHead.getInt16("splittedintoqty");
	reference				= resultSetOrderHead.getString("reference");
	ordernocsc				= resultSetOrderHead.getInt32("ordernocsc");
	ordernomf				= resultSetOrderHead.getString("ordernomf");
	manufacturerno			= resultSetOrderHead.getInt32("manufacturerno");
	edideliverydatemf		= resultSetOrderHead.getInt32("edideliverydatemf");
	pharmacynomf			= resultSetOrderHead.getString("pharmacynomf");
	purchaseorderno			= resultSetOrderHead.getInt32("purchaseorderno");

	// workaround BASAR
	//if (transactiontype.empty())		transactiontype = " ";
	//if (deletedflag.empty())			deletedflag = " ";
	//if (processedflag.empty())			processedflag = " ";
	//if (customersuppliername.empty())	customersuppliername = " ";
	//if (customersupplierstreet.empty())	customersupplierstreet = " ";
	//if (customersuppliercity.empty())	customersuppliercity = " ";
	//if (narcgroup.empty())				narcgroup = " ";
	//if (narcname.empty())				narcname = " ";
	//if (narcunit.empty())				narcunit = " ";
	//if (username.empty())				username = " ";
	//if (tourid.empty())					tourid = " ";
	// workaround BASAR

	stmtInsOrderHead.setInt16(	0, branchno);
	stmtInsOrderHead.setString(	1, status.c_str());
	stmtInsOrderHead.setString( 2, ordertype.c_str());
	stmtInsOrderHead.setString(	3, orderpickingtype.c_str());
	stmtInsOrderHead.setString(	4, bookentrytype.c_str());
	stmtInsOrderHead.setInt32(	5, orderdate);
	stmtInsOrderHead.setInt32(	6, ordervalue);
	stmtInsOrderHead.setInt32(	7, deferredpaymentdate);
	stmtInsOrderHead.setInt16(	8, containers);
	stmtInsOrderHead.setInt16(	9, narcoticsqty);
	stmtInsOrderHead.setInt32(	10, totalweight);
	stmtInsOrderHead.setInt32(	11, pharmacyno);
	stmtInsOrderHead.setString(	12, headertext.c_str());
	stmtInsOrderHead.setString(	13, deliveryrunreference.c_str());
	stmtInsOrderHead.setString(	14, deliveryrun.c_str());
	stmtInsOrderHead.setString(	15, readoutstatus.c_str());
	stmtInsOrderHead.setInt32(	16, deliveryrunsequenceno);
	stmtInsOrderHead.setInt16(	17, customergroup);
	stmtInsOrderHead.setInt32(	18, salesmanno);
	stmtInsOrderHead.setInt32(	19, imsareano);
	stmtInsOrderHead.setInt16(	20, taxtype);
	stmtInsOrderHead.setInt32(	21, orderacquisitiondate);
	stmtInsOrderHead.setString(	22,	datasourcetype.c_str());
	stmtInsOrderHead.setString(	23, recordtype.c_str());
	stmtInsOrderHead.setInt16(	24,	branchnoorig);
	stmtInsOrderHead.setString(	25, customertype.c_str());
	stmtInsOrderHead.setInt32(	26, rekbno);
	stmtInsOrderHead.setString(	27, mainwarehouseflag.c_str());
	stmtInsOrderHead.setInt32(	28, edideliverydate);
	stmtInsOrderHead.setString(	29, isdeliverynoteflag.c_str());
	stmtInsOrderHead.setString(	30, isedicopyflag.c_str());
	stmtInsOrderHead.setString(	31, islkmailflag.c_str());
	stmtInsOrderHead.setString(	32, estorelflashflag.c_str());
	stmtInsOrderHead.setString(	33, isestorelbulkorderflag.c_str());
	stmtInsOrderHead.setString(	34, isinvoicedflag.c_str());
	stmtInsOrderHead.setInt16(	35, splittedintoqty);
	stmtInsOrderHead.setString(	36, reference.c_str());
	stmtInsOrderHead.setInt32(	37, ordernocsc);
	stmtInsOrderHead.setString(	38, ordernomf.c_str());
	stmtInsOrderHead.setInt32(	39, manufacturerno);
	stmtInsOrderHead.setInt32(	40, edideliverydatemf);
	stmtInsOrderHead.setString(	41, pharmacynomf.c_str());
	stmtInsOrderHead.setInt32(	42, purchaseorderno);

	basar::db::sql::ExecuteReturnInfo retInfo = stmtInsOrderHead.execute();
	return retInfo;
}

//----------------------------------------------------------------------------
//
//----------------------------------------------------------------------------

basar::db::sql::ExecuteReturnInfo insertOrderPos(basar::Int32 newOrderNo, basar::db::sql::ResultsetRef resultSetOrderPos)
{
	static basar::Int16		branchno				= 0;
	static basar::Int32		orderno					= 0;
	static basar::Int32		posno					= 0;
	static basar::Int32		articleno				= 0;
	static basar::VarString	article_code			= basar::VarString();
	static basar::Int32		orderedqty				= 0;
	static basar::Int32		nonchargedqty			= 0;
	static basar::VarString	textfield				= basar::VarString();
	static basar::VarString	ibtrecordtype			= basar::VarString();
	static basar::VarString	selloffriseflag			= basar::VarString();
	static basar::Int32		customerorderno			= 0;
	static basar::Int32		customerposno			= 0;
	static basar::Int32		subsequentdeliveryqty	= 0;
	static basar::VarString	bookingflag				= basar::VarString();
	static basar::Decimal	wholesalerpurchaseprice	= basar::Decimal();
	static basar::VarString	subsequentdeliveryflag	= basar::VarString();
	static basar::Int16		customerordernoadd		= 0;
	static basar::VarString	estorelflashcode		= basar::VarString();
	static basar::Int32		lkmailshortageqty		= 0;
	static basar::Decimal	discountpct				= basar::Decimal();
	static basar::VarString	batch					= basar::VarString();
	static basar::Int32		expirydate				= 0;

	branchno				= resultSetOrderPos.getInt16("branchno");
	orderno					= newOrderNo;
	posno					= resultSetOrderPos.getInt32("posno");
	articleno				= resultSetOrderPos.getInt32("articleno");
	article_code			= resultSetOrderPos.getString("article_code");
	orderedqty				= resultSetOrderPos.getInt32("orderedqty");
	nonchargedqty			= resultSetOrderPos.getInt32("nonchargedqty");
	textfield				= resultSetOrderPos.getString("textfield");
	ibtrecordtype			= resultSetOrderPos.getString("ibtrecordtype");
	selloffriseflag			= resultSetOrderPos.getString("selloffriseflag");
	customerorderno			= resultSetOrderPos.getInt32("customerorderno");
	customerposno			= resultSetOrderPos.getInt32("customerposno");
	subsequentdeliveryqty	= resultSetOrderPos.getInt32("subsequentdeliveryqty");
	bookingflag				= resultSetOrderPos.getString("bookingflag");
	wholesalerpurchaseprice	= resultSetOrderPos.getDecimal("wholesalerpurchaseprice");
	subsequentdeliveryflag	= resultSetOrderPos.getString("subsequentdeliveryflag");
	customerordernoadd		= resultSetOrderPos.getInt16("customerordernoadd");
	estorelflashcode		= resultSetOrderPos.getString("estorelflashcode");
	lkmailshortageqty		= resultSetOrderPos.getInt32("lkmailshortageqty");
	discountpct				= resultSetOrderPos.getDecimal("discountpct");
	batch					= resultSetOrderPos.getString("batch");
	expirydate				= resultSetOrderPos.getInt32("expirydate");

	// workaround BASAR
	//if (username.empty())			username = " ";
	//if (activitytype.empty())		activitytype = " ";
	//if (changedattribute.empty())	changedattribute = " ";
	//if (valueold.empty())			valueold = " ";
	//if (valuenew.empty())			valuenew = " ";
	// workaround BASAR

	stmtInsOrderPos.setInt16(	0,	branchno);
	stmtInsOrderPos.setInt32(	1,	orderno);
	stmtInsOrderPos.setInt32(	2,	posno);
	stmtInsOrderPos.setInt32(	3,	articleno);
	stmtInsOrderPos.setString(	4,	article_code.c_str());
	stmtInsOrderPos.setInt32(	5,	orderedqty);
	stmtInsOrderPos.setInt32(	6,	nonchargedqty);
	stmtInsOrderPos.setString(	7,	textfield.c_str());
	stmtInsOrderPos.setString(	8, 	ibtrecordtype.c_str());
	stmtInsOrderPos.setString(	9,	selloffriseflag.c_str());
	stmtInsOrderPos.setInt32(	10,	customerorderno);
	stmtInsOrderPos.setInt32(	11,	customerposno);
	stmtInsOrderPos.setInt32(	12,	subsequentdeliveryqty);
	stmtInsOrderPos.setString(	13,	bookingflag.c_str());
	stmtInsOrderPos.setDecimal(	14, wholesalerpurchaseprice);
	stmtInsOrderPos.setString(	15,	subsequentdeliveryflag.c_str());
	stmtInsOrderPos.setInt16(	16,	customerordernoadd);
	stmtInsOrderPos.setString(	17,	estorelflashcode.c_str());
	stmtInsOrderPos.setInt32(	18,	lkmailshortageqty);
	stmtInsOrderPos.setDecimal(	19,	discountpct);	
	stmtInsOrderPos.setString(	20,	batch.c_str());
	stmtInsOrderPos.setInt32(	21,	expirydate);

	basar::db::sql::ExecuteReturnInfo retInfo = stmtInsOrderPos.execute();

	return retInfo;
}

//----------------------------------------------------------------------------
//
//----------------------------------------------------------------------------

basar::db::sql::ExecuteReturnInfo insertOrderProposal(basar::Int32 newOrderNo, basar::db::sql::ResultsetRef resultSetOrderProposal)
{
	static basar::Int16		branchno					= 0;
	static basar::Int32		articleno					= 0;
	static basar::Int32		orderproposalqty			= 0;
	static basar::Int32		ordernobatch				= 0;
	static basar::Int32		posnobatch					= 0;
	static basar::Int16		ordertypebatch				= 0;
	static basar::Int16		reasonid					= 0;
	static basar::DateTime	orderproposaltime			= basar::DateTime();
	static basar::DateTime	transfertime				= basar::DateTime();
	static basar::DateTime	orderproposalprocessedtime	= basar::DateTime();
	static basar::VarString	orderproposaldisclaimnote	= basar::VarString();
	static basar::Int32		orderproposalno				= 0;
	static basar::Int32		purchaseorderno				= 0;
	static basar::Int32		dateofpurchase				= 0;
	static basar::DateTime	transfertimebi				= basar::DateTime();
	static basar::Int32		expectedgoodsindate			= 0;
	static basar::Int16		completed					= 0;

	branchno					= resultSetOrderProposal.getInt16("branchno");
	articleno					= resultSetOrderProposal.getInt32("articleno");
	orderproposalqty			= resultSetOrderProposal.getInt32("orderproposalqty");
	ordernobatch				= newOrderNo;
	posnobatch					= resultSetOrderProposal.getInt32("posnobatch");
	ordertypebatch				= resultSetOrderProposal.getInt16("ordertypebatch");
	reasonid					= resultSetOrderProposal.getInt16("reasonid");
	orderproposaltime			= resultSetOrderProposal.getDateTime("orderproposaltime");
	transfertime				= resultSetOrderProposal.getDateTime("transfertime");
	orderproposalprocessedtime	= resultSetOrderProposal.getDateTime("orderproposalprocessedtime");
	orderproposaldisclaimnote	= resultSetOrderProposal.getString("orderproposaldisclaimnote");
	orderproposalno				= resultSetOrderProposal.getInt32("orderproposalno");
	purchaseorderno				= resultSetOrderProposal.getInt32("purchaseorderno");
	dateofpurchase				= resultSetOrderProposal.getInt32("dateofpurchase");
	transfertimebi				= resultSetOrderProposal.getDateTime("transfertimebi");
	expectedgoodsindate			= resultSetOrderProposal.getInt32("expectedgoodsindate");
	completed					= resultSetOrderProposal.getInt16("completed");

	stmtInsOrderProposal.setInt16(		0,	branchno);
	stmtInsOrderProposal.setInt32(		1,	articleno);
	stmtInsOrderProposal.setInt32(		2,	orderproposalqty);
	stmtInsOrderProposal.setInt32(		3,	ordernobatch);
	stmtInsOrderProposal.setInt32(		4,	posnobatch);
	stmtInsOrderProposal.setInt16(		5,	ordertypebatch);
	stmtInsOrderProposal.setInt16(		6,	reasonid);
	stmtInsOrderProposal.setDateTime(	7,	orderproposaltime);
	stmtInsOrderProposal.setDateTime(	8, 	transfertime);
	stmtInsOrderProposal.setDateTime(	9,	orderproposalprocessedtime);
	stmtInsOrderProposal.setString(		10,	orderproposaldisclaimnote.c_str());
	stmtInsOrderProposal.setInt32(		11,	orderproposalno);
	stmtInsOrderProposal.setInt32(		12,	purchaseorderno);
	stmtInsOrderProposal.setInt32(		13,	dateofpurchase);
	stmtInsOrderProposal.setDateTime(	14, transfertimebi);
	stmtInsOrderProposal.setInt32(		15,	expectedgoodsindate);
	stmtInsOrderProposal.setInt16(		16,	completed);

	basar::db::sql::ExecuteReturnInfo retInfo = stmtInsOrderProposal.execute();

	return retInfo;
}

//----------------------------------------------------------------------------
//
//----------------------------------------------------------------------------

basar::db::sql::ExecuteReturnInfo insertOrderHeadProtocol(basar::Int32 newOrderNo, basar::db::sql::ResultsetRef resultSetOrderHead)
{
	static basar::Int16		branchno			= 0;
	static basar::Int32		orderno				= 0;
	static basar::VarString	activitytype		= basar::VarString();
	static basar::Int16		eventtype			= 0;
	static basar::Int16		processedby			= 0;
	static basar::VarString	changedattribute	= basar::VarString();
	static basar::VarString	oldvalue			= basar::VarString();
	static basar::VarString	newvalue			= basar::VarString();

	branchno			= resultSetOrderHead.getInt16("branchno");
	orderno				= newOrderNo;
	activitytype		= "M";
	eventtype			= 9;	// "Move order"
	processedby			= 4;	// "abbapep"
	changedattribute	= "orderno";
	oldvalue.itos(resultSetOrderHead.getInt32("orderno"));
	newvalue.itos(newOrderNo);

	stmtInsOrderHeadProtocol.setInt16(0, branchno);
	stmtInsOrderHeadProtocol.setInt32(1, orderno);
	stmtInsOrderHeadProtocol.setString(2, activitytype.c_str());
	stmtInsOrderHeadProtocol.setInt16(3, eventtype);
	stmtInsOrderHeadProtocol.setInt16(4, processedby);
	stmtInsOrderHeadProtocol.setString(5, changedattribute.c_str());
	stmtInsOrderHeadProtocol.setString(6, oldvalue.c_str());
	stmtInsOrderHeadProtocol.setString(7, newvalue.c_str());

	basar::db::sql::ExecuteReturnInfo retInfo = stmtInsOrderHeadProtocol.execute();

	return retInfo;
}

//----------------------------------------------------------------------------
//
//----------------------------------------------------------------------------

basar::db::sql::ExecuteReturnInfo updateBeskopf_CscBatch(basar::Int32 newOrderNo, basar::db::sql::ResultsetRef resultSetOrderHead)
{
	static basar::Int32	ordernonew	= 0;
	static basar::Int16	branchno	= 0;	
	static basar::Int32	ordernoold	= 0;

	ordernonew	= newOrderNo;
	branchno	= resultSetOrderHead.getInt16("branchno");
	ordernoold	= resultSetOrderHead.getInt32("orderno");

	stmtUpdBeskopfCscBatch.setInt32(0, ordernonew);
	stmtUpdBeskopfCscBatch.setInt16(1, branchno);
	stmtUpdBeskopfCscBatch.setInt32(2, ordernoold);

	basar::db::sql::ExecuteReturnInfo retInfo = stmtUpdBeskopfCscBatch.execute();
	return retInfo;
}

//----------------------------------------------------------------------------
//
//----------------------------------------------------------------------------

basar::db::sql::ExecuteReturnInfo updatePuBatchOrderPos(basar::Int32 newOrderNo, basar::db::sql::ResultsetRef resultSetOrderHead)
{
	static basar::Int32	ordernonew	= 0;
	static basar::Int16	branchno	= 0;	
	static basar::Int32	ordernoold	= 0;

	ordernonew	= newOrderNo;
	branchno	= resultSetOrderHead.getInt16("branchno");
	ordernoold	= resultSetOrderHead.getInt32("orderno");

	stmtUpdPuBatchOrderPos.setInt32(0, ordernonew);
	stmtUpdPuBatchOrderPos.setInt16(1, branchno);
	stmtUpdPuBatchOrderPos.setInt32(2, ordernoold);

	basar::db::sql::ExecuteReturnInfo retInfo = stmtUpdPuBatchOrderPos.execute();
	return retInfo;
}

//----------------------------------------------------------------------------
//
//----------------------------------------------------------------------------

int abbaPEP(basar::db::sql::ConnectionRef dbConn, const basar::Int16 branchNo, const basar::VarString& fromDB, const basar::VarString& toDB, const basar::VarString& inboundDB)
{
	basar::Int32 retval = 0;
	try
	{
		basar::db::sql::ResultsetRef		resultSetOrderHead;
		basar::db::sql::ResultsetRef		resultSetOrderPos;
		basar::db::sql::ResultsetRef		resultSetOrderProposal;
		basar::db::sql::ExecuteReturnInfo	retInfo;

		createPreparedStatementsABBA(dbConn, fromDB, toDB, inboundDB);

		stmtSelOrderHead.setInt16(0, branchNo);

		resultSetOrderHead = stmtSelOrderHead.executeQuery();
		long counter=0;
		while (resultSetOrderHead.next())
		{
			retInfo = insertOrderHead(resultSetOrderHead);
			if (retInfo.hasError())
			{
				std::stringstream ss;
				ss << "Error while inserting orderno" << resultSetOrderHead.getInt32("orderno");
				LOG4CPLUS_ERROR(log4cplus::Logger::getRoot(), ss.str());
				retval = -1;
				break;
			}
			if (retInfo.m_lastSerial.begin() == retInfo.m_lastSerial.end())
			{
				std::stringstream ss;
				ss << "Error no last serial for " << resultSetOrderHead.getInt32("orderno");
				LOG4CPLUS_ERROR(log4cplus::Logger::getRoot(), ss.str());
				retval = -1;
				break;
			}      
			basar::Int32 newOrderNo = static_cast<basar::Int32>(*retInfo.m_lastSerial.begin());

			stmtSelOrderPos.setInt32(0, resultSetOrderHead.getInt32("orderno"));
			resultSetOrderPos = stmtSelOrderPos.executeQuery();
			while (resultSetOrderPos.next())
			{
				retInfo = insertOrderPos(newOrderNo, resultSetOrderPos);
				if (retInfo.hasError())
				{
					std::stringstream ss;
					ss << "Error while inserting orderpos for order " << newOrderNo;
					LOG4CPLUS_ERROR(log4cplus::Logger::getRoot(), ss.str());
					retval = -1;
					break;
				}
			}

			stmtSelOrderProposal.setInt32(0, resultSetOrderHead.getInt32("orderno"));
			resultSetOrderProposal = stmtSelOrderProposal.executeQuery();
			while (resultSetOrderProposal.next())
			{
				retInfo = insertOrderProposal(newOrderNo, resultSetOrderProposal);
				if (retInfo.hasError())
				{
					std::stringstream ss;
					ss << "Error while inserting orderproposal for order " << newOrderNo;
					LOG4CPLUS_ERROR(log4cplus::Logger::getRoot(), ss.str());
					retval = -1;
					break;
				}
			}
			if (retval == -1)
			{
				break;
			}

			updateBeskopf_CscBatch(newOrderNo, resultSetOrderHead);
			updatePuBatchOrderPos(newOrderNo, resultSetOrderHead);
			insertOrderHeadProtocol(newOrderNo, resultSetOrderHead);

			counter++;
			if ((counter % 10000) == 0)
			{
				std::stringstream ss;
				ss << "Es sind " << counter << " Datensätze übertragen";
				LOG4CPLUS_INFO(log4cplus::Logger::getRoot(), ss.str());
			}
		}
		resetPreparedStatementsABBA();

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
