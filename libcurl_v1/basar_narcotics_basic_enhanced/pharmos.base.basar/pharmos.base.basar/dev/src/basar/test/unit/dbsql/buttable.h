/** $Id$
*
* @file table.h
* @brief ...
* @author Anke Klink
* @date 2007
*/

#ifndef  ___MARATHON_LIB_BASAR_TEST_UNIT_DBSQL_BUTTABLE_H__ 
#define  ___MARATHON_LIB_BASAR_TEST_UNIT_DBSQL_BUTTABLE_H__ 

//------------------------------------------------------------------------------
#include <set>
#include "libbasarcmnutil_datetime.h"
#include "libbasarcmnutil_date.h"
#include "libbasarcmnutil_decimal.h"
#include "libbasarcmnutil_timespan.h"

//------------------------------------------------------------------------------
namespace basar
{
namespace test
{
namespace unit
{
namespace dbsql
{
/*!
* Table for Basar Unit Test Tables
*/
class BUTTable
{
public:
	//! Indices of the Table columns
	enum ColIndex
	{
		colnumber_colser = 0,
		colnumber_colint,
		colnumber_colsmint,
		colnumber_colfl,
		colnumber_colsmfl,
		colnumber_coldec,
		colnumber_colmon,
		colnumber_colch1,
		colnumber_colch2,
		colnumber_colvch1,
		colnumber_colvch2,
		colnumber_colvch3,
		colnumber_colvch4,
		colnumber_collvch,
		colnumber_coldate,
		colnumber_coldt1,
		colnumber_coldt2,
		colnumber_coltime1,
		colnumber
	};

	static const basar::UInt32 csm_collengthColch1; //!< Length of column Colchch1 
	static const basar::UInt32  csm_collengthColch2;//!< Length of column Colchch2 
	static const basar::UInt32 csm_collengthColvch1;//!< Length of column Colchvch1 
	static const basar::UInt32 csm_collengthColvch2;//!< Length of column Colchvch2
	static const basar::UInt32 csm_collengthColvch3;//!< Length of column Colchvch3 
	static const basar::UInt32 csm_collengthColvch4;//!< Length of column Colchvch4 

protected:

	//! Entry of a column 
	struct Entry
	{
		basar::Int32        m_colser;
		basar::Int32        m_colint;
		basar::Int16        m_colsmint;
		basar::Float64	    m_colfl;
		basar::Float32	    m_colsmfl;
		basar::Decimal  	m_coldec;
		basar::Decimal   	m_colmon;
		basar::ConstString	m_colch1;
		basar::ConstString	m_colch2;
		basar::ConstString	m_colvch1;
		basar::ConstString	m_colvch2;
		basar::ConstString	m_colvch3;
		basar::ConstString	m_colvch4;
		basar::ConstString	m_collvch;
		basar::Date	        m_coldate;
		basar::DateTime	    m_coldt1;
		basar::DateTime	    m_coldt2;
		basar::DateTime  	m_coltime1;

		//! Default constructor
		Entry();

		//! Constructor with a whole row
		Entry(
			const basar::Int32        colser,
			const basar::Int32        colint,
			const basar::Int16        colsmint,
			const basar::Float64	  colfl,
			const basar::Float32	  colsmfl,
			const basar::Decimal  	& coldec,
			const basar::Decimal   	& colmon,
			const basar::ConstString colch1,
			const basar::ConstString colch2,
			const basar::ConstString colvch1,
			const basar::ConstString colvch2,
			const basar::ConstString colvch3,
			const basar::ConstString colvch4,
			const basar::ConstString collvch,
			const basar::Date	   & coldate,
			const basar::DateTime  & coldt1,
			const basar::DateTime  & coldt2,
			const basar::DateTime  & coltime1);

	private:
		Entry & operator=(const Entry &);
		Entry(const Entry &);

	};

	//!< Column names 
	std::vector<std::string> m_colNames; 

	//!< Set of rows (not all rows need to be given)
	std::set<basar::UInt32 > m_rows;

	//! Set of entries for various rows
	std::map<basar::UInt32 , boost::shared_ptr<Entry> > m_entries;

protected:

	//! init Function
	void init();

	//! init column names
	void initNames();

public:

	//! constructor
	BUTTable();

	//! Destructor
	virtual ~BUTTable();

	//! returns the name of a column 
	std::string getName(ColIndex )const;

	//! returns list of culumn names
	const std::vector< std::string > getColNames()const;

	//! returns the Value of a row/column entry
	basar::Int32 getInt32(basar::UInt32 row,ColIndex ) ;

	//! returns the Value of a row/column entry
	basar::Int16 getInt16(basar::UInt32  row,ColIndex ) ;

	//! returns the Value of a row/column entry
	basar::Float32 getFloat32(basar::UInt32  row,ColIndex ) ;

	//! returns the Value of a row/column entry
	basar::Float64 getFloat64(basar::UInt32  row,ColIndex ) ;

	//! returns the Value of a row/column entry
	basar::Decimal getDecimal(basar::UInt32  row,ColIndex ) ;

	//! returns the Value of a row/column entry
	basar::ConstBuffer getString(basar::UInt32  row,ColIndex ) ;

	//! returns the Value of a row/column entry
	basar::DateTime getDateTime(basar::UInt32  row,ColIndex )    ;

	//! returns the Value of a row/column entry
	basar::Time     getTime     (basar::UInt32  row,ColIndex )    ;

	//! returns the Value of a row/column entry
	basar::Date getDate(basar::UInt32  row,ColIndex ) ;


	//! returns the minimum Entry in a column over all rows
	basar::Int32 getMinInt32(ColIndex);

	//! returns the sum of the  Entries in a culumn over all rows
	basar::Int32 getSumInt32(ColIndex);

	//! adds a new row (or changes an existing row), and fills it with values
	void addRow(
		const basar::UInt32      row,
		const basar::Int32       colser,
		const basar::Int32       colint,
		const basar::Int16       colsmint,
		const basar::Float64	 colfl,
		const basar::Float32     colsmfl,
		const basar::Decimal   & coldec,
		const basar::Decimal   & colmon,
		const basar::ConstString colch1,
		const basar::ConstString colch2,
		const basar::ConstString colvch1,
		const basar::ConstString colvch2,
		const basar::ConstString colvch3,
		const basar::ConstString colvch4,
		const basar::ConstString collvch,
		const basar::Date	   & coldate,
		const basar::DateTime  & coldt1,
		const basar::DateTime  & coldt2,
		const basar::DateTime  & coltime1);

	//! adds a new row (or changes an existing row), and fills it with values
	void addRowsFromResultSets(basar::db::sql::ResultsetRef);

};

}
}
}
}
#endif 

// ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~  ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~
