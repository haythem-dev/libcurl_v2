/*! $Id$                                          
*                                                  
* @file                                            
* @author Anke Klink                      
* @date 2008                            
*/    

#ifndef  __BASAR_TEST_UNIT_GUITIE_BASEMATCHERHOLDER_H__ 
#define  __BASAR_TEST_UNIT_GUITIE_BASEMATCHERHOLDER_H__ 

#include "libbasardbaspect_iaccessor.h"
#include "libbasardbaspect_accessor.h"
#include "libbasardbaspect_connectionref.h"

#include <map>

namespace basar
{
	namespace test
	{
		namespace unit
		{
			namespace guitie
			{
				/*!
				* Base class for Test classes revolving 
				* around Tests with one Matcher,
				* used in severall Testfixture classes 
				*/
				class BaseMatcherHolder
				{
				public:
					/*!
					* Specfies the test, that is  done for columns and rows
					* <ul>
					*   <li> NORM/EQUAL
					*       <ul>
					*          <li> columns: the columns in the AccessorpropertyTable and the Guipropertytable 
					*                        are defined identical (same names and types) </li>
					*          <li> rows: the numb er of rows in the Accessor PropertyTable and the Guiproperty table identical </li>
					*       </ul>
					*   </li>
					*   <li> MORE_GUI
					*       <ul>
					*          <li> columns: all the columns in the AccessorpropertyTable are identical 
					*                        defined in the Guipropertytable,
					*                        but there is one (or more) Columns in the Guipropertytable</li>
					*          <li> rows: there are more lines in the  Guipropertytable than in the Accessorpropertytable</li>
					*       </ul>
					*   </li>
					*   <li> MORE_ACC
					*       <ul>
					*          <li> columns: all the columns in the Guipropertytable are identical 
					*                        defined in the AccessorpropertyTable,
					*                        but there is one (or more) Columns in the AccessorpropertyTable</li>
					*          <li> rows: there are more lines in the  Guipropertytable than in the Accessorpropertytable</li>
					*       </ul>
					*   </li>
					*   <li> MORE_BOTH
					*       <ul>
					*          <li> columns: not all the columns in the Guipropertytable are  
					*                        defined in the AccessorpropertyTable,
					*                        there is one (or more) Columns defined in the AccessorpropertyTable and
					*                        one (or more) Columns in the Guipropertytable</li>
					*          <li> rows: not possible, as the rows are not named</li>
					*       </ul>
					*   </li>
					* </ul>
					*/
					enum TestNumber
					{
						NORM = 0,
						EQUAL = NORM,
						MORE_GUI,
						MORE_ACC,
						MORE_BOTH
					};
				protected:

					// Matcher: 
					basar::gui::tie::MatcherRef            m_matcher;          //!< Matcher, that is used in this Test
                    basar::db::aspect::IAccessorSharedPtr  m_accessor;		   //!< Accessor Instance, within the Matcher
					basar::db::aspect::ConnectionRef       m_connection;       //!< Connection, used by the Accessor
					QWidget                              * m_widget;           //!< Widget, used in the Matcher

					basar::VarString m_accessornameColTestNorm;                //!< Name of the Accessor, used in Test with m_testnumberColTest NORM
					basar::VarString m_accessornameColTestMoreAccNames;        //!< Name of the Accessor, used in Test with m_testnumberColTest MORE_ACC
					basar::VarString m_accessormethodnameColTestNorm;          //!< Name of the AccessorMethod, used in Test with m_testnumberColTest NORM
					basar::VarString m_accessormethodnameColTestMoreAccNames;  //!< Name of the AccessorMethod, used in Test with m_testnumberColTest MORE_ACC

                    typedef std::map<basar::VarString, basar::db::aspect::IAccessorSharedPtr> AccessorMap;

                    AccessorMap m_AccessorMap; //!< map holding all accessors needed for test scenarios

					/*!
					*  Specfies the actual test, that is done for columns :
					* <ul>
					*   <li> NORM/EQUAL: the columns in the AccessorpropertyTable and the Guipropertytable 
					*                        are defined identically (same names and types) </li>
					*        
					*   <li> MORE_GUI:  all the columns in the AccessorpropertyTable are identically 
					*                        defined in the Guipropertytable,
					*                        but there is one (or more) Columns in the Guipropertytable</li>
					*       
					*   <li> MORE_ACC: all the columns in the Guipropertytable are identically 
					*                        defined in the AccessorpropertyTable,
					*                        but there is one (or more) Columns in the AccessorpropertyTable</li>
					*   <li> MORE_BOTH: not all the columns in the Guipropertytable are  
					*                        defined in the AccessorpropertyTable,
					*                        there is one (or more) Columns defined in the AccessorpropertyTable and
					*                        one (or more) Columns in the Guipropertytable</li>
					* </ul>
					*/
					TestNumber m_testnumberColTest;
					/*!
					* Specfies the actual test, that is done as follows
					* <ul>
					*   <li> NORM/EQUAL: the number of rows in the Accessor PropertyTable and the Guiproperty table identical </li>     
					*   <li> MORE_GUI: there are more lines in the  Guipropertytable than in the Accessorpropertytable</li>
					*      
					*   <li> MORE_ACC: there are more lines in the  Guipropertytable than in the Accessorpropertytable</li>
					*   <li> MORE_BOTH: not possible, as the rows are not named</li>
					* </ul>
					*/
					TestNumber m_testnumberRowTest;

					/*!
					* help value, that indicates that the values 
					* (f.ex. Accessor) are to be reset
					*/
					bool m_needsReset;

				protected:

					/*!
					* activates the Connection, this is not done in the
					* Constructor, as the class is used in TEXT_FIXTURE
					* context, and the Constructor there is called 
					* outside of the 
					* TEXT_FIXTURE finction, and can therefore not be 
					* included between a try-Catch Statement
					*/
					void setConnection();

					/*!
					* cleans the  Accessor Instance
					*/
					bool cleanAccessor();

					/*! 
					* checks if all Values are initalised
					*/
					bool checkInitalisation();

				public:

					BaseMatcherHolder();
					~BaseMatcherHolder();

					/*! 
					* returns the Name of the active Accessor
					*/
					VarString getAccessorName();

					/*! 
					* returns the Name of the active AccessorMethod
					*/
					VarString getAccessorMethodName();

					/*! 
					* returns the Matcher Instance
					*/
					basar::gui::tie::MatcherRef & getMatcher();

					/*! 
					* returns the Widget, used for the Matcher
					*/
					QWidget * getGui();

					/*! 
					* returns the AccessorInstanceRef, used for the Matcher
					*/
					//basar::db::aspect::AccessorInstanceRef & getAccessor();

					/*!
					* returns the Test done on Columns
					* <ul>
					*   <li> NORM/EQUAL: the columns in the AccessorpropertyTable and the Guipropertytable 
					*                        are defined identically (same names and types) </li>
					*        
					*   <li> MORE_GUI:  all the columns in the AccessorpropertyTable are identically 
					*                        defined in the Guipropertytable,
					*                        but there is one (or more) Columns in the Guipropertytable</li>
					*       
					*   <li> MORE_ACC: all the columns in the Guipropertytable are identically 
					*                        defined in the AccessorpropertyTable,
					*                        but there is one (or more) Columns in the AccessorpropertyTable</li>
					*   <li> MORE_BOTH: not all the columns in the Guipropertytable are  
					*                        defined in the AccessorpropertyTable,
					*                        there is one (or more) Columns defined in the AccessorpropertyTable and
					*                        one (or more) Columns in the Guipropertytable</li>
					* </ul>
					*/
					TestNumber getColTestNumber() const;

					/*!
					* returns the Test done on rows
					* <ul>
					*   <li> NORM/EQUAL: the numb er of rows in the Accessor PropertyTable and the Guiproperty table identically </li>     
					*   <li> MORE_GUI: there are more lines in the  Guipropertytable than in the Accessorpropertytable</li>
					*      
					*   <li> MORE_ACC: there are more lines in the  Guipropertytable than in the Accessorpropertytable</li>
					*   <li> MORE_BOTH: not possible, as the rows are not named</li>
					* </ul>
					*/
					TestNumber getRowTestNumber()const;

					/*!
					* returns if there are columns in the Accessor Propertytable, that
					* are not in the Gui Propertytable
					*/ 
					bool hasMoreAccNamesTest()const;

					/*!
					* returns if there are columns in the Gui Propertytable, that
					* are not in the Accessor Propertytable
					*/
					bool hasMoreGuiNamesTest()const;

					/*!
					* returns if the Accessor Propertytable has more rows than the
					* Gui Propertytable
					*/
					bool hasMoreAccRowsTest()const;

					/*!
					* returns if the Gui Propertytable has more rows than the
					* Accessor Propertytable
					*/
					bool hasMoreGuiRowsTest()const;

					/*!
					* sets the Informations for the Accessor (Accessorname and 
					* AccessorMethod name)
					*/
					bool setAccessorInfo(
						const basar::VarString & accessornameNorm , 
						const basar::VarString & accessornameMoreAccNames,
						const basar::VarString & accessormethodnameNorm , 
						const basar::VarString & accessormethodnameMoreAccNames,
						bool useresetAccessorAndMatcher);

					/*!
					* rests the Accessor and the Matcher
					*/
					bool resetAccessorAndMatcher();  

					/** 
					* sets the Test Number for Column and Row Tests
					*/
					bool setTestNumber(TestNumber coltest , TestNumber rowtest=EQUAL);

					/**
					* starts with Test Numbers 
					*/
					bool start(TestNumber coltest , TestNumber rowtest );


					/**
					* virtual function for start
					*/
					virtual bool start();


					/**
					* virtual function for reset of the gui values
					* that are defined in subclasses
					*/
					virtual bool resetGuiValues();

					/**
					* calls the defined AccessorMethod on the Accessor
					*/
					bool select();

					/**
					* calls the defined AccessorMethod on the Accessor
					*/
					bool select(bool flagRemoveBeforeSelect);

					/**
					 * reset Matcher
					 */
					bool reset();
				};

			}
		}
	}
}
#endif 

// ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~  ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~
