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

void report( std::string fun, std::string testcase, std::string exceptInfo = "", bool ok = true, ostream& strm = std::cout); 
AccessorInstanceRef checkBBAccessorInstanceRef( ConnectionRef connection, CachingPolicyEnum );
const ostringstream& getTime();
const ostringstream& getDate();

//--------------------------------------------------------------------------------------------------------//
// void check_Inserts( ConnectionRef connection, AccessorInstanceRef air )
//--------------------------------------------------------------------------------------------------------//
void check_Inserts( ConnectionRef connection, AccessorInstanceRef air )
{
	static const char* const fun = "check_Inserts( ConnectionRef, AccessorInstanceRef )";

	//ostream& strm = cout;
	ofstream log ( "aspect.check_inserts.log" ); ostream& strm = log;
	strm << "====================================================================" << endl;
	strm << fun << " - Entry!" << endl;
	strm << "====================================================================" << endl << endl;

	strm << "Date: " << getDate().str().c_str() << "\tTime: " << getTime().str().c_str() << endl;

	ostringstream os;

	try {
		for( int i = 0; i < 3; ++i ){
			for( int h = 0; h < 4; ++h ){
				try {
					air.getPropertyTable().clear();
				} catch( const basar::Exception& e ){
					report( "", "air.getPropertyTable().clear();", e.what(), false, strm );
				}
				//------------------------------------------------------------------------------------//
				// caching policy
				//------------------------------------------------------------------------------------//
				strm << endl << "-----------------------------------------------------------" << endl;
				strm << "caching policy: ";
				switch( h ){
					case 0:
							strm << "NO_CACHING: ";
							air = checkBBAccessorInstanceRef( connection, basar::db::aspect::NO_CACHING );			
							break;
					case 1:
							strm << "SINGLE_ROW_CACHING: ";
							air = checkBBAccessorInstanceRef( connection, basar::db::aspect::SINGLE_ROW_CACHING );			
							break;
					case 2:
							strm << "ON_DEMAND_CACHING: ";
							air = checkBBAccessorInstanceRef( connection, basar::db::aspect::ON_DEMAND_CACHING );			
							break;
					case 3:
							strm << "FULL_CACHING: ";
							air = checkBBAccessorInstanceRef( connection, basar::db::aspect::FULL_CACHING );			
							break;
					default:
						throw "What policy?";
				}
				try {
					if( 0 == i ){
						strm << "Query with hits!" << endl;
						air.getPropertyTable().clear();
						basar::I18nString s("artikel_nr=1057;" );
						AccessorPropertyTable_YIterator yit = basar::db::aspect::Manager::getInstance().createAccessorYIterator(s);
						air.execute("ReadArticle", yit);
					} else if( 1 == i ) {
						strm << "Query without hits!" << endl;
						air.getPropertyTable().clear();
						basar::I18nString s("artikel_nr=9999999;" );
						AccessorPropertyTable_YIterator yit = basar::db::aspect::Manager::getInstance().createAccessorYIterator(s);
						air.execute("ReadArticle", yit);
					} else {
						strm << "Without Query!" << endl;
						air.getPropertyTable().clear();
					}
				} catch( const basar::Exception& e ){
					report( "", "air.getPropertyTable().clear();", e.what(), false, strm );
				} catch( ... ){
					report( "", "Exception: ...", "", false, strm );
				}

				//-----------------------------------------------------------------------------------------------------------------------//
				for( int x = 0; x < 3; ++x ){
					strm	<< endl
							<< "------------------------------------------------------------------------"	<< endl;
					if( 0 == x ){
							strm << "1. inserts at the begin()!" << endl;
							strm	<< "------------------------------------------------------------------------"	<< endl;
					} else if( 1 == x ){
							strm << "2. inserts in the middle()!" << endl;
							strm	<< "------------------------------------------------------------------------"	<< endl;
							try {
								AccessorPropertyTable_YIterator yit = air.getPropertyTable().begin();
								report( "", "air.getPropertyTable().begin();", "", true, strm );
								++yit;
								report( "", "++yit;", "", true, strm );
							} catch( const basar::Exception& e ){
								report( "", "begin() + (++yit)", e.what(), false, strm );
							}
					} else {
							strm << "3. inserts at the end()!" << endl;
							strm	<< "------------------------------------------------------------------------"	<< endl;
							try {
								AccessorPropertyTable_YIterator yit = air.getPropertyTable().begin();
								report( "", "air.getPropertyTable().begin();", "", true, strm );
								for( ; yit != air.getPropertyTable().end(); ++yit );
								report( "", "for( ; yit != air.getPropertyTable().end(); ++yit );", "", true, strm );
							} catch( const basar::Exception& e ){
								report( "", "begin() + for( ; yit != air.getPropertyTable().end(); ++yit );", e.what(), false, strm );
							}
					}						
					
					//////////////////////////////////////////////////////////////////////////////////////////////////////////
					// insert( basar::FOR_CLEAN );
					//////////////////////////////////////////////////////////////////////////////////////////////////////////
					AccessorPropertyTable_YIterator yit;
					try {
						yit = air.getPropertyTable().insert( basar::FOR_CLEAN );
						report( "", "yit = air.getPropertyTable().insert( FOR_CLEAN );", "", true, strm );
					} catch( const basar::Exception& e ){
						report( "", "yit = air.getPropertyTable().insert( FOR_CLEAN );", e.what(), false, strm );
					}
					// state check
					try {
						report( "", "yit.getState().toString().c_str(): ", yit.getState().toString().c_str(), true, strm );
					} catch( const basar::Exception& e ){
						report( "", "yit.getState().toString().c_str()", e.what(), false, strm );
					}
					try {
						AccessorPropertyTable_YIterator yit2 = yit;
						AccessorPropertyTable_XIterator xit = yit2.begin();
						for(; xit != yit2.end(); ++yit2 ){
							report( "", xit.getState().toString().c_str(), "", true, strm );
						}
					} catch( const basar::Exception& e ){
						report( "", "for(; xit != yit.end(); ++yit )", e.what(), false, strm );
					}
					// set any value
					try {
						yit.setInt32( "bestand", 99 );
						report( "", "yit.setInt32( \"bestand\", 99 );", "", true, strm );
					} catch( const basar::Exception& e ){
						report( "", "yit.setInt32( \"bestand\", 99 );", e.what(), false, strm );
					}
					try {
						report( "", "yit.getState().toString().c_str(): ", yit.getState().toString().c_str(), true, strm );
					} catch( const basar::Exception& e ){
						report( "", "yit.getState().toString().c_str()", e.what(), false, strm );
					}
					try {
						AccessorPropertyTable_YIterator yit2 = yit;
						AccessorPropertyTable_XIterator xit = yit2.begin();
						for(; xit != yit2.end(); ++yit2 ){
							report( "", xit.getState().toString().c_str(), "", true, strm );
						}
					} catch( const basar::Exception& e ){
						report( "", "for(; xit != yit.end(); ++yit )", e.what(), false, strm );
					}

					//////////////////////////////////////////////////////////////////////////////////////////////////////////
					// insert( basar::FOR_INSERT );
					//////////////////////////////////////////////////////////////////////////////////////////////////////////
					try {
						air.getPropertyTable().insert( basar::FOR_INSERT );
						report( "", "air.getPropertyTable().insert( FOR_INSERT );", "", true, strm );
					} catch( const basar::Exception& e ){
						report( "", "air.getPropertyTable().insert( FOR_INSERT );", e.what(), false, strm );
					}

					//////////////////////////////////////////////////////////////////////////////////////////////////////////
					// insert( basar::FOR_UPDATE );
					//////////////////////////////////////////////////////////////////////////////////////////////////////////
					try {
						air.getPropertyTable().insert( basar::FOR_UPDATE );
						report( "", "air.getPropertyTable().insert( FOR_UPDATE );", "", true, strm );
					} catch( const basar::Exception& e ){
						report( "", "air.getPropertyTable().insert( FOR_UPDATE );", e.what(), false, strm );
					}

					//////////////////////////////////////////////////////////////////////////////////////////////////////////
					// insert( basar::FOR_DELETE );
					//////////////////////////////////////////////////////////////////////////////////////////////////////////
					try {
						air.getPropertyTable().insert( basar::FOR_DELETE );
						report( "", "air.getPropertyTable().insert( FOR_DELETE );", "", true, strm );
					} catch( const basar::Exception& e ){
						report( "", "air.getPropertyTable().insert( FOR_DELETE );", e.what(), false, strm );
					}

				} // end for (all inserts)
				
			} // end for (caching policies)
		} // end for (hits)

	} catch( const basar::Exception& e ){
		report( "", "", e.what(), false, strm );
	} catch( ... ){
		report( fun, "unexpected exception!;", "", false, strm );
	}
}