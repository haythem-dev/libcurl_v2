/** $Id$                                          
*                                                  
* @file                                            
* @author Anke Klink                      
* @date 2008                            
*/ 

#include <UnitTest++/UnitTest++.h>
#include "libbasarguitie.h"

#include "libbasarproperty_propertydescriptionlistref.h"
#include "libbasardbaspect_connectionref.h"
#include "libbasardbaspect_accessorpropertytable_yiterator.h"

#include "testconnection.h"
#include "buttablepropertydescriptions.h"
#include "buttablepropertydescriptionvectors.h"
#include "accessornames.h"
#include "basematcherholder.h"
#include "basematcherholdertestszenario.h"
#include "basematcherholdertestszenariolist.h"
#include "indexcountholder.h"
#include "matcherholderlist.h"
#include "macros.h"
#include "tablewidgetmatcherholder.h"
#include "loggerpool.h"

#include "converttostringmockcessor.h"
#include <boost/make_shared.hpp>

using basar::ConstString;
using basar::VarString;
using basar::Int32;
using basar::Float64;
using basar::Float32;
using basar::Decimal;

using basar::gui::tie::MatcherRef;
using basar::gui::tie::GuiPropertyTable_YIterator;
using basar::gui::tie::GuiPropertyTableRef;

using basar::db::aspect::AccessorInstanceRef;
using basar::db::aspect::ConnectionRef;
using basar::db::aspect::AccessorPropertyTable_YIterator;
using basar::db::aspect::AccessorPropertyTableRef;
using basar::db::aspect::ExecuteResultInfo;

using basar::property::PropertyDescription;

using basar::test::unit::guitie::TestConnection;
using basar::test::unit::guitie::BaseMatcherHolder;
using basar::test::unit::guitie::BaseMatcherHolderTestSzenario;
using basar::test::unit::guitie::MatcherHolderList;
using basar::test::unit::guitie::Direction;

using basar::test::unit::guitie::BaseMatcherHolderTestSzenarioList;
using basar::test::unit::guitie::IndexCountHolder;
using basar::test::unit::guitie::TableWidgetMatcherHolder;

extern bool g_RUNSHORT;


//------------------------------------------------------------------------------

//#define TEXTOUTPUT(XXX) std::cout << XXX << std::endl; 
//#define TEXTOUTPUT2(XXX) std::cout << XXX << std::endl;

SUITE(GuiTieTest_Matcher_ConvertToStrings)
{
	typedef basar::gui::tie::MatcherPolicyType MatcherPolicyType;

	static const VarString STRING1 = PROPDEF_STRING1.getName();
	static const VarString STRING2 = PROPDEF_STRING2.getName();

	static ConstString normString[] = {"1621" ,"1622" ,"1623" ,"1624" ,"1625" ,"1626" ,"1627" ,"1628" ,"1629"  };
	static Int32       normInt   [] = { 1621  , 1622  , 1623  , 1624  , 1625  , 1626  , 1627  , 1628  , 1629   };
	static Float64     normFloat [] = { 1621.0, 1622.0, 1623.0, 1624.0, 1625.0, 1626.0, 1627.0, 1628.0, 1629.0 };

	static ConstString dateString[] = 
	{
		"01.01.2008", 
			"02.02.2008",
			"03.03.2008",
			"04.04.2008",
			"05.05.2008",
			"06.06.2008",
			"07.07.2008",
			"08.08.2008",
			"09.09.2008"
	};
	static ConstString dateTimeString[] =  
	{
		"01.01.2008 01:01:01.000",
			"02.02.2008 02:02:02.000",
			"03.03.2008 01:01:01.000",
			"04.04.2008 01:01:01.000",
			"05.05.2008 01:01:01.000",
			"06.06.2008 01:01:01.000",
			"07.07.2008 01:01:01.000",
			"08.08.2008 01:01:01.000",
			"09.09.2008 01:01:01.000"
	};
	static ConstString timeString[] =
	{
		"01:01:01.000",
			"02:01:01.000",
			"03:01:01.000",
			"04:01:01.000",
			"05:01:01.000",
			"06:01:01.000",
			"07:01:01.000",
			"08:01:01.000",
			"09:01:01.000"
	};

	// --------------------------------------------------------------------------
	class ConvertToStringMatcherHolder : public TableWidgetMatcherHolder
	{    

		double m_delta;

	protected:

		int m_string1Number;
		int m_string2Number;
		int m_dateStringNumber;
		int m_dateTime1StringNumber;
		int m_dateTime2StringNumber;
		int m_timeStringNumber;


	public:

		ConvertToStringMatcherHolder():
			  m_dateStringNumber(14),
			  m_dateTime1StringNumber(15),
			  m_dateTime2StringNumber(16),
			  m_timeStringNumber(17),
			  m_delta(0.001) 
		  { 
			  m_guiColCountNorm =  static_cast<int>(propDescVecButTableDescAll.m_vector.size());
			  m_guiColCount = m_guiColCountNorm;

			  m_string1Number = 0;
			  m_string2Number =  1;

			  QStringList list;
			  QStringList list2;
			  std::vector< PropertyDescription>::const_iterator iter = propDescVecButTableDescAll.m_vector.begin();
			  for ( ; iter != propDescVecButTableDescAll.m_vector.end(); ++iter)
			  {
				  list << iter->getName().c_str();
			  }
			  list2 << "unknowncolumntest";
			  setStringListNames(list, list2);

              basar::db::aspect::IAccessorSharedPtr acc = boost::make_shared<ConvertToStringMockCessor>(ACCESSORNAME_ButMasterTableReader_All);

              m_AccessorMap.insert(AccessorMap::value_type(ACCESSORNAME_ButMasterTableReader_All, acc));

              acc = boost::make_shared<ConvertToStringMockCessor>(ACCESSORNAME_ButMasterTableReader_All_Plus);

              m_AccessorMap.insert(AccessorMap::value_type(ACCESSORNAME_ButMasterTableReader_All_Plus, acc));
		  }

		  ~ConvertToStringMatcherHolder()
		  {
		  }

		  basar::ConstString getText(int row, int col)
		  {    
			  if (col == m_dateStringNumber)
			  {
				  return dateString[row];
			  }
			  else if (col == m_dateTime1StringNumber || col == m_dateTime2StringNumber)
			  {
				  return dateTimeString[row];
			  }
			  else if (col ==  m_timeStringNumber)
			  {
				  return timeString[row];
			  }
			  return normString[row];
		  }

		  bool setAccessorInfo( )
		  { 
			  return BaseMatcherHolder::setAccessorInfo( 
				  ACCESSORNAME_ButMasterTableReader_All,
				  ACCESSORNAME_ButMasterTableReader_All_Plus,
				  ACCESSORMETHODNAME_SELECT_MORELINES,
				  ACCESSORMETHODNAME_SELECT_MORELINES,
				  false);
		  }

		  Int32 readInt32(const VarString & v)
		  {
			  basar::Int32 retval;
			  TEXTOUTPUT2( "v =XXX" << v <<"XXX");
			  if (v.empty())
			  {
				  retval = 0;
			  }
			  else
			  {
				  std::istringstream istr(v.c_str());
				  istr >> retval;
				  if (istr.fail())
				  {
					  retval = -1; 
				  }
			  }
			  return retval;
		  }

		  Float64 readFloat(const VarString & value)
		  {
			  return value.stof();
		  }

		  Decimal readDec(const VarString & value)
		  {
			  return Decimal(value);
		  }


		  void checkGuiEqualToEmpty(
			  GuiPropertyTable_YIterator & guiiter )
		  {
			  CHECK_EQUAL ("", guiiter.getString(PROPDEF_COLSER.getName()));
			  CHECK_EQUAL ("", guiiter.getString(PROPDEF_COLINT.getName()));
			  CHECK_EQUAL ("", guiiter.getString(PROPDEF_COLSMINT.getName()));
			  CHECK_EQUAL ("", guiiter.getString(PROPDEF_COLSMFL.getName()));
			  CHECK_EQUAL ("", guiiter.getString(PROPDEF_COLFL.getName()));
			  CHECK_EQUAL ("", guiiter.getString(PROPDEF_COLDEC.getName()));
			  CHECK_EQUAL ("", guiiter.getString(PROPDEF_COLMON.getName()));

		  }

		  void checkGuiEqualToText(
			  int row,
			  GuiPropertyTable_YIterator & guiiter , 
			  bool stringsToo)
		  {
			  CHECK_EQUAL (normInt[row]             , readInt32(guiiter.getString(PROPDEF_COLSER  .getName())));
			  CHECK_EQUAL (normInt[row]             , readInt32(guiiter.getString(PROPDEF_COLINT  .getName())));
			  CHECK_EQUAL (normInt[row]             , readInt32(guiiter.getString(PROPDEF_COLSMINT.getName())));
			  CHECK_CLOSE (normFloat[row]           , readFloat(guiiter.getString(PROPDEF_COLSMFL .getName())), m_delta);
			  CHECK_CLOSE (normFloat[row]           , readFloat(guiiter.getString(PROPDEF_COLFL   .getName())), m_delta);
			  CHECK       (Decimal(normFloat[row]) == readDec  (guiiter.getString(PROPDEF_COLDEC  .getName())));
			  CHECK       (Decimal(normFloat[row]) == readDec  (guiiter.getString(PROPDEF_COLMON  .getName())));

			  if (stringsToo)
			  {
				  CHECK_EQUAL(normString[row], guiiter.getString(PROPDEF_COLCH1.getName()));
				  CHECK_EQUAL(normString[row], guiiter.getString(PROPDEF_COLCH2.getName()));
				  CHECK_EQUAL(normString[row], guiiter.getString(PROPDEF_COLVCH1.getName()));
				  CHECK_EQUAL(normString[row], guiiter.getString(PROPDEF_COLVCH2.getName()));
				  CHECK_EQUAL(normString[row], guiiter.getString(PROPDEF_COLVCH3.getName()));
				  CHECK_EQUAL(normString[row], guiiter.getString(PROPDEF_COLVCH4.getName()));
				  CHECK_EQUAL(normString[row], guiiter.getString(PROPDEF_COLLVCH.getName()));
			  }

		  }

		  void checkAccEqualToText(
			  int row,
			  AccessorPropertyTable_YIterator & acciter ,
			  bool stringsToo)
		  {

			  CHECK_EQUAL (normInt[row]             , acciter.getInt32  (PROPDEF_COLSER  .getName()));
			  CHECK_EQUAL (normInt[row]             , acciter.getInt32  (PROPDEF_COLINT  .getName()));
			  CHECK_EQUAL (normInt[row]             , acciter.getInt16  (PROPDEF_COLSMINT.getName()));
			  CHECK_CLOSE (normFloat[row]           , acciter.getFloat32(PROPDEF_COLSMFL .getName()), m_delta);
			  CHECK_CLOSE (normFloat[row]           , acciter.getFloat64(PROPDEF_COLFL   .getName()), m_delta);
			  CHECK       (Decimal(normFloat[row]) == acciter.getDecimal(PROPDEF_COLDEC  .getName()));
			  CHECK       (Decimal(normFloat[row]) == acciter.getDecimal(PROPDEF_COLMON  .getName()));

			  if (stringsToo)
			  {
				  CHECK_EQUAL(normString[row], acciter.getString(PROPDEF_COLCH1.getName()));
				  CHECK_EQUAL(normString[row], acciter.getString(PROPDEF_COLCH2.getName()));
				  CHECK_EQUAL(normString[row], acciter.getString(PROPDEF_COLVCH1.getName()));
				  CHECK_EQUAL(normString[row], acciter.getString(PROPDEF_COLVCH2.getName()));
				  CHECK_EQUAL(normString[row], acciter.getString(PROPDEF_COLVCH3.getName()));
				  CHECK_EQUAL(normString[row], acciter.getString(PROPDEF_COLVCH4.getName()));
				  CHECK_EQUAL(normString[row], acciter.getString(PROPDEF_COLLVCH.getName()));
			  }

		  }


		  void checkAccNotEqualToTest(
			  int row,
			  AccessorPropertyTable_YIterator & acciter ,
			  bool stringsToo)
		  {
			  CHECK (normInt[row]           != acciter.getInt32  (PROPDEF_COLSER.getName  ()) );
			  CHECK (normInt[row]           != acciter.getInt32  (PROPDEF_COLINT.getName  ()) );
			  CHECK (normInt[row]           != acciter.getInt16  (PROPDEF_COLSMINT.getName()) );
			  CHECK (normFloat[row]         != acciter.getFloat32(PROPDEF_COLSMFL.getName ()) );
			  CHECK (normFloat[row]         != acciter.getFloat64(PROPDEF_COLFL.getName   ()) );
			  CHECK (Decimal(normFloat[row])!= acciter.getDecimal(PROPDEF_COLDEC.getName  ()) );
			  CHECK (Decimal(normFloat[row])!= acciter.getDecimal(PROPDEF_COLMON.getName  ()) );

			  if (stringsToo)
			  {
				  CHECK(normString[row]!= acciter.getString(PROPDEF_COLCH1.getName()));
				  CHECK(normString[row]!= acciter.getString(PROPDEF_COLCH2.getName()));
				  CHECK(normString[row]!= acciter.getString(PROPDEF_COLVCH1.getName()));
				  CHECK(normString[row]!= acciter.getString(PROPDEF_COLVCH2.getName()));
				  CHECK(normString[row]!= acciter.getString(PROPDEF_COLVCH3.getName()));
				  CHECK(normString[row]!= acciter.getString(PROPDEF_COLVCH4.getName()));
				  CHECK(normString[row]!= acciter.getString(PROPDEF_COLLVCH.getName()));
			  }

		  }


		  void checkEqual( 
			  Direction::DirEnum dir, 
			  int row,
			  GuiPropertyTable_YIterator & guiiter , 
			  AccessorPropertyTable_YIterator & acciter ,
			  BaseMatcherHolderTestSzenario & test,
			  bool needsCheckExtryPolicy = false,
			  MatcherPolicyType policy = 0 )
		  {
			  // strings       
			  CHECK_EQUAL_DIRECTION(dir,acciter.getString(PROPDEF_COLCH1 .getName()), guiiter.getString(PROPDEF_COLCH1 .getName()));
			  CHECK_EQUAL_DIRECTION(dir,acciter.getString(PROPDEF_COLCH2 .getName()), guiiter.getString(PROPDEF_COLCH2 .getName()));
			  CHECK_EQUAL_DIRECTION(dir,acciter.getString(PROPDEF_COLVCH1.getName()), guiiter.getString(PROPDEF_COLVCH1.getName()));
			  CHECK_EQUAL_DIRECTION(dir,acciter.getString(PROPDEF_COLVCH2.getName()), guiiter.getString(PROPDEF_COLVCH2.getName()));
			  CHECK_EQUAL_DIRECTION(dir,acciter.getString(PROPDEF_COLVCH3.getName()), guiiter.getString(PROPDEF_COLVCH3.getName()));
			  CHECK_EQUAL_DIRECTION(dir,acciter.getString(PROPDEF_COLVCH4.getName()), guiiter.getString(PROPDEF_COLVCH4.getName()));
			  CHECK_EQUAL_DIRECTION(dir,acciter.getString(PROPDEF_COLLVCH.getName()), guiiter.getString(PROPDEF_COLLVCH.getName()));

			  bool hasConvertType = false;
			  if (needsCheckExtryPolicy)
			  {
				  hasConvertType = test.checkPolicy(policy, basar::gui::tie::CONVERTTYPE);
			  }
			  else
			  {
				  hasConvertType = test.checkPolicy(basar::gui::tie::CONVERTTYPE);
			  }
			  if (hasConvertType)
			  {
				  CHECK_EQUAL_DIRECTION(dir, acciter.getInt32           (PROPDEF_COLSER  .getName()) , 
					                         readInt32(guiiter.getString(PROPDEF_COLSER  .getName())));
				  CHECK_EQUAL_DIRECTION(dir, acciter.getInt32           (PROPDEF_COLINT  .getName()) , 
					                         readInt32(guiiter.getString(PROPDEF_COLINT  .getName())));
				  CHECK_EQUAL_DIRECTION(dir, acciter.getInt16           (PROPDEF_COLSMINT.getName()) , 
					                         readInt32(guiiter.getString(PROPDEF_COLSMINT.getName())));
				  CHECK_CLOSE_DIRECTION(dir, acciter.getFloat32         (PROPDEF_COLSMFL .getName()) , 
					                         readFloat(guiiter.getString(PROPDEF_COLSMFL .getName())), m_delta);
				  CHECK_CLOSE_DIRECTION(dir, acciter.getFloat64         (PROPDEF_COLFL   .getName()) , 
					                         readFloat(guiiter.getString(PROPDEF_COLFL   .getName())), m_delta);
				  CHECK_DIRECTION      (dir, acciter.getDecimal         (PROPDEF_COLDEC  .getName()) , 
					                         readDec  (guiiter.getString(PROPDEF_COLDEC  .getName())));
				  CHECK_DIRECTION      (dir, acciter.getDecimal         (PROPDEF_COLMON  .getName()) , 
					                         readDec  (guiiter.getString(PROPDEF_COLMON  .getName())));
			  }
			  else if (basar::test::unit::guitie::Direction::RightToLeft == dir)
			  {
				  if (row < m_guiRowCount)
				  {
					  checkGuiEqualToText(row, guiiter,false);
				  }
				  else
				  {
					  checkGuiEqualToEmpty(guiiter);
				  }
			  }
			  /*            else if (basar::test::unit::guitie::Direction::LeftToRight == dir)
			  {
			  CHECK(acciter.getInt32(PROPDEF_COLSER.getName()) != readInt32(guiiter.getString(PROPDEF_COLSER.getName())));
			  CHECK(acciter.getInt32(PROPDEF_COLINT.getName())!= readInt32(guiiter.getString(PROPDEF_COLINT.getName())));
			  CHECK(acciter.getInt16(PROPDEF_COLSMINT.getName())!= readInt32(guiiter.getString(PROPDEF_COLSMINT.getName())));
			  CHECK(acciter.getFloat32(PROPDEF_COLSMFL.getName())!= readFloat(guiiter.getString(PROPDEF_COLSMFL.getName())) );
			  CHECK(acciter.getFloat64(PROPDEF_COLFL.getName())!= readFloat(guiiter.getString(PROPDEF_COLFL.getName())));
			  CHECK(acciter.getDecimal(PROPDEF_COLDEC.getName()).toFloat64()!= readFloat(guiiter.getString(PROPDEF_COLDEC.getName())) );
			  CHECK(acciter.getDecimal(PROPDEF_COLMON.getName()).toFloat64()!= readFloat(guiiter.getString(PROPDEF_COLMON.getName())) );
			  }*/

			  if (basar::test::unit::guitie::Direction::LeftToRight == dir)
			  {

			  }

		  }
	};

	// --------------------------------------------------------------------------
	TEST_FIXTURE(ConvertToStringMatcherHolder, GuiTieTest_Matcher_ConvertToStrings_Start)
	{
		BLOG_TRACE_METHOD(basar::test::unit::guitie::LoggerPool::logPerf, "GuiTieTest_Matcher_ConvertToStrings_Start");

		std::vector<TableWidgetKind> tableWidgetKindVec = getTableWidgetKindVector();
		std::vector<TableWidgetKind>::const_iterator tableWidgetKindIter;

		for(tableWidgetKindIter = tableWidgetKindVec.begin(); tableWidgetKindIter != tableWidgetKindVec.end(); ++tableWidgetKindIter)
		{
			try
			{
				CHECK(initTableWidgetKind(*tableWidgetKindIter));
				GuiPropertyTableRef guiprop = getMatcher().getLeft();
				AccessorPropertyTableRef accprop = getMatcher().getRight();
				GuiPropertyTable_YIterator guiiter = guiprop.begin();
				AccessorPropertyTable_YIterator acciter =accprop.begin();

				CHECK_EQUAL(0, static_cast<int>(accprop.size()));
				CHECK_EQUAL(0, static_cast<int>(guiprop.size())); 

				CHECK(guiiter.isEnd());
				CHECK(acciter.isEnd());
				CHECK(acciter.isBegin());
				CHECK(guiiter.isBegin());
				CHECK(!acciter.isNull());
				CHECK(!guiiter.isNull());

				CHECK(select());

				guiprop = getMatcher().getLeft();
				accprop = getMatcher().getRight();
				guiiter = guiprop.begin();
				acciter = accprop.begin();

				CHECK_EQUAL(m_accRowCount, static_cast<int>(accprop.size()));
				CHECK_EQUAL(0,             static_cast<int>(guiprop.size()));
				CHECK(guiiter.isEnd());
				CHECK(!acciter.isEnd());
				CHECK(acciter.isBegin());
				CHECK(guiiter.isBegin());
				CHECK(!acciter.isNull());
				CHECK(!acciter.isNull());

				resetGuiValues();

				guiprop = getMatcher().getLeft();
				accprop = getMatcher().getRight();
				guiiter = guiprop.begin();
				acciter = accprop.begin();

				CHECK_EQUAL(m_accRowCount, static_cast<int>(accprop.size()));
				CHECK_EQUAL(m_guiRowCount, static_cast<int>(guiprop.size()));
				CHECK_EQUAL(0,guiprop.getAbsoluteIndex());


				CHECK(!guiiter.isEnd());
				CHECK(!acciter.isEnd());
				CHECK(acciter.isBegin());
				CHECK(guiiter.isBegin());
				CHECK(!acciter.isNull());
				CHECK(!acciter.isNull());

				//       CHECK_EQUAL(getText(0,m_string1Number),guiiter.getString(STRING1));
				//       CHECK_EQUAL(getText(0,m_string2Number),guiiter.getString(STRING2));
			}
			catch (basar::Exception & ex)
			{
				std::cout << ex.what() << std::endl;
				const bool ExceptionOccured = false;
				CHECK(ExceptionOccured);
			}
		}
	}

	// -----------------------------------------------------------------------------------------------------
	TEST_FIXTURE(ConvertToStringMatcherHolder, GuiTieTest_Matcher_ConvertToStrings_RightToLeft_NoVar_withMemory)
	{
		BLOG_TRACE_METHOD(basar::test::unit::guitie::LoggerPool::logPerf, "GuiTieTest_Matcher_ConvertToStrings_RightToLeft_NoVar_withMemory");

		std::vector<TableWidgetKind> tableWidgetKindVec = getTableWidgetKindVector();
		std::vector<TableWidgetKind>::const_iterator tableWidgetKindIter;
		for(tableWidgetKindIter = tableWidgetKindVec.begin(); tableWidgetKindIter != tableWidgetKindVec.end(); ++tableWidgetKindIter)
		{
			try
			{
				CHECK(initTableWidgetKind(*tableWidgetKindIter));

				GuiPropertyTableRef guiprop = getMatcher().getLeft();
				AccessorPropertyTableRef accprop = getMatcher().getRight();
				GuiPropertyTable_YIterator guiiter = guiprop.begin();
				AccessorPropertyTable_YIterator acciter = accprop.begin();

				std::vector<BaseMatcherHolderTestSzenario> tests = BaseMatcherHolderTestSzenarioList::getPolicyTestSzenariosOneColTest(NORM);
				std::vector<BaseMatcherHolderTestSzenario>::iterator testiter;
				for (testiter = tests.begin(); testiter != tests.end(); ++testiter)
				{
					try
					{
						CHECK(testiter->start("GuiTieTest_Matcher_ConvertToStrings_RightToLeft_NoVar_withMemory", *this));
						TEXTOUTPUT(testiter->toText());
						resetGuiValues();
						CHECK(select());

						guiprop = getMatcher().getLeft();
						accprop = getMatcher().getRight();

						CHECK_EQUAL(m_accRowCount, static_cast<int>(accprop.size()));
						CHECK_EQUAL(m_guiRowCount, static_cast<int>(guiprop.size()));

						guiprop.setMemoryPolicy(basar::gui::tie::WITH_MEMORY);
						CHECK_EQUAL(0,guiprop.getAbsoluteIndex());

						if ( testiter->throwsConvertException() )
						{
							CHECK_THROW( getMatcher().RightToLeft(), basar::Exception);
						}
						else if (testiter->throwsToManyEntriesException() ) 
						{
							CHECK_THROW( getMatcher().RightToLeft(), basar::Exception);
						}
						else
						{          
							int maxnumber = testiter->calculateMinRowNumber(Direction::RightToLeft, m_accRowCount, m_guiRowCount);
							int guinumber = (m_guiRowCount > maxnumber) ? m_guiRowCount : maxnumber;

							CHECK_EQUAL(maxnumber,getMatcher().RightToLeft());

							guiprop = getMatcher().getLeft();
							accprop = getMatcher().getRight();
							CHECK_EQUAL(m_accRowCount, static_cast<int>(accprop.size()));
							CHECK_EQUAL(guinumber,     static_cast<int>(guiprop.size()));
							CHECK_EQUAL(0,guiprop.getAbsoluteIndex());

							guiiter = guiprop.begin();
							acciter = accprop.begin();

							int number = 0;
							for (; (!guiiter.isEnd()) && (!acciter.isEnd()); ++acciter, ++guiiter, ++number)
							{
								checkEqual(Direction::RL, number, guiiter , acciter, *testiter);
							}
							CHECK_EQUAL(maxnumber, number);
							if (number < m_guiRowCount)
							{                         
								// no changes there:
								for (; (!guiiter.isEnd()) && (number < m_guiRowCount); ++guiiter, ++number)
								{
									checkGuiEqualToText(number,guiiter , true);
								}
								CHECK_EQUAL(m_guiRowCount, number);
							}
						}
					}
					catch (basar::Exception & ex)
					{
						std::cout << " Exception in" << testiter->toText() << std::endl << ex.what() << std::endl;
						const bool ExceptionOccured = false;
						CHECK(ExceptionOccured); 
					}
					catch (...)
					{
						std::cout << "Non Basar Exception in" << testiter->toText() << std::endl;
						const bool NonBasarExceptionOccured = false;
						CHECK(NonBasarExceptionOccured);
					}
				}
			}
			catch (basar::Exception & ex)
			{
				std::cout << ex.what() << std::endl;
				const bool ExceptionOccured2 = false;
				CHECK(ExceptionOccured2);
			}
			catch (...)
			{
				std::cout << "Exception ocurred" << std::endl;
				const bool NonBasarExceptionOccured2 = false;
				CHECK(NonBasarExceptionOccured2);
				throw;
			}

		}
	}

	// -----------------------------------------------------------------------------------------------------
	TEST_FIXTURE(ConvertToStringMatcherHolder, GuiTieTest_Matcher_ConvertToStrings_LeftToRight_NoVar_withMemory)
	{
		if (g_RUNSHORT)
			return;

		BLOG_TRACE_METHOD(basar::test::unit::guitie::LoggerPool::logPerf, "GuiTieTest_Matcher_ConvertToStrings_LeftToRight_NoVar_withMemory");

		std::vector<TableWidgetKind> tableWidgetKindVec = getTableWidgetKindVector();
		std::vector<TableWidgetKind>::const_iterator tableWidgetKindIter;
		for(tableWidgetKindIter = tableWidgetKindVec.begin(); tableWidgetKindIter != tableWidgetKindVec.end(); ++tableWidgetKindIter)
		{
			try
			{
				CHECK(initTableWidgetKind(*tableWidgetKindIter));

				GuiPropertyTableRef guiprop = getMatcher().getLeft();
				AccessorPropertyTableRef accprop = getMatcher().getRight();
				GuiPropertyTable_YIterator guiiter = guiprop.begin();
				AccessorPropertyTable_YIterator acciter = accprop.begin();

				std::vector<BaseMatcherHolderTestSzenario> tests = BaseMatcherHolderTestSzenarioList::getPolicyTestSzenarios();
				std::vector<BaseMatcherHolderTestSzenario>::iterator testiter;
				for (testiter = tests.begin(); testiter != tests.end(); ++testiter)
				{
					try
					{
						CHECK(testiter->start("GuiTieTest_Matcher_ConvertToStrings_LeftToRight_NoVar_withMemory", *this));

						CHECK(select());

						accprop = getMatcher().getRight();
						guiprop = getMatcher().getLeft();
						CHECK_EQUAL(m_accRowCount, static_cast<int>(accprop.size()));
						CHECK_EQUAL(m_guiRowCount, static_cast<int>(guiprop.size()));

						guiprop.setMemoryPolicy(basar::gui::tie::WITH_MEMORY);

						if (testiter->throwsConvertException() )
						{
							CHECK_THROW( getMatcher().LeftToRight(), basar::Exception);
						}
						else if (testiter->throwsToManyEntriesException() )
						{  
							CHECK_THROW( getMatcher().LeftToRight(), basar::Exception);
						}
						else
						{
							int maxnumber = testiter->calculateMinRowNumber(Direction::LeftToRight, m_accRowCount, m_guiRowCount);
							int accnumber = (m_accRowCount < maxnumber) ? maxnumber : m_accRowCount;

							getMatcher().LeftToRight();
							CHECK_EQUAL(maxnumber,getMatcher().LeftToRight());

							accprop = getMatcher().getRight();
							guiprop = getMatcher().getLeft();

							CHECK_EQUAL(m_guiRowCount, static_cast<int>(guiprop.size()));
							CHECK_EQUAL(accnumber,     static_cast<int>(accprop.size()));

							guiiter = guiprop.begin();
							acciter = accprop.begin();

							int number = 0;
							for (; (!guiiter.isEnd()) && (!acciter.isEnd()); ++acciter, ++guiiter, ++number)
							{
								checkEqual(Direction::LR, number, guiiter , acciter, *testiter);    
							}
							CHECK_EQUAL(maxnumber, number);
							if (number < m_accRowCount)
							{
								// no changes there:
								for (; (!acciter.isEnd()) && (number < m_accRowCount); ++acciter, ++number)
								{
									checkAccNotEqualToTest(number,acciter, true);
								}
								CHECK_EQUAL(m_accRowCount, number);
							}
						}
					}
					catch (basar::Exception & ex)
					{
						std::cout << "Exception in" << testiter->toText() << std::endl
							<< ex.what() << std::endl;
						const bool ExceptionOccured = false;
						CHECK(ExceptionOccured);
					}
				}
			}
			catch (basar::Exception & ex)
			{
				std::cout << ex.what() << std::endl;
				const bool ExceptionOccured2 = false;
				CHECK(ExceptionOccured2);

			}
		}
	}
	// -----------------------------------------------------------------------------------------------------
	TEST_FIXTURE (ConvertToStringMatcherHolder, GuiTieTest_Matcher_ConvertToStrings_RightToLeft_Iter_internal)
	{ 
		BLOG_TRACE_METHOD(basar::test::unit::guitie::LoggerPool::logPerf, "GuiTieTest_Matcher_ConvertToStrings_RightToLeft_Iter_internal");

		std::vector<TableWidgetKind> tableWidgetKindVec = getTableWidgetKindVector();
		std::vector<TableWidgetKind>::const_iterator tableWidgetKindIter;
		for(tableWidgetKindIter = tableWidgetKindVec.begin(); tableWidgetKindIter != tableWidgetKindVec.end(); ++tableWidgetKindIter)
		{
			try
			{
				CHECK(initTableWidgetKind(*tableWidgetKindIter));
				GuiPropertyTableRef guiprop = getMatcher().getLeft();
				AccessorPropertyTableRef accprop = getMatcher().getRight();
				GuiPropertyTable_YIterator guiiter = guiprop.begin();
				AccessorPropertyTable_YIterator acciter = accprop.begin();

				std::vector<BaseMatcherHolderTestSzenario> tests = BaseMatcherHolderTestSzenarioList::getPolicyTestSzenarios(EQUAL,EQUAL);
				std::vector<BaseMatcherHolderTestSzenario>::iterator testiter;
				for (testiter = tests.begin(); testiter != tests.end(); ++testiter)
				{
					try
					{
						//BaseMatcherHolderTestSzenario & test = *testiter;
						CHECK(testiter->start("GuiTieTest_Matcher_ConvertToStrings_RightToLeft_Iter_internal", *this));
						TEXTOUTPUT(testiter->toText());
						CHECK(select()); 

						guiprop = getMatcher().getLeft();
						accprop = getMatcher().getRight();
						CHECK_EQUAL(m_accRowCount, static_cast<int>(accprop.size()));
						CHECK_EQUAL(m_guiRowCount, static_cast<int>(guiprop.size()));
						guiiter = guiprop.begin();
						acciter = accprop.begin();
						if (testiter->throwsConvertException() )
						{
							CHECK_THROW( getMatcher().RightToLeft(guiiter, acciter), basar::Exception);
						}
						else if (testiter->throwsToManyEntriesException() )
						{  
							CHECK_THROW( getMatcher().RightToLeft(guiiter, acciter), basar::Exception);
						}
						else
						{       

							int number = 0;
							for (; ((number<m_accRowCount) && (!acciter.isEnd())); ++number, acciter++)
							{
								getMatcher().RightToLeft(guiiter,acciter);
								checkEqual(Direction::RL, 0, guiiter , acciter, *testiter);
							}
							CHECK_EQUAL(m_accRowCount, number);
						}
					}
					catch (basar::Exception & ex)
					{
						std::cout << " Exception in" << testiter->toText() << std::endl << ex.what() << std::endl;
						const bool ExceptionOccured = false;
						CHECK(ExceptionOccured);
					}
					catch (...)
					{
						std::cout << "Non Basar Exception in" << testiter->toText() << std::endl;
						const bool NonBasarExceptionOccured = false;
						CHECK(NonBasarExceptionOccured);
					}
				}
			}
			catch (basar::Exception & ex)
			{
				std::cout << ex.what() << std::endl;
				const bool ExceptionOccured2 = false;
				CHECK(ExceptionOccured2);
			}
			catch (...)
			{
				std::cout << "Exception ocurred" << std::endl;
				const bool NonBasarExceptionOccured2 = false;
				CHECK(NonBasarExceptionOccured2);
				throw;
			}

		}
	}


	//------------------------------------------------------------------------------------------------------	
	TEST_FIXTURE(ConvertToStringMatcherHolder, GuiTieTest_Matcher_ConvertToStrings_LeftToRight_Iter_internal)
	{
		BLOG_TRACE_METHOD(basar::test::unit::guitie::LoggerPool::logPerf, "GuiTieTest_Matcher_ConvertToStrings_LeftToRight_Iter_internal");

		std::vector<TableWidgetKind> tableWidgetKindVec = getTableWidgetKindVector();
		std::vector<TableWidgetKind>::const_iterator tableWidgetKindIter;
		for(tableWidgetKindIter = tableWidgetKindVec.begin(); tableWidgetKindIter != tableWidgetKindVec.end(); ++tableWidgetKindIter)
		{
			try
			{
				CHECK(initTableWidgetKind(*tableWidgetKindIter));
				GuiPropertyTable_YIterator guiiter;
				AccessorPropertyTable_YIterator acciter;

				std::vector<BaseMatcherHolderTestSzenario> tests = BaseMatcherHolderTestSzenarioList::getPolicyTestSzenarios(NORM, NORM);
				std::vector<BaseMatcherHolderTestSzenario>::iterator testiter;
				for (testiter = tests.begin(); testiter != tests.end(); ++testiter)
				{
					try
					{
						CHECK(testiter->start("GuiTieTest_Matcher_ConvertToStrings_LeftToRight_Iter_internal", *this));
						TEXTOUTPUT(testiter->toText());

						CHECK(select()); 
						guiiter = getMatcher().getLeft().begin();
						acciter = getMatcher().getRight().begin();
						if (testiter->throwsConvertException() ) 
						{
							CHECK_THROW( getMatcher().LeftToRight(guiiter, acciter), basar::Exception);
						}
						else if (testiter->throwsToManyEntriesException() )
						{  
							CHECK_THROW( getMatcher().LeftToRight(guiiter, acciter), basar::Exception);
						}
						else
						{  
							int number = 0;

							for (; ((number<m_accRowCount) && (!acciter.isEnd())); ++number, acciter++)
							{
								getMatcher().LeftToRight(guiiter, acciter);
								checkEqual(Direction::LR, 0, guiiter , acciter, *testiter);    

							}
							CHECK_EQUAL(m_accRowCount, number);
						}
					}
					catch (basar::Exception & ex)
					{
						std::cout << "Exception in" << testiter->toText() << std::endl
							<< ex.what() << std::endl;
						const bool ExceptionOccured = false;
						CHECK(ExceptionOccured);
					}
				}
			}
			catch (basar::Exception & ex)
			{
				std::cout << ex.what() << std::endl;
				const bool ExceptionOccured2 = false;
				CHECK(ExceptionOccured2);
				// 
			}
		}
	}

	// -----------------------------------------------------------------------------------------------------
	TEST_FIXTURE(ConvertToStringMatcherHolder, GuiTieTest_Matcher_ConvertToStrings_RightToLeft_int_withMemory)
	{
		BLOG_TRACE_METHOD(basar::test::unit::guitie::LoggerPool::logPerf, "GuiTieTest_Matcher_ConvertToStrings_RightToLeft_int_withMemory");

		std::vector<TableWidgetKind> tableWidgetKindVec = getTableWidgetKindVector();
		std::vector<TableWidgetKind>::const_iterator tableWidgetKindIter;
		for(tableWidgetKindIter = tableWidgetKindVec.begin(); tableWidgetKindIter != tableWidgetKindVec.end(); ++tableWidgetKindIter)
		{
			try
			{
				CHECK(initTableWidgetKind(*tableWidgetKindIter));
				GuiPropertyTableRef guiprop = getMatcher().getLeft();
				AccessorPropertyTableRef accprop = getMatcher().getRight();
				GuiPropertyTable_YIterator guiiter = guiprop.begin();
				AccessorPropertyTable_YIterator acciter = accprop.begin();

				std::vector<BaseMatcherHolderTestSzenario> tests = BaseMatcherHolderTestSzenarioList::getPolicyTestSzenarios(NORM, NORM);
				std::vector<BaseMatcherHolderTestSzenario>::iterator testiter;
				for (testiter = tests.begin(); testiter != tests.end(); ++testiter)
				{
					try
					{
						CHECK(testiter->start("GuiTieTest_Matcher_ConvertToStrings_RightToLeft_int_withMemory", *this));
						TEXTOUTPUT(testiter->toText());
						CHECK(select());

						guiprop = getMatcher().getLeft();
						accprop = getMatcher().getRight();
						CHECK_EQUAL(0,guiprop.getAbsoluteIndex());

						CHECK_EQUAL(m_accRowCount, static_cast<int>(accprop.size()));
						CHECK_EQUAL(m_guiRowCount, static_cast<int>(guiprop.size()));

						guiprop.setMemoryPolicy(basar::gui::tie::WITH_MEMORY);

						if (testiter->throwsConvertException())
						{
							CHECK_THROW( getMatcher().RightToLeft(0,1), basar::Exception);
							CHECK_THROW( getMatcher().RightToLeft(1,2), basar::Exception);
							CHECK_THROW( getMatcher().RightToLeft(-10,-10), basar::Exception);
						}
						else if (testiter->throwsToManyEntriesException() )
						{  
							CHECK_THROW( getMatcher().RightToLeft(0,1), basar::Exception);
							CHECK_THROW( getMatcher().RightToLeft(1,2), basar::Exception);
							CHECK_THROW( getMatcher().RightToLeft(-10,-10), basar::Exception);
						}
						else
						{   
							TEXTOUTPUT2(testiter->toText());
							// test exception throwing:
							{             
								// negative index:
								CHECK_THROW(getMatcher().RightToLeft(-1, 1), basar::Exception);

								// beyond begin()
								CHECK_THROW(getMatcher().RightToLeft(0, -2), basar::Exception);
								// beyond begin()
								CHECK_THROW(getMatcher().RightToLeft(2, -27), basar::Exception); 
							}

							//    if (testiter->checkPolicy(basar::gui::tie::INSERT) 
							//        || getRowTestNumber() == MORE_GUI)
							{
								CHECK_THROW(getMatcher().RightToLeft(27, 1), basar::Exception); 
								CHECK_THROW(getMatcher().RightToLeft(27, 0), basar::Exception); 
								CHECK_THROW(getMatcher().RightToLeft(27, 2), basar::Exception); 
							}
							//else
							//{
							//    getMatcher().RightToLeft(27, 1);
							//    CHECK_EQUAL(0,getMatcher().RightToLeft(27, 1));
							//    CHECK_EQUAL(0,getMatcher().RightToLeft(27, 0));
							//    CHECK_EQUAL(0,getMatcher().RightToLeft(27, 2));
							//}

							// various, working tests
							{
								int maxnumber = testiter->calculateMinRowNumber(Direction::RightToLeft, m_accRowCount, m_guiRowCount);
								std::vector<IndexCountHolder> indexcountlist;
								indexcountlist.push_back(IndexCountHolder(0,1, maxnumber));
								indexcountlist.push_back(IndexCountHolder(1,1, maxnumber));
								indexcountlist.push_back(IndexCountHolder(3,2, maxnumber));
								indexcountlist.push_back(IndexCountHolder(0,0, maxnumber));
								indexcountlist.push_back(IndexCountHolder(1,-1, maxnumber)); 
								indexcountlist.push_back(IndexCountHolder(1,-2, maxnumber)); 
								indexcountlist.push_back(IndexCountHolder(3,10, maxnumber)); 
								std::vector<IndexCountHolder>::const_iterator indexcount;
								for (indexcount = indexcountlist.begin(); indexcount != indexcountlist.end(); ++indexcount)
								{     
									try
									{
										TEXTOUTPUT2(testiter->toText() << " (index,count,start,number)=(" << indexcount->m_index << "," << indexcount->m_count << "," << indexcount->m_start << "," <<indexcount->m_number << ")");
										resetGuiValues();

										getMatcher().RightToLeft(indexcount->m_index, indexcount->m_count);
										CHECK_EQUAL(indexcount->m_number,getMatcher().RightToLeft(indexcount->m_index, indexcount->m_count));
										maxnumber = testiter->calculateMinRowNumber(Direction::RightToLeft, m_accRowCount, m_guiRowCount,*indexcount );
										if (maxnumber > m_guiRowCount) m_guiRowCount = maxnumber;

										guiiter = getMatcher().getLeft().begin();
										acciter = getMatcher().getRight().begin();
										int i=0;
										int j=0;
										// nothing changed on the first:
										for (; i<indexcount->m_start && !acciter.isEnd() && !guiiter.isEnd(); ++i)
										{
											TEXTOUTPUT2("i1=" << i);
											checkGuiEqualToText(i,guiiter , true);
											++acciter;
											++guiiter;
										}
										CHECK_EQUAL(indexcount->m_start, i);
										// the changed values:
										for (; j<indexcount->m_number && !acciter.isEnd() && !guiiter.isEnd(); ++i, ++j)
										{
											TEXTOUTPUT2("i2=" << i);
											checkEqual(Direction::RL, i, guiiter , acciter, *testiter);

											++acciter;
											++guiiter;
										}
										CHECK_EQUAL(indexcount->m_number ,j); 
										CHECK_EQUAL(j + indexcount->m_start,i); 
										// nothing changed on the rest:
										for (; i<maxnumber && !acciter.isEnd() && !guiiter.isEnd(); ++i)
										{
											TEXTOUTPUT2("i3=" << i);
											checkGuiEqualToText(i,guiiter , true);

											++acciter;
											++guiiter;
										}
										CHECK_EQUAL(maxnumber, i); 
									}

									catch (basar::Exception & ex)
									{
										std::cout << " Exception in" << testiter->toText()
											<< " (index,count,start,number)=(" << indexcount->m_index << "," << indexcount->m_count << "," << indexcount->m_start << "," <<indexcount->m_number << ")" << std::endl 
											<< ex.what() << std::endl;
										const bool ExceptionOccured3 = false;
										CHECK(ExceptionOccured3);
									}
									catch (...)
									{
										std::cout << "Non Basar Exception in" << testiter->toText()
											<< " (index,count,start,number)=(" << indexcount->m_index << "," << indexcount->m_count << "," << indexcount->m_start << "," <<indexcount->m_number << ")" << std::endl 
											<< std::endl;
										const bool NonBasarExceptionOccured3 = false;
										CHECK(NonBasarExceptionOccured3);

									}
								}
							}
						}
					}
					catch (basar::Exception & ex)
					{
						std::cout << " Exception in" << testiter->toText() << std::endl << ex.what() << std::endl;
						const bool ExceptionOccured2 = false;
						CHECK(ExceptionOccured2);
					}
					catch (...)
					{
						std::cout << "Non Basar Exception in" << testiter->toText() << std::endl;
						const bool NonBasarExceptionOccured2 = false;
						CHECK(NonBasarExceptionOccured2);
					}
				}
			}
			catch (basar::Exception & ex)
			{
				std::cout << ex.what() << std::endl;
				const bool ExceptionOccured1 = false;
				CHECK(ExceptionOccured1);
			}
			catch (...)
			{
				std::cout << "Exception ocurred" << std::endl;
				const bool NonBasarExceptionOccured1 = false;
				CHECK(NonBasarExceptionOccured1);
				throw;
			}
		}
	}

	// -----------------------------------------------------------------------------------------------------
	TEST_FIXTURE(ConvertToStringMatcherHolder, GuiTieTest_Matcher_ConvertToStrings_LeftToRight_int_withMemory)
	{
		BLOG_TRACE_METHOD(basar::test::unit::guitie::LoggerPool::logPerf, "GuiTieTest_Matcher_ConvertToStrings_LeftToRight_int_withMemory");

		std::vector<TableWidgetKind> tableWidgetKindVec = getTableWidgetKindVector();
		std::vector<TableWidgetKind>::const_iterator tableWidgetKindIter;
		for(tableWidgetKindIter = tableWidgetKindVec.begin(); tableWidgetKindIter != tableWidgetKindVec.end(); ++tableWidgetKindIter)
		{
			try
			{
				CHECK(initTableWidgetKind(*tableWidgetKindIter));

				GuiPropertyTableRef guiprop = getMatcher().getLeft();
				AccessorPropertyTableRef accprop = getMatcher().getRight();
				GuiPropertyTable_YIterator guiiter = guiprop.begin();
				AccessorPropertyTable_YIterator acciter = accprop.begin();

				std::vector<BaseMatcherHolderTestSzenario> tests = BaseMatcherHolderTestSzenarioList::getPolicyTestSzenarios(NORM,NORM);
				std::vector<BaseMatcherHolderTestSzenario>::iterator testiter;
				for (testiter = tests.begin(); testiter != tests.end(); ++testiter)
				{
					try
					{
						CHECK(testiter->start("GuiTieTest_Matcher_ConvertToStrings_LeftToRight_int_withMemory", *this));
						TEXTOUTPUT(testiter->toText());

						CHECK(select());

						guiprop = getMatcher().getLeft();
						accprop = getMatcher().getRight();
						CHECK_EQUAL(0,guiprop.getAbsoluteIndex());

						CHECK_EQUAL(m_accRowCount, static_cast<int>(accprop.size()));
						CHECK_EQUAL(m_guiRowCount, static_cast<int>(guiprop.size()));

						guiprop.setMemoryPolicy(basar::gui::tie::WITH_MEMORY);


						if (testiter->throwsConvertException() )
						{
							CHECK_THROW(getMatcher().LeftToRight(0, 1), basar::Exception);
							CHECK_THROW(getMatcher().LeftToRight(4, 7), basar::Exception);
						}
						else if (testiter->throwsToManyEntriesException() )
						{  
							CHECK_THROW(getMatcher().LeftToRight(0, 1), basar::Exception);
							CHECK_THROW(getMatcher().LeftToRight(4, 7), basar::Exception);
						}
						else
						{
							{             
								// negative index:
								CHECK_THROW(getMatcher().LeftToRight(-1, 1), basar::Exception);
								// index too high
								CHECK_THROW(getMatcher().LeftToRight(27, 1), basar::Exception);
								// beyond begin()
								CHECK_THROW(getMatcher().LeftToRight(0, -2), basar::Exception);
								// beyond begin()
								CHECK_THROW(getMatcher().LeftToRight(2, -27), basar::Exception); 

							}
							// various, working tests
							{
								int maxnumber = testiter->calculateMinRowNumber(Direction::LeftToRight, m_accRowCount, m_guiRowCount);
								std::vector<IndexCountHolder> indexcountlist;
								indexcountlist.push_back(IndexCountHolder(0,1, maxnumber));
								indexcountlist.push_back(IndexCountHolder(1,1, maxnumber));
								indexcountlist.push_back(IndexCountHolder(3,2, maxnumber));
								indexcountlist.push_back(IndexCountHolder(0,0, maxnumber));
								indexcountlist.push_back(IndexCountHolder(1,-1, maxnumber)); 
								indexcountlist.push_back(IndexCountHolder(1,-2, maxnumber)); 

								std::vector<IndexCountHolder>::const_iterator indexcount;
								for (indexcount = indexcountlist.begin(); indexcount != indexcountlist.end(); ++indexcount)
								{     
									try
									{
										resetGuiValues();
										CHECK(select());
										CHECK_EQUAL(indexcount->m_number,getMatcher().LeftToRight(indexcount->m_index, indexcount->m_count));
										getMatcher().LeftToRight(indexcount->m_index, indexcount->m_count);
										maxnumber = testiter->calculateMinRowNumber(Direction::LeftToRight, m_accRowCount, m_guiRowCount,*indexcount );
										if (maxnumber > m_guiRowCount) m_guiRowCount = maxnumber;

										guiiter = getMatcher().getLeft().begin();
										acciter = getMatcher().getRight().begin();
										int i=0;
										int j=0;
										// nothing changed on the first:
										for (; i<indexcount->m_start && !acciter.isEnd() && !guiiter.isEnd(); ++i)
										{
											TEXTOUTPUT2("i1=" << i);
											checkAccNotEqualToTest(i,acciter, true);
											++acciter;
											++guiiter;
										}
										CHECK_EQUAL(indexcount->m_start, i);
										// the changed values:
										for (; j<indexcount->m_number && !acciter.isEnd() && !guiiter.isEnd(); ++i, ++j)
										{
											TEXTOUTPUT2("i2=" << i);
											checkEqual(Direction::LR, i, guiiter , acciter, *testiter);    

											++acciter;
											++guiiter;
										}
										CHECK_EQUAL(indexcount->m_number ,j); 
										CHECK_EQUAL(j + indexcount->m_start,i); 
										// nothing changed on the rest:
										for (; i<maxnumber && !acciter.isEnd() && !guiiter.isEnd(); ++i)
										{
											TEXTOUTPUT2("i3=" << i);
											checkAccNotEqualToTest(i,acciter, true);

											++acciter;
											++guiiter;
										}
										CHECK_EQUAL(maxnumber, i); 
									}
									catch (basar::Exception & ex)
									{
										std::cout << " Exception in" << testiter->toText()
											<< " (index,count,start,number)=(" << indexcount->m_index << "," << indexcount->m_count << "," << indexcount->m_start << "," <<indexcount->m_number << ")" << std::endl 
											<< ex.what() << std::endl;
										const bool ExceptionOccured3 = false;
										CHECK(ExceptionOccured3);
									}
									catch (...)
									{
										std::cout << "Non Basar Exception in" << testiter->toText()
											<< " (index,count,start,number)=(" << indexcount->m_index << "," << indexcount->m_count << "," << indexcount->m_start << "," <<indexcount->m_number << ")" << std::endl 
											<< std::endl;
										const bool NonBasarExceptionOccured3 = false;
										CHECK(NonBasarExceptionOccured3);
									}
								}
							}
						}
					}
					catch (basar::Exception & ex)
					{
						std::cout << "Exception in" << testiter->toText() << std::endl
							<< ex.what() << std::endl;
						const bool ExceptionOccured = false;
						CHECK(ExceptionOccured);
					}
				}
			}
			catch (basar::Exception & ex)
			{
				std::cout << ex.what() << std::endl;
				const bool ExceptionOccured2 = false;
				CHECK(ExceptionOccured2);
			}
		}
	}
	// -----------------------------------------------------------------------------------------------------
	TEST_FIXTURE (ConvertToStringMatcherHolder, GuiTieTest_Matcher_ConvertToStrings_RightToLeft_policy)
	{ 
		BLOG_TRACE_METHOD(basar::test::unit::guitie::LoggerPool::logPerf, "GuiTieTest_Matcher_ConvertToStrings_RightToLeft_policy");

		std::vector<TableWidgetKind> tableWidgetKindVec = getTableWidgetKindVector();
		std::vector<TableWidgetKind>::const_iterator tableWidgetKindIter;
		for(tableWidgetKindIter = tableWidgetKindVec.begin(); tableWidgetKindIter != tableWidgetKindVec.end(); ++tableWidgetKindIter)
		{
			try
			{
				CHECK(initTableWidgetKind(*tableWidgetKindIter));
				GuiPropertyTableRef guiprop = getMatcher().getLeft();
				AccessorPropertyTableRef accprop = getMatcher().getRight();
				GuiPropertyTable_YIterator guiiter = guiprop.begin();
				AccessorPropertyTable_YIterator acciter = accprop.begin();

				std::vector<BaseMatcherHolderTestSzenario> tests = BaseMatcherHolderTestSzenarioList::getPolicyTestSzenarios(NORM,NORM);
				std::vector<BaseMatcherHolderTestSzenario>::iterator testiter;
				for (testiter = tests.begin(); testiter != tests.end(); ++testiter)
				{
					try
					{
						//BaseMatcherHolderTestSzenario & test = *testiter;
						CHECK(testiter->start("GuiTieTest_Matcher_ConvertToStrings_RightToLeft_Iter_internal", *this));
						TEXTOUTPUT(testiter->toText());
						CHECK(select()); 

						guiprop = getMatcher().getLeft();
						accprop = getMatcher().getRight();
						CHECK_EQUAL(m_accRowCount, static_cast<int>(accprop.size()));
						CHECK_EQUAL(m_guiRowCount, static_cast<int>(guiprop.size()));

						std::vector<MatcherPolicyType> policyvec = BaseMatcherHolderTestSzenarioList::getTestMatcherPolicyTypeVector();
						std::vector<MatcherPolicyType>::const_iterator policyiter;
						for (policyiter = policyvec.begin(); policyiter != policyvec.end(); ++policyiter)
						{		
							try
							{	
								TEXTOUTPUT(testiter->toText() << "with policy " << *policyiter);
								resetGuiValues();
								guiiter = guiprop.begin();
								acciter = accprop.begin();
								if (testiter->throwsConvertException(*policyiter) )
								{
									CHECK_THROW( MatcherRef::RightToLeft(guiiter, acciter,*policyiter), basar::Exception);
								}
								else if (testiter->throwsToManyEntriesException(*policyiter) )
								{	
									CHECK_THROW( MatcherRef::RightToLeft(guiiter, acciter,*policyiter), basar::Exception);
								}
								else
								{     
									int number = 0;
									for (; ((number<m_accRowCount) && (!acciter.isEnd())); ++number, acciter++)
									{
										MatcherRef::RightToLeft(guiiter,acciter,*policyiter);
										checkEqual(Direction::RL, 0, guiiter , acciter, *testiter, true, *policyiter);
									}
									CHECK_EQUAL(m_accRowCount, number);
								}
							}
							catch (basar::Exception & ex)
							{
								std::cout << " Exception in" << testiter->toText() << std::endl << ex.what() << std::endl;
								const bool InnerExceptionOccured = false;
								CHECK(InnerExceptionOccured);
							}
							catch (...)
							{
								std::cout << "Non Basar Exception in" << testiter->toText() << std::endl;
								const bool InnerNonBasarExceptionOccured = false;
								CHECK(InnerNonBasarExceptionOccured);
							}
						}

					}
					catch (basar::Exception & ex)
					{
						std::cout << " Exception in" << testiter->toText() << std::endl << ex.what() << std::endl;
						const bool ExceptionOccured = false;
						CHECK(ExceptionOccured);
					}
					catch (...)
					{
						std::cout << "Non Basar Exception in" << testiter->toText() << std::endl;
						const bool NonBasarExceptionOccured = false;
						CHECK(NonBasarExceptionOccured);
					}
				}
			}
			catch (basar::Exception & ex)
			{
				std::cout << ex.what() << std::endl;
				const bool ExceptionOccured2 = false;
				CHECK(ExceptionOccured2);
			}
			catch (...)
			{
				std::cout << "Exception ocurred" << std::endl;
				const bool NonBasarExceptionOccured2 = false;
				CHECK(NonBasarExceptionOccured2);
				throw;
			}

		}
	}

	// -----------------------------------------------------------------------------------------------------
	TEST_FIXTURE(ConvertToStringMatcherHolder, GuiTieTest_Matcher_ConvertToStrings_LeftToRight_policy)
	{
		BLOG_TRACE_METHOD(basar::test::unit::guitie::LoggerPool::logPerf, "GuiTieTest_Matcher_ConvertToStrings_LeftToRight_policy");

		std::vector<TableWidgetKind> tableWidgetKindVec = getTableWidgetKindVector();
		std::vector<TableWidgetKind>::const_iterator tableWidgetKindIter;
		for(tableWidgetKindIter = tableWidgetKindVec.begin(); tableWidgetKindIter != tableWidgetKindVec.end(); ++tableWidgetKindIter)
		{
			try
			{
				CHECK(initTableWidgetKind(*tableWidgetKindIter));
				GuiPropertyTable_YIterator guiiter;
				AccessorPropertyTable_YIterator acciter;

				std::vector<BaseMatcherHolderTestSzenario> tests = BaseMatcherHolderTestSzenarioList::getPolicyTestSzenarios(NORM,NORM);
				std::vector<BaseMatcherHolderTestSzenario>::iterator testiter;
				for (testiter = tests.begin(); testiter != tests.end(); ++testiter)
				{
					try
					{
						CHECK(testiter->start("GuiTieTest_Matcher_ConvertToStrings_LeftToRight_Iter_internal", *this));
						TEXTOUTPUT(testiter->toText());

						std::vector<MatcherPolicyType> policyvec = BaseMatcherHolderTestSzenarioList::getTestMatcherPolicyTypeVector();
						std::vector<MatcherPolicyType>::const_iterator policyiter;
						for (policyiter = policyvec.begin(); policyiter != policyvec.end(); ++policyiter)
						{
							CHECK(select()); 
							guiiter = getMatcher().getLeft().begin();
							acciter = getMatcher().getRight().begin();
							try
							{
								if (testiter->throwsConvertException(*policyiter) ) 
								{
									CHECK_THROW( MatcherRef::LeftToRight(guiiter, acciter, *policyiter), basar::Exception);
								}
								else if (testiter->throwsToManyEntriesException(*policyiter) ) 
								{
									CHECK_THROW( MatcherRef::LeftToRight(guiiter, acciter, *policyiter), basar::Exception);
								}
								else
								{  
									int number = 0;
									for (; ((number<m_accRowCount) && (!acciter.isEnd())); ++number, acciter++)
									{
										MatcherRef::LeftToRight(guiiter, acciter, *policyiter);
										checkEqual(Direction::LR, 0, guiiter , acciter, *testiter, true, *policyiter);    
									}
									CHECK_EQUAL(m_accRowCount, number);
								}
							}
							catch (basar::Exception & ex)
							{
								std::cout << " Exception in" << testiter->toText() << std::endl << ex.what() << std::endl;
								const bool InnerExceptionOccured = false;
								CHECK(InnerExceptionOccured);
							}
							catch (...)
							{
								std::cout << "Non Basar Exception in" << testiter->toText() << std::endl;
								const bool InnerNonBasarExceptionOccured = false;
								CHECK(InnerNonBasarExceptionOccured);
							}
						}
					}
					catch (basar::Exception & ex)
					{
						std::cout << "Exception in" << testiter->toText() << std::endl
							<< ex.what() << std::endl;
						const bool ExceptionOccured = false;
						CHECK(ExceptionOccured);
					}
				}
			}
			catch (basar::Exception & ex)
			{
				std::cout << ex.what() << std::endl;
				const bool ExceptionOccured2 = false;
				CHECK(ExceptionOccured2);
				// 
			}
		}
	}

	// -----------------------------------------------------------------------------------------------------

}

// ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~  ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~
