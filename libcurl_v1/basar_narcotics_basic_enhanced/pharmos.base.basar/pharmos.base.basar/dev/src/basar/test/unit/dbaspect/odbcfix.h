#ifndef ODBCFIX_H
#define ODBCFIX_H

//---------------------------------------------------------------
// issue with Informix CSDK 3.70 Windows 32 bit (v2.90 is OK)
class OdbcFix
{
public:
	OdbcFix();
	~OdbcFix();

private:
	OdbcFix & operator = (const OdbcFix &);
};

//---------------------------------------------------------------
#endif	// GUARD
