#ifndef ODBCFIX_H
#define ODBCFIX_H

//#define INFX_CSDK_370

//---------------------------------------------------------------
// issue with Informix CSDK 3.70 Windows 32 bit (v2.90 is OK)
class OdbcFix
{
public:
	OdbcFix();
	~OdbcFix();

private:
	OdbcFix & operator = (const OdbcFix &);

#ifdef INFX_CSDK_370
	basar::db::sql::DatabaseInfo dbODBC;
	basar::db::sql::DatabaseInfo dbESQLC;
#endif
};

//---------------------------------------------------------------
#endif	// GUARD
