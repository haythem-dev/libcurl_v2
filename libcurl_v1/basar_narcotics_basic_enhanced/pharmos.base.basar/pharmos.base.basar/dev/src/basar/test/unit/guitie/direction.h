/** $Id$                                          
*                                                  
* @file                                            
* @author Anke Klink                      
* @date 2008                            
*/    


#ifndef  __BASAR_TEST_UNIT_GUITIE_DIRECTION_H__ 
#define  __BASAR_TEST_UNIT_GUITIE_DIRECTION_H__ 

namespace basar
{
	namespace test
	{
		namespace unit
		{
			namespace guitie
			{
				/*!
				* Direction for matcher (LeftToRight, RightToLeft)
				* asd Enum for Tests
			    */
				struct Direction
				{
					enum DirEnum
					{
						LeftToRight,
						LR = LeftToRight,
						RightToLeft,
						RL = RightToLeft
					};
				};
			}
		}
	}
}
#endif 

// ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~  ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~
