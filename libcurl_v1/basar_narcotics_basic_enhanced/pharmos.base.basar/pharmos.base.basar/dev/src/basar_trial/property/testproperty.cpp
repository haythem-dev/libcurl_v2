//////////////////////////////////////////////////////////////////////////////////
// testproperty.cpp : Definiert den Einstiegspunkt für die Konsolenanwendung.
//////////////////////////////////////////////////////////////////////////////////
// includes
//////////////////////////////////////////////////////////////////////////////////
#include <iostream>
#include <string>
#include <iomanip>
#include <libbasarproperty.h>
#include <libbasardbaspect.h>
//---------------------------------------------------//
// for test purposes only - not for public interface
//---------------------------------------------------//
#include "testmacro.h"
#include "propertydescriptionlist.h"
#include "propertytable.h"
#include "property_any.hpp"

#include <boost/any.hpp>

//---------------------------------------------------//

//---------------------------------------------------//
// using declaration
//---------------------------------------------------//
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
using basar::property::any;


using namespace std;
//---------------------------------------------------//

class TestBoostAny
{
public:
	void setInt( int i )
	{
		m_Any = i;
	}
	void setFloat( float f )
	{
		m_Any = f;	
	}
	void setString( std::string s )
	{
		m_Any = s;
	}
	
	int getInt()
	{
		int i = 0;
		try {
			i = boost::any_cast<int>(m_Any);
		} catch( const boost::bad_any_cast& r ){
			cout << r.what() << endl;
		}
		return i;
	}
	float getFloat()
	{
		float f = 0.0;
		try {
			f = boost::any_cast<float>(m_Any);
		} catch( const boost::bad_any_cast& r ){
			cout << r.what() << endl;
		}
		return f;
	}
	std::string getString()
	{
		std::string s = "";
		if( m_Any.empty() ){
			s = "";
		} else {
			try {
				s = boost::any_cast<std::string>(m_Any);
			} catch( const boost::bad_any_cast& r ){
				cout << r.what() << endl;
			}
		}
		return s;
	}

public:
	boost::any m_Any;
};

////////////////////////////////////////////////////////////////////////////////
// forward declaration
////////////////////////////////////////////////////////////////////////////////
//void checkPropertyType();
void checkPropertyStateSet();
//void checkSubsetPdl();
void checkPropertyDescriptionListRef();
void checkPropertyDescriptionList();
void checkPropertyTableRef();
void checkBoostSharedPtr();
//boost::shared_ptr<int> getSPtAsValue();
//boost::shared_ptr<int>& getSPtAsRef();
void testBoostAny();
void check_arePropertiesSet();
void check_isPropertyInState();
void check_arePropertiesSetAndAllInState();
void check_arePropertiesSetAndOneMustBeInState();
void check_XIterator();
void check_are1stPropertiesSetAnd2ndOneInState();
void measureTimeOfBombsOfInserts();
const std::ostringstream& getTime();

typedef vector<any> vecAny;
vecAny check_Basar_Any();

void checkErasePropertyTable();
////////////////////////////////////////////////////////////////////////////////
// int main(int argc, char* argv[])
////////////////////////////////////////////////////////////////////////////////
//boost::shared_ptr<int> g;
int main()
{
	__FUN__( main )
	DEBUG_ENTER( cout , fun )
	COUT( basar::property::getVersion() )

	checkErasePropertyTable();
	
	getchar();
	return 0;

	//measureTimeOfBombsOfInserts();

	{
		vecAny v = check_Basar_Any();
		
		cout << "-------------------------------------" << endl;
		cout << "Werte-Ausgabe (main):" << endl;
		cout << "-------------------------------------" << endl;
		vecAny::iterator it = v.begin();
		for( ; it != v.end(); ++it ){
			cout << "-----------------------------" << endl;
			cout << "Adresse von Any: "<< &(*it) << endl;
			cout << "Wert: "<< it->getInt32() << endl;
			cout << "Adresse von content (union): "<< &(it->content) << endl;
			cout << "Adresse von content.complex_val (placeholder): "<< &(it->content.complex_val) << endl;
			cout << "Inhalt von content.complex_val (placeholder): "<< (it->content.complex_val) << endl;
			cout << "Inhalt von content.int16_val: "<< &(it->content.int16_val) << endl;
			cout << "Inhalt von content.int32_val: "<< &(it->content.int32_val) << endl;
			cout << "-----------------------------" << endl;
		}
	}

	DEBUG_LEAVE( cout, fun )
	getchar();
    return 0;
}
//--------------------------------------------------------------------------------------------------------//
// void checkErasePropertyTable()
//--------------------------------------------------------------------------------------------------------//
void checkErasePropertyTable()
{
	__FUN__( checkErasePropertyTable )
	DEBUG_ENTER( cout , fun )

	PropertyDescriptionListRef pdlr( PropertyDescriptionList::create() );
	/* pdlr.construct( "int16 Spalte1;" "int16 Spalte2;" "int32 Spalte3;" "float32 Spalte4;" "float64 Spalte5;" 
					"decimal Spalte6;" "string Spalte7;" ); */

#define SPALTE_1 "Spalte1"
#define SPALTE_2 "Spalte2"
	pdlr.push_back( SPALTE_1, basar::INT16	);    
	pdlr.push_back( SPALTE_2, basar::STRING	);

	PropertyTableRef ptr1( PropertyTable::create(pdlr) );
	//for( int ins = 0; ins < 5; ++ins ){
	//	ptr1.insert( basar::FOR_INSERT );
	//}

	std::ostringstream os;
	int i = 0;
	PropertyTable_YIterator yit = ptr1.begin();
	for( ; !yit.isEnd(); ++yit, ++i ){
		yit.setInt16(  SPALTE_1, i );
		os.str(""); os << i << " " << "Spalte";
		yit.setString( SPALTE_2, os.str().c_str() );
	}

	yit = ptr1.begin();
	for( ; !yit.isEnd(); ++yit ){
		cout << yit.getInt16( SPALTE_1 ) << ", " << yit.getString( SPALTE_2 ).c_str() << ";" << endl;
	}

	try {
		cout << "ptr1.size(): >" << static_cast<int>( ptr1.size() ) << "<" << endl;
		cout << boolalpha << "ptr1.empty(): >" << ptr1.empty() << "<" << endl;
	} catch( const basar::Exception& e ){
		cout << e.what() << endl;
	//} catch( const std::execption& e ){
	//	cout << e.what() << endl;
	} catch( ...  ){
		cout << "unknown exception!" << endl;
	}

	try {
		yit = ptr1.begin();
		if( !yit.isEnd() ){	
			++yit;
		}
		while( !yit.isEnd() ){
			cout << "akt. yit (wird geloescht): " << yit.getInt16( SPALTE_1 ) << ", " << yit.getString( SPALTE_2 ).c_str() << ";" << endl;
			yit = ptr1.erase( yit );
			if( !yit.isEnd() ){
				cout << "zurueckgegebener yit (nach Loeschen): " << yit.getInt16( SPALTE_1 ) << ", " << yit.getString( SPALTE_2 ).c_str() << ";" << endl;
			}
		}
	} catch( const basar::Exception& e ){
		cout << e.what() << endl;
	//} catch( const std::execption& e ){
	//	cout << e.what() << endl;
	} catch( ...  ){
		cout << "unknown exception!" << endl;
	}

	DEBUG_LEAVE( cout, fun )
}
//--------------------------------------------------------------------------------------------------------//
// void check_Basar_Any()
//--------------------------------------------------------------------------------------------------------//
vecAny check_Basar_Any()
{
	static const char* const fun = "check_Basar_Any()";
	DEBUG_ENTER( cout , fun )

	vecAny v(3);
	v[0] = 100;
	v[1] = 200;
	v[2] = 300;
	
	cout << "-------------------------------------" << endl;
	cout << "Werte-Ausgabe nach Initialisierung:" << endl;
	cout << "-------------------------------------" << endl;
	vecAny::iterator it = v.begin();
	for( ; it != v.end(); ++it ){
		cout << "-----------------------------" << endl;
		cout << "Adresse von Any: "<< &(*it) << endl;
		cout << "Wert: "<< it->getInt32() << endl;
		cout << "Adresse von content (union): "<< &(it->content) << endl;
		cout << "Adresse von content.complex_val (placeholder): "<< &(it->content.complex_val) << endl;
		cout << "Inhalt von content.complex_val (placeholder): "<< (it->content.complex_val) << endl;
		cout << "Inhalt von content.int16_val: "<< &(it->content.int16_val) << endl;
		cout << "Inhalt von content.int32_val: "<< &(it->content.int32_val) << endl;
		cout << "-----------------------------" << endl;
	}

	cout << "-------------------------------------" << endl;
	cout << "Werte-Ausgabe nach erneuter Aenderung:" << endl;
	cout << "-------------------------------------" << endl;
	v[0] = 100100;
	v[1] = 200200;
	v[2] = 300300;
	it = v.begin();
	for( ; it != v.end(); ++it ){
		cout << "-----------------------------" << endl;
		cout << "Adresse von Any: "<< &(*it) << endl;
		cout << "Wert: "<< it->getInt32() << endl;
		cout << "Adresse von content (union): "<< &(it->content) << endl;
		cout << "Adresse von content.complex_val (placeholder): "<< &(it->content.complex_val) << endl;
		cout << "Inhalt von content.complex_val (placeholder): "<< (it->content.complex_val) << endl;
		cout << "Inhalt von content.int16_val: "<< &(it->content.int16_val) << endl;
		cout << "Inhalt von content.int32_val: "<< &(it->content.int32_val) << endl;
		cout << "-----------------------------" << endl;
	}
	DEBUG_LEAVE( cout, fun )
	return v;
}
//--------------------------------------------------------------------------------------------------------//
// const ostringstream& getTime()
//--------------------------------------------------------------------------------------------------------//
const std::ostringstream& getTime()
{
	struct tm *pTm;
	time_t tSec = time(NULL);
	pTm = localtime( &tSec );	

	static std::ostringstream os;
	os.str("");
	os	<< setw(2) << setfill('0') << pTm->tm_hour
		<< ":"
		<< setw(2) << setfill('0') << pTm->tm_min
		<< ":"
	    << setw(2) << setfill('0') << pTm->tm_sec;	
	return os;
}
//--------------------------------------------------------------------------------------------------------//
// void measureTimeOfBombsOfInserts(){
//--------------------------------------------------------------------------------------------------------//
void measureTimeOfBombsOfInserts()
{
	static const char* const fun = "measureTimeOfBombsOfInserts"; 
	//std::ofstream of();
	
	basar::property::PropertyDescriptionListRef pdlr( basar::property::PropertyDescriptionList::create() );
	pdlr.push_back( "spalte_int16",		basar::INT16	);    
	pdlr.push_back( "spalte_int32",		basar::INT32	);    
	pdlr.push_back( "spalte_float32",	basar::FLOAT32	);    
	pdlr.push_back( "spalte_float64",	basar::FLOAT64	);    
	pdlr.push_back( "spalte_string",	basar::STRING	);
	pdlr.push_back( "spalte_Decimal",	basar::DECIMAL	);
	pdlr.push_back( "spalte_DateTime",	basar::DATETIME	);

	PropertyTableRef ptr1( PropertyTable::create(pdlr) );


	basar::cmnutil::Decimal dc( 132.456 );
	basar::cmnutil::DateTime dt; 
	dt.setDate( 20061105);
	
	cout << fun << " - Start der INSERTS: " << getTime().str() << endl;
	basar::property::PropertyTable_YIterator yit; 
	for( int i = 0; i < 300000; ++i ){ 
		yit = ptr1.insert( basar::FOR_INSERT );
		yit.setInt16(		"spalte_int16",		16					);
		yit.setInt32(		"spalte_int32",		32					);
		yit.setFloat32(		"spalte_float32",	32.32F				);
		yit.setFloat64(		"spalte_float64",	64.64				);
		yit.setString(		"spalte_string",	"Bjoern Bischof"	);
		yit.setDecimal(		"spalte_Decimal",	dc					);
		yit.setDateTime(	"spalte_DateTime",	dt					);
	}

	//cout << fun << " - Göesse PT: " << ptr1.size() << endl;
	cout << fun << " - Ende der INSERTS: " << getTime().str() << endl;

	basar::Int16		a;
	basar::Int32		b;
	basar::Float32		c;
	basar::Float64		d;
	basar::VarString	e;
	basar::Decimal		f;
	basar::DateTime		g;

	cout << fun << " - Start der Werte-Abfrage: " << getTime().str() << endl;
	yit = ptr1.begin();
	for( ; yit != ptr1.end(); ++yit ){ 
		a = yit.getInt16(		"spalte_int16");
		b = yit.getInt32(		"spalte_int32");
		c = yit.getFloat32(		"spalte_float32");
		d = yit.getFloat64(		"spalte_float64");
		e = yit.getString(		"spalte_string");
		f = yit.getDecimal(		"spalte_Decimal");
		g = yit.getDateTime(	"spalte_DateTime");
	}
	cout << fun << " - Ende der Werte-Abfrage: " << getTime().str() << endl;
}
////////////////////////////////////////////////////////////////////////////////
// void check_XIterator()
////////////////////////////////////////////////////////////////////////////////
void check_XIterator()
{
	static const char* const fun = "check_XIterator()";
	cout << "============================================================="<< endl;
	cout << fun << ": Entry" << endl;
	cout << "============================================================="<< endl;

	PropertyDescriptionListRef pdlr( PropertyDescriptionList::create() );
    
	/* pdlr.construct( "int16 Spalte1;" "int16 Spalte2;" "int32 Spalte3;" "float32 Spalte4;" "float64 Spalte5;" 
					"decimal Spalte6;" "string Spalte7;" ); */

	pdlr.push_back( "Spalte1", basar::INT16		);    
	pdlr.push_back( "Spalte2", basar::UNKNOWN	);    
	pdlr.push_back( "Spalte3", basar::INT32		);
	pdlr.push_back( "Spalte4", basar::UNKNOWN	);
	pdlr.push_back( "Spalte5", basar::INT32		);
	pdlr.push_back( "Spalte6", basar::STRING	);

	PropertyTableRef ptr1( PropertyTable::create(pdlr) );
	PropertyTableRef ptr2( PropertyTable::create(pdlr) );

	ptr1.insert( basar::FOR_INSERT );
	ptr1.insert( basar::FOR_INSERT );
	ptr1.insert( basar::FOR_INSERT );

	cout << "ptr1.size(): >" << static_cast<int>( ptr1.size() ) << "<" << endl;
	cout << boolalpha << "ptr1.empty(): >" << ptr1.empty() << "<" << endl;
	
	PropertyTable_YIterator yit;

	try {
		yit = ptr1.begin();
		for( ; yit != ptr1.end(); ++yit ){
			yit.setInt16("Spalte1", 99 );
			cout << yit.getInt16("Spalte1") << endl;
		}
	} catch( const basar::Exception& e ){
		cout << e.what() << endl;
	}

	cout << endl 
		 << "-----------------------------------------------------------" << endl;
	cout << "for: yit = ptr1.erase( yit );" << endl;
	cout << "-----------------------------------------------------------" << endl;
	try {
		yit = ptr1.begin();
		for( ; yit != ptr1.end(); ++yit ){
			yit = ptr1.erase( yit );
			cout << "ptr1.size(): >" << static_cast<int>( ptr1.size() ) << "<" << endl;
			cout << boolalpha << "ptr1.empty(): >" << ptr1.empty() << "<" << endl;
		}
	} catch( const basar::Exception& e ){
		cout << e.what() << endl;
	}

	ptr1.insert( basar::FOR_INSERT );
	ptr1.insert( basar::FOR_INSERT );
	ptr1.insert( basar::FOR_INSERT );
	
	cout << endl 
		 << "-----------------------------------------------------------" << endl;
	cout << "ptr1.erase( ptr1.begin(), ptr1.end() );" << endl;
	cout << "-----------------------------------------------------------" << endl;
	try {
		PropertyTable_YIterator beg = ptr1.begin();
		PropertyTable_YIterator end = ptr1.end();
		ptr1.erase( beg, end ); 
	} catch( const basar::Exception& e ){
		cout << e.what() << endl;
	} 
	cout << "ptr1.size(): >" << static_cast<int>( ptr1.size() ) << "<" << endl;
	cout << boolalpha << "ptr1.empty(): >" << ptr1.empty() << "<" << endl;

	cout << "============================================================="<< endl;
	cout << fun << ": Exit" << endl;
	cout << "============================================================="<< endl;
}
////////////////////////////////////////////////////////////////////////////////
// void check_arePropertiesSetAndOneMustBeInState()
////////////////////////////////////////////////////////////////////////////////
void check_arePropertiesSetAndOneMustBeInState()
{
	__FUN__( check_arePropertiesSetAndOneMustBeInState() )
	DEBUG_ENTER( cout , fun )

	cout << "PropertyDescriptionListRef pdlr( PropertyDescriptionList::create() );" << endl;
	PropertyDescriptionListRef pdlr( PropertyDescriptionList::create() );
	PropertyStateSet pss;

	//-------------------------------------------------------------------------------------//
	// 1. Test
	//-------------------------------------------------------------------------------------//
	cout << "------------------------------------------------------------------------------" << endl;
	cout	<< "All parameterlist attributes exists in the yiterator." << endl 
			<< "State of the attributes of the yiterator are: 'SS_CLEAN'." << endl 
			<< "PropertyStateSet contains states 'SS_UNKNOWN' and 'SS_DELETE'" << endl 
			<< "Should return FALSE." << endl;
	cout << "------------------------------------------------------------------------------" << endl;
	//-------------------------------------------------------------------------------------//
	cout << "pdlr.push_backs();"<< endl;
	pdlr.push_back( "Spalte4", basar::INT16  );    
	pdlr.push_back( "Spalte3", basar::STRING );    
	pdlr.push_back( "Spalte2", basar::INT32	);
	
	cout << "pdlr.toStream();" << endl;
	pdlr.toStream();

	cout << "PropertyTableRef ptr( PropertyTable::create(pdlr) );"<< endl;
	PropertyTableRef ptr( PropertyTable::create(pdlr) );
	
	cout << "ptr.insert( basar::FOR_CLEAN );"<< endl;
	ptr.insert(basar::FOR_CLEAN);

	COUT( ptr.begin().getState().toString().c_str() )

	cout << "ptr.begin().setInt16( 'Spalte4', 16 );" << endl;
	ptr.begin().setInt16( "Spalte4", 16 );
	COUT( ptr.begin().begin().getState().toString().c_str() )

	cout << "ptr.begin().setString( \"Spalte3\", \"String\" );"<< endl;
	ptr.begin().setString( "Spalte3", "String" );
	cout << (ptr.begin().begin()++).getState().toString().c_str() << endl;

	cout << "ptr.begin().setInt32( \"Spalte2\", 32 );"<< endl;
	ptr.begin().setInt32( "Spalte2", 32 );
	cout << ((ptr.begin().begin()++)++).getState().toString().c_str() << endl;

	{
		cout << "pss.insert( basar::SS_DELETE );"<< endl;
		pss.insert( basar::SS_DELETE );
		cout << "pss.toString().c_str();" << endl;
		cout << pss.toString().c_str() << endl;

		cout << "ParameterList pl( \"Spalte4; Spalte3; Spalte2;\");" << endl;
		basar::cmnutil::ParameterList pl( "Spalte4; Spalte3; Spalte2;");
		
		cout << "pl.toStream();" << endl;
		pl.toStream();

		try {
			cout << std::boolalpha << "isPropertyInState( ptr.begin(), pl, pss ): "
				 << basar::property::arePropertiesSetAndOneMustBeInState( ptr.begin(), pl, pss ) << endl;
		} catch( const basar::Exception& e ){
			COUT( e.what() )
		}
	}

	//-------------------------------------------------------------------------------------//
	// 2. Test
	//-------------------------------------------------------------------------------------//
	cout << "------------------------------------------------------------------------------"<< endl;
	cout	<< "All parameterlist attributes exists in the yiterator." << endl
			<< "State of the attributes of the yiterator are: 'SS_CLEAN'." << endl
			<< "PropertyStateSet contains states 'SS_UNKNOWN', 'SS_CLEAN' and 'SS_DELETE'" << endl
			<< "Should return TRUE." << endl;
	cout << "------------------------------------------------------------------------------"<< endl;
	//-------------------------------------------------------------------------------------//
	cout << "pdlr.toStream();" << endl;
	pdlr.toStream();
	{
		cout << "pss.insert( static_cast<basar::SupportedStateEnum>(basar::SS_DELETE | basar::SS_CLEAN) );"<< endl;
		pss.insert( static_cast<basar::SupportedStateEnum>(basar::SS_DELETE | basar::SS_CLEAN) );
		cout << "pss.toString().c_str();" << endl;
		cout << pss.toString().c_str() << endl;

		cout << "ParameterList pl( \"Spalte4; Spalte3; Spalte2;\");" << endl;
		basar::cmnutil::ParameterList pl( "Spalte4; Spalte3; Spalte2;");
		
		cout << "pl.toStream();" << endl;
		pl.toStream();

		try {
			cout << std::boolalpha << "isPropertyInState( ptr.begin(), pl, pss ): "
				 << basar::property::arePropertiesSetAndOneMustBeInState( ptr.begin(), pl, pss ) << endl;
		} catch( const basar::Exception& e ){
			COUT( e.what() )
		}
	}
	

	//-------------------------------------------------------------------------------------//
	// 3. Test
	//-------------------------------------------------------------------------------------//
	cout << "------------------------------------------------------------------------------"<< endl;
	cout	<< "All parameterlist attributes exists in the yiterator." << endl
			<< "State of the attributes of the yiterator are: 'SS_CLEAN' and 'SS_UPDATE'." << endl
			<< "PropertyStateSet contains states 'SS_UNKNOWN', 'SS_CLEAN' and 'SS_DELETE'" << endl
			<< "Should return TRUE." << endl;
	cout << "------------------------------------------------------------------------------"<< endl;
	//-------------------------------------------------------------------------------------//
	cout << "pdlr.toStream();" << endl;
	pdlr.toStream();
	{
		cout << "pss.insert( static_cast<basar::SupportedStateEnum>(basar::SS_DELETE | basar::SS_CLEAN) );"<< endl;
		pss.insert( static_cast<basar::SupportedStateEnum>(basar::SS_DELETE | basar::SS_CLEAN) );
		cout << "pss.toString().c_str();" << endl;
		cout << pss.toString().c_str() << endl;

		cout << "ParameterList pl( \"Spalte4; Spalte3; Spalte2;\");" << endl;
		basar::cmnutil::ParameterList pl( "Spalte4; Spalte3; Spalte2;");
		
		cout << "pl.toStream();" << endl;
		pl.toStream();

		cout << "ptr.begin().setInt16( \"Spalte4\", 1616 );" << endl;
		ptr.begin().setInt16( "Spalte4", 1616 );
		cout << "ptr.begin().begin().getState().toString().c_str();" << endl;
		cout << ptr.begin().begin().getState().toString().c_str() << endl;

		try {
			cout << std::boolalpha << "isPropertyInState( ptr.begin(), pl, pss ): "
				 << basar::property::arePropertiesSetAndOneMustBeInState( ptr.begin(), pl, pss ) << endl;
		} catch( const basar::Exception& e ){
			COUT( e.what() )
		}
	}
	
	//-------------------------------------------------------------------------------------//
	// 4. Test
	//-------------------------------------------------------------------------------------//
	cout << "------------------------------------------------------------------------------"<< endl;	
	cout	<< "Not all parameterlist attributes exists in the yiterator." << endl
			<< "State of the attributes of the yiterator are: 'SS_CLEAN' and 'SS_UPDATE'." << endl
			<< "PropertyStateSet contains states 'SS_UNKNOWN', 'SS_CLEAN' and 'SS_DELETE'" << endl
			<< "Should return FALSE." << endl;
	cout << "------------------------------------------------------------------------------"<< endl;
	//-------------------------------------------------------------------------------------//
	cout << "pdlr.toStream();" << endl;
	pdlr.toStream();
	{
		//cout << "pss.insert( static_cast<basar::SupportedStateEnum>(basar::SS_DELETE | basar::SS_CLEAN) );"<< endl;
		cout << "PropertyStateSet psset;" << endl;
		PropertyStateSet psset;
		psset.insert( static_cast<basar::SupportedStateEnum>(basar::SS_DELETE | basar::SS_CLEAN) );
		cout << "psset.toString().c_str();" << endl;
		cout << psset.toString().c_str() << endl;
	
		cout << "ParameterList pl( \"Spalte4; Spalte3;\");" << endl;
		basar::cmnutil::ParameterList pl( "Spalte4; Spalte5;");
		
		cout << "pl.toStream();" << endl;
		pl.toStream();
		try {
			cout << std::boolalpha << "basar::property::arePropertiesSet(ptr.begin(), pl): "
				 << basar::property::arePropertiesSetAndOneMustBeInState(ptr.begin(), pl, psset) << endl;
		} catch( const basar::Exception& e ){
			COUT( e.what() )
		}
	}

	//-------------------------------------------------------------------------------------//
	// 5. Test
	//-------------------------------------------------------------------------------------//
	cout << "------------------------------------------------------------------------------"<< endl;
	cout	<< "All parameterlist attributes exists in the yiterator." << endl
			<< "States of the attributes of the yiterator are: 'SS_UPDATE' and 'SS_CLEAN'." << endl
			<< "PropertyStateSet contains only the state 'SS_UPDATE'"  << endl
			<< "Should return TRUE." << endl;
	cout << "------------------------------------------------------------------------------"<< endl;
	//-------------------------------------------------------------------------------------//
	cout << "pdlr.toStream();" << endl;
	pdlr.toStream();

	COUT( ptr.begin().getState().toString().c_str() )
	COUT( ptr.begin().begin().getState().toString().c_str() )
	COUT( (ptr.begin().begin()++).getState().toString().c_str() )

	{
		PropertyStateSet psset;
		psset.insert( basar::SS_UPDATE );
		COUT( psset.toString().c_str() )

		cout << "ParameterList pl( \"Spalte4; Spalte3; Spalte2;\");" << endl;
		basar::cmnutil::ParameterList pl( "Spalte4; Spalte3; Spalte2;");
		cout << "pl.toStream();" << endl;
		pl.toStream();

		try {
			cout << std::boolalpha << "isPropertyInState( ptr.begin(), pl, psset ): "
				 << basar::property::arePropertiesSetAndOneMustBeInState( ptr.begin(), pl, psset ) << endl;
		} catch( const basar::Exception& e ){
			COUT( e.what() )
		}
	}

	//-------------------------------------------------------------------------------------//
	// 6. Test
	//-------------------------------------------------------------------------------------//
	cout << "------------------------------------------------------------------------------"<< endl;
	cout	<< "Not All parameterlist attributes exist in the yiterator." << endl
			<< "States of the attributes of the yiterator are: 'SS_UPDATE' and 'SS_CLEAN'." << endl
			<< "PropertyStateSet contains only the state 'SS_UPDATE'" << endl	
			<< "Should return FALSE." << endl;
	cout << "------------------------------------------------------------------------------"<< endl;
	//-------------------------------------------------------------------------------------//
	cout << "pdlr.toStream();" << endl;
	pdlr.toStream();

	COUT( ptr.begin().getState().toString().c_str() )
	COUT( ptr.begin().begin().getState().toString().c_str() )
	COUT( (ptr.begin().begin()++).getState().toString().c_str() )

	{
		PropertyStateSet psset;
		psset.insert( basar::SS_UPDATE );
		COUT( psset.toString().c_str() )

		cout << "ParameterList pl( \"Spalte4; Spalte3; Spalte2;\");" << endl;
		basar::cmnutil::ParameterList pl( "Spalte5; Spalte3; Spalte2;");
		cout << "pl.toStream();" << endl;
		pl.toStream();

		try {
			cout << std::boolalpha << "isPropertyInState( ptr.begin(), pl, psset ): "
				 << basar::property::arePropertiesSetAndOneMustBeInState( ptr.begin(), pl, psset ) << endl;
		} catch( const basar::Exception& e ){
			COUT( e.what() )
		}
	}

	//--------------------------------------------------------------------------------------------------------------//
	// 7. Test: 
	//--------------------------------------------------------------------------------------------------------------//
	cout << "------------------------------------------------------------------------------"<< endl;
	cout	<< "Parameter List has valid values, state is not Unset but there are no states in PropertyStateSet" << endl;
	cout << "------------------------------------------------------------------------------"<< endl;
	{
		cout << "pdlr.toStream();" << endl;
		pdlr.toStream();

		COUT( ptr.begin().begin().getState().toString().c_str() )
		COUT( (ptr.begin().begin()++).getState().toString().c_str() )
		COUT( (ptr.begin().begin()++)++.getState().toString().c_str() )

		cout << "ParameterList pl( \"Spalte4; Spalte3; Spalte2;\");" << endl;
		basar::cmnutil::ParameterList pl( "Spalte4; Spalte3; Spalte2;");
		cout << "pl.toStream();" << endl;
		pl.toStream();

		PropertyStateSet pss;
		COUT( pss.toString().c_str() )

		try {
			cout << std::boolalpha << "arePropertiesSetAndOneMustBeInState( ptr.begin(), pl, pss ): "
				 << basar::property::arePropertiesSetAndOneMustBeInState( ptr.begin(), pl, pss ) << endl;
		} catch( const basar::Exception& e ){
			COUT( e.what() )
		}

	}

	//--------------------------------------------------------------------------------------------------------------//
	// 8. Test
	//--------------------------------------------------------------------------------------------------------------//
	cout << "------------------------------------------------------------------------------"<< endl;
	cout	<< "y-Iterator is not set or at end" << endl;
	cout << "------------------------------------------------------------------------------"<< endl;
	{
		PropertyStateSet pss;
		pss.insert(basar::SS_UPDATE);

		cout << "ParameterList pl( \"Spalte4; Spalte3; Spalte2;\");" << endl;
		basar::cmnutil::ParameterList pl( "Spalte4; Spalte3; Spalte2;");
		cout << "pl.toStream();" << endl;
		pl.toStream();
		PropertyTable_YIterator yit;


		try {
			cout << std::boolalpha << "arePropertiesSetAndOneMustBeInState( ptr.end(), pl, pss ): "
				 << basar::property::arePropertiesSetAndOneMustBeInState( ptr.end(), pl, pss ) << endl;
		} catch( const basar::Exception& e ){
			COUT( e.what() )
		}

		try {
			cout << std::boolalpha << "arePropertiesSetAndOneMustBeInState( yit, pl, pss ): "
				 << basar::property::arePropertiesSetAndOneMustBeInState( yit, pl, pss ) << endl;
		} catch( .../*const basar::Exception& e*/ ){
			COUT( "Fehler"/*e.what()*/ )
		}
	}


	DEBUG_LEAVE( cout, fun )
}
////////////////////////////////////////////////////////////////////////////////
// void check_arePropertiesSetAndAllInState()
////////////////////////////////////////////////////////////////////////////////
void check_arePropertiesSetAndAllInState()
{
	__FUN__( check_arePropertiesSetAndAllInState() )
	DEBUG_ENTER( cout , fun )

	cout << "PropertyDescriptionListRef pdlr( PropertyDescriptionList::create() );" << endl;
	PropertyDescriptionListRef pdlr( PropertyDescriptionList::create() );
	PropertyStateSet pss;

	//-------------------------------------------------------------------------------------//
	cout << "------------------------------------------------------------------------------"<< endl;
	cout << "check all parameterlist attributes if they exist in the yiterator and "
			"every yiterator attribute has not the state SS_UNSET AND "
			"every attribute found has to be in the state 'SS_DELETE' or 'SS_UNKNOWN' "
			"! --> should return false;" << endl;
	cout << "------------------------------------------------------------------------------"<< endl;
	//-------------------------------------------------------------------------------------//
	cout << "pdlr.push_backs();"<< endl;
	pdlr.push_back( "Spalte4", basar::INT16  );    
	pdlr.push_back( "Spalte3", basar::STRING );    
	pdlr.push_back( "Spalte2", basar::INT32	);
	
	cout << "pdlr.toStream();" << endl;
	pdlr.toStream();

	cout << "PropertyTableRef ptr( PropertyTable::create(pdlr) );"<< endl;
	PropertyTableRef ptr( PropertyTable::create(pdlr) );
	
	cout << "ptr.insert( basar::FOR_CLEAN );"<< endl;
	ptr.insert(basar::FOR_CLEAN);

	COUT( ptr.begin().getState().toString().c_str() )

	cout << "ptr.begin().setInt16( 'Spalte4', 16 );" << endl;
	ptr.begin().setInt16( "Spalte4", 16 );
	COUT( ptr.begin().begin().getState().toString().c_str() )

	cout << "ptr.begin().setString( \"Spalte3\", \"String\" );"<< endl;
	ptr.begin().setString( "Spalte3", "String" );
	cout << (ptr.begin().begin()++).getState().toString().c_str() << endl;

	cout << "ptr.begin().setInt32( \"Spalte2\", 32 );"<< endl;
	ptr.begin().setInt32( "Spalte2", 32 );
	cout << ((ptr.begin().begin()++)++).getState().toString().c_str() << endl;

	{
		cout << "pss.insert( basar::SS_DELETE );"<< endl;
		pss.insert( basar::SS_DELETE );
		cout << "pss.toString().c_str();" << endl;
		cout << pss.toString().c_str() << endl;

		cout << "ParameterList pl( \"Spalte4; Spalte3; Spalte2;\");" << endl;
		basar::cmnutil::ParameterList pl( "Spalte4; Spalte3; Spalte2;");
		
		cout << "pl.toStream();" << endl;
		pl.toStream();

		try {
			cout << std::boolalpha << "isPropertyInState( ptr.begin(), pl, pss ): "
				 << basar::property::arePropertiesSetAndAllInState( ptr.begin(), pl, pss ) << endl;
		} catch( const basar::Exception& e ){
			COUT( e.what() )
		}
	}

	//-------------------------------------------------------------------------------------//
	cout << "------------------------------------------------------------------------------"<< endl;
	cout << "check all parameterlist attributes if they exist in the yiterator and "
			"every yiterator attribute has not the state SS_UNSET AND "
			"every attribute found has to be in the state 'SS_DELETE' or 'SS_CLEAN' or 'SS_UNKNOWN' "
			"! --> should return true;" << endl;
	cout << "------------------------------------------------------------------------------"<< endl;
	//-------------------------------------------------------------------------------------//
	cout << "pdlr.toStream();" << endl;
	pdlr.toStream();
	{
		cout << "pss.insert( static_cast<basar::SupportedStateEnum>(basar::SS_DELETE | basar::SS_CLEAN) );"<< endl;
		pss.insert( static_cast<basar::SupportedStateEnum>(basar::SS_DELETE | basar::SS_CLEAN) );
		cout << "pss.toString().c_str();" << endl;
		cout << pss.toString().c_str() << endl;

		cout << "ParameterList pl( \"Spalte4; Spalte3; Spalte2;\");" << endl;
		basar::cmnutil::ParameterList pl( "Spalte4; Spalte3; Spalte2;");
		
		cout << "pl.toStream();" << endl;
		pl.toStream();

		try {
			cout << std::boolalpha << "isPropertyInState( ptr.begin(), pl, pss ): "
				 << basar::property::arePropertiesSetAndAllInState( ptr.begin(), pl, pss ) << endl;
		} catch( const basar::Exception& e ){
			COUT( e.what() )
		}
	}
	

	//-------------------------------------------------------------------------------------//
	cout << "------------------------------------------------------------------------------"<< endl;
	cout << "check all parameterlist attributes if they exist in the yiterator and "
			"every yiterator attribute has not the state SS_UNSET AND "
			"every attribute found has to be in the state 'SS_DELETE' or 'SS_CLEAN' or 'SS_UNKNOWN' "
			"! --> should return false;" << endl;
	cout << "------------------------------------------------------------------------------"<< endl;
	//-------------------------------------------------------------------------------------//
	cout << "pdlr.toStream();" << endl;
	pdlr.toStream();
	{
		cout << "pss.insert( static_cast<basar::SupportedStateEnum>(basar::SS_DELETE | basar::SS_CLEAN) );"<< endl;
		pss.insert( static_cast<basar::SupportedStateEnum>(basar::SS_DELETE | basar::SS_CLEAN) );
		cout << "pss.toString().c_str();" << endl;
		cout << pss.toString().c_str() << endl;

		cout << "ParameterList pl( \"Spalte4; Spalte3; Spalte2;\");" << endl;
		basar::cmnutil::ParameterList pl( "Spalte4; Spalte3; Spalte2;");
		
		cout << "pl.toStream();" << endl;
		pl.toStream();

		cout << "ptr.begin().setInt16( \"Spalte4\", 1616 );" << endl;
		ptr.begin().setInt16( "Spalte4", 1616 );
		cout << "ptr.begin().begin().getState().toString().c_str();" << endl;
		cout << ptr.begin().begin().getState().toString().c_str() << endl;

		try {
			cout << std::boolalpha << "isPropertyInState( ptr.begin(), pl, pss ): "
				 << basar::property::arePropertiesSetAndAllInState( ptr.begin(), pl, pss ) << endl;
		} catch( const basar::Exception& e ){
			COUT( e.what() )
		}
	}
	
	//-------------------------------------------------------------------------------------//
	cout << "------------------------------------------------------------------------------"<< endl;	
	cout << "one parameterlist attribute is missed --> should return false;" << endl;
	cout << "------------------------------------------------------------------------------"<< endl;
	//-------------------------------------------------------------------------------------//
	cout << "pdlr.toStream();" << endl;
	pdlr.toStream();
	{
		//cout << "pss.insert( static_cast<basar::SupportedStateEnum>(basar::SS_DELETE | basar::SS_CLEAN) );"<< endl;
		cout << "PropertyStateSet psset;" << endl;
		PropertyStateSet psset;
		//pss.insert( static_cast<basar::SupportedStateEnum>(basar::SS_DELETE | basar::SS_CLEAN) );
	
		cout << "ParameterList pl( \"Spalte4; Spalte3;\");" << endl;
		basar::cmnutil::ParameterList pl( "Spalte4; Spalte5;");
		
		cout << "pl.toStream();" << endl;
		pl.toStream();
		try {
			cout << std::boolalpha << "basar::property::arePropertiesSet(ptr.begin(), pl): "
				 << basar::property::arePropertiesSetAndAllInState(ptr.begin(), pl, psset) << endl;
		} catch( const basar::Exception& e ){
			COUT( e.what() )
		}
	}

	//-------------------------------------------------------------------------------------//
	cout << "------------------------------------------------------------------------------"<< endl;	
	cout << "y-Iterator is not set or is at end() --> should return false;" << endl;
	cout << "------------------------------------------------------------------------------"<< endl;
	//-------------------------------------------------------------------------------------//

	{
		cout << "ParameterList pl( \"Spalte4; Spalte3;\");" << endl;
		basar::cmnutil::ParameterList pl( "Spalte4; Spalte3;");

		PropertyStateSet psset;
		psset.insert( basar::SS_CLEAN );
		psset.insert( basar::SS_UPDATE );
		cout << "contents PropertyStateSet" << endl;
		cout << psset.toString().c_str() << endl;

		try {
			cout << std::boolalpha << "basar::property::arePropertiesSetAndAllInState(ptr.begin(), pl, psset): "
			<< basar::property::arePropertiesSetAndAllInState(ptr.end(), pl, psset) << endl;
			} catch( const basar::Exception& e ){
				COUT( e.what() )
			}

		PropertyTable_YIterator yit;

		try {
			cout << std::boolalpha << "basar::property::arePropertiesSetAndAllInState(ptr.begin(), pl, psset): "
			<< basar::property::arePropertiesSetAndAllInState(yit, pl, psset) << endl;
			} catch( .../*const basar::Exception& e*/ ){
				COUT( "Fehler"/*e.what()*/ )
			}
	}

	//-------------------------------------------------------------------------------------//
	cout << "------------------------------------------------------------------------------"<< endl;	
	cout << "PropertyStateSet is not set --> should return false;" << endl;
	cout << "------------------------------------------------------------------------------"<< endl;
	//-------------------------------------------------------------------------------------//
	{
		cout << "ParameterList pl( \"Spalte4; Spalte3;\");" << endl;
		basar::cmnutil::ParameterList pl( "Spalte4; Spalte3;");

		PropertyStateSet set_onlyUnknown;
		cout << "contents PropertyStateSet" << endl;
		cout << set_onlyUnknown.toString().c_str() << endl;

		ptr.clear();
		ptr.insert(basar::FOR_CLEAN);
		ptr.begin().setInt16("Spalte4", 16);
		ptr.begin().setString("Spalte3", "Test");
		ptr.begin().setInt32("Spalte2", 32);

		COUT( ptr.begin().begin().getState().toString().c_str() )
		COUT( ptr.begin().begin()++.getState().toString().c_str() )
		COUT( (ptr.begin().begin()++)++.getState().toString().c_str() )

		try {
			cout << std::boolalpha << "basar::property::arePropertiesSetAndAllInState(ptr.begin(), pl, set_onlyUnknown): "
			<< basar::property::arePropertiesSetAndAllInState(ptr.begin(), pl, set_onlyUnknown) << endl;
			} catch( const basar::Exception& e ){
				COUT( e.what() )
			}
	}

	//-------------------------------------------------------------------------------------//
	cout << "------------------------------------------------------------------------------"<< endl;	
	cout << "Parameter List contains only values that are not in Property Description List "
			"should return false;" << endl;
	cout << "------------------------------------------------------------------------------"<< endl;
	//-------------------------------------------------------------------------------------//
	{
		cout << "ParameterList pl( \"Spalte7; Spalte8;\");" << endl;
		basar::cmnutil::ParameterList pl( "Spalte7; Spalte8;");

		PropertyStateSet set;
		set.insert(basar::SS_CLEAN);
		set.insert(basar::SS_UPDATE);
		cout << "contents PropertyStateSet" << endl;
		cout << set.toString().c_str() << endl;

		try {
			cout << std::boolalpha << "basar::property::arePropertiesSetAndAllInState(ptr.begin(), pl, set_onlyUnknown): "
			<< basar::property::arePropertiesSetAndAllInState(ptr.begin(), pl, set) << endl;
			} catch( const basar::Exception& e ){
				COUT( e.what() )
			}
	}
	DEBUG_LEAVE( cout, fun )
}
////////////////////////////////////////////////////////////////////////////////
// void check_are1stPropertiesSetAnd2ndOneInState()
////////////////////////////////////////////////////////////////////////////////
void check_are1stPropertiesSetAnd2ndOneInState()
{
	__FUN__( check_arePropertiesSetAndAllInState() )
	DEBUG_ENTER( cout , fun )

	//-------------------------------------------------------------------------------------//
	cout << "------------------------------------------------------------------------------"<< endl;	
	cout << "Check if all properties are NOT in state SS_UNSET, all Parameters from ParameterList 1 are in PropertyTable, "
			"the Parameter from ParameterList 2 is in PropertyTable and the column in the ParameterList 2 has the "
			"demanded state "
			"should return true " << endl;
	cout << "------------------------------------------------------------------------------"<< endl;
	//-------------------------------------------------------------------------------------//

	cout << "PropertyDescriptionListRef pdl( PropertyDescriptionList::create() );" << endl;
	PropertyDescriptionListRef pdlr( PropertyDescriptionList::create() );
	PropertyStateSet pset;
	
	cout << "pdlr.push_backs();"<< endl;
	pdlr.push_back("Spalte4", basar::INT16);
	pdlr.push_back("Spalte3", basar::STRING);
	pdlr.push_back("Spalte2", basar::INT32);

	cout << "pdlr.toStream();" << endl;
	pdlr.toStream();

	//Creating and filling first PropertyTable
	cout << "PropertyTableRef pt1( PropertyTable::create(pdlr) );"<< endl;
	PropertyTableRef pt1( PropertyTable::create(pdlr) );
	
	cout << "pt1.insert( basar::FOR_CLEAN );"<< endl;
	pt1.insert( basar::FOR_CLEAN );

	cout << pt1.begin().getState().toString().c_str() << endl;

	PropertyTable_XIterator xit = pt1.begin().begin();
	cout << "pt1.begin().setInt16( 'Spalte4', 16 );" << endl;
	pt1.begin().setInt16( "Spalte4", 16 );
	cout << xit++.getState().toString().c_str() << endl;

	cout << "pt1.begin().setString( 'Spalte3', 'Test' );" << endl;
	pt1.begin().setString("Spalte3", "Test");
	cout << xit++.getState().toString().c_str() << endl;

	cout << "pt1.begin().setInt32( 'Spalte2', 32 );" << endl;
	pt1.begin().setInt32("Spalte2", 32);
	cout << xit++.getState().toString().c_str() << endl;


	//Creating two ParameterLists
	cout << "ParameterList pl1( \"Spalte4; Spalte3; Spalte2;\");" << endl;
	basar::cmnutil::ParameterList pl1( "Spalte4; Spalte3; Spalte2;");
	cout << "pl.toStream();" << endl;
	pl1.toStream();

	cout << "ParameterList pl2('Spalte4');" << endl;
	basar::cmnutil::ParameterList pl2( "Spalte4;");
	cout << "pl2.toStream();" << endl;
	pl2.toStream();

	//Creating PropertyStateSet
	PropertyStateSet set;
	set.insert( basar::SS_CLEAN );
	cout << "contents PropertyStateSet" << endl;
	cout << set.toString().c_str() << endl;

	try {
			cout << std::boolalpha << "basar::property::are1stPropertiesSetAnd2ndOneInState(pt1.begin(), pl1, pl2, set): "
			<< basar::property::are1stPropertiesSetAnd2ndOneInState(pt1.begin(), pl1, pl2, set) << endl;
		} catch( const basar::Exception& e ){
			COUT( e.what() )
		}

	//-------------------------------------------------------------------------------------//
	cout << "------------------------------------------------------------------------------"<< endl;	
	cout << "yIterator is at end() "
			"should throw an exception" << endl;
	cout << "------------------------------------------------------------------------------"<< endl;
	//-------------------------------------------------------------------------------------//
	try {
		cout << std::boolalpha << "basar::property::are1stPropertiesSetAnd2ndOneInState(pt1.begin(), pl1, pl2, set): "
		<< basar::property::are1stPropertiesSetAnd2ndOneInState(pt1.end(), pl1, pl2, set) << endl;
		} catch( const basar::Exception& e ){
			COUT( e.what() )
		}


	//-------------------------------------------------------------------------------------//
	cout << "------------------------------------------------------------------------------"<< endl;	
	cout << "yIterator is not set "
			"should throw an exception ==> Issue Report" << endl;
	cout << "------------------------------------------------------------------------------"<< endl;
	//-------------------------------------------------------------------------//
	//keine Exception: steht in IssueReport
	PropertyTable_YIterator yit;
	try {
		cout << std::boolalpha << "basar::property::are1stPropertiesSetAnd2ndOneInState(yit, pl1, pl2, set): "
		<< basar::property::are1stPropertiesSetAnd2ndOneInState(yit, pl1, pl2, set) << endl;
		} catch( .../*const basarstd::exception& e*/ ){
			COUT( "Fehler"/*e.what()*/ )
		}

	//-------------------------------------------------------------------------------------//
	cout << "------------------------------------------------------------------------------"<< endl;	
	cout << "ParameterList has no values from property description list "
			"should return false" << endl;
	cout << "------------------------------------------------------------------------------"<< endl;
	//-------------------------------------------------------------------------//

	cout << "ParameterList pl_notvalid('Spalte8');" << endl;
	basar::cmnutil::ParameterList pl_notvalid( "Spalte8;");
	cout << "pl_notvalid.toStream();" << endl;
	pl_notvalid.toStream();
	
	try {
		cout << std::boolalpha << "basar::property::are1stPropertiesSetAnd2ndOneInState(pt1.begin(), pl_notvalid, pl2, set): "
		<< basar::property::are1stPropertiesSetAnd2ndOneInState(pt1.begin(), pl_notvalid, pl2, set) << endl;
		} catch( const basar::Exception& e ){
			COUT( e.what() )
		}



	cout << "ParameterList pl_notvalid2('Spalte7');" << endl;
	basar::cmnutil::ParameterList pl_notvalid2( "Spalte7;");
	cout << "pl_notvalid2.toStream();" << endl;
	pl_notvalid2.toStream();
	
	try {
		cout << std::boolalpha << "basar::property::are1stPropertiesSetAnd2ndOneInState(pt1.begin(), pl1, pl_notvalid2, set): "
		<< basar::property::are1stPropertiesSetAnd2ndOneInState(pt1.begin(), pl1, pl_notvalid2, set) << endl;
		} catch( const basar::Exception& e ){
			COUT( e.what() )
		}


	//-------------------------------------------------------------------------------------//
	cout << "------------------------------------------------------------------------------"<< endl;	
	cout << "ParameterLists have no values from property description list "
			"so both checks in the method return false "
			"should return false" << endl;
	cout << "------------------------------------------------------------------------------"<< endl;
	//-------------------------------------------------------------------------//
	
	try {
		cout << std::boolalpha << "basar::property::are1stPropertiesSetAnd2ndOneInState(pt1.begin(), pl_notvalid, pl_notvalid2, set): "
		<< basar::property::are1stPropertiesSetAnd2ndOneInState(pt1.begin(), pl_notvalid, pl_notvalid2, set) << endl;
		} catch( const basar::Exception& e ){
			COUT( e.what() )
		}
}


////////////////////////////////////////////////////////////////////////////////
// void check_isPropertyInState()
////////////////////////////////////////////////////////////////////////////////
void check_isPropertyInState()
{
	__FUN__( void check_isPropertyInState() )
	DEBUG_ENTER( cout , fun )

	cout << "PropertyDescriptionListRef pdlr( PropertyDescriptionList::create() );" << endl;
	PropertyDescriptionListRef pdlr( PropertyDescriptionList::create() );
	PropertyStateSet pss;

	//-------------------------------------------------------------------------------------//
	cout << "------------------------------------------------------------------------------"<< endl;
	cout << "check all parameterlist attributes if they exist in the yiterator and "
			"every yiterator attribute has not the state SS_UNSET AND "
			"every attribute found has to be in the state 'SS_DELETE' or 'SS_UNKNOWN' "
			"! --> should return false;" << endl;
	cout << "------------------------------------------------------------------------------"<< endl;
	//-------------------------------------------------------------------------------------//
	cout << "pdlr.push_backs();"<< endl;
	pdlr.push_back( "Spalte4", basar::INT16  );    
	pdlr.push_back( "Spalte3", basar::STRING );    
	pdlr.push_back( "Spalte2", basar::INT32	);
	
	cout << "pdlr.toStream();" << endl;
	pdlr.toStream();

	cout << "PropertyTableRef ptr( PropertyTable::create(pdlr) );"<< endl;
	PropertyTableRef ptr( PropertyTable::create(pdlr) );
	
	cout << "ptr.insert( basar::FOR_CLEAN );"<< endl;
	ptr.insert( basar::FOR_CLEAN );

	cout << ptr.begin().getState().toString().c_str() << endl;

	cout << "ptr.begin().setInt16( 'Spalte4', 16 );" << endl;
	ptr.begin().setInt16( "Spalte4", 16 );
	cout << ptr.begin().begin().getState().toString().c_str() << endl;

	cout << "ptr.begin().setString( \"Spalte3\", \"String\" );"<< endl;
	ptr.begin().setString( "Spalte3", "String" );
	cout << (ptr.begin().begin()++).getState().toString().c_str() << endl;

	cout << "ptr.begin().setInt32( \"Spalte2\", 32 );"<< endl;
	ptr.begin().setInt32( "Spalte2", 32 );
	cout << ((ptr.begin().begin()++)++).getState().toString().c_str() << endl;

	{
		cout << "pss.insert( basar::SS_DELETE );"<< endl;
		pss.insert( basar::SS_DELETE );
		cout << "pss.toString().c_str();" << endl;
		cout << pss.toString().c_str() << endl;

		cout << "ParameterList pl( \"Spalte4; Spalte3; Spalte2;\");" << endl;
		basar::cmnutil::ParameterList pl( "Spalte4; Spalte3; Spalte2;");
		
		cout << "pl.toStream();" << endl;
		pl.toStream();

		//try {
		//	cout << std::boolalpha << "isPropertyInState( ptr.begin(), pl, pss ): "
		//		 << basar::property::isPropertyInState( ptr.begin(), pl, pss ) << endl;
		//} catch( const basar::Exception& e ){
		//	COUT( e.what() )
		//}
	}

	//-------------------------------------------------------------------------------------//
	cout << "------------------------------------------------------------------------------"<< endl;
	cout << "check all parameterlist attributes if they exist in the yiterator and "
			"every yiterator attribute has not the state SS_UNSET AND "
			"every attribute found has to be in the state 'SS_DELETE' or 'SS_CLEAN' or 'SS_UNKNOWN' "
			"! --> should return true;" << endl;
	cout << "------------------------------------------------------------------------------"<< endl;
	//-------------------------------------------------------------------------------------//
	cout << "pdlr.toStream();" << endl;
	pdlr.toStream();
	{
		cout << "pss.insert( static_cast<basar::SupportedStateEnum>(basar::SS_DELETE | basar::SS_CLEAN) );"<< endl;
		pss.insert( static_cast<basar::SupportedStateEnum>(basar::SS_DELETE | basar::SS_CLEAN) );
		cout << "pss.toString().c_str();" << endl;
		cout << pss.toString().c_str() << endl;

		cout << "ParameterList pl( \"Spalte4; Spalte3; Spalte2;\");" << endl;
		basar::cmnutil::ParameterList pl( "Spalte4; Spalte3; Spalte2;");
		
		cout << "pl.toStream();" << endl;
		pl.toStream();

		//try {
		//	cout << std::boolalpha << "isPropertyInState( ptr.begin(), pl, pss ): "
		//		 << basar::property::isPropertyInState( ptr.begin(), pl, pss ) << endl;
		//} catch( const basar::Exception& e ){
		//	COUT( e.what() )
		//}
	}
	

	//-------------------------------------------------------------------------------------//
	cout << "------------------------------------------------------------------------------"<< endl;
	cout << "check all parameterlist attributes if they exist in the yiterator and "
			"every yiterator attribute has not the state SS_UNSET AND "
			"every attribute found has to be in the state 'SS_DELETE' or 'SS_CLEAN' or 'SS_UNKNOWN' "
			"! --> should return false;" << endl;
	cout << "------------------------------------------------------------------------------"<< endl;
	//-------------------------------------------------------------------------------------//
	cout << "pdlr.toStream();" << endl;
	pdlr.toStream();
	{
		cout << "pss.insert( static_cast<basar::SupportedStateEnum>(basar::SS_DELETE | basar::SS_CLEAN) );"<< endl;
		pss.insert( static_cast<basar::SupportedStateEnum>(basar::SS_DELETE | basar::SS_CLEAN) );
		cout << "pss.toString().c_str();" << endl;
		cout << pss.toString().c_str() << endl;

		cout << "ParameterList pl( \"Spalte4; Spalte3; Spalte2;\");" << endl;
		basar::cmnutil::ParameterList pl( "Spalte4; Spalte3; Spalte2;");
		
		cout << "pl.toStream();" << endl;
		pl.toStream();

		cout << "ptr.begin().setInt16( \"Spalte4\", 1616 );" << endl;
		ptr.begin().setInt16( "Spalte4", 1616 );
		cout << "ptr.begin().begin().getState().toString().c_str();" << endl;
		cout << ptr.begin().begin().getState().toString().c_str() << endl;

		//try {
		//	cout << std::boolalpha << "isPropertyInState( ptr.begin(), pl, pss ): "
		//		 << basar::property::isPropertyInState( ptr.begin(), pl, pss ) << endl;
		//} catch( const basar::Exception& e ){
		//	COUT( e.what() )
		//}
	}
	

	DEBUG_LEAVE( cout, fun )
}
////////////////////////////////////////////////////////////////////////////////
// void check_Properties()
////////////////////////////////////////////////////////////////////////////////
void check_arePropertiesSet()
{
	__FUN__( check_arePropertiesSet() )
	DEBUG_ENTER( cout , fun )

	PropertyDescriptionListRef pdlr( PropertyDescriptionList::create() );
    
	//pdlr.construct( "int16 Spalte1;" "int16 Spalte2;" "int32 Spalte3;" "float32 Spalte4;" "float64 Spalte5;" 
	//				"decimal Spalte6;" "string Spalte7;" ); 

	//-------------------------------------------------------------------------------------//
	cout << "------------------------------------------------------------------------------"<< endl;
	cout << "check all parameterlist attributes if they exist in the yiterator and "
			"every yiterator attribute has not the state SS_UNSET! --> should return true;" << endl;
	cout << "------------------------------------------------------------------------------"<< endl;
	//-------------------------------------------------------------------------------------//
	cout << "pdlr.push_backs();"<< endl;
	pdlr.push_back( "Spalte4", basar::INT16  );    
	pdlr.push_back( "Spalte3", basar::STRING );    
	pdlr.push_back( "Spalte2", basar::INT32	);
	//pdlr.push_back( "Spalte1", basar::UNKNOWN	);
	//pdlr.push_back( "Spalte5", basar::INT32		);
	//pdlr.push_back( "Spalte6", basar::UNKNOWN	);
	pdlr.toStream();

	cout << "PropertyTableRef ptr( PropertyTable::create(pdlr) );"<< endl;
	PropertyTableRef ptr( PropertyTable::create(pdlr) );
	
	cout << "ptr.insert( basar::FOR_CLEAN );"<< endl;
	ptr.insert( basar::FOR_CLEAN );

	cout << "ptr.begin().setInt16( 'Spalte4', 16 );"<< endl;
	ptr.begin().setInt16( "Spalte4", 16 );
	
	cout << "ptr.begin().setString( \"Spalte3\", \"String\" );"<< endl;
	ptr.begin().setString( "Spalte3", "String" );
	
	cout << "ptr.begin().setInt32( \"Spalte2\", 32 );"<< endl;
	ptr.begin().setInt32( "Spalte2", 32 );

	{
		cout << "ParameterList pl( \"Spalte4; Spalte3; Spalte2;\");" << endl;
		basar::cmnutil::ParameterList pl( "Spalte4; Spalte3; Spalte2;");
		
		cout << "pl.toStream();" << endl;
		pl.toStream();
		try {
			cout << std::boolalpha << "basar::property::arePropertiesSet(ptr.begin(), pl): "
				<< basar::property::arePropertiesSet(ptr.begin(), pl) << endl;
		} catch( const basar::Exception& e ){
			COUT( e.what() )
		}
	}

	//-------------------------------------------------------------------------------------//
	cout << "------------------------------------------------------------------------------"<< endl;	
	cout << "one parameterlist attribute is missed --> should return false;" << endl;
	cout << "------------------------------------------------------------------------------"<< endl;
	//-------------------------------------------------------------------------------------//
	cout << "pdlr.toStream();" << endl;
	pdlr.toStream();
	{
		cout << "ParameterList pl( \"Spalte4; Spalte3;\");" << endl;
		basar::cmnutil::ParameterList pl( "Spalte4; Spalte3;");
		
		cout << "pl.toStream();" << endl;
		pl.toStream();
		try {
			cout << std::boolalpha << "basar::property::arePropertiesSet(ptr.begin(), pl): "
				<< basar::property::arePropertiesSet(ptr.begin(), pl) << endl;
		} catch( const basar::Exception& e ){
			COUT( e.what() )
		}
	}
	

	//-------------------------------------------------------------------------------------//
	cout << "------------------------------------------------------------------------------"<< endl;
	cout << "one yiterator attribute has the state SS_UNSET --> should return false;" << endl;
	cout << "------------------------------------------------------------------------------"<< endl;
	//-------------------------------------------------------------------------------------//
	
	cout << "ptr.clear();" << endl;
	ptr.clear();
	cout << "ptr.insert( basar::FOR_CLEAN );" << endl;
	ptr.insert( basar::FOR_CLEAN );
	
	cout << "ptr.begin().setInt32( \"Spalte2\", 32 );"<< endl;
	ptr.begin().setInt32( "Spalte2", 32 );
	
	cout << "ptr.begin().setString( \"Spalte3\", \"String\" );"<< endl;
	ptr.begin().setString( "Spalte3", "String" );
	
	cout << "pdlr.toStream();" << endl;
	pdlr.toStream();
	{
		cout << "ParameterList pl( \"Spalte4; Spalte3; Spalte2;\");" << endl;
		basar::cmnutil::ParameterList pl( "Spalte4; Spalte3; Spalte2;");
		cout << "pl.toStream();" << endl;
		pl.toStream();
		try {
			cout << std::boolalpha << "basar::property::arePropertiesSet(ptr.begin(), pl): "
				<< basar::property::arePropertiesSet(ptr.begin(), pl) << endl;
		} catch( const basar::Exception& e ){
			COUT( e.what() )
		}
	}

	//-------------------------------------------------------------------------------------//
	cout << "------------------------------------------------------------------------------"<< endl;
	cout << "two of the three columns are in ParameterList, these two are set in PropertyTable "
			"and the third is unset --> should return true;" << endl;
	cout << "------------------------------------------------------------------------------"<< endl;
	//-------------------------------------------------------------------------------------//

	cout << "ptr.clear();" << endl;
	ptr.clear();
	cout << "ptr.insert( basar::FOR_CLEAN );" << endl;
	ptr.insert( basar::FOR_CLEAN );
	
	cout << "ptr.begin().setInt32( \"Spalte2\", 32 );"<< endl;
	ptr.begin().setInt32( "Spalte2", 32 );
	
	cout << "ptr.begin().setString( \"Spalte3\", \"String\" );"<< endl;
	ptr.begin().setString( "Spalte3", "String" );
	
	cout << "pdlr.toStream();" << endl;
	pdlr.toStream();
	{
		cout << "ParameterList pl( \"Spalte3; Spalte2;\");" << endl;
		basar::cmnutil::ParameterList pl( "Spalte3; Spalte2;");
		cout << "pl.toStream();" << endl;
		pl.toStream();
		try {
			cout << std::boolalpha << "basar::property::arePropertiesSet(ptr.begin(), pl): "
				<< basar::property::arePropertiesSet(ptr.begin(), pl) << endl;
		} catch( const basar::Exception& e ){
			COUT( e.what() )
		}
	}
	DEBUG_LEAVE( cout, fun )
}
////////////////////////////////////////////////////////////////////////////////
// void checkPropertyTableRef()
////////////////////////////////////////////////////////////////////////////////
void checkPropertyTableRef()
{
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
	
	PropertyDescriptionListRef pdlr( PropertyDescriptionList::create() );
    
	/* pdlr.construct( "int16 Spalte1;" "int16 Spalte2;" "int32 Spalte3;" "float32 Spalte4;" "float64 Spalte5;" 
					"decimal Spalte6;" "string Spalte7;" ); */

	pdlr.push_back( "Spalte1", basar::INT16		);    
	pdlr.push_back( "Spalte2", basar::UNKNOWN	);    
	pdlr.push_back( "Spalte3", basar::INT32		);
	pdlr.push_back( "Spalte4", basar::UNKNOWN	);
	pdlr.push_back( "Spalte5", basar::INT32		);
	pdlr.push_back( "Spalte6", basar::UNKNOWN	);

	PropertyTableRef ptr1( PropertyTable::create(pdlr) );
	PropertyTableRef ptr2( PropertyTable::create(pdlr) );

	PropertyTable_YIterator yitBegin = ptr1.begin();
	PropertyTable_YIterator yitEnd   = ptr1.end();

	if( yitBegin == yitEnd ){
		COUT( "yitBegin == yitEnd" );
	} else {
		COUT( "yitBegin != yitEnd" );
	}

	/*if( ptr1.begin() == ptr1.end() ){
		COUT( "ptr1 == ptr1" );
	} else {
		COUT( "ptr1 != ptr1" );
	}*/

	PropertyDescriptionList_Iterator beg1 = pdlr.begin();
	cout << (*beg1).first << endl;


	PropertyDescriptionList_Iterator beg2 = pdlr.begin();
	++beg2;
	++beg1;

	if( beg1 == beg2 ){
		COUT("beg1 == beg2");
	} else {
		COUT("beg1 != beg2");
	}
	//return;


	PropertyTableRef ptr( PropertyTable::create(pdlr) );
	ptr.insert( basar::FOR_CLEAN );
	ptr.insert( basar::FOR_CLEAN );
	//COUT( ptr.size() )
	cout << endl;

	PropertyTable_YIterator yit1, yit2;
	yit1  = ptr.begin();
	yit2 = ptr.begin();
	
	//PropertyTable_XIterator xit = yit.begin();
	//xit.setInt16( 16 );

	//yit2++;
//	COUT( yit2.getCurrentIndex() )
//	COUT( yit1.getCurrentIndex() )

//	COUT( xit.getInt16() )
//	for( ; xit != yit.end(); xit++ ){
//		cout << endl;
//		cout << endl;
//		COUT( xit.getIndex() )
//		COUT( xit.getName() )
//		COUT( xit.getType().toString() )
//		COUT( xit.getState().toString() )
		
//		COUT( xit.getIteratedStateSet().toString() )
//	}

	//COUT( yit.getState("Spalte1").toString() )
	//COUT( yit.getType("Spalte1").toString() )
	
	//return;

}
/*
////////////////////////////////////////////////////////////////////////////////
// void checkPropertyDescriptionListRef()
////////////////////////////////////////////////////////////////////////////////
void checkPropertyDescriptionListRef()
{
	using basar::property::PropertyType;    
    using basar::property::PropertyDescriptionList;
    using basar::property::PropertyDescriptionListRef;
    using basar::property::ColumnName;
    using basar::property::ColumnSize;
//    using basar::property::UnknownColumnNameException;
//    using basar::property::OutOfRangeException;
	using basar::property::PdlSharedPtr;
    using basar::property::PropertyDescriptionList_Iterator;
	using basar::property::PropertyNameAlreadyExistsException;


    PropertyDescriptionListRef p1( PropertyDescriptionList::create() );
	try {
		p1.push_back( "Spalte1", basar::INT32      );    
		p1.push_back( "Spalte2", basar::UNKNOWN  );    
		p1.push_back( "Spalte3", basar::INT32      );
		p1.push_back( "Spalte4", basar::UNKNOWN  );
		p1.push_back( "Spalte4", basar::UNKNOWN  );
	} catch( const PropertyNameAlreadyExistsException& r ){
		COUT( (r.what()) )
	}
    PropertyDescriptionList_Iterator it1 = p1.begin();
    COUT( "---------------------" )
    COUT( "p1" )	
    COUT( "---------------------" )
    for(; it1 != p1.end(); ++it1 ){
        COUT( ((*it1).first) )
		COUT( ((*it1).second.getType()) )
    }
    
    PropertyDescriptionListRef p2( PropertyDescriptionList::create() );
    p2.push_back( "Spalte1", basar::INT32      );    
    p2.push_back( "Spalte2", basar::UNKNOWN  );    
    p2.push_back( "Spalte3", basar::UNKNOWN  );
    
 
    PropertyDescriptionList_Iterator it2 = p2.begin();
    COUT( "---------------------" )
    COUT( "p2" )    
    COUT( "---------------------" )
    for(; it2 != p2.end(); ++it2 ){
        COUT( ((*it2).first) )
		COUT( ((*it2).second.getType()) )
    }
 
    //COUT_BOOL( basar::property::isSubset(p1.begin(), p1.end(), p2.begin(), p2.end()))
}
*/
////////////////////////////////////////////////////////////////////////////////
// void checkPropertyDescriptionList()
////////////////////////////////////////////////////////////////////////////////
void checkPropertyDescriptionList()
{
    /*
	using basar::property::PropertyType;    
    using basar::property::PropertyDescriptionList;
    using basar::property::ColumnName;
    using basar::property::ColumnSize;
    using basar::property::UnknownColumnNameException;
    using basar::property::OutOfRangeException;
    using basar::property::PdlSharedPtr;
    
    PdlSharedPtr p = PropertyDescriptionList::create();

	COUT( p.use_count() )
	

	//return;
    p->push_back( "Spalte1", basar::INT      );    
    p->push_back( "Spalte2", basar::UNKNOWN  );    
    p->push_back( "Spalte3", basar::INT      );
    p->push_back( "Spalte4", basar::UNKNOWN  );

	PropertyDescriptionList::Iterator it = p->begin();
    for(; it != p->end(); ++it ){
        COUT( ((*it).first) )
		COUT( ((*it).second.getType()) )
    }
	*/
/*
    PropertyDescriptionList pdl;
    ColumnName colName = "Spalte1";
    PropertyType pt( basar::INT );
    //pdl.push_back( std::make_pair(colName, pt) );
    pdl.push_back( colName, pt );

    pdl.push_back( "Spalte2", basar::UNKNOWN  );    
    pdl.push_back( "Spalte3", basar::INT );
    pdl.push_back( "Spalte4", basar::UNKNOWN );

    /*
    //std::pair<ColumnName, PropertyType>
    try {
        cout << pdl.getType( "Spalte3" ).getType() << endl;
    } catch ( ... ){
        cout << "Exception" << endl;
    }
    
	PropertyDescriptionList::Iterator it = pdl.begin();
	for(; it != pdl.end(); ++it ){
        COUT( ((*it).first) )
		COUT( ((*it).second.getType()) )
    }
    try
    {
        ColumnSize size = pdl.getIndex( "BB" );
    } catch( const UnknownColumnNameException& r ){
        COUT("UnknownColumnNameException");
    }
    
    try
    {
        ColumnName name = pdl.getName( static_cast<ColumnSize>(999) );
    } catch( const OutOfRangeException& r ){
        COUT("OutOfRangeException");
    }
  try
    {
        PropertyType p = pdl.getType( "BBB" );
    } catch( const UnknownColumnNameException& r ){
        COUT("UnknownColumnNameException");
    }
    try
    {
        PropertyType p = pdl.getType( static_cast<ColumnSize>( 999 ) );
    } catch( const OutOfRangeException& r ){
        COUT("OutOfRangeException");
    }
    */
}
////////////////////////////////////////////////////////////////////////////////
// void checkPropertyType()
////////////////////////////////////////////////////////////////////////////////
void checkPropertyType()
{
    /*
	FUNCTION_NAME( checkPropertyType() )
    COUT( fun )
    DEBUG_ENTER( fun )

    using basar::property::PropertyType;
    using basar::property::UnknownPropertyTypeException;

    PropertyType pt1( basar::INT32 );
    PropertyType pt2( basar::UNKNOWN );

    COUT_BOOL( (pt1 == pt2) )
    
    try {
        PropertyType pt3( static_cast<basar::SupportedTypeEnum>(-100) );
    } catch ( const UnknownPropertyTypeException& r ){
        COUT( (r.what()) )
    }
    
    DEBUG_LEAVE( fun )
	*/
}
////////////////////////////////////////////////////////////////////////////////
// void checkPropertyState()
////////////////////////////////////////////////////////////////////////////////
void checkPropertyState()
{
    /*
	FUNCTION_NAME( void checkPropertyState() )
    DEBUG_ENTER( fun )

    using basar::property::PropertyState;
    using basar::property::IllegalPropertyStateException;    
	using basar::property::SupportedStateEnum;

    PropertyState ps;


	COUT( (ps.toString()) )
    PropertyState ps0( basar::property::CLEAN );
    COUT( ps0.toString() )
 
//    PropertyState ps1( basar::property::DIRTY );
    //COUT( ps1.toString() )
    
    PropertyState ps2( basar::property::INSERT );
    COUT( ps2.toString() )
    
    PropertyState ps3( basar::property::DELETE );
    COUT( ps3.toString() )
    
    PropertyState ps4( basar::property::UNKNOWN );
    COUT( ps4.toString() )

    try {
        PropertyState ptex( static_cast<SupportedStateEnum>(-100) );
    } catch ( const IllegalPropertyStateException& r ){
        COUT( (r.what()) )
    }    
    DEBUG_LEAVE( fun )
	*/
} 
////////////////////////////////////////////////////////////////////////////////
// void testBoostAny()
////////////////////////////////////////////////////////////////////////////////
void checkPropertyStateSet()
{
	using basar::property::PropertyStateSet; 
	using basar::SupportedStateEnum;
	using basar::property::IllegalPropertyStateException;

	try {
		PropertyStateSet pss1( static_cast<SupportedStateEnum>(64) );
	} catch( IllegalPropertyStateException& e ){
		COUT( e.what() )
	}
}
////////////////////////////////////////////////////////////////////////////////
// void testBoostAny()
////////////////////////////////////////////////////////////////////////////////
void testBoostAny()
{
	TestBoostAny tba;
	
	if( tba.m_Any.empty() ){
		COUT( "tba.m_Any.empty()" )	
	} else {
		COUT( !"tba.m_Any.empty()" )	
	}

	tba.setInt( 999 );

	COUT( tba.m_Any.empty() )	
	if( tba.m_Any.empty() ){
		COUT( tba.m_Any.empty() )	
	} else {
		COUT( !tba.m_Any.empty() )	
	}

	COUT( tba.getInt() )	
	//tba.setFloat( 999.99 );
	COUT( tba.getFloat() )
	tba.setString( "999 as string" );
	COUT( tba.getString() )

	

		
//	boost::any myAnyInt( 99 );
	/*
	_CRTIMP int before(const type_info& rhs) const;
    _CRTIMP const char* name() const;
    _CRTIMP const char* raw_name() const;
	*/
//	COUT( myAnyInt.type().name() )
//	COUT( myAnyInt.type().raw_name() )

}
/*
////////////////////////////////////////////////////////////////////////////////
// void checkBoostSharedPtr()
////////////////////////////////////////////////////////////////////////////////
void checkBoostSharedPtr()
{
	COUT( g.use_count() )
	boost::shared_ptr<int> rp( new int(999) );
	COUT( rp.use_count() )

	g = rp;
	COUT( "nach g = rp: " )
	COUT( g.use_count() )
	COUT( rp.use_count() )

	//COUT( *rp )
	//COUT( *g )
	//COUT( (*getSPt()) )
	boost::shared_ptr<int>& xxx = getSPtAsRef();
	COUT( g.use_count() )
	COUT( rp.use_count() )
	COUT( xxx.use_count() )
}
boost::shared_ptr<int> getSPtAsValue()
{
	COUT( g.use_count() )
	return g;
}
boost::shared_ptr<int>& getSPtAsRef()
{
	COUT( g.use_count() )
	return g;
}
*/