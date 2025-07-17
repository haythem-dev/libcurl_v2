/** $Id$                                          
*                                                  
* @file                                            
* @author Anke Klink                      
* @date 2009                            
*/    

#include <UnitTest++/UnitTest++.h>
#include "libbasarguitie.h"

#include "test1vc.h"


namespace basar
{
	namespace test
	{
		namespace unit
		{
			namespace guitie
			{
				Test1VC::Test1VC(QWidget * parent ): QWidget(parent)
				{ 
				}

				Test1VC::~Test1VC()
				{ 
				}


				basar::gui::tie::WidgetReturnEnum	Test1VC::show ()
				{
					// do nothing
					return basar::gui::tie::WidgetReturnEnum();
				}

				void Test1VC::hide ()
				{
					// do nothing
				}

				bool Test1VC::shutdown	() 
				{
					// do nothing
					return true;
				}

				void Test1VC::use()
				{
					// do nothing
				}

				basar::gui::tie::ViewConnPtr<Test1VC> Test1VC::getViewConnPtr()
				{ 
					return basar::gui::tie::ViewConnPtr<Test1VC>(shared_from_this());
				}
			}
		}
	}
}
// ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~  ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~
