/*
* @file                                            
* @author Anke Klink                      
* @date 2008                            
*/ 

#include <UnitTest++/UnitTest++.h>
#include "libbasarguitie.h"

#include "libbasardbaspect_connectionref.h"
#include "libbasardbaspect_accessorpropertytable_yiterator.h"

#include "testconnection.h"
#include "basematcherholder.h"
#include "basematcherholdertestszenario.h"
#include "basematcherholdertestszenariolist.h"
#include "indexcountholder.h"
#include "matcherholderlist.h"
#include "stringmatcherholdermorelines.h"
#include "loggerpool.h"
#include "testexceptionhandler.h"

#include <QtCore/QEvent>

#pragma warning (push)
#pragma warning(disable: 4244)
#include <QtGui/QKeyEvent>
#pragma warning(pop)

#include <QtWidgets/QApplication>

#pragma warning (push)
#pragma warning(disable: 4481)
#include <QtWidgets/QTableWidget>
#pragma warning(pop)

using basar::ConstString;
using basar::VarString;

using basar::gui::tie::MatcherRef;
using basar::gui::tie::GuiPropertyTable_YIterator;
using basar::gui::tie::GuiPropertyTableRef;

using basar::db::aspect::AccessorInstanceRef;
using basar::db::aspect::ConnectionRef;
using basar::db::aspect::AccessorPropertyTable_YIterator;
using basar::db::aspect::AccessorPropertyTableRef;
using basar::db::aspect::ExecuteResultInfo;

using basar::test::unit::guitie::TestConnection;
using basar::test::unit::guitie::BaseMatcherHolder;
using basar::test::unit::guitie::BaseMatcherHolderTestSzenario;
using basar::test::unit::guitie::MatcherHolderList;
using basar::test::unit::guitie::Direction;

using basar::test::unit::guitie::BaseMatcherHolderTestSzenarioList;
using basar::test::unit::guitie::IndexCountHolder;
using basar::test::unit::guitie::StringMatcherHolderMoreLines;
using  basar::test::unit::guitie::TestExceptionHandler;
using  basar::test::unit::guitie::TableWidgetMatcherHolder;


//------------------------------------------------------------------------------
SUITE(GuiTieTest_Matcher_Strings_NoMemory_MoreLines_Events)
{
	typedef basar::gui::tie::MatcherPolicyType MatcherPolicyType;

	void startTestExceptionHandlerCheck()
	{
		boost::shared_ptr<basar::gui::tie::ExceptionHandler> exhandler = 
			boost::shared_ptr<basar::gui::tie::ExceptionHandler> (new TestExceptionHandler(true));
		basar::gui::tie::ExceptionHandlerRef exhandlerref(exhandler);
		basar::gui::tie::Manager::getInstance().getExceptionHandlerManager().setExceptionHandlerRef(exhandlerref);
		basar::gui::tie::Manager::getInstance().getExceptionHandlerManager().resetNumberOfExceptionHandled();

	}

	bool checkNoException(bool reset)
	{
		bool retval = basar::gui::tie::Manager::getInstance().getExceptionHandlerManager().checkNoExceptionHandled();
		if (reset)
		{
			basar::gui::tie::Manager::getInstance().getExceptionHandlerManager().resetNumberOfExceptionHandled();
		}
		return retval;
	}


	// ------------------------------------------------------------------------------------------------------------------------------------------------
	TEST_FIXTURE(StringMatcherHolderMoreLines, GuiTieTest_Matcher_Strings_MoreLines_Events_NoMemory_PageDown)
	{
		startTestExceptionHandlerCheck();
		CHECK(checkNoException(true)); 
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

				std::vector<BaseMatcherHolderTestSzenario> tests = BaseMatcherHolderTestSzenarioList::getPolicyTestSzenariosOneColTest(BaseMatcherHolder::EQUAL);
				std::vector<BaseMatcherHolderTestSzenario>::iterator testiter;
				for (testiter = tests.begin(); testiter != tests.end(); ++testiter)
				{ 
					try
					{
						CHECK(testiter->start("GuiTieTest_Matcher_Strings_MoreLines_Events_NoMemory_LeftToRight_int_withoutMemory_2", *this));
						TEXTOUTPUT(testiter->toText());

						CHECK(select());

						guiprop = getMatcher().getLeft();
						accprop = getMatcher().getRight();
						CHECK_EQUAL(0,guiprop.getAbsoluteIndex());

						CHECK_EQUAL(m_accRowCount, static_cast<int>(accprop.size()));
						CHECK_EQUAL(m_guiRowCount, static_cast<int>(guiprop.size()));

						guiprop.setMemoryPolicy(basar::gui::tie::WITHOUT_MEMORY);

						QKeyEvent pageDownEvent(QEvent::KeyPress, Qt::Key_PageDown, Qt::NoModifier);

						CHECK(getMatcher().getCurrentRight().isEnd() );
						CHECK(getMatcher().getCurrentLeft().isEnd());
						CHECK_EQUAL(0,getMatcher().getLeft().getAbsoluteIndex());


						int number = (m_accRowCount < m_guiRowCount) ? m_accRowCount : m_guiRowCount;
						int maxnumber = testiter->calculateMinRowNumber(Direction::RightToLeft, m_accRowCount, m_guiRowCount);


						IndexCountHolder index1(0,number, maxnumber);

						int maxnumber2 = testiter->calculateMinRowNumber(Direction::RightToLeft, m_accRowCount, m_guiRowCount,index1 );


						// first matcher call on 0 to number
						CHECK_EQUAL(maxnumber2, getMatcher().RightToLeft(0, number));
						m_tableWidget->setCurrentCell(0,0);
						CHECK_EQUAL(0, getMatcher().getCurrentRight().getRowNumber());
						CHECK_EQUAL(0, getMatcher().getCurrentLeft().getAbsolutePosition());
						CHECK_EQUAL(0, getMatcher().getLeft().getAbsoluteIndex());

						// 1. Event:
						QApplication::sendEvent(this->m_widget, &pageDownEvent); 
						CHECK(checkNoException(true));

						CHECK_EQUAL(0, getMatcher().getLeft().getAbsoluteIndex());
						CHECK_EQUAL(m_guiRowCount-1, getMatcher().getCurrentLeft().getAbsolutePosition());
						if ((m_guiRowCount > m_accRowCount) && (maxnumber == m_accRowCount))
						{ 
							CHECK (getMatcher().getCurrentRight().isEnd()); 
						}
						else  
						{ 
							CHECK_EQUAL(m_guiRowCount-1, getMatcher().getCurrentRight().getRowNumber());
						}
						QApplication::sendEvent(this->m_widget, &pageDownEvent); 
						CHECK(checkNoException(true));

						//std::cout << m_guiRowCount << " - " << m_accRowCount << " - " << maxnumber << std::endl;
						if (m_accRowCount > m_guiRowCount && maxnumber != m_guiRowCount)
						{ 
							int overflow = m_accRowCount - m_guiRowCount;
							CHECK_EQUAL(overflow, getMatcher().getLeft().getAbsoluteIndex());
							CHECK_EQUAL(m_guiRowCount+1, getMatcher().getCurrentLeft().getAbsolutePosition());
							CHECK_EQUAL(m_guiRowCount+1, getMatcher().getCurrentRight().getRowNumber());
						}
						else  
						{ 
							CHECK_EQUAL(0, getMatcher().getLeft().getAbsoluteIndex());
							CHECK_EQUAL(m_guiRowCount-1, getMatcher().getCurrentLeft().getAbsolutePosition());
							if ((m_guiRowCount > m_accRowCount) && (maxnumber == m_accRowCount))
							{ 
								CHECK (getMatcher().getCurrentRight().isEnd()); 
							}
							else  
							{ 
								CHECK_EQUAL(m_guiRowCount-1, getMatcher().getCurrentRight().getRowNumber());
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

	// ------------------------------------------------------------------------------------------------------------------------------------------------
	TEST_FIXTURE(StringMatcherHolderMoreLines, GuiTieTest_Matcher_Strings_MoreLines_Events_NoMemory_KeyDownKeyUp_severalPages)
	{
		startTestExceptionHandlerCheck();
		CHECK(checkNoException(true));
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

				QKeyEvent keyUpEvent( QEvent::KeyPress, Qt::Key_Up, Qt::NoModifier);
				QKeyEvent keyDownEvent( QEvent::KeyPress, Qt::Key_Down, Qt::NoModifier);


				m_accRowCount = 12;	
				CHECK(select());
				CHECK(select(false));

				CHECK(getMatcher().getCurrentRight().isEnd() );
				CHECK(getMatcher().getCurrentLeft().isEnd());
				CHECK_EQUAL(0,getMatcher().getLeft().getAbsoluteIndex());
				std::vector<BaseMatcherHolderTestSzenario> tests = BaseMatcherHolderTestSzenarioList::getInsertableOnlyPolicyTestSzenarios(BaseMatcherHolder::EQUAL,BaseMatcherHolder::EQUAL);
				std::vector<BaseMatcherHolderTestSzenario>::iterator testiter;
				for (testiter = tests.begin(); testiter != tests.end(); ++testiter)
				{
					try
					{ 
						CHECK(testiter->start("GuiTieTest_Matcher_Strings_MoreLines_Events_NoMemory_KeyeDownKeyUp_severallPages", *this));
						TEXTOUTPUT(testiter->toText());

						guiprop = getMatcher().getLeft();
						accprop = getMatcher().getRight();
						CHECK_EQUAL(0,guiprop.getAbsoluteIndex());

						CHECK_EQUAL(m_accRowCount, static_cast<int>(accprop.size()));
						CHECK_EQUAL(m_guiRowCount, static_cast<int>(guiprop.size()));

						guiprop.setMemoryPolicy(basar::gui::tie::WITHOUT_MEMORY);


						basar::Int32 guiLine = 0;
						basar::Int32 accLine = 0;
						basar::Int32 absLine = 0;


						// first matcher call on 0 to number
						CHECK_EQUAL(m_guiRowCount, getMatcher().RightToLeft(0, m_guiRowCount));
						m_tableWidget->setCurrentCell(0,0);

						CHECK_EQUAL(absLine, getMatcher().getLeft().getAbsoluteIndex());
						CHECK_EQUAL(accLine, getMatcher().getCurrentRight().getRowNumber());
						CHECK_EQUAL(guiLine, getMatcher().getCurrentLeft().getAbsolutePosition());

						int someNumber = 3;

						// 1. Event (KeyDown to last line)
						for (int i=0; i<m_guiRowCount-1; ++i)
						{
							QApplication::sendEvent(this->m_widget, &keyDownEvent); 
							CHECK(checkNoException(true));
							guiLine++;
							accLine++;
							CHECK_EQUAL(absLine, getMatcher().getLeft().getAbsoluteIndex());
							CHECK_EQUAL(guiLine, getMatcher().getCurrentLeft().getAbsolutePosition());
							CHECK_EQUAL(accLine, getMatcher().getCurrentRight().getRowNumber());
						}
						// 1. Event (KeyDown on lastLine)
						for (int i=0; i<someNumber; ++i)
						{
							QApplication::sendEvent(this->m_widget, &keyDownEvent); 
							CHECK(checkNoException(true));
							guiLine++;
							accLine++;
							absLine++;
							CHECK_EQUAL(absLine, getMatcher().getLeft().getAbsoluteIndex());
							CHECK_EQUAL(guiLine, getMatcher().getCurrentLeft().getAbsolutePosition());
							CHECK_EQUAL(accLine, getMatcher().getCurrentRight().getRowNumber());
						}

						// test on keyUp
						// 1. Event (KeyUp to first line)
						for (int i=0; i<m_guiRowCount-1; ++i)
						{
							QApplication::sendEvent(this->m_widget, &keyUpEvent); 
							CHECK(checkNoException(true));
							guiLine--;
							accLine--;
							CHECK_EQUAL(absLine, getMatcher().getLeft().getAbsoluteIndex());
							CHECK_EQUAL(guiLine, getMatcher().getCurrentLeft().getAbsolutePosition());
							CHECK_EQUAL(accLine, getMatcher().getCurrentRight().getRowNumber());
						}
						// 1. Event (KeyDown on lastLine)
						for (int i=0; i<someNumber; ++i)
						{
							QApplication::sendEvent(this->m_widget, &keyUpEvent); 
							CHECK(checkNoException(true));
							guiLine--;
							accLine--;
							absLine--;
							CHECK_EQUAL(absLine, getMatcher().getLeft().getAbsoluteIndex());
							CHECK_EQUAL(guiLine, getMatcher().getCurrentLeft().getAbsolutePosition());
							CHECK_EQUAL(accLine, getMatcher().getCurrentRight().getRowNumber());
						}
						// after the first Line
						CHECK_EQUAL(0,absLine);
						CHECK_EQUAL(0,guiLine);
						CHECK_EQUAL(0,accLine);
						QApplication::sendEvent(this->m_widget, &keyUpEvent); 
						CHECK(checkNoException(true));
						CHECK_EQUAL(absLine, getMatcher().getLeft().getAbsoluteIndex());
						CHECK_EQUAL(guiLine, getMatcher().getCurrentLeft().getAbsolutePosition());
						CHECK_EQUAL(accLine, getMatcher().getCurrentRight().getRowNumber());

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

	// ------------------------------------------------------------------------------------------------------------------------------------------------
	TEST_FIXTURE(StringMatcherHolderMoreLines, GuiTieTest_Matcher_Strings_MoreLines_Events_NoMemory_PageDown_serveralPages)
	{	 
		startTestExceptionHandlerCheck();
		CHECK(checkNoException(true));
		int times = 5;
		m_accRowCount = m_guiRowCount*times;
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

				QKeyEvent pageDownEvent(QEvent::KeyPress, Qt::Key_PageDown, Qt::NoModifier);

				//QKeyEvent * pageUpEvent = new QKeyEvent(
				//	QEvent::KeyPress,
				//	Qt::Key_PageUp,
				//	Qt::NoModifier);

				CHECK(getMatcher().getCurrentRight().isEnd() );
				CHECK(getMatcher().getCurrentLeft().isEnd());

				std::vector<BaseMatcherHolderTestSzenario> tests = BaseMatcherHolderTestSzenarioList::getInsertableOnlyPolicyTestSzenarios(BaseMatcherHolder::EQUAL,BaseMatcherHolder::EQUAL);
				std::vector<BaseMatcherHolderTestSzenario>::iterator testiter;
				for (testiter = tests.begin(); testiter != tests.end(); ++testiter)
				{
					resetAccessorAndMatcher();
					CHECK(testiter->start("GuiTieTest_Matcher_Strings_MoreLines_Events_NoMemory_PageDown_serverallPages", *this));
					TEXTOUTPUT(testiter->toText());
					try
					{	
						CHECK(select());
						CHECK(select(false));
						CHECK(select(false));
						CHECK(select(false)); 
						CHECK(select(false)); 

						guiprop = getMatcher().getLeft();
						accprop = getMatcher().getRight();
						guiprop.setMemoryPolicy(basar::gui::tie::WITHOUT_MEMORY);
						try
						{
							getMatcher().RightToLeft(0, m_guiRowCount);
						}
						catch (basar::Exception & ex)
						{
							std::cout << ex.what() << std::endl;
						}
						CHECK_EQUAL(m_guiRowCount, getMatcher().RightToLeft(0, m_guiRowCount));
						CHECK_EQUAL(m_accRowCount, static_cast<int>(accprop.size()));
						CHECK_EQUAL(m_guiRowCount, static_cast<int>(guiprop.size()));

						m_tableWidget->setCurrentCell(0,0);
						CHECK_EQUAL(0,guiprop.getAbsoluteIndex());
						// pageDown:
						{
							//std::cout << "A" << std::endl;
							// first matcher call on 0 to number

							basar::Int32 guiLine = 0;
							basar::Int32 accLine = 0;
							basar::Int32 absLine = 0;
							CHECK_EQUAL(absLine, getMatcher().getLeft().getAbsoluteIndex());
							CHECK_EQUAL(accLine, getMatcher().getCurrentRight().getRowNumber());
							CHECK_EQUAL(guiLine, getMatcher().getCurrentLeft().getAbsolutePosition());
							//std::cout << "b" <<  std::endl;
							//std::cout << m_guiRowCount << " - " << m_accRowCount << " + " << guiLine << " - " << accLine << " - " << absLine << std::endl;

							// 1. Event (Pagedown to last line)
							QApplication::sendEvent(this->m_widget, &pageDownEvent); 
							CHECK(checkNoException(true));
							guiLine = accLine = m_guiRowCount-1;
							CHECK_EQUAL(absLine, getMatcher().getLeft().getAbsoluteIndex());
							CHECK_EQUAL(guiLine, getMatcher().getCurrentLeft().getAbsolutePosition());
							CHECK_EQUAL(accLine, getMatcher().getCurrentRight().getRowNumber());
							//std::cout << "c" << std::endl;

							// 2. Event Pagedown (Pagedown to first line next page)
							QApplication::sendEvent(this->m_widget, &pageDownEvent); 
							CHECK(checkNoException(true));
							guiLine = accLine = m_guiRowCount + 5;
							absLine = m_guiRowCount;
							CHECK_EQUAL(absLine, getMatcher().getLeft().getAbsoluteIndex());
							CHECK_EQUAL(guiLine, getMatcher().getCurrentLeft().getAbsolutePosition());
							CHECK_EQUAL(accLine, getMatcher().getCurrentRight().getRowNumber());
							//std::cout << "d" << std::endl;

							// more Events Pagedown (Pagedown to first line third page)
							for (int i=2; i<times; ++i)
							{
								QApplication::sendEvent(this->m_widget, &pageDownEvent);
								CHECK(checkNoException(true));
								absLine = accLine+1; 
								guiLine += m_guiRowCount;
								accLine += m_guiRowCount;

								CHECK_EQUAL(absLine, getMatcher().getLeft().getAbsoluteIndex());
								CHECK_EQUAL(guiLine, getMatcher().getCurrentLeft().getAbsolutePosition());
								CHECK_EQUAL(accLine, getMatcher().getCurrentRight().getRowNumber());
								//std::cout << "e" << std::endl;
							}

							// no more changes:
							QApplication::sendEvent(this->m_widget, &pageDownEvent);
							CHECK(checkNoException(true));

							CHECK_EQUAL(absLine, getMatcher().getLeft().getAbsoluteIndex());
							CHECK_EQUAL(guiLine, getMatcher().getCurrentLeft().getAbsolutePosition());
							CHECK_EQUAL(accLine, getMatcher().getCurrentRight().getRowNumber());
							//std::cout << "f" << std::endl;

							// no more changes:
							QApplication::sendEvent(this->m_widget, &pageDownEvent);
							CHECK(checkNoException(true));

							CHECK_EQUAL(absLine, getMatcher().getLeft().getAbsoluteIndex());
							CHECK_EQUAL(guiLine, getMatcher().getCurrentLeft().getAbsolutePosition());
							CHECK_EQUAL(accLine, getMatcher().getCurrentRight().getRowNumber());
							//std::cout << "f" << std::endl;

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
			catch (std::exception & ex)
			{
				std::cout << ex.what() << std::endl;
				const bool STDExceptionOccured = false;
				CHECK(STDExceptionOccured);
				throw;
			}
			catch (...)
			{ 
				const bool UnknownExceptionOccured = false;
				CHECK(UnknownExceptionOccured);
				throw;
			}
		}
	}

	// ------------------------------------------------------------------------------------------------------------------------------------------------
	TEST_FIXTURE(StringMatcherHolderMoreLines, GuiTieTest_Matcher_Strings_MoreLines_Events_NoMemory_PageUp_serveralPages)
	{	
		startTestExceptionHandlerCheck();
		CHECK(checkNoException(true));
		int times = 5;
		m_accRowCount = m_guiRowCount*times;
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

				QKeyEvent pageUpEvent (QEvent::KeyPress,Qt::Key_PageUp, Qt::NoModifier);

				CHECK(getMatcher().getCurrentRight().isEnd() );
				CHECK(getMatcher().getCurrentLeft().isEnd());

				std::vector<BaseMatcherHolderTestSzenario> tests = BaseMatcherHolderTestSzenarioList::getInsertableOnlyPolicyTestSzenarios(BaseMatcherHolder::EQUAL,BaseMatcherHolder::EQUAL);
				std::vector<BaseMatcherHolderTestSzenario>::iterator testiter;
				testiter = tests.begin();
				for (testiter; testiter != tests.end(); ++testiter)
				{ 
					initTableWidget();
					resetAccessorAndMatcher();

					CHECK(testiter->start("GuiTieTest_Matcher_Strings_MoreLines_Events_NoMemory_PageUp_serverallPages", *this));
					TEXTOUTPUT(testiter->toText());
					try
					{	
						CHECK(select());
						CHECK(select(false));
						CHECK(select(false));
						CHECK(select(false)); 
						CHECK(select(false)); 
						guiprop = getMatcher().getLeft();
						accprop = getMatcher().getRight();
						guiprop.setMemoryPolicy(basar::gui::tie::WITHOUT_MEMORY);

						int whole = m_accRowCount;

						CHECK_EQUAL(whole, getMatcher().RightToLeft());
						CHECK_EQUAL(whole, static_cast<int>(accprop.size()));
						CHECK_EQUAL(whole, static_cast<int>(guiprop.size()));

						m_tableWidget->setCurrentCell(0,0); 
						basar::Int32 absLine = whole - m_guiRowCount;
						basar::Int32 accLine = absLine;
						basar::Int32 guiLine = absLine;

						CHECK_EQUAL(absLine, getMatcher().getLeft().getAbsoluteIndex());
						CHECK_EQUAL(accLine, getMatcher().getCurrentRight().getRowNumber());
						CHECK_EQUAL(guiLine, getMatcher().getCurrentLeft().getAbsolutePosition());

						CHECK(!getMatcher().getRight().begin().isEnd());
						// pageUp:
						{  
							QApplication::sendEvent(this->m_widget, &pageUpEvent); 
							CHECK(checkNoException(true)); 
							absLine -= m_guiRowCount; 
							guiLine -= m_guiRowCount;
							accLine -= m_guiRowCount;
							CHECK_EQUAL(absLine, getMatcher().getLeft().getAbsoluteIndex());
							CHECK_EQUAL(guiLine, getMatcher().getCurrentLeft().getAbsolutePosition());
							CHECK_EQUAL(accLine, getMatcher().getCurrentRight().getRowNumber());
						
							// test: pageup from not first row:
							m_tableWidget->setCurrentCell(1,1); 
							CHECK_EQUAL(absLine, getMatcher().getLeft().getAbsoluteIndex());
							CHECK_EQUAL(guiLine+1, getMatcher().getCurrentLeft().getAbsolutePosition());
							CHECK_EQUAL(accLine+1, getMatcher().getCurrentRight().getRowNumber());
                            QApplication::sendEvent(this->m_widget, &pageUpEvent); 
							CHECK(checkNoException(true));  
							CHECK_EQUAL(absLine, getMatcher().getLeft().getAbsoluteIndex());
							CHECK_EQUAL(guiLine, getMatcher().getCurrentLeft().getAbsolutePosition());
							CHECK_EQUAL(accLine, getMatcher().getCurrentRight().getRowNumber());
						
							// more Events Pagedown (Pagedown to first line third page)
							for (int i=2; i<times; ++i)
							{
								QApplication::sendEvent(this->m_widget, &pageUpEvent);
								CHECK(checkNoException(true));
								absLine -= m_guiRowCount; 
								guiLine -= m_guiRowCount;
								accLine -= m_guiRowCount;

								CHECK_EQUAL(absLine, getMatcher().getLeft().getAbsoluteIndex());
								CHECK_EQUAL(guiLine, getMatcher().getCurrentLeft().getAbsolutePosition());
								CHECK_EQUAL(accLine, getMatcher().getCurrentRight().getRowNumber());
							}

							// no more changes:
							QApplication::sendEvent(this->m_widget, &pageUpEvent);
							CHECK(checkNoException(true));

							CHECK_EQUAL(0, getMatcher().getLeft().getAbsoluteIndex());
							CHECK_EQUAL(0, getMatcher().getCurrentLeft().getAbsolutePosition());
							CHECK_EQUAL(0, getMatcher().getCurrentRight().getRowNumber());
							 
							// no more changes:
							QApplication::sendEvent(this->m_widget, &pageUpEvent);
							CHECK(checkNoException(true));

							CHECK_EQUAL(0, getMatcher().getLeft().getAbsoluteIndex());
							CHECK_EQUAL(0, getMatcher().getCurrentLeft().getAbsolutePosition());
							CHECK_EQUAL(0, getMatcher().getCurrentRight().getRowNumber());
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
			catch (std::exception & ex)
			{
				std::cout << ex.what() << std::endl;
				const bool STDExceptionOccured = false;
				CHECK(STDExceptionOccured);
				throw;
			}
			catch (...)
			{ 
				const bool UnknownExceptionOccured = false;
				CHECK(UnknownExceptionOccured);
				throw;
			}
		}
	}

	//// ------------------------------------------------------------------------------------------------------------------------------------------------

}

// ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~  ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~
