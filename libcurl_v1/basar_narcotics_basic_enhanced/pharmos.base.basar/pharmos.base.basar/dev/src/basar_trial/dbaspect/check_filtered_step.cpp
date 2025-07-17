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

void					report(	std::string fun, std::string testcase, std::string exceptInfo = "", 
								bool ok = true,	ostream& strm = std::cout); 
AccessorInstanceRef		checkBBAccessorInstanceRef( ConnectionRef connection, CachingPolicyEnum );
void					printHits( AccessorInstanceRef air, ostream& strm );
const ostringstream&	getTime();
const ostringstream&	getDate();

//--------------------------------------------------------------------------------------------------------//
// void check_filtered_step( ConnectionRef connection, AccessorInstanceRef air )
//--------------------------------------------------------------------------------------------------------//
void check_filtered_step( ConnectionRef connection, AccessorInstanceRef air )
{
	static const char* const fun = "check_filtered_step( ConnectionRef, AccessorInstanceRef)";

	//ostream& strm = cout;
	ofstream log ( "aspect.check_filtered_step.log" ); ostream& strm = log;
	strm << "====================================================================" << endl;
	strm << fun << " - Entry!" << endl;
	strm << "====================================================================" << endl << endl;

	strm << "Date: " << getDate().str().c_str() << "\tTime: " << getTime().str().c_str() << endl;

	ostringstream os;
	
	////////////////////////////////////////////////////////////////////////////////////
	// full caching
	////////////////////////////////////////////////////////////////////////////////////
	strm	<< endl << "-----------------------------------------------------------" << endl
			<< "caching policy: "
			<< "FULL_CACHING: "
			<< "Query with hits!" << endl
			<< "-----------------------------------------------------------" << endl;
	try {
		air = checkBBAccessorInstanceRef( connection, basar::db::aspect::FULL_CACHING );			
		report( "", "checkBBAccessorInstanceRef( connection, basar::db::aspect::FULL_CACHING );", "", true, strm );
	} catch( const basar::Exception& e ){
		report( "", "checkBBAccessorInstanceRef( connection, basar::db::aspect::FULL_CACHING );", e.what(), false, strm );
	}
	
	
	////////////////////////////////////////////////////////////////////////////////////
	// execute
	////////////////////////////////////////////////////////////////////////////////////
	try {
		air.getPropertyTable().clear();
		basar::I18nString s("artikel_nr = 1057;" );
		basar::db::aspect::AccessorPropertyTable_YIterator yit = basar::db::aspect::Manager::getInstance().createAccessorYIterator(s);
		air.execute("ReadArticle", yit);
		report( "", "air.execute(\"ReadArticle\");", "", true, strm );
	} catch( const basar::Exception& e ){
		report( "", "air.execute(\"ReadArticle\");", e.what(), false, strm );
	} catch( ... ){
		report( "", "Exception: ...", "", false, strm );
	}
	
	////////////////////////////////////////////////////////////////////////////////////
	// printing hits
	////////////////////////////////////////////////////////////////////////////////////
	try {
		report( "", "Treffer ausgeben (vor Aenderung): ", "", true, strm );
		printHits( air, strm );
	} catch( const basar::Exception& e ){
		report( "", "air.execute(\"ReadArticle\");", e.what(), false, strm );
	}
	
	////////////////////////////////////////////////////////////////////////////////////
	// 
	////////////////////////////////////////////////////////////////////////////////////
	AccessorPropertyTable_YIterator yit = air.getPropertyTable().insert( basar::FOR_DELETE );
	yit.setString( "artikel_name", "neuer Artikelname" );
	strm << "yit.getState().toString(): " << yit.getState().toString().c_str() << ";" << endl;

	yit = air.getPropertyTable().begin( basar::SS_DELETE ); 
	basar::Int32 idx = yit.getRowNumber();

	////////////////////////////////////////////////////////////////////////////////////
	// change values and states of the first and third row
	////////////////////////////////////////////////////////////////////////////////////
	/*try {
		air.getPropertyTable().begin().setInt32( "artikel_nr", 1111111 );
		report( "", "air.getPropertyTable().begin().setInt32( \"artikel_nr\", 1111111 );", "", true, strm );
	} catch( const basar::Exception& e ){
		report( "", "air.getPropertyTable().begin().setInt32( \"artikel_nr\", 1111111 );", e.what(), false, strm );
	}
	try {
		(++++(air.getPropertyTable().begin())).setInt32( "artikel_nr", 3333333 );
		report( "", "(air.getPropertyTable().begin())++++.setInt32( \"artikel_nr\", 3333333 );", "", true, strm );
	} catch( const basar::Exception& e ){
		report( "", "(air.getPropertyTable().begin())++++.setInt32( \"artikel_nr\", 3333333 );", e.what(), false, strm );
	}*/

	////////////////////////////////////////////////////////////////////////////////////
	// printing hits after change
	////////////////////////////////////////////////////////////////////////////////////
	/*try {
		report( "", "Treffer ausgeben (nach Aenderung): ", "", true, strm );
		printHits( air );
	} catch( const basar::Exception& e ){
		report( "", "air.execute(\"ReadArticle\");", e.what(), false, strm );
	}*/

	////////////////////////////////////////////////////////////////////////////////////
	// printing hits (filtered iteration: basar::SS_UPDATE)
	////////////////////////////////////////////////////////////////////////////////////
	/*try {
		report( "", "Filtered Iteration (basar::SS_UPDATE): ", "", true, strm );
		AccessorPropertyTable_YIterator yit = air.getPropertyTable().begin( basar::SS_UPDATE );
		for( ; yit != air.getPropertyTable().end(); ++yit ){
			strm << "yit.getState().toString(): " << yit.getState().toString().c_str() << ";" << endl;
			strm << "artikel_nr:   " << yit.getInt32 (	"artikel_nr"	) << endl;		
			strm << "artikel_name: " << yit.getString(	"artikel_name"	) << endl;
			strm << "filialnr:     " << yit.getInt16 (	"filialnr"		) << endl;
			strm << "bestand:      " << yit.getInt32 (	"bestand"		) << endl << endl;
		}
	} catch( const basar::Exception& e ){
		report( "", "Filtered Iteration (basar::SS_UPDATE): ", e.what(), false, strm );
	}*/


	////////////////////////////////////////////////////////////////////////////////////
	// change values and states of the second row
	////////////////////////////////////////////////////////////////////////////////////
	try {
		(++(air.getPropertyTable().begin())).setInt32( "artikel_nr", 2222222 );
		report( "", "(++(air.getPropertyTable().begin()).setInt32( \"artikel_nr\", 2222222 );", "", true, strm );
	} catch( const basar::Exception& e ){
		report( "", "(++(air.getPropertyTable().begin()).setInt32( \"artikel_nr\", 2222222 );;", e.what(), false, strm );
	}
	try {
		report( "", "(++(air.getPropertyTable().begin())).getState().toString().c_str();", "", true, strm );
		strm << (++(air.getPropertyTable().begin())).getState().toString().c_str() << endl;
	} catch( const basar::Exception& e ){
		report( "", "(++(air.getPropertyTable().begin())).getState().toString().c_str();", e.what(), false, strm );
	}

	////////////////////////////////////////////////////////////////////////////////////
	// printing hits (filtered iteration: basar::SS_UPDATE)
	////////////////////////////////////////////////////////////////////////////////////
	try {
		report( "", "Filtered Iteration (basar::SS_UPDATE): ", "", true, strm );
		AccessorPropertyTable_YIterator yit = air.getPropertyTable().begin( basar::SS_UPDATE );
		for( ; yit != air.getPropertyTable().end(); ++yit ){
			strm << "yit.getState().toString(): " << yit.getState().toString().c_str() << ";" << endl;
			strm << "artikel_nr:   " << yit.getInt32 (	"artikel_nr"	) << endl;		
			strm << "artikel_name: " << yit.getString(	"artikel_name"	) << endl;
			strm << "filialnr:     " << yit.getInt16 (	"filialnr"		) << endl;
			strm << "bestand:      " << yit.getInt32 (	"bestand"		) << endl << endl;
		}
	} catch( const basar::Exception& e ){
		report( "", "Filtered Iteration (basar::SS_UPDATE): ", e.what(), false, strm );
	}

	////////////////////////////////////////////////////////////////////////////////////
	// change values and states of the second row
	////////////////////////////////////////////////////////////////////////////////////
	/*try {
		air.getPropertyTable().markForDelete( ++(air.getPropertyTable().begin()) );
		report( "", "air.getPropertyTable().markForDelete( ++(air.getPropertyTable().begin()) );", "", true, strm );
	} catch( const basar::Exception& e ){
		report( "", "air.getPropertyTable().markForDelete( ++(air.getPropertyTable().begin()) );", e.what(), false, strm );
	}*/

	////////////////////////////////////////////////////////////////////////////////////
	// printing hits (filtered iteration: basar::SS_DELETE)
	////////////////////////////////////////////////////////////////////////////////////
	/*try {
		report( "", "Filtered Iteration (basar::SS_DELETE): ", "", true, strm );
		AccessorPropertyTable_YIterator yit = air.getPropertyTable().begin( basar::SS_DELETE );
		for( ; yit != air.getPropertyTable().end(); ++yit ){
			strm << "yit.getState().toString(): " << yit.getState().toString().c_str() << ";" << endl;
			strm << "artikel_nr:   " << yit.getInt32 (	"artikel_nr"	) << endl;		
			strm << "artikel_name: " << yit.getString(	"artikel_name"	) << endl;
			strm << "filialnr:     " << yit.getInt16 (	"filialnr"		) << endl;
			strm << "bestand:      " << yit.getInt32 (	"bestand"		) << endl << endl;
		}
	} catch( const basar::Exception& e ){
		report( "", "Filtered Iteration (basar::SS_DELETE): ", e.what(), false, strm );
	}*/


	strm << "====================================================================" << endl;
	strm << fun << " - Exit!" << endl;
	strm << "====================================================================" << endl;
}
//--------------------------------------------------------------------------------------------------------//
// void printHits( AccessorInstanceRef air )
//--------------------------------------------------------------------------------------------------------//
//void printHits( AccessorInstanceRef air, ostream& strm )
//{
//	static const char* fun = "printHits( AccessorInstanceRef )";
//	try {
//		AccessorPropertyTable_YIterator yit = air.getPropertyTable().begin();
//		for( ; yit != air.getPropertyTable().end(); ++yit ){
//			strm << "yit.getState().toString(): " << yit.getState().toString().c_str() << ";" << endl;
//			strm << "artikel_nr:   " << yit.getInt32 (	"artikel_nr"	) << endl;		
//			strm << "artikel_name: " << yit.getString(	"artikel_name"	) << endl;
//			strm << "filialnr:     " << yit.getInt16 (	"filialnr"		) << endl;
//			strm << "bestand:      " << yit.getInt32 (	"bestand"		) << endl << endl;
//		}
//	} catch( const basar::Exception& e ){
//		report( "", fun, e.what(), false, strm );
//	}
//}