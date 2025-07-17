/** $Id$                                          
*                                                  
* @file                                            
* @author Anke Klink                      
* @date 2009                           
*/    



#include <UnitTest++/UnitTest++.h>
#include <libbasarcmnutil.h>
#include <libbasardbaspect.h>
#include <libbasarappl.h>
#include <libbasarguitie.h>
#include <libbasardbaspect_usings.h>
#include "indexcountholder.h"

#include "config.h"



#include "TestWidget.h"


namespace basar
{
	namespace test
	{
		namespace unit
		{
			namespace guitie
			{
				TestWidget1::TestWidget1(QWidget * parent ): QWidget(parent)
				{ 
				}

				TestWidget1::~TestWidget1()
				{ 
				}


				basar::gui::tie::WidgetReturnEnum	TestWidget1::show		()
				{
					// do nothing
					return basar::gui::tie::WidgetReturnEnum();
				}

				void								TestWidget1::hide		()
				{
					// do nothing
				}

				bool								TestWidget1::shutdown	() 
				{
					// do nothing
					return true;
				}

				void								TestWidget1::use		()
				{
					// do nothing
				}

				basar::gui::tie::ViewConnPtr<TestWidget1> TestWidget1::getViewConnPtr()
				{ 
					return basar::gui::tie::ViewConnPtr<TestWidget1>(shared_from_this());
				}
			}
		}
	}
}
// ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~  ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~
