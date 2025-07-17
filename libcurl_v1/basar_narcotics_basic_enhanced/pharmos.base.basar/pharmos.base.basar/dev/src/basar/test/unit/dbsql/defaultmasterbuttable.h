/** $Id$
*
* @file defaultmasterbuttable.h
* @author Anke Klink
* @date 2007
*/


#ifndef  ___MARATHON_LIB_BASAR_TEST_UNIT_DBSQL_DEFAULTMASTERBUTTABLE_H__ 
#define  ___MARATHON_LIB_BASAR_TEST_UNIT_DBSQL_DEFAULTMASTERBUTTABLE_H__ 

#include "buttable.h"


namespace basar
{
    namespace test
    {
        namespace unit
        {
            namespace dbsql
            {
                //! Default values  for the But Master Table
                class DefaultMasterBUTTable : public BUTTable
                {
                    //! number of rows
                    int m_numberOfRows;

                    //! writes the Entries
                    void initEntries();

                public:
                    //! Constructor
                    DefaultMasterBUTTable();

                    //! Destructor
                    virtual ~DefaultMasterBUTTable();

                    /*! reads the values and writes them to stdout
                    * this can be used to visualize the data for
                    * debug purposses when the data changes
                    */
                    void readAndShow();

                    //! returns the number of lines
                    int getNumberOfRows();

                    //! usable as Singleton instance 
                    static DefaultMasterBUTTable & getInstance();
                };
            }
        }
    }
}

#endif 

// ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~  ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~
