/** $Id$                                          
*                                                  
* @file                                            
* @author Anke Klink                      
* @date 2009                            
*/    


#ifndef  __BASAR_TEST_UNIT_GUITIE_TestWidget1_H__ 
#define  __BASAR_TEST_UNIT_GUITIE_TestWidget1_H__ 

#pragma warning (push)
#pragma warning(disable: 4127 4244 4251 4311 4312 4512 4800)
#include <QT/QWidget.h>
#include <QT/QTableWidget.h>
#pragma warning (pop)

namespace basar
{
	namespace test
	{
		namespace unit
		{
			namespace guitie
			{
				/**
				* Class: TestWidget1 
				*  
				*/ 
				class TestWidget1 : 
					public QWidget , 
					public boost::enable_shared_from_this<TestWidget1>,
					public basar::gui::tie::IViewConnector
				{      
				public: 

					TestWidget1(QWidget * parent = 0);
					virtual ~TestWidget1();

					basar::gui::tie::ViewConnPtr<TestWidget1> getViewConnPtr();

					/*! \brief show vc´s dialog
					\n no-throw */
					virtual basar::gui::tie::WidgetReturnEnum	show		();

					/*! \brief hide vc´s dialog
					\n no-throw */
					virtual void								hide		();

					/*! \brief close vc´s dialog
					\n no-throw */
					virtual bool								shutdown	() ;
					void							        	use	() ;		
				}; 

			}
		}
	}
}
#endif 

// ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~  ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~
