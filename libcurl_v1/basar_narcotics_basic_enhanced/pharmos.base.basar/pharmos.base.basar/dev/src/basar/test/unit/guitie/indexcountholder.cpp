/** $Id$                                          
*                                                  
* @file                                            
* @author Anke Klink                      
* @date 2008                            
*/    

#include <UnitTest++/UnitTest++.h>
#include "libbasarguitie.h"

#include "indexcountholder.h"

namespace basar
{
	namespace test
	{
		namespace unit
		{
			namespace guitie
			{
				// ---------------------------------------------------------------------------

				IndexCountHolder::IndexCountHolder(
					int index, 
					int count, 
					int max)  :
				m_index(index),
					m_count(count),
					m_number(count),
					m_start(index),
					m_max(max)
				{
					if (count == 0 && index == 0)
					{
						m_number = max;
					}
					else if (count < 0)
					{
						m_number = -count;
						m_start = m_index + count + 1;
					}
					else if (count > 0 )
					{
						int help = max - index;
						if (help < count)
						{
							m_number = help;
						}
					}
				}
				// ---------------------------------------------------------------------------
				std::vector<int> IndexCountHolder::getInnerIndices()const
				{
					std::vector<int> retval;
					if (m_count < 0)
					{
							for (int i=m_index; i > m_start-m_number && i > -1; i--)
							{
								retval.push_back(i);
							}
					}
					else
					{
						for (int i=m_start; i< m_start+m_number; i++)
						{
							retval.push_back(i);
						}
					}
					return retval;
				}
				// ---------------------------------------------------------------------------
					basar::VarString  IndexCountHolder::toText() const
					{
						basar::VarString retval;
						retval.format("(max, index, count, start, number)=(%d,%d,%d,%d,%d)",
                                        m_max, m_index, m_count, m_start, m_number);
						return retval;
					}
				// ---------------------------------------------------------------------------

			}
		}
	}
}
// ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~  ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~
