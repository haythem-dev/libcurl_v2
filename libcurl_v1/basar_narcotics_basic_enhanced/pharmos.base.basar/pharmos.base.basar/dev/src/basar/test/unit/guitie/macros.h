 /** $Id$                                          
  *                                                  
  * @file                                            
  * @author Anke Klink                      
  * @date 2008                            
  */    
                                     
#ifndef  __BASAR_TEST_UNIT_GUITIE_MACROS_H__ 
#define  __BASAR_TEST_UNIT_GUITIE_MACROS_H__ 

#include "direction.h"

 #define CHECK_DIRECTION(dir, one, two) \
    { \
    if (dir == basar::test::unit::guitie::Direction::LeftToRight)  \
        {  \
            CHECK(two == one); \
        } \
        else \
         { \
            CHECK(one == two); \
        } \
    } \

 #define CHECK_EQUAL_DIRECTION(dir, one, two) \
    { \
    if (dir == basar::test::unit::guitie::Direction::LeftToRight)  \
        {  \
            CHECK_EQUAL(two, one); \
        } \
        else \
         { \
            CHECK_EQUAL(one, two); \
        } \
    } \

#define CHECK_CLOSE_DIRECTION(dir, one, two, close) \
    { \
    if (dir == basar::test::unit::guitie::Direction::LeftToRight)   \
        {  \
            CHECK_CLOSE(two, one, close); \
        } \
        else \
         { \
            CHECK_CLOSE(one, two,close); \
        } \
    } \

#endif 

// ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~  ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~
