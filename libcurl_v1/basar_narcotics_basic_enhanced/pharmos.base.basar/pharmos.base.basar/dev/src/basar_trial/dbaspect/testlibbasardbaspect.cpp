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

#include <libbasardbsql_connectionlist_iterator.h>

using std::string;
using std::ostream;
using std::ofstream;
using std::ostringstream;
using std::boolalpha;

void measure_cics();
void connect_mem();

//--------------------------------------------------------------------------------------------------------//
// macros
//--------------------------------------------------------------------------------------------------------//
namespace acc1 
{
//--------------------------------------------------------------------------------------//
// Query Builder 'ArticleSearch'
//--------------------------------------------------------------------------------------//
BEGIN_QUERY_BUILDER_DECLARATION( ArticleSearch )
END_BUILDER_DECLARATION

BUILDER_DEFINITION( ArticleSearch )
	ENSURE_PROPERTIES_ARE_SET( ArticleSearch,"artikel_nr;" )
	SQL_PATTERN(ArticleSearch, 
				"select z.artikel_nr as artikel_nr, artikel_name, filialnr, bestand "
				"from zartikel z, artikelf f "
				"where z.artikel_nr = f.artikel_nr "
				"and z.artikel_nr = ?artikel_nr? "
				"and f.filialnr = ?filialnr? "
				"order by filialnr")

//--------------------------------------------------------------------------------------//
// Query Builder 'ArticleSearchViaName'
//--------------------------------------------------------------------------------------//
BEGIN_QUERY_BUILDER_DECLARATION( ArticleSearchViaName )
END_BUILDER_DECLARATION

BUILDER_DEFINITION( ArticleSearchViaName )
	ENSURE_PROPERTIES_ARE_SET( ArticleSearchViaName, "artikel_name;" )
	SQL_PATTERN( ArticleSearchViaName, "select z.artikel_nr as artikel_nr, artikel_name, filialnr, bestand "
										"from zartikel z, artikelf f "
										"where z.artikel_nr = f.artikel_nr and "
										"z.artikel_name like ?artikel_name? order by filialnr")
//--------------------------------------------------------------------------------------//
// Write Builder 'ArticleStockWriter'
//--------------------------------------------------------------------------------------//
BEGIN_WRITE_BUILDER_DECLARATION( ArticleStockWriter )
END_BUILDER_DECLARATION

BUILDER_DEFINITION( ArticleStockWriter )
	ENSURE_PROPERTIES_ARE_SET_AND_ONE_IN_STATE( ArticleStockWriter, "bestand;", "UPDATE" )
	SQL_PATTERN( ArticleStockWriter,	"UPDATE artikelf "
										"SET	bestand		= ?bestand? "
										"WHERE	artikel_nr	= ?artikel_nr? "
										"AND	filialnr	= ?filialnr?" )
//--------------------------------------------------------------------------------------//
BEGIN_ACCESSOR_DEFINITION("AccessorArticle")
	PROPERTY_DESCRIPTION_LIST(	"int32 artikel_nr; string artikel_name; int16 filialnr; int32 bestand;")

	ACCESS_METHOD( "ReadArticle" )
		SQL_BUILDER_CREATE( ArticleSearch )
		SQL_BUILDER_PUSH_BACK( ArticleSearch )

	ACCESS_METHOD( "ReadArticleViaName" )
		SQL_BUILDER_CREATE( ArticleSearchViaName )
		SQL_BUILDER_PUSH_BACK( ArticleSearchViaName )

	ACCESS_METHOD( "UpdateArticleStock" )
		SQL_BUILDER_CREATE( ArticleStockWriter )
		SQL_BUILDER_PUSH_BACK( ArticleStockWriter )

END_ACCESSOR_DEFINITION
}

//-----------------------------------------------------------------------------------------------------------//
namespace acc2 {
BEGIN_QUERY_BUILDER_DECLARATION( QueryBLieSort )
END_BUILDER_DECLARATION

BEGIN_WRITE_BUILDER_DECLARATION( WriteBLieSort )
END_BUILDER_DECLARATION

BEGIN_WRITE_BUILDER_DECLARATION( DeleteBLieSort )
END_BUILDER_DECLARATION

BEGIN_WRITE_BUILDER_DECLARATION( InsertBLieSort )
END_BUILDER_DECLARATION

BEGIN_ACCESSOR_DEFINITION( "AccessorBLieSort" )
	PROPERTY_DESCRIPTION_LIST(	"int32 besla_nr; int32 artikel_nr;")

	ACCESS_METHOD( "SelBLieSort" )
		SQL_BUILDER_CREATE( QueryBLieSort )
		SQL_BUILDER_PUSH_BACK( QueryBLieSort )

	ACCESS_METHOD( "WriteBLieSort" )
		SQL_BUILDER_CREATE( WriteBLieSort )
		SQL_BUILDER_PUSH_BACK( WriteBLieSort )

	ACCESS_METHOD( "DeleteBLieSort" )
		SQL_BUILDER_CREATE( DeleteBLieSort )
		SQL_BUILDER_PUSH_BACK( DeleteBLieSort )


	ACCESS_METHOD( "InsertBLieSort" )
		SQL_BUILDER_CREATE( InsertBLieSort )
		SQL_BUILDER_PUSH_BACK( InsertBLieSort )

END_ACCESSOR_DEFINITION


BUILDER_DEFINITION( QueryBLieSort )
	ENSURE_PROPERTIES_ARE_SET( QueryBLieSort, "artikel_nr; besla_nr;" )
	SQL_PATTERN( QueryBLieSort,	"select * from bliesort where besla_nr = ?besla_nr? and artikel_nr = ?artikel_nr?")
	
BUILDER_DEFINITION( WriteBLieSort )
	ENSURE_PROPERTIES_ARE_SET( WriteBLieSort, "besla_nr;" )
	SQL_PATTERN( WriteBLieSort,	"update bliesort set besla_nr = ?besla_nr? "
								"where artikel_nr = ?artikel_nr?")

BUILDER_DEFINITION( DeleteBLieSort )
	ENSURE_PROPERTIES_ARE_SET( DeleteBLieSort, "artikel_nr;" )
	SQL_PATTERN( DeleteBLieSort,	"delete from bliesort where besla_nr = ?besla_nr? and artikel_nr = ?artikel_nr?" )

BUILDER_DEFINITION( InsertBLieSort )
	ENSURE_PROPERTIES_ARE_SET( InsertBLieSort, "artikel_nr; besla_nr;" )
	SQL_PATTERN( InsertBLieSort,	"insert into bliesort( besla_nr, artikel_nr ) values( ?besla_nr?, ?artikel_nr? )") 
	
}

//--------------------------------------------------------------------------------------------------------//
// enum section
//--------------------------------------------------------------------------------------------------------//
enum Increment_or_DecrementEnum
{
	PRE_INCREMENT,
	POST_INCREMENT,
	PRE_DECREMENT,
	POST_DECREMENT
};

struct Parameter
{
	bool withHits;
	Increment_or_DecrementEnum eIncrement_or_Decrement;
} g_objParameter;


//--------------------------------------------------------------------------------------------------------//
// forward declaration functions
//--------------------------------------------------------------------------------------------------------//
void					printPackageInfo();
ConnectionRef			checkBBConnection();
AccessorInstanceRef		checkBBAccessorInstanceRef( ConnectionRef connection, CachingPolicyEnum );
AccessorInstanceRef		checkBLieSortAccessorInstanceRef( ConnectionRef connection, CachingPolicyEnum );
void					checkCachingPolicies( AccessorInstanceRef air, CachingPolicyEnum eCachingPolicy, const char* what = "" );
void					check_FULL_CACHING_Policy( AccessorInstanceRef air );
void					check_ON_DEMAND_CACHING_Policy( AccessorInstanceRef air );
void 					check_SINGLE_ROW_CACHING_Policy( AccessorInstanceRef air );
void 					check_NO_CACHING_Policy( AccessorInstanceRef air );
void 					checkBBTransaction( AccessorInstanceRef air );
void 					checkBBSnapshots( AccessorInstanceRef air );
void 					showElements( AccessorInstanceRef air, const char* what = "" );
std::string				selectOption();
void 					report( std::string fun, std::string testcase, std::string exceptInfo = "", bool ok = true, ostream& strm = std::cout); 
void 					check_AccessorPropertyTable( AccessorInstanceRef air, string hits = "" );
void 					check_AccessorPropertyTable_Iterators( AccessorInstanceRef air, string hits = "" );
void 					check_Properties( AccessorInstanceRef air );
void 					check_Transaction( ConnectionRef connection, AccessorInstanceRef air );
void 					check_empty( ConnectionRef connection, AccessorInstanceRef air );
void 					check_Accessor( ConnectionRef connection, AccessorInstanceRef air );
void 					check_Filtered_Iteration( ConnectionRef connection, AccessorInstanceRef air );
void 					check_Filtered_Erase( ConnectionRef connection, AccessorInstanceRef air );
void 					check_YIterator_XIterator( ConnectionRef connection, AccessorInstanceRef air );
void 					check_Transactions( ConnectionRef connection, AccessorInstanceRef air );
void 					check_Inserts( ConnectionRef connection, AccessorInstanceRef air );
void					check_praepost_indecrement( ConnectionRef connection, AccessorInstanceRef air );
void					check_filtered_step( ConnectionRef connection, AccessorInstanceRef air );
void					check_const( ConnectionRef connection, AccessorInstanceRef air );
void					check_States( ConnectionRef connection, AccessorInstanceRef air );
void					check_BoostAny_Union( ConnectionRef connection, AccessorInstanceRef air );
void					doFastExecute( ConnectionRef  connection, AccessorInstanceRef  air );
void					doInsertUpdate( ConnectionRef connection, AccessorInstanceRef air );
void					check_policies( ConnectionRef connection, AccessorInstanceRef air );
void					check_Basar_Any( ConnectionRef connection, AccessorInstanceRef air );

void checkErasePropertyTable()
{
	static const char* const fun = "checkErasePropertyTable";

	cout << "====================================================================" << endl;
	cout << fun << " - Entry!" << endl;
	cout << "====================================================================" << endl << endl;

#define SPALTE_1 "name1"
	basar::cmnutil::ParameterList pl( "name1=wert1;" ); 
	AccessorPropertyTable_YIterator yit = basar::db::aspect::Manager::getInstance().createAccessorYIterator( pl );
	basar::db::aspect::AccessorPropertyTableRef apt = yit.getPropertyTable();
	int i;
	for( i = 0; i < 10; ++i ){
		apt.insert( basar::FOR_UNKNOWN );
	}

	cout << "apt.size(): " << apt.size() << endl;

	std::ostringstream os;
	yit = apt.begin();
	i = 0;
	for( ; !yit.isEnd(); ++yit, ++i ){
		os.str("");
		os << i << endl;
		yit.setString( "name1", os.str().c_str() );
	}

	yit = apt.begin();
	for( ; !yit.isEnd(); ++yit ){
		cout << SPALTE_1 << " " << yit.getString( "name1" ) << endl;
	}

	try {
		yit = apt.begin();
		/*if( !yit.isEnd() ){	
			++yit;
		}*/
		while( !yit.isEnd() ){
			++yit;
			if( !yit.isEnd() ){
				cout	<< "akt. yit (wird geloescht): " 
						<< yit.getString( SPALTE_1 ) << ";" << endl;
				yit = apt.erase( yit );
				cout	<< "zurueckgegebener yit (nach Loeschen): " 
						<< yit.getString( SPALTE_1 ).c_str() << ";" << endl;
			}
		}
	} catch( const basar::Exception& e ){
		cout << e.what() << endl;
	} catch( ...  ){
		cout << "unknown exception!" << endl;
	}

	cout << "====================================================================" << endl;
	cout << fun << " - Entry!" << endl;
	cout << "====================================================================" << endl << endl;

}

//--------------------------------------------------------------------------------------------------------//
// int main(int argc, char * argv[])
//--------------------------------------------------------------------------------------------------------//
int main( int argc, char * argv[] )
{
	static const char* const fun = "main(int, char *)";

	//checkErasePropertyTable();
	//measure_cics();
	connect_mem();

	printf("press key to quit\n");
	getchar();
	return 0;

#ifdef XYZ

	using basar::cmnutil::DateTime;
	DateTime date( DateTime::getCurrent().getDate(), 0 );
	DateTime date2;
	basar::VarString strDate;

	strDate = "01.01.2007";
	date2.fromStrDate(strDate);
	
	cout << boolalpha << strDate.c_str() << ". " << date2.isValidDate() << endl;

	strDate = "0101.2007";
	date2.fromStrDate(strDate);
	cout << boolalpha << strDate.c_str() << ". " << date2.isValidDate() << endl;

	strDate = "01.01.007";
	date2.fromStrDate(strDate);
	cout << boolalpha << strDate.c_str() << ". " << date2.isValidDate() << endl;

	strDate = "w1.01.2007";
	date2.fromStrDate(strDate);
	cout << boolalpha << strDate.c_str() << ". " << date2.isValidDate() << endl;

	strDate = "01.01.200w";
	date2.fromStrDate(strDate);
	cout << boolalpha << strDate.c_str() << ". " << date2.isValidDate() << endl;

	strDate = "willi";
	date2.fromStrDate(strDate);
	cout << boolalpha << strDate.c_str() << ". " << date2.isValidDate() << endl;

	return 0;

#endif
//date.convToDate( const Int16  day, const Int16 month, Int16 year);
//date.convToDate(  );

//date.addDate( 1 );
//cout << date.toDateStd() << endl;
//
//date.addDate( -2 );
//cout << date.toDateStd() << endl;

//getchar();
//return 0;

	argc = argc;
	argv = argv;

	//eIncrement_or_Decrement = PRE_INCREMENT;
	//eIncrement_or_Decrement = POST_INCREMENT;
	//g_objParameter.eIncrement_or_Decrement = PRE_DECREMENT;
	g_objParameter.eIncrement_or_Decrement = POST_DECREMENT;


	// read in the options	  
	string s;// = selectOption();
	//if( "q" == s ) return 0;

	try {
		printPackageInfo();

		// create connection
		basar::db::aspect::ConnectionRef connection = checkBBConnection();
		
		// create AccessorInstanceRef 
		AccessorInstanceRef air;
			
		/*basar::cmnutil::ParameterList pl2( "name3; name2;" ); 
		basar::cmnutil::ParameterList_Iterator it = pl2.begin();
		for( ; it != pl2.end(); ++it ){
			cout << it->first.c_str() << endl;
		}*/


		/*basar::cmnutil::ParameterList pl( "name1=wert1; name2=wert2; name3=wert3;" ); 
		AccessorPropertyTable_YIterator yit = basar::db::aspect::Manager::getInstance().createAccessorYIterator( pl );
		cout << "------------------------------" << endl;
		yit.toStream();
		cout << "------------------------------" << endl;
		AccessorPropertyTable_YIterator yitCloned;
		yitCloned= yit.cloneToUnboundedPropertyRow();
		cout << "------------------------------" << endl;
		yitCloned.toStream();
		cout << "------------------------------" << endl;*/
		
		{
			/*basar::cmnutil::ParameterList pl3( "k1=w1; name2=wert2; name3=wert3;" ); 
			AccessorPropertyTable_YIterator yitCloned2 = basar::db::aspect::Manager::getInstance().createAccessorYIterator( pl3 );
			cout << "------------------------------" << endl;
			yitCloned2.toStream();
			cout << "------------------------------" << endl;
			if( yit.compareSettedValues( yitCloned2 ) ){
				cout << "yit.compareSettedValues( yitCloned )== true" << endl;
			} else {
				cout << "yit.compareSettedValues( yitCloned )== false" << endl;
			}*/
		}

		{
			//yit.setString( "name1", "veraendert" );
			////yit.setString( "name3", "veraendert" );
			//basar::cmnutil::ParameterList pl( "name1=wert1; name2=wert2; name3=wert3;" ); 
			//cout << "------------------------------" << endl;
			//pl.toStream();
			//cout << "------------------------------" << endl;
			//std::pair< bool, basar::cmnutil::ParameterList > ret( yit.compareSetValues(yitCloned) );
			//if( ret.first ){
			//	cout << "yit.compareSettedValues( yitCloned )== true" << endl;
			//} else {
			//	cout << "yit.compareSettedValues( yitCloned )== false" << endl << endl;
			//	ret.second.toStream();
			//}
		}

		
		//AccessorPropertyTable_YIterator yit1, yit2;
		//try {
		//	yit2 = yit1.cloneToUnboundedPropertyRow(); // here an assertion happened
		//} catch( basar::Exception& e ){
		//	cout << e.what().c_str() << endl;
		//} catch( ... ){
		//	cout << "Unspecified exception!" << endl;
		//}

		/*int xxx = 123;

		try {
			basar::db::aspect::PropertyStateSet pss = yitCloned.getState();
		} catch ( const basar::Exception& e ){
			cout << e.what() << endl;
		}
		int i = 99;*/

		//doInsertUpdate( connection, air );
		//doFastExecute( connection, air );

		//-----------------------------------------------------------------------------------//
		// check_policies( connection, air );
		//-----------------------------------------------------------------------------------//
		check_policies( connection, air );

		//-----------------------------------------------------------------------------------//
		// check_BoostAny_Union( connection, air );
		//-----------------------------------------------------------------------------------//
		//check_BoostAny_Union( connection, air );

		//-----------------------------------------------------------------------------------//
		// checkBBSnapshots( air );
		//-----------------------------------------------------------------------------------//
		//checkBBSnapshots( air );

		//-----------------------------------------------------------------------------------//
		// check_States( connection, air );
		//-----------------------------------------------------------------------------------//
		//check_States( connection, air );

		//-----------------------------------------------------------------------------------//
		// check_Accessor( connection, air );
		//-----------------------------------------------------------------------------------//
		//check_Accessor( connection, air );
		
		//-----------------------------------------------------------------------------------//
		// check_Filtered_Iteration( connection, air );
		//-----------------------------------------------------------------------------------//
		//check_Filtered_Iteration( connection, air );

		//-----------------------------------------------------------------------------------//
		// check_Filtered_Erase( connection, air );
		//-----------------------------------------------------------------------------------//
		//check_Filtered_Erase( connection, air );

		//-----------------------------------------------------------------------------------//
		// check_YIterator_XIterator( connection, air );
		//-----------------------------------------------------------------------------------//
		//check_YIterator_XIterator( connection, air );

		//-----------------------------------------------------------------------------------//
		// check_Transaction( connection, air );
		//-----------------------------------------------------------------------------------//
		//check_Transactions( connection, air );

		//-----------------------------------------------------------------------------------//
		// check_Inserts( connection, air );
		//-----------------------------------------------------------------------------------//
		//check_Inserts( connection, air );

		//-----------------------------------------------------------------------------------//
		// 	check_praepost_indecrement( connection, air );
		//-----------------------------------------------------------------------------------//
		//check_praepost_indecrement( connection, air );

		//-----------------------------------------------------------------------------------//
		// 	check_filtered_step( connection, air );
		//-----------------------------------------------------------------------------------//
		//check_filtered_step( connection, air );

		//-----------------------------------------------------------------------------------//
		// 	check_const( connection, air );
		//-----------------------------------------------------------------------------------//
		//check_const( connection, air );

	} catch( ... )
	{
		cout << fun << "EXCEPTION (...)" << endl;
	}

	basar::db::aspect::Manager::clear();
	//printf("press 'e' and return\n");
	getchar();

	return 0;
}
//--------------------------------------------------------------------------------------------------------//
// void doInsertUpdate( ConnectionRef connection, AccessorInstanceRef air )
//--------------------------------------------------------------------------------------------------------//
void doInsertUpdate( ConnectionRef connection, AccessorInstanceRef air )
{
	
	air = checkBLieSortAccessorInstanceRef( connection, basar::db::aspect::FULL_CACHING );
	AccessorPropertyTable_YIterator ayit;
	ayit = air.getPropertyTable().insert( basar::FOR_UPDATE );

	try {
		cout << "1. Stream the state(s) in the context of the yiterator." << endl;
		cout << ayit.getState().toString().c_str() << endl; 
		cout << "2. Stream the first row of the propertytable (yiterator)." << endl;
		ayit.toStream( cout );
	} catch ( const basar::Exception& e){
		cout << e.what() << endl;
	}
	
	// UPDATE //
	try {
		//ayit.setInt32( "artikel_nr", 0 );
		ayit.setInt32( "besla_nr", 11 );

		cout << "3. Stream (before execute()) the state(s) in the context of the yiterator." << endl;
		cout << ayit.getState().toString().c_str() << endl; 
		cout << "4. Stream (before execute()) the first row of the propertytable (yiterator)." << endl;
		ayit.toStream( cout );
		
		basar::BULong r = air.execute( "WriteBLieSort", ayit ).m_affectedRows;
		cout << "5. Rueckgabe con execute() Stream : " << r << endl;

		try {
			cout << "6. Stream (after execute()) the state(s) in the context of the yiterator." << endl;
			cout << ayit.getState().toString().c_str() << endl; 
		} catch ( const basar::Exception& e){
			cout << e.what() << endl;
		}
		try {
			cout << "7. Stream (after execute()) the first row of the propertytable (yiterator)." << endl;
			ayit.toStream( cout );
		} catch ( const basar::Exception& e){
			cout << e.what() << endl;
		}

		cout << endl << "--------------------------------------------------------------------------" << endl;
		// DELETE //
		air.getPropertyTable().clear();
		cout << "ayit = air.getPropertyTable().insert( basar::FOR_DELETE );" << endl;
		ayit = air.getPropertyTable().insert( basar::FOR_DELETE );
		ayit.setInt32( "artikel_nr", 99 );
		ayit.setInt32( "besla_nr", 99 );

		cout << "3. Stream (before execute(delete)) the state(s) in the context of the yiterator." << endl;
		cout << ayit.getState().toString().c_str() << endl; 
		cout << "4. Stream (before execute(delete)) the first row of the propertytable (yiterator)." << endl;
		ayit.toStream( cout );
		
		cout << "air.getPropertyTable().markForDelete( ayit );" << endl;
		air.getPropertyTable().markForDelete( ayit );
		cout << "5. Stream (before execute(delete)) the state(s) in the context of the yiterator." << endl;
		cout << ayit.getState().toString().c_str() << endl; 
		cout << "6. Stream (before execute(delete)) the first row of the propertytable (yiterator)." << endl;
		ayit.toStream( cout );

		r = air.execute( "DeleteBLieSort", ayit ).m_affectedRows;
		cout << "7. Rueckgabe von execute() Stream : " << r << endl;

		try {
			cout << "8. Stream (after execute()) the state(s) in the context of the yiterator." << endl;
			cout << ayit.getState().toString().c_str() << endl; 
		} catch ( const basar::Exception& e){
			cout << e.what() << endl;
		}
		try {
			cout << "9. Stream (after execute()) the first row of the propertytable (yiterator)." << endl;
			ayit.toStream( cout );
		} catch ( const basar::Exception& e){
			cout << e.what() << endl;
		}

		cout << endl << "--------------------------------------------------------------------------" << endl;
		// INSERT //
		ayit.setInt32( "artikel_nr", 99 );
		ayit.setInt32( "besla_nr", 99 );

		cout << "10. Stream (before execute()) the state(s) in the context of the yiterator." << endl;
		cout << ayit.getState().toString().c_str() << endl; 
		cout << "11. Stream (before execute()) the first row of the propertytable (yiterator)." << endl;
		ayit.toStream( cout );

		r = air.execute( "InsertBLieSort", ayit ).m_affectedRows;
		cout << "12. Rueckgabe von execute() Stream : " << r << endl;

		try {
			cout << "13. Stream (after execute()) the state(s) in the context of the yiterator." << endl;
			cout << ayit.getState().toString().c_str() << endl; 
		} catch ( const basar::Exception& e){
			cout << e.what() << endl;
		}
		try {
			cout << "14. Stream (after execute()) the first row of the propertytable (yiterator)." << endl;
			ayit.toStream( cout );
		} catch ( const basar::Exception& e){
			cout << e.what() << endl;
		}


	} catch ( const basar::Exception& e){
		cout << e.what() << endl;
	}

}
//--------------------------------------------------------------------------------------------------------//
// void doFastExecute( ConnectionRef  connection, AccessorInstanceRef  air )
//--------------------------------------------------------------------------------------------------------//
void doFastExecute( ConnectionRef  connection, AccessorInstanceRef  air )
{
	try {
		air = checkBBAccessorInstanceRef( connection, basar::db::aspect::NO_CACHING );
		basar::I18nString s("artikel_nr = 1057;" );
		basar::db::aspect::AccessorPropertyTable_YIterator yit = basar::db::aspect::Manager::getInstance().createAccessorYIterator(s);
		basar::BULong ret = air.execute( "ReadArticle", yit ).m_affectedRows;
		cout << "ret: " << ret << endl;
		if( 0 >= ret ){
			yit = air.getPropertyTable().begin(); 
			cout << "yit.getString( \"artikel_name\" ): " << yit.getString( "artikel_name" ) << endl;
		}

	} catch( const basar::Exception& e ){
		cout << e.what() << endl;
	} catch( ...  ){
		cout << "Exception ..." << endl;
	}
}
//--------------------------------------------------------------------------------------------------------//
// check accessor
//--------------------------------------------------------------------------------------------------------//
AccessorInstanceRef checkBBAccessorInstanceRef( ConnectionRef connection, CachingPolicyEnum eCachingPolicy )
{
	AccessorInstanceRef air;
	try {
		air = basar::db::aspect::Manager::getInstance().createAccessorInstance(
				"xxx", "AccessorArticle", connection, eCachingPolicy );		
	} catch( const basar::Exception& ex ){
		cout << ex.what() << endl;
	}
	
	return air;
}
//--------------------------------------------------------------------------------------------------------//
// check accessor
//--------------------------------------------------------------------------------------------------------//
AccessorInstanceRef checkBLieSortAccessorInstanceRef( ConnectionRef connection, CachingPolicyEnum eCachingPolicy )
{
	AccessorInstanceRef air;
	try {
		air = basar::db::aspect::Manager::getInstance().createAccessorInstance(
				"bls", "AccessorBLieSort", connection, eCachingPolicy );		
	} catch( const basar::Exception& ex ){
		cout << ex.what() << endl;
	}
	
	return air;
}
//--------------------------------------------------------------------------------------------------------//
// void check_empty( ConnectionRef connection, AccessorInstanceRef air )
//--------------------------------------------------------------------------------------------------------//
void check_empty( ConnectionRef connection, AccessorInstanceRef air )
{
	static const char* const fun = "check_empty( ConnectionRef connection, AccessorInstanceRef air )";
	report( fun, "Entry!" );

	for( int i = 0; i < 2; ++i ){
		for( int i = 0; i < 4; ++i ){
			//------------------------------------------------------------------------------------//
			// caching policy
			//------------------------------------------------------------------------------------//
			cout << "caching policy: ";
			switch( i ){
				case 0:
						cout << "NO_CACHING!" << endl;
						air = checkBBAccessorInstanceRef( connection, basar::db::aspect::NO_CACHING );			
						break;
				case 1:
						cout << "SINGLE_ROW_CACHING!" << endl;
						air = checkBBAccessorInstanceRef( connection, basar::db::aspect::SINGLE_ROW_CACHING );			
						break;
				case 2:
						cout << "ON_DEMAND_CACHING!" << endl;
						air = checkBBAccessorInstanceRef( connection, basar::db::aspect::ON_DEMAND_CACHING );			
						break;
				case 3:
						cout << "FULL_CACHING!" << endl;
						air = checkBBAccessorInstanceRef( connection, basar::db::aspect::FULL_CACHING );			
						break;
				default:
					throw "What policy?";
			}
		
			basar::db::aspect::AccessorPropertyTable_YIterator yit;
			if( 0 == i ){
				cout << "Query with hits!" << endl;
				air.getPropertyTable().clear();
				basar::I18nString s("artikel_nr=1057;" );
				 yit = basar::db::aspect::Manager::getInstance().createAccessorYIterator(s);
				air.execute("ReadArticle", yit);
			} else {
				cout << "Query without hits!" << endl;
				air.getPropertyTable().clear();
				basar::I18nString s("artikel_nr=9999999;" );
				yit = basar::db::aspect::Manager::getInstance().createAccessorYIterator(s);
				air.execute("ReadArticle", yit);
			}

			//----------------------------------------------------------------------------//
			report( "", "transaction with rollback" );
			//----------------------------------------------------------------------------//
			report( "", "air.getPropertyTable().empty();", "", air.getPropertyTable().empty() );			
			try {
				report( "", "AccessorPropertyTable_YIterator yit = air.getPropertyTable().begin();" );
				yit = air.getPropertyTable().begin();	
			} catch( const basar::Exception& e ){
				report( "", "yit = air.getPropertyTable().begin();", e.what(), false );
			}
			report( "", "air.getPropertyTable().empty();", "", air.getPropertyTable().empty() );			
			
			try {
				report( "", "for( ; yit != air.getPropertyTable().end(); ++ yit );" );
				for( ; yit != air.getPropertyTable().end(); ++ yit );
			} catch( const basar::Exception& e ){
				report( "", "for( ; yit != air.getPropertyTable().end(); ++ yit );", e.what(), false );
			}

			report( "", "air.getPropertyTable().empty();", "", air.getPropertyTable().empty() );			
			
		} // end for caching policies
	} // end for hits
	report( fun, "Exit!" );
}
//--------------------------------------------------------------------------------------------------------//
// void check_Transaction( AccessorInstanceRef air )
//--------------------------------------------------------------------------------------------------------//
void check_Transaction( ConnectionRef connection, AccessorInstanceRef air )
{
	static const char* const fun = "check_Transaction( ConnectionRef connection, AccessorInstanceRef air )";
	report( fun, "Entry!" );

	for( int i = 0; i < 4; ++i ){
		//------------------------------------------------------------------------------------//
		// caching policy
		//------------------------------------------------------------------------------------//
		switch( i ){
			case 0:
					air = checkBBAccessorInstanceRef( connection, basar::db::aspect::NO_CACHING );			
					try {
						air.switchCreateInternalSnapshot( true );
					} catch( const basar::Exception& e ){
						report( "", "air.switchCreateInternalSnapshot( true );", e.what(), false );
						continue;
					}
					break;
			case 1:
					air = checkBBAccessorInstanceRef( connection, basar::db::aspect::SINGLE_ROW_CACHING );			
					try {
						air.switchCreateInternalSnapshot( true );
					} catch( const basar::Exception& e ){
						report( "", "air.switchCreateInternalSnapshot( true );", e.what(), false );
						continue;
					}
					break;
			case 2:
					air = checkBBAccessorInstanceRef( connection, basar::db::aspect::ON_DEMAND_CACHING );			
					try {
						air.switchCreateInternalSnapshot( true );
					} catch( const basar::Exception& e ){
						report( "", "air.switchCreateInternalSnapshot( true );", e.what(), false );
						continue;
					}
					break;
			case 3:
					air = checkBBAccessorInstanceRef( connection, basar::db::aspect::FULL_CACHING );			
					try {
						air.switchCreateInternalSnapshot( false );
					} catch( const basar::Exception& e ){
						report( "", "air.switchCreateInternalSnapshot( true );", e.what(), false );
						continue;
					}
					break;
			default:
				throw "What policy?";
		}
		basar::I18nString s("artikel_nr=1057;" );
		AccessorPropertyTable_YIterator yit = basar::db::aspect::Manager::getInstance().createAccessorYIterator(s);

		//----------------------------------------------------------------------------//
		report( "", "transaction with rollback" );
		//----------------------------------------------------------------------------//
		report( "", "air.transactionBegin();" );
		air.beginTransaction();
		try {
			report( "", "air.execute(\"ReadArticle\");" );
			air.execute("ReadArticle", yit);
		} catch( const basar::Exception& e ){
			report( "", "yIt = air.getPropertyTable().end()", e.what(), false );
		
		}
		report( "", "air.transactionRollback();", "", false );
		air.rollbackTransaction();
		air.getPropertyTable().clear();

		//----------------------------------------------------------------------------//
		report( "", "transaction with commit" );
		//----------------------------------------------------------------------------//
		report( "", "air.transactionBegin();" );
		air.beginTransaction();
		try {
			report( "", "air.execute(\"ReadArticle\");" );
			air.execute("ReadArticle");
		} catch( const basar::Exception& e ){
			report( "", "yIt = air.getPropertyTable().end()", e.what(), false );
		
		}
		report( "", "air.transactionCommit();", "", false );
		air.commitTransaction();
		air.getPropertyTable().clear();
	}

	report( fun, "Exit!" );
}
//--------------------------------------------------------------------------------------------------------//
// void check_Properties( AccessorInstanceRef air, AccessorPropertyTable_YIterator yit )
//--------------------------------------------------------------------------------------------------------//
void check_Properties( AccessorInstanceRef air )
{
	static const char* const fun = "check_Properties(AccessorInstanceRef, AccessorPropertyTable_YIterator)";
	report( fun, "Entry!" );

	bool propertiesSet = air.getPropertyTable().begin().arePropertiesSet( "artikel_nr=1057;" ); 
	propertiesSet;

	report( fun, "Exit!" );
}
//--------------------------------------------------------------------------------------------------------//
// package info
//--------------------------------------------------------------------------------------------------------//
void printPackageInfo()
{
	printf("ver : %s\n", basar::db::aspect::getVersion    ());
//	printf("pkg : %s\n", basar::db::aspect::getPkgVersion ());
	printf("full: %s\n", basar::db::aspect::getFullVersion());
}
//--------------------------------------------------------------------------------------------------------//
// void report( std::string& fun, std::string testcase, std::string excepInfo )
//--------------------------------------------------------------------------------------------------------//
void report( std::string fun, std::string testcase, std::string info, bool ok, ostream& strm )
{
	strm << endl;

	if( !fun.empty() ){
		strm << "----------------------------------------------------------------------------" << endl;
		strm << "Function: " << fun << endl;
		strm << "----------------------------------------------------------------------------" << endl;
	}

	strm << "Testfall: " << testcase << ":\t";

	if( !ok ){
		if( !info.empty() ){
			strm << endl << "* * * Exception * * *" << endl;
			strm << ">" << info << "<" << endl;
		} else {
			strm << ">NICHT OK<" << endl;
		}	
	} else {
		if( !info.empty() ){
			strm << ">" << info << "<" << endl;
		} else {
			strm << ">OK<" << endl;
		}
	}
}
//--------------------------------------------------------------------------------------------------------//
// void check_AccessorPropertyTable( AccessorInstanceRef air, string hits = "" )
//--------------------------------------------------------------------------------------------------------//
void check_AccessorPropertyTable_Iterators( AccessorInstanceRef air, string hits )
{
	const char* fun = "check_AccessorPropertyTable_Iterators( AccessorInstanceRef, string )";
	
	AccessorPropertyTable_YIterator yIt;
	ostringstream os; 
	int i = 0;
	bool toBreak = false;






	//-----------------------------------------------------------------------------------------------//
	// PRE_INCREMENT
	//-----------------------------------------------------------------------------------------------//
	if( PRE_INCREMENT == g_objParameter.eIncrement_or_Decrement )
	{
		cout << "PRE_INCREMENT == eIncrement_or_Decrement --> not implemented" << endl;
	} 
	//-----------------------------------------------------------------------------------------------//
	// POST_INCREMENT
	//-----------------------------------------------------------------------------------------------//
	else if( POST_INCREMENT == g_objParameter.eIncrement_or_Decrement )
	{
		cout << "POST_INCREMENT == eIncrement_or_Decrement --> not implemented" << endl;
	}
	//-----------------------------------------------------------------------------------------------//
	// PRE_DECREMENT
	//-----------------------------------------------------------------------------------------------//
	else if( PRE_DECREMENT == g_objParameter.eIncrement_or_Decrement )
	{
		//-------------------------------------------------------//
		// with clear()
		//-------------------------------------------------------//
		report( fun, "air.getPropertyTable().clear()" );
		air.getPropertyTable().clear();
		os.str("");
		os << static_cast<unsigned>( air.getPropertyTable().size() );
		report( fun, "air.getPropertyTable().size()", os.str().c_str() );
		i = 0;
		
		try {
			yIt = air.getPropertyTable().end();
			report( "", "yIt = air.getPropertyTable().end()");
		} catch( const basar::Exception& ex ){
			report( "", "yIt = air.getPropertyTable().end()", ex.what(), false );
		}
		try {
			--yIt;
			report( "", "--yIt;");
		} catch( const basar::Exception& ex ){
			report( "", "--yIt;", ex.what(), false );
		}

		toBreak = false;
		while( true ){
			if( yIt == air.getPropertyTable().begin() ){
				try {
					os.str("");
					os << yIt.getInt32( "artikel_nr" );
					report( "", "yIt.getInt32( 'artikel_nr')", os.str().c_str() );
				} catch( const basar::Exception& ex ){
					report( "", "yIt.getInt32( 'artikel_nr' )", ex.what(), false );
				}
				try {
					yIt.setInt32( "artikel_nr" , 1234567 );
					report( "", "yIt.setInt32( 'artikel_nr' , 1234567 )" );
				} catch( const basar::Exception& ex ){
					report( "", "yIt.setInt32( 'artikel_nr' , 1234567 )", ex.what(), false );
				}
				try {
					os.str("");
					os << yIt.getString( "artikel_name" );
					report( "", "yIt.getString( 'artikel_name' )", os.str().c_str() );
				} catch( const basar::Exception& ex ){
					report( "", "yIt.getString( 'artikel_name' )", ex.what(), false );
				}
				try {
					os.str("");
					os << yIt.getInt16( "filialnr" );
					report( "", "yIt.getInt16( 'filialnr' )", os.str().c_str() );
				} catch( const basar::Exception& ex ){
					report( "", "yIt.getInt16( 'filialnr' )", ex.what(), false );
				}
				try {
					os.str("");
					os << yIt.getInt32( "bestand" );
					report( "", "yIt.getInt32( 'bestand' )", os.str().c_str() );
				} catch( const basar::Exception& ex ){
					report( "", "yIt.getInt32( 'bestand' )", ex.what(), false );
				}
					
				toBreak = true;
			} 
			if( true == toBreak ){
				break;
			}
			try {
				os.str("");
				os << yIt.getInt32( "artikel_nr"	);
				report( fun, "yIt.getInt32( 'artikel_nr')", os.str().c_str() );
			} catch( const basar::Exception& ex ){
				report( fun, "yIt.getInt32( 'artikel_nr' )", ex.what(), false );
			}
			try {
				os.str("");
				os << yIt.getString( "artikel_name" );
				report( "", "yIt.getString( 'artikel_name' )", os.str().c_str() );
			} catch( const basar::Exception& ex ){
				report( "", "yIt.getString( 'artikel_name' )", ex.what(), false );
			}
			try {
				os.str("");
				os << yIt.getInt16( "filialnr" );
				report( "", "yIt.getInt16( 'filialnr' )", os.str().c_str() );
			} catch( const basar::Exception& ex ){
				report( "", "yIt.getInt16( 'filialnr' )", ex.what(), false );
			}
			try {
				os.str("");
				os << yIt.getInt32( "bestand" );
				report( "", "yIt.getInt32( 'bestand' )", os.str().c_str() );
			} catch( const basar::Exception& ex ){
				report( "", "yIt.getInt32( 'bestand' )", ex.what(), false );
			}
			
			--yIt;

		} // end while
		//-------------------------------------------------------//
		// without clear()
		//-------------------------------------------------------//
		report( fun, "air.execute()" );
		air.execute("ReadArticle");
		os.str("");
		os << static_cast<unsigned>( air.getPropertyTable().size() );
		report( fun, "air.getPropertyTable().size()", os.str().c_str() );
		i = 0;
		// end()
		try {
			yIt = air.getPropertyTable().end();
			report( "", "yIt = air.getPropertyTable().end()");
		} catch( const basar::Exception& ex ){
			report( "", "yIt = air.getPropertyTable().end()", ex.what(), false );
		}
		try {
			--yIt;
			report( "", "--yIt;");
		} catch( const basar::Exception& ex ){
			report( "", "--yIt;", ex.what(), false );
		}

		// begin()
		try {
			yIt = air.getPropertyTable().begin();
			report( "", "yIt = air.getPropertyTable().begin()");
		} catch( const basar::Exception& ex ){
			report( "", "yIt = air.getPropertyTable().begin()", ex.what(), false );
		}
		try {
			os.str("");
			os << yIt.getInt32( "artikel_nr" );
			report( "", "yIt.getInt32( 'artikel_nr')", os.str().c_str() );
		} catch( const basar::Exception& ex ){
			report( "", "yIt.getInt32( 'artikel_nr' )", ex.what(), false );
		}
		try {
			yIt.setInt32( "artikel_nr" , 1234567 );
			report( "", "yIt.setInt32( 'artikel_nr' , 1234567 )" );
		} catch( const basar::Exception& ex ){
			report( "", "yIt.setInt32( 'artikel_nr' , 1234567 )", ex.what(), false );
		}

		toBreak = false;
		while( true ){
			if( yIt == air.getPropertyTable().begin() ){
				try {
					os.str("");
					os << yIt.getInt32( "artikel_nr" );
					report( "", "yIt.getInt32( 'artikel_nr')", os.str().c_str() );
				} catch( const basar::Exception& ex ){
					report( "", "yIt.getInt32( 'artikel_nr' )", ex.what(), false );
				}
				try {
					yIt.setInt32( "artikel_nr" , 1234567 );
					report( "", "yIt.setInt32( 'artikel_nr' , 1234567 )" );
				} catch( const basar::Exception& ex ){
					report( "", "yIt.setInt32( 'artikel_nr' , 1234567 )", ex.what(), false );
				}
				try {
					os.str("");
					os << yIt.getString( "artikel_name" );
					report( "", "yIt.getString( 'artikel_name' )", os.str().c_str() );
				} catch( const basar::Exception& ex ){
					report( "", "yIt.getString( 'artikel_name' )", ex.what(), false );
				}
				try {
					os.str("");
					os << yIt.getInt16( "filialnr" );
					report( "", "yIt.getInt16( 'filialnr' )", os.str().c_str() );
				} catch( const basar::Exception& ex ){
					report( "", "yIt.getInt16( 'filialnr' )", ex.what(), false );
				}
				try {
					os.str("");
					os << yIt.getInt32( "bestand" );
					report( "", "yIt.getInt32( 'bestand' )", os.str().c_str() );
				} catch( const basar::Exception& ex ){
					report( "", "yIt.getInt32( 'bestand' )", ex.what(), false );
				}
					
				toBreak = true;
			} 
			if( true == toBreak ){
				break;
			}
			try {
				os.str("");
				os << yIt.getInt32( "artikel_nr"	);
				report( fun, "yIt.getInt32( 'artikel_nr')", os.str().c_str() );
			} catch( const basar::Exception& ex ){
				report( fun, "yIt.getInt32( 'artikel_nr' )", ex.what(), false );
			}
			try {
				os.str("");
				os << yIt.getString( "artikel_name" );
				report( "", "yIt.getString( 'artikel_name' )", os.str().c_str() );
			} catch( const basar::Exception& ex ){
				report( "", "yIt.getString( 'artikel_name' )", ex.what(), false );
			}
			try {
				os.str("");
				os << yIt.getInt16( "filialnr" );
				report( "", "yIt.getInt16( 'filialnr' )", os.str().c_str() );
			} catch( const basar::Exception& ex ){
				report( "", "yIt.getInt16( 'filialnr' )", ex.what(), false );
			}
			try {
				os.str("");
				os << yIt.getInt32( "bestand" );
				report( "", "yIt.getInt32( 'bestand' )", os.str().c_str() );
			} catch( const basar::Exception& ex ){
				report( "", "yIt.getInt32( 'bestand' )", ex.what(), false );
			}
			
			--yIt;

		} // end while

	}
	//-----------------------------------------------------------------------------------------------//
	// POST_DECREMENT
	//-----------------------------------------------------------------------------------------------//
	else if( POST_DECREMENT == g_objParameter.eIncrement_or_Decrement )
	{
		//-------------------------------------------------------//
		// with clear()
		//-------------------------------------------------------//
		report( fun, "air.getPropertyTable().clear()" );
		air.getPropertyTable().clear();
		os.str("");
		os << static_cast<unsigned>( air.getPropertyTable().size() );
		report( fun, "air.getPropertyTable().size()", os.str().c_str() );
		i = 0;
		
		try {
			yIt = air.getPropertyTable().end();
			report( "", "yIt = air.getPropertyTable().end()");
		} catch( const basar::Exception& ex ){
			report( "", "yIt = air.getPropertyTable().end()", ex.what(), false );
		}
		try {
			yIt--;
			report( "", "--yIt;");
		} catch( const basar::Exception& ex ){
			report( "", "--yIt;", ex.what(), false );
		}

		report( "", "yIt == air.getPropertyTable().begin()", "", yIt == air.getPropertyTable().begin() );

		toBreak = false;
		while( true ){
			if( yIt == air.getPropertyTable().begin() ){
				try {
					os.str("");
					os << yIt.getInt32( "artikel_nr" );
					report( "", "yIt.getInt32( 'artikel_nr')", os.str().c_str() );
				} catch( const basar::Exception& ex ){
					report( "", "yIt.getInt32( 'artikel_nr' )", ex.what(), false );
				}
				try {
					yIt.setInt32( "artikel_nr" , 1234567 );
					report( "", "yIt.setInt32( 'artikel_nr' , 1234567 )" );
				} catch( const basar::Exception& ex ){
					report( "", "yIt.setInt32( 'artikel_nr' , 1234567 )", ex.what(), false );
				}
				try {
					os.str("");
					os << yIt.getString( "artikel_name" );
					report( "", "yIt.getString( 'artikel_name' )", os.str().c_str() );
				} catch( const basar::Exception& ex ){
					report( "", "yIt.getString( 'artikel_name' )", ex.what(), false );
				}
				try {
					os.str("");
					os << yIt.getInt16( "filialnr" );
					report( "", "yIt.getInt16( 'filialnr' )", os.str().c_str() );
				} catch( const basar::Exception& ex ){
					report( "", "yIt.getInt16( 'filialnr' )", ex.what(), false );
				}
				try {
					os.str("");
					os << yIt.getInt32( "bestand" );
					report( "", "yIt.getInt32( 'bestand' )", os.str().c_str() );
				} catch( const basar::Exception& ex ){
					report( "", "yIt.getInt32( 'bestand' )", ex.what(), false );
				}
					
				toBreak = true;
			} 
			if( true == toBreak ){
				break;
			}
			try {
				os.str("");
				os << yIt.getInt32( "artikel_nr"	);
				report( fun, "yIt.getInt32( 'artikel_nr')", os.str().c_str() );
			} catch( const basar::Exception& ex ){
				report( fun, "yIt.getInt32( 'artikel_nr' )", ex.what(), false );
			}
			try {
				os.str("");
				os << yIt.getString( "artikel_name" );
				report( "", "yIt.getString( 'artikel_name' )", os.str().c_str() );
			} catch( const basar::Exception& ex ){
				report( "", "yIt.getString( 'artikel_name' )", ex.what(), false );
			}
			try {
				os.str("");
				os << yIt.getInt16( "filialnr" );
				report( "", "yIt.getInt16( 'filialnr' )", os.str().c_str() );
			} catch( const basar::Exception& ex ){
				report( "", "yIt.getInt16( 'filialnr' )", ex.what(), false );
			}
			try {
				os.str("");
				os << yIt.getInt32( "bestand" );
				report( "", "yIt.getInt32( 'bestand' )", os.str().c_str() );
			} catch( const basar::Exception& ex ){
				report( "", "yIt.getInt32( 'bestand' )", ex.what(), false );
			}
			
			yIt--;

		} // end while
		//-------------------------------------------------------//
		// without clear()
		//-------------------------------------------------------//
		report( fun, "air.execute()" );
		air.execute("ReadArticle");
		os.str("");
		os << static_cast<unsigned>( air.getPropertyTable().size() );
		report( fun, "air.getPropertyTable().size()", os.str().c_str() );
		i = 0;
		// end()
		try {
			yIt = air.getPropertyTable().end();
			report( "", "yIt = air.getPropertyTable().end()");
		} catch( const basar::Exception& ex ){
			report( "", "yIt = air.getPropertyTable().end()", ex.what(), false );
		}
		try {
			yIt--;
			report( "", "--yIt;");
		} catch( const basar::Exception& ex ){
			report( "", "--yIt;", ex.what(), false );
		}

		report( "", "yIt == air.getPropertyTable().begin()", "", yIt == air.getPropertyTable().begin() );
		
		// begin()
		try {
			yIt = air.getPropertyTable().begin();
			report( "", "yIt = air.getPropertyTable().begin()");
		} catch( const basar::Exception& ex ){
			report( "", "yIt = air.getPropertyTable().begin()", ex.what(), false );
		}
		try {
			os.str("");
			os << yIt.getInt32( "artikel_nr" );
			report( "", "yIt.getInt32( 'artikel_nr')", os.str().c_str() );
		} catch( const basar::Exception& ex ){
			report( "", "yIt.getInt32( 'artikel_nr' )", ex.what(), false );
		}
		try {
			yIt.setInt32( "artikel_nr" , 1234567 );
			report( "", "yIt.setInt32( 'artikel_nr' , 1234567 )" );
		} catch( const basar::Exception& ex ){
			report( "", "yIt.setInt32( 'artikel_nr' , 1234567 )", ex.what(), false );
		}

		toBreak = false;
		while( true ){
			if( yIt == air.getPropertyTable().begin() ){
				try {
					os.str("");
					os << yIt.getInt32( "artikel_nr" );
					report( "", "yIt.getInt32( 'artikel_nr')", os.str().c_str() );
				} catch( const basar::Exception& ex ){
					report( "", "yIt.getInt32( 'artikel_nr' )", ex.what(), false );
				}
				try {
					yIt.setInt32( "artikel_nr" , 1234567 );
					report( "", "yIt.setInt32( 'artikel_nr' , 1234567 )" );
				} catch( const basar::Exception& ex ){
					report( "", "yIt.setInt32( 'artikel_nr' , 1234567 )", ex.what(), false );
				}
				try {
					os.str("");
					os << yIt.getString( "artikel_name" );
					report( "", "yIt.getString( 'artikel_name' )", os.str().c_str() );
				} catch( const basar::Exception& ex ){
					report( "", "yIt.getString( 'artikel_name' )", ex.what(), false );
				}
				try {
					os.str("");
					os << yIt.getInt16( "filialnr" );
					report( "", "yIt.getInt16( 'filialnr' )", os.str().c_str() );
				} catch( const basar::Exception& ex ){
					report( "", "yIt.getInt16( 'filialnr' )", ex.what(), false );
				}
				try {
					os.str("");
					os << yIt.getInt32( "bestand" );
					report( "", "yIt.getInt32( 'bestand' )", os.str().c_str() );
				} catch( const basar::Exception& ex ){
					report( "", "yIt.getInt32( 'bestand' )", ex.what(), false );
				}
					
				toBreak = true;
			} 
			if( true == toBreak ){
				break;
			}
			try {
				os.str("");
				os << yIt.getInt32( "artikel_nr"	);
				report( fun, "yIt.getInt32( 'artikel_nr')", os.str().c_str() );
			} catch( const basar::Exception& ex ){
				report( fun, "yIt.getInt32( 'artikel_nr' )", ex.what(), false );
			}
			try {
				os.str("");
				os << yIt.getString( "artikel_name" );
				report( "", "yIt.getString( 'artikel_name' )", os.str().c_str() );
			} catch( const basar::Exception& ex ){
				report( "", "yIt.getString( 'artikel_name' )", ex.what(), false );
			}
			try {
				os.str("");
				os << yIt.getInt16( "filialnr" );
				report( "", "yIt.getInt16( 'filialnr' )", os.str().c_str() );
			} catch( const basar::Exception& ex ){
				report( "", "yIt.getInt16( 'filialnr' )", ex.what(), false );
			}
			try {
				os.str("");
				os << yIt.getInt32( "bestand" );
				report( "", "yIt.getInt32( 'bestand' )", os.str().c_str() );
			} catch( const basar::Exception& ex ){
				report( "", "yIt.getInt32( 'bestand' )", ex.what(), false );
			}
			
			yIt--;

		} // end while
	}
}
//--------------------------------------------------------------------------------------------------------//
// void check_AccessorPropertyTable( AccessorInstanceRef air )
//--------------------------------------------------------------------------------------------------------//
void check_AccessorPropertyTable( AccessorInstanceRef air, string hits )
{
	const char* fun = "check_AccessorPropertyTable( AccessorInstanceRef )";

	using basar::db::aspect::AccessorPropertyTableRef;
	using basar::db::aspect::CachingPolicyEnum;

	//if( g_objParameter.withHits ){
		air.execute( "ReadArticle" );
	//}

	AccessorPropertyTableRef aptr = air.getPropertyTable();
	switch( aptr.getCachingPolicy() )
	{
		case basar::db::aspect::FULL_CACHING:		
				report( fun, "Caching Policy: 'FULL_CACHING' - " + hits );
				break;
		case basar::db::aspect::SINGLE_ROW_CACHING:	
				report( fun, "Caching Policy: 'SINGLE_ROW_CACHING' - " + hits );
				break;
		case basar::db::aspect::ON_DEMAND_CACHING:	
				report( fun, "Caching Policy: 'ON_DEMAND_CACHING' - " + hits );
				break;
		case basar::db::aspect::NO_CACHING:			
				report( fun, "Caching Policy: 'NO_CACHING' - " + hits );
				break;
		default:									
				std::string s( "Keine bekannt Caching Policy! - " + hits );
				throw s;
				break;
	}
	
	ostringstream os;
	for( int i = 0; i < 2; ++i ){
		if( 1 == i ){
			try {
				AccessorPropertyTable_YIterator yit = aptr.begin();
				for( ; yit != aptr.end(); ++yit );
				report( "", "for( ; yit != aptr.end(); ++yit )" );
			} catch( const basar::Exception& e ){
				report( "", "for( ; yit != aptr.end(); ++yit )", e.what(), false );
			}
		}
		try {
			os.str("");
			os << static_cast<unsigned>( aptr.size() );
			report( "", "aptr.size()", os.str().c_str() );
		} catch( const basar::Exception& e ){
			report( "", "aptr.size()", e.what(), false );
		}
		
		try {
			os.str("");
			os << (aptr.empty() ? "true" : "false") ;
			report( "", "aptr.empty()", os.str().c_str() );
		} catch( const basar::Exception& e ){
			report( "", "aptr.empty()", e.what(), false );
		}
	}

	try {
		os.str("");
		os << static_cast<unsigned>( aptr.max_size() );
		report( "", "aptr.max_size()", os.str().c_str() );
	} catch( const basar::Exception& e ){
		report( "", "aptr.max_size()", e.what(), false );
	}


	try {
		aptr.markForDelete( aptr.begin() );
		report( "", "aptr.markForDelete( aptr.begin() )" );
	} catch( const basar::Exception& e ){
		report( "", "aptr.markForDelete( aptr.begin() )", e.what(), false );
	}
	try {
		aptr.erase( aptr.begin() );
		report( "", "aptr.erase( aptr.begin() )" );
	} catch( const basar::Exception& e ){
		report( "", "aptr.erase( aptr.begin() )", e.what(), false );
	}
	try {
		os.str("");
		os << static_cast<unsigned>( aptr.size() );
		report( "", "aptr.size()", os.str().c_str() );
	} catch( const basar::Exception& e ){
		report( "", "aptr.size()", e.what(), false );
	}
	try {
		aptr.erase( aptr.begin(), aptr.end() );
		report( "", "aptr.erase( aptr.begin(), aptr.end() )" );
	} catch( const basar::Exception& e ){
		report( "", "aptr.erase( aptr.begin(), aptr.end() )", e.what(), false );
	}

	try {
		os.str("");
		os << static_cast<unsigned>( aptr.size() );
		report( "", "aptr.size()", os.str().c_str() );
	} catch( const basar::Exception& e ){
		report( "", "aptr.size()", e.what(), false );
	}

	//----------------//
	// CLEAR
	//----------------//
	try {
		aptr.clear();
		report( "", "aptr.clear()" );
	} catch( const basar::Exception& e ){
		report( "", "aptr.clear()", e.what(), false );
	}
	
	try {
		aptr.begin();
		report( "", "aptr.begin()" );
	} catch( const basar::Exception& e ){
		report( "", "aptr.begin()", e.what(), false );
	}
	try {
		aptr.begin();
		report( "", "aptr.begin()" );
	} catch( const basar::Exception& e ){
		report( "", "aptr.begin()", e.what(), false );
	}
	
	try {
		aptr.end();
		report( "", "aptr.end()" );
	} catch( const basar::Exception& e ){
		report( "", "aptr.end()", e.what(), false );
	}
	
	try {
		aptr.begin( basar::SS_UPDATE );
		report( "", "aptr.begin(basar::property::SS_UPDATE)" );
	} catch( const basar::Exception& e ){
		report( "", "aptr.begin(basar::property::SS_UPDATE)", e.what(), false );
	}

	// INSERTS
	try {
		aptr.insert( basar::FOR_CLEAN );
		report( "", "aptr.insert( FOR_CLEAN )" );
	} catch( const basar::Exception& e ){
		report( "", "aptr.insert( FOR_CLEAN )", e.what(), false );
	}
	try {
		aptr.insert( basar::FOR_INSERT );
		report( "", "aptr.insert( FOR_INSERT )" );
	} catch( const basar::Exception& e ){
		report( "", "aptr.insert( FOR_INSERT )", e.what(), false );
	}
	try {
		aptr.insert( basar::FOR_UPDATE );
		report( "", "aptr.insert( FOR_UPDATE )" );
	} catch( const basar::Exception& e ){
		report( "", "aptr.insert( FOR_UPDATE )", e.what(), false );
	}
	try {
		aptr.insert( basar::FOR_DELETE );
		report( "", "aptr.insert( FOR_DELETE )" );
	} catch( const basar::Exception& e ){
		report( "", "aptr.insert( FOR_DELETE )", e.what(), false );
	}
	
	try {
		aptr.markForDelete( aptr.begin() );
		report( "", "aptr.markForDelete( aptr.begin() )" );
	} catch( const basar::Exception& e ){
		report( "", "aptr.markForDelete( aptr.begin() )", e.what(), false );
	}
	try {
		aptr.erase( aptr.begin() );
		report( "", "aptr.erase( aptr.begin() )" );
	} catch( const basar::Exception& e ){
		report( "", "aptr.erase( aptr.begin() )", e.what(), false );
	}
	try {
		aptr.erase( aptr.begin(), aptr.end() );
		report( "", "aptr.erase( aptr.begin(), aptr.end() )" );
	} catch( const basar::Exception& e ){
		report( "", "aptr.erase( aptr.begin(), aptr.end() )", e.what(), false );
	}
	try {
		aptr.clear();
		report( "", "aptr.clear()" );
	} catch( const basar::Exception& e ){
		report( "", "aptr.clear()", e.what(), false );
	}

	//if( g_objParameter.withHits ){
		air.execute("ReadArticle");
	//}

	try {
		os.str("");
		os << static_cast<unsigned>( aptr.size() );
		report( "", "aptr.size()", os.str().c_str() );
	} catch( const basar::Exception& e ){
		report( "", "aptr.size()", e.what(), false );
	}
}
//--------------------------------------------------------------------------------------------------------//
// void selectOption()
//--------------------------------------------------------------------------------------------------------//
string selectOption()
{
	cout << "####################################################" << endl;
	cout << "# fh: FULL_CACHING with hits                       #" << endl;
	cout << "# oh: ON_DEMAND_CACHING with hits                  #" << endl;
	cout << "# sh: SINGLE_ROW_CACHING with hits                 #" << endl;
	cout << "# nh: NO_CACHING with hits                         #" << endl;
	cout << "# fn: FULL_CACHING with no hits                    #" << endl;
	cout << "# on: ON_DEMAND_CACHING with no hits               #" << endl;
	cout << "# sn: SINGLE_ROW_CACHING with no hits              #" << endl;
	cout << "# nn: NO_CACHING with no hits                      #" << endl;
	cout << "# ah: all policies with hits                       #" << endl;
	cout << "# an: all policies no hits                         #" << endl;
	cout << "# q:  quit                                         #" << endl;
	cout << "####################################################" << endl;
	cout << "select option - confirm with return: ";

	string s;
	cin >> s;

	return s;
}
//--------------------------------------------------------------------------------------------------------//
// check connection
//--------------------------------------------------------------------------------------------------------//
basar::db::aspect::ConnectionRef checkBBConnection()
{
	const std::string fun = "checkBBConnection()";
	// BB 27.10.05
	using basar::db::aspect::ConnectionRef;
	using basar::ConnectionTypeEnum;
	using basar::db::aspect::Manager;
	//using basar::db::aspect::ManagerBase;
	//using basar::db::aspect::ManagerBase::ConnectionList_Iterator;

	ConnectionRef				connRef1;
	ConnectionRef::DatabaseInfo	dbInfo1;
	

	dbInfo1.dbServer	 = "onl_sofumi1_tcp";
	dbInfo1.database	 = "pps1cg";
	dbInfo1.user		 = "devuser";
	dbInfo1.passwd		 = "phoenix";
	
	/*! \todo	the client is obliged to cast the ConnectionTypeEnum-value of the 
				class ConnectionRef to the ConnectionTypeEnum of the namespace 'basar::db::sql'. */
	dbInfo1.eConnectType = basar::INFORMIX;	
	
	report( fun, "erste Connection-Instanz oeffnen:" ); 
	//cout << fun << " erste Connection-Instanz oeffnen: " << endl;
	connRef1 = Manager::getInstance().createConnect( dbInfo1 );

	report( fun, "zweite Connection-Instanz oeffnen:" ); 
	//cout << fun << " zweite Connection-Instanz oeffnen: " << endl;
	ConnectionRef connRef2;
	connRef2 = Manager::getInstance().createConnect( dbInfo1 );

	//cout << fun << " - Manager::getInstance().sizeConn(): "		<< Manager::getInstance().sizeConn()	 << endl;
//	COUT( static_cast<unsigned>( Manager::getInstance().sizeConn() ) );
	//cout << fun << " - Manager::getInstance().sizeOpenConn(): " << Manager::getInstance().sizeOpenConn() << endl;
//	COUT( static_cast<unsigned>( Manager::getInstance().sizeOpenConn() ) );

	report( fun, "erste Connection schliessen:" ); 
	//cout << fun << " erste Connection schliessen: " << endl;
	connRef2.close();

//	COUT( static_cast<unsigned>( Manager::getInstance().sizeConn() ) );
//	COUT( static_cast<unsigned>( Manager::getInstance().sizeOpenConn() ) );

	basar::db::aspect::ManagerBase::ConnectionList_Iterator it = Manager::getInstance().beginConn();
	VarString connId;
	const ConnectionRef::DatabaseInfo& dbInfo = Manager::getInstance().getConnInfo( it, connId );
	cout << fun << ": connId:>" << connId << "<" << endl;
	
	cout << fun << ": dbInfo.eConnectType:>"	<<  dbInfo.eConnectType		<< "<" << endl;
	cout << fun << ": dbInfo.dbServer:>"		<<  dbInfo.dbServer			<< "<" << endl;
	cout << fun << ": dbInfo.database:>"		<<  dbInfo.database			<< "<" << endl;
	cout << fun << ": dbInfo.user:>"			<<  dbInfo.user				<< "<" << endl;
	cout << fun << ": dbInfo.passwd:>"			<<  dbInfo.passwd			<< "<" << endl;

	//ConnectionList_Iterator it = Manager::getInstance().beginConn();
	int  i = 0;
	for( ; it != Manager::getInstance().endConn(); ++it ){
		cout << ": Durchlaeufe!" << ++i <<  endl;
		cout << std::boolalpha << Manager::getInstance().isOpen( it ) << endl;
	}

	return Manager::getInstance().createConnect( dbInfo1 );
}
//--------------------------------------------------------------------------------------------------------//
// check snapshots
//--------------------------------------------------------------------------------------------------------//
////////////////////////////////////////////////////////////////////////////////////////////////////////////
// void checkCachingPolicies( AccessorInstanceRef ai1, const char* what )
////////////////////////////////////////////////////////////////////////////////////////////////////////////
void checkCachingPolicies( AccessorInstanceRef air, CachingPolicyEnum eCachingPolicy, const char* what )
{
	what;
	switch( eCachingPolicy ){
		case FULL_CACHING:			check_FULL_CACHING_Policy( air );
									break;
		case ON_DEMAND_CACHING:		check_ON_DEMAND_CACHING_Policy( air );
									break;
		case SINGLE_ROW_CACHING:	check_SINGLE_ROW_CACHING_Policy( air );
									break;
		case NO_CACHING:			check_NO_CACHING_Policy( air );
									break;
		default:
			break;
	}
}
//--------------------------------------------------------------------------------------------------------//
// caching policies
//--------------------------------------------------------------------------------------------------------//
////////////////////////////////////////////////////////////////////////////////////////////////////////////
// void check_FULL_CACHING_Policy( AccessorInstanceRef air )
////////////////////////////////////////////////////////////////////////////////////////////////////////////
void check_FULL_CACHING_Policy( AccessorInstanceRef air )
{
	try {
		AccessorPropertyTable_YIterator yIt;
		yIt = air.getPropertyTable().begin(); // to fill up the propertytable only
		
		for( ; yIt != air.getPropertyTable().end(); ++yIt ){
			//--yIt;
			cout << "---------------------------------------------------" << endl;
			cout << "aller erste Treffer: " << endl;
			cout << "---------------------------------------------------" << endl;
			cout << "artikel_nr:   " << yIt.getInt32 (	"artikel_nr"	) << endl;		
			cout << "artikel_name: " << yIt.getString(	"artikel_name"	) << endl;
			cout << "filialnr:     " << yIt.getInt16 (	"filialnr"		) << endl;
			cout << "bestand:      " << yIt.getInt32 (	"bestand"		) << endl;
		}

	} catch( const basar::Exception& e )
	{
		cout << e.what() << endl;
	}
	//showElements( air, "first  run (check_FULL_CACHING_Policy): " );
	//showElements( air, "second run (check_FULL_CACHING_Policy): " );
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////
// void check_ON_DEMAND_CACHING_Policy( AccessorInstanceRef air )
////////////////////////////////////////////////////////////////////////////////////////////////////////////
void check_ON_DEMAND_CACHING_Policy( AccessorInstanceRef air )
{
	showElements( air, "first  run (check_ON_DEMAND_CACHING_Policy): " );
	showElements( air, "second run (check_ON_DEMAND_CACHING_Policy): " );
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////
// void check_SINGLE_ROW_CACHING_Policy( AccessorInstanceRef air )
////////////////////////////////////////////////////////////////////////////////////////////////////////////
void check_SINGLE_ROW_CACHING_Policy( AccessorInstanceRef air )
{
	//showElements( air, "first  run (check_SINGLE_ROW_CACHING_Policy): " );	
	//return;

	//cout << "---------------------------------------------------------------------" << endl;
	//cout << "show elements after " /*<<what*/ << endl;
	//cout << "---------------------------------------------------------------------" << endl;

	AccessorPropertyTable_YIterator yIt;
	yIt = air.getPropertyTable().begin(); // to fill up the propertytable only
	yIt = air.getPropertyTable().end();
	--yIt;
	cout << "---------------------------------------------------" << endl;
	cout << "aller erste Treffer: " << endl;
	cout << "---------------------------------------------------" << endl;
	cout << "artikel_nr:   " << yIt.getInt32 (	"artikel_nr"	) << endl;		
	cout << "artikel_name: " << yIt.getString(	"artikel_name"	) << endl;
	cout << "filialnr:     " << yIt.getInt16 (	"filialnr"		) << endl;
	cout << "bestand:      " << yIt.getInt32 (	"bestand"		) << endl;


	return;


	//AccessorPropertyTable_YIterator yIt2;

	//yIt2 = air.getPropertyTable().begin();
	//cout << "---------------------------------------------------" << endl;
	//cout << "aller erste Treffer: " << endl;
	//cout << "---------------------------------------------------" << endl;
	//cout << "artikel_nr:   " << yIt2.getInt32 (	"artikel_nr"	) << endl;		
	//cout << "artikel_name: " << yIt2.getString(	"artikel_name"	) << endl;
	//cout << "filialnr:     " << yIt2.getInt16 (	"filialnr"		) << endl;
	//cout << "bestand:      " << yIt2.getInt32 (	"bestand"		) << endl;

	//int i = 0;
	//AccessorPropertyTable_YIterator yIt;
	//for( yIt = yIt2; yIt != air.getPropertyTable().end(); yIt++ ){
	//	cout << "----------------------------------------------------------------" << endl;
	//	cout << "artikel:      " << ++i << endl;
	//	cout << "----------------------------------------------------------------" << endl;
	//	cout << "artikel_nr:   " << yIt.getInt32 (	"artikel_nr"	) << endl;		
	//	cout << "artikel_name: " << yIt.getString(	"artikel_name"	) << endl;
	//	cout << "filialnr:     " << yIt.getInt16 (	"filialnr"		) << endl;
	//	cout << "bestand:      " << yIt.getInt32 (	"bestand"		) << endl;
	//}

	//cout << "---------------------------------------------------" << endl;
	//cout << "aller erste Treffer: " << endl;
	//cout << "---------------------------------------------------" << endl;
	//cout << "artikel_nr:   " << yIt2.getInt32 (	"artikel_nr"	) << endl;		

	//cout << "artikel_name: " << yIt2.getString(	"artikel_name"	) << endl;

	//yIt2.setString(	"artikel_name", "Artikel_name geändert"	);
	//cout << "artikel_name: " << yIt2.getString(	"artikel_name"	) << endl;

	//cout << "filialnr:     " << yIt2.getInt16 (	"filialnr"		) << endl;
	//cout << "bestand:      " << yIt2.getInt32 (	"bestand"		) << endl;
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////
// void check_ON_DEMAND_CACHING_Policy( AccessorInstanceRef air )
////////////////////////////////////////////////////////////////////////////////////////////////////////////
void check_NO_CACHING_Policy( AccessorInstanceRef air )
{
	showElements( air, "first  run (check_NO_CACHING_Policy): " );
	//showElements( air, "second run (check_NO_CACHING_Policy): " );

	AccessorPropertyTable_YIterator yIt;
	yIt = air.getPropertyTable().begin(); // to fill up the propertytable only
	AccessorPropertyTable_XIterator xIt = yIt.begin();
	xIt.setInt32( 366 );

}
//--------------------------------------------------------------------------------------------------------//
// check snapshots
//--------------------------------------------------------------------------------------------------------//
////////////////////////////////////////////////////////////////////////////////////////////////////////////
// void checkBBSnapshots( AccessorInstanceRef air )
////////////////////////////////////////////////////////////////////////////////////////////////////////////
void checkBBSnapshots( AccessorInstanceRef air )
{
	// create a snapshot
	basar::property::PropertyTableSnapshotRef snapPtr = air.createSnapshot();
	// clean up (clear) the PropertyTable that hits of new query won't be pushed back to the old one.
	air.getPropertyTable().clear(); 
	air.execute("ReadArticle");
	air.resetToSnapshot( snapPtr );
}
//--------------------------------------------------------------------------------------------------------//
// check transaction
//--------------------------------------------------------------------------------------------------------//
////////////////////////////////////////////////////////////////////////////////////////////////////////////
// void checkBBTransaction( AccessorInstanceRef air )
////////////////////////////////////////////////////////////////////////////////////////////////////////////
void checkBBTransaction( AccessorInstanceRef air )
{
	//ai1.transactionCommit();		
	//ai1.resetToSnapshot( snapPtr );

	// show elements after snapshot
	//cout << "show elements after snapshot: " << endl;
	//showElements( ai1 );

	//ai1.transactionCommit();		
	//ai1.transactionRollback();

	// create Snapshot 
	//snapPtr = ai1.getPropertyTable().createSnapshot();
	//ai1.resetToSnapshot( snapPtr );

	// show elements after snapshot
	//showElements( ai1, "snapshot: " );
}
//--------------------------------------------------------------------------------------------------------//
// help function
//--------------------------------------------------------------------------------------------------------//
////////////////////////////////////////////////////////////////////////////////////////////////////////////
// void showElements( AccessorInstanceRef ai1 )
////////////////////////////////////////////////////////////////////////////////////////////////////////////
void showElements( AccessorInstanceRef air, const char* what )
{
	if( PRE_INCREMENT == g_objParameter.eIncrement_or_Decrement ){
		int i = 0;
		AccessorPropertyTable_YIterator yIt;
		for( yIt = air.getPropertyTable().begin(); yIt != air.getPropertyTable().end(); ++yIt ){
			cout << "----------------------------------------------------------------" << endl;
			cout << "artikel:      " << ++i << " " << what << endl;
			cout << "----------------------------------------------------------------" << endl;
			cout << "artikel_nr:   " << yIt.getInt32 (	"artikel_nr"	) << endl;		
			cout << "artikel_name: " << yIt.getString(	"artikel_name"	) << endl;
			cout << "artikel_name: " << yIt.getString(	"artikel_name"	) << endl;
			cout << "filialnr:     " << yIt.getInt16 (	"filialnr"		) << endl;
			cout << "bestand:      " << yIt.getInt32 (	"bestand"		) << endl;
		}
	} else if( POST_INCREMENT == g_objParameter.eIncrement_or_Decrement ){
		int i = 0;
		AccessorPropertyTable_YIterator yIt;
		for( yIt = air.getPropertyTable().begin(); yIt != air.getPropertyTable().end(); yIt++ ){
			cout << "----------------------------------------------------------------" << endl;
			cout << "artikel:      " << ++i << " " << what << endl;
			cout << "----------------------------------------------------------------" << endl;
			cout << "artikel_nr:   " << yIt.getInt32 (	"artikel_nr"	) << endl;		
			cout << "artikel_name: " << yIt.getString(	"artikel_name"	) << endl;
			cout << "artikel_name: " << yIt.getString(	"artikel_name"	) << endl;
			cout << "filialnr:     " << yIt.getInt16 (	"filialnr"		) << endl;
			cout << "bestand:      " << yIt.getInt32 (	"bestand"		) << endl;
		}
	} else if( PRE_DECREMENT == g_objParameter.eIncrement_or_Decrement ){
		int i = 0;
		AccessorPropertyTable_YIterator yIt;
		yIt = air.getPropertyTable().begin(); // to fill up the propertytable only
		yIt = air.getPropertyTable().end();
		--yIt;
		bool toBreak = false;
		while( true ){
			if( yIt == air.getPropertyTable().begin() ){
				cout << "----------------------------------------------------------------" << endl;
				cout << "artikel:      " << ++i << " " << what << endl;
				cout << "----------------------------------------------------------------" << endl;
				cout << "artikel_nr:   " << yIt.getInt32 (	"artikel_nr"	) << endl;		
				cout << "artikel_name: " << yIt.getString(	"artikel_name"	) << endl;
				cout << "artikel_name: " << yIt.getString(	"artikel_name"	) << endl;
				cout << "filialnr:     " << yIt.getInt16 (	"filialnr"		) << endl;
				cout << "bestand:      " << yIt.getInt32 (	"bestand"		) << endl;
				toBreak = true;
			} 
			if( true == toBreak ){
				break;
			}
			cout << "----------------------------------------------------------------" << endl;
			cout << "artikel:      " << ++i << " " << what << endl;
			cout << "----------------------------------------------------------------" << endl;
			cout << "artikel_nr:   " << yIt.getInt32 (	"artikel_nr"	) << endl;		
			cout << "artikel_name: " << yIt.getString(	"artikel_name"	) << endl;
			cout << "artikel_name: " << yIt.getString(	"artikel_name"	) << endl;
			cout << "filialnr:     " << yIt.getInt16 (	"filialnr"		) << endl;
			cout << "bestand:      " << yIt.getInt32 (	"bestand"		) << endl;
			--yIt;
		}
	} else if( POST_DECREMENT == g_objParameter.eIncrement_or_Decrement ){
				int i = 0;
		AccessorPropertyTable_YIterator yIt;
		yIt = air.getPropertyTable().begin(); // to fill up the propertytable only
		yIt--;
		yIt = air.getPropertyTable().end();
		yIt--;
		bool toBreak = false;
		while( true ){
			if( yIt == air.getPropertyTable().begin() ){
				cout << "----------------------------------------------------------------" << endl;
				cout << "artikel:      " << ++i << " " << what << endl;
				cout << "----------------------------------------------------------------" << endl;
				cout << "artikel_nr:   " << yIt.getInt32 (	"artikel_nr"	) << endl;		
				cout << "artikel_name: " << yIt.getString(	"artikel_name"	) << endl;
				cout << "artikel_name: " << yIt.getString(	"artikel_name"	) << endl;
				cout << "filialnr:     " << yIt.getInt16 (	"filialnr"		) << endl;
				cout << "bestand:      " << yIt.getInt32 (	"bestand"		) << endl;
				toBreak = true;
			} 
			if( true == toBreak ){
				break;
			}
			cout << "----------------------------------------------------------------" << endl;
			cout << "artikel:      " << ++i << " " << what << endl;
			cout << "----------------------------------------------------------------" << endl;
			cout << "artikel_nr:   " << yIt.getInt32 (	"artikel_nr"	) << endl;		
			cout << "artikel_name: " << yIt.getString(	"artikel_name"	) << endl;
			cout << "artikel_name: " << yIt.getString(	"artikel_name"	) << endl;
			cout << "filialnr:     " << yIt.getInt16 (	"filialnr"		) << endl;
			cout << "bestand:      " << yIt.getInt32 (	"bestand"		) << endl;
			yIt--;
		}
	}
}