/** $Id$                                          
*                                                  
* @file                                            
* @author Anke Klink                      
* @date 2009                            
*/    

#include <UnitTest++/UnitTest++.h>
#include "libbasarguitie.h"

#include "test2vc.h"


namespace basar
{
	namespace test
	{
		namespace unit
		{
			namespace guitie
			{
				Test2VC::Test2VC(QWidget * parent ): QWidget(parent)
				{ 
				}

				Test2VC::~Test2VC()
				{ 
				}


				basar::gui::tie::WidgetReturnEnum	Test2VC::show ()
				{
					// do nothing
					return basar::gui::tie::WidgetReturnEnum();
				}

				void Test2VC::hide ()
				{
					// do nothing
				}

				bool Test2VC::shutdown	() 
				{
					// do nothing
					return true;
				}

				void Test2VC::use()
				{
					// do nothing
				}

				basar::gui::tie::ViewConnPtr<Test2VC> Test2VC::getViewConnPtr()
				{ 
					return basar::gui::tie::ViewConnPtr<Test2VC>(shared_from_this());
				}
			}
		}
	}
}
// ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~  ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~
