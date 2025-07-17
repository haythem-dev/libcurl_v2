 /** $Id$                                          
  *                                                  
  * @file                                            
  * @author Anke Klink                      
  * @date 2008                            
  */    
                                      
#ifndef  __BASAR_TRIAL_GUITIE_CONNECTIONHOLDER_H__ 
#define  __BASAR_TRIAL_GUITIE_CONNECTIONHOLDER_H__ 

 #include "usings.h"

 namespace basar_trial
  {
   namespace guitie
    {
      /**
       * Class: TestConnection 
       *  
       */ 
      class TestConnection 
       {      
        public: 
            static basar::db::aspect::ConnectionRef  getBasarTestConnection();

       }; 
 
    }
  }
#endif 

// ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~  ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~
