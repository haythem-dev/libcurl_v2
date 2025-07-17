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
void printYIterator( AccessorPropertyTable_YIterator yit, const string& s= "", ostream& strm = cout );

//--------------------------------------------------------------------------------------------------------//
// void check_FilteredIteration( ConnectionRef connection, AccessorInstanceRef air )
//--------------------------------------------------------------------------------------------------------//
void check_YIterator_XIterator( ConnectionRef connection, AccessorInstanceRef air )
{
	static const char* const fun = "check_YIterator_XIterator( ConnectionRef, AccessorInstanceRef)";

	//ostream& strm = cout;
	ofstream log ( "aspect.check_yiterator_xiterator.log" ); ostream& strm = log;
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
	

	{	////////////////////////////////////////////////////////////////////////////////////
		strm << "- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - " << endl;
		strm	<< "--> 1.test case: " 
				<< endl << "a) dflt. construction" 
				<< endl << "b) following pre-incrementing" 
				<< endl << "c) printing yiterator (access to values)!" << endl;
		strm << "- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - " << endl;
		////////////////////////////////////////////////////////////////////////////////////
		AccessorPropertyTable_YIterator yit1;
		try {
			report( "", "AccessorPropertyTable_YIterator yit1;", "", true, strm );
			++yit1;
			report( "", "++yit1;", "", true, strm );
		} catch( const basar::Exception& e ){
			report( "", "++yit1;", e.what(), false, strm );
		}
		////////////////////////////////////////////////////////////////////////////////////
		// yiterator state and direct access
		////////////////////////////////////////////////////////////////////////////////////
		printYIterator( yit1, "yit1", strm );
	}

	////////////////////////////////////////////////////////////////////////////////////
	strm << "- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - " << endl;
	strm	<< "--> 2.test case: " 
			<< endl << "a) dflt. construction" 
			<< endl << "b) state output" 
			<< endl << "c) filter output" 
			<< endl << "d) copy c'tor!" 
			<< endl << "e) printing yiterator (access to values)!"
			<< endl;
	strm << "- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - " << endl;
	////////////////////////////////////////////////////////////////////////////////////
	{
		AccessorPropertyTable_YIterator yit2;
		try {
			strm << "yit2.getState().toString(): " << yit2.getState().toString().c_str() << ";" << endl;
		} catch( const basar::Exception& e ){
			report( "", "yit2.getState().toString();", e.what(), false, strm );
		}	

		try {
			strm << "yit2.getFilter().toString(): " << yit2.getFilter().toString().c_str() << ";" << endl;
		} catch( const basar::Exception& e ){
			report( "", "yit2.getFilter().toString()", e.what(), false, strm );
		}

		AccessorPropertyTable_YIterator yit3( yit2 );
		try {
			report( "", "AccessorPropertyTable_YIterator yit3( yit2 );", "", true, strm );
		} catch( const basar::Exception& e ){
			report( "", "AccessorPropertyTable_YIterator yit3( yit2 );", e.what(), false, strm );
		}
		printYIterator( yit3, "yit3", strm );
	}
	
	////////////////////////////////////////////////////////////////////////////////////
	strm << "- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - " << endl;
	strm	<< "--> 3.test case: "
			<< endl << "a) construction via dflt. begin()"
			<< endl << "b) copy c'tor"
			<< endl << "c) new iterator (dflt. c'tor) following an assignment!"
			<< endl << "d) printing yiterator (access to values)"
			<< endl << "e) manipulate an iterator value"
			<< endl << "f) state output"
			<< endl << "g) filter output"
			<< endl << "g) assignment of manipulated e)-iter!"
			<< endl;
	strm << "- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - " << endl;
	////////////////////////////////////////////////////////////////////////////////////
	try {
		// yit
		AccessorPropertyTable_YIterator yit4 = air.getPropertyTable().begin();
		report( "", "yit4 = air.getPropertyTable().begin();", "", true, strm );
		printYIterator( yit4, "yit4", strm );

		// yit2
		AccessorPropertyTable_YIterator yit5( yit4 );
		report( "", "AccessorPropertyTable_YIterator yit5( yit4 );", "", true, strm );
		printYIterator( yit5, "yit5", strm );


		AccessorPropertyTable_YIterator yit6;
		yit6 = yit5;
		report( "", "yit6 = yit5;", "", true, strm );
		
		printYIterator( yit6, "yit6", strm );

		//-------------------------------------------------------------------------------------------------------//
		// yit6 will be set the same value it has
		long l = yit6.getInt32( "artikel_nr");
		report( "", "long l = yit6.getInt32( \"artikel_nr\" );", "", true, strm );
		try {
			strm << "yit6.getState().toString(): " << yit6.getState().toString().c_str() << ";" << endl;
		} catch( const basar::Exception& e ){
			report( "", "yit6.getState().toString()", e.what(), false, strm );
		}
		try {
			strm << "yit6.getFilter().toString(): " << yit6.getFilter().toString().c_str() << ";" << endl;
		} catch( const basar::Exception& e ){
			report( "", "yit6.getFilter().toString()", e.what(), false, strm );
		}

		yit6.setInt32( "artikel_nr", l );
		report( "", "yit6.setInt32( \"artikel_nr\", l );", "", true, strm );
		try {
			strm << "yit6.getState().toString(): " << yit6.getState().toString().c_str() << ";" << endl;
		} catch( const basar::Exception& e ){
			report( "", "yit6.getState().toString()", e.what(), false, strm );
		}
		try {
			strm << "yit6.getFilter().toString(): " << yit6.getFilter().toString().c_str() << ";" << endl;
		} catch( const basar::Exception& e ){
			report( "", "yit6.getFilter().toString()", e.what(), false, strm );
		}
		//-------------------------------------------------------------------------------------------------------//

		// yit is changed: set twice the same value
		yit6.setInt32( "artikel_nr", 1234567 );
		report( "", "yit6.setInt32( \"artikel_nr\", 1234567 );", "", true, strm );
		
		try {
			strm << "yit6.getState().toString(): " << yit6.getState().toString().c_str() << ";" << endl;
		} catch( const basar::Exception& e ){
			report( "", "yit6.getState().toString()", e.what(), false, strm );
		}
		try {
			strm << "yit6.getFilter().toString(): " << yit6.getFilter().toString().c_str() << ";" << endl;
		} catch( const basar::Exception& e ){
			report( "", "yit6.getFilter().toString()", e.what(), false, strm );
		}

		os.str("");
		os << yit6.getInt32( "artikel_nr");
		report( "", "yit6.getInt32( \"artikel_nr\");", os.str().c_str(), true, strm );
		
		yit6.setInt32( "artikel_nr", 1234567 );
		report( "", "yit6.setInt32( \"artikel_nr\", 1234567 );", "", true, strm );
		
		try {
			strm << "yit6.getState().toString(): " << yit6.getState().toString().c_str() << ";" << endl;
		} catch( const basar::Exception& e ){
			report( "", "yit6.getState().toString()", e.what(), false, strm );
		}
		try {
			strm << "yit6.getFilter().toString(): " << yit6.getFilter().toString().c_str() << ";" << endl;
		} catch( const basar::Exception& e ){
			report( "", "yit6.getFilter().toString()", e.what(), false, strm );
		}
		os.str("");
		os << yit6.getInt32( "artikel_nr");
		report( "", "yit6.getInt32( \"artikel_nr\");", os.str().c_str(), true, strm );
		//-------------------------------------------------------------------------------------------------------//

		AccessorPropertyTable_YIterator yit7;
		yit7 = yit6;
		report( "", "yit7 = yit6;", "", true, strm );
		printYIterator( yit7, "yit7", strm );
	
	} catch( const basar::Exception& e ){
		report( "", "AccessorPropertyTable_YIterator 4 - 7;", e.what(), false, strm );
	}

	////////////////////////////////////////////////////////////////////////////////////
	strm << "- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - " << endl;
	strm	<< "--> 4.test case: " 
			<< endl << "a) construction via two different states" 
			<< endl << "b) state output" 
			<< endl << "c) filter output" 
			<< endl << "d) dflr. c'tor" 
			<< endl << "e) assignment of the two-states-iterator to a dflt.-instantiated-c'tor" 
			<< endl << "f) state output"
			<< endl << "g) filter output"
			<< endl << "h) instantiate an iterator with another one and an propertystateset" 
			<< endl << "i) assignment of the iterator to a dflt. instantiated one"
			<< endl << "j) check equality of iterators!"
			<< endl;
	strm << "- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - " << endl;
	////////////////////////////////////////////////////////////////////////////////////
	try {
		/*AccessorPropertyTable_YIterator yit8( 
			static_cast<basar::SupportedStateEnum>(basar::SS_UPDATE | basar::SS_INSERT) );*/
		AccessorPropertyTable_YIterator yit8( basar::SS_UPDATE | basar::SS_INSERT );
		report( "", "yit8( basar::SS_UPDATE | basar::SS_INSERT );", "", true, strm );
		
		try {
			strm << "yit8.getState().toString(): " << yit8.getState().toString().c_str() << ";" << endl;
		} catch( const basar::Exception& e ){
			report( "", "yit8.getState().toString();", e.what(), false, strm );
		}	

		try {
			strm << "yit8.getFilter().toString(): " << yit8.getFilter().toString().c_str() << ";" << endl;
		} catch( const basar::Exception& e ){
			report( "", "yit8.getFilter().toString()", e.what(), false, strm );
		}


		AccessorPropertyTable_YIterator yit9; 
		yit9 = yit8; 
		report( "", "yit9 = yit8;", "", true, strm );
		
		try {
			strm << "yit9.getState().toString(): " << yit9.getState().toString().c_str() << ";" << endl;
		} catch( const basar::Exception& e ){
			report( "", "yit9.getState().toString();", e.what(), false, strm );
		}	
		try {
			strm << "yit9.getFilter().toString(): " << yit9.getFilter().toString().c_str() << ";" << endl;
		} catch( const basar::Exception& e ){
			report( "", "yit9.getFilter().toString()", e.what(), false, strm );
		}
		strm << boolalpha << "yit9 == yit8: " << (yit9 == yit8) << endl;

		//////////////////////////////////////////////////////////////
		//	instantiation with another yiterator and propertystateset, 
		//	assignment and 
		//	check on equality
		//////////////////////////////////////////////////////////////	
		AccessorPropertyTable_YIterator yit10( yit9, basar::db::aspect::PropertyStateSet(basar::SS_INSERT) ); 
		report( "", "yit10( yit9, PropertyStateset(basar::SS_INSERT) );", "", true, strm );

		AccessorPropertyTable_YIterator yit11;
		yit11 = yit10;
		report( "", "yit11 = yit10;", "", true, strm );

		strm << boolalpha << "yit11 == yit10: " << (yit11 == yit10) << endl;

	} catch( const basar::Exception& e ){
		report( "", "yit8 bis yit10", e.what(), false, strm );
	}

	
	////////////////////////////////////////////////////////////////////////////////////
	strm << "- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - " << endl;
	strm	<< "--> 5.test case: " 
			<< endl << "a) construction with a state" 
			<< endl << "b) state output"
			<< endl << "c) filter output"
			<< endl << "d) assignment to the a)-iterator via an propertytable begin(state) access"
			<< endl << "e) state output"
			<< endl << "f) filter output!"
			<< endl;
	strm << "- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - " << endl;
	////////////////////////////////////////////////////////////////////////////////////
	try {
		//AccessorPropertyTable_YIterator yit11( static_cast<basar::SupportedStateEnum>(basar::SS_DELETE) );
		AccessorPropertyTable_YIterator yit11( basar::SS_DELETE );
		report( "", "yit11 (static_cast<basar::SupportedStateEnum>(basar::SS_DELETE) );", "", true, strm );
		
		try {
			strm << "yit11.getState().toString(): " << yit11.getState().toString().c_str() << ";" << endl;
		} catch( const basar::Exception& e ){
			report( "", "yit11.getState().toString();", e.what(), false, strm );
		}
		try {
			strm << "yit11.getFilter().toString(): " << yit11.getFilter().toString().c_str() << ";" << endl;
		} catch( const basar::Exception& e ){
			report( "", "yit11.getFilter().toString()", e.what(), false, strm );
		}

		/*yit11 = air.getPropertyTable().begin( 
			static_cast<basar::SupportedStateEnum>(basar::SS_UPDATE | basar::SS_INSERT) );*/
		yit11 = air.getPropertyTable().begin( basar::SS_UPDATE | basar::SS_INSERT );
		report( "", "yit11 = air.getPropertyTable.begin( basar::SS_UPDATE | basar::SS_INSERT );", "", true, strm );
		
		try {
			strm << "yit11.getState().toString(): " << yit11.getState().toString().c_str() << ";" << endl;
		} catch( const basar::Exception& e ){
			report( "", "yit11.getState().toString();", e.what(), false, strm );
		}	
		try {
			strm << "yit11.getFilter().toString(): " << yit11.getFilter().toString().c_str() << ";" << endl;
		} catch( const basar::Exception& e ){
			report( "", "yit11.getFilter().toString()", e.what(), false, strm );
		}
		
	} catch( const basar::Exception& e ){
		report( "", "yit11", e.what(), false, strm );
	}

	////////////////////////////////////////////////////////////////////////////////////
	strm << "- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - " << endl;
	strm	<< "--> 6.test case: " 
			<< endl << "a) construction with two different states" 
			<< endl << "b) state output" 
			<< endl << "c) filter output" 
			<< endl << "d) assignment of a)-iterator to a dflt. constructed iterator"
			<< endl << "e) state output"
			<< endl << "f) filter output!"
			<< endl;
	strm << "- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - " << endl;
	////////////////////////////////////////////////////////////////////////////////////
	try {
		/*AccessorPropertyTable_YIterator yit13 = 
			air.getPropertyTable().begin( static_cast<basar::SupportedStateEnum>(basar::SS_UPDATE | basar::SS_INSERT) );*/
		AccessorPropertyTable_YIterator yit13 = air.getPropertyTable().begin( basar::SS_UPDATE | basar::SS_INSERT );
		report( "", "yit13 = air.getPropertyTable.begin( basar::SS_UPDATE | basar::SS_INSERT );", "", true, strm );
		
		try {
			strm << "yit13.getState().toString(): " << yit13.getState().toString().c_str() << ";" << endl;
		} catch( const basar::Exception& e ){
			report( "", "yit13.getState().toString();", e.what(), false, strm );
		}
		try {
			strm << "yit13.getFilter().toString(): " << yit13.getFilter().toString().c_str() << ";" << endl;
		} catch( const basar::Exception& e ){
			report( "", "yit13.getFilter().toString()", e.what(), false, strm );
		}

		AccessorPropertyTable_YIterator yit14;
		yit14 = yit13;
		report( "", "yit14 = yit13;", "", true, strm );
		strm << boolalpha << "yit14 == yit13: " << (yit14 == yit13) << endl;

		try {
			strm << "yit14.getState().toString(): " << yit14.getState().toString().c_str() << ";" << endl;
		} catch( const basar::Exception& e ){
			report( "", "yit14.getState().toString();", e.what(), false, strm );
		}	
		try {
			strm << "yit14.getFilter().toString(): " << yit14.getFilter().toString().c_str() << ";" << endl;
		} catch( const basar::Exception& e ){
			report( "", "yit14.getFilter().toString()", e.what(), false, strm );
		}
		
	} catch( const basar::Exception& e ){
		report( "", "yit13 - yit14", e.what(), false, strm );
	}

	////////////////////////////////////////////////////////////////////////////////////
	strm << "- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - " << endl;
	strm	<< "--> 7.test case: " 
			<< endl << "a) construction with two different states" 
			<< endl << "b) state output" 
			<< endl << "c) filter output" 
			<< endl << "d) new assignment to a)-iterator via an propertytable::insert(type) call"
			<< endl << "e) state output" 
			<< endl << "f) filter output!" 
			<< endl;
	strm << "- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - " << endl;
	////////////////////////////////////////////////////////////////////////////////////
	try {
		/*AccessorPropertyTable_YIterator yit15 = 
			air.getPropertyTable().begin( static_cast<basar::SupportedStateEnum>(basar::SS_UPDATE | basar::SS_INSERT) );*/
		AccessorPropertyTable_YIterator yit15 = air.getPropertyTable().begin( basar::SS_UPDATE | basar::SS_INSERT );
		report( "", "yit15 = air.getPropertyTable.begin( basar::SS_UPDATE | basar::SS_INSERT );", "", true, strm );
		
		try {
			strm << "yit15.getState().toString(): " << yit15.getState().toString().c_str() << ";" << endl;
		} catch( const basar::Exception& e ){
			report( "", "yit15.getState().toString()", e.what(), false, strm );
		}
		
		try {
			strm << "yit15.getFilter().toString(): " << yit15.getFilter().toString().c_str() << ";" << endl;
		} catch( const basar::Exception& e ){
			report( "", "yit15.getFilter().toString()", e.what(), false, strm );
		}

		yit15 = air.getPropertyTable().insert( basar::FOR_DELETE );
		report( "", "yit15 = air.getPropertyTable().insert( basar::FOR_DELETE );", "", true, strm );
		
		try {
			strm << "yit15.getState().toString(): " << yit15.getState().toString().c_str() << ";" << endl;
		} catch( const basar::Exception& e ){
			report( "", "yit15.getFilter().toString()", e.what(), false, strm );
		}
		try {
			strm << "yit15.getFilter().toString(): " << yit15.getFilter().toString().c_str() << ";" << endl;
		} catch( const basar::Exception& e ){
			report( "", "yit15.getFilter().toString()", e.what(), false, strm );
		}
		
	} catch( const basar::Exception& e ){
		report( "", "yit15", e.what(), false, strm );
	}

	strm << "====================================================================" << endl;
	strm << fun << " - Exit!" << endl;
	strm << "====================================================================" << endl;
}
//--------------------------------------------------------------------------------------------------------//
// void printYIterator( AccessorPropertyTable_YIterator yit )
//--------------------------------------------------------------------------------------------------------//
void printYIterator( AccessorPropertyTable_YIterator yit, const string& name, ostream& strm )
{	
	static const char* const fun = "printYIterator( AccessorPropertyTable_YIterator, const string&, ostream& )";		
	
	strm << "= = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = =" << endl;
	strm << fun << " - Entry!" << endl;
	strm << "= = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = =" << endl;

	strm << endl << "----------------------------------------------------------" << endl;
	strm << "Iterator-Name: " << name << endl;;
	
	try {
		strm << "yit.getState().toString(): " << yit.getState().toString().c_str() << ";" << endl;
		report( "", "yit.getState().toString();", "", true, strm );
	} catch( const basar::Exception& e ){
		report( "", "yit.getState().toString();", e.what(), false, strm );
	}
	try {
			strm << "yit.getFilter().toString(): " << yit.getFilter().toString().c_str() << ";" << endl;
	} catch( const basar::Exception& e ){
		report( "", "yit.getFilter().toString()", e.what(), false, strm );
	}

	///////////////
	// artikel_nr
	///////////////
	try {
		strm << "artikel_nr:   " << yit.getInt32 (	"artikel_nr"	) << endl;		
		report( "", "yit.getInt32(\"artikel_nr\");", "", true, strm );
	} catch( const basar::Exception& e ){
		report( "", "yit.getInt32(\"artikel_nr\");", e.what(), false, strm );
	}
	try {
		strm << "artikel_nr(Type):   " << yit.getType( "artikel_nr" ).toString() << endl;		
		report( "", "yit.getType( \"artikel_nr\" ).toString()", "", true, strm );
	} catch( const basar::Exception& e ){
		report( "", "yit.getType( \"artikel_nr\" ).toString()", e.what(), false, strm );
	}


	/////////////////
	// artikel_name
	/////////////////
	try {
		strm << "artikel_name: " << yit.getString(	"artikel_name"	) << endl;
		report( "", "yit.getString(\"artikel_name\");", "", true, strm );
	} catch( const basar::Exception& e ){
		report( "", "yit.getString(\"artikel_name\");", e.what(), false, strm );
	}	
	try {
		strm << "artikel_name(Type):   " << yit.getType( "artikel_name" ).toString() << endl;		
		report( "", "yit.getType( \"artikel_name\" ).toString()", "", true, strm );
	} catch( const basar::Exception& e ){
		report( "", "yit.getType( \"artikel_name\" ).toString()", e.what(), false, strm );
	}

	/////////////////
	// filialnr
	/////////////////
	try {	
		strm << "filialnr:     " << yit.getInt16 (	"filialnr"		) << endl;
		report( "", "yit.getInt16(\"filialnr\");", "", true, strm );
	} catch( const basar::Exception& e ){
		report( "", "yit.getInt16(\"filialnr\");", e.what(), false, strm );
	}	
	try {
		strm << "filial_nr(Type):   " << yit.getType( "filial_nr" ).toString() << endl;		
		report( "", "yit.getType( \"filial_nr\" ).toString()", "", true, strm );
	} catch( const basar::Exception& e ){
		report( "", "yit.getType( \"filial_nr\" ).toString()", e.what(), false, strm );
	}

	/////////////////
	// bestand
	/////////////////
	try {
		strm << "bestand:      " << yit.getInt32 (	"bestand"		) << endl;
		report( "", "yit.getInt32(\"bestand\");", "", true, strm );
	} catch( const basar::Exception& e ){
		report( "", "yit.getInt32(\"bestand\");", e.what(), false, strm );
	}
	try {
		strm << "Bestand(Type):   " << yit.getType( "bestand" ).toString() << endl;		
		report( "", "yit.getType( \"bestand\" ).toString()", "", true, strm );
	} catch( const basar::Exception& e ){
		report( "", "yit.getType( \"bestand\" ).toString()", e.what(), false, strm );
	}
	
	strm << "----------------------------------------------------------" << endl;

	strm << "= = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = =" << endl;
	strm << fun << " - Exit!" << endl;
	strm << "= = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = =" << endl;
}