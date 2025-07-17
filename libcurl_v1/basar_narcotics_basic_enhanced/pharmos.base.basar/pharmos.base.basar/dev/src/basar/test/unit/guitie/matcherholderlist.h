/** $Id$                                          
*                                                  
* @file                                            
* @author Anke Klink                      
* @date 2008                            
*/    

#ifndef  __BASAR_TEST_UNIT_GUITIE_MATCHERHOLDERLIST_H__ 
#define  __BASAR_TEST_UNIT_GUITIE_MATCHERHOLDERLIST_H__ 

#include "basematcherholder.h"

namespace basar
{
    namespace test
    {
        namespace unit
        {
            namespace guitie
            {
		
				/**
				 * List of boost::shared_ptr<T> with T as a 
				 * childclass to BaseMatcherHolder, is used in 
				 * struct TwoStringMatcherHolderMoreLines and 
				 * struct TwoStringMatcherHolderOneLine 
				 */ 
                template <class T>
				class MatcherHolderList
                {
                public: 
					std::vector<boost::shared_ptr<T> > m_list;

                    MatcherHolderList()
                    {
                    }
                    virtual ~MatcherHolderList()
                    {
                    }

					/**
					 * calls start(number) on all the list members
					 */
                    bool start(BaseMatcherHolder::TestNumber number)
                    {
                        bool retval = true;
                        std::vector< boost::shared_ptr<T> >::iterator iter;
                        for (iter = m_list.begin(); iter != m_list.end(); ++iter)
                        {
                            if (0 != iter->get())
                            {
                                retval = retval && (*iter)->start(number);
                            }
                        }
                        return retval;
                    }

				    /**
					 * calls select() on all the list members
					 */
                    bool select()
                    {
                        bool retval = true;
                        std::vector< boost::shared_ptr<T> >::iterator iter;
                        for (iter = m_list.begin(); iter != m_list.end(); ++iter)
                        {
                            if (0 != iter->get())
                            {
                                retval = retval && (*iter)->select();
                            }
                        }
                        return true;
                    }
                };

            }
        }
    }
}
#endif 

// ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~  ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~
