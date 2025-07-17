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
// void check_Policies( ConnectionRef connection, AccessorInstanceRef air )
//--------------------------------------------------------------------------------------------------------//
void check_Accessor( ConnectionRef connection, AccessorInstanceRef air )
{
	static const char* const fun = "check_Accessor(ConnectionRef, AccessorInstanceRef)";

	//ostream& strm = cout;
	ofstream log ( "aspect.check_accessor.log" ); ostream& strm = log;
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
					basar::I18nString s("artikel_nr = 1057;" );
					basar::db::aspect::AccessorPropertyTable_YIterator yit = basar::db::aspect::Manager::getInstance().createAccessorYIterator(s);
					air.execute("ReadArticle", yit);
				} else if( 1 == i ) {
					strm << "Query without hits!" << endl;
					air.getPropertyTable().clear();
					basar::I18nString s("artikel_nr = 9999999;" );
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
			
			//////////////////////////////////////////////////////////////////
			// size
			//////////////////////////////////////////////////////////////////
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

			AccessorPropertyTable_YIterator yit;
			
			//////////////////////////////////////////////////////////////////
			// begin
			//////////////////////////////////////////////////////////////////
			try {
				report( "", "air.getPropertyTable().begin();", "", true, strm );
				yit = air.getPropertyTable().begin();	
			} catch( const basar::Exception& e ){
				report( "", "air.getPropertyTable().begin();", e.what(), false, strm );
			}
			
			//////////////////////////////////////////////////////////////////
			// isBegin()
			//////////////////////////////////////////////////////////////////
			try {
				os.str("");
				os << boolalpha << yit.isBegin();	
				report( "", "yit.isBegin();", os.str().c_str(), true, strm );
			} catch( const basar::Exception& e ){
				report( "", "yit.isBegin();", e.what(), false, strm );
			}

			//////////////////////////////////////////////////////////////////
			// isEnd()
			//////////////////////////////////////////////////////////////////
			try {
				os.str("");
				os << boolalpha << yit.isEnd();	
				report( "", "yit.isEnd();", os.str().c_str(), true, strm );
			} catch( const basar::Exception& e ){
				report( "", "yit.isEnd();", e.what(), false, strm );
			}

			//////////////////////////////////////////////////////////////////
			// insert
			//////////////////////////////////////////////////////////////////
			try {
				air.getPropertyTable().insert( basar::FOR_CLEAN );
				report( "", "air.getPropertyTable().insert( FOR_CLEAN )", "", true, strm );
			} catch( const basar::Exception& e ){
				report( "", "air.getPropertyTable().insert( FOR_CLEAN )", e.what(), false, strm );
			}
			try {
				air.getPropertyTable().insert( basar::FOR_INSERT );
				report( "", "air.getPropertyTable().insert( FOR_INSERT )", "", true, strm );
			} catch( const basar::Exception& e ){
				report( "", "air.getPropertyTable().insert( FOR_INSERT )", e.what(), false, strm );
			}
			try {
				air.getPropertyTable().insert( basar::FOR_UPDATE );
				report( "", "air.getPropertyTable().insert( FOR_UPDATE )", "", true, strm );
			} catch( const basar::Exception& e ){
				report( "", "air.getPropertyTable().insert( FOR_UPDATE )", e.what(), false, strm );
			}
			try {
				air.getPropertyTable().insert( basar::FOR_DELETE );
				report( "", "air.getPropertyTable().insert( FOR_DELETE )", "", true, strm );
			} catch( const basar::Exception& e ){
				report( "", "air.getPropertyTable().insert( FOR_DELETE )", e.what(), false, strm );
			}
			
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

			//////////////////////////////////////////////////////////////////
			// markForDelete, setStateUnknown
			//////////////////////////////////////////////////////////////////
			try {
				yit = air.getPropertyTable().begin();
				report( "", "yit = air.getPropertyTable().begin();", "", true, strm );
				air.getPropertyTable().markForDelete( yit );
				report( "", "air.getPropertyTable().begin().markForDelete();", "", true, strm );
			} catch( const basar::Exception& e ){
				report( "", "air.getPropertyTable().begin().markForDelete();", e.what(), false, strm );
			}
			try {
				air.getPropertyTable().setStateUnknown( air.getPropertyTable().begin() );
				report( "", "air.getPropertyTable().begin().setStateUnknown();", "", true, strm );
			} catch( const basar::Exception& e ){
				report( "", "air.getPropertyTable().begin().setStateUnknown();", e.what(), false, strm );
			}

			//////////////////////////////////////////////////////////////////
			// erase
			//////////////////////////////////////////////////////////////////
			try {
				yit = air.getPropertyTable().begin();
				air.getPropertyTable().erase( yit );
				report( "", "air.getPropertyTable().erase( air.getPropertyTable().begin());", "", true, strm );
			} catch( const basar::Exception& e ){
				report( "", "air.getPropertyTable().erase( air.getPropertyTable().begin());", e.what(), false, strm );
			}
			
			// size
			try {
				os.str("");
				os << static_cast<int>(air.getPropertyTable().size());	
				report( "", "air.getPropertyTable().size();", os.str().c_str(), true, strm );
			} catch( const basar::Exception& e ){
				report( "", "air.getPropertyTable().size();", e.what(), false, strm );
			}
			// empty()
			try {
				os.str("");
				os << boolalpha << air.getPropertyTable().empty();	
				report( "", "air.getPropertyTable().empty();", os.str().c_str(), true, strm );
			} catch( const basar::Exception& e ){
				report( "", "air.getPropertyTable().empty();", e.what(), false, strm );
			}

			try {
				report( "", "ausgeben:", "", true, strm );
				AccessorPropertyTable_YIterator yit = air.getPropertyTable().begin();

				yit.toStream();

				for( ; yit != air.getPropertyTable().end(); ++yit ){
					strm << "artikel_nr:   " << yit.getInt32 (	"artikel_nr"	) << endl;		
					strm << "artikel_name: " << yit.getString(	"artikel_name"	) << endl;
					strm << "filialnr:     " << yit.getInt16 (	"filialnr"		) << endl;
					strm << "bestand:      " << yit.getInt32 (	"bestand"		) << endl;
				}
			} catch( const basar::Exception& e ){
				report( "", "for( ; yit != air.getPropertyTable().end(); ++yit );", e.what(), false, strm );
			} catch( ... ){
				report( "", "EXCEPTION: for( ; yit != air.getPropertyTable().end(); ++yit );", "", false, strm );
			}
						
			
			try {
				// size()
				try {
					os.str("");
					os << static_cast<int>(air.getPropertyTable().size());	
					report( "", "air.getPropertyTable().size();", os.str().c_str(), true, strm );
				} catch( const basar::Exception& e ){
					report( "", "air.getPropertyTable().size();", e.what(), false, strm );
				}
				// empty()
				try {
					os.str("");
					os << boolalpha << air.getPropertyTable().empty();	
					report( "", "air.getPropertyTable().empty();", os.str().c_str(), true, strm );
				} catch( const basar::Exception& e ){
					report( "", "air.getPropertyTable().empty();", e.what(), false, strm );
				}
				
				
				air.getPropertyTable().erase( air.getPropertyTable().begin(), air.getPropertyTable().end() );
				AccessorPropertyTable_YIterator beg = air.getPropertyTable().begin();
				AccessorPropertyTable_YIterator end = air.getPropertyTable().end();
				
				report( "", "air.getPropertyTable().erase( beg, end );", "", true, strm );
				air.getPropertyTable().erase( beg, end );
				
			
				//AccessorPropertyTable_YIterator yit = air.getPropertyTable().begin();
				//for( ; yit != air.getPropertyTable().end(); ++yit ){
				//	air.getPropertyTable().erase( yit );
				//	try {
				//		os.str("");
				//		os << static_cast<int>(air.getPropertyTable().size());	
				//		report( "", "air.getPropertyTable().size();", os.str().c_str(), true, strm );
				//	} catch( const basar::Exception& e ){
				//		report( "", "air.getPropertyTable().size();", e.what(), false, strm );
				//	}
				//	// empty()
				//	try {
				//		os.str("");
				//		os << boolalpha << air.getPropertyTable().empty();	
				//		report( "", "air.getPropertyTable().empty();", os.str().c_str(), true, strm );
				//	} catch( const basar::Exception& e ){
				//		report( "", "air.getPropertyTable().empty();", e.what(), false, strm );
				//	}
				//}
				//report( "", "air.getPropertyTable().erase( air.getPropertyTable().begin());", "", true, strm );
			} catch( const basar::Exception& e ){
				report( "", "air.getPropertyTable().erase( air.getPropertyTable().begin());", e.what(), false, strm );
			}

			try {
				yit = air.getPropertyTable().end();
				air.getPropertyTable().erase( yit );
				report( "", "air.getPropertyTable().erase( air.getPropertyTable().end());", "", true, strm );
			} catch( const basar::Exception& e ){
				report( "", "air.getPropertyTable().erase( air.getPropertyTable().end());", e.what(), false, strm );
			}

			// size
			try {
				os.str("");
				os << static_cast<int>(air.getPropertyTable().size());	
				report( "", "air.getPropertyTable().size();", os.str().c_str(), true, strm );
			} catch( const basar::Exception& e ){
				report( "", "air.getPropertyTable().size();", e.what(), false, strm );
			}
			// empty()
			try {
				os.str("");
				os << boolalpha << air.getPropertyTable().empty();	
				report( "", "air.getPropertyTable().empty();", os.str().c_str(), true, strm );
			} catch( const basar::Exception& e ){
				report( "", "air.getPropertyTable().empty();", e.what(), false, strm );
			}

			try {
				yit = air.getPropertyTable().begin();
				AccessorPropertyTable_YIterator yit2 = air.getPropertyTable().end();
				//++yit2;
				os.str("");
				os << boolalpha << (yit == yit2);	
				report( "", "yit == yit2;", os.str().c_str(), true, strm );
				air.getPropertyTable().erase( yit, yit2 );
				report( "", "erase( air.getPropertyTable().begin(), air.getPropertyTable().end() );", "", true, strm );
			} catch( const basar::Exception& e ){
				report( "", "erase( air.getPropertyTable().begin(), air.getPropertyTable().end() );", e.what(), false, strm );
			}
			
			// size
			try {
				os.str("");
				os << static_cast<int>(air.getPropertyTable().size());	
				report( "", "air.getPropertyTable().size();", os.str().c_str(), true, strm );
			} catch( const basar::Exception& e ){
				report( "", "air.getPropertyTable().size();", e.what(), false, strm );
			}
			// empty()
			try {
				os.str("");
				os << boolalpha << air.getPropertyTable().empty();	
				report( "", "air.getPropertyTable().empty();", os.str().c_str(), true, strm );
			} catch( const basar::Exception& e ){
				report( "", "air.getPropertyTable().empty();", e.what(), false, strm );
			}
			
			//////////////////////////////////////////////////////////////////
			// createSnapshot
			//////////////////////////////////////////////////////////////////
			basar::db::aspect::PropertyTableSnapshotRef schni_schna_schnappi;
			try {
				schni_schna_schnappi = air.createSnapshot();
				report( "", "air.createSnapshot();", "", true, strm );
			} catch( const basar::Exception& e ){
				report( "", "air.createSnapshot();", e.what(), false, strm );
			}
			try {
				air.resetToSnapshot( schni_schna_schnappi );
				report( "", "air.resetToSnapshot( schnappi );", "", true, strm );
			} catch( const basar::Exception& e ){
				report( "", "air.resetToSnapshot( schnappi );", e.what(), false, strm );
			}
			
			//////////////////////////////////////////////////////////////////
			// size
			//////////////////////////////////////////////////////////////////
			try {
				os.str("");
				os << static_cast<int>(air.getPropertyTable().size());	
				report( "", "air.getPropertyTable().size();", os.str().c_str(), true, strm );
			} catch( const basar::Exception& e ){
				report( "", "air.getPropertyTable().size();", e.what(), false, strm );
			}
			// empty()
			try {
				os.str("");
				os << boolalpha << air.getPropertyTable().empty();	
				report( "", "air.getPropertyTable().empty();", os.str().c_str(), true, strm );
			} catch( const basar::Exception& e ){
				report( "", "air.getPropertyTable().empty();", e.what(), false, strm );
			}

		} // end for caching policies
	} // end for hits
} catch( ... ){
	report( "", "Exception ...", "", false, strm );
}
	strm << "====================================================================" << endl;
	strm << fun << " - Exit!" << endl;
	strm << "====================================================================" << endl;
}