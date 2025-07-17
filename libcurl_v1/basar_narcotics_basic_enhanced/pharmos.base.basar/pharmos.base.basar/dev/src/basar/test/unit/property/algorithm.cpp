/** $Id$                                          
*                                                  
* @file                                            
* @author Anke Klink                      
* @date 2008                            
*/    

#include <UnitTest++/UnitTest++.h>

#include "libbasarproperty.h"
#include "libbasarproperty.h"
#include <set>

#include "propertydescriptionlist.h"
#include "propertytable.h"
#include "loggerpool.h"

using  basar::property::PropertyDescriptionListRef;
using  basar::property::PropertyDescriptionList;
using  basar::property::PropertyDescription;
using  basar::property::PropertyType;
using  basar::property::PropertyDescriptionVector;
using  basar::property::PropertyTableRef;
using  basar::property::PropertyTable_YIterator;
using  basar::property::PropertyDescriptionArray;
using  basar::property::PropertyDescriptionVector;
using  basar::property::PropertyTableSharedPtr;
using  basar::Int32;
using  basar::VarString;
using  basar::ConstString;


//------------------------------------------------------------------------------
SUITE(property_algorithm)
{
	// in this test Suite used Property Descriptions 
	CONST_PROPERTY_DESCRIPTION(PROPDEF_COLINT32,      "colint32",      basar::INT32) 
		CONST_PROPERTY_DESCRIPTION(PROPDEF_COLSTRING1,    "colstring1",    basar::STRING)
		CONST_PROPERTY_DESCRIPTION(PROPDEF_COLSTRING2,    "colstring2",    basar::STRING)
		CONST_PROPERTY_DESCRIPTION(PROPDEF_COLSTRING3,    "colstring3",    basar::STRING)
		CONST_PROPERTY_DESCRIPTION(PROPDEF_COLSTRING4,    "colstring4",    basar::STRING)
		CONST_PROPERTY_DESCRIPTION(PROPDEF_COLSTRING5,    "colstring5",    basar::STRING)

		CONST_PROPERTY_DESCRIPTION(PROPDEF_NOCOLINT32,    "nocolint32",    basar::INT32)
		CONST_PROPERTY_DESCRIPTION(PROPDEF_NOCOLSTRING1,  "nocolstring1",  basar::STRING)
		CONST_PROPERTY_DESCRIPTION(PROPDEF_NOCOLSTRING2,  "nocolstring2",  basar::STRING)

		const PropertyDescriptionArray propertyDescriptionArrayColValues =  
	{
		PROPDEF_COLINT32, 
			PROPDEF_COLSTRING1, 
			PROPDEF_COLSTRING2,
			PROPDEF_COLSTRING3,
			PROPDEF_COLSTRING4,
			PROPDEF_COLSTRING5
	};

	const PropertyDescriptionArray propertyDescriptionArrayOtherValues =  
	{
		PROPDEF_NOCOLINT32, 
			PROPDEF_NOCOLSTRING1, 
			PROPDEF_NOCOLSTRING2
	};

	const bool ONLYONE = true;
	const bool NEEDSALL = false;

	/**
	* Helperclass, to check various  functions from
	* libbasarproperty_algorithm
	* the test is a check over all states (SupportedStateEnum enum)
	* on all Y-Iterator listet in the vector m_supportedStateYIteratorList
	* 
	*/
	struct Fixture
	{
		/**
		* A Helperclass around one Y-Iterator, holds the following
		* information, in order to know the suppossed testresults the tests
		* are checked against.
		* <ul>
		*    <li> m_yiter - Y-Itertator</li>
		*    <li> m_supportedState - Main supported state in this Y-Iterator</li>
		*    <li> m_insertType - Type used to create the Y-Iterator, f.ex. FOR_INSERT </li>
		*    <li> m_SupportedStateString - Name of the supported state (String) </li>
		*    <li> m_isSet - are the values in this Y-Iterator set</li>
		*    <li> m_supportedStates - all states that are set in this Y.Iterator 
		*         (for most tests that is only m_supportedState)
		*    </li>
		* </ul>
		*/
		struct SupportedStateEnumYIteratorHolder
		{
			basar::SupportedStateEnum m_supportedState;             //!< Main supported state in this Y-Iterator
			basar::InsertTypeEnum m_insertType;                     //!< Type used to create the Y-Iterator, f.ex. FOR_INSERT
			VarString m_SupportedStateString;                       //!< Name of the supported state (m_supportedState)
			bool m_isSet;                                           //!< are the values in this Y-Iterator set

			/**
			* all states that are set in this Y.Iterator 
			* (for most tests that is only m_supportedState)
			*/
			std::set< basar::SupportedStateEnum> m_supportedStates; 

			/**
			* The Y-Iterator
			*/
			PropertyTable_YIterator m_yiter;

			/** 
			* default Constructor
			*/
			SupportedStateEnumYIteratorHolder()
			{
				init( basar::SS_UNKNOWN, basar::FOR_UNKNOWN, "UNKNOWN" );
			}

			/**
			* used constructor
			*/
			SupportedStateEnumYIteratorHolder(
				PropertyTableRef        & proptab,     //!< PropertyTable, where one line is added
				basar::SupportedStateEnum state,       //!< Supported State
				basar::InsertTypeEnum     inserttype,  //!< Insert state
				ConstString               string,      //!< Name of the SupportedStateEnum
				bool                      setDefault   //!< shall the values be set to default values
				)
			{
				init(state, inserttype, string);
				addYIterator(proptab, setDefault);
			}

			/**
			*  the values are set
			*/
			void init ( 
				basar::SupportedStateEnum state, 
				basar::InsertTypeEnum     inserttype,
				ConstString               string )
			{
				m_supportedState = state;
				m_insertType = inserttype;
				m_SupportedStateString = string;
				m_supportedStates.insert(state);
				m_isSet = false; 
				m_yiter.reset();
			}

			/**
			* creates the Y-Iterator 
			*/
			void addYIterator(
				PropertyTableRef & proptab,  //!< PropertyTable, where one line is added
				bool setDefault              //!< shall the values be set to default values
				)
			{
				m_yiter = proptab.insert( m_insertType );
				m_isSet = setDefault;
				if (setDefault)
				{
					m_yiter.setInt32(PROPDEF_COLINT32.getName(), 0);
					m_yiter.setString(PROPDEF_COLSTRING1.getName(), "abc");
					m_yiter.setString(PROPDEF_COLSTRING2.getName(), "def");       
					m_yiter.setString(PROPDEF_COLSTRING3.getName(), "ghi");       
					m_yiter.setString(PROPDEF_COLSTRING4.getName(), "jkl");       
					m_yiter.setString(PROPDEF_COLSTRING5.getName(), "mno");       
				}
			}

			/**
			* writes the testinformation to a string
			*/
			VarString toString()const
			{
				VarString retval;
				retval.format("\"Test with %s (parameter %s)\"",
					m_SupportedStateString.c_str(),
					(m_isSet) ? "set" : "unset");
				return retval;
			}

			/**
			* checks if a type state2 will be recognized as valid
			* f.ex. SS_UNKNOWN is always recognized
			* in check of the Y-Iterator. This value is used as
			* Checkvalue for the algorithm functions that check
			* stated
			*/
			bool isSupposedToBeRecognizedAsValid(
				basar::SupportedStateEnum state2,  //!< the state to be checked
				bool one                           //!<needs only one state as equal (true) or all (false)
				)
			{
				bool retval = false;
				if (state2 == basar::SS_UNKNOWN)
				{
					retval = true;
				}
				else if (1 == m_supportedStates.size())
				{
					retval = (m_supportedState == state2);
				}
				else if (one)
				{
					std::set< basar::SupportedStateEnum>::const_iterator iter;
					for (iter = m_supportedStates.begin(); iter != m_supportedStates.end() && !retval; ++iter)
					{
						retval = (*iter == state2);
					}
				}
				return retval;
			}

			/**
			* add a PropertyState to the Y-Iterator
			*/
			void addPropertyState(const VarString & colname, basar::SupportedStateEnum state)
			{
				m_yiter.setPropertyState(colname, state);      
				m_supportedStates.insert(state);
			}

			/**
			* returns if more than one state is used in the Y-Iterator
			*/
			bool isMixed()
			{
				return (m_supportedStates.size() > 1);
			}

		};


		/**
		* A typedef for better readability
		*/
		typedef std::vector<SupportedStateEnumYIteratorHolder> SupportedStateEnumYIteratorHolderList;


		PropertyDescriptionListRef m_propertyDescriptionListRef;  //!< the Property Description
		PropertyTableRef m_propertytable;                         //!< the Property Table
		PropertyTable_YIterator m_yiter;
		PropertyTable_YIterator m_yiterNull;
		PropertyTable_YIterator m_yiterEnd;

		PropertyDescriptionVector m_propertyDescriptionVectorColValues;   //!< Vector of column definition
		PropertyDescriptionVector m_propertyDescriptionVectorOtherValues; //!< Vector of other column definition

		basar::cmnutil::ParameterList m_paramListColValues;     //!< List of Columns, that are used
		basar::cmnutil::ParameterList m_paramListOtherValues;   //!< List of other Columns, that aren't used
		basar::cmnutil::ParameterList m_paramListMoreValues;    //!< List of used Columns + one more value
		basar::cmnutil::ParameterList m_paramListOneValue;      //!< List of one used Column

		VarString m_stringColValues;                //!< List of Columns, that are used
		VarString m_stringMoreValues;               //!< List of other Columns, that aren't used
		VarString m_stringOtherValues;              //!< List of used Columns + one more value
		VarString m_stringOneValue;                 //!< List of one used Column
		VarString m_stringWithBlanksColValues;      //!< List of Columns, that are used
		VarString m_stringWithBlanksMoreValues;     //!< List of other Columns, that aren't used
		VarString m_stringWithBlanksOtherValues;    //!< List of used Columns + one more value
		VarString m_stringWithBlanksOneValue;      //!< List of one used Column

		/**
		* List of Y-Itertators for test
		*/
		SupportedStateEnumYIteratorHolderList m_supportedStateYIteratorList;

		/**
		* Map of all known SupportedStateEnum
		*/
		std::map< basar::SupportedStateEnum, VarString> m_supportedStates;

		/**
		* has an Error ocurred
		*/
		bool m_error;

		/**
		* default constructor, sets all the valiables used for tests
		*/
		Fixture():
		m_propertyDescriptionListRef(basar::property::PropertyDescriptionList::create() ),          
			m_propertytable( PropertyTableSharedPtr() ),
			m_propertyDescriptionVectorColValues(6,propertyDescriptionArrayColValues),
			m_propertyDescriptionVectorOtherValues(3,propertyDescriptionArrayOtherValues),
			m_paramListColValues(m_propertyDescriptionVectorColValues),
			m_paramListOtherValues(m_propertyDescriptionVectorOtherValues),
			m_paramListMoreValues(m_propertyDescriptionVectorColValues),
			m_error(false)
		{
			try
			{
				m_paramListMoreValues.push_back(PROPDEF_NOCOLINT32.getName());
				m_paramListOneValue.push_back(PROPDEF_COLINT32.getName());

				m_stringColValues   = "colint32;colstring1;colstring2;colstring3;colstring4;colstring5;";
				m_stringMoreValues  = "colint32;colstring1;colstring2;colstring3;colstring4;colstring5;nocolint32;";
				m_stringOtherValues = "nocolint32;nocolstring1;nocolstring2;";
				m_stringOneValue    = "colint32;";

				m_stringWithBlanksColValues   = "   colint32  ;  colstring1  ;  colstring2  ;  colstring3  ;  colstring4    ; colstring5 ; ";
				m_stringWithBlanksMoreValues  = "colint32  ;  colstring1  ;     colstring2  ;  colstring3  ;  colstring4    ; colstring5 ; nocolint32  ;   ";
				m_stringWithBlanksOtherValues = "  nocolint32  ;  nocolstring1  ;   nocolstring2;  ";
				m_stringWithBlanksOneValue    = "  colint32  ;  ";

				m_propertyDescriptionListRef.construct(m_propertyDescriptionVectorColValues);

				m_propertytable = PropertyTableRef( basar::property::PropertyTable::create(m_propertyDescriptionListRef) );

				m_supportedStates[basar::SS_UPDATE] = "UPDATE";
				m_supportedStates[basar::SS_INSERT] = "INSERT";
				m_supportedStates[basar::SS_UNKNOWN]= "UNKNOWN";
				m_supportedStates[basar::SS_CLEAN]  = "CLEAN";
				m_supportedStates[basar::SS_DELETE] = "DELETE";

				// list of tests once with default values, once unset
				bool setDefault = true;
				for  (int i=0; i<2; ++i)
				{
					// all SupportedStateEnum
					m_supportedStateYIteratorList.push_back( SupportedStateEnumYIteratorHolder(m_propertytable, basar::SS_INSERT,  basar::FOR_INSERT,  "INSERT",  setDefault));
					m_supportedStateYIteratorList.push_back( SupportedStateEnumYIteratorHolder(m_propertytable, basar::SS_UPDATE,  basar::FOR_UPDATE,  "UPDATE",  setDefault));
					m_supportedStateYIteratorList.push_back( SupportedStateEnumYIteratorHolder(m_propertytable, basar::SS_UNKNOWN, basar::FOR_UNKNOWN, "UNKNOWN", setDefault));
					m_supportedStateYIteratorList.push_back( SupportedStateEnumYIteratorHolder(m_propertytable, basar::SS_CLEAN,   basar::FOR_CLEAN,   "CLEAN",   setDefault));
					m_supportedStateYIteratorList.push_back( SupportedStateEnumYIteratorHolder(m_propertytable, basar::SS_DELETE,  basar::FOR_DELETE,  "DELETE",  setDefault));

					setDefault = false;
				}
				// one test with all states in one Y-Iterator
				{
					SupportedStateEnumYIteratorHolder a(m_propertytable, basar::SS_CLEAN,   basar::FOR_CLEAN,   "Diverse",  true);

					a.addPropertyState(PROPDEF_COLSTRING3.getName(), basar::SS_INSERT);     
					a.addPropertyState(PROPDEF_COLSTRING2.getName(), basar::SS_UPDATE);       
					a.addPropertyState(PROPDEF_COLSTRING1.getName(), basar::SS_DELETE);       
					m_supportedStateYIteratorList.push_back(a);
				}

				m_yiter = m_propertytable.begin();
				m_yiterEnd = m_propertytable.end();
			}
			catch (basar::Exception & ex)
			{
				std::cout << "Exception init Fixture :" << ex.what() << std::endl;
				m_error = true;
			}
		}

		~Fixture()
		{
		}

		/**
		* checks if an error has ocurred, used in
		* CHECK(check())
		*/
		bool check()
		{
			return !m_error;
		}


	};

	TEST_FIXTURE(Fixture,property_algorithm_arePropertiesSet)
	{
		CHECK(check());
		SupportedStateEnumYIteratorHolderList::iterator listiter;
		for (listiter = m_supportedStateYIteratorList.begin(); listiter != m_supportedStateYIteratorList.end(); ++listiter)
		{
			try
			{
				bool isSet = listiter->m_isSet;
				CHECK_EQUAL(isSet, basar::property::arePropertiesSet(listiter->m_yiter, m_paramListColValues));
				CHECK(!basar::property::arePropertiesSet(listiter->m_yiter, m_paramListOtherValues));
				CHECK(!basar::property::arePropertiesSet(listiter->m_yiter, m_paramListMoreValues));
				CHECK_EQUAL(isSet,basar::property::arePropertiesSet(listiter->m_yiter, m_paramListOneValue));

				CHECK_EQUAL(isSet,basar::property::arePropertiesSet(listiter->m_yiter, m_stringColValues));
				CHECK(!basar::property::arePropertiesSet(listiter->m_yiter, m_stringOtherValues));
				CHECK(!basar::property::arePropertiesSet(listiter->m_yiter, m_stringMoreValues));
				CHECK_EQUAL(isSet,basar::property::arePropertiesSet(listiter->m_yiter, m_stringOneValue));

				CHECK_EQUAL(isSet,basar::property::arePropertiesSet(listiter->m_yiter, m_stringWithBlanksColValues));
				CHECK(!basar::property::arePropertiesSet(listiter->m_yiter, m_stringWithBlanksOtherValues));
				CHECK(!basar::property::arePropertiesSet(listiter->m_yiter, m_stringWithBlanksMoreValues));
				CHECK_EQUAL(isSet,basar::property::arePropertiesSet(listiter->m_yiter, m_stringWithBlanksOneValue));
			}
			catch (basar::Exception & ex)
			{
				std::cout  << "Exception in " << listiter->toString() << " : "
					<< ex.what() << std::endl;
				const bool ExceptionOccured = false;
				CHECK(ExceptionOccured);

			}

		}

		try
		{
			CHECK_THROW(basar::property::arePropertiesSet(m_yiterNull, m_paramListColValues       ), basar::NullReferenceException);
			CHECK_THROW(basar::property::arePropertiesSet(m_yiterNull, m_stringColValues          ), basar::NullReferenceException);
			CHECK_THROW(basar::property::arePropertiesSet(m_yiterNull, m_stringWithBlanksColValues), basar::NullReferenceException);
		}
		catch (basar::Exception & ex)
		{
			std::cout  << ex.what() << std::endl;
			const bool ThrowExceptionOccured = false;
			CHECK(ThrowExceptionOccured);
			throw;
		}

	}

	TEST_FIXTURE(Fixture,property_algorithm_arePropertiesSetAndAllInState)
	{
		CHECK(check());
		// check all states:
		std::map< basar::SupportedStateEnum, VarString>::iterator stateiter;
		for (stateiter = m_supportedStates.begin(); stateiter != m_supportedStates.end(); ++stateiter)
		{
			basar::SupportedStateEnum state =  stateiter->first ;
			// check all y-iterators prepaired in Fixture:
			SupportedStateEnumYIteratorHolderList::iterator listiter;
			for (listiter = m_supportedStateYIteratorList.begin(); listiter != m_supportedStateYIteratorList.end(); ++listiter)
			{
				try
				{
					bool isSet  = listiter->m_isSet;
					bool isType = listiter->isSupposedToBeRecognizedAsValid(state, NEEDSALL);
					bool result = (isSet && isType);

					TEXTOUTPUT( "checking " << stateiter->second 
						<< " with " << listiter->toString()  
						<< " isType = " << isType
						<< " isSet = " << isSet
						<< " expectedresult = " << result
						<< std::endl)

					CHECK_EQUAL(result, basar::property::arePropertiesSetAndAllInState(listiter->m_yiter, m_paramListColValues, state));
					CHECK(!basar::property::arePropertiesSetAndAllInState(listiter->m_yiter, m_paramListOtherValues, state));
					CHECK(!basar::property::arePropertiesSetAndAllInState(listiter->m_yiter, m_paramListMoreValues, state));

					CHECK_EQUAL(result,basar::property::arePropertiesSetAndAllInState(listiter->m_yiter, m_stringColValues, state));
					CHECK(!basar::property::arePropertiesSetAndAllInState(listiter->m_yiter, m_stringOtherValues, state));
					CHECK(!basar::property::arePropertiesSetAndAllInState(listiter->m_yiter, m_stringMoreValues, state));

					CHECK_EQUAL(result,basar::property::arePropertiesSetAndAllInState(listiter->m_yiter, m_stringWithBlanksColValues, state));
					CHECK(!basar::property::arePropertiesSetAndAllInState(listiter->m_yiter, m_stringWithBlanksOtherValues, state));
					CHECK(!basar::property::arePropertiesSetAndAllInState(listiter->m_yiter, m_stringWithBlanksMoreValues, state));

					if (!listiter->isMixed())
					{
						CHECK_EQUAL(result,basar::property::arePropertiesSetAndAllInState(listiter->m_yiter, m_paramListOneValue, state));
						CHECK_EQUAL(result,basar::property::arePropertiesSetAndAllInState(listiter->m_yiter, m_stringOneValue, state));
						CHECK_EQUAL(result,basar::property::arePropertiesSetAndAllInState(listiter->m_yiter, m_stringWithBlanksOneValue, state));

					}
				}
				catch (basar::Exception & ex)
				{
					std::cout  << "Exception in check " << stateiter->second 
						<< " with " << listiter->toString() << " : "
						<< ex.what() << std::endl;
					const bool ExceptionOccured = false;
					CHECK(ExceptionOccured);
					//throw;
				}
			}

			try
			{
				CHECK_THROW(basar::property::arePropertiesSetAndAllInState(m_yiterNull, m_stringWithBlanksColValues, state), basar::NullReferenceException);
				CHECK_THROW(basar::property::arePropertiesSetAndAllInState(m_yiterNull, m_stringColValues          , state), basar::NullReferenceException);
				CHECK_THROW(basar::property::arePropertiesSetAndAllInState(m_yiterNull, m_paramListColValues       , state), basar::NullReferenceException);
			}
			catch (basar::Exception & ex)
			{
				std::cout 
					<< ex.what() << std::endl;
				const bool ThrowExceptionOccured = false;
				CHECK(ThrowExceptionOccured);
				//throw;
			}
		}
	}

	TEST_FIXTURE(Fixture,property_algorithm_arePropertiesSetAndOneMustBeInState)
	{
		// check all states:
		std::map< basar::SupportedStateEnum, VarString>::iterator stateiter;
		for (stateiter = m_supportedStates.begin(); stateiter != m_supportedStates.end(); ++stateiter)
		{
			basar::SupportedStateEnum state =  stateiter->first ;
			// check all y-iterators prepaired in Fixture:
			SupportedStateEnumYIteratorHolderList::iterator listiter;
			for (listiter = m_supportedStateYIteratorList.begin(); listiter != m_supportedStateYIteratorList.end(); ++listiter)
			{
				try
				{
					bool isSet  = listiter->m_isSet;
					bool isType = listiter->isSupposedToBeRecognizedAsValid(state, ONLYONE);
					bool result = (isSet && isType);

					CHECK_EQUAL(result, basar::property::arePropertiesSetAndOneMustBeInState(listiter->m_yiter, m_paramListColValues, state));
					CHECK(!basar::property::arePropertiesSetAndOneMustBeInState(listiter->m_yiter, m_paramListOtherValues, state));
					CHECK(!basar::property::arePropertiesSetAndOneMustBeInState(listiter->m_yiter, m_paramListMoreValues, state));

					CHECK_EQUAL(result,basar::property::arePropertiesSetAndOneMustBeInState(listiter->m_yiter, m_stringColValues, state));
					CHECK(!basar::property::arePropertiesSetAndOneMustBeInState(listiter->m_yiter, m_stringOtherValues, state));
					CHECK(!basar::property::arePropertiesSetAndOneMustBeInState(listiter->m_yiter, m_stringMoreValues, state));

					CHECK_EQUAL(result,basar::property::arePropertiesSetAndOneMustBeInState(listiter->m_yiter, m_stringWithBlanksColValues, state));
					CHECK(!basar::property::arePropertiesSetAndOneMustBeInState(listiter->m_yiter, m_stringWithBlanksOtherValues, state));
					CHECK(!basar::property::arePropertiesSetAndOneMustBeInState(listiter->m_yiter, m_stringWithBlanksMoreValues, state));

					if (!listiter->isMixed())
					{
						CHECK_EQUAL(result,basar::property::arePropertiesSetAndOneMustBeInState(listiter->m_yiter, m_paramListOneValue, state));
						CHECK_EQUAL(result,basar::property::arePropertiesSetAndOneMustBeInState(listiter->m_yiter, m_stringOneValue, state));
						CHECK_EQUAL(result,basar::property::arePropertiesSetAndOneMustBeInState(listiter->m_yiter, m_stringWithBlanksOneValue, state));
					}
				}
				catch (basar::Exception & ex)
				{
					std::cout  << "Exception in check " << stateiter->second 
						<< " with " << listiter->toString() << " : "
						<< ex.what() << std::endl;
					const bool ExceptionOccured = false;
					CHECK(ExceptionOccured);
					//throw;
				}
			}
			try
			{
				CHECK_THROW(basar::property::arePropertiesSetAndOneMustBeInState(m_yiterNull, m_stringWithBlanksColValues, state), basar::NullReferenceException);
				CHECK_THROW(basar::property::arePropertiesSetAndOneMustBeInState(m_yiterNull, m_stringColValues          , state), basar::NullReferenceException);
				CHECK_THROW(basar::property::arePropertiesSetAndOneMustBeInState(m_yiterNull, m_paramListColValues       , state), basar::NullReferenceException);
			}
			catch (basar::Exception & ex)
			{
				std::cout 
					<< ex.what() << std::endl;
				const bool ThrowExceptionOccured = false;
				CHECK(ThrowExceptionOccured);
				throw;
			}
		}
	}

	// isOnePropertyInState
	TEST_FIXTURE(Fixture,property_algorithm_isOnePropertyInState)
	{
		// check all states:
		std::map< basar::SupportedStateEnum, VarString>::iterator stateiter;
		for (stateiter = m_supportedStates.begin(); stateiter != m_supportedStates.end(); ++stateiter)
		{
			basar::SupportedStateEnum state =  stateiter->first ;
			// check all y-iterators prepaired in Fixture:
			SupportedStateEnumYIteratorHolderList::iterator listiter;
			for (listiter = m_supportedStateYIteratorList.begin(); listiter != m_supportedStateYIteratorList.end(); ++listiter)
			{
				try
				{
					bool isSet  = listiter->m_isSet;
					bool isType = listiter->isSupposedToBeRecognizedAsValid(state, ONLYONE);
					bool result = (isType && isSet);
					if (basar::SS_UNKNOWN == state )
					{
						result = true;
					}

					TEXTOUTPUT( "checking " << stateiter->second 
						<< " with " << listiter->toString()  
						<< " isType = " << isType
						<< " isSet = " << isSet
						<< " expectedresult = " << result
						<< std::endl)


						CHECK_EQUAL(result, basar::property::isOnePropertyInState(listiter->m_yiter, m_paramListColValues, state));
					CHECK(!basar::property::isOnePropertyInState(listiter->m_yiter, m_paramListOtherValues, state));
					CHECK_EQUAL(result,basar::property::isOnePropertyInState(listiter->m_yiter, m_paramListMoreValues, state));
					if (!listiter->isMixed())
					{
						CHECK_EQUAL(result,basar::property::isOnePropertyInState(listiter->m_yiter, m_paramListOneValue, state));
					}
				}
				catch (basar::Exception & ex)
				{
					std::cout  << "Exception in check " << stateiter->second 
						<< " with " << listiter->toString() << " : "
						<< ex.what() << std::endl;
					const bool ExceptionOccured = false;
					CHECK(ExceptionOccured);
					//throw;
				}
			}
			try
			{
				CHECK_THROW(basar::property::isOnePropertyInState(m_yiterNull, m_paramListColValues, state), basar::NullReferenceException);
			}
			catch (basar::Exception & ex)
			{
				std::cout 
					<< ex.what() << std::endl;
				const bool InnerExceptionOccured = false;
				CHECK(InnerExceptionOccured);
				throw;
			}
		}
	}

	TEST_FIXTURE(Fixture,property_algorithm_areKnownPropertiesInState)
	{
		// check all states:
		std::map< basar::SupportedStateEnum, VarString>::iterator stateiter;
		for (stateiter = m_supportedStates.begin(); stateiter != m_supportedStates.end(); ++stateiter)
		{
			basar::SupportedStateEnum state =  stateiter->first ;
			// check all y-iterators prepaired in Fixture:
			SupportedStateEnumYIteratorHolderList::iterator listiter;
			for (listiter = m_supportedStateYIteratorList.begin(); listiter != m_supportedStateYIteratorList.end(); ++listiter)
			{
				try
				{
					bool isSet  = listiter->m_isSet;
					bool isType = listiter->isSupposedToBeRecognizedAsValid(state, ONLYONE);
					bool isMixed = listiter->isMixed();
					bool result = (isType && isSet);
					if (isMixed)
					{
						result = false;
					}
					if (basar::SS_UNKNOWN == state )
					{
						result = true;
					}


					TEXTOUTPUT("checking " << stateiter->second 
						<< " with " << listiter->toString()  
						<< " isType = " << isType
						<< " isSet = " << isSet
						<< " expectedresult = " << result
						<< std::endl)

						bool acceptNoProperty = true;

					CHECK_EQUAL(result, basar::property::areKnownPropertiesInState(listiter->m_yiter, m_paramListColValues, state, acceptNoProperty )  );
					CHECK_EQUAL(acceptNoProperty,basar::property::areKnownPropertiesInState(listiter->m_yiter, m_paramListOtherValues, state, acceptNoProperty ) );
					CHECK_EQUAL(result,basar::property::areKnownPropertiesInState(listiter->m_yiter, m_paramListMoreValues, state, acceptNoProperty )  );

					acceptNoProperty = false;
					CHECK_EQUAL(result, basar::property::areKnownPropertiesInState(listiter->m_yiter, m_paramListColValues, state, acceptNoProperty )  );
					CHECK_EQUAL(acceptNoProperty,basar::property::areKnownPropertiesInState(listiter->m_yiter, m_paramListOtherValues, state, acceptNoProperty ) );
					CHECK_EQUAL(result,basar::property::areKnownPropertiesInState(listiter->m_yiter, m_paramListMoreValues, state, acceptNoProperty )  );

					if (!isMixed)
					{
						CHECK_EQUAL(result,basar::property::areKnownPropertiesInState(listiter->m_yiter, m_paramListOneValue, state, true )  );
						CHECK_EQUAL(result,basar::property::areKnownPropertiesInState(listiter->m_yiter, m_paramListOneValue, state, false )  );

					}
				}
				catch (basar::Exception & ex)
				{
					std::cout  << "Exception in check " << stateiter->second 
						<< " with " << listiter->toString() << " : "
						<< ex.what() << std::endl;
					const bool ExceptionOccured = false;
					CHECK(ExceptionOccured);
					//throw;
				}
			}
			try
			{
				CHECK_THROW(basar::property::areKnownPropertiesInState(m_yiterNull, m_paramListColValues, state, true), basar::NullReferenceException);
			}

			catch (basar::Exception & ex)
			{
				std::cout 
					<< ex.what() << std::endl;
				const bool InnerExceptionOccured = false;
				CHECK(InnerExceptionOccured);
				//throw;
			}
		}

	}

	TEST_FIXTURE(Fixture,property_algorithm_areKnownAndSetPropertiesInState)
	{
		// check all states:
		std::map< basar::SupportedStateEnum, VarString>::iterator stateiter;
		for (stateiter = m_supportedStates.begin(); stateiter != m_supportedStates.end(); ++stateiter)
		{
			basar::SupportedStateEnum state =  stateiter->first ;
			// check all y-iterators prepaired in Fixture:
			SupportedStateEnumYIteratorHolderList::iterator listiter;
			for (listiter = m_supportedStateYIteratorList.begin(); listiter != m_supportedStateYIteratorList.end(); ++listiter)
			{
				try
				{
					bool isSet  = listiter->m_isSet;
					bool isType = listiter->isSupposedToBeRecognizedAsValid(state, ONLYONE);
					bool isMixed = listiter->isMixed(); 
					bool result = (isType && isSet );
					if (isMixed)
					{
						result = false;
						if ( basar::SS_UNKNOWN == state )
						{
							result = isSet;
						}
					}


					bool acceptNoProperty = false;
					{ 
						TEXTOUTPUT( "checking " << stateiter->second 
							<< " with " << listiter->toString()  
							<< " isType = " << isType
							<< " isSet = " << isSet
							<< " acceptNoProperty = " << acceptNoProperty
							<< "mixed = " << isMixed
							<< " expectedresult = " << result
							<< std::endl)

							CHECK_EQUAL(result, basar::property::areKnownAndSetPropertiesInState(listiter->m_yiter, m_paramListColValues, state, acceptNoProperty )  );
						CHECK_EQUAL(acceptNoProperty,basar::property::areKnownAndSetPropertiesInState(listiter->m_yiter, m_paramListOtherValues, state, acceptNoProperty ) );
						CHECK_EQUAL(result,basar::property::areKnownAndSetPropertiesInState(listiter->m_yiter, m_paramListMoreValues, state, acceptNoProperty )  );
						if (!isMixed)
						{
							CHECK_EQUAL(result,basar::property::areKnownAndSetPropertiesInState(listiter->m_yiter, m_paramListOneValue, state, acceptNoProperty )  );
						}


					}

					acceptNoProperty = false;
					{    
						TEXTOUTPUT( "checking " << stateiter->second 
							<< " with " << listiter->toString()  
							<< " isType = " << isType
							<< " isSet = " << isSet
							<< " acceptNoProperty = " << acceptNoProperty
							<< " expectedresult = " << result
							<< std::endl)

							CHECK_EQUAL(result, basar::property::areKnownAndSetPropertiesInState(listiter->m_yiter, m_paramListColValues, state, acceptNoProperty )  );
						CHECK_EQUAL(acceptNoProperty,basar::property::areKnownAndSetPropertiesInState(listiter->m_yiter, m_paramListOtherValues, state, acceptNoProperty ) );
						CHECK_EQUAL(result,basar::property::areKnownAndSetPropertiesInState(listiter->m_yiter, m_paramListMoreValues, state, acceptNoProperty )  );
						if (!isMixed)
						{
							CHECK_EQUAL(result,basar::property::areKnownAndSetPropertiesInState(listiter->m_yiter, m_paramListOneValue, state, acceptNoProperty )  );
						}
					}
				}
				catch (basar::Exception & ex)
				{
					std::cout  << "Exception in check " << stateiter->second 
						<< " with " << listiter->toString() << " : "
						<< ex.what() << std::endl;
					const bool ExceptionOccured = false;
					CHECK(ExceptionOccured);
					//throw;
				}
			}
			try
			{
				CHECK_THROW(basar::property::areKnownAndSetPropertiesInState(m_yiterNull, 
																			 m_paramListColValues, 
																			 state, 
																			 true), 
						    basar::NullReferenceException);
			}

			catch (basar::Exception & ex)
			{
				std::cout 
					<< ex.what() << std::endl;
				const bool InnerExceptionOccured = false;
				CHECK(InnerExceptionOccured);
				throw;
			}
		}
	}

	// are1stPropertiesSetAnd2ndOneInState
	TEST_FIXTURE(Fixture,property_algorithm_are1stPropertiesSetAnd2ndOneInState)
	{
		// check all states:
		std::map< basar::SupportedStateEnum, VarString>::iterator stateiter;
		for (stateiter = m_supportedStates.begin(); stateiter != m_supportedStates.end(); ++stateiter)
		{
			basar::SupportedStateEnum state =  stateiter->first ;
			// check all y-iterators prepaired in Fixture:
			SupportedStateEnumYIteratorHolderList::iterator listiter;
			for (listiter = m_supportedStateYIteratorList.begin(); listiter != m_supportedStateYIteratorList.end(); ++listiter)
			{
				try
				{
					bool isSet  = listiter->m_isSet;
					bool isType = listiter->isSupposedToBeRecognizedAsValid(state, ONLYONE);
					bool result = (isSet && isType);

					TEXTOUTPUT( "checking " << stateiter->second 
						<< " with " << listiter->toString()  
						<< " isType = " << isType
						<< " isSet = " << isSet
						<< " expectedresult = " << result
						<< std::endl)

						const bool notset = false;
					const bool nottype = false;
					const bool notset_nottype = false;
					bool ismixed = listiter->isMixed();



					// m_paramListColValues :
					CHECK_EQUAL(result , basar::property::are1stPropertiesSetAnd2ndOneInState(listiter->m_yiter, m_paramListColValues, m_paramListColValues, state));
					CHECK_EQUAL(result , basar::property::are1stPropertiesSetAnd2ndOneInState(listiter->m_yiter, m_paramListColValues, m_paramListMoreValues, state));
					CHECK_EQUAL(nottype , basar::property::are1stPropertiesSetAnd2ndOneInState(listiter->m_yiter, m_paramListColValues, m_paramListOtherValues, state));
					if (!ismixed)
					{
						CHECK_EQUAL(result , basar::property::are1stPropertiesSetAnd2ndOneInState(listiter->m_yiter, m_paramListColValues, m_paramListOneValue, state));
					}

					// m_paramListMoreValues :
					CHECK_EQUAL(notset , basar::property::are1stPropertiesSetAnd2ndOneInState(listiter->m_yiter, m_paramListMoreValues, m_paramListColValues, state));
					CHECK_EQUAL(notset , basar::property::are1stPropertiesSetAnd2ndOneInState(listiter->m_yiter, m_paramListMoreValues, m_paramListMoreValues, state));
					CHECK_EQUAL(notset , basar::property::are1stPropertiesSetAnd2ndOneInState(listiter->m_yiter, m_paramListMoreValues, m_paramListOneValue, state));
					CHECK_EQUAL(notset_nottype , basar::property::are1stPropertiesSetAnd2ndOneInState(listiter->m_yiter, m_paramListMoreValues, m_paramListOtherValues, state));

					// m_paramListOneValue :
					CHECK_EQUAL(result , basar::property::are1stPropertiesSetAnd2ndOneInState(listiter->m_yiter, m_paramListOneValue, m_paramListColValues, state));
					CHECK_EQUAL(result , basar::property::are1stPropertiesSetAnd2ndOneInState(listiter->m_yiter, m_paramListOneValue, m_paramListMoreValues, state));
					if (!ismixed)
					{
						CHECK_EQUAL(result , basar::property::are1stPropertiesSetAnd2ndOneInState(listiter->m_yiter, m_paramListOneValue, m_paramListOneValue, state));
					}
					CHECK_EQUAL(nottype , basar::property::are1stPropertiesSetAnd2ndOneInState(listiter->m_yiter, m_paramListOneValue, m_paramListOtherValues, state));

					// m_paramListOtherValues :
					CHECK_EQUAL(notset , basar::property::are1stPropertiesSetAnd2ndOneInState(listiter->m_yiter, m_paramListOtherValues, m_paramListColValues, state));
					CHECK_EQUAL(notset , basar::property::are1stPropertiesSetAnd2ndOneInState(listiter->m_yiter, m_paramListOtherValues, m_paramListMoreValues, state));
					CHECK_EQUAL(notset , basar::property::are1stPropertiesSetAnd2ndOneInState(listiter->m_yiter, m_paramListOtherValues, m_paramListOneValue, state));
					CHECK_EQUAL(notset , basar::property::are1stPropertiesSetAnd2ndOneInState(listiter->m_yiter, m_paramListOtherValues, m_paramListOtherValues, state));

					// m_stringColValues :
					CHECK_EQUAL(result , basar::property::are1stPropertiesSetAnd2ndOneInState(listiter->m_yiter, m_stringColValues, m_stringColValues, state));
					CHECK_EQUAL(nottype , basar::property::are1stPropertiesSetAnd2ndOneInState(listiter->m_yiter, m_stringColValues, m_stringOtherValues, state));
					CHECK_EQUAL(result , basar::property::are1stPropertiesSetAnd2ndOneInState(listiter->m_yiter, m_stringColValues, m_stringMoreValues, state));

					CHECK_EQUAL(result , basar::property::are1stPropertiesSetAnd2ndOneInState(listiter->m_yiter, m_stringColValues, m_stringWithBlanksColValues, state));
					CHECK_EQUAL(nottype , basar::property::are1stPropertiesSetAnd2ndOneInState(listiter->m_yiter, m_stringColValues, m_stringWithBlanksOtherValues, state));
					CHECK_EQUAL(result , basar::property::are1stPropertiesSetAnd2ndOneInState(listiter->m_yiter, m_stringColValues, m_stringWithBlanksMoreValues, state));
					if (!ismixed)
					{
						CHECK_EQUAL(result , basar::property::are1stPropertiesSetAnd2ndOneInState(listiter->m_yiter, m_stringColValues, m_stringOneValue, state));
						CHECK_EQUAL(result , basar::property::are1stPropertiesSetAnd2ndOneInState(listiter->m_yiter, m_stringColValues, m_stringWithBlanksOneValue, state));
					}
					// m_stringOtherValues :
					CHECK_EQUAL(notset , basar::property::are1stPropertiesSetAnd2ndOneInState(listiter->m_yiter, m_stringOtherValues, m_stringColValues, state));
					CHECK_EQUAL(notset , basar::property::are1stPropertiesSetAnd2ndOneInState(listiter->m_yiter, m_stringOtherValues, m_stringOtherValues, state));
					CHECK_EQUAL(notset , basar::property::are1stPropertiesSetAnd2ndOneInState(listiter->m_yiter, m_stringOtherValues, m_stringMoreValues, state));
					CHECK_EQUAL(notset , basar::property::are1stPropertiesSetAnd2ndOneInState(listiter->m_yiter, m_stringOtherValues, m_stringOneValue, state));
					CHECK_EQUAL(notset , basar::property::are1stPropertiesSetAnd2ndOneInState(listiter->m_yiter, m_stringOtherValues, m_stringWithBlanksColValues, state));
					CHECK_EQUAL(notset , basar::property::are1stPropertiesSetAnd2ndOneInState(listiter->m_yiter, m_stringOtherValues, m_stringWithBlanksOtherValues, state));
					CHECK_EQUAL(notset , basar::property::are1stPropertiesSetAnd2ndOneInState(listiter->m_yiter, m_stringOtherValues, m_stringWithBlanksMoreValues, state));
					CHECK_EQUAL(notset , basar::property::are1stPropertiesSetAnd2ndOneInState(listiter->m_yiter, m_stringOtherValues, m_stringWithBlanksOneValue, state));

					// m_stringMoreValues :
					CHECK_EQUAL(notset , basar::property::are1stPropertiesSetAnd2ndOneInState(listiter->m_yiter, m_stringMoreValues, m_stringColValues, state));
					CHECK_EQUAL(notset , basar::property::are1stPropertiesSetAnd2ndOneInState(listiter->m_yiter, m_stringMoreValues, m_stringOtherValues, state));
					CHECK_EQUAL(notset , basar::property::are1stPropertiesSetAnd2ndOneInState(listiter->m_yiter, m_stringMoreValues, m_stringMoreValues, state));
					CHECK_EQUAL(notset , basar::property::are1stPropertiesSetAnd2ndOneInState(listiter->m_yiter, m_stringMoreValues, m_stringOneValue, state));
					CHECK_EQUAL(notset , basar::property::are1stPropertiesSetAnd2ndOneInState(listiter->m_yiter, m_stringMoreValues, m_stringWithBlanksColValues, state));
					CHECK_EQUAL(notset , basar::property::are1stPropertiesSetAnd2ndOneInState(listiter->m_yiter, m_stringMoreValues, m_stringWithBlanksOtherValues, state));
					CHECK_EQUAL(notset , basar::property::are1stPropertiesSetAnd2ndOneInState(listiter->m_yiter, m_stringMoreValues, m_stringWithBlanksMoreValues, state));
					CHECK_EQUAL(notset , basar::property::are1stPropertiesSetAnd2ndOneInState(listiter->m_yiter, m_stringMoreValues, m_stringWithBlanksOneValue, state));

					// m_stringOneValue :
					CHECK_EQUAL(result , basar::property::are1stPropertiesSetAnd2ndOneInState(listiter->m_yiter, m_stringOneValue, m_stringColValues, state));
					CHECK_EQUAL(nottype , basar::property::are1stPropertiesSetAnd2ndOneInState(listiter->m_yiter, m_stringOneValue, m_stringOtherValues, state));
					CHECK_EQUAL(result , basar::property::are1stPropertiesSetAnd2ndOneInState(listiter->m_yiter, m_stringOneValue, m_stringMoreValues, state));

					CHECK_EQUAL(result , basar::property::are1stPropertiesSetAnd2ndOneInState(listiter->m_yiter, m_stringOneValue, m_stringWithBlanksColValues, state));
					CHECK_EQUAL(nottype , basar::property::are1stPropertiesSetAnd2ndOneInState(listiter->m_yiter, m_stringOneValue, m_stringWithBlanksOtherValues, state));
					CHECK_EQUAL(result , basar::property::are1stPropertiesSetAnd2ndOneInState(listiter->m_yiter, m_stringOneValue, m_stringWithBlanksMoreValues, state));
					if (!ismixed)
					{
						CHECK_EQUAL(result , basar::property::are1stPropertiesSetAnd2ndOneInState(listiter->m_yiter, m_stringOneValue, m_stringOneValue, state));
						CHECK_EQUAL(result , basar::property::are1stPropertiesSetAnd2ndOneInState(listiter->m_yiter, m_stringOneValue, m_stringWithBlanksOneValue, state));
					}

					// m_stringWithBlanksColValues :
					CHECK_EQUAL(result , basar::property::are1stPropertiesSetAnd2ndOneInState(listiter->m_yiter, m_stringWithBlanksColValues, m_stringColValues, state));
					CHECK_EQUAL(nottype , basar::property::are1stPropertiesSetAnd2ndOneInState(listiter->m_yiter, m_stringWithBlanksColValues, m_stringOtherValues, state));
					CHECK_EQUAL(result , basar::property::are1stPropertiesSetAnd2ndOneInState(listiter->m_yiter, m_stringWithBlanksColValues, m_stringMoreValues, state));
					CHECK_EQUAL(result , basar::property::are1stPropertiesSetAnd2ndOneInState(listiter->m_yiter, m_stringWithBlanksColValues, m_stringWithBlanksColValues, state));
					CHECK_EQUAL(nottype , basar::property::are1stPropertiesSetAnd2ndOneInState(listiter->m_yiter, m_stringWithBlanksColValues, m_stringWithBlanksOtherValues, state));
					CHECK_EQUAL(result , basar::property::are1stPropertiesSetAnd2ndOneInState(listiter->m_yiter, m_stringWithBlanksColValues, m_stringWithBlanksMoreValues, state));
					if (!ismixed)
					{
						CHECK_EQUAL(result , basar::property::are1stPropertiesSetAnd2ndOneInState(listiter->m_yiter, m_stringWithBlanksColValues, m_stringOneValue, state));
						CHECK_EQUAL(result , basar::property::are1stPropertiesSetAnd2ndOneInState(listiter->m_yiter, m_stringWithBlanksColValues, m_stringWithBlanksOneValue, state));
					}

					// m_stringWithBlanksOtherValues :
					CHECK_EQUAL(notset , basar::property::are1stPropertiesSetAnd2ndOneInState(listiter->m_yiter, m_stringWithBlanksOtherValues, m_stringColValues, state));
					CHECK_EQUAL(notset , basar::property::are1stPropertiesSetAnd2ndOneInState(listiter->m_yiter, m_stringWithBlanksOtherValues, m_stringOtherValues, state));
					CHECK_EQUAL(notset , basar::property::are1stPropertiesSetAnd2ndOneInState(listiter->m_yiter, m_stringWithBlanksOtherValues, m_stringMoreValues, state));
					CHECK_EQUAL(notset , basar::property::are1stPropertiesSetAnd2ndOneInState(listiter->m_yiter, m_stringWithBlanksOtherValues, m_stringOneValue, state));
					CHECK_EQUAL(notset , basar::property::are1stPropertiesSetAnd2ndOneInState(listiter->m_yiter, m_stringWithBlanksOtherValues, m_stringWithBlanksColValues, state));
					CHECK_EQUAL(notset , basar::property::are1stPropertiesSetAnd2ndOneInState(listiter->m_yiter, m_stringWithBlanksOtherValues, m_stringWithBlanksOtherValues, state));
					CHECK_EQUAL(notset , basar::property::are1stPropertiesSetAnd2ndOneInState(listiter->m_yiter, m_stringWithBlanksOtherValues, m_stringWithBlanksMoreValues, state));
					CHECK_EQUAL(notset , basar::property::are1stPropertiesSetAnd2ndOneInState(listiter->m_yiter, m_stringWithBlanksOtherValues, m_stringWithBlanksOneValue, state));

					// m_stringWithBlanksMoreValues :
					CHECK_EQUAL(notset , basar::property::are1stPropertiesSetAnd2ndOneInState(listiter->m_yiter, m_stringWithBlanksMoreValues, m_stringColValues, state));
					CHECK_EQUAL(notset , basar::property::are1stPropertiesSetAnd2ndOneInState(listiter->m_yiter, m_stringWithBlanksMoreValues, m_stringOtherValues, state));
					CHECK_EQUAL(notset , basar::property::are1stPropertiesSetAnd2ndOneInState(listiter->m_yiter, m_stringWithBlanksMoreValues, m_stringMoreValues, state));
					CHECK_EQUAL(notset , basar::property::are1stPropertiesSetAnd2ndOneInState(listiter->m_yiter, m_stringWithBlanksMoreValues, m_stringOneValue, state));
					CHECK_EQUAL(notset , basar::property::are1stPropertiesSetAnd2ndOneInState(listiter->m_yiter, m_stringWithBlanksMoreValues, m_stringWithBlanksColValues, state));
					CHECK_EQUAL(notset , basar::property::are1stPropertiesSetAnd2ndOneInState(listiter->m_yiter, m_stringWithBlanksMoreValues, m_stringWithBlanksOtherValues, state));
					CHECK_EQUAL(notset , basar::property::are1stPropertiesSetAnd2ndOneInState(listiter->m_yiter, m_stringWithBlanksMoreValues, m_stringWithBlanksMoreValues, state));
					CHECK_EQUAL(notset , basar::property::are1stPropertiesSetAnd2ndOneInState(listiter->m_yiter, m_stringWithBlanksMoreValues, m_stringWithBlanksOneValue, state));

					// m_stringWithBlanksOneValue :
					CHECK_EQUAL(result , basar::property::are1stPropertiesSetAnd2ndOneInState(listiter->m_yiter, m_stringWithBlanksOneValue, m_stringColValues, state));
					CHECK_EQUAL(nottype , basar::property::are1stPropertiesSetAnd2ndOneInState(listiter->m_yiter, m_stringWithBlanksOneValue, m_stringOtherValues, state));
					CHECK_EQUAL(result , basar::property::are1stPropertiesSetAnd2ndOneInState(listiter->m_yiter, m_stringWithBlanksOneValue, m_stringMoreValues, state));
					CHECK_EQUAL(result , basar::property::are1stPropertiesSetAnd2ndOneInState(listiter->m_yiter, m_stringWithBlanksOneValue, m_stringWithBlanksColValues, state));
					CHECK_EQUAL(nottype , basar::property::are1stPropertiesSetAnd2ndOneInState(listiter->m_yiter, m_stringWithBlanksOneValue, m_stringWithBlanksOtherValues, state));
					CHECK_EQUAL(result , basar::property::are1stPropertiesSetAnd2ndOneInState(listiter->m_yiter, m_stringWithBlanksOneValue, m_stringWithBlanksMoreValues, state));
					if (!ismixed)
					{
						CHECK_EQUAL(result , basar::property::are1stPropertiesSetAnd2ndOneInState(listiter->m_yiter, m_stringWithBlanksOneValue, m_stringOneValue, state));
						CHECK_EQUAL(result , basar::property::are1stPropertiesSetAnd2ndOneInState(listiter->m_yiter, m_stringWithBlanksOneValue, m_stringWithBlanksOneValue, state));
					}
				}
				catch (basar::Exception & ex)
				{
					std::cout  << "Exception in check " << stateiter->second 
						<< " with " << listiter->toString() << " : "
						<< ex.what() << std::endl;
					const bool ExceptionOccured = false;
					CHECK(ExceptionOccured);
					//throw;
				}
			}
			try
			{
				// CHECK_THROW(basar::property::are1stPropertiesSetAnd2ndOneInState(m_yiterNull, m_paramListColValues, m_paramListColValues, state), basar::InvalidIteratorException);
				// CHECK_THROW(basar::property::are1stPropertiesSetAnd2ndOneInState(m_yiterNull, m_stringWithBlanksColValues, m_stringWithBlanksColValues, state), basar::InvalidIteratorException);
				// CHECK_THROW(basar::property::are1stPropertiesSetAnd2ndOneInState(m_yiterNull, m_stringColValues, m_stringColValues, state), basar::InvalidIteratorException);
			}
			catch (basar::Exception & ex)
			{
				std::cout 
					<< ex.what() << std::endl;
				const bool ExceptionOccured = false;
				CHECK(ExceptionOccured);
				throw;
			}
		}
	}

}
// ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~  ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~
