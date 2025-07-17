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

void					report( std::string fun, std::string testcase, 
							    std::string exceptInfo = "", bool ok = true, ostream& strm = std::cout); 
AccessorInstanceRef		checkBBAccessorInstanceRef( ConnectionRef connection, CachingPolicyEnum );
void					printYIterator( AccessorPropertyTable_YIterator yit, ostream& strm );
const ostringstream&	getTime();
const ostringstream&	getDate();

//--------------------------------------------------------------------------------------------------------//
// void check_praepost_indecrement( ConnectionRef connection, AccessorInstanceRef air )
//--------------------------------------------------------------------------------------------------------//
void check_praepost_indecrement( ConnectionRef connection, AccessorInstanceRef air )
{
	static const char* const fun = "check_praepost_indecrement( ConnectionRef, AccessorInstanceRef )";

	//ostream& strm = cout;
	ofstream log ( "aspect.check_praepost_indecrement.log" ); ostream& strm = log;
	strm << "====================================================================" << endl;
	strm << fun << " - Entry!" << endl;
	strm << "====================================================================" << endl << endl;

	strm << "Date: " << getDate().str().c_str() << "\tTime: " << getTime().str().c_str() << endl;

	ostringstream os;
	try {
		//------------------------------------------------------------------------------------//
		// prae or post increment or decrement
		//------------------------------------------------------------------------------------//
		for( int praePostInDecrementNo = 2; praePostInDecrementNo < 4; ++praePostInDecrementNo ){
			praePostInDecrementNo;
			strm << endl << "===========================================================" << endl;
			strm << "crement: ";
			if( 0 == praePostInDecrementNo ){
				strm << "praeIncrement (++yit): " << endl;
			} else if( 1 == praePostInDecrementNo ){
				strm << "postIncrement (yit++): " << endl;
			} else if( 2 == praePostInDecrementNo ){
				strm << "praeDecrement (--yit): " << endl;
			} else if( 3 == praePostInDecrementNo ){
				strm << "postDecrement (yit--): " << endl;
			}
			strm << "===========================================================" << endl;

			//------------------------------------------------------------------------------------//
			// hits policy
			//------------------------------------------------------------------------------------//
			for( int hitsPolicyNo = 1; hitsPolicyNo < 2; ++hitsPolicyNo ){
				//------------------------------------------------------------------------------------//
				// caching policy
				//------------------------------------------------------------------------------------//
				for( int cachingPolicyNo = 0; cachingPolicyNo < 4; ++cachingPolicyNo ){
					try {
						air.getPropertyTable().clear();
						//report( "", "air.getPropertyTable().clear();", "", true, strm );
					} catch( const basar::Exception& e ){
						//report( "", "air.getPropertyTable().clear();", e.what(), false, strm );
					}
					//------------------------------------------------------------------------------------//
					// caching policy
					//------------------------------------------------------------------------------------//
					strm << endl << "-----------------------------------------------------------" << endl;
					strm << "caching policy: ";
					switch( cachingPolicyNo ){
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
						if( 0 == hitsPolicyNo){
							strm << "Query with hits!" << endl;
							air.getPropertyTable().clear();
							basar::I18nString s("artikel_nr=1057;" );
							basar::db::aspect::AccessorPropertyTable_YIterator yit = basar::db::aspect::Manager::getInstance().createAccessorYIterator(s);
							air.execute("ReadArticle", yit);
						} else if( 1 == hitsPolicyNo ) {
							strm << "Query without hits!" << endl;
							air.getPropertyTable().clear();
							basar::I18nString s("artikel_nr=9999999;" );
							basar::db::aspect::AccessorPropertyTable_YIterator yit = basar::db::aspect::Manager::getInstance().createAccessorYIterator(s);
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
					strm << "-----------------------------------------------------------" << endl;
					//-------------------------------------------------------------------------//
					// which crement?
					//-------------------------------------------------------------------------//
					AccessorPropertyTable_YIterator yit;
					try {
						yit = air.getPropertyTable().begin();
					} catch( const basar::Exception& e ){
						report( "", "air.getPropertyTable().begin();", e.what(), false, strm );
					}
					
					/*if( 0 == praePostInDecrementNo || 1 == praePostInDecrementNo ){
						int i = 0;
						if( 0 == praePostInDecrementNo ){
							strm << "----------------------------------------------------------------"		<< endl;
							strm <<	"artikel " << "(++yit)" << endl;
							strm << "----------------------------------------------------------------"		<< endl;
							for(; yit != air.getPropertyTable().end(); ++yit ){
								strm << "----------------------------------------------------------------" << endl;
								strm << "artikel: " << ++i << endl;
								strm << "----------------------------------------------------------------" << endl;
								printYIterator( yit, strm );
							}
						} else {
							strm << "----------------------------------------------------------------"		<< endl;
							strm <<	"artikel " << "(yit++)" << endl;
							strm << "----------------------------------------------------------------"		<< endl;
							for(; yit != air.getPropertyTable().end(); yit++ )
							{
								strm << "----------------------------------------------------------------" << endl;
								strm << "artikel: " << ++i << endl;
								strm << "----------------------------------------------------------------" << endl;
								printYIterator( yit, strm );
							}
						}
					} else */
					if( 2 == praePostInDecrementNo || 3 == praePostInDecrementNo ){
						int i = 0;
						try {
							yit = air.getPropertyTable().end();
							report( "", "yit = air.getPropertyTable().end();", "", true, strm );
						} catch( const basar::Exception& e ){
			                report( "", "yit = air.getPropertyTable().end();", e.what(), false, strm );
						}

						if( 2 == praePostInDecrementNo ){
							strm << "----------------------------------------------------------------"		<< endl;
							strm <<	"artikel " << "(--yit)" << endl;
							strm << "----------------------------------------------------------------"		<< endl;	
							try {
								--yit;
								report( "", "--yit;", "", true, strm );
							} catch( const basar::Exception& e ){
			                    report( "", "--yit;", e.what(), false, strm );
							}
							bool toBreak = false;
							try {
								while( true ){
									if( yit == air.getPropertyTable().begin() ){
										strm << "----------------------------------------------------------------" << endl;
										strm << "artikel: " << ++i << endl;
										strm << "----------------------------------------------------------------" << endl;
										printYIterator( yit, strm );
										toBreak = true;
									} 
									if( true == toBreak ){
										break;
									}
									strm << "----------------------------------------------------------------" << endl;
									strm << "artikel: " << ++i << endl;
									strm << "----------------------------------------------------------------" << endl;
									printYIterator( yit, strm );
									--yit;
									/*try {
										--yit;
										report( "", "--yit;", "", true, strm );
									} catch( const basar::Exception& e ){
                         				report( "", "--yit;", e.what(), false, strm );
									}*/
									
								} // end while()
							} catch( const basar::Exception& e ){
								report( "", "--yit", e.what(), false, strm );
							}
					//	} else {
					//		strm << "----------------------------------------------------------------"		<< endl;
					//		strm <<	"artikel " << "(yit--)" << endl;
					//		strm << "----------------------------------------------------------------"		<< endl;
					//		try {
					//			yit--;
					//			report( "", "yit--;", "", true, strm );
					//		} catch( const basar::Exception& e ){
			  //                  report( "", "yit--;", e.what(), false, strm );
					//		}
					//		bool toBreak = false;
					//		try {
					//			while( true ){
					//				if( yit == air.getPropertyTable().begin() ){
					//					strm << "----------------------------------------------------------------" << endl;
					//					strm << "artikel: " << ++i << endl;
					//					strm << "----------------------------------------------------------------" << endl;
					//					printYIterator( yit, strm );
					//					toBreak = true;
					//				} 
					//				if( true == toBreak ){
					//					break;
					//				}
					//				strm << "----------------------------------------------------------------" << endl;
					//				strm << "artikel: " << ++i << endl;
					//				strm << "----------------------------------------------------------------" << endl;
					//				printYIterator( yit, strm );
					//				yit--;
					//				/*try {
					//					yit--;
					//					report( "", "yit--;", "", true, strm );
					//				} catch( const basar::Exception& e ){
     //                    				report( "", "yit--;", e.what(), false, strm );
					//				}*/
					//			} // end while() 
					//		} catch( const basar::Exception& e ){
					//			report( "", "yit--", e.what(), false, strm );
					//		}
						}
					} // if( 2 == praePostInDecrementNo || 3 == praePostInDecrementNo )
				} // END FOR CACHINGPOLICYNO
			} // END FOR HITSPOLICYNO
		} // END FOR PRAEPOSTINDECREMENTNO
	} catch( const basar::Exception& e ){
		report( "", "???", e.what(), false, strm );
	} catch( ... ){
		report( "", "Exception ...", "", false, strm );
	}
	strm << "====================================================================" << endl;
	strm << fun << " - Exit!" << endl;
	strm << "====================================================================" << endl;
}
//--------------------------------------------------------------------------------------------------------//
// printYIterator( AccessorPropertyTable_YIterator yit, ostream& strm )
//--------------------------------------------------------------------------------------------------------//
void printYIterator( AccessorPropertyTable_YIterator yit, ostream& strm )
{
	try {
		strm << "artikel_nr: "		<< yit.getInt32(  "artikel_nr" )		<< endl;
		strm << "artikel_name: "	<< yit.getString( "artikel_name" )	<< endl;
		strm << "filialnr: "		<< yit.getInt16(  "filialnr" )		<< endl;
		strm << "bestand: "			<< yit.getInt32(  "bestand" )		<< endl;
	} catch( const basar::Exception& e ){
		report( "", "yit.getInt32( \"bestand\" )", e.what(), false, strm );
	}
}