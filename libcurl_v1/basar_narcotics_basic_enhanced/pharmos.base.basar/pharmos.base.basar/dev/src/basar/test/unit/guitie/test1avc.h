/** $Id$                                          
*                                                  
* @file                                            
* @author Anke Klink                      
* @date 2009                            
*/    


#ifndef  __BASAR_TEST_UNIT_GUITIE_TEST1AVC_H__ 
#define  __BASAR_TEST_UNIT_GUITIE_TEST1AVC_H__ 

#include "test1vc.h"


namespace basar
{
	namespace test
	{
		namespace unit
		{
			namespace guitie
			{
				/**
				* Testclass
				*/ 
				class Test1AVC : public  Test1VC
				{      
				private: 
				protected: 
				public: 
					Test1AVC(QWidget * parent = 0);
					virtual ~Test1AVC();

				private:

					// can be defined if neccessary:
					Test1AVC(const Test1AVC &);
					Test1AVC & operator=(const Test1AVC &);

				}; 

			}
		}
	}
}
#endif 

// ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~  ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~
