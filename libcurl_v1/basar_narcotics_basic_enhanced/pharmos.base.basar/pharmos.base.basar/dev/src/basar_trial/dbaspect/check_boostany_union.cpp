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

void report( std::string fun, std::string testcase, std::string exceptInfo = "", bool ok = true, ostream& strm = std::cout); 
AccessorInstanceRef checkBBAccessorInstanceRef( ConnectionRef connection, CachingPolicyEnum );
AccessorInstanceRef checkBLieSortAccessorInstanceRef( ConnectionRef connection, CachingPolicyEnum );
const ostringstream& getTime();
const ostringstream& getDate();

union DataType
	{
		//basar::ConstString	m_constString;
		//basar::VarString	m_varString;
		//basar::I18nString   m_i18nString;

		basar::Int8			m_int8;
		basar::Int16		m_int16;
		basar::Int32		m_int32;
		basar::Long32		m_long32;

		basar::UInt8		m_uint8;
		basar::UInt16		m_uint16;
		basar::UInt32		m_uint32;
		basar::ULong32		m_ulong32;

		basar::Float64		m_float64;
		basar::Float32		m_float32;
		//basar::Decimal		m_decimal;
		//basar::DateTime		m_dateTime;
};
//--------------------------------------------------------------------------------------------------------//
// void check_States( ConnectionRef connection, AccessorInstanceRef air )
//--------------------------------------------------------------------------------------------------------//
void check_BoostAny_Union( ConnectionRef connection, AccessorInstanceRef air )
{
	static const char* const fun = "check_BoostAny_Union(ConnectionRef, AccessorInstanceRef)";

	//ostream& strm = cout;
	ofstream log ( "aspect.check_boostany_union.log" ); ostream& strm = log;
	strm << "====================================================================" << endl;
	strm << fun << " - Entry!" << endl;
	strm << "====================================================================" << endl << endl;

	strm << "Date: " << getDate().str().c_str() << "\tTime: " << getTime().str().c_str() << endl;
	ostringstream os;
	
	strm << "--------------------------------------------------------------------" << endl;
	strm << "sizeof basar datatypes" << endl;
	strm << "--------------------------------------------------------------------" << endl;
	strm << "sizeof( ConstString ): " << sizeof( basar::ConstString ) << endl; 
	strm << "sizeof( VarString ): " << sizeof( basar::VarString ) << endl; 
	strm << "sizeof( I18nString ): " << sizeof( basar::I18nString ) << endl; 

	strm << "sizeof( Int8 ): " << sizeof( basar::Int8 ) << endl; 
	strm << "sizeof( Int16 ): " << sizeof( basar::Int16 ) << endl; 
	strm << "sizeof( Int32 ): " << sizeof( basar::Int32 ) << endl; 
	strm << "sizeof( Long32 ): " << sizeof( basar::Long32 ) << endl; 

	strm << "sizeof( UInt8 ): " << sizeof( basar::UInt8 ) << endl; 
	strm << "sizeof( UInt16 ): " << sizeof( basar::UInt16 ) << endl; 
	strm << "sizeof( UInt32 ): " << sizeof( basar::UInt32 ) << endl; 
	strm << "sizeof( ULong32 ): " << sizeof( basar::ULong32 ) << endl; 

	strm << "sizeof( Float64 ): " << sizeof( basar::Float64 ) << endl; 
	strm << "sizeof( Float32 ): " << sizeof( basar::Float32 ) << endl; 
	strm << "sizeof( Decimal ): " << sizeof( basar::Decimal ) << endl; 
	strm << "sizeof( DateTime ): " << sizeof( basar::DateTime ) << endl; 
	
	strm << "--------------------------------------------------------------------" << endl;
	strm << "sizeof union of basar::datatypes" << endl;
	strm << "--------------------------------------------------------------------" << endl;
	strm << "sizeof( DataType ): " << sizeof( DataType ) << endl; 
	
	
	air = checkBLieSortAccessorInstanceRef( connection, basar::db::aspect::FULL_CACHING );




	strm << "====================================================================" << endl;
	strm << fun << " - Exit!" << endl;
	strm << "====================================================================" << endl;
}
