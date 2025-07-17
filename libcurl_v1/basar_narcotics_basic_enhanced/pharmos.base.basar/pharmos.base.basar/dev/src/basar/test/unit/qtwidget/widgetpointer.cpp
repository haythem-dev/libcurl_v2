/*
* @file                                            
* @author Anke Klink                      
* @date 2008                            
*/    
#include <UnitTest++/UnitTest++.h>

#include "libbasarcmnutil.h"
#include "libbasarqtwidget.h"

#include "libbasarqtwidget_widgetpointer.h"
#include "loggerpool.h"

//------------------------------------------------------------------------------
SUITE(test_utqtwidget_widgetpointer)
{
	using basar::gui::widget::WidgetPointer;
	// ----------------------------------------------------------

	template <class T> 
	struct SPDeleter 
	{
		QPointer<QObject> m_inner;

		SPDeleter(QObject * p): m_inner(p)
		{
		};

		SPDeleter(int i) 
		{
		}

		SPDeleter(const SPDeleter & l):
		m_inner(l.m_inner)
		{

		}

		const SPDeleter & operator=(const SPDeleter & l) 
		{
			m_inner = l.m_inner;
			return *this;
		}

		void operator()(T * t) 
		{
			TEXTOUTPUT("SPDeleter::operator() called on " << t );
			if (!m_inner.isNull())
			{
				boost::checked_delete(t);
		    	TEXTOUTPUT("SPDeleter::operator() removed " << t );
			}
			m_inner = 0;
		}
	};

	//----------------------------------------------------------
	struct HelperWidgetPointer : public basar::gui::widget::WidgetPointer
	{
	public:
		int m_timesEntryDestroyed;
		int m_timesParentDestroyed;
		int m_timesSiblingDestroyed;

		enum What
		{
			Parent,
			Sibling,
			Entry
		};

		std::vector<What> m_last;

	public:
		HelperWidgetPointer(bool m_tryUseOlderSiblingMechanism):
		  WidgetPointer(m_tryUseOlderSiblingMechanism),
			  m_timesEntryDestroyed(0),
			  m_timesParentDestroyed(0),
			  m_timesSiblingDestroyed(0)
		  {
			  TEXTOUTPUT2("HelperWidgetPointer()");
		  }
		  virtual ~HelperWidgetPointer()
		  {
			  TEXTOUTPUT2("~HelperWidgetPointer()");
		  }
		  virtual void onDestroyParent()
		  {
			  TEXTOUTPUT2("onDestroyParent()");
			  m_timesParentDestroyed++;
			  m_last.push_back(Parent);
			  basar::gui::widget::WidgetPointer::onDestroyParent();
		  }
		  virtual void onDestroyEntry()
		  {
			  TEXTOUTPUT2("onDestroyEntry()");
			  m_timesEntryDestroyed++;
			  m_last.push_back(Entry);
			  basar::gui::widget::WidgetPointer::onDestroyEntry();
		  }

		  virtual void onDestroySibling()
		  {
			  TEXTOUTPUT2("onDestroySibling()");
			  m_timesSiblingDestroyed++;
			  m_last.push_back(Sibling);
			  basar::gui::widget::WidgetPointer::onDestroySibling();
		  }
		  //check last destroyed?
		  bool checkLast(What w)
		  {
			  bool retval = false;
			  std::vector<What>::reverse_iterator riter =  m_last.rbegin();
			  if  ( riter != m_last.rend())
			  {
				  retval = ( w == *riter);
			  }
			  return retval;
		  }
		  //check last n (times) destroyed?
		  //also checks order, in which objects were deleted
		  //if order is important -> no idea
		  bool checkLast(int times, const std::vector<What> & w)
		  {
			  bool retval = false;			 
			  if (static_cast<int>(w.size()) >= times)
			  {
				  bool ok = true;
				  int i=times-1;
				  std::vector<What>::reverse_iterator riter;
				  for (riter = m_last.rbegin(); riter != m_last.rend() && i > -1 /* && ok */; ++riter)
				  {
					  TEXTOUTPUT("checking["<< i << "]: " << w[i] << " == " << *riter);
					  ok = (ok && ( w[i] == *riter));
					  TEXTOUTPUT2("checked" );
					  --i;
				  }
				  retval = (ok && (i == -1)) ;
			  }
			  return retval;
		  }
	};
	//----------------------------------------------------------
	struct HelperWidgetPointer2 : public HelperWidgetPointer
	{
	public:

		boost::shared_ptr<QWidget> m_ptr;

	public:
		HelperWidgetPointer2(bool tryUseOlderSiblingMechanism):
		  HelperWidgetPointer(tryUseOlderSiblingMechanism)
		  {
			  // m_ptr = boost::shared_ptr<QWidget>(0, SPDeleter<QWidget>());
		  }

		  virtual ~HelperWidgetPointer2()
		  {
			  if (!getEntry().isNull())
			  {
				  m_ptr.reset();
			  }
		  }

		  void init()
		  {
			  prepairForNewEntry();
			  QWidget * w = new QWidget ;
			  SPDeleter<QWidget> a(w );
			  m_ptr   = boost::shared_ptr<QWidget >( w , a); 
			  setEntry(m_ptr.get());	  
		  }

		  virtual void onDestroyParent()
		  {
			  HelperWidgetPointer::onDestroyParent();
		  }
		  virtual void onDestroyEntry()
		  {
			  HelperWidgetPointer::onDestroyEntry();
		  }

		  virtual void onDestroySibling()
		  {
			  if (0 != getEntry())
			  {
				  getEntry()->hide();
				  setParent(0); 
			  }
			  HelperWidgetPointer::onDestroySibling();
		  }
	};
	//----------------------------------------------------------
	TEST (test_utqtwidget_widgetpointer)
	{
		try
		{
			basar::gui::widget::WidgetPointer widgetPointer; 
			basar::gui::widget::WidgetPointer widgetPointer2 = widgetPointer; 

			QWidget * widget1 = new QWidget(0);

			QWidget * widget2 = new QWidget(widget1);
			QWidget * widget3 = new QWidget(0);
			QWidget * widget4 = new QWidget(0);

			CHECK(0       == widgetPointer.getEntry());
			CHECK(0       == widgetPointer.getParent());

			widgetPointer.setEntryAndReadEntryParent(widget1);
			CHECK(widget1 == widgetPointer.getEntry());
			CHECK(0       == widgetPointer.getParent());
			CHECK(0       == widget1->parentWidget());

			widgetPointer.setEntryAndReadEntryParent(widget2);
			CHECK(widget2 == widgetPointer.getEntry());
			CHECK(widget1 == widgetPointer.getParent());
			CHECK(widget1 == widget2->parentWidget());

			CHECK(0       == widget3->parentWidget());
			widgetPointer.setEntry(widget3);
			CHECK(widget3 == widgetPointer.getEntry());
			CHECK(widget1 == widgetPointer.getParent());
			CHECK(widget1 == widget3->parentWidget());

			widgetPointer.setParent(widget4);
			CHECK(widget3 == widgetPointer.getEntry());
			CHECK(widget4 == widgetPointer.getParent());
			CHECK(widget4 == widget3->parentWidget());

			//widgetPointer.setParent(widget3);
			//CHECK(widget3 == widgetPointer.getEntry());
			//CHECK(widget3 == widgetPointer.getParent());
			//CHECK(widget3 == widget3->parentWidget());

			// clean up
			{
				if (!widget1->parentWidget())
					delete widget1;
				if (!widget4->parentWidget())
					delete widget4;
			}

		} 
		catch (basar::Exception & ex)
		{
			std::cout << "exception on test : " << ex.what() << std::endl;
			const bool InnerExceptionOccured = false;
			CHECK(InnerExceptionOccured); 
		} 
		catch (...)
		{
			std::cout << "unknown exception on test " << std::endl;
			const bool UnknownInnerExceptionOccured = false;
			CHECK(UnknownInnerExceptionOccured);
		}

	}
	// ----------------------------------------------------------
	TEST (test_utqtwidget_widgetpointer_signals_and_slots_1)
	{
		std::vector<bool> testvec(2);
		testvec[0] = true;
		testvec[1] = false;

		std::vector<bool>::const_iterator iter;
		for (iter = testvec.begin(); iter != testvec.end(); ++iter)
		{ 
			try
			{
				HelperWidgetPointer widgetPointer(*iter);
				int timesEntryDestroyed = 0;
				int timesParentDestroyed = 0;
				int timesSiblingDestroyed = 0;
				std::vector<HelperWidgetPointer::What> what;

				CHECK(0  == widgetPointer.getEntry());
				CHECK(0  == widgetPointer.getEntry());
				CHECK_EQUAL(0, widgetPointer.m_timesEntryDestroyed);
				CHECK_EQUAL(0, widgetPointer.m_timesParentDestroyed);
				CHECK_EQUAL(0, widgetPointer.m_timesSiblingDestroyed);
				// entry only:
				{ 
					QWidget widget2;
					widgetPointer.setEntryAndReadEntryParent(&widget2);
					CHECK(&widget2 == widgetPointer.getEntry());
					CHECK(0        == widgetPointer.getParent());
					CHECK(0        == widget2.parentWidget());
					timesEntryDestroyed++;
				}
				CHECK(0  == widgetPointer.getEntry());
				CHECK(0 == widgetPointer.getParent());
				CHECK_EQUAL(timesEntryDestroyed,  widgetPointer.m_timesEntryDestroyed);
				CHECK_EQUAL(timesParentDestroyed, widgetPointer.m_timesParentDestroyed);
				CHECK_EQUAL(timesSiblingDestroyed, widgetPointer.m_timesSiblingDestroyed);
				CHECK_EQUAL(1, static_cast<int>(widgetPointer.m_last.size())); 
				what.push_back(HelperWidgetPointer::Entry); 

				CHECK(widgetPointer.checkLast(static_cast<int>(what.size()),what));

				// parent only:
				{
					QWidget widget2;
					widgetPointer.setParent(&widget2);
					CHECK(&widget2 == widgetPointer.getParent());
					CHECK(0        == widgetPointer.getEntry());
					timesParentDestroyed++;
				}
				CHECK(0  == widgetPointer.getEntry());
				CHECK(0 == widgetPointer.getParent());
				CHECK_EQUAL(timesEntryDestroyed, widgetPointer.m_timesEntryDestroyed);
				CHECK_EQUAL(timesParentDestroyed, widgetPointer.m_timesParentDestroyed);
				CHECK_EQUAL(timesSiblingDestroyed, widgetPointer.m_timesSiblingDestroyed);

				what.push_back(HelperWidgetPointer::Parent); 

				CHECK(widgetPointer.checkLast(static_cast<int>(what.size()),what));
				CHECK_EQUAL(static_cast<int>(what.size()), static_cast<int>(widgetPointer.m_last.size()));
			} 
			catch (basar::Exception & ex)
			{
				std::cout << "exception on test : " << ex.what() << std::endl;
				const bool InnerExceptionOccured = false;
				CHECK(InnerExceptionOccured); 
			} 
			catch (...)
			{
				std::cout << "unknown exception on test with method " << std::endl;
				const bool UnknownInnerExceptionOccured = false;
				CHECK(UnknownInnerExceptionOccured);
			}
		}
	}

	// ----------------------------------------------------------
	TEST (test_utqtwidget_widgetpointer_signals_and_slots_2)
	{
		std::vector<bool> testvec(2);
		testvec[0] = true;
		testvec[1] = false;

		std::vector<bool>::const_iterator iter;
		for (iter = testvec.begin(); iter != testvec.end(); ++iter)
		{
			try
			{
				bool useSib = *iter;
				HelperWidgetPointer widgetPointer(useSib);
				int timesEntryDestroyed = 0;
				int timesParentDestroyed = 0;
				std::vector<HelperWidgetPointer::What> what;

				CHECK(0 == widgetPointer.getEntry());
				CHECK(0 == widgetPointer.getParent());
				CHECK_EQUAL(0, widgetPointer.m_timesEntryDestroyed);
				CHECK_EQUAL(0, widgetPointer.m_timesParentDestroyed);
				// parent with longliving widget :
				{
					QPointer<QWidget> widget1 = new QWidget(0);			
					{ 
						QWidget widget2;
						//widget1->setParent(&widget2);
						widgetPointer.setEntry(widget1);
						widgetPointer.setParent(&widget2);

						CHECK(&widget2 == widgetPointer.getParent());
						CHECK(widget1 == widgetPointer.getEntry());
						timesEntryDestroyed++;
						timesParentDestroyed++;
					}
					CHECK(0          == widgetPointer.getEntry());
					CHECK(0          == widgetPointer.getParent());
					CHECK_EQUAL(timesEntryDestroyed,  widgetPointer.m_timesEntryDestroyed);
					CHECK_EQUAL(timesParentDestroyed, widgetPointer.m_timesParentDestroyed);
					CHECK(widget1.isNull());
					
					what.push_back(HelperWidgetPointer::Parent);
					if (useSib)
					{
						what.push_back(HelperWidgetPointer::Sibling);
					}
					what.push_back(HelperWidgetPointer::Entry);
					
					CHECK_EQUAL(static_cast<int>(what.size()), static_cast<int>(widgetPointer.m_last.size()));
					CHECK(widgetPointer.checkLast(static_cast<int>(what.size()),what));
				}
				CHECK(0  == widgetPointer.getEntry());
				CHECK(0 == widgetPointer.getParent());
				CHECK_EQUAL(timesEntryDestroyed, widgetPointer.m_timesEntryDestroyed);
				CHECK_EQUAL(timesParentDestroyed, widgetPointer.m_timesParentDestroyed);
				CHECK_EQUAL(static_cast<int>(what.size()), static_cast<int>(widgetPointer.m_last.size()));
				CHECK(widgetPointer.checkLast(static_cast<int>(what.size()),what));
			}
			catch (basar::Exception & ex)
			{
				std::cout << "exception on test : " 
					<< ex.what() << std::endl;
				const bool InnerExceptionOccured = false;
				CHECK(InnerExceptionOccured); 
			} 
			catch (...)
			{
				std::cout << "unknown exception on test with method " << std::endl;
				const bool UnknownInnerExceptionOccured = false;
				CHECK(UnknownInnerExceptionOccured);
			}
		}
	}

	// ----------------------------------------------------------
	TEST (test_utqtwidget_widgetpointer_signals_and_slots_3)
	{
		std::vector<bool> testvec(2);
		testvec[0] = true;
		testvec[1] = false;

		std::vector<bool>::const_iterator iter;
		for (iter = testvec.begin(); iter != testvec.end(); ++iter)
		{
			try
			{
				bool useSib = *iter;
				HelperWidgetPointer widgetPointer(useSib);
				int timesEntryDestroyed = 0;
				int timesParentDestroyed = 0;
				std::vector<HelperWidgetPointer::What> what;

				CHECK(0 == widgetPointer.getEntry());
				CHECK(0 == widgetPointer.getParent());
				CHECK_EQUAL(0, widgetPointer.m_timesEntryDestroyed);
				CHECK_EQUAL(0, widgetPointer.m_timesParentDestroyed);
				// parent with longliving widget :
				{
					QPointer<QWidget> widget1 = new QWidget(0);			
					{ 
						QWidget widget2;
						widget1->setParent(&widget2);
						widgetPointer.setEntryAndReadEntryParent(widget1);

						CHECK(&widget2 == widgetPointer.getParent());
						CHECK(widget1 == widgetPointer.getEntry());
						timesEntryDestroyed++;
						timesParentDestroyed++;
					}
					CHECK(0          == widgetPointer.getEntry());
					CHECK(0          == widgetPointer.getParent());
					CHECK_EQUAL(timesEntryDestroyed,  widgetPointer.m_timesEntryDestroyed);
					CHECK_EQUAL(timesParentDestroyed, widgetPointer.m_timesParentDestroyed);
					CHECK(widget1.isNull());


					what.push_back(HelperWidgetPointer::Parent);
					// different:
					//what.push_back(HelperWidgetPointer::Sibling);

					what.push_back(HelperWidgetPointer::Entry);
					
					CHECK_EQUAL(static_cast<int>(what.size()), static_cast<int>(widgetPointer.m_last.size()));
					CHECK(widgetPointer.checkLast(static_cast<int>(what.size()),what));
				}
				CHECK(0  == widgetPointer.getEntry());
				CHECK(0 == widgetPointer.getParent());
				CHECK_EQUAL(timesEntryDestroyed, widgetPointer.m_timesEntryDestroyed);
				CHECK_EQUAL(timesParentDestroyed, widgetPointer.m_timesParentDestroyed);

				CHECK_EQUAL(static_cast<int>(what.size()), static_cast<int>(widgetPointer.m_last.size()));
				CHECK(widgetPointer.checkLast(static_cast<int>(what.size()),what));
			}
			catch (basar::Exception & ex)
			{
				std::cout << "exception on test : " 
					<< ex.what() << std::endl;
				const bool InnerExceptionOccured = false;
				CHECK(InnerExceptionOccured); 
			} 
			catch (...)
			{
				std::cout << "unknown exception on test with method " << std::endl;
				const bool UnknownInnerExceptionOccured = false;
				CHECK(UnknownInnerExceptionOccured);
			}
		}
	}

	// ----------------------------------------------------------
	TEST (test_utqtwidget_widgetpointer_signals_and_slots_HelperWidgetPointer2_1)
	{
		std::vector<bool> testvec(2);
		testvec[0] = true;
		testvec[1] = false;

		std::vector<bool>::const_iterator iter;
		for (iter = testvec.begin(); iter != testvec.end(); ++iter)
		{
			try
			{
				bool useSib = *iter;
				HelperWidgetPointer2 widgetPointer(useSib);
				int timesEntryDestroyed = 0;
				int timesParentDestroyed = 0;
				std::vector<HelperWidgetPointer::What> what;

				CHECK(0          == widgetPointer.getEntry());
				CHECK(0          == widgetPointer.getParent());
				CHECK_EQUAL(timesEntryDestroyed, widgetPointer.m_timesEntryDestroyed);
				CHECK_EQUAL(timesParentDestroyed, widgetPointer.m_timesParentDestroyed);

				widgetPointer.init();

				CHECK(0          != widgetPointer.getEntry());
				CHECK(0          == widgetPointer.getParent());
				CHECK_EQUAL(timesEntryDestroyed, widgetPointer.m_timesEntryDestroyed);
				CHECK_EQUAL(timesParentDestroyed, widgetPointer.m_timesParentDestroyed);


				widgetPointer.m_ptr.reset();
				++timesEntryDestroyed; 
				what.push_back(HelperWidgetPointer::Entry); 


				CHECK(0          == widgetPointer.getEntry());
				CHECK(0          == widgetPointer.getParent());
				CHECK_EQUAL(timesEntryDestroyed, widgetPointer.m_timesEntryDestroyed);
				CHECK_EQUAL(timesParentDestroyed, widgetPointer.m_timesParentDestroyed);


				CHECK_EQUAL(static_cast<int>(what.size()), static_cast<int>(widgetPointer.m_last.size()));
				CHECK(widgetPointer.checkLast(static_cast<int>(what.size()),what));

				CHECK_EQUAL(static_cast<int>(what.size()), static_cast<int>(widgetPointer.m_last.size()));
				CHECK(widgetPointer.checkLast(static_cast<int>(what.size()),what));
			}
			catch (basar::Exception & ex)
			{
				std::cout << "exception on test : " 
					<< ex.what() << std::endl;
				const bool InnerExceptionOccured = false;
				CHECK(InnerExceptionOccured); 
			} 
			catch (...)
			{
				std::cout << "unknown exception on test with method " << std::endl;
				const bool UnknownInnerExceptionOccured = false;
				CHECK(UnknownInnerExceptionOccured);
			}
		}
	}
	// ----------------------------------------------------------
	TEST (test_utqtwidget_widgetpointer_signals_and_slots_HelperWidgetPointer2_2)
	{
		std::vector<bool> testvec(2);
		testvec[0] = true;
		testvec[1] = false;

		std::vector<bool>::const_iterator iter;
		for (iter = testvec.begin(); iter != testvec.end(); ++iter)
		{
			try
			{
				bool useSib = *iter;
				HelperWidgetPointer2 widgetPointer(useSib);
				int timesEntryDestroyed = 0;
				int timesParentDestroyed = 0;
				std::vector<HelperWidgetPointer::What> what;

				CHECK(0          == widgetPointer.getEntry());
				CHECK(0          == widgetPointer.getParent());
				CHECK_EQUAL(timesEntryDestroyed, widgetPointer.m_timesEntryDestroyed);
				CHECK_EQUAL(timesParentDestroyed, widgetPointer.m_timesParentDestroyed);

				widgetPointer.init();

				CHECK(0          != widgetPointer.getEntry());
				CHECK(0          == widgetPointer.getParent());
				CHECK_EQUAL(timesEntryDestroyed, widgetPointer.m_timesEntryDestroyed);
				CHECK_EQUAL(timesParentDestroyed, widgetPointer.m_timesParentDestroyed);


				delete widgetPointer.getEntry(); //double delete - during delete boost::shared_ptr is reset


				++timesEntryDestroyed; 
				what.push_back(HelperWidgetPointer::Entry); 

				CHECK(0          == widgetPointer.getEntry());
				CHECK(0          == widgetPointer.getParent());
				CHECK_EQUAL(timesEntryDestroyed, widgetPointer.m_timesEntryDestroyed);
				CHECK_EQUAL(timesParentDestroyed, widgetPointer.m_timesParentDestroyed);


				CHECK_EQUAL(static_cast<int>(what.size()), static_cast<int>(widgetPointer.m_last.size()));
				CHECK(widgetPointer.checkLast(static_cast<int>(what.size()),what));

				CHECK_EQUAL(static_cast<int>(what.size()), static_cast<int>(widgetPointer.m_last.size()));
				CHECK(widgetPointer.checkLast(static_cast<int>(what.size()),what));
			}
			catch (basar::Exception & ex)
			{
				std::cout << "exception on test : " 
					<< ex.what() << std::endl;
				const bool InnerExceptionOccured = false;
				CHECK(InnerExceptionOccured); 
			} 
			catch (...)
			{
				std::cout << "unknown exception on test with method " << std::endl;
				const bool UnknownInnerExceptionOccured = false;
				CHECK(UnknownInnerExceptionOccured);
			}
		}
	}

	// ----------------------------------------------------------
	TEST (test_utqtwidget_widgetpointer_signals_and_slots_HelperWidgetPointer2_3)
	{
		std::vector<bool> testvec(2);
		testvec[0] = true;
		testvec[1] = false;
		std::vector<bool>::const_iterator iter;
		for (iter = testvec.begin(); iter != testvec.end(); ++iter)
		{
			try
			{
				WidgetPointer upperWidgetPointer;
				{
					bool useSib = *iter;
					HelperWidgetPointer2 widgetPointer(useSib);
					int timesEntryDestroyed = 0;
					int timesParentDestroyed = 0;
					std::vector<HelperWidgetPointer::What> what;

					CHECK(0          == widgetPointer.getEntry());
					CHECK(0          == widgetPointer.getParent());
					CHECK_EQUAL(timesEntryDestroyed, widgetPointer.m_timesEntryDestroyed);
					CHECK_EQUAL(timesParentDestroyed, widgetPointer.m_timesParentDestroyed);

					widgetPointer.init();

					CHECK(0          != widgetPointer.getEntry());
					CHECK(0          == widgetPointer.getParent());
					CHECK_EQUAL(timesEntryDestroyed, widgetPointer.m_timesEntryDestroyed);
					CHECK_EQUAL(timesParentDestroyed, widgetPointer.m_timesParentDestroyed);

					upperWidgetPointer.setEntry(widgetPointer.getEntry());
					CHECK(widgetPointer.getEntry() == upperWidgetPointer.getEntry());
					CHECK(0                        == upperWidgetPointer.getParent());
				}
				CHECK(0  == upperWidgetPointer.getEntry());
				CHECK(0  == upperWidgetPointer.getParent());
			}
			catch (basar::Exception & ex)
			{
				std::cout << "exception on test : " 
					<< ex.what() << std::endl;
				const bool InnerExceptionOccured = false;
				CHECK(InnerExceptionOccured); 
			} 
			catch (...)
			{
				std::cout << "unknown exception on test with method " << std::endl;
				const bool UnknownInnerExceptionOccured = false;
				CHECK(UnknownInnerExceptionOccured);
			}
		}
	}

	// ----------------------------------------------------------
	TEST (test_utqtwidget_widgetpointer_signals_and_slots_HelperWidgetPointer2_4)
	{
		std::multimap<bool, bool> testvec;
		testvec.insert(std::pair<bool, bool>(true, true));
		testvec.insert(std::pair<bool, bool>(true, false));
		testvec.insert(std::pair<bool, bool>(false, false));
		testvec.insert(std::pair<bool, bool>(false, true));
		std::multimap<bool, bool>::const_iterator iter;
		for (iter = testvec.begin(); iter != testvec.end(); ++iter)
		{
			try
			{
				bool useSib1 = iter->first;
				bool useSib2 = iter->second;
				WidgetPointer upperWidgetPointer;
				{
					HelperWidgetPointer2 widgetPointer2(useSib2);
					{
						HelperWidgetPointer2 widgetPointer1(useSib1);

						int timesEntryDestroyed = 0;
						int timesParentDestroyed = 0;
						std::vector<HelperWidgetPointer::What> what;

						CHECK(0          == widgetPointer1.getEntry());
						CHECK(0          == widgetPointer1.getParent());
						CHECK_EQUAL(timesEntryDestroyed, widgetPointer1.m_timesEntryDestroyed);
						CHECK_EQUAL(timesParentDestroyed, widgetPointer1.m_timesParentDestroyed);

						CHECK(0          == widgetPointer2.getEntry());
						CHECK(0          == widgetPointer2.getParent());
						CHECK_EQUAL(timesEntryDestroyed, widgetPointer2.m_timesEntryDestroyed);
						CHECK_EQUAL(timesParentDestroyed, widgetPointer2.m_timesParentDestroyed);

						widgetPointer1.init();
						QPointer<QWidget> w1 = widgetPointer1.getEntry();

						CHECK(0          != widgetPointer1.getEntry());
						CHECK(0          == widgetPointer1.getParent());
						CHECK_EQUAL(timesEntryDestroyed, widgetPointer1.m_timesEntryDestroyed);
						CHECK_EQUAL(timesParentDestroyed, widgetPointer1.m_timesParentDestroyed);

						CHECK(0          == widgetPointer2.getEntry());
						CHECK(0          == widgetPointer2.getParent());
						CHECK_EQUAL(timesEntryDestroyed, widgetPointer2.m_timesEntryDestroyed);
						CHECK_EQUAL(timesParentDestroyed, widgetPointer2.m_timesParentDestroyed);

						widgetPointer2.setParent(widgetPointer1.getEntry());

						CHECK(w1         == widgetPointer1.getEntry());
						CHECK(0          == widgetPointer1.getParent());
						CHECK_EQUAL(timesEntryDestroyed, widgetPointer1.m_timesEntryDestroyed);
						CHECK_EQUAL(timesParentDestroyed, widgetPointer1.m_timesParentDestroyed);

						CHECK(0           == widgetPointer2.getEntry());
						CHECK(w1          == widgetPointer2.getParent());
						CHECK_EQUAL(timesEntryDestroyed, widgetPointer2.m_timesEntryDestroyed);
						CHECK_EQUAL(timesParentDestroyed, widgetPointer2.m_timesParentDestroyed);

						widgetPointer2.init();
						QPointer<QWidget> w2 = widgetPointer2.getEntry();

						CHECK(w1         == widgetPointer1.getEntry());
						CHECK(0          == widgetPointer1.getParent());
						CHECK_EQUAL(timesEntryDestroyed, widgetPointer1.m_timesEntryDestroyed);
						CHECK_EQUAL(timesParentDestroyed, widgetPointer1.m_timesParentDestroyed);

						CHECK(0           != widgetPointer2.getEntry());
						CHECK(w2          == widgetPointer2.getEntry());
						CHECK(w1          == widgetPointer2.getParent());
						CHECK_EQUAL(timesEntryDestroyed, widgetPointer2.m_timesEntryDestroyed);
						CHECK_EQUAL(timesParentDestroyed, widgetPointer2.m_timesParentDestroyed);

						upperWidgetPointer.setEntry(w1);
					}
				}
				CHECK(0  == upperWidgetPointer.getEntry());
				CHECK(0  == upperWidgetPointer.getParent());
			}
			catch (basar::Exception & ex)
			{
				std::cout << "exception on test : " 
					<< ex.what() << std::endl;
				const bool InnerExceptionOccured = false;
				CHECK(InnerExceptionOccured); 
			} 
			catch (...)
			{
				std::cout << "unknown exception on test with method " << std::endl;
				const bool UnknownInnerExceptionOccured = false;
				CHECK(UnknownInnerExceptionOccured);
			}
		}
	}
	// ----------------------------------------------------------

	TEST (test_utqtwidget_widgetpointer_signals_and_slots_HelperWidgetPointer2_5)
	{
		std::multimap<bool, bool> testvec;
		testvec.insert(std::pair<bool, bool>(true, true));
		testvec.insert(std::pair<bool, bool>(true, false));
		testvec.insert(std::pair<bool, bool>(false, false));
		testvec.insert(std::pair<bool, bool>(false, true));
		std::multimap<bool, bool>::const_iterator iter;
		for (iter = testvec.begin(); iter != testvec.end(); ++iter)
		{
			try
			{
				bool useSib1 = iter->first;
				bool useSib2 = iter->second;
				WidgetPointer upperWidgetPointer;
				{
					HelperWidgetPointer2 widgetPointer2(useSib2);
					{
						HelperWidgetPointer2 widgetPointer1(useSib1);

						int timesEntryDestroyed = 0;
						int timesParentDestroyed = 0;
						std::vector<HelperWidgetPointer::What> what;

						CHECK(0          == widgetPointer1.getEntry());
						CHECK(0          == widgetPointer1.getParent());
						CHECK_EQUAL(timesEntryDestroyed, widgetPointer1.m_timesEntryDestroyed);
						CHECK_EQUAL(timesParentDestroyed, widgetPointer1.m_timesParentDestroyed);

						CHECK(0          == widgetPointer2.getEntry());
						CHECK(0          == widgetPointer2.getParent());
						CHECK_EQUAL(timesEntryDestroyed, widgetPointer2.m_timesEntryDestroyed);
						CHECK_EQUAL(timesParentDestroyed, widgetPointer2.m_timesParentDestroyed);

						widgetPointer1.init();
						QPointer<QWidget> w1 = widgetPointer1.getEntry();

						CHECK(0          != widgetPointer1.getEntry());
						CHECK(0          == widgetPointer1.getParent());
						CHECK_EQUAL(timesEntryDestroyed, widgetPointer1.m_timesEntryDestroyed);
						CHECK_EQUAL(timesParentDestroyed, widgetPointer1.m_timesParentDestroyed);

						CHECK(0          == widgetPointer2.getEntry());
						CHECK(0          == widgetPointer2.getParent());
						CHECK_EQUAL(timesEntryDestroyed, widgetPointer2.m_timesEntryDestroyed);
						CHECK_EQUAL(timesParentDestroyed, widgetPointer2.m_timesParentDestroyed);

						widgetPointer2.setParent(widgetPointer1.getEntry());

						CHECK(w1         == widgetPointer1.getEntry());
						CHECK(0          == widgetPointer1.getParent());
						CHECK_EQUAL(timesEntryDestroyed, widgetPointer1.m_timesEntryDestroyed);
						CHECK_EQUAL(timesParentDestroyed, widgetPointer1.m_timesParentDestroyed);

						CHECK(0           == widgetPointer2.getEntry());
						CHECK(w1          == widgetPointer2.getParent());
						CHECK_EQUAL(timesEntryDestroyed, widgetPointer2.m_timesEntryDestroyed);
						CHECK_EQUAL(timesParentDestroyed, widgetPointer2.m_timesParentDestroyed);

						widgetPointer2.init();
						QPointer<QWidget> w2 = widgetPointer2.getEntry();

						CHECK(w1         == widgetPointer1.getEntry());
						CHECK(0          == widgetPointer1.getParent());
						CHECK_EQUAL(timesEntryDestroyed, widgetPointer1.m_timesEntryDestroyed);
						CHECK_EQUAL(timesParentDestroyed, widgetPointer1.m_timesParentDestroyed);

						CHECK(0           != widgetPointer2.getEntry());
						CHECK(w2          == widgetPointer2.getEntry());
						CHECK(w1          == widgetPointer2.getParent());
						CHECK_EQUAL(timesEntryDestroyed, widgetPointer2.m_timesEntryDestroyed);
						CHECK_EQUAL(timesParentDestroyed, widgetPointer2.m_timesParentDestroyed);

						upperWidgetPointer.setEntry(w1);
					}

				}
				CHECK(0  == upperWidgetPointer.getEntry());
				CHECK(0  == upperWidgetPointer.getParent());
			}
			catch (basar::Exception & ex)
			{
				std::cout << "exception on test : " 
					<< ex.what() << std::endl;
				const bool InnerExceptionOccured = false;
				CHECK(InnerExceptionOccured); 
			} 
			catch (...)
			{
				std::cout << "unknown exception on test with method " << std::endl;
				const bool UnknownInnerExceptionOccured = false;
				CHECK(UnknownInnerExceptionOccured);
			}
		}
	}
	// ----------------------------------------------------------
}
// ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~  ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~
