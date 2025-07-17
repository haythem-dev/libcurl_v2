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
//using basar::property::PropertyDescriptionListRef;
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

void check_FOR_UNKNOWN	( AccessorInstanceRef  air, ostream& strm = cout );
void check_FOR_CLEAN	( AccessorInstanceRef  air, ostream& strm = cout );
void check_FOR_INSERT	( AccessorInstanceRef  air, ostream& strm = cout );
void check_FOR_UPDATE	( AccessorInstanceRef  air, ostream& strm = cout );
void check_FOR_DELETE	( AccessorInstanceRef  air, ostream& strm = cout );


//--------------------------------------------------------------------------------------------------------//
// void check_States( ConnectionRef connection, AccessorInstanceRef air )
//--------------------------------------------------------------------------------------------------------//
void check_States( ConnectionRef connection, AccessorInstanceRef air )
{
	static const char* const fun = "check_States(ConnectionRef, AccessorInstanceRef)";

	//ostream& strm = cout;
	ofstream log ( "aspect.check_states.log" ); ostream& strm = log;
	strm << "====================================================================" << endl;
	strm << fun << " - Entry!" << endl;
	strm << "====================================================================" << endl << endl;

	strm << "Date: " << getDate().str().c_str() << "\tTime: " << getTime().str().c_str() << endl;
	ostringstream os;
	air = checkBLieSortAccessorInstanceRef( connection, basar::db::aspect::FULL_CACHING );
	
	// Iteration via all InsertTypes
	for( int i = 0; i < 5; ++i ){
		if( i == 0 ){
			check_FOR_UNKNOWN( air, strm );
		} else if( i == 1 ){
			check_FOR_CLEAN( air, strm  );
		} else if( i == 2 ){
			check_FOR_INSERT( air, strm  );
		} else if( i == 3 ){
			check_FOR_UPDATE( air, strm  );
		} else if( i == 4 ){
			check_FOR_DELETE( air, strm  );
		}			
	}
	
	strm << "====================================================================" << endl;
	strm << fun << " - Exit!" << endl;
	strm << "====================================================================" << endl;
}
//------------------------------------------------------------------------------------------------------------//
// void check_FOR_UNKNOWN( AccessorInstanceRef  air, ostream& strm = cout )
//------------------------------------------------------------------------------------------------------------//
void check_FOR_UNKNOWN( AccessorInstanceRef air, ostream& strm )
{
	static const char* const fun = "check_FOR_UNKNOWN(AccessorInstanceRef, ostream&)";

	strm << "====================================================================" << endl;
	strm << fun << " - Entry!" << endl;
	strm << "====================================================================" << endl << endl;

	AccessorPropertyTable_YIterator ayit;
	try {
		try {
			strm << "Clearing PropertyTable." << endl;	
			air.getPropertyTable().clear();
		} catch( basar::Exception& e ){
			report( "", "air.getPropertyTable().clear();", e.what(), false, strm );
		} catch( ... ){
			report( "", "air.getPropertyTable().clear();", "Exception ...", false, strm );
		}

		strm << "2. Insert new row into the PropertyTable with InsertType 'basar::FOR_UNKNOWN'." << endl;	
		ayit = air.getPropertyTable().insert( basar::FOR_UNKNOWN );
				
		strm << "-----------------------------------------------------------"	<< endl;
		strm << "3. Getting a value with an SS_UNSET state." << endl;
		strm << "-----------------------------------------------------------"	<< endl;
		try {
			basar::Int32 i = ayit.getInt32( "artikel_nr" );
			strm << "ayit.getInt32( \"artikel_nr\" ): " << i << endl;
		} catch( const basar::Exception& e ){
			report( "", "yit.getInt16( \"spalte1\")", e.what(), false, strm );
		}
		strm << "-----------------------------------------------------------"	<< endl;
		strm << "4. Execute() on an SS_UNSET state." << endl;
		strm << "-----------------------------------------------------------"	<< endl;
		try {
			strm << "4.1 Stream the state(s) in the context of the yiterator." << endl;
			strm << ayit.getState().toString().c_str() << endl; 
			strm << "4.2 Stream the first row of the propertytable (yiterator)." << endl;
			ayit.toStream( strm );
			
			basar::I18nString s("besla_nr=11;" );
			basar::db::aspect::AccessorPropertyTable_YIterator yit = basar::db::aspect::Manager::getInstance().createAccessorYIterator(s);
			air.execute( "WriteBLieSort", yit );
		} catch( const basar::Exception& e ){
			report( "", "air.execute( \"WriteBLieSort\", ayit );", e.what(), false, strm );
		}
		try {
			strm << "4.3 Stream (after execute()) the state(s) in the context of the yiterator." << endl;
			strm << ayit.getState().toString().c_str() << endl; 
			strm << "4.4 Stream (after execute()) the first row of the propertytable (yiterator)." << endl;
			ayit.toStream( strm );
		} catch( const basar::Exception& e ){
			report( "", "air.execute( \"WriteBLieSort\", ayit );", e.what(), false, strm );
		}

		strm << "-----------------------------------------------------------"	<< endl;
		strm << "5. Erase on an SS_UNSET state." << endl;
		strm << "-----------------------------------------------------------"	<< endl;
		try {
			air.getPropertyTable().clear();
			ayit = air.getPropertyTable().insert( basar::FOR_UNKNOWN );
			strm << "5.1 Stream the state(s) in the context of the yiterator." << endl;
			strm << ayit.getState().toString().c_str() << endl; 
			strm << "5.2 Stream the first row of the propertytable (yiterator)." << endl;
			ayit.toStream( strm );
			air.getPropertyTable().erase( ayit );
		} catch( const basar::Exception& e ){
			report( "", "air.getPropertyTable().erase( ayit );", e.what(), false, strm );
		}

		strm << "-----------------------------------------------------------"	<< endl;
		strm << "6. Setting value (32) with an SS_UNSET state --> SS_UNKNOWN state." << endl;
		strm << "-----------------------------------------------------------"	<< endl;
		try {
			air.getPropertyTable().clear();
			ayit = air.getPropertyTable().insert( basar::FOR_UNKNOWN );
			strm << "6.1 Stream the state(s) in the context of the yiterator." << endl;
			strm << ayit.getState().toString().c_str() << endl; 
			strm << "6.2 Stream the first row of the propertytable (yiterator)." << endl;
			ayit.toStream( strm );
			ayit.setInt32( "artikel_nr", 32 );
			strm << "6.3 Stream the state(s) in the context of the yiterator." << endl;
			strm << ayit.getState().toString().c_str() << endl; 
			strm << "6.4 Stream the first row of the propertytable (yiterator)." << endl;
			ayit.toStream( strm );
		} catch( const basar::Exception& e ){
			report( "", "ayit.setInt32( \"artikel_nr\", 32 );", e.what(), false, strm );
		}
		//---//
		strm << "-----------------------------------------------------------"	<< endl;
		strm << "7 Setting value (3232 - second time) with an SS_UNKNOWN state --> SS_UNKNOWN state." << endl;
		strm << "-----------------------------------------------------------"	<< endl;
		try {
			ayit.setInt32( "artikel_nr", 3232 );
			strm << "7.1 Stream the state(s) in the context of the yiterator." << endl;
			strm << ayit.getState().toString().c_str() << endl; 
			strm << "7.2 Stream the first row of the propertytable (yiterator)." << endl;
			ayit.toStream( strm );
		} catch( const basar::Exception& e ){
			report( "", "ayit.setInt32( \"artikel_nr\", 3232 );", e.what(), false, strm );
		}
		strm << "-----------------------------------------------------------"	<< endl;
		strm << "8 Getting a value with an SS_UNKNOWN state." << endl;
		strm << "-----------------------------------------------------------"	<< endl;
		try {
			basar::Int32 i = ayit.getInt32( "artikel_nr" );
			strm << "ayit.getInt32( \"artikel_nr\" ): " << i << endl;
			strm << "8.1 Stream the state(s) in the context of the yiterator." << endl;
			strm << ayit.getState().toString().c_str() << endl; 
			strm << "8.2 Stream the first row of the propertytable (yiterator)." << endl;
			ayit.toStream( strm );
		} catch( const basar::Exception& e ){
			report( "", "ayit.getInt32( \"artikel_nr\" );", e.what(), false, strm );
		}
		strm << "-----------------------------------------------------------"	<< endl;
		strm << "9. Execute() on an SS_UNKNOWN state." << endl;
		strm << "-----------------------------------------------------------"	<< endl;
		try {
			strm << "9.1 Stream the state(s) in the context of the yiterator." << endl;
			strm << ayit.getState().toString().c_str() << endl; 
			strm << "9.2 Stream the first row of the propertytable (yiterator)." << endl;
			ayit.toStream( strm );
			strm << "Setting besla_nr=123456 and artikel_nr=12346" << endl;
			ayit.setInt32( "besla_nr",   123456 );
			ayit.setInt32( "artikel_nr", 123456 );
			air.execute( "WriteBLieSort", ayit );
		} catch( const basar::Exception& e ){
			report( "", "air.execute( \"WriteBLieSort\", ayit );", e.what(), false, strm );
		}
		try {
			strm << "9.3 Stream (after execute()) the state(s) in the context of the yiterator." << endl;
			strm << ayit.getState().toString().c_str() << endl; 
			strm << "9.4 Stream (after execute()) the first row of the propertytable (yiterator)." << endl;
			ayit.toStream( strm );
		} catch( const basar::Exception& e ){
			report( "", "air.execute( \"WriteBLieSort\", ayit );", e.what(), false, strm );
		}

	} catch( const basar::Exception& e ){
		report( "", "xxx", e.what(), false, strm );
	}

	strm << "====================================================================" << endl;
	strm << fun << " - Exit!" << endl;
	strm << "====================================================================" << endl;

}
//------------------------------------------------------------------------------------------------------------//
// void check_FOR_CLEAN	( AccessorInstanceRef  air, ostream& strm = cout )
//------------------------------------------------------------------------------------------------------------//
void check_FOR_CLEAN ( AccessorInstanceRef  air, ostream& strm )
{
	static const char* const fun = "check_FOR_CLEAN(AccessorInstanceRef, ostream&)";

	strm << "====================================================================" << endl;
	strm << fun << " - Entry!" << endl;
	strm << "====================================================================" << endl << endl;

	AccessorPropertyTable_YIterator ayit;
	try {
		try {
			strm << "Clearing PropertyTable!" << endl;	
			air.getPropertyTable().clear();
			strm << "PropertyTable cleared." << endl;	
		} catch( basar::Exception& e ){
			report( "", "air.getPropertyTable().clear();", e.what(), false, strm );
		} catch( ... ){
			report( "", "air.getPropertyTable().clear();", "Exception ...", false, strm );
		}

		strm << "-----------------------------------------------------------"	<< endl;
		strm << "1. Getting a value with an SS_UNSET state." << endl;
		strm << "-----------------------------------------------------------"	<< endl;
		try {
			strm << "1.2 Insert new row into the PropertyTable with InsertType 'basar::FOR_CLEAN'." << endl;	
			ayit = air.getPropertyTable().insert( basar::FOR_CLEAN );
			strm << "1.2 In PropertyTable inserted." << endl;	
			basar::Int32 i = ayit.getInt32( "artikel_nr" );
			strm << "1.3 ayit.getInt32( \"artikel_nr\" ): " << i << endl;
		} catch( const basar::Exception& e ){
			report( "", "yit.getInt16( \"spalte1\")", e.what(), false, strm );
		}
		strm << "-----------------------------------------------------------"	<< endl;
		strm << "2. Execute() on an SS_UNSET state." << endl;
		strm << "-----------------------------------------------------------"	<< endl;
		try {
			strm << "2.1 Stream the state(s) in the context of the yiterator." << endl;
			strm << ayit.getState().toString().c_str() << endl; 
			strm << "2.2 Stream the first row of the propertytable (yiterator)." << endl;
			ayit.toStream( strm );
			
			strm << "2.3 Executing via ParameterList <besla_nr=11;>." << endl;
			basar::I18nString s("besla_nr=11;" );
			basar::db::aspect::AccessorPropertyTable_YIterator yit = basar::db::aspect::Manager::getInstance().createAccessorYIterator(s);
			basar::Int32 written = air.execute( "WriteBLieSort", yit ).m_affectedRows;
			strm << "2.4 Number of written rows: " << written << endl; 
		} catch( const basar::Exception& e ){
			report( "", "air.execute( \"WriteBLieSort\", ayit );", e.what(), false, strm );
		}
		try {
			strm << "2.5 Stream (after execute()) the state(s) in the context of the yiterator." << endl;
			strm << ayit.getState().toString().c_str() << endl; 
			strm << "2.6 Stream (after execute()) the first row of the propertytable (yiterator)." << endl;
			ayit.toStream( strm );
		} catch( const basar::Exception& e ){
			report( "", "air.execute( \"WriteBLieSort\", ayit );", e.what(), false, strm );
		}

		strm << "-----------------------------------------------------------"	<< endl;
		strm << "3. Erase on an SS_UNSET state." << endl;
		strm << "-----------------------------------------------------------"	<< endl;
		try {
			strm << "3.1 Clearing PropertyTable." << endl;
			air.getPropertyTable().clear();
			strm << "3.2 PropertyTable cleared." << endl;
			strm << "3.3 Inserting into PropertyTable." << endl;
			ayit = air.getPropertyTable().insert( basar::FOR_CLEAN );
			strm << "3.4 in PropertyTable inserted." << endl;
			strm << "3.5 Stream the state(s) in the context of the yiterator." << endl;
			strm << ayit.getState().toString().c_str() << endl; 
			strm << "3.6 Stream the first row of the propertytable (yiterator)." << endl;
			ayit.toStream( strm );
			strm << "3.7 Erasing dataset of PropertyTable." << endl;
			air.getPropertyTable().erase( ayit );
			strm << "3.8 dataset of PropertyTable erased." << endl;
		} catch( const basar::Exception& e ){
			report( "", "air.getPropertyTable().erase( ayit );", e.what(), false, strm );
		}

		//---------------------------------------------//
		// state: SS_CLEAN
		//---------------------------------------------//
		strm << "-----------------------------------------------------------"	<< endl;
		strm << "4. Setting value (32) with an SS_UNSET state --> SS_CLEAN state." << endl;
		strm << "-----------------------------------------------------------"	<< endl;
		try {
			strm << "4.1 Clearing PropertyTable." << endl;
			air.getPropertyTable().clear();
			strm << "4.2 PropertyTable erased." << endl;
			strm << "4.3 Inserting into PropertyTable." << endl;
			ayit = air.getPropertyTable().insert( basar::FOR_CLEAN );
			strm << "4.4 Into PropertyTable inserted." << endl;
			strm << "4.5 Stream the state(s) in the context of the yiterator." << endl;
			strm << ayit.getState().toString().c_str() << endl; 
			strm << "4.6 Stream the first row of the propertytable (yiterator)." << endl;
			ayit.toStream( strm );
			strm << "4.7 Setting value <32> to attribute <artikel_nr>." << endl;
			ayit.setInt32( "artikel_nr", 32 );
			strm << "4.8 Stream the state(s) in the context of the yiterator." << endl;
			strm << ayit.getState().toString().c_str() << endl; 
			strm << "4.9 Stream the first row of the propertytable (yiterator)." << endl;
			ayit.toStream( strm );
		} catch( const basar::Exception& e ){
			report( "", "ayit.setInt32( \"artikel_nr\", 32 );", e.what(), false, strm );
		}
		strm << "-----------------------------------------------------------"	<< endl;
		strm << "5. Getting a value with an SS_CLEAN state --> SS_CLEAN state." << endl;
		strm << "-----------------------------------------------------------"	<< endl;
		try {
			strm << "5.1 Getting attribute <artikel_nr>." << endl;
			basar::Int32 i = ayit.getInt32( "artikel_nr" );
			strm << "5.2 ayit.getInt32( \"artikel_nr\" ): " << i << endl;
			strm << "5.3 Stream the state(s) in the context of the yiterator." << endl;
			strm << ayit.getState().toString().c_str() << endl; 
			strm << "5.4 Stream the first row of the propertytable (yiterator)." << endl;
			ayit.toStream( strm );
		} catch( const basar::Exception& e ){
			report( "", "ayit.getInt32( \"artikel_nr\" );", e.what(), false, strm );
		}
		strm << "-----------------------------------------------------------"	<< endl;
		strm << "6. Execute() on an SS_UPDATE state." << endl;
		strm << "-----------------------------------------------------------"	<< endl;
		try {
			strm << "6.1 Stream the state(s) in the context of the yiterator." << endl;
			strm << ayit.getState().toString().c_str() << endl; 
			strm << "6.2 Stream the first row of the propertytable (yiterator)." << endl;
			ayit.toStream( strm );
			basar::Int32 written = air.execute( "WriteBLieSort", ayit ).m_affectedRows;
			strm << "6.3 Number of written datasets: " << written << endl;
		} catch( const basar::Exception& e ){
			report( "", "air.execute( \"WriteBLieSort\", ayit );", e.what(), false, strm );
		}
		//---------------------------------------------//
		// state: SS_UPDATE
		//---------------------------------------------//
		strm << "-----------------------------------------------------------"	<< endl;
		strm << "7. Setting value with an SS_CLEAN state --> SS_UPDATE state." << endl;
		strm << "-----------------------------------------------------------"	<< endl;
		try {
			strm << "7.1 Setting value <3232> attribute <artikel_nr>" << endl;
			ayit.setInt32( "artikel_nr", 3232 );
			strm << "7.2 Stream the state(s) in the context of the yiterator." << endl;
			strm << ayit.getState().toString().c_str() << endl; 
			strm << "7.3 Stream the first row of the propertytable (yiterator)." << endl;
			ayit.toStream( strm );
		} catch( const basar::Exception& e ){
			report( "", "ayit.setInt32( \"artikel_nr\", 3232 );", e.what(), false, strm );
		}
		strm << "-----------------------------------------------------------"	<< endl;
		strm << "8. Setting value with an SS_UPDATE state --> SS_UPDATE state." << endl;
		strm << "-----------------------------------------------------------"	<< endl;
		try {
			strm << "Setting value <323232> attribute <artikel_nr>" << endl;
			ayit.setInt32( "artikel_nr", 323232 );
			strm << "8.1 Stream the state(s) in the context of the yiterator." << endl;
			strm << ayit.getState().toString().c_str() << endl; 
			strm << "8.2 Stream the first row of the propertytable (yiterator)." << endl;
			ayit.toStream( strm );
		} catch( const basar::Exception& e ){
			report( "", "ayit.setInt32( \"artikel_nr\", 3232 );", e.what(), false, strm );
		}
		strm << "-----------------------------------------------------------"	<< endl;
		strm << "9. Getting a value with an SS_UPDATE state --> SS_UPDATE state." << endl;
		strm << "-----------------------------------------------------------"	<< endl;
		try {
			strm << "9.1 Getting value of attribute <artikel_nr>." << endl;
			basar::Int32 i = ayit.getInt32( "artikel_nr" );
			strm << "9.2 ayit.getInt32( \"artikel_nr\" ): " << i << endl;
			strm << "9.3 Stream the state(s) in the context of the yiterator." << endl;
			strm << ayit.getState().toString().c_str() << endl; 
			strm << "9.4 Stream the first row of the propertytable (yiterator)." << endl;
			ayit.toStream( strm );
		} catch( const basar::Exception& e ){
			report( "", "ayit.getInt32( \"artikel_nr\" );", e.what(), false, strm );
		}
		strm << "-----------------------------------------------------------"	<< endl;
		strm << "10. Execute() on an SS_UPDATE state." << endl;
		strm << "-----------------------------------------------------------"	<< endl;
		try {
			strm << "10.1 Stream the state(s) in the context of the yiterator." << endl;
			strm << ayit.getState().toString().c_str() << endl; 
			strm << "10.2 Stream the first row of the propertytable (yiterator)." << endl;
			ayit.toStream( strm );
			strm << "10.3 Setting besla_nr=123456 and artikel_nr=12346" << endl;
			strm << "10.4 Setting values <123456, 123456> of attributes <besla_nr, artikel_nr>." << endl;
			ayit.setInt32( "besla_nr",   123456 );
			ayit.setInt32( "artikel_nr", 123456 );
			strm << "10.5 Executing (WriteBLieSort) ..." << endl;
			basar::Int32 written = air.execute( "WriteBLieSort", ayit ).m_affectedRows;
			strm << "10.6 Number of written datasets: " << written << endl;
		} catch( const basar::Exception& e ){
			report( "", "air.execute( \"WriteBLieSort\", ayit );", e.what(), false, strm );
		}
		try {
			strm << "10.7 Stream (after execute()) the state(s) in the context of the yiterator." << endl;
			strm << ayit.getState().toString().c_str() << endl; 
			strm << "10.8 Stream (after execute()) the first row of the propertytable (yiterator)." << endl;
			ayit.toStream( strm );
		} catch( const basar::Exception& e ){
			report( "", "air.execute( \"WriteBLieSort\", ayit );", e.what(), false, strm );
		}

		//---------------------------------------------//
		// state: SS_UPDATE
		//---------------------------------------------//
		strm << "-----------------------------------------------------------"	<< endl;
		strm << "11. MarkForDelete with a SS_UPDATE state." << endl;
		strm << "-----------------------------------------------------------"	<< endl;
		try {
			strm << "11.1 Stream the state(s) in the context of the yiterator." << endl;
			strm << ayit.getState().toString().c_str() << endl; 
			strm << "11.2 Stream the first row of the propertytable (yiterator)." << endl;
			ayit.toStream( strm );
			
			strm << "11.3 MarkForDelete ..." << endl;
			air.getPropertyTable().markForDelete( ayit );
			
			strm << "11.4 Stream the state(s) in the context of the yiterator." << endl;
			strm << ayit.getState().toString().c_str() << endl; 
			strm << "11.5 Stream the first row of the propertytable (yiterator)." << endl;
			ayit.toStream( strm );
		} catch( const basar::Exception& e ){
			report( "", "air.execute( \"WriteBLieSort\", ayit );", e.what(), false, strm );
		}
		strm << "-----------------------------------------------------------"	<< endl;
		strm << "12. Setting value with an SS_DELETE state --> SS_DELETE state." << endl;
		strm << "-----------------------------------------------------------"	<< endl;
		try {
			strm << "12.1 Setting value <9999> at attribute <artikel_nr>." << endl;
			ayit.setInt32( "artikel_nr", 9999 );
			strm << "12.2 Stream the state(s) in the context of the yiterator." << endl;
			strm << ayit.getState().toString().c_str() << endl; 
			strm << "12.3 Stream the first row of the propertytable (yiterator)." << endl;
			ayit.toStream( strm );
		} catch( const basar::Exception& e ){
			report( "", "ayit.setInt32( \"artikel_nr\", 9999 );", e.what(), false, strm );
		}
		strm << "-----------------------------------------------------------"	<< endl;
		strm << "13. Getting value value with an SS_DELETE state --> SS_DELETE state." << endl;
		strm << "-----------------------------------------------------------"	<< endl;
		try {
			basar::Int32 i = ayit.getInt32( "artikel_nr" );
			strm << "13.1 ayit.getInt32( \"artikel_nr\" ): " << i << endl;
			
			strm << "13.2 Stream the state(s) in the context of the yiterator." << endl;
			strm << ayit.getState().toString().c_str() << endl; 
			strm << "13.3 Stream the first row of the propertytable (yiterator)." << endl;
			ayit.toStream( strm );
		} catch( const basar::Exception& e ){
			report( "", "ayit.getInt32( \"artikel_nr\" );", e.what(), false, strm );
		}
		strm << "-----------------------------------------------------------"	<< endl;
		strm << "14. Execute() on an SS_DELETE state." << endl;
		strm << "-----------------------------------------------------------"	<< endl;
		try {
			strm << "14.1 Stream the state(s) in the context of the yiterator." << endl;
			strm << ayit.getState().toString().c_str() << endl; 
			strm << "14.2 Stream the first row of the propertytable (yiterator)." << endl;
			ayit.toStream( strm );
			strm << "14.3 Setting besla_nr=123456 and artikel_nr=12346" << endl;
			ayit.setInt32( "besla_nr",   123456 );
			ayit.setInt32( "artikel_nr", 123456 );
			strm << "14.4 Deleting (DeleteBLieSort) ... " << endl;
			basar::Int32 deleted = air.execute( "DeleteBLieSort", ayit ).m_affectedRows;
			strm << "14.5 Number of deleted datasets: " << deleted << endl;
		} catch( const basar::Exception& e ){
			report( "", "air.execute( \"WriteBLieSort\", ayit );", e.what(), false, strm );
		}
		try {
			strm << "14.6 Number of datasets in the propertytable: " << air.getPropertyTable().size() << endl;
			strm << "14.7 Stream (after execute()) the state(s) in the context of the yiterator." << endl;
			strm << ayit.getState().toString().c_str() << endl; 
			strm << "14.8 Stream (after execute()) the first row of the propertytable (yiterator)." << endl;
			ayit.toStream( strm );
		} catch( const basar::Exception& e ){
			report( "", "air.execute( \"WriteBLieSort\", ayit );", e.what(), false, strm );
		}

	} catch( const basar::Exception& e ){
		report( "", "xxx", e.what(), false, strm );
	}
	strm << "====================================================================" << endl;
	strm << fun << " - Exit!" << endl;
	strm << "====================================================================" << endl;

}
//------------------------------------------------------------------------------------------------------------//
// void check_FOR_UPDATE	( AccessorInstanceRef  air, ostream& strm = cout )
//------------------------------------------------------------------------------------------------------------//
void check_FOR_UPDATE ( AccessorInstanceRef  air, ostream& strm )
{
	static const char* const fun = "check_FOR_UPDATE(AccessorInstanceRef, ostream&)";

	strm << "====================================================================" << endl;
	strm << fun << " - Entry!" << endl;
	strm << "====================================================================" << endl << endl;

	AccessorPropertyTable_YIterator ayit;
	try {
		try {
			strm << "Clearing PropertyTable!" << endl;	
			air.getPropertyTable().clear();
			strm << "PropertyTable cleared." << endl;	
		} catch( basar::Exception& e ){
			report( "", "air.getPropertyTable().clear();", e.what(), false, strm );
		} catch( ... ){
			report( "", "air.getPropertyTable().clear();", "Exception ...", false, strm );
		}

		strm << "-----------------------------------------------------------"	<< endl;
		strm << "1. Getting a value with an SS_UNSET state." << endl;
		strm << "-----------------------------------------------------------"	<< endl;
		try {
			strm << "1.2 Insert new row into the PropertyTable with InsertType 'basar::FOR_UPDATE'." << endl;	
			ayit = air.getPropertyTable().insert( basar::FOR_UPDATE );
			strm << "1.2 In PropertyTable inserted." << endl;	
			basar::Int32 i = ayit.getInt32( "artikel_nr" );
			strm << "1.3 ayit.getInt32( \"artikel_nr\" ): " << i << endl;
		} catch( const basar::Exception& e ){
			report( "", "yit.getInt16( \"spalte1\")", e.what(), false, strm );
		}
		strm << "-----------------------------------------------------------"	<< endl;
		strm << "2. Execute() on an SS_UNSET state." << endl;
		strm << "-----------------------------------------------------------"	<< endl;
		try {
			strm << "2.1 Stream the state(s) in the context of the yiterator." << endl;
			strm << ayit.getState().toString().c_str() << endl; 
			strm << "2.2 Stream the first row of the propertytable (yiterator)." << endl;
			ayit.toStream( strm );
			
			strm << "2.3 Executing via ParameterList <besla_nr=11;>." << endl;
			basar::I18nString s("besla_nr=11;" );
			basar::db::aspect::AccessorPropertyTable_YIterator yit = basar::db::aspect::Manager::getInstance().createAccessorYIterator(s);
			basar::Int32 written = air.execute( "WriteBLieSort", yit ).m_affectedRows;
			strm << "2.4 Number of written rows: " << written << endl; 
		} catch( const basar::Exception& e ){
			report( "", "air.execute( \"WriteBLieSort\", ayit );", e.what(), false, strm );
		}
		try {
			strm << "2.5 Stream (after execute()) the state(s) in the context of the yiterator." << endl;
			strm << ayit.getState().toString().c_str() << endl; 
			strm << "2.6 Stream (after execute()) the first row of the propertytable (yiterator)." << endl;
			ayit.toStream( strm );
		} catch( const basar::Exception& e ){
			report( "", "air.execute( \"WriteBLieSort\", ayit );", e.what(), false, strm );
		}

		strm << "-----------------------------------------------------------"	<< endl;
		strm << "3. Erase on an SS_UNSET state." << endl;
		strm << "-----------------------------------------------------------"	<< endl;
		try {
			strm << "3.1 Clearing PropertyTable." << endl;
			air.getPropertyTable().clear();
			strm << "3.2 PropertyTable cleared." << endl;
			strm << "3.3 Inserting into PropertyTable." << endl;
			ayit = air.getPropertyTable().insert( basar::FOR_UPDATE );
			strm << "3.4 in PropertyTable inserted." << endl;
			strm << "3.5 Stream the state(s) in the context of the yiterator." << endl;
			strm << ayit.getState().toString().c_str() << endl; 
			strm << "3.6 Stream the first row of the propertytable (yiterator)." << endl;
			ayit.toStream( strm );
			strm << "3.7 Erasing dataset of PropertyTable." << endl;
			air.getPropertyTable().erase( ayit );
			strm << "3.8 dataset of PropertyTable erased." << endl;
		} catch( const basar::Exception& e ){
			report( "", "air.getPropertyTable().erase( ayit );", e.what(), false, strm );
		}

		//---------------------------------------------//
		// state: SS_UPDATE
		//---------------------------------------------//
		strm << "-----------------------------------------------------------"	<< endl;
		strm << "4. Setting value (32) with an SS_UNSET state --> SS_UPDATE state." << endl;
		strm << "-----------------------------------------------------------"	<< endl;
		try {
			strm << "4.1 Clearing PropertyTable." << endl;
			air.getPropertyTable().clear();
			strm << "4.2 PropertyTable erased." << endl;
			strm << "4.3 Inserting into PropertyTable." << endl;
			ayit = air.getPropertyTable().insert( basar::FOR_UPDATE );
			strm << "4.4 Into PropertyTable inserted." << endl;
			strm << "4.5 Stream the state(s) in the context of the yiterator." << endl;
			strm << ayit.getState().toString().c_str() << endl; 
			strm << "4.6 Stream the first row of the propertytable (yiterator)." << endl;
			ayit.toStream( strm );
			strm << "4.7 Setting value <32> to attribute <artikel_nr>." << endl;
			ayit.setInt32( "artikel_nr", 32 );
			strm << "4.8 Stream the state(s) in the context of the yiterator." << endl;
			strm << ayit.getState().toString().c_str() << endl; 
			strm << "4.9 Stream the first row of the propertytable (yiterator)." << endl;
			ayit.toStream( strm );
		} catch( const basar::Exception& e ){
			report( "", "ayit.setInt32( \"artikel_nr\", 32 );", e.what(), false, strm );
		}
		strm << "-----------------------------------------------------------"	<< endl;
		strm << "5. Setting value (32) with an SS_UPDATE state --> SS_UPDATE state." << endl;
		strm << "-----------------------------------------------------------"	<< endl;
		try {
			strm << "5.1 Setting value <3232> to attribute <artikel_nr> again." << endl;
			ayit.setInt32( "artikel_nr", 3232 );
			strm << "5.2 Stream the state(s) in the context of the yiterator." << endl;
			strm << ayit.getState().toString().c_str() << endl; 
			strm << "5.3 Stream the first row of the propertytable (yiterator)." << endl;
			ayit.toStream( strm );
		} catch( const basar::Exception& e ){
			report( "", "ayit.setInt32( \"artikel_nr\", 3232 );", e.what(), false, strm );
		}
		strm << "-----------------------------------------------------------"	<< endl;
		strm << "6. Getting a value with an SS_UPDATE state --> SS_UPDATE state." << endl;
		strm << "-----------------------------------------------------------"	<< endl;
		try {
			strm << "6.1 Getting attribute <artikel_nr>." << endl;
			basar::Int32 i = ayit.getInt32( "artikel_nr" );
			strm << "6.2 ayit.getInt32( \"artikel_nr\" ): " << i << endl;
			strm << "6.3 Stream the state(s) in the context of the yiterator." << endl;
			strm << ayit.getState().toString().c_str() << endl; 
			strm << "6.4 Stream the first row of the propertytable (yiterator)." << endl;
			ayit.toStream( strm );
		} catch( const basar::Exception& e ){
			report( "", "ayit.getInt32( \"artikel_nr\" );", e.what(), false, strm );
		}
		strm << "-----------------------------------------------------------"	<< endl;
		strm << "7. Execute() on an SS_UPDATE state --> SS_CLEAN state." << endl;
		strm << "-----------------------------------------------------------"	<< endl;
		try {
			strm << "7.1 Stream the state(s) in the context of the yiterator." << endl;
			strm << ayit.getState().toString().c_str() << endl; 
			strm << "7.2 Stream the first row of the propertytable (yiterator)." << endl;
			ayit.toStream( strm );
			basar::Int32 written = air.execute( "WriteBLieSort", ayit ).m_affectedRows;
			strm << "7.3 Number of written datasets: " << written << endl;
		} catch( const basar::Exception& e ){
			report( "", "air.execute( \"WriteBLieSort\", ayit );", e.what(), false, strm );
		}
		//---------------------------------------------//
		// state: SS_UPDATE
		//---------------------------------------------//
		strm << "-----------------------------------------------------------"	<< endl;
		strm << "8. Setting value with an SS_CLEAN state --> SS_UPDATE state." << endl;
		strm << "-----------------------------------------------------------"	<< endl;
		try {
			strm << "8.1 Setting value <3232> attribute <artikel_nr>" << endl;
			ayit.setInt32( "artikel_nr", 3232 );
			strm << "8.2 Stream the state(s) in the context of the yiterator." << endl;
			strm << ayit.getState().toString().c_str() << endl; 
			strm << "8.3 Stream the first row of the propertytable (yiterator)." << endl;
			ayit.toStream( strm );
		} catch( const basar::Exception& e ){
			report( "", "ayit.setInt32( \"artikel_nr\", 3232 );", e.what(), false, strm );
		}
		strm << "-----------------------------------------------------------"	<< endl;
		strm << "9. Setting value with an SS_UPDATE state --> SS_UPDATE state." << endl;
		strm << "-----------------------------------------------------------"	<< endl;
		try {
			strm << "Setting value <323232> attribute <artikel_nr>" << endl;
			ayit.setInt32( "artikel_nr", 323232 );
			strm << "9.1 Stream the state(s) in the context of the yiterator." << endl;
			strm << ayit.getState().toString().c_str() << endl; 
			strm << "9.2 Stream the first row of the propertytable (yiterator)." << endl;
			ayit.toStream( strm );
		} catch( const basar::Exception& e ){
			report( "", "ayit.setInt32( \"artikel_nr\", 3232 );", e.what(), false, strm );
		}
		strm << "-----------------------------------------------------------"	<< endl;
		strm << "10. Getting a value with an SS_UPDATE state --> SS_UPDATE state." << endl;
		strm << "-----------------------------------------------------------"	<< endl;
		try {
			strm << "10.1 Getting value of attribute <artikel_nr>." << endl;
			basar::Int32 i = ayit.getInt32( "artikel_nr" );
			strm << "10.2 ayit.getInt32( \"artikel_nr\" ): " << i << endl;
			strm << "10.3 Stream the state(s) in the context of the yiterator." << endl;
			strm << ayit.getState().toString().c_str() << endl; 
			strm << "10.4 Stream the first row of the propertytable (yiterator)." << endl;
			ayit.toStream( strm );
		} catch( const basar::Exception& e ){
			report( "", "ayit.getInt32( \"artikel_nr\" );", e.what(), false, strm );
		}
		strm << "-----------------------------------------------------------"	<< endl;
		strm << "11. Execute() on an SS_UPDATE state --> SS_CLEAN state." << endl;
		strm << "-----------------------------------------------------------"	<< endl;
		try {
			strm << "11.1 Stream the state(s) in the context of the yiterator." << endl;
			strm << ayit.getState().toString().c_str() << endl; 
			strm << "11.2 Stream the first row of the propertytable (yiterator)." << endl;
			ayit.toStream( strm );
			strm << "11.3 Setting besla_nr=123456 and artikel_nr=12346" << endl;
			strm << "11.4 Setting values <123456, 123456> of attributes <besla_nr, artikel_nr>." << endl;
			ayit.setInt32( "besla_nr",   123456 );
			ayit.setInt32( "artikel_nr", 123456 );
			strm << "11.5 Executing (WriteBLieSort) ..." << endl;
			basar::Int32 written = air.execute( "WriteBLieSort", ayit ).m_affectedRows;
			strm << "11.6 Number of written datasets: " << written << endl;
		} catch( const basar::Exception& e ){
			report( "", "air.execute( \"WriteBLieSort\", ayit );", e.what(), false, strm );
		}
		try {
			strm << "11.7 Stream (after execute()) the state(s) in the context of the yiterator." << endl;
			strm << ayit.getState().toString().c_str() << endl; 
			strm << "11.8 Stream (after execute()) the first row of the propertytable (yiterator)." << endl;
			ayit.toStream( strm );
		} catch( const basar::Exception& e ){
			report( "", "air.execute( \"WriteBLieSort\", ayit );", e.what(), false, strm );
		}

		//---------------------------------------------//
		// state: SS_CLEAN
		//---------------------------------------------//
		strm << "-----------------------------------------------------------"	<< endl;
		strm << "12. MarkForDelete with a SS_UPDATE state." << endl;
		strm << "-----------------------------------------------------------"	<< endl;
		try {
			strm << "12.1 Stream the state(s) in the context of the yiterator." << endl;
			strm << ayit.getState().toString().c_str() << endl; 
			strm << "12.2 Stream the first row of the propertytable (yiterator)." << endl;
			ayit.toStream( strm );
			
			strm << "12.3 MarkForDelete ..." << endl;
			air.getPropertyTable().markForDelete( ayit );
			
			strm << "12.4 Stream the state(s) in the context of the yiterator." << endl;
			strm << ayit.getState().toString().c_str() << endl; 
			strm << "12.5 Stream the first row of the propertytable (yiterator)." << endl;
			ayit.toStream( strm );
		} catch( const basar::Exception& e ){
			report( "", "air.execute( \"WriteBLieSort\", ayit );", e.what(), false, strm );
		}
		strm << "-----------------------------------------------------------"	<< endl;
		strm << "13. Setting value with an SS_DELETE state --> SS_DELETE state." << endl;
		strm << "-----------------------------------------------------------"	<< endl;
		try {
			strm << "13.1 Setting value <9999> at attribute <artikel_nr>." << endl;
			ayit.setInt32( "artikel_nr", 9999 );
			strm << "13.2 Stream the state(s) in the context of the yiterator." << endl;
			strm << ayit.getState().toString().c_str() << endl; 
			strm << "13.3 Stream the first row of the propertytable (yiterator)." << endl;
			ayit.toStream( strm );
		} catch( const basar::Exception& e ){
			report( "", "ayit.setInt32( \"artikel_nr\", 9999 );", e.what(), false, strm );
		}
		strm << "-----------------------------------------------------------"	<< endl;
		strm << "14. Getting value value with an SS_DELETE state --> SS_DELETE state." << endl;
		strm << "-----------------------------------------------------------"	<< endl;
		try {
			basar::Int32 i = ayit.getInt32( "artikel_nr" );
			strm << "14.1 ayit.getInt32( \"artikel_nr\" ): " << i << endl;
			
			strm << "14.2 Stream the state(s) in the context of the yiterator." << endl;
			strm << ayit.getState().toString().c_str() << endl; 
			strm << "14.3 Stream the first row of the propertytable (yiterator)." << endl;
			ayit.toStream( strm );
		} catch( const basar::Exception& e ){
			report( "", "ayit.getInt32( \"artikel_nr\" );", e.what(), false, strm );
		}
		strm << "-----------------------------------------------------------"	<< endl;
		strm << "15. Execute() on an SS_DELETE state." << endl;
		strm << "-----------------------------------------------------------"	<< endl;
		try {
			strm << "15.1 Stream the state(s) in the context of the yiterator." << endl;
			strm << ayit.getState().toString().c_str() << endl; 
			strm << "15.2 Stream the first row of the propertytable (yiterator)." << endl;
			ayit.toStream( strm );
			strm << "15.3 Setting besla_nr=123456 and artikel_nr=12346" << endl;
			ayit.setInt32( "besla_nr",   123456 );
			ayit.setInt32( "artikel_nr", 123456 );
			strm << "15.4 Deleting (DeleteBLieSort) ... " << endl;
			basar::Int32 deleted = air.execute( "DeleteBLieSort", ayit ).m_affectedRows;
			strm << "15.5 Number of deleted datasets: " << deleted << endl;
		} catch( const basar::Exception& e ){
			report( "", "air.execute( \"WriteBLieSort\", ayit );", e.what(), false, strm );
		}
		try {
			strm << "15.6 Number of datasets in the propertytable: " << air.getPropertyTable().size() << endl;
			strm << "15.7 Stream (after execute()) the state(s) in the context of the yiterator." << endl;
			strm << ayit.getState().toString().c_str() << endl; 
			strm << "15.8 Stream (after execute()) the first row of the propertytable (yiterator)." << endl;
			ayit.toStream( strm );
		} catch( const basar::Exception& e ){
			report( "", "air.execute( \"WriteBLieSort\", ayit );", e.what(), false, strm );
		}

	} catch( const basar::Exception& e ){
		report( "", "xxx", e.what(), false, strm );
	}
	strm << "====================================================================" << endl;
	strm << fun << " - Exit!" << endl;
	strm << "====================================================================" << endl;
}
//------------------------------------------------------------------------------------------------------------//
// void check_FOR_DELETE	( AccessorInstanceRef  air, ostream& strm = cout )
//------------------------------------------------------------------------------------------------------------//
void check_FOR_DELETE ( AccessorInstanceRef  air, ostream& strm )
{
	static const char* const fun = "check_FOR_DELETE (AccessorInstanceRef, ostream&)";

	strm << "====================================================================" << endl;
	strm << fun << " - Entry!" << endl;
	strm << "====================================================================" << endl << endl;

	AccessorPropertyTable_YIterator ayit;
	try {
		try {
			strm << "Clearing PropertyTable." << endl;	
			air.getPropertyTable().clear();
		} catch( basar::Exception& e ){
			report( "", "air.getPropertyTable().clear();", e.what(), false, strm );
		} catch( ... ){
			report( "", "air.getPropertyTable().clear();", "Exception ...", false, strm );
		}

		strm << "2. Insert new row into the PropertyTable with InsertType 'basar::FOR_UNKNOWN'." << endl;	
		ayit = air.getPropertyTable().insert( basar::FOR_DELETE );
				
		strm << "-----------------------------------------------------------"	<< endl;
		strm << "3. Getting a value with an SS_DELETE state." << endl;
		strm << "-----------------------------------------------------------"	<< endl;
		try {
			basar::Int32 i = ayit.getInt32( "artikel_nr" );
			strm << "ayit.getInt32( \"artikel_nr\" ): " << i << endl;
		} catch( const basar::Exception& e ){
			report( "", "yit.getInt16( \"spalte1\")", e.what(), false, strm );
		}
		strm << "-----------------------------------------------------------"	<< endl;
		strm << "4. Execute() on an SS_DELETE state." << endl;
		strm << "-----------------------------------------------------------"	<< endl;
		try {
			strm << "4.1 Stream the state(s) in the context of the yiterator." << endl;
			strm << ayit.getState().toString().c_str() << endl; 
			strm << "4.2 Stream the first row of the propertytable (yiterator)." << endl;
			ayit.toStream( strm );
			
			basar::I18nString s("besla_nr=11;" );
			basar::db::aspect::AccessorPropertyTable_YIterator yit = basar::db::aspect::Manager::getInstance().createAccessorYIterator(s);
			air.execute( "WriteBLieSort", yit );
		} catch( const basar::Exception& e ){
			report( "", "air.execute( \"WriteBLieSort\", ayit );", e.what(), false, strm );
		}
		try {
			strm << "4.3 Stream (after execute()) the state(s) in the context of the yiterator." << endl;
			strm << ayit.getState().toString().c_str() << endl; 
			strm << "4.4 Stream (after execute()) the first row of the propertytable (yiterator)." << endl;
			ayit.toStream( strm );
		} catch( const basar::Exception& e ){
			report( "", "air.execute( \"WriteBLieSort\", ayit );", e.what(), false, strm );
		}

		strm << "-----------------------------------------------------------"	<< endl;
		strm << "5. Erase on an SS_DELETE state." << endl;
		strm << "-----------------------------------------------------------"	<< endl;
		try {
			air.getPropertyTable().clear();
			ayit = air.getPropertyTable().insert( basar::FOR_DELETE );
			strm << "5.1 Stream the state(s) in the context of the yiterator." << endl;
			strm << ayit.getState().toString().c_str() << endl; 
			strm << "5.2 Stream the first row of the propertytable (yiterator)." << endl;
			ayit.toStream( strm );
			air.getPropertyTable().erase( ayit );
		} catch( const basar::Exception& e ){
			report( "", "air.getPropertyTable().erase( ayit );", e.what(), false, strm );
		}

		strm << "-----------------------------------------------------------"	<< endl;
		strm << "6. Setting value (32) with an SS_UNSET state --> SS_UNKNOWN state." << endl;
		strm << "-----------------------------------------------------------"	<< endl;
		try {
			air.getPropertyTable().clear();
			ayit = air.getPropertyTable().insert( basar::FOR_DELETE );
			strm << "6.1 Stream the state(s) in the context of the yiterator." << endl;
			strm << ayit.getState().toString().c_str() << endl; 
			strm << "6.2 Stream the first row of the propertytable (yiterator)." << endl;
			ayit.toStream( strm );
			ayit.setInt32( "artikel_nr", 32 );
			strm << "6.3 Stream the state(s) in the context of the yiterator." << endl;
			strm << ayit.getState().toString().c_str() << endl; 
			strm << "6.4 Stream the first row of the propertytable (yiterator)." << endl;
			ayit.toStream( strm );
		} catch( const basar::Exception& e ){
			report( "", "ayit.setInt32( \"artikel_nr\", 32 );", e.what(), false, strm );
		}
		//---//
		strm << "-----------------------------------------------------------"	<< endl;
		strm << "7 Setting value (3232 - second time) with an SS_DELETE state --> SS_DELETE state." << endl;
		strm << "-----------------------------------------------------------"	<< endl;
		try {
			ayit.setInt32( "artikel_nr", 3232 );
			strm << "7.1 Stream the state(s) in the context of the yiterator." << endl;
			strm << ayit.getState().toString().c_str() << endl; 
			strm << "7.2 Stream the first row of the propertytable (yiterator)." << endl;
			ayit.toStream( strm );
		} catch( const basar::Exception& e ){
			report( "", "ayit.setInt32( \"artikel_nr\", 3232 );", e.what(), false, strm );
		}
		strm << "-----------------------------------------------------------"	<< endl;
		strm << "8 Getting a value with an SS_DELETE state." << endl;
		strm << "-----------------------------------------------------------"	<< endl;
		try {
			basar::Int32 i = ayit.getInt32( "artikel_nr" );
			strm << "ayit.getInt32( \"artikel_nr\" ): " << i << endl;
			strm << "8.1 Stream the state(s) in the context of the yiterator." << endl;
			strm << ayit.getState().toString().c_str() << endl; 
			strm << "8.2 Stream the first row of the propertytable (yiterator)." << endl;
			ayit.toStream( strm );
		} catch( const basar::Exception& e ){
			report( "", "ayit.getInt32( \"artikel_nr\" );", e.what(), false, strm );
		}
		strm << "-----------------------------------------------------------"	<< endl;
		strm << "9. Execute() on an SS_DELETE state." << endl;
		strm << "-----------------------------------------------------------"	<< endl;
		try {
			strm << "9.1 Stream the state(s) in the context of the yiterator." << endl;
			strm << ayit.getState().toString().c_str() << endl; 
			strm << "9.2 Stream the first row of the propertytable (yiterator)." << endl;
			ayit.toStream( strm );
			strm << "Setting besla_nr=99 and artikel_nr=99" << endl;
			ayit.setInt32( "besla_nr",   99 );
			ayit.setInt32( "artikel_nr", 99 );
			basar::Int32 deleted = air.execute( "DeleteBLieSort", ayit ).m_affectedRows;
			strm << "Number of deleted rows: " << deleted << endl; 
		} catch( const basar::Exception& e ){
			report( "", "air.execute( \"DeleteBLieSort\", ayit );", e.what(), false, strm );
		}
		try {
			strm << "9.3 Stream (after execute()) the state(s) in the context of the yiterator." << endl;
			strm << ayit.getState().toString().c_str() << endl; 
			strm << "9.4 Stream (after execute()) the first row of the propertytable (yiterator)." << endl;
			ayit.toStream( strm );
		} catch( const basar::Exception& e ){
			report( "", "streaming ayit", e.what(), false, strm );
		}

	} catch( const basar::Exception& e ){
		report( "", "xxx", e.what(), false, strm );
	}

	strm << "====================================================================" << endl;
	strm << fun << " - Exit!" << endl;
	strm << "====================================================================" << endl;
}
//------------------------------------------------------------------------------------------------------------//
// void check_FOR_INSERT ( AccessorInstanceRef  air, ostream& strm = cout )
//------------------------------------------------------------------------------------------------------------//
void check_FOR_INSERT ( AccessorInstanceRef  air, ostream& strm )
{
	static const char* const fun = "check_FOR_INSERT (AccessorInstanceRef, ostream&)";

	strm << "====================================================================" << endl;
	strm << fun << " - Entry!" << endl;
	strm << "====================================================================" << endl << endl;

	AccessorPropertyTable_YIterator ayit;
	try {
		try {
			strm << "1. Clearing PropertyTable." << endl;	
			air.getPropertyTable().clear();
		} catch( basar::Exception& e ){
			report( "", "air.getPropertyTable().clear();", e.what(), false, strm );
		} catch( ... ){
			report( "", "air.getPropertyTable().clear();", "Exception ...", false, strm );
		}

		strm << "2. Insert new row into the PropertyTable with InsertType 'basar::FOR_INSERT'." << endl;	
		ayit = air.getPropertyTable().insert( basar::FOR_INSERT );
				
		strm << "-----------------------------------------------------------"	<< endl;
		strm << "3. Getting a value with an SS_UNSET state." << endl;
		strm << "-----------------------------------------------------------"	<< endl;
		try {
			basar::Int32 i = ayit.getInt32( "artikel_nr" );
			strm << "ayit.getInt32( \"artikel_nr\" ): " << i << endl;
		} catch( const basar::Exception& e ){
			report( "", "yit.getInt16( \"spalte1\")", e.what(), false, strm );
		}
		strm << "-----------------------------------------------------------"	<< endl;
		strm << "4. Execute() on an SS_UNSET state." << endl;
		strm << "-----------------------------------------------------------"	<< endl;
		try {
			strm << "4.1 Stream the state(s) in the context of the yiterator." << endl;
			strm << ayit.getState().toString().c_str() << endl; 
			strm << "4.2 Stream the first row of the propertytable (yiterator)." << endl;
			ayit.toStream( strm );
			
			basar::I18nString s("besla_nr=11;" );
			basar::db::aspect::AccessorPropertyTable_YIterator yit = basar::db::aspect::Manager::getInstance().createAccessorYIterator(s);
			air.execute( "WriteBLieSort", yit );
		} catch( const basar::Exception& e ){
			report( "", "air.execute( \"WriteBLieSort\", ayit );", e.what(), false, strm );
		}
		try {
			strm << "4.3 Stream (after execute()) the state(s) in the context of the yiterator." << endl;
			strm << ayit.getState().toString().c_str() << endl; 
			strm << "4.4 Stream (after execute()) the first row of the propertytable (yiterator)." << endl;
			ayit.toStream( strm );
		} catch( const basar::Exception& e ){
			report( "", "air.execute( \"WriteBLieSort\", ayit );", e.what(), false, strm );
		}

		strm << "-----------------------------------------------------------"	<< endl;
		strm << "5. Erase on an SS_UNSET state." << endl;
		strm << "-----------------------------------------------------------"	<< endl;
		try {
			air.getPropertyTable().clear();
			ayit = air.getPropertyTable().insert( basar::FOR_INSERT );
			strm << "5.1 Stream the state(s) in the context of the yiterator." << endl;
			strm << ayit.getState().toString().c_str() << endl; 
			strm << "5.2 Stream the first row of the propertytable (yiterator)." << endl;
			ayit.toStream( strm );
			air.getPropertyTable().erase( ayit );
		} catch( const basar::Exception& e ){
			report( "", "air.getPropertyTable().erase( ayit );", e.what(), false, strm );
		}

		strm << "-----------------------------------------------------------"	<< endl;
		strm << "6. Setting value (32) with an SS_UNSET state --> SS_INSERT state." << endl;
		strm << "-----------------------------------------------------------"	<< endl;
		try {
			air.getPropertyTable().clear();
			ayit = air.getPropertyTable().insert( basar::FOR_INSERT );
			strm << "6.1 Stream the state(s) in the context of the yiterator." << endl;
			strm << ayit.getState().toString().c_str() << endl; 
			strm << "6.2 Stream the first row of the propertytable (yiterator)." << endl;
			ayit.toStream( strm );
			strm << "6.3 Setting a value: 'ayit.setInt32( \"artikel_nr\", 32 ).'" << endl;
			ayit.setInt32( "artikel_nr", 32 );
			strm << "6.4 Stream the state(s) in the context of the yiterator." << endl;
			strm << ayit.getState().toString().c_str() << endl; 
			strm << "6.5 Stream the first row of the propertytable (yiterator)." << endl;
			ayit.toStream( strm );
		} catch( const basar::Exception& e ){
			report( "", "ayit.setInt32( \"artikel_nr\", 32 );", e.what(), false, strm );
		}
		//---//
		strm << "-----------------------------------------------------------"	<< endl;
		strm << "7. Setting value (3232 - second time) with an SS_INSERT state --> SS_INSERT state." << endl;
		strm << "-----------------------------------------------------------"	<< endl;
		try {
			strm << "7.1 Setting a value: 'ayit.setInt32( \"artikel_nr\", 3232 )'." << endl;
			ayit.setInt32( "artikel_nr", 3232 );
			strm << "7.2 Stream the state(s) in the context of the yiterator." << endl;
			strm << ayit.getState().toString().c_str() << endl; 
			strm << "7.3 Stream the first row of the propertytable (yiterator)." << endl;
			ayit.toStream( strm );
		} catch( const basar::Exception& e ){
			report( "", "ayit.setInt32( \"artikel_nr\", 3232 );", e.what(), false, strm );
		}
		strm << "-----------------------------------------------------------"	<< endl;
		strm << "8. Getting a value with an SS_INSERT state." << endl;
		strm << "-----------------------------------------------------------"	<< endl;
		try {
			strm << "8.1 Getting a value: 'basar::Int32 i = ayit.getInt32( \"artikel_nr\" )'." << endl;
			basar::Int32 i = ayit.getInt32( "artikel_nr" );
			strm << "ayit.getInt32( \"artikel_nr\" ): " << i << endl;
			strm << "8.2 Stream the state(s) in the context of the yiterator." << endl;
			strm << ayit.getState().toString().c_str() << endl; 
			strm << "8.3 Stream the first row of the propertytable (yiterator)." << endl;
			ayit.toStream( strm );
		} catch( const basar::Exception& e ){
			report( "", "ayit.getInt32( \"artikel_nr\" );", e.what(), false, strm );
		}
		strm << "-----------------------------------------------------------"	<< endl;
		strm << "9. Execute() on an SS_INSERT state --> SS_CLEAN state." << endl;
		strm << "-----------------------------------------------------------"	<< endl;
		try {
			strm << "9.1 Stream the state(s) in the context of the yiterator." << endl;
			strm << ayit.getState().toString().c_str() << endl; 
			strm << "9.2 Stream the first row of the propertytable (yiterator)." << endl;
			ayit.toStream( strm );
			strm << "Setting besla_nr=99 and artikel_nr=99" << endl;
			ayit.setInt32( "besla_nr",   99 );
			ayit.setInt32( "artikel_nr", 99 );
			basar::Int32 deleted = air.execute( "DeleteBLieSort", ayit ).m_affectedRows;
			strm << "Number of deleted rows: " << deleted << endl; 
		} catch( const basar::Exception& e ){
			report( "", "air.execute( \"DeleteBLieSort\", ayit );", e.what(), false, strm );
		}
		try {
			strm << "9.3 Stream (after execute()) the state(s) in the context of the yiterator." << endl;
			strm << ayit.getState().toString().c_str() << endl; 
			strm << "9.4 Stream (after execute()) the first row of the propertytable (yiterator)." << endl;
			ayit.toStream( strm );
		} catch( const basar::Exception& e ){
			report( "", "streaming ayit", e.what(), false, strm );
		}
		//---//
		strm << "-----------------------------------------------------------"	<< endl;
		strm << " 10. Getting a value with an SS_CLEAN state." << endl;
		strm << "-----------------------------------------------------------"	<< endl;
		try {
			strm << "10.1 ayit.getInt32( \"artikel_nr\" ): " << ayit.getInt32( "artikel_nr" ) << endl;
			strm << "10.2 Stream the state(s) in the context of the yiterator." << endl;
			strm << ayit.getState().toString().c_str() << endl; 
			strm << "10.3 Stream the first row of the propertytable (yiterator)." << endl;
			ayit.toStream( strm );
		} catch( const basar::Exception& e ){
			report( "", "ayit.getInt32( \"artikel_nr\" );", e.what(), false, strm );
		}
		strm << "-----------------------------------------------------------"	<< endl;
		strm << " 11. Execute() on an SS_CLEAN state --> SS_CLEAN state." << endl;
		strm << "-----------------------------------------------------------"	<< endl;
		try {
			strm << "11.4 Stream the state(s) in the context of the yiterator." << endl;
			strm << ayit.getState().toString().c_str() << endl; 
			strm << "11.5 Stream the first row of the propertytable (yiterator)." << endl;
			ayit.toStream( strm );
			strm << "11.6 Setting besla_nr=99 and artikel_nr=99" << endl;
			ayit.setInt32( "besla_nr",   99 );
			ayit.setInt32( "artikel_nr", 99 );
			strm << "11.7 Setting besla_nr=99 and artikel_nr=99" << endl;
			basar::Int32 deleted = air.execute( "DeleteBLieSort", ayit ).m_affectedRows;
			strm << "Number of deleted rows: " << deleted << endl; 
		} catch( const basar::Exception& e ){
			report( "", "air.execute( \"DeleteBLieSort\", ayit );", e.what(), false, strm );
		}
		try {
			strm << "11.8 Stream (after execute()) the state(s) in the context of the yiterator." << endl;
			strm << ayit.getState().toString().c_str() << endl; 
			strm << "11.9 Stream (after execute()) the first row of the propertytable (yiterator)." << endl;
			ayit.toStream( strm );
		} catch( const basar::Exception& e ){
			report( "", "streaming ayit", e.what(), false, strm );
		}
		//---//
		strm << "-----------------------------------------------------------"	<< endl;
		strm << " 12. Setting a value SS_CLEAN state --> SS_UPDATE state." << endl;
		strm << "-----------------------------------------------------------"	<< endl;
		try {
			strm << "12.1 Stream the state(s) in the context of the yiterator." << endl;
			strm << ayit.getState().toString().c_str() << endl; 
			strm << "12.2 Stream the first row of the propertytable (yiterator)." << endl;
			ayit.toStream( strm );

			strm << "12.3 Setting besla_nr=123123" << endl;
			ayit.setInt32( "besla_nr",   123132 );
			
			strm << "12.4 Stream the state(s) in the context of the yiterator." << endl;
			strm << ayit.getState().toString().c_str() << endl; 
			strm << "12.5 Stream the first row of the propertytable (yiterator)." << endl;
			ayit.toStream( strm );
		} catch( const basar::Exception& e ){
			report( "", "air.execute( \"DeleteBLieSort\", ayit );", e.what(), false, strm );
		}
		strm << "-----------------------------------------------------------"	<< endl;
		strm << " 13. Setting a value (second time) SS_UPDATE state --> SS_UPDATE state." << endl;
		strm << "-----------------------------------------------------------"	<< endl;
		try {
			strm << "13.1 Stream the state(s) in the context of the yiterator." << endl;
			strm << ayit.getState().toString().c_str() << endl; 
			strm << "13.2 Stream the first row of the propertytable (yiterator)." << endl;
			ayit.toStream( strm );

			strm << "13.3 Setting besla_nr=456456" << endl;
			ayit.setInt32( "besla_nr",   456456 );
			
			strm << "13.4 Stream the state(s) in the context of the yiterator." << endl;
			strm << ayit.getState().toString().c_str() << endl; 
			strm << "13.5 Stream the first row of the propertytable (yiterator)." << endl;
			ayit.toStream( strm );
		} catch( const basar::Exception& e ){
			report( "", "air.execute( \"DeleteBLieSort\", ayit );", e.what(), false, strm );
		}
		strm << "-----------------------------------------------------------"	<< endl;
		strm << " 14. Getting a value SS_UPDATE state --> SS_UPDATE state." << endl;
		strm << "-----------------------------------------------------------"	<< endl;
		try {
			strm << "14.1 Stream the state(s) in the context of the yiterator." << endl;
			strm << ayit.getState().toString().c_str() << endl; 
			strm << "14.2 Stream the first row of the propertytable (yiterator)." << endl;
			ayit.toStream( strm );

			strm << "14.3 ayit.getInt32( \"besla_nr\" ): " << ayit.getInt32( "besla_nr" ) << endl;
			
			strm << "14.4 Stream the state(s) in the context of the yiterator." << endl;
			strm << ayit.getState().toString().c_str() << endl; 
			strm << "14.5 Stream the first row of the propertytable (yiterator)." << endl;
			ayit.toStream( strm );
		} catch( const basar::Exception& e ){
			report( "", "air.execute( \"DeleteBLieSort\", ayit );", e.what(), false, strm );
		}
		strm << "-----------------------------------------------------------"	<< endl;
		strm << " 15. MarkForDelete SS_UPDATE state --> SS_DELETE state." << endl;
		strm << "-----------------------------------------------------------"	<< endl;
		try {
			strm << "15.1 Stream the state(s) in the context of the yiterator." << endl;
			strm << ayit.getState().toString().c_str() << endl; 
			strm << "15.2 Stream the first row of the propertytable (yiterator)." << endl;
			ayit.toStream( strm );

			strm << "15.3 air.getPropertytable().markForDelete( ayit ): " << endl;
			air.getPropertyTable().markForDelete( ayit );
			
			strm << "15.4 Stream the state(s) in the context of the yiterator." << endl;
			strm << ayit.getState().toString().c_str() << endl; 
			strm << "15.5 Stream the first row of the propertytable (yiterator)." << endl;
			ayit.toStream( strm );
		} catch( const basar::Exception& e ){
			report( "", "air.execute( \"DeleteBLieSort\", ayit );", e.what(), false, strm );
		}
		strm << "-----------------------------------------------------------"	<< endl;
		strm << " 16. Getting a value SS_DELETE state --> SS_DELETE state." << endl;
		strm << "-----------------------------------------------------------"	<< endl;
		try {
			strm << "16.1 Stream the state(s) in the context of the yiterator." << endl;
			strm << ayit.getState().toString().c_str() << endl; 
			strm << "16.2 Stream the first row of the propertytable (yiterator)." << endl;
			ayit.toStream( strm );

			strm << "16.3 ayit.getInt32( \"besla_nr\" ): " << ayit.getInt32( "besla_nr" ) << endl;
			
			strm << "16.4 Stream the state(s) in the context of the yiterator." << endl;
			strm << ayit.getState().toString().c_str() << endl; 
			strm << "16.5 Stream the first row of the propertytable (yiterator)." << endl;
			ayit.toStream( strm );
		} catch( const basar::Exception& e ){
			report( "", "air.execute( \"DeleteBLieSort\", ayit );", e.what(), false, strm );
		}
		strm << "-----------------------------------------------------------"	<< endl;
		strm << " 17. Setting a value SS_DELETE state --> SS_DELETE state." << endl;
		strm << "-----------------------------------------------------------"	<< endl;
		try {
			strm << "17.1 Stream the state(s) in the context of the yiterator." << endl;
			strm << ayit.getState().toString().c_str() << endl; 
			strm << "17.2 Stream the first row of the propertytable (yiterator)." << endl;
			ayit.toStream( strm );

			strm << "17.3 Setting besla_nr=789789" << endl;
			ayit.setInt32( "besla_nr",   789789 );
			
			strm << "17.4 Stream the state(s) in the context of the yiterator." << endl;
			strm << ayit.getState().toString().c_str() << endl; 
			strm << "17.5 Stream the first row of the propertytable (yiterator)." << endl;
			ayit.toStream( strm );
		} catch( const basar::Exception& e ){
			report( "", "air.execute( \"DeleteBLieSort\", ayit );", e.what(), false, strm );
		}
	} catch( const basar::Exception& e ){
		report( "", "xxx", e.what(), false, strm );
	}

	strm << "====================================================================" << endl;
	strm << fun << " - Exit!" << endl;
	strm << "====================================================================" << endl;	
}
