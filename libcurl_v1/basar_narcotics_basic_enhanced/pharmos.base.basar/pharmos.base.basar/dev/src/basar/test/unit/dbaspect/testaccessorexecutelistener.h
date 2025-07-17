/** $Id$                                          
*                                                  
* @file accessorexecutelistener.h                                  
* @namespace                   
* @brief ...                                       
* @author Anke Klink                      
* @date 2007                            
*/    


#ifndef  ___GUARD_BASAR_TEST_UNIT_DBASPECT_ACCESSOREXECUTELISTENER_H__ 
#define  ___GUARD_BASAR_TEST_UNIT_DBASPECT_ACCESSOREXECUTELISTENER_H__ 

namespace basar
{
	namespace db
	{
		namespace aspect
		{
			class AccessorExecuteEvent;
		}
	}

    namespace test
    {
        namespace unit
        {
            namespace dbaspect
            {
                struct AccessorExecuteEventHolder
                {
                   boost::shared_ptr<basar::db::aspect::AccessorExecuteEvent> m_lastEvent;
                   int m_times;
                };

                BEGIN_ACCESSOREXECUTELISTENER_DECLARATION(TestAccessorExecuteListenerStdOutLog)
                END_ACCESSOREXECUTELISTENER_DECLARATION

               
                BEGIN_ACCESSOREXECUTELISTENER_DECLARATION(TestAccessorExecuteListenerHoldLastEvent)
  
                public:
                    TestAccessorExecuteListenerHoldLastEvent();
                boost::shared_ptr<AccessorExecuteEventHolder> m_lastEvent;
                END_ACCESSOREXECUTELISTENER_DECLARATION


            }
        }
    }
}

#endif 

// ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~  ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~
