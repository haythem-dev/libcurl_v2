/*
* @file                                            
* @author Anke Klink                      
* @date 2008                            
*/    


#ifndef  __BASAR_TEST_UNIT_DBASPECT_TESTACCESSORHOLDER_H__ 
#define  __BASAR_TEST_UNIT_DBASPECT_TESTACCESSORHOLDER_H__ 

#include "libbasardbaspect.h"
#include "libbasarcmnutil_parameterlist.h"
#include "libbasar_definitions.h"
#include "libbasarcmnutil_parameterlist.h"
#include "libbasarproperty_propertytablesnapshotref.h"

#include "testconnection.h"
#include "buttablereader.h"
#include "testsqlresolvequeries.h"
#include "testaccessorexecutelistener.h"
#include "testnames.h"
#include "buttablepropertydescriptions.h"
#include "loggerpool.h"

#define SHOW_METHODNAME(Y, X) TEXTOUTPUT(Y << " Test with method " << X);

namespace basar    {
namespace test     {
namespace unit     {
namespace dbaspect {

/*!
* Class that represents all data used for the tests:
* <ul>
*   <li> List of Methodnames for the ensure Tests </li> 
*   <li> 3 Acessors that each hold one line, with selected data </li>
*   <li> 1 Acessor that holds all the ensureTest Methods </li>
*   <li> various DefaultParamter Lists </li>
*   <li> The Connection </li>
* </ul>
*/
struct TestAccessorHolder
{ 

	/*!
	* Helperclass around
	* an Accesoor that holds one 
	* line of selected data
	*/
	struct SelectAccessor
	{     
		basar::db::aspect::AccessorInstanceRef             m_ai;				//!< The AccessorInstanceRef, that holds one line of selected data
		basar::db::aspect::AccessorPropertyTableRef        m_propTable;			//!< The AccessorPropertyTableRef, that holds one line of selected data
		basar::db::aspect::AccessorPropertyTable_YIterator m_yiter;				//!< The one line of selected data
		basar::db::aspect::ExecuteResultInfo               m_Error;				//!< The result of the select Accessormethod
		VarString                                          m_accessorMethod;	//!< The name of the select Accessormethod

		/*!
		* cleans the data 
		*/
		void clean();
		/*!
		* initializes the object, creates the 
		* AccessorInstanceRef and calls select
		*/
		void init(
			basar::db::aspect::ConnectionRef & connref, 
			ConstString                        accessorname1, 
			ConstString                        realaccessorname, 
			ConstString                        accessormethod);
		/*!
		* executes the m_accessorMethod Method and initites
		* m_propTable and m_yiter
		* @return Error ocurred
		*/
		bool select();
		/*!
		* Sets the PropertyState to one column of the Y-Iterator
		*/
		void setPropertyState(
			const basar::VarString & col,  //!< columnnamme
			basar::SupportedStateEnum  state //!< state to be set
			);
		/*!
		* Sets the PropertyState to all columns of the Y-Iterator
		*/
		void setPropertyState( basar::SupportedStateEnum  state);
		/*!
		* returns if the execute Statement was sucessfull and if more than
		* 0 lines are selected.
		*/
		bool isOk() const;
	};

	/*!
	* Helperclass around
	* an Accesoor that holds the Test Acessor
	*/
	struct TestAccessor
	{ 
		basar::db::aspect::AccessorInstanceRef m_ai;   //!< The Test Acessor 

		/*!
		* initializes the object, creates the 
		* AccessorInstanceRef and calls select
		*/
		void init(
			basar::db::aspect::ConnectionRef & connref, 
			ConstString                        accessorname1, 
			ConstString                        realaccessorname);
		/*!
		* resets the AccessorInstanceRef
		*/
		void clean();
	};


	/*!
	* Helperclass around an AccessorMethod, holds the information:
	* <ul>
	*    <li>Methodname</li>
	*    <li>Necessary Supported State </li>
	*    <li>other Supported State, that is not the Necessary Supported State
	*        and is not the supported state of the next AccessorMethodEntry
	*        in a AccessorMethodEntryList </li>
	*    <li> Helpermember that show, what actions are to be done, before and 
	*         after execution and test 
	*     <ul>
	*         <li> m_changeColser : Does the colser variable in the selected line
	*              and/or ParameterList have to be changed, to one of the last
	*              inserted serial numbers (update and remove Tests) </li>
	*         <li> m_removeColser : Are all colser variables in Ficture to
	*              be removed befor the Tests starts (insert tests) </li>
	*         <li> m_resetSelectIterator: Does the select Y-Itertor 
	*              have to be reselected after the method execution (remove Tests) </li>
	*     </ul>
	*  </li>
	* </ul>
	*/
	struct AccessorMethodEntry
	{
		VarString m_methodname;                              //!<< Methodname
		basar::SupportedStateEnum m_usedSupportedStateEnum;  //!<< Necessary Supported State

		/*!
		* other Supported State, that is not the Necessary Supported State
		* and is not the supported state of the next AccessorMethodEntry
		* in a AccessorMethodEntryList
		*/
		basar::SupportedStateEnum m_otherSupportedStateEnum; 

		/*!
		* Does the colser variable in the selected line
		* and/or ParameterList have to be changed, to one of the last
		* inserted serial numbers (update and remove Tests)
		*/

		bool m_changeColser;
		/*!
		* Are all colser variables in Ficture to
		*              be removed befor the Tests starts (insert tests)
		*/
		bool m_removeColser;

		/*!
		* Does the select Y-Itertor 
		* have to be reselected after the method execution (remove Tests)
		*/
		bool m_resetSelectIterator;

		/*!
		* constructor:
		*/
		AccessorMethodEntry(
			const VarString & name, 
			basar::SupportedStateEnum s1,
			basar::SupportedStateEnum s2);
		/*!
		* returns the colser value number nr 
		* from the AccessorInstanceRef, 
		* calls ai.getLastInsertedSerial(nr)
		*/
		std::pair<bool , Int64> getColser(
			basar::db::aspect::AccessorInstanceRef & ai,  //!< AccessorInstance, the LastInsertedSerial is read from
			Int32                                    nr   //!< number of the LastInsertedSerial to be returned
			)const;
		/*!
		* changes the colser variable on the 
		* AccessorPropertyTable_YIterator yiter
		* to the nr. LastInsertedSerial from the
		* AccessorInstanceRef ai
		*/
		bool changeColserIfNecessary(
			basar::db::aspect::AccessorPropertyTable_YIterator & yiter, //!< YIterator, to be changed
			basar::db::aspect::AccessorInstanceRef             & ai,    //!< AccessorInstance, the LastInsertedSerial is read from
			Int32                                                  nr,    //!< number of the LastInsertedSerial to be returned
			bool                                               & colserCanged
			) const;
		/*!
		* changes the colser variable on the ParameterList plist
		* to the nr. LastInsertedSerial from the
		* AccessorInstanceRef ai
		*/
		bool changeColserIfNecessary(
			basar::cmnutil::ParameterList          & plist,   //!< ParameterList, to be changed
			basar::db::aspect::AccessorInstanceRef & ai,      //!< AccessorInstance, the LastInsertedSerial is read from
			Int32                                      nr,      //!< number of the LastInsertedSerial to be returned
			bool                                   & colserCanged
			) const;
	};

	/*!
	* List of AccessorMethodEntrys, that can be done 
	* sequencially.
	*/
	struct AccessorMethodEntryList
	{
		std::vector<AccessorMethodEntry> m_entries;  //< Vector of entries
		Int32 m_numberOfTestMethods;                       //< number of tests
		Int32 m_numberOfInsertTestMethods;                 //< number of entries

		/*! 
		* constructor
		*/
		AccessorMethodEntryList();
		/*!
		* returns a SupportedStateEnum that is not s1 and not s2
		* f.ex getOtherSupportedStateEnum(Insert,Delete) => Update
		*/
		basar::SupportedStateEnum getOtherSupportedStateEnum(
			basar::SupportedStateEnum s1, 
			basar::SupportedStateEnum s2)const;
		/*!
		* adds a new entry and changes the m_otherSupportedStateEnum of the previous entry,
		* to not be used the state of this test, in order to start with states that are 
		* seen as wrong for the test (SS_CLEAN can't be set for an Y-Iterator Value)
		*/
		void addEntry( basar::SupportedStateEnum  s1, const VarString & name);
	};

	basar::db::aspect::ConnectionRef m_connRef ;     //!< The Connection


	basar::cmnutil::ParameterList m_paramListEmpty;                  //!< Empty Paramter List
	basar::cmnutil::ParameterList m_paramListNotEnoughValues;        //!< Lists some becessary Default Values but not all
	basar::cmnutil::ParameterList m_paramListNonIntValues;           //!< Lists all necessay non Int Values (completed by the Select Int Y-Iteratory)
	basar::cmnutil::ParameterList m_paramListAllEnsureValues;        //!< Lists all necessay Default Values (no Y-Iterator needed)

	TestAccessor   m_ensureTest;                     //!< Test Acessor for all Test methods
	SelectAccessor m_selectAllEnsureParameters;      //!< Selects all necessray Values 
	std::vector<SelectAccessor> m_selectIntVector;   //!< Selects all Int Values (completed by the Default Values in m_paramListNonIntValues)

	int m_idOfEnsureTestInIntVector;


	/*!
	* The actual active colser number, for INSERT, the number of LastSerialNumbers in TestAcessor
	* for Update and Delete the number of the LastSerialNumber that is written to the statement
	*/
	Int32 m_colsernumber;                           

	/*!
	* the constructor does not call the AcessorInstances as it will be 
	* calles outside of our own test scenario, the test starts with
	* a call to the member function start:
	* CHECK(start());
	*/
	TestAccessorHolder();
	/*!
	* The desructor does not call reset, as the test should do that 
	* itself
	*/
	virtual ~TestAccessorHolder();
	/*!
	* resets all values and closes the Connection
	*/
	void reset();
	/*!
	* returns if all select statements have returned valid
	* results
	*/
	bool isSelectOk();
	/*!
	* returns the size of the selectIntVector:
	*/
	Int32  getSelectIntVectorSize();
	/*!
	* start Routine that creates the Connection and 
	* initializes the AcessorInstances (create and 
	* execute Select Method) and retuns isSelectOk()
	* This is the first test to be done
	* CHECK(start())
	*/
	bool start( );
	/*!
	* all that is to be done before the test with a new Method,
	* f.ex. remove the Colser List (clearLastInsertedSerials), if
	* m_removeColser is set to true (Insert-Test)
	*/
	bool startNewAccessorMethodTest(
		const AccessorMethodEntry & entry //!< the method that is tobe tested
		);
	/*!
	* all that is to be done before the new test with a Method and a Y-Iterator
	* f.ex. reselect the Y-Iterator, when m_resetSelectIterator is
	* set to true (Delete-Test)
	* and change the colser variable, when m_changeColser is set
	* to true (Delete or Update Test)
	* advances the colsernumber, so each time startNewTest is called a
	* new colser variable is set.
	*/
	bool startNewTest( 
		const  AccessorMethodEntry & entry, //!< the method that is to be tested
		SelectAccessor & acc ,               //!< the Accessor the holds the Y-Iterator
		bool & colserCanged
		);
	/*!
	* all that is to be done before the new test with a Method and a ParameterList (no Y-Iterator)
	* change the colser variable, when m_changeColser is set
	* to true (Delete or Update Test)
	* advances the colsernumber, so each time startNewTest is called a
	* new colser variable is set.
	*/
	bool startNewTest( 
		const AccessorMethodEntry     & entry,  //!< the method that is to be tested
		basar::cmnutil::ParameterList & plist,  //!< the Default Paramter List, where the colser variable is to be set to
		bool                          & colserCanged);
	/*!
	* all that is to be done after all tests with a new Method,
	* f.ex. reselect all Y-Iterator when m_resetSelectIterator is
	* set to true 
	*/
	bool endAccessorMethodTest(
		const AccessorMethodEntry & entry //!< the method that was tested
		);
	/*!
	* sets all colums of the Y-Iterator to the state 
	* and executes the method
	*/
	basar::db::aspect::ExecuteResultInfo setPropertyAndExecute( 
		basar::VarString & method,            //!< the method that is to be tested 
		SelectAccessor & accessor,            //!< the y-Iterator that is to be set to state 
		basar::SupportedStateEnum state       //!< the state the y-Iterator is to be set to
		);
	/*!
	* List of all the methods that are tested in EnsureParametersAreSet
	*/
	Int32 addMethodsEnsureAreSet(AccessorMethodEntryList & methodnames);
	/*!
	* List of all the methods that are tested in EnsureParametersAreSetAndAllInState
	* the list has to be structured as a list of an TestTripel(InsertTest, UpdateTest, DeleteTest)
	*/
	Int32 addMethodsEnsureAreSetAndAllInState(AccessorMethodEntryList  & methodnames);
	/*!
	* List of all the methods that are tested in EnsureParametersAreSetAndOneInState
	* the list has to be structured as a list of an TestTripel(InsertTest, UpdateTest, DeleteTest)
	*/
	Int32 addMethodsEnsureAreSetAndOneInState(AccessorMethodEntryList & methodnames);
	/*!
	* List of all the methods that 
	* the list has to be structured as a list of an TestTripel(InsertTest, UpdateTest, DeleteTest)
	*/
	Int32 addMethodsEnsure1AreSetAnd2OneInState(AccessorMethodEntryList & methodnames);
	/*!
	* List of all the methods that are tested in EnsureParameters1AreSetAnd2OneInState
	* the list has to be structured as a list of an TestTripel(InsertTest, UpdateTest, DeleteTest)
	*/
	Int32 addMethodsEnsure1AreSetAnd2OneInStateUnsetAndInStateTest(AccessorMethodEntryList  & methodnames);

	bool checkYIteratorAfterExecutionAndReselectAtDelete(
		basar::SupportedStateEnum & usedSupportedStateEnum, 
		SelectAccessor & selectaccessor);
};


// --------------------------------------------------------------------------
}
}
}
}

#endif 

// ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~  ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~
