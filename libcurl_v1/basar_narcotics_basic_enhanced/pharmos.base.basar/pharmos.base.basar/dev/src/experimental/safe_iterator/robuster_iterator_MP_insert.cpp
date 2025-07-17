// robit.cpp : Definiert den Einstiegspunkt für die Konsolenanwendung.
//

#include "stdafx.h"


// robuster_iterator.cpp : feasibility  study
//

#include <iostream>
#include <list>
#include <boost/shared_ptr.hpp>
#include <boost/weak_ptr.hpp>

using namespace std;


class Row
{
public:
	Row() {}
	~Row() { cout << "destruct: " << m_value << endl; }
	Row( int value ) : m_value( value ) {}
	int value() { return m_value; }
private:
	int m_value;
};

typedef boost::shared_ptr<Row> RowSharedPtr;					// equal to basar::property::PairInsertTypeCollPropertyInternalColumn
typedef boost::weak_ptr<Row> RowWeakPtr;
typedef std::list<RowSharedPtr> RowSharedPtrTable;				// equal to basar::property::CollPropertyInternalRow
typedef RowSharedPtrTable::iterator RowSharedPtrTableIter;		// equal to basar::property::CollPropertyInternalRowIter
typedef boost::shared_ptr<RowSharedPtrTable> TableSharedPtr;	// equal to basar::property::PropertyTableSharedPtr
typedef boost::weak_ptr<RowSharedPtrTable> TableWeakPtr;		

class YIterator
{
public:
	YIterator( TableWeakPtr tablePtr, RowSharedPtrTableIter tableIter ) 
		: m_tablePtr( tablePtr ), m_iter( tableIter ) 
	{
		if( tableIter != tablePtr.lock()->end() )
		{
			// get weak pointer from shared pointer
			m_weakRow = ( *tableIter );
		}
	}
	bool isNull() { return m_weakRow.expired(); }
	bool isEnd() { return ( m_tablePtr.lock()->end() == m_iter ); }
	YIterator operator++()
	{
		if( isNull() )
		{
			cout << "operator++() - Exception: iterator is NULL!" << endl;
		}
		else if( isEnd() )
		{
			cout << "operator++() - Exception: iterator is END!" << endl;
		}
		else
		{
			++m_iter;
			// MP_INSERT: get weak pointer from shared pointer AGAIN!!!
			if (!isEnd()) m_weakRow = ( *m_iter );			
		}
		return ( *this );
	}
	void print() 
	{
		if( isNull() ) 
		{
			cout << "print() - Exception: iterator is NULL!" << endl;
			return;
		}
		if( isEnd() ) 
		{
			cout << "print() - Exception: iterator is END!" << endl;
			return;
		}
		cout << ( *m_iter )->value() << endl;
	}
	void erase() { m_tablePtr.lock()->erase(m_iter); } // MP_INSERT
private:
	TableWeakPtr			m_tablePtr;
	RowSharedPtrTableIter	m_iter;
	RowWeakPtr				m_weakRow;
};

class Table
{
public:
	Table() : m_tablePtr( new RowSharedPtrTable() ) {}
	Table( const Table & right ) : m_tablePtr( right.m_tablePtr ) {}
	Table & operator=( const Table & right ) { clear(); m_tablePtr = right.m_tablePtr; return ( *this ); }
	YIterator begin() { return YIterator( m_tablePtr, m_tablePtr->begin() ); }
	YIterator end() { return YIterator( m_tablePtr, m_tablePtr->end() ); }
	void insert( int value ) { m_tablePtr->push_back( RowSharedPtr( new Row( value ) ) ); }
	void erase( YIterator yit) { yit.erase(); } // MP_INSERT
	void clear() { m_tablePtr->clear(); }
private:
	TableSharedPtr	m_tablePtr;
};




int main(int argc, char* argv[])
{
	Table t, x, y; 
	t.insert( 4711 ); t.insert( 4712 ); t.insert( 4713 );
	x.insert( 1 ); x.insert( 2 ); x.insert( 3 );
	y.insert( 23 ); y.insert( 24 ); y.insert( 25 );
	
	cout << "start values" << endl;
	YIterator it = t.begin();
	while( !it.isEnd() )
	{
		it.print();
		it++;
	}

	YIterator it1 = t.begin();
	YIterator it2 = t.begin(); it2++;
	YIterator it3 = t.begin(); it3++; it3++;

	cout << "\ntest iters" << endl;
	it1.print(); it2.print(); it3.print(); 

	// test operator=
	cout << "\ntest operator=" << endl;
	t = x;
	it = t.begin();
	while( !it.isEnd() )
	{
		it.print();
		it++;
	}

	// this would crash with basar::YIterators
	it1.print(); it2.print(); it3.print(); 

	it1 = t.begin();
	it2 = t.begin(); it2++;
	it3 = t.begin(); it3++; it3++;

	// test copy constructor
	cout << "\ncopy constructor" << endl;
	t = Table( y );
	it = t.begin();
	while( !it.isEnd() )
	{
		it.print();
		it++;
	}

	// this would crash with basar::YIterators
	it1.print(); it2.print(); it3.print(); 

	// try to crash
	cout << "\ntest end()" << endl;
	it = t.end(); it.print();

	cout << "\ntest operator++" << endl;
	it = t.end(); it++; it.print();

	cout << "\ntest iterator of temporary table" << endl;
	Table * temp = new Table(); temp->insert( 890 );
	YIterator itNull = temp->begin();
	itNull.print();
	delete temp;
	itNull.print();

	cout << "\ntest copy iterators" << endl;
	YIterator itEnd = t.end(); itEnd.print();
	YIterator itNewEnd = itEnd; itNewEnd.print();	

	cout << "\n\n\n" << endl;
	
	// MP_INSERT: Test erase();
	it = t.begin();
	it++;
	t.erase(it);
	it.print();

	return 0;
}


