/** $Id$
*
* @file defaultmasterbuttable.cpp
* @author Anke Klink
* @date 2007
*/

#include <UnitTest++/UnitTest++.h>
#include "libbasardbsql.h"

#include "defaultmasterbuttable.h"
#include "testinfxconnection.h"
#include "libbasarcmnutil_i18nstring.h"

#include <iostream>


namespace basar
{
	namespace test
	{
		namespace unit
		{
			namespace dbsql
			{
				// -------------------------------------------------------------------------

				DefaultMasterBUTTable::DefaultMasterBUTTable()
				{
					initEntries();
				}
				// -------------------------------------------------------------------------
				DefaultMasterBUTTable::~DefaultMasterBUTTable()
				{
				}
				// -------------------------------------------------------------------------
				
				DefaultMasterBUTTable & DefaultMasterBUTTable::getInstance()
				{
				  static DefaultMasterBUTTable sm_instance;
				  return sm_instance;
				}

				// -------------------------------------------------------------------------
				void DefaultMasterBUTTable::initEntries()
				{
					addRow(  1,  1, 100001, 2001, Float64(300001.345678900), Float32(4001.9875), Decimal(500001.61), Decimal(600001.32), "1", "COLCH21" , "" , "COLCH21" , "0COLCH21"               , "0COLCH21   0COLCH21" , "0COLCH21   0COLCH21   0COLCH21   0COLCH21" , Date(99970405), DateTime(87640530, 205956000), DateTime(87640530,  52758990), DateTime(0, 235300000)); 
					addRow(  2,  2, 100002, 2002, Float64(300002.345678900), Float32(4002.9875), Decimal(500002.61), Decimal(600002.32), "2", "COLCH22" , "#", "\\#"     , "\\\\ \\\\ \\\\ \\ #####", "'"                   , "1COLCH22   1COLCH22   1COLCH22   1COLCH22" , Date(99940710), DateTime(87630529, 175953000), DateTime(87630528, 105558980), DateTime(0, 234601000)); 
					addRow(  3,  3, 100003, 2003, Float64(300003.345678900), Float32(4003.9875), Decimal(500003.61), Decimal(600003.32), "3", "COLCH23" , "2", "COLCH23" , "2COLCH23"               , "2COLCH23   2COLCH23" , "2COLCH23   2COLCH23   2COLCH23   2COLCH23" , Date(99911014), DateTime(87620528, 145950000), DateTime(87620526, 162358970), DateTime(0, 233902000)); 
					addRow(  4,  4, 100004, 2004, Float64(300004.345678900), Float32(4004.9875), Decimal(500004.61), Decimal(600004.32), "4", "COLCH24" , "3", "' ' '"   , "3COLCH24"               , "3COLCH24   3COLCH24" , "3COLCH24   3COLCH24   3COLCH24   3COLCH24" , Date(99890117), DateTime(87610527, 115947000), DateTime(87610524, 215158960), DateTime(0, 233203000)); 
					addRow(  5,  5, 100005, 2005, Float64(300005.345678900), Float32(4005.9875), Decimal(500005.61), Decimal(600005.32), "5", "COLCH25" , "4", "COLCH25" , "4COLCH25"               , "4COLCH25   4COLCH25" , "4COLCH25   4COLCH25   4COLCH25   4COLCH25" , Date(99860423), DateTime(87600526,  85944000), DateTime(87600523,  31958950), DateTime(0, 232504000)); 
					addRow(  6,  6, 100006, 2006, Float64(300006.345678900), Float32(4006.9875), Decimal(500006.61), Decimal(600006.32), "6", "COLCH26" , "5", "COLCH26" , "5COLCH26"               , "5COLCH26   5COLCH26" , "5COLCH26   5COLCH26   5COLCH26   5COLCH26" , Date(99830728), DateTime(87590525,  55941000), DateTime(87590521,  84758940), DateTime(0, 231805000)); 
					addRow(  7,  7, 100007, 2007, Float64(300007.345678900), Float32(4007.9875), Decimal(500007.61), Decimal(600007.32), "7", "COLCH27" , "6", "COLCH27" , "6COLCH27"               , "6COLCH27   6COLCH27" , "6COLCH27   6COLCH27   6COLCH27   6COLCH27" , Date(99801031), DateTime(87580524,  25938000), DateTime(87580519, 141558930), DateTime(0, 231106000)); 
					addRow(  8,  8, 100008, 2008, Float64(300008.345678900), Float32(4008.9875), Decimal(500008.61), Decimal(600008.32), "8", "COLCH28" , "7", "COLCH28" , "7COLCH28"               , "7COLCH28   7COLCH28" , "7COLCH28   7COLCH28   7COLCH28   7COLCH28" , Date(99780204), DateTime(87570522, 235935000), DateTime(87570517, 194358920), DateTime(0, 230407000)); 
					addRow(  9,  9, 100009, 2009, Float64(300009.345678900), Float32(4009.9875), Decimal(500009.61), Decimal(600009.32), "9", "COLCH29" , "8", "COLCH29" , "8COLCH29"               , "8COLCH29   8COLCH29" , "8COLCH29   8COLCH29   8COLCH29   8COLCH29" , Date(99750511), DateTime(87560521, 205932000), DateTime(87560516,  11158910), DateTime(0, 225708000)); 
					addRow( 10, 10, 100010, 2010, Float64(300010.345678900), Float32(4010.9875), Decimal(500010.61), Decimal(600010.32), "0", "COLCH210", "9", "COLCH210", "9COLCH210"              , "9COLCH210  9COLCH210", "9COLCH210  9COLCH210  9COLCH210  9COLCH210", Date(99720814), DateTime(87550520, 175929000), DateTime(87550514,  63958900), DateTime(0, 225009000)); 
			        m_numberOfRows = 10;
				}	
				// -------------------------------------------------------------------------
				int DefaultMasterBUTTable::getNumberOfRows()
				{
					return m_numberOfRows;
				}
				// -------------------------------------------------------------------------
				void DefaultMasterBUTTable::readAndShow()
				{
					basar::db::sql::ConnectionRef connRef =TestInfxConnection::createDefaultConnectionRef();
					{
						try
						{	
							basar::db::sql::StatementRef statement = connRef.createStatement();
							connRef.begin();
							// only 2 at present:
							basar::ConstString statementstring = "select first 10 colser ,colint ,colsmint ,colfl ,colsmfl ,coldec ,colmon ,colch1 ,colch2 ,colvch1 ,colvch2 ,colvch3 ,colvch4 ,collvch ,coldate ,coldt1 ,coldt2 ,coltime1 from but_master";
							basar::db::sql::ResultsetRef resultset  = statement.executeQuery(statementstring);

							while(resultset.next())
							{
								std::cout << " addRow( " <<  resultset.getFetchedRows() << ", " ;
								std::cout << resultset.getInt32(colnumber_colser  ) << ", " ;
								std::cout << resultset.getInt32(colnumber_colint  ) << ", " ;
								std::cout << resultset.getInt16(colnumber_colsmint) << ", " ;
								std::cout << "Float64(" <<resultset.getFloat64(colnumber_colfl   ) << "), " ;
								std::cout << "Float32(" << resultset.getFloat32(colnumber_colsmfl ) << "), " ;
								std::cout << "Decimal(" << resultset.getDecimal(colnumber_coldec  ).toString() << "), " ;
								std::cout << "Decimal(" << resultset.getDecimal(colnumber_colmon  ).toFloat64() << "), " ;

								std::cout << "\"" << resultset.getString(colnumber_colch1  ) << "\"" << ", " ;
								std::cout << "\"" << resultset.getString(colnumber_colch2  ) << "\"" << ", " ;

								std::cout << "\"" << resultset.getString(colnumber_colvch1 ) << "\"" << ", " ;
								std::cout << "\"" << resultset.getString(colnumber_colvch2 ) << "\"" << ", " ;
								std::cout << "\"" << resultset.getString(colnumber_colvch3 ) << "\"" << ", " ;
								std::cout << "\"" << resultset.getString(colnumber_colvch4 ) << "\"" << ", " ;
								std::cout << "\"" << resultset.getString(colnumber_collvch ) << "\"" << ", " ;
								std::cout << "Date(" << resultset.getDate(colnumber_coldate ).getDate() << "), " ;
								basar::DateTime helper = resultset.getDateTime(colnumber_coldt1  );
								std::cout  << "DateTime("  <<helper.getDate() << ", " << helper.getTime() << "), " ;
								helper = resultset.getDateTime(colnumber_coldt2  );
								std::cout  << "DateTime("  <<helper.getDate() << ", " << helper.getTime() << "), " ;
								helper = resultset.getDateTime(colnumber_coltime1  );
								std::cout  << "DateTime("  <<helper.getDate() << ", " << helper.getTime() << ")";
								std::cout << "); " << std::endl;
							}

						}
						catch (...)
						{
							// do nothing
						}
					}
					connRef.close();
				}
				// -------------------------------------------------------------------------
			}
		}
	}
}

// ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~  ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~
