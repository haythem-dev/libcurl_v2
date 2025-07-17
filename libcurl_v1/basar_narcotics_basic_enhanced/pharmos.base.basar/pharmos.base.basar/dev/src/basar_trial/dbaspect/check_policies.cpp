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
AccessorInstanceRef createAccessor4checkPolicies( ConnectionRef connection, CachingPolicyEnum eCachingPolicy );
void checkSizeAndEmpty( AccessorInstanceRef air, ostream& strm = std::cout );
void checkFilterAndState( AccessorPropertyTable_YIterator yit, ostream& strm = std::cout );
void check_NO_CACHING( AccessorInstanceRef air, ostream& strm = std::cout );

//--------------------------------------------------------------------------------------------------------//
// void check_policies( ConnectionRef connection, AccessorInstanceRef air )
//--------------------------------------------------------------------------------------------------------//
void check_policies( ConnectionRef connection, AccessorInstanceRef air )
{
	static const char* const fun = "check_policies(ConnectionRef, AccessorInstanceRef)";

	//ostream& strm = cout;
	//ofstream log( "aspect.check_policies.SINGLE_ROW_CACHING.old" ); 
	ofstream log( "aspect.check_policies.log" ); 
	ostream& strm = log;

	strm << "====================================================================" << endl;
	strm << fun << " - Entry!" << endl;
	strm << "====================================================================" << endl << endl;

	strm << "Date: " << getDate().str().c_str() << "\tTime: " << getTime().str().c_str() << endl;
	
	string cachingPolicy; 

	ostringstream os;
	for( int h = 0; h < 1; ++h ){
		for( int i = 0; i < 3; ++i ){
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
				//case 0:
						//cachingPolicy = "NO_CACHING";
						//strm << "NO_CACHING: ";
						//air = createAccessor4checkPolicies( connection, basar::db::aspect::NO_CACHING );			
						//break;
				case 0:
						cachingPolicy = "FULL_CACHING";
						strm << "FULL_CACHING: ";
						air = createAccessor4checkPolicies( connection, basar::db::aspect::FULL_CACHING );			
						break;
				/*case 0:
						cachingPolicy = "ON_DEMAND_CACHING";
						strm << "ON_DEMAND_CACHING: ";
						air = createAccessor4checkPolicies( connection, basar::db::aspect::ON_DEMAND_CACHING );			
						break;*/
				/*case 0:
						cachingPolicy = "SINGLE_ROW_CACHING";
						strm << "SINGLE_ROW_CACHING: ";
						air = createAccessor4checkPolicies( connection, basar::db::aspect::SINGLE_ROW_CACHING );			
						break;*/
				default:
					strm << "throw \"What policy?\": ";
					throw "What policy?";
			}
			try {
				if( 0 == i ){
					strm << "Query with hits!" << endl;
					air.getPropertyTable().clear();
					//air.setParameterList( "artikel_name=%ASPIRIN%;" );
					//air.setParameterList( "artikel_name=ASPIRIN C BRAUSE EMRA MED   N3;" );
					//air.execute("ReadArticleViaName");

					AccessorPropertyTable_YIterator yit;

					basar::I18nString s("artikel_nr=112;filialnr=9;" );
					yit = basar::db::aspect::Manager::getInstance().createAccessorYIterator(s);
					air.execute("ReadArticle", yit);

					yit = air.getPropertyTable().begin();
					for( ; yit != air.getPropertyTable().end(); ++yit ){
						yit.toStream( strm );
					}
					
				} else if( 1 == i ) {
					strm << "Query without hits!" << endl;
					air.getPropertyTable().clear();
					basar::I18nString s("artikel_name=XXX;" );
					AccessorPropertyTable_YIterator yit = basar::db::aspect::Manager::getInstance().createAccessorYIterator(s);
					air.execute("ReadArticleViaName", yit);
				} else {
					strm << "Without Query!" << endl;
					air.getPropertyTable().clear();
				}
			} catch( const basar::Exception& e ){
				report( "", "air.getPropertyTable().clear();", e.what(), false, strm );
			} catch( ... ){
				report( "", "Exception: ...", "", false, strm );
			}

			if( cachingPolicy == "NO_CACHING" ){
				check_NO_CACHING( air, strm );
				continue;
			} else {
				strm << "-----------------------------------------------------------" << endl;
				
				AccessorPropertyTable_YIterator yit, yitEnd;
				checkSizeAndEmpty( air, strm );

				//////////////////////////////////////////////////////////////////
				// begin(), isBegin()
				//////////////////////////////////////////////////////////////////
				try {
					strm << "1. yit = air.getPropertyTable().begin(): ";
					yit = air.getPropertyTable().begin();
					yit.toStream( strm );
					strm << "OK;" << endl;
				} catch( const basar::Exception& e ){
					report( "", "yit = air.getPropertyTable().begin()", e.what(), false, strm );
				}
				try {
					strm << "2. yit = air.getPropertyTable().end(): ";
					yitEnd = air.getPropertyTable().end();	
					strm << "OK;" << endl;
				} catch( const basar::Exception& e ){
					report( "", "yit = air.getPropertyTable().end()", e.what(), false, strm );
				}

				//--------------------------------------------------------------------------------------------//
				//
				//--------------------------------------------------------------------------------------------//
				try {
					yit = air.getPropertyTable().begin();
					yit.toStream( strm );
					strm << "2a. yit.getInt32( \"bestand\"): " << yit.getInt32( "bestand") << endl;
					strm << "\t2a: OK;" << endl;
				} catch( const basar::Exception& e ){
					report( "", "2a. yit.getInt32( \"bestand\")", e.what(), false, strm );
				}
				try {
					strm << "2b. yit.setInt32( \"bestand\", 999 ): ";
					yit.setInt32( "bestand", 999 );
					strm << "\t2b: OK;" << endl;
				} catch( const basar::Exception& e ){
					report( "", "2b. yit.setInt32( \"bestand\", 999 )", e.what(), false, strm );
				} catch( ... ){
					report( "", "2b. yit.setInt32( \"bestand\", 999 ): --> UNSPECIFIED EXCEPTION!", "", false, strm );
				}
				
				checkFilterAndState( yit, strm );

				try {
					strm << "3. yit.toStream( strm ): " << endl;
					yit.toStream( strm );
				} catch( const basar::Exception& e ){
					report( "", "yit.toStream( strm )", e.what(), false, strm );
				} catch( ... ){
					report( "", "...-EXCEPTION: yit.toStream( strm )", 
							"", false, strm );
				}
				try {
					strm << "4. yit.getRowNumber(): " << yit.getRowNumber() << endl;
				} catch( const basar::Exception& e ){
					report( "", "yit.getRowNumber()", e.what(), false, strm );
				}
				
				strm.flush();

				try {
					air.beginTransaction();
					strm << "2c. air.execute( \"UpdateArticleStock\", yit ): ";
					air.execute( "UpdateArticleStock", yit );
					strm << "\t2c: OK;" << endl;
					air.commitTransaction();
				} catch( const basar::Exception& e ){
					report( "", "2c. air.execute( \"UpdateArticleStock\", yit )", e.what(), false, strm );
				}
				//--------------------------------------------------------------------------------------------//



				while( yit != yitEnd )
				{
					checkFilterAndState( yit, strm );
					try {
						strm << "3. yit.toStream( strm ): " << endl;
						yit.toStream( strm );
					} catch( const basar::Exception& e ){
						report( "", "yit.toStream( strm )", e.what(), false, strm );
					} catch( ... ){
						report( "", "...-EXCEPTION: yit.toStream( strm )", 
								"", false, strm );
					}
					try {
						strm << "4. yit.getRowNumber(): " << yit.getRowNumber() << endl;
					} catch( const basar::Exception& e ){
						report( "", "yit.getRowNumber()", e.what(), false, strm );
					}
					
					try {
						strm << "5. ++yit: " << endl;
						++yit;
						strm << "OK;"<< endl;
					} catch( const basar::Exception& e ){
						report( "", "++yit", e.what(), false, strm );
					}
				} // end while
				//-----------------------------------------------------------------------------------------------------//

				try {
					os.str("");
					os << boolalpha << "6. yit.isBegin(): " << yit.isBegin();	
					strm << "OK;" << endl;
				} catch( const basar::Exception& e ){
					report( "", "yit.isBegin()", e.what(), false, strm );
				}
			
				//////////////////////////////////////////////////////////////////
				// setString(), begin(), begin(filter)
				//////////////////////////////////////////////////////////////////
				try {
					strm << "7. yit is shifted 1 times with postincrement: " ;
					//yit++; yit++; yit++; 
					yit++;
					strm << "OK;" << endl;
				} catch( const basar::Exception& e ){
					report( "", "yit++; yit++; yit++;", e.what(), false, strm );
				}
				try {
					strm << "8. yit.toStream( strm ): " << endl;
					yit.toStream( strm );
				} catch( const basar::Exception& e ){
					report( "", "yit.toStream( strm )", e.what(), false, strm );
				} catch( ... ){
					report( "", "...-EXCEPTION: yit.toStream( strm )", 
							"", false, strm );
				}

				checkFilterAndState( yit, strm );

				try {
					strm << "9. yit.getRowNumber(): " << yit.getRowNumber() << endl;
				} catch( const basar::Exception& e ){
					report( "", "yit.getRowNumber()", e.what(), false, strm );
				}


				try {
					strm << "10. yit.setString( \"artikel_name\", \"keine Aspirin mehr fuer 4\" ): ";
					yit.setString( "artikel_name", "keine Aspirin mehr fuer 4" ); 
					strm << "OK" << endl;
				} catch( const basar::Exception& e ){
					report( "", "yit.setString( \"artikel_name\", \"keine Aspirin mehr fuer 4\" )", e.what(), false, strm );
				} catch( ... ){
					report( "", "...-EXCEPTION: yit.setString( \"artikel_name\", \"keine Aspirin mehr fuer 4\" )", 
							"", false, strm );
				}

				try {
					strm << "11. yit = air.getPropertyTable().begin(): ";
					yit = air.getPropertyTable().begin();	
					strm << "OK;" << endl;
				} catch( const basar::Exception& e ){
					report( "", "yit = air.getPropertyTable().begin()", e.what(), false, strm );
				} catch( ... ){
					report( "", "...-EXCEPTION: yit = air.getPropertyTable().begin()", 
							"", false, strm );
				}
				
				checkFilterAndState( yit, strm );

				try {
					strm << "12. yit.toStream( strm ): " << endl;
					yit.toStream( strm );
				} catch( const basar::Exception& e ){
					report( "", "yit.toStream( strm )", e.what(), false, strm );
				} catch( ... ){
					report( "", "...-EXCEPTION: yit.toStream( strm )", 
							"", false, strm );
				}
				
				try {
					strm << "13. yit.getRowNumber(): " << yit.getRowNumber() << endl;
				} catch( const basar::Exception& e ){
					report( "", "yit.getRowNumber()", e.what(), false, strm );
				} catch( ... ){
					report( "", "...-EXCEPTION: yit.getRowNumber()", 
							"", false, strm );
				}

				try {
					strm << "14. yit = air.getPropertyTable().begin(SS_UPDATE): ";
					yit = air.getPropertyTable().begin( basar::SS_UPDATE );	
					strm << "OK" << endl;
				} catch( const basar::Exception& e ){
					report( "", "yit = air.getPropertyTable().begin(SS_UPDATE)", e.what(), false, strm );
				} catch( ... ){
					report( "", "...-EXCEPTION: yit = air.getPropertyTable().begin(SS_UPDATE)", 
							"", false, strm );
				}
				
				checkFilterAndState( yit, strm );

				try {
					strm << "15. yit.toStream( strm ): " << endl;
					yit.toStream( strm );
				} catch( const basar::Exception& e ){
					report( "", "yit.toStream( strm )", e.what(), false, strm );
				} catch( ... ){
					report( "", "...-EXCEPTION: yit.toStream( strm )", 
							"", false, strm );
				}
				try {
					strm << "16. yit.getRowNumber(): " << yit.getRowNumber() << endl;
				} catch( const basar::Exception& e ){
					report( "", "yit.getRowNumber()", e.what(), false, strm );
				} catch( ... ){
					report( "", "...-EXCEPTION: yit.getRowNumber()", 
							"", false, strm );
				}
				
				//////////////////////////////////////////////////////////////////
				// setString(), begin(), begin(filter)
				//////////////////////////////////////////////////////////////////
				try {
					strm << "17. yit = air.getPropertyTable().begin(SS_CLEAN): ";
					yit = air.getPropertyTable().begin(basar::SS_CLEAN);	
					strm << "OK" << endl;
				} catch( const basar::Exception& e ){
					report( "", "yit = air.getPropertyTable().begin(SS_CLEAN)", e.what(), false, strm );
				} catch( ... ){
					report( "", "...-EXCEPTION: yit = air.getPropertyTable().begin(SS_CLEAN)", 
							"", false, strm );
				}
					
				checkFilterAndState( yit, strm );
				
				try {
					strm << "18. yit.getRowNumber(): " << yit.getRowNumber() << endl;
				} catch( const basar::Exception& e ){
					report( "", "yit.getRowNumber()", e.what(), false, strm );
				} catch( ... ){
					report( "", "...-EXCEPTION: yit.getRowNumber()", 
							"", false, strm );
				}

				try {
					strm << "19. yit is shifted 2 times with postincrement: ";
					yit++; yit++;
					strm << "OK" << endl;
				} catch( const basar::Exception& e ){
					report( "", "yit++; yit++;", e.what(), false, strm );
				} catch( ... ){
					report( "", "...-EXCEPTION: yit++; yit++;", 
							"", false, strm );
				}
					
				checkFilterAndState( yit, strm );

				try {
					strm << "20. yit.getRowNumber(): " << yit.getRowNumber() << endl;
				} catch( const basar::Exception& e ){
					report( "", "yit.getRowNumber()", e.what(), false, strm );
				} catch( ... ){
					report( "", "...-EXCEPTION: yit.getRowNumber()", 
							"", false, strm );
				}

				try {
					strm << "21. yit.setString( \"artikel_name\", \"keine Aspirin mehr fuer 3\" ): "  << endl;
					yit.setString( "artikel_name", "keine Aspirin mehr fuer 3" );	
				} catch( const basar::Exception& e ){
					report( "", "setString(keine Aspirin mehr fuer 3)", e.what(), false, strm );
				} catch( ... ){
					report( "", "...-EXCEPTION: setString(keine Aspirin mehr fuer 3);", 
							"", false, strm );
				}

				try {
					strm << "22. yit = air.getPropertyTable().begin():" << endl;
					yit = air.getPropertyTable().begin();	
				} catch( const basar::Exception& e ){
					report( "", "yit = air.getPropertyTable().begin()", e.what(), false, strm );
				} catch( ... ){
					report( "", "...-EXCEPTION: yit = air.getPropertyTable().begin()", 
							"", false, strm );
				}

				checkFilterAndState( yit, strm );
				
				try {
					strm << "23. yit.toStream( strm ): " << endl;
					yit.toStream( strm );
				} catch( const basar::Exception& e ){
					report( "", "yit.toStream( strm )", e.what(), false, strm );
				} catch( ... ){
					report( "", "...-EXCEPTION: yit.toStream( strm )", 
							"", false, strm );
				}
				
				try {
					strm << "24. yit.getRowNumber(): " << yit.getRowNumber() << endl;
				} catch( const basar::Exception& e ){
					report( "", "yit.getRowNumber()", e.what(), false, strm );
				} catch( ... ){
					report( "", "...-EXCEPTION: yit.getRowNumber()", 
							"", false, strm );
				}

				try {
					strm << "25. yit = air.getPropertyTable().begin(SS_UPDATE):" << endl;
					yit = air.getPropertyTable().begin( basar::SS_UPDATE );	
				} catch( const basar::Exception& e ){
					report( "", "yit = air.getPropertyTable().begin(SS_UPDATE)", e.what(), false, strm );
				} catch( ... ){
					report( "", "...-EXCEPTION: yit = air.getPropertyTable().begin(SS_UPDATE)", 
							"", false, strm );
				}
					
				checkFilterAndState( yit, strm );	

				try {
					strm << "26. yit.toStream( strm ): " << endl;
					yit.toStream( strm );
				} catch( const basar::Exception& e ){
					report( "", "yit.toStream( strm );", e.what(), false, strm );
				} catch( ... ){
					report( "", "...-EXCEPTION: yit.toStream( strm )", 
							"", false, strm );
				}
				
				try {
					strm << "27. yit.getRowNumber(): " << yit.getRowNumber() << endl;
				} catch( const basar::Exception& e ){
					report( "", "yit.getRowNumber();", e.what(), false, strm );
				} catch( ... ){
					report( "", "...-EXCEPTION: yit.getRowNumber()", 
							"", false, strm );
				}

				//////////////////////////////////////////////////////////////////
				// checkFilterAndState( air, strm );
				//////////////////////////////////////////////////////////////////
				checkFilterAndState( yit, strm );
		
				//////////////////////////////////////////////////////////////////
				// get...()begin()
				//////////////////////////////////////////////////////////////////
				try {
					//"int32 artikel_nr; string artikel_name; int16 filialnr; int32 bestand;"
					strm << "28. yit.getInt32( \"artikel_nr\" );:" << yit.getInt32( "artikel_nr" ) << endl;
				} catch( const basar::Exception& e ){
					report( "", "get32()", e.what(), false, strm );
				} catch( ... ){
					report( "", "...-EXCEPTION: get32();", 
							"", false, strm );
				}
				try {
					//"int32 artikel_nr; string artikel_name; int16 filialnr; int32 bestand;"
					strm << "29. yit.getInt32( 0 );:" << yit.getInt32( 0 ) << endl;
				} catch( const basar::Exception& e ){
					report( "", "get32()", e.what(), false, strm );
				} catch( ... ){
					report( "", "...-EXCEPTION: get32();", 
							"", false, strm );
				}
				
				try {
					strm << "30. yit.getString( \"artikel_name\" );:" << yit.getString( "artikel_name" ) << endl;
				} catch( const basar::Exception& e ){
					report( "", "getString()", e.what(), false, strm );
				} catch( ... ){
					report( "", "...-EXCEPTION: getString();", 
							"", false, strm );
				}
				try {
					strm << "31. yit.getString( 1 );:" << yit.getString( 1 ) << endl;
				} catch( const basar::Exception& e ){
					report( "", "getString()", e.what(), false, strm );
				} catch( ... ){
					report( "", "...-EXCEPTION: getString();", 
							"", false, strm );
				}
				
				try {
					strm << "32. yit.getInt16( \"filialnr\" );:" << yit.getInt16( "filialnr" ) << endl;
				} catch( const basar::Exception& e ){
					report( "", "get16()", e.what(), false, strm );
				} catch( ... ){
					report( "", "...-EXCEPTION: get16();", 
							"", false, strm );
				}
				try {
					strm << "33. yit.getInt16( 2 );:" << yit.getInt16( 2 ) << endl;
				} catch( const basar::Exception& e ){
					report( "", "get16()", e.what(), false, strm );
				} catch( ... ){
					report( "", "...-EXCEPTION: get16();", 
							"", false, strm );
				}

				try {
					strm << "34. yit.getInt32( \"bestand\" );:" << yit.getInt32( "bestand" ) << endl;
				} catch( const basar::Exception& e ){
					report( "", "get32();", e.what(), false, strm );
				} catch( ... ){
					report( "", "...-EXCEPTION: getInt32();", 
							"", false, strm );
				}
				try {
					strm << "35. yit.getInt32( 3 );:" << yit.getInt32( 3 ) << endl;
				} catch( const basar::Exception& e ){
					report( "", "get32();", e.what(), false, strm );
				} catch( ... ){
					report( "", "...-EXCEPTION: getInt32();", 
							"", false, strm );
				}

				//////////////////////////////////////////////////////////////////
				// read in all
				//////////////////////////////////////////////////////////////////
				for( int z = 0; z < 2; ++z ){
					if( 0 == z ){
						try {
							strm << "36. yit = air.getPropertyTable().begin(SS_CLEAN):" << endl;
							yit = air.getPropertyTable().begin(basar::SS_CLEAN);
						} catch( const basar::Exception& e ){
							report( "", "yit = air.getPropertyTable().begin(SS_CLEAN)", e.what(), false, strm );
						} catch( ... ){
							report( "", "...-EXCEPTION: yit = air.getPropertyTable().begin(SS_CLEAN)", 
							"", false, strm );
						}
					} else {
						try {
							strm << "37. yit = air.getPropertyTable().begin(SS_UPDATE):" << endl;
							yit = air.getPropertyTable().begin(basar::SS_UPDATE);
						} catch( const basar::Exception& e ){
							report( "", "yit = air.getPropertyTable().begin(SS_UPDATE)", e.what(), false, strm );
						} catch( ... ){
							report( "", "...-EXCEPTION: yit = air.getPropertyTable().begin(SS_UPDATE)", 
									"", false, strm );
						}
					}

					int zz =  1;
					//////////////////////////////////////////////////////////////////
					// for(; yit != air.getPropertyTable().end(); ++yit, ++zz)
					//////////////////////////////////////////////////////////////////
					strm << "38. yit != air.getPropertyTable().end(); ++yit ); ++yit: " << endl;
					for(; yit != air.getPropertyTable().end(); ++yit, ++zz){
						strm	<< endl << "--------------------- Schleifen-Durchlauf (start) " 
								<< zz << " ---------------------" << endl;

						try {
							strm << "yit.getRowNumber(): " << yit.getRowNumber() << endl;
						} catch( const basar::Exception& e ){
							report( "", "yit.getRowNumber()", e.what(), false, strm );
						} catch( ... ){
							report( "", "...-EXCEPTION: yit.getRowNumber()", 
									"", false, strm );
						}
						//////////////////////////////////////////////////////////////////
						// contains()
						//////////////////////////////////////////////////////////////////
						try {	
							os.str("");
							os	<< "39. yit.contains(\"artikel_name\", basar::STRING, basar::SS_UPDATE): " 
								<< boolalpha << yit.contains("artikel_name", basar::STRING, basar::SS_UPDATE) << endl;
							strm << os.str();
						} catch( const basar::Exception& e ){
							report( "", "contains()", e.what(), false, strm );
						} catch( ... ){
							report( "", "...-EXCEPTION: contains()", 
									"", false, strm );
						}	
						try {		
							std::pair<VarString, basar::db::aspect::PropertyType> pair("artikel_name", basar::STRING ); 
							os.str("");
							os	<< "40. yit.contains( pair, SS_UPDATE): " 
								<< boolalpha << yit.contains( pair, basar::SS_UPDATE) << endl;
							strm << os.str();
						} catch( const basar::Exception& e ){
							report( "", "contains()", e.what(), false, strm );
						} catch( ... ){
							report( "", "...-EXCEPTION: contains()", 
									"", false, strm );
						}
						strm	<< "--------------------- Schleifen-Durchlauf (stop) " 
								<< zz << " ---------------------" << endl << endl;
					} // end for(; yit != air.getPropertyTable().end(); ++yit, ++zz)
				}	// end for( int z = 0; z < 2; ++z )

				///////////////////////////////////////////////////////////////////////
				//	begin(basar::SS_UPDATE), setStateUnknown(yit), markForDelete(yit)
				//	erase(yit), begin(basar::SS_UNKNOWN), 
				///////////////////////////////////////////////////////////////////////
				try {
					strm << "41. yit = air.getPropertyTable().begin(SS_UPDATE):" << endl;
					yit = air.getPropertyTable().begin(basar::SS_UPDATE);
					strm << "OK" << endl;
				} catch( const basar::Exception& e ){
					report( "", "yit = air.getPropertyTable().begin(SS_UPDATE)", e.what(), false, strm );
				} catch( ... ){
					report( "", "...-EXCEPTION: yit = air.getPropertyTable().begin(SS_UPDATE)", 
							"", false, strm );
				}
				try {
					strm << "42. yit.getRowNumber(): " << yit.getRowNumber() << endl;
				} catch( const basar::Exception& e ){
					report( "", "yit.getRowNumber()", e.what(), false, strm );
				} catch( ... ){
					report( "", "...-EXCEPTION: yit.getRowNumber()", 
							"", false, strm );
				}

				try {
					strm << "43. air.getPropertyTable().setStateUnknown( yit ):" << endl;
					air.getPropertyTable().setStateUnknown( yit );
					strm << "OK" << endl;
				} catch( const basar::Exception& e ){
					report( "", "setStateUnknown(), getRowNumber()", e.what(), false, strm );
				} catch( ... ){
					report( "", "...-EXCEPTION: setStateUnknown(), getRowNumber()", 
							"", false, strm );
				}
				try {
					strm << "44. yit.getRowNumber(): " << yit.getRowNumber() << endl;
				} catch( const basar::Exception& e ){
					report( "", "yit.getRowNumber()", e.what(), false, strm );
				} catch( ... ){
					report( "", "...-EXCEPTION: yit.getRowNumber()", 
							"", false, strm );
				}

				try {
					strm << "45. air.getPropertyTable().markForDelete( yit ):" << endl;
					air.getPropertyTable().markForDelete( yit );
					strm << "OK" << endl;
				} catch( const basar::Exception& e ){
					report( "", "air.getPropertyTable().markForDelete( yit )", e.what(), false, strm );
				} catch( ... ){
					report( "", "...-EXCEPTION: air.getPropertyTable().markForDelete( yit )", 
							"", false, strm );
				}

				try {
					strm << "46. air.getPropertyTable().erase( yit ):" << endl;
					air.getPropertyTable().erase( yit );
				} catch( const basar::Exception& e ){
					report( "", "air.getPropertyTable().erase( yit )", e.what(), false, strm );
				} catch( ... ){
					report( "", "... EXCEPTION: air.getPropertyTable().erase( yit )", "", false, strm );
				}
				try {
					strm << "47. yit.getRowNumber(): " << yit.getRowNumber() << endl;
				} catch( const basar::Exception& e ){
					report( "", "yit.getRowNumber()", e.what(), false, strm );
				} catch( ... ){
					report( "", "... EXCEPTION: yit.getRowNumber()", "", false, strm );
				}

				try {
					strm << "48. yit = air.getPropertyTable().begin(SS_UNKNOWN):" << endl;
					yit = air.getPropertyTable().begin(basar::SS_UNKNOWN);
					strm << "OK" << endl;
				} catch( const basar::Exception& e ){
					report( "", "yit = air.getPropertyTable().begin(SS_UNKNOWN)", e.what(), false, strm );
				} catch( ... ){
					report( "", "...-EXCEPTION: yit = air.getPropertyTable().begin(SS_UNKNOWN)", 
							"", false, strm );
				}
				try {
					strm << "49. yit.getRowNumber(): " << yit.getRowNumber() << endl;
				} catch( const basar::Exception& e ){
					report( "", "yit.getRowNumber()", e.what(), false, strm );
				} catch( ... ){
					report( "", "...-EXCEPTION: yit.getRowNumber()", 
							"", false, strm );
				}

				///////////////////////////////////////////////////////////////////////////
				//	checkSizeAndEmpty(air, strm), end(), erase(yit, yend), getRowNumber()
				///////////////////////////////////////////////////////////////////////////
				checkSizeAndEmpty( air, strm );

				AccessorPropertyTable_YIterator yend;
				try {
					strm << "450. air.getPropertyTable().end():";
					yend = air.getPropertyTable().end();
					strm << "OK" << endl;
				} catch( const basar::Exception& e ){
					report( "", "air.getPropertyTable().end()", e.what(), false, strm );
				} catch( ... ){
					report( "", "... EXCEPTION: air.getPropertyTable().end()", "", false, strm );
				}
				try {
					strm << "51. air.getPropertyTable().erase( yit, yend ):";
					air.getPropertyTable().erase( yit, yend );
					strm << "OK" << endl;
				} catch( const basar::Exception& e ){
					report( "", "air.getPropertyTable().erase( yit, yend )", e.what(), false, strm );
				} catch( ... ){
					report( "", "... EXCEPTION: air.getPropertyTable().erase( yit, yend )", "", false, strm );
				}

				try {
					strm << "52. yit.getRowNumber(): " << yit.getRowNumber() << endl;
				} catch( const basar::Exception& e ){
					report( "", "yit.getRowNumber()", e.what(), false, strm );
				} catch( ... ){
					report( "", "... EXCEPTION: yit.getRowNumber()", "", false, strm );
				}
				try {
					strm << "53. yend.getRowNumber(): " << yend.getRowNumber() << endl;
				} catch( const basar::Exception& e ){
					report( "", "yend.getRowNumber()", e.what(), false, strm );
				} catch( ... ){
					report( "", "... EXCEPTION: yend.getRowNumber()", "", false, strm );
				}

				///////////////////////////////////////////////////////////////////////////
				//	checkSizeAndEmpty(air, strm), end(), erase(yit, yend), 
				//	checkSizeAndEmpty(air, strm), insert(basar::FOR_CLEAN), getRowNumber(), 
				//	checkSizeAndEmpty(air, strm), 
				//	createSnapshot(), switchCreateInternalSnapshot(false), 
				///////////////////////////////////////////////////////////////////////////
				checkSizeAndEmpty( air, strm );

				try {
					strm << "54. air.getPropertyTable().end(): ";
					AccessorPropertyTable_YIterator yend = air.getPropertyTable().end();
					strm << "OK" << endl;
				} catch( const basar::Exception& e ){
					report( "", "air.getPropertyTable().end()", e.what(), false, strm );
				} catch( ... ){
					report( "", "... EXCEPTION: air.getPropertyTable().end()", "", false, strm );
				}
				
				try {
					strm << "55. air.getPropertyTable().erase( yit, yend ): ";
					air.getPropertyTable().erase( yit, yend );
					strm << "OK" << endl;
				} catch( const basar::Exception& e ){
					report( "", "air.getPropertyTable().erase( yit, yend )", e.what(), false, strm );
				} catch( ... ){
					report( "", "... EXCEPTION: air.getPropertyTable().erase( yit, yend )", "", false, strm );
				}

				checkSizeAndEmpty( air, strm );

				try {
					strm << "56. air.getPropertyTable().insert( FOR_CLEAN ): ";
					yit = air.getPropertyTable().insert( basar::FOR_CLEAN );
					strm << "OK" << endl;
				} catch( const basar::Exception& e ){
					report( "", "air.getPropertyTable().insert( FOR_CLEAN )", e.what(), false, strm );
				} catch( ... ){
					report( "", "...-EXCEPTION: air.getPropertyTable().insert( FOR_CLEAN )", 
							"", false, strm );
				}
				try {
					strm << "57. yit.getRowNumber(): " << yit.getRowNumber() << endl;
				} catch( const basar::Exception& e ){
					report( "", "yit.getRowNumber()", e.what(), false, strm );
				} catch( ... ){
					report( "", "...-EXCEPTION: yit.getRowNumber()", "", false, strm );
				}

				checkSizeAndEmpty( air, strm );

				basar::property::PropertyTableSnapshotRef schnappi;
				try {
					strm << "58. air.createSnapshot(): " ;
					schnappi = air.createSnapshot();
					strm << "OK" << endl;
				} catch( const basar::Exception& e ){
					report( "", "air.createSnapshot()", e.what(), false, strm );
				} catch( ... ){
					report( "", "...-EXCEPTION: air.createSnapshot()", 
							"", false, strm );
				}

				try {
					strm << "59. air.switchCreateInternalSnapshot( false ): ";
					air.switchCreateInternalSnapshot( false );
					strm << "OK" << endl;
				} catch( const basar::Exception& e ){
					report( "", "air.switchCreateInternalSnapshot( false )", e.what(), false, strm );
				} catch( ... ){
					report( "", "... EXCEPTION: air.switchCreateInternalSnapshot( false )", "", false, strm );
				}

				try {
					strm << "60. air.resetToSnapshot( schnappi ): ";
					air.resetToSnapshot( schnappi );
					strm << "OK" << endl;
				} catch( const basar::Exception& e ){
					report( "", "air.resetToSnapshot( schnappi )", e.what(), false, strm );
				} catch( ... ){
					report( "", "... EXCEPTION: air.resetToSnapshot( schnappi )", "", false, strm );
				}
			}	
		} // end for caching policies
	} // end for hits

	strm << "====================================================================" << endl;
	strm << fun << " - Exit!" << endl;
	strm << "====================================================================" << endl;
}
//--------------------------------------------------------------------------------------------------------//
// void check_NO_CACHING( AccessorInstanceRef air, ostream& strm = std::cout )
//--------------------------------------------------------------------------------------------------------//
void check_NO_CACHING( AccessorInstanceRef air, ostream& strm )
{
	static const char* const fun = "check_NO_CACHING( AccessorInstanceRef, ostream&)";

	strm << "--------------------------------------------------------------------" << endl;
	strm << fun << " - Entry!" << endl;
	strm << "--------------------------------------------------------------------" << endl;

	AccessorPropertyTable_YIterator yit, yitEnd;
	checkSizeAndEmpty( air, strm );
	ostringstream os;

	//////////////////////////////////////////////////////////////////
	// begin(), isBegin()
	//////////////////////////////////////////////////////////////////
	try {
		os.str("");
		os << boolalpha << "1. yit.isBegin(): " << yit.isBegin();	
		strm << os.str() << " OK;" << endl;
	} catch( const basar::Exception& e ){
		report( "", "1. yit.isBegin()", e.what(), false, strm );
	}
	try {
		strm << "2. yit = air.getPropertyTable().begin(): ";
		yit = air.getPropertyTable().begin();	
		strm << os.str() << " OK;" << endl;
	} catch( const basar::Exception& e ){
		report( "", "2. yit = air.getPropertyTable().begin()", e.what(), false, strm );
	}
	try {
		os.str("");
		os << boolalpha << "3. yit.isBegin(): " << yit.isBegin();	
		strm << os.str() << " OK;" << endl;
	} catch( const basar::Exception& e ){
		report( "", "3. yit.isBegin()", e.what(), false, strm );
	}
	
	//////////////////////////////////////////////////////////////////
	// end(), isEnd()
	//////////////////////////////////////////////////////////////////
	try {
		os.str("");
		os << boolalpha << "4. yitEnd.isEnd(): " << yitEnd.isEnd();	
		strm << os.str() << " OK;" << endl;
	} catch( const basar::Exception& e ){
		report( "", "4. yitEnd.isEnd()", e.what(), false, strm );
	}
	try {
		strm << "5. yitEnd = air.getPropertyTable().end(): ";
		yitEnd = air.getPropertyTable().end();	
		strm << " OK;" << endl;
	} catch( const basar::Exception& e ){
		report( "", "5. yitEnd = air.getPropertyTable().end()", e.what(), false, strm );
	}
	try {
		os.str("");
		os << boolalpha << "6. yitEnd.isEnd(): " << yitEnd.isBegin();	
		strm << os.str() << " OK;" << endl;
	} catch( const basar::Exception& e ){
		report( "", "6. yitEnd.isEnd()", e.what(), false, strm );
	}

	//-----------------------------------------------------------------------------------------------------//
	// while
	//-----------------------------------------------------------------------------------------------------//
	int bb = 1;
	while( yit != yitEnd )
	{
		checkFilterAndState( yit, strm );
		
		/////////////////////////////////////////////////////////////////////////////////
		// gets via index and name in the context of a yiterator
		/////////////////////////////////////////////////////////////////////////////////
		try {
			//"int32 artikel_nr; string artikel_name; int16 filialnr; int32 bestand;"
			strm << bb << "-" << "7. yit.getInt32( 0 );:" << yit.getInt32( 0 ) << endl;
		} catch( const basar::Exception& e ){
			report( "", "7. get32()", e.what(), false, strm );
		} catch( ... ){
			report( "", "7. ...-EXCEPTION: get32();", 
					"", false, strm );
		}	
		try {
			strm << bb << "-" << "8. yit.getString( \"artikel_name\" );:" << yit.getString( "artikel_name" ) << endl;
		} catch( const basar::Exception& e ){
			report( "", "8. getString()", e.what(), false, strm );
		} catch( ... ){
			report( "", "8. ...-EXCEPTION: getString();", 
					"", false, strm );
		}
		try {
			strm << bb << "-" << "9. yit.getString( 1 );:" << yit.getString( 1 ) << endl;
		} catch( const basar::Exception& e ){
			report( "", "9. getString()", e.what(), false, strm );
		} catch( ... ){
			report( "", "9. ...-EXCEPTION: getString();", 
					"", false, strm );
		}
		try {
			strm << bb << "-" << "10. yit.getInt16( \"filialnr\" );:" << yit.getInt16( "filialnr" ) << endl;
		} catch( const basar::Exception& e ){
			report( "", "10. get16()", e.what(), false, strm );
		} catch( ... ){
			report( "", "10. ...-EXCEPTION: get16();", 
					"", false, strm );
		}
		try {
			strm << bb << "-" << "11. yit.getInt16( 2 );:" << yit.getInt16( 2 ) << endl;
		} catch( const basar::Exception& e ){
			report( "", "11. get16()", e.what(), false, strm );
		} catch( ... ){
			report( "", "11. ...-EXCEPTION: get16();", 
					"", false, strm );
		}
		try {
			strm << bb << "-" << "12. yit.getInt32( \"bestand\" );:" << yit.getInt32( "bestand" ) << endl;
		} catch( const basar::Exception& e ){
			report( "", "12. get32();", e.what(), false, strm );
		} catch( ... ){
			report( "", "12. ...-EXCEPTION: getInt32();", 
					"", false, strm );
		}
		try {
			strm << bb << "-" << "13. yit.getInt32( 3 );:" << yit.getInt32( 3 ) << endl;
		} catch( const basar::Exception& e ){
			report( "", "13. get32();", e.what(), false, strm );
		} catch( ... ){
			report( "", "13. ...-EXCEPTION: getInt32();", 
					"", false, strm );
		}
		
		////////////////////////////////////////////////////////////////////////
		// yit.getRowNumber()
		////////////////////////////////////////////////////////////////////////
		try {
			strm << bb << "-" << "14. yit.getRowNumber(): " << yit.getRowNumber() << endl;
		} catch( const basar::Exception& e ){
			report( "", "14. yit.getRowNumber()", e.what(), false, strm );
		}
		
		////////////////////////////////////////////////////////////////////////
		// try to set a value
		////////////////////////////////////////////////////////////////////////
		try {
			strm << bb << "-" << "15. yit.setString( \"artikel_name\", \"keine Aspirin mehr fuer 4\" ): ";
			yit.setString( "artikel_name", "keine Aspirin mehr fuer 4" ); 
			strm << " OK" << endl;
		} catch( const basar::Exception& e ){
			report( "", "15. yit.setString( \"artikel_name\", \"keine Aspirin mehr fuer 4\" )", e.what(), false, strm );
		} catch( ... ){
			report( "", "15. ...-EXCEPTION: yit.setString( \"artikel_name\", \"keine Aspirin mehr fuer 4\" )", 
					"", false, strm );
		}
		
		//////////////////////////////////////////////////////////////////
		// contains()
		//////////////////////////////////////////////////////////////////
		try {	
			os.str("");
			os	<< bb << "-" << "16. yit.contains(\"artikel_name\", basar::STRING, basar::SS_UPDATE): " 
				<< boolalpha << yit.contains("artikel_name", basar::STRING, basar::SS_UPDATE) << endl;
			strm << os.str();
		} catch( const basar::Exception& e ){
			report( "", "16. contains()", e.what(), false, strm );
		} catch( ... ){
			report( "", "16. ...-EXCEPTION: contains()", 
					"", false, strm );
		}	
		try {		
			std::pair<VarString, basar::db::aspect::PropertyType> pair("artikel_name", basar::STRING ); 
			os.str("");
			os	<< bb << "-" << "17. yit.contains( pair, SS_UPDATE): " 
				<< boolalpha << yit.contains( pair, basar::SS_UPDATE) << endl;
			strm << os.str();
		} catch( const basar::Exception& e ){
			report( "", "17. contains()", e.what(), false, strm );
		} catch( ... ){
			report( "", "17. ...-EXCEPTION: contains()", 
					"", false, strm );
		}
		
		//////////////////////////////////////////////////////////////////
		// setStateUnknown( yit )
		//////////////////////////////////////////////////////////////////
		try {
			strm << bb << "-" << "18. air.getPropertyTable().setStateUnknown( yit ):" << endl;
			air.getPropertyTable().setStateUnknown( yit );
			strm << " OK" << endl;
		} catch( const basar::Exception& e ){
			report( "", "18. setStateUnknown(), getRowNumber()", e.what(), false, strm );
		} catch( ... ){
			report( "", "18. ...-EXCEPTION: setStateUnknown(), getRowNumber()", 
					"", false, strm );
		}

		////////////////////////////////////////////////////////////////////////
		// incrementing for loop
		////////////////////////////////////////////////////////////////////////
		try {
			strm << bb << "-" << "19. ++yit: " << endl;
			++yit;
			strm << " OK;"<< endl;
		} catch( const basar::Exception& e ){
			report( "", "19. ++yit", e.what(), false, strm );
		}
		++bb;
	} // end while
	//-----------------------------------------------------------------------------------------------------//

	//////////////////////////////////////////////////////////////////
	// setString(), begin(), begin(filter)
	//////////////////////////////////////////////////////////////////
	try {
		strm << "20. yit is shifted 1 times with postincrement: " ;
		//yit++; yit++; yit++; 
		yit++;
		strm << " OK;" << endl;
	} catch( const basar::Exception& e ){
		report( "", "20. yit++; yit++; yit++;", e.what(), false, strm );
	}
	
	checkFilterAndState( yit, strm );

	try {
		strm << "21. yit.getRowNumber(): " << yit.getRowNumber() << endl;
	} catch( const basar::Exception& e ){
		report( "", "21. yit.getRowNumber()", e.what(), false, strm );
	}

	try {
		strm << "22. yit = air.getPropertyTable().begin(): ";
		yit = air.getPropertyTable().begin();	
		strm << " OK;" << endl;
	} catch( const basar::Exception& e ){
		report( "", "22. yit = air.getPropertyTable().begin()", e.what(), false, strm );
	} catch( ... ){
		report( "", "22. ...-EXCEPTION: yit = air.getPropertyTable().begin()", 
				"", false, strm );
	}

	try {
		strm << "23. yit = air.getPropertyTable().begin(SS_UPDATE): ";
		yit = air.getPropertyTable().begin( basar::SS_UPDATE );	
		strm << " OK" << endl;
	} catch( const basar::Exception& e ){
		report( "", "23. yit = air.getPropertyTable().begin(SS_UPDATE)", e.what(), false, strm );
	} catch( ... ){
		report( "", "23. ...-EXCEPTION: yit = air.getPropertyTable().begin(SS_UPDATE)", 
				"", false, strm );
	}
	
	try {
		strm << "24. yit.getRowNumber(): " << yit.getRowNumber() << endl;
	} catch( const basar::Exception& e ){
		report( "", "24. yit.getRowNumber()", e.what(), false, strm );
	} catch( ... ){
		report( "", "24. ...-EXCEPTION: yit.getRowNumber()", 
				"", false, strm );
	}

	try {
		strm << "25. air.getPropertyTable().markForDelete( yit ):" << endl;
		air.getPropertyTable().markForDelete( yit );
		strm << " OK" << endl;
	} catch( const basar::Exception& e ){
		report( "", "25. air.getPropertyTable().markForDelete( yit )", e.what(), false, strm );
	} catch( ... ){
		report( "", "25. ...-EXCEPTION: air.getPropertyTable().markForDelete( yit )", 
				"", false, strm );
	}

	try {
		strm << "26. air.getPropertyTable().erase( yit ):" << endl;
		air.getPropertyTable().erase( yit );
	} catch( const basar::Exception& e ){
		report( "", "26. air.getPropertyTable().erase( yit )", e.what(), false, strm );
	} catch( ... ){
		report( "", "26. ... EXCEPTION: air.getPropertyTable().erase( yit )", "", false, strm );
	}
	try {
		strm << "27. yit.getRowNumber(): " << yit.getRowNumber() << endl;
	} catch( const basar::Exception& e ){
		report( "", "27. yit.getRowNumber()", e.what(), false, strm );
	} catch( ... ){
		report( "", "27. ... EXCEPTION: yit.getRowNumber()", "", false, strm );
	}

	try {
		strm << "28. yit = air.getPropertyTable().begin(SS_UNKNOWN):" << endl;
		yit = air.getPropertyTable().begin(basar::SS_UNKNOWN);
		strm << " OK" << endl;
	} catch( const basar::Exception& e ){
		report( "", "28. yit = air.getPropertyTable().begin(SS_UNKNOWN)", e.what(), false, strm );
	} catch( ... ){
		report( "", "28. ...-EXCEPTION: yit = air.getPropertyTable().begin(SS_UNKNOWN)", 
				"", false, strm );
	}
	try {
		strm << "29. yit.getRowNumber(): " << yit.getRowNumber() << endl;
	} catch( const basar::Exception& e ){
		report( "", "29. yit.getRowNumber()", e.what(), false, strm );
	} catch( ... ){
		report( "", "29. ...-EXCEPTION: yit.getRowNumber()", 
				"", false, strm );
	}

	AccessorPropertyTable_YIterator yend;
	try {
		strm << "30. air.getPropertyTable().erase( yit, yend ):";
		air.getPropertyTable().erase( yit, yend );
		strm << " OK" << endl;
	} catch( const basar::Exception& e ){
		report( "", "30. air.getPropertyTable().erase( yit, yend )", e.what(), false, strm );
	} catch( ... ){
		report( "", "30. ... EXCEPTION: air.getPropertyTable().erase( yit, yend )", "", false, strm );
	}

	
	try {
		strm << "31. air.getPropertyTable().erase( yit, yend ): ";
		air.getPropertyTable().erase( yit, yend );
		strm << " OK" << endl;
	} catch( const basar::Exception& e ){
		report( "", "31. air.getPropertyTable().erase( yit, yend )", e.what(), false, strm );
	} catch( ... ){
		report( "", "31. ... EXCEPTION: air.getPropertyTable().erase( yit, yend )", "", false, strm );
	}


	try {
		strm << "32. air.getPropertyTable().insert( FOR_CLEAN ): ";
		yit = air.getPropertyTable().insert( basar::FOR_CLEAN );
		strm << " OK" << endl;
	} catch( const basar::Exception& e ){
		report( "", "32. air.getPropertyTable().insert( FOR_CLEAN )", e.what(), false, strm );
	} catch( ... ){
		report( "", "32. ...-EXCEPTION: air.getPropertyTable().insert( FOR_CLEAN )", 
				"", false, strm );
	}
	try {
		strm << "33. yit.getRowNumber(): " << yit.getRowNumber() << endl;
	} catch( const basar::Exception& e ){
		report( "", "33. yit.getRowNumber()", e.what(), false, strm );
	} catch( ... ){
		report( "", "33. ...-EXCEPTION: yit.getRowNumber()", "", false, strm );
	}

	checkSizeAndEmpty( air, strm );

	basar::property::PropertyTableSnapshotRef schnappi;
	try {
		strm << "34. air.createSnapshot(): " ;
		schnappi = air.createSnapshot();
		strm << " OK" << endl;
	} catch( const basar::Exception& e ){
		report( "", "34. air.createSnapshot()", e.what(), false, strm );
	} catch( ... ){
		report( "", "34. ...-EXCEPTION: air.createSnapshot()", 
				"", false, strm );
	}

	try {
		strm << "35>. air.switchCreateInternalSnapshot( false ): ";
		air.switchCreateInternalSnapshot( false );
		strm << " OK" << endl;
	} catch( const basar::Exception& e ){
		report( "", "35. air.switchCreateInternalSnapshot( false )", e.what(), false, strm );
	} catch( ... ){
		report( "", "35. ... EXCEPTION: air.switchCreateInternalSnapshot( false )", "", false, strm );
	}

	try {
		strm << "36. air.resetToSnapshot( schnappi ): ";
		air.resetToSnapshot( schnappi );
		strm << " OK" << endl;
	} catch( const basar::Exception& e ){
		report( "", "36. air.resetToSnapshot( schnappi )", e.what(), false, strm );
	} catch( ... ){
		report( "", "36. ... EXCEPTION: air.resetToSnapshot( schnappi )", "", false, strm );
	}
		
	strm << "--------------------------------------------------------------------" << endl;
	strm << fun << " - Exit!" << endl;
	strm << "--------------------------------------------------------------------" << endl;
}
//--------------------------------------------------------------------------------------------------------//
// AccessorInstanceRef createAccessor4checkPolicies( ConnectionRef connection, CachingPolicyEnum eCachingPolicy )
//--------------------------------------------------------------------------------------------------------//
AccessorInstanceRef createAccessor4checkPolicies( ConnectionRef connection, CachingPolicyEnum eCachingPolicy )
{
	AccessorInstanceRef air;
	try {
		air = basar::db::aspect::Manager::getInstance().createAccessorInstance(
				"chkPolicy", "AccessorArticle", connection, eCachingPolicy );		
	} catch( const basar::Exception& ex ){
		cout << ex.what() << endl;
	}
	
	return air;
}
//--------------------------------------------------------------------------------------------------------//
// AccessorInstanceRef createAccessor4checkPolicies( ConnectionRef connection, CachingPolicyEnum eCachingPolicy )
//--------------------------------------------------------------------------------------------------------//
void checkSizeAndEmpty( AccessorInstanceRef air, ostream& strm )
{
	ostringstream os;
	////////////////////////////////////////////////////////////////////
	//// size
	////////////////////////////////////////////////////////////////////
	try {
		os.str("");
		os << static_cast<int>(air.getPropertyTable().size());	
		report( "", "air.getPropertyTable().size();", os.str().c_str(), true, strm );
	} catch( const basar::Exception& e ){
		report( "", "air.getPropertyTable().size();", e.what(), false, strm );
	}
	//////////////////////////////////////////////////////////////////
	// empty()
	//////////////////////////////////////////////////////////////////
	try {
		os.str("");
		os << boolalpha << air.getPropertyTable().empty();	
		report( "", "air.getPropertyTable().empty();", os.str().c_str(), true, strm );
	} catch( const basar::Exception& e ){
		report( "", "air.getPropertyTable().empty();", e.what(), false, strm );
	}
}
//--------------------------------------------------------------------------------------------------------//
// void checkFilterAndState( air, strm )
//--------------------------------------------------------------------------------------------------------//
void checkFilterAndState( AccessorPropertyTable_YIterator yit, ostream& strm )
{
	try {
		strm << "yit.getFilter().toString().c_str(): " << yit.getFilter().toString().c_str() << endl;	
	} catch( const basar::Exception& e ){
		report( "", "yit.getFilter().toString().c_str()", e.what(), false, strm );
	}
	try {
		strm << "yit.getState().toString().c_str(): " << yit.getState().toString().c_str() << endl;
	} catch( const basar::Exception& e ){
		report( "", "yit.getState().toString().c_str()", e.what(), false, strm );
	}
}
