/*! $Id$                                          
*                                                  
* @file                                            
* @author Anke Klink                      
* @date 2008                            
*/    

#ifndef  __BASAR_TEST_UNIT_GUITIE_INDEXCOUNTHOLDER_H__ 
#define  __BASAR_TEST_UNIT_GUITIE_INDEXCOUNTHOLDER_H__ 

namespace basar
{
	namespace test
	{
		namespace unit
		{
			namespace guitie
			{

				/*!
				* Helperclass dor Matcher functions:
				* RightToLeft(int index, int count)
				* LeftToRight(int index, int count)
				* m_index - Index parameter 
				* m_count - Count parameter
				* m_number - Number of lines matched 
				* m_start - first index that was matched 
				*/
				struct IndexCountHolder
				{
					int m_index;   //!< Index parameter 
					int m_count;   //!< Count parameter 
					int m_number;  //!< Number of lines matched 
					int m_start;   //!< first index that was matched 
					int m_max;     //!< maximal rownumber

					/*!
					* constructor, that calculates the values
					*/
					IndexCountHolder(
						int index,  //!< Index parameter 
						int count,  //!< Count parameter 
						int max     //!< Maximal Number of lines
						);

					/**
					 * returns a vector of indices that are changed
					 * in the sequence that they are changed
					 */
					std::vector<int> getInnerIndices() const;

					/** 
					* used for test log entries
					*/
					basar::VarString toText() const;
				};

			}
		}
	}
}
#endif 

// ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~  ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~
