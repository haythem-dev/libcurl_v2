/** $Id$
* @file table.cpp
* @brief ...
* @author Anke Klink
* @date 2007
*/

#include <UnitTest++/UnitTest++.h>
#include "libbasardbsql.h"

#include "buttable.h"


namespace basar
{
    namespace test
    {
        namespace unit
        {
            namespace dbsql
            {
                const basar::UInt32 BUTTable::csm_collengthColch1 = 1;
                const basar::UInt32 BUTTable::csm_collengthColch2 = 10;
                const basar::UInt32 BUTTable::csm_collengthColvch1 = 1;
                const basar::UInt32 BUTTable::csm_collengthColvch2 = 10;
                const basar::UInt32 BUTTable::csm_collengthColvch3 = 254;
                const basar::UInt32 BUTTable::csm_collengthColvch4 = 255;

                // -------------------------------------------------------------------------
                // BUTTable::Entry
                // -------------------------------------------------------------------------
                BUTTable::Entry::Entry():
                m_colser  (0  ),
                    m_colint  (0  ),
                    m_colsmint(0),
                    m_colfl   (0   ),
                    m_colsmfl (0 ),
                    m_coldec  (0.0  ),
                    m_colmon  (0.0  ),
                    m_colch1  (""  ),
                    m_colch2  (""  ),
                    m_colvch1 ("" ),
                    m_colvch2 ("" ),
                    m_colvch3 ("" ),
                    m_colvch4 ("" ),
                    m_collvch ("" )
                {
                }
                // -------------------------------------------------------------------------

                BUTTable::Entry::Entry(
                    const basar::Int32       colser,
                    const basar::Int32       colint,
                    const basar::Int16       colsmint,
                    const basar::Float64     colfl,
                    const basar::Float32     colsmfl,
                    const basar::Decimal   & coldec,
                    const basar::Decimal   & colmon,
                    const basar::ConstString colch1,
                    const basar::ConstString colch2,
                    const basar::ConstString  colvch1,
                    const basar::ConstString  colvch2,
                    const basar::ConstString  colvch3,
                    const basar::ConstString  colvch4,
                    const basar::ConstString  collvch,
                    const basar::Date	    & coldate,
                    const basar::DateTime   & coldt1,
                    const basar::DateTime   & coldt2,
                    const basar::DateTime  	& coltime1):
                m_colser  (colser  ),
                    m_colint  (colint  ),
                    m_colsmint(colsmint),
                    m_colfl   (colfl   ),
                    m_colsmfl (colsmfl ),
                    m_coldec  (coldec  ),
                    m_colmon  (colmon  ),
                    m_colch1  (colch1  ),
                    m_colch2  (colch2  ),
                    m_colvch1 (colvch1 ),
                    m_colvch2 (colvch2 ),
                    m_colvch3 (colvch3 ),
                    m_colvch4 (colvch4 ),
                    m_collvch (collvch ),
                    m_coldate (coldate ),
                    m_coldt1  (coldt1  ),
                    m_coldt2  (coldt2  ),
                    m_coltime1(coltime1)
                {
                }
                // -------------------------------------------------------------------------
                // BUTTable
                // -------------------------------------------------------------------------
                BUTTable::BUTTable()
                {
                    init();
                }
                // -------------------------------------------------------------------------
                BUTTable::~BUTTable()
                {
                }
                // -------------------------------------------------------------------------
                void BUTTable::init()
                {
                    initNames();
                }
                // -------------------------------------------------------------------------
                void BUTTable::initNames()
                {
                    m_colNames.resize(colnumber);
                    m_colNames[colnumber_colser  ] ="colser";
                    m_colNames[colnumber_colint  ] ="colint";
                    m_colNames[colnumber_colsmint] ="colsmint";
                    m_colNames[colnumber_colfl   ] ="colfl";
                    m_colNames[colnumber_colsmfl ] ="colsmfl";
                    m_colNames[colnumber_coldec  ] ="coldec";
                    m_colNames[colnumber_colmon  ] ="colmon";
                    m_colNames[colnumber_colch1  ] ="colch1";
                    m_colNames[colnumber_colch2  ] ="colch2";
                    m_colNames[colnumber_colvch1 ] ="colvch1";
                    m_colNames[colnumber_colvch2 ] ="colvch2";
                    m_colNames[colnumber_colvch3 ] ="colvch3";
                    m_colNames[colnumber_colvch4 ] ="colvch4";
                    m_colNames[colnumber_collvch ] ="collvch";
                    m_colNames[colnumber_coldate ] ="coldate";
                    m_colNames[colnumber_coldt1  ] ="coldt1";
                    m_colNames[colnumber_coldt2  ] ="coldt2";
                    m_colNames[colnumber_coltime1] ="coltime1";
                }
                // -------------------------------------------------------------------------
                std::string BUTTable::getName(ColIndex index) const
                {
                    return m_colNames[index];
                }
                // -------------------------------------------------------------------------

                const std::vector< std::string > BUTTable::getColNames() const
                {
                    return m_colNames;
                }
                // -------------------------------------------------------------------------


                basar::Int32 BUTTable::getInt32(basar::UInt32  row, ColIndex index) 
                {
                    basar::Int32 retval = 0;
                    if (m_entries[row].get())
                    {
                        if (index == colnumber_colint )
                        {
                            retval = m_entries[row]->m_colint;
                        }
                        else if ( index == colnumber_colser)
                        {
                            retval = m_entries[row]->m_colser;
                        }
                    }
                    return retval;
                }
                // -------------------------------------------------------------------------
                basar::Int16 BUTTable::getInt16(basar::UInt32  row,ColIndex index) 
                {
                    basar::Int16 retval = 0;
                    if (m_entries[row].get())
                    {
                        if (index == colnumber_colsmint )
                        {
                            retval = m_entries[row]->m_colsmint;
                        }
                    }
                    return retval;
                }
                // -------------------------------------------------------------------------
                basar::Float64 BUTTable::getFloat64(basar::UInt32  row,ColIndex index) 
                {
                    basar::Float64 retval = 0;
                    if (m_entries[row].get())
                    {
                        if (index == colnumber_colfl )
                        {
                            retval = m_entries[row]->m_colfl;
                        }
                    }
                    return retval;
                }
                // -------------------------------------------------------------------------
                basar::Float32 BUTTable::getFloat32(basar::UInt32  row,ColIndex index) 
                {
                    basar::Float32 retval = 0;
                    if (m_entries[row].get())
                    {
                        if (index == colnumber_colsmfl )
                        {
                            retval = m_entries[row]->m_colsmfl;
                        }
                    }
                    return retval;
                }
                // -------------------------------------------------------------------------
                basar::Decimal BUTTable::getDecimal(basar::UInt32  row,ColIndex index) 
                {
                    basar::Decimal retval;
                    if (m_entries[row].get())
                    {
                        if (index == colnumber_coldec   ) 
                        {
                            retval = m_entries[row]->m_coldec   ; 
                        }
                        else if (index == colnumber_colmon   ) 
                        {
                            retval = m_entries[row]->m_colmon   ; 
                        }

                    }
                    return retval;
                }
                // -------------------------------------------------------------------------
                basar::ConstBuffer BUTTable::getString(basar::UInt32  row,ColIndex index) 
                {
                    basar::ConstString dummi = "";
                    basar::ConstString * retval = &dummi;
                    if (m_entries[row].get())
                    {
                        if (index == colnumber_colch1   ) 
                        {
                            retval = &(m_entries[row]->m_colch1 )  ; 
                        }
                        else if (index == colnumber_colch2   ) 
                        {
                            retval = &(m_entries[row]->m_colch2 )  ; 
                        }
                        else  if (index == colnumber_colvch1  ) 
                        {
                            retval = &(m_entries[row]->m_colvch1)  ; 
                        }
                        else  if (index == colnumber_colvch2  ) 
                        {
                            retval = &(m_entries[row]->m_colvch2 ) ; 
                        }
                        else  if (index == colnumber_colvch3  ) 
                        {
                            retval = &(m_entries[row]->m_colvch3 ) ; 
                        }
                        else  if (index == colnumber_colvch4  ) 
                        {
                            retval = &(m_entries[row]->m_colvch4)  ; 
                        }
                        else  if (index == colnumber_collvch  ) 
                        {
                            retval = &(m_entries[row]->m_collvch)  ; 
                        }
                    }
                    return *retval;
                }
                // ------------------------------------------------------------------------
                basar::DateTime BUTTable::getDateTime(basar::UInt32  row,ColIndex index) 
                {
                    basar::DateTime retval;
                    if (m_entries[row].get())
                    {
                        if (index == colnumber_coldt1   ) 
                        {
                            retval = m_entries[row]->m_coldt1   ; 
                        }
                        else  if (index == colnumber_coldt2   ) 
                        {
                            retval = m_entries[row]->m_coldt2   ; 
                        }
                        else  if (index == colnumber_coltime1 ) 
                        {
                            retval = m_entries[row]->m_coltime1 ; 
                        }
                    }
                    return retval;
                }
                // ------------------------------------------------------------------------
                basar::Time BUTTable::getTime(basar::UInt32  row,ColIndex index) 
                {
					return getDateTime(row, index);
				}
                // -------------------------------------------------------------------------
                basar::Date BUTTable::getDate(basar::UInt32  row,ColIndex index) 
                {
                    basar::Date retval;
                    if (m_entries[row].get())
                    {
                        if (index == colnumber_coldate   ) 
                        {
                            retval = m_entries[row]->m_coldate  ; 
                        }

                    }
                    return retval;
                }
                // -------------------------------------------------------------------------

                basar::Int32 BUTTable::getMinInt32(ColIndex index)
                {
                    basar::Int32 retval = 0;
                    if (m_rows.size() > 0 && 
                        (index == colnumber_colint ||  index == colnumber_colser))
                    {
                        std::set<basar::UInt32>::const_iterator iter = m_rows.begin();
                        retval = getInt32(*(iter), index);
                        for (++iter; iter != m_rows.end(); ++iter)
                        {
                            Int32 newvalue = getInt32(*iter, index);
                            if ( newvalue < retval)
                            {
                                retval = newvalue;
                            }
                        }
                    }
                    return retval;
                }
                // -------------------------------------------------------------------------
                basar::Int32 BUTTable::getSumInt32(ColIndex index)
                {
                    basar::Int32 retval = 0;

                    if (m_rows.size() > 0 && 
                        (index == colnumber_colint ||  index == colnumber_colser))
                    {

                        std::set<basar::UInt32>::const_iterator iter = m_rows.begin();
                        for ( ;iter != m_rows.end(); ++iter)
                        {
                            retval += getInt32(*iter, index);
                        }
                    }
                    return retval;
                }


                // -------------------------------------------------------------------------
                void BUTTable::addRow(
                    const basar::UInt32      number,
                    const basar::Int32       colser,
                    const basar::Int32       colint,
                    const basar::Int16       colsmint,
                    const basar::Float64	 colfl,
                    const basar::Float32     colsmfl,
                    const basar::Decimal   & coldec,
                    const basar::Decimal   & colmon,
                    const basar::ConstString colch1,
                    const basar::ConstString colch2,
                    const basar::ConstString colvch1,
                    const basar::ConstString colvch2,
                    const basar::ConstString colvch3,
                    const basar::ConstString colvch4,
                    const basar::ConstString collvch,
                    const basar::Date	   & coldate,
                    const basar::DateTime  & coldt1,
                    const basar::DateTime  & coldt2,
                    const basar::DateTime  & coltime1)
                {
                    boost::shared_ptr<Entry> entry(new Entry(colser ,colint ,colsmint ,colfl ,colsmfl ,coldec ,colmon ,colch1 ,colch2 ,colvch1 ,colvch2 ,colvch3 ,colvch4 ,collvch ,coldate ,coldt1 ,coldt2 ,coltime1));
                    m_entries[number] = entry;
                    m_rows.insert(number);
                }
                // -------------------------------------------------------------------------
                void BUTTable::addRowsFromResultSets(basar::db::sql::ResultsetRef resultset)
                {
                    while (resultset.next())
                    {
                        addRow(resultset.getFetchedRows(),
                            resultset.getInt32(colnumber_colser  ),
                            resultset.getInt32(colnumber_colint  ),
                            resultset.getInt16(colnumber_colsmint),
                            resultset.getFloat64(colnumber_colfl   ),
                            resultset.getFloat32(colnumber_colsmfl ),
                            resultset.getDecimal(colnumber_coldec  ),
                            resultset.getDecimal(colnumber_colmon  ),
                            resultset.getString(colnumber_colch1  ),
                            resultset.getString(colnumber_colch2  ),
                            resultset.getString(colnumber_colvch1 ),
                            resultset.getString(colnumber_colvch2 ),
                            resultset.getString(colnumber_colvch3 ),
                            resultset.getString(colnumber_colvch4 ),
                            resultset.getString(colnumber_collvch ),
                            resultset.getDate(colnumber_coldate ),
                            resultset.getDateTime(colnumber_coldt1  ),
                            resultset.getDateTime(colnumber_coldt2  ),
                            resultset.getDateTime(colnumber_coltime1));
                    }

                }
                // -------------------------------------------------------------------------
            }
        }
    }
}

// ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~  ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~
