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
void printHits( AccessorInstanceRef air, ostream& strm = cout );

//--------------------------------------------------------------------------------------------------------//
// void check_Transactions( ConnectionRef connection, AccessorInstanceRef air )
//--------------------------------------------------------------------------------------------------------//
void check_Transactions( ConnectionRef connection, AccessorInstanceRef air )
{
	static const char* const fun = "check_Transactions( ConnectionRef, AccessorInstanceRef)";

	//ostream& strm = cout;
	ofstream log ( "aspect.check_transaction.log" ); ostream& strm = log;
	strm << "====================================================================" << endl;
	strm << fun << " - Entry!" << endl;
	strm << "====================================================================" << endl << endl;

	strm << "Date: " << getDate().str().c_str() << "\tTime: " << getTime().str().c_str() << endl;

	ostringstream os;

	try {
		
		air = checkBBAccessorInstanceRef( connection, basar::db::aspect::FULL_CACHING );			

		//-----------------------------------------------------------------------------------------------------------------------//
		for( int i = 0; i < 2; ++i ){
			if( 0 == i ){
				strm	<< endl
						<< "----------------------------------------------------------------------------------------" << endl
						<< "without hits" << endl
						<< "----------------------------------------------------------------------------------------" << endl;
				try {
					air.getPropertyTable().clear();
					report( "", "air.getPropertyTable().clear();", "", true, strm );
				} catch( const basar::Exception& e ){
					report( "", "air.getPropertyTable().clear();", e.what(), false, strm );
				}
			} else {
				strm	<< endl
						<< "----------------------------------------------------------------------------------------" << endl
						<< "with hits" << endl
						<< "----------------------------------------------------------------------------------------" << endl;
				try {
					air.getPropertyTable().clear();
					report( "", "air.getPropertyTable().clear();", "", true, strm );
					basar::I18nString s("artikel_nr = 1057;" );
					basar::db::aspect::AccessorPropertyTable_YIterator yit = basar::db::aspect::Manager::getInstance().createAccessorYIterator(s);
					report( "", "air.setParameterList( \"artikel_nr=1057;\" );", "", true, strm );
					air.execute("ReadArticle", yit);
					report( "", "air.execute( \"ReadArticle\" );", "", true, strm );
				} catch( const basar::Exception& e ){
					report( "", "clear() + setParameterList(x) + execute(x);", e.what(), false, strm );
				}
			}
			strm	<< endl
					<< "----------------------------------------------------------------------------------------" << endl
					<< "1. a) beginTransaction  - b) beginTransaction:                             --> exception" << endl
					<< "2. a) commitTransaction - b) commitTransaction:                            --> exception" << endl
					<< "3. a) beginTransaction  - b) rollbackTransaction - c) rollbackTransaction: --> exception" << endl
					<< "4. a) nur commitTransaction:                                               --> exception" << endl
					<< "5. a) nur rollbackTransaction:                                             --> exception" << endl
					<< "----------------------------------------------------------------------------------------" << endl;

			strm << endl << "1. a) beginTransaction  - b) beginTransaction: --> exception" << endl;
			try {
				air.beginTransaction();
				report( "", "air.beginTransaction();", "", true, strm );
				air.beginTransaction();			
				report( "", "air.beginTransaction();", "", true, strm );
			} catch( const basar::Exception& e ){
				report( "", "air.beginTransaction();", e.what(), false, strm );
			}

			strm << endl << "2. a) commitTransaction - b) commitTransaction: --> exception" << endl;
			try {
				air.commitTransaction();
				report( "", "air.commitTransaction();", "", true, strm );
				air.commitTransaction();			
				report( "", "air.commitTransaction();", "", true, strm );
			} catch( const basar::Exception& e ){
				report( "", "air.commitTransaction();", e.what(), false, strm );
			}
			
			strm << endl << "3. a) beginTransaction  - b) rollbackTransaction - c) rollbackTransaction: --> exception" << endl;
			try {
				air.beginTransaction();
				report( "", "air.beginTransaction();", "", true, strm );
				air.rollbackTransaction();			
				report( "", "air.rollbackTransaction();", "", true, strm );
				air.rollbackTransaction();			
				report( "", "air.rollbackTransaction();", "", true, strm );
			} catch( const basar::Exception& e ){
				report( "", "air.commitTransaction();", e.what(), false, strm );
			}
		
			strm << endl << "4. a) nur commitTransaction: --> exception" << endl;
			try {
				air.commitTransaction();
				report( "", "air.commitTransaction();", "", true, strm );
			} catch( const basar::Exception& e ){
				report( "", "air.commitTransaction();", e.what(), false, strm );
			}

			strm << endl << "5. a) nur rollbackTransaction: --> exception" << endl;
			try {
				air.rollbackTransaction();
				report( "", "air.rollbackTransaction();", "", true, strm );
			} catch( const basar::Exception& e ){
				report( "", "air.rollbackTransaction();", e.what(), false, strm );
			}
		} // end for( i )

		//-----------------------------------------------------------------------------------------------------------------------//
		strm	<< endl
				<< "--------------------------------------------------------------------------------------------------"		<< endl
				<< "6. a) clear() - b) execute() --> no transaction causes no internal snapshot (imply debuggen)!"			<< endl
				<< "7. a) rollback               --> no snapshot, consequently no reset (imply debuggen) --> exception!"	<< endl
				<< "---------------------------------------------------------------------------------------------------"	<< endl;

		strm << "6. a) clear() - b) execute() --> no transaction causes no internal snapshot (imply debuggen)!"	<< endl;
		try {
			air.getPropertyTable().clear();
			report( "", "air.getPropertyTable().clear();", "", true, strm );
			basar::I18nString s("artikel_nr = 1057;" );
			basar::db::aspect::AccessorPropertyTable_YIterator yit = basar::db::aspect::Manager::getInstance().createAccessorYIterator(s);
			report( "", "air.setParameterList( \"artikel_nr=1057;\" );", "", true, strm );
			air.execute("ReadArticle", yit);
			report( "", "air.execute( \"ReadArticle\" );", "", true, strm );
		} catch( const basar::Exception& e ){
			report( "", "clear() + setParameterList(x) + execute(x);", e.what(), false, strm );
		}

		strm << "7. a) rollback --> no snapshot, consequently no reset (imply debuggen) --> exception!" << endl;
		try {
			air.rollbackTransaction();
			report( "", "air.rollbackTransaction();", "", true, strm );
		} catch( const basar::Exception& e ){
			report( "", "air.rollbackTransaction();", e.what(), false, strm );
		}

		//-----------------------------------------------------------------------------------------------------------------------//
		for( int i = 0; i < 2; ++i ){
			strm	<< endl
					<< "--------------------------------------------------------------------------------------------------"		<< endl
					<< "8. a) beginTransaction()!"																				<< endl
					<< "8. b) print hits!"																						<< endl	
					<< "8. c) execute()  --> internal snapshot (within a transaction)!"											<< endl	
					<< "8. d) manipulate one value!"																			<< endl	
					<< "8. e) print hits!"																						<< endl	
					<< ( (0 == i) ? "8. f) rollback()   --> reset to internal snapshot with manipulated value; "
									"snapshot and all accessors should be cleared afterwards(imply debuggen)!" : 
									"8. f) commitback() --> reset to internal snapshot with manipulated value; "
									"snapshot and all accessors should be cleared (imply debuggen)!")																<< endl	
					<< "8. g) print hits!"																						<< endl	
					<< "---------------------------------------------------------------------------------------------------"	<< endl;

			strm << endl << "8. a) beginTransaction()!" << endl;
			try {
				air.beginTransaction();
				report( "", "air.beginTransaction();", "", true, strm );
			} catch( const basar::Exception& e ){
				report( "", "air.beginTransaction();", e.what(), false, strm );
			}
			
			strm << endl << "8. b) print hits!" << endl;
			try {
				printHits( air, strm );
				report( "", "printHits( air );", "", true, strm );
			} catch( const basar::Exception& e ){
				report( "", "printHits( air );", e.what(), false, strm );
			}

			strm << endl << "8. c) execute()  --> internal snapshot (within a transaction)!" << endl;
			try {
				air.execute("ReadArticle");
				report( "", "air.execute( \"ReadArticle\" );", "", true, strm );
			} catch( const basar::Exception& e ){
				report( "", "air.execute( \"ReadArticle\" );", e.what(), false, strm );
			}

			strm << endl << "8. d) manipulate one value!" << endl;
			try {
				AccessorPropertyTable_YIterator yit = air.getPropertyTable().begin(); 
				yit.setInt32( "artikel_nr", 1234567 );
				report( "", "yit.setInt32( \"artikel_nr\", 1234567 );", "", true, strm );
			} catch( const basar::Exception& e ){
				report( "", "yit.setInt32( \"artikel_nr\", 1234567 );", e.what(), false, strm );	
			}
		
			strm << endl << "8. e) print hits!" << endl;
			try {
				printHits( air, strm );
				report( "", "printHits( air );", "", true, strm );
			} catch( const basar::Exception& e ){
				report( "", "printHits( air );", e.what(), false, strm );
			}

			if( 0 == i ){
				strm << endl << "8. f) rollback() --> reset to internal snapshot with manipulated value! "
								"(internal snapshot and all accessors should be cleared afterwards)" << endl;
				try {
					air.rollbackTransaction();
					report( "", "air.rollbackTransaction();", "", true, strm );
				} catch( const basar::Exception& e ){
					report( "", "air.rollbackTransaction();", e.what(), false, strm );
				}
			} else {
				strm << endl << "8. f) commit() --> no reset to internal snapshot "
								"(internal snapshot and all accessors should be cleared)!" << endl;
				try {
					air.commitTransaction();
					report( "", "air.commitTransaction();", "", true, strm );
				} catch( const basar::Exception& e ){
					report( "", "air.commitTransaction();", e.what(), false, strm );
				}
			}

			strm << endl << "8. g) print hits!" << endl;
			try {
				printHits( air, strm );
				report( "", "printHits( air );", "", true, strm );
			} catch( const basar::Exception& e ){
				report( "", "printHits( air );", e.what(), false, strm );
			}
		} // end for

		//-----------------------------------------------------------------------------------------------------------------------//
		strm	<< endl
				<< "--------------------------------------------------------------------------------------------------"		<< endl
				<< "9. a) create an external snapshot()!"																	<< endl
				<< "9. b) print hits before mainpulation!"																	<< endl	
				<< "9. c) manipulate one value!"																			<< endl	
				<< "9. d) print hits after mainpulation!"																	<< endl	
				<< "9. e) reset the external snapshot --> old value(s) has/have to be (re)set!"								<< endl	
				<< "9. f) print hits after reset of external snapshot!"																						<< endl	
				<< "---------------------------------------------------------------------------------------------------"	<< endl;

		strm << endl << "9. a) create an external snapshot()!" << endl;
		basar::db::aspect::PropertyTableSnapshotRef schnappi;
		try {
			schnappi = air.createSnapshot();
			report( "", "air.getPropertyTable().createSnapshot();", "", true, strm );
		} catch( const basar::Exception& e ){
			report( "", "air.getPropertyTable().createSnapshot();", e.what(), false, strm );
		}

		strm << endl << "9. b) print hits before mainpulation!" << endl;
		try {
			printHits( air, strm );
			report( "", "printHits( air );", "", true, strm );
		} catch( const basar::Exception& e ){
			report( "", "printHits( air );", e.what(), false, strm );
		}

		strm << endl << "9. c) manipulate one value!" << endl;
		try {
			AccessorPropertyTable_YIterator yit = air.getPropertyTable().begin(); 
			yit.setInt32( "artikel_nr", 7654321 );
			report( "", "yit.setInt32( \"artikel_nr\", 7654321 );", "", true, strm );
		} catch( const basar::Exception& e ){
			report( "", "yit.setInt32( \"artikel_nr\", 7654321 );", e.what(), false, strm );	
		}

		strm << endl << "9. d) print hits after manipulation!" << endl;
		try {
			printHits( air, strm );
			report( "", "printHits( air );", "", true, strm );
		} catch( const basar::Exception& e ){
			report( "", "printHits( air );", e.what(), false, strm );
		}

		strm << endl << "9. e) reset the external snapshot --> old value(s) has/have to be (re)set!" << endl;
		try {
			air.resetToSnapshot( schnappi );
			report( "", "air.resetToSnapshot( schnappi );", "", true, strm );
		} catch( const basar::Exception& e ){
			report( "", "air.resetToSnapshot( schnappi );", e.what(), false, strm );
		}
	
		strm << endl << "print hits after reset of external snapshot" << endl;
		try {
			printHits( air, strm );
			report( "", "printHits( air );", "", true, strm );
		} catch( const basar::Exception& e ){
			report( "", "printHits( air );", e.what(), false, strm );
		}

		//-----------------------------------------------------------------------------------------------------------------------//
		strm	<< endl
				<< "--------------------------------------------------------------------------------------------------"		<< endl
				<< "10. a) create an external snapshot() before a transaction!"												<< endl
				<< "10. b) beginTransaction!"																				<< endl
				<< "10. c) create an external snapshot() after a transaction!"												<< endl
				<< "10. d) reset the external snapshot --> exception!"														<< endl
				<< "10. e) commitTransaction()!"																			<< endl
				<< "10. f) reset the external snapshot --> exception!"														<< endl
				<< "10. g) beginTransaction()!"																				<< endl
				<< "10. h) rollback()!"																						<< endl
				<< "10. i) reset the external snapshot --> exception!"														<< endl
				<< "---------------------------------------------------------------------------------------------------"	<< endl;
	
		strm << endl << "10. a) create an external snapshot() before a transaction!" << endl;
		try {
			schnappi = air.createSnapshot();
			report( "", "air.getPropertyTable().createSnapshot();", "", true, strm );
		} catch( const basar::Exception& e ){
			report( "", "air.getPropertyTable().createSnapshot();", e.what(), false, strm );
		}
		
		strm << endl << "10. b) beginTransaction!" << endl;
		try {
			air.beginTransaction();
			report( "", "air.beginTransaction();", "", true, strm );
		} catch( const basar::Exception& e ){
			report( "", "air.beginTransaction();", e.what(), false, strm );
		}

		strm << endl << "10. c) create an external snapshot() after a transaction!" << endl;
		try {
			schnappi = air.createSnapshot();
			report( "", "air.getPropertyTable().createSnapshot();", "", true, strm );
		} catch( const basar::Exception& e ){
			report( "", "air.getPropertyTable().createSnapshot();", e.what(), false, strm );
		}

		strm << endl << "10. d) reset the external snapshot --> exception!" << endl;
		try {
			air.resetToSnapshot( schnappi );
			report( "", "air.resetToSnapshot( schnappi );", "", true, strm );
		} catch( const basar::Exception& e ){
			report( "", "air.resetToSnapshot( schnappi );", e.what(), false, strm );
		}
		
		strm << endl << "10. e) commitTransaction()!" << endl;
		try {
			air.commitTransaction();
			report( "", "air.commitTransaction();", "", true, strm );
		} catch( const basar::Exception& e ){
			report( "", "air.commitTransaction();", e.what(), false, strm );
		}

		strm << endl << "10. f) reset the external snapshot --> exception!" << endl;
		try {
			air.resetToSnapshot( schnappi );
			report( "", "air.resetToSnapshot( schnappi );", "", true, strm );
		} catch( const basar::Exception& e ){
			report( "", "air.resetToSnapshot( schnappi );", e.what(), false, strm );
		}

		strm << endl << "10. g) beginTransaction!" << endl;
		try {
			air.beginTransaction();
			report( "", "air.beginTransaction();", "", true, strm );
		} catch( const basar::Exception& e ){
			report( "", "air.beginTransaction();", e.what(), false, strm );
		}

		strm << endl << "10. h) rollback()!" << endl;
		try {
			air.rollbackTransaction();
			report( "", "air.rollbackTransaction();", "", true, strm );
		} catch( const basar::Exception& e ){
			report( "", "air.rollbackTransaction();", e.what(), false, strm );
		}

		strm << endl << "10. i) reset the external snapshot --> exception!" << endl;
		try {
			air.resetToSnapshot( schnappi );
			report( "", "air.resetToSnapshot( schnappi );", "", true, strm );
		} catch( const basar::Exception& e ){
			report( "", "air.resetToSnapshot( schnappi );", e.what(), false, strm );
		}

	} catch( const basar::Exception& e ){
		report( "", "", e.what(), false, strm );
	} catch( ... ){
		report( fun, "unexpected exception!;", "", false, strm );
	}

	strm << "====================================================================" << endl;
	strm << fun << " - Exit!" << endl;
	strm << "====================================================================" << endl;
}
//--------------------------------------------------------------------------------------------------------//
// void printHits( AccessorInstanceRef air )
//--------------------------------------------------------------------------------------------------------//
void printHits( AccessorInstanceRef air, ostream& strm  )
{
	static const char* fun = "printHits( AccessorInstanceRef)";
	strm << "= = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = " << endl;
	strm << fun << " - Entry!" << endl;
	strm << "= = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = " << endl;

	AccessorPropertyTable_YIterator yit = air.getPropertyTable().begin();
	for( ; yit != air.getPropertyTable().end(); ++yit ){
		//////////////////////
		// current row number
		//////////////////////
		try {
			strm << endl << "yit.getRowNumber():   " << yit.getRowNumber() << endl;		
			report( "", "yit.getRowNumber()", "", true, strm );
		} catch( const basar::Exception& e ){
			report( "", "yit.getRowNumber()", e.what(), false, strm );
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
		/////////////////
		// artikel_name
		/////////////////
		try {
			strm << "artikel_name: " << yit.getString(	"artikel_name"	) << endl;
			report( "", "yit.getString(\"artikel_name\");", "", true, strm );
		} catch( const basar::Exception& e ){
			report( "", "yit.getString(\"artikel_name\");", e.what(), false, strm );
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
		/////////////////
		// bestand
		/////////////////
		try {
			strm << "bestand:      " << yit.getInt32 (	"bestand"		) << endl;
			report( "", "yit.getInt32(\"bestand\");", "", true, strm );
		} catch( const basar::Exception& e ){
			report( "", "yit.getInt32(\"bestand\");", e.what(), false, strm );
		}
	}

	strm << "= = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = " << endl;
	strm << fun << " - Exit!" << endl;
	strm << "= = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = " << endl;
	
}