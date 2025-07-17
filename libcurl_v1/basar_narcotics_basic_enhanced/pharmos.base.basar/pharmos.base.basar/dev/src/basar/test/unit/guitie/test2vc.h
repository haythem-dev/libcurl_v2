/** $Id$                                          
*                                                  
* @file                                            
* @author Anke Klink                      
* @date 2009                            
*/    


#ifndef  __BASAR_TEST_UNIT_GUITIE_TEST2VC_H__ 
#define  __BASAR_TEST_UNIT_GUITIE_TEST2VC_H__ 
 
#include <boost/enable_shared_from_this.hpp>

namespace basar
{
	namespace test
	{
		namespace unit
		{
			namespace guitie
			{
				/*!
				* Testclass 
				*/
				class Test2VC : 
					public QWidget , 
					public boost::enable_shared_from_this<Test2VC>,
					public basar::gui::tie::IViewConnector
				{      
				public: 

					Test2VC(QWidget * parent = 0);
					virtual ~Test2VC();

					basar::gui::tie::ViewConnPtr<Test2VC> getViewConnPtr();

					/*! \brief show vc's dialog
					\n no-throw */
					virtual basar::gui::tie::WidgetReturnEnum	show		();

					/*! \brief hide vc's dialog
					\n no-throw */
					virtual void								hide		();

					/*! \brief close vc's dialog
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
