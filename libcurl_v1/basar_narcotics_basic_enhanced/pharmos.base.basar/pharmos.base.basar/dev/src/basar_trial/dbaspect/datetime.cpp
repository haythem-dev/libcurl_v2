//--------------------------------------------------------------------------------------------------------//
// includes
//--------------------------------------------------------------------------------------------------------//
#include <stdio.h>
#include <string>
#include "usings.h"
#include <iostream>
#include <iomanip>
#include <fstream>
#include <sstream>
#include <ctime>


using std::string;
using std::ostream;
using std::ofstream;
using std::ostringstream;
using std::boolalpha;
using std::setw;
using std::setfill;


void report( std::string fun, std::string testcase, std::string exceptInfo = "", bool ok = true, ostream& strm = std::cout); 
AccessorInstanceRef checkBBAccessorInstanceRef( ConnectionRef connection, CachingPolicyEnum );
//--------------------------------------------------------------------------------------------------------//
// const ostringstream& getTime()
//--------------------------------------------------------------------------------------------------------//
const ostringstream& getTime()
{
	struct tm *pTm;
	time_t tSec = time(NULL);
	pTm = localtime( &tSec );	

	static std::ostringstream os;
	os.str("");
	os	<< setw(2) << setfill('0') << pTm->tm_hour
		<< ":"
		<< setw(2) << setfill('0') << pTm->tm_min
		<< ":"
	    << setw(2) << setfill('0') << pTm->tm_sec;	
	return os;
}
//--------------------------------------------------------------------------------------------------------//
// const ostringstream& getDate()
//--------------------------------------------------------------------------------------------------------//
const ostringstream& getDate()
{
	struct tm *pTm;
	time_t tSec = time(NULL);
	pTm = localtime( &tSec );	

	static std::ostringstream os;
	os.str("");
	os	<< setw(2) << setfill('0') << pTm->tm_mday
		<< "."
		<< setw(2) << setfill('0') << (pTm->tm_mon+1) 
		<< "."
		<< setw(2) << setfill('0') << (pTm->tm_year+1900)%100;
	return os;
}