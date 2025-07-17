
#include <libbasarcmnutil.h>

#include "testcmnutil.h"

//------------------------------------------------------------------------------
using std::endl;
using std::cout;

using basar::cmnutil::ParameterList;
using basar::cmnutil::ParameterList_Iterator;

//------------------------------------------------------------------------------
void test_parameterlist()
{
	printf("\n");
	printf("--- test_parameterlist ---------------------------\n");
	printf("\n");

	const ParameterList pl2; // empty
	ParameterList_Iterator itbeginX = pl2.begin();
	ParameterList_Iterator itendX   = pl2.end();

	try {
		++itbeginX;
	} catch( const basar::OutOfRangeIteratorException& e ){
		cout << e.what().c_str() << endl;	
	}


	if( itbeginX.isBegin() ){
		cout << "itbeginX.isBegin()" << endl;
		cout << (*itbeginX).first << ", " << (*itbeginX).second << endl;
	} else {
		cout << "!itbeginX.isBegin()" << endl;
	}
	if( itendX.isEnd() ){
		cout << "itendX.isEnd()" << endl;
	} else {
		cout << "!itendX.isEnd()" << endl;
	}


	VarString str;
	str = "key;;1 == ;;val=ue1; key2;;; key3=value3;";
	ParameterList pl1(str);

	//pl1.clear();

	ParameterList_Iterator itbegin1 = pl1.begin();
	ParameterList_Iterator itend1   = pl1.end();

	if( itbegin1.isBegin() ){
		cout << "itbegin1.isBegin()" << endl;
	} else {
		cout << "!itbegin1.isBegin()" << endl;
	}
	if( itend1.isEnd() ){
		cout << "itend1.isEnd()" << endl;
	} else {
		cout << "!itend1.isEnd()" << endl;
	}
	if( itend1.isBegin() ){
		cout << "itend1.isBegin()" << endl;
	} else {
		cout << "!itend1.isBegin()" << endl;
	}
	if( itbegin1.isEnd() ){
		cout << "itbegin1.isEnd()" << endl;
	} else {
		cout << "!itbegin1.isEnd()" << endl;
	}

	++itbegin1;
	cout << "nach dem ++itbegin1" << endl;
    if( itbegin1.isBegin() ){
		cout << "itbegin1.isBegin()" << endl;
	} 
	if( itbegin1.isEnd() ){
		cout << "itbegin1.isEnd()" << endl;
	} 

	ParameterList pl;
	pl.push_back("key1","value1");
	pl.push_back("key2");
    pl.push_back(basar::cmnutil::ColParamListMap::value_type("key3","value3"));
	
	size_t size = pl.size();
	size = size;
	bool b = pl.isValueEmpty("key1");
	b = pl.isValueEmpty("key2");
	b = pl.isValueEmpty("key3");

	ParameterList_Iterator itbegin = pl.begin();
	ParameterList_Iterator itend = pl.end();

	while (itbegin != itend)
	{
		b = itbegin.isValueEmpty();
		++itbegin;
	}
	
	VarString str1 = pl.getValue("key1");
	pl.setValue("key2","value2");
	VarString str2 = pl.getValue("key2");

	if ( itbegin == itend )
		return;

	itbegin++;
	++itbegin;

	//ParameterList& pl3 = pl;
	//ParameterList *pl3 = new ParameterList;
	//pl3 = &pl;
	//delete pl3; //wann delete bzw. überhaupt?

	ParameterList pl4 (pl);

    ParameterList_Iterator it = pl4.begin();
    cout << (*it).first << ", " << (*it).second << endl;
    (*it).second = "value1_1";
    it->second = "value1_1_1";

    ParameterList_Iterator itt = pl1.begin();
   
	while( itt != pl1.end() ){
        cout << (*itt).first << ", " << (*itt).second << endl;
        ++itt;
    }
}
