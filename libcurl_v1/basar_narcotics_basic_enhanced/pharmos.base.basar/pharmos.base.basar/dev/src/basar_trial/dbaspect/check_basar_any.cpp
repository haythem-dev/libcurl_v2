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
#include "propertydescriptionlist.h"
#include "propertytable.h"
#include <libbasarproperty.h>
#include <libbasarproperty_propertydescriptionlistref.h>
#include <libbasarproperty_propertytableref.h>

//---------------------------------------------------//
// using declaration
//---------------------------------------------------//
using std::string;
using std::ostream;
using std::ofstream;
using std::ostringstream;
using std::boolalpha;
using basar::property::PropertyType;    
using basar::property::PropertyDescriptionList;
using basar::property::PropertyDescriptionListRef;
using basar::property::ColumnName;
using basar::property::ColumnIndex;
using basar::property::PdlSharedPtr;
using basar::property::PropertyDescriptionList_Iterator;
using basar::property::PropertyTable;    
using basar::property::PropertyTableRef;
using basar::property::PropertyTable_YIterator;
using basar::property::PropertyTable_XIterator;
using basar::property::PropertyStateSet;
using basar::InvalidIteratorException;

void report( std::string fun, std::string testcase, std::string exceptInfo = "", bool ok = true, ostream& strm = std::cout); 
AccessorInstanceRef checkBBAccessorInstanceRef( ConnectionRef connection, CachingPolicyEnum );
AccessorInstanceRef checkBLieSortAccessorInstanceRef( ConnectionRef connection, CachingPolicyEnum );
const ostringstream& getTime();
const ostringstream& getDate();

//--------------------------------------------------------------------------------------------------------//
// void check_Basar_Any( ConnectionRef connection, AccessorInstanceRef air )
//--------------------------------------------------------------------------------------------------------//
void check_Basar_Any( ConnectionRef connection, AccessorInstanceRef air )
{
	static const char* const fun = "check_Basar_Any()";

	//ostream& strm = cout;
	ofstream log ( "aspect.check_basar_any.log" ); ostream& strm = log;
	strm << "====================================================================" << endl;
	strm << fun << " - Entry!" << endl;
	strm << "====================================================================" << endl << endl;

	strm << "Date: " << getDate().str().c_str() << "\tTime: " << getTime().str().c_str() << endl;
	ostringstream os;

	air = checkBLieSortAccessorInstanceRef( connection, basar::db::aspect::FULL_CACHING );
	
	AccessorPropertyTable_YIterator ayit;
	try {
	
		try {
			strm << "1. Clearing PropertyTable." << endl;	
			air.getPropertyTable().clear();
		} catch( basar::Exception& e ){
			report( "", "1. air.getPropertyTable().clear();", e.what(), false, strm );
		} catch( ... ){
			report( "", "1. air.getPropertyTable().clear();", "Exception ...", false, strm );
		}
	
		try {
			strm << "2. Insert new row into the PropertyTable with InsertType 'basar::FOR_CLEAN': ";
			ayit = air.getPropertyTable().insert( basar::FOR_CLEAN );
			strm << "Insert OK." << endl;	
		} catch( const basar::Exception& e ){
			report( "", "2. Insert new row into the PropertyTable with InsertType 'basar::FOR_CLEAN'", e.what(), false, strm );
		}

	} catch( const basar::Exception& e ){
		report( fun, "Basar-EXCEPTION", e.what(), false, strm );
	} catch( ...  ){
		report( fun, "EXCEPTION", "", false, strm );
	}


	strm << "====================================================================" << endl;
	strm << fun << " - Exit!" << endl;
	strm << "====================================================================" << endl;
}