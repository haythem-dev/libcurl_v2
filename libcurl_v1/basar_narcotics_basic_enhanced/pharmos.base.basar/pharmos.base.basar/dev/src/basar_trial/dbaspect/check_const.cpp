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

#include "classdefinition.h"

using std::string;
using std::ostream;
using std::ofstream;
using std::ostringstream;
using std::boolalpha;

void report(	std::string fun, std::string testcase, std::string exceptInfo = "", 
				bool ok = true,	ostream& strm = std::cout); 
AccessorInstanceRef checkBBAccessorInstanceRef( ConnectionRef connection, CachingPolicyEnum );
const ostringstream& getTime();
const ostringstream& getDate();

//--------------------------------------------------------------------------------------------------------//
// void check_const( ConnectionRef connection, AccessorInstanceRef air )
//--------------------------------------------------------------------------------------------------------//
void check_const( ConnectionRef connection, AccessorInstanceRef air )
{
	static const char* const fun = "check_const( ConnectionRef, AccessorInstanceRef )";

	//ostream& strm = cout;
	ofstream log ( "aspect.check_const.log" ); ostream& strm = log;
	strm << "====================================================================" << endl;
	strm << fun << " - Entry!" << endl;
	strm << "====================================================================" << endl << endl;

	strm << "Date: " << getDate().str().c_str() << "\tTime: " << getTime().str().c_str() << endl;

	ostringstream os;
	
	//basar::db::aspect::Manager::->isOpen( const basar::db::sql::ConnectionList_Iterator it ) const;

	strm << "====================================================================" << endl;
	strm << fun << " - Exit!" << endl;
	strm << "====================================================================" << endl;
}