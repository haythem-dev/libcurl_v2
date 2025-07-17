/** $Id$
 *
 * @file
 * @author Anke Klink
 * @date 2009
 */


#ifndef  __BASAR_TEST_UNIT_GUITIE_TESTEXCEPTIONTHROWER_H__ 
#define  __BASAR_TEST_UNIT_GUITIE_TESTEXCEPTIONTHROWER_H__ 

namespace basar
 {
  namespace test
   {
    namespace unit
     {
      namespace guitie
       {
        /**
         * Class: testexceptionthrower
         *
         */
        class TestExceptionThrower
         {
          public:
			void throwBasarException(bool);
			void throwSTDException(bool);
			void throwUnknownException(bool);

			void TestExceptionThrower::throwBasarException( );
			void TestExceptionThrower::throwSTDException( );
			void TestExceptionThrower::throwUnknownException();

         };

       }
     }
   }
 }
#endif 

// ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~  ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~
