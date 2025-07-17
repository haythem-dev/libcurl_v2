
//------------------------------------------------------------------------------
#include <assert.h>
#include <sys/timeb.h>

#include <libbasardbsql.h>
#include <libbasardbaspect.h>
#include "cicsoldvoucheracc.h"

//------------------------------------------------------------------------------
using basar::db::aspect::ConnectionRef;
using basar::db::aspect::AccessorInstanceRef;
using basar::db::aspect::Manager;
using basar::db::aspect::ON_DEMAND_CACHING;
using basar::db::aspect::NO_CACHING;
using basar::db::aspect::SINGLE_ROW_CACHING;
using basar::db::aspect::ExecuteResultInfo;
using basar::db::aspect::AccessorPropertyTableRef;
using basar::db::aspect::AccessorPropertyTable_YIterator;

//------------------------------------------------------------------------------
ConnectionRef getConn()
{
	basar::db::aspect::ConnectionRef::DatabaseInfo  dbInfo;

	dbInfo.eConnectType = basar::CICS;
	dbInfo.dbServer     = "VSE94_82";
	dbInfo.database     = "";		
	dbInfo.user         = "";
	dbInfo.passwd       = "";

	return  Manager::getInstance().createConnect( dbInfo );
}

//------------------------------------------------------------------------------
void measure_cics()
{
	const basar::Int32 LIM = 1000;
	basar::Int32       cnt = 0;
	timeb              start, 
		               stop;
	try
	{
		ConnectionRef connect;
		{
			connect = getConn();
		}

		AccessorInstanceRef accInstSel = Manager::getInstance().createAccessorInstance
											("Sel_CICS_VSAM_DKRPR",
											 "AccCicsOldVoucher",
											 connect,
											 //ON_DEMAND_CACHING
											 //SINGLE_ROW_CACHING
											 NO_CACHING
											 );


		{
			ftime(&start);

			ExecuteResultInfo ret = accInstSel.execute("selectAll");
			assert( ! ret.hasError() );

			AccessorPropertyTableRef propTbl = accInstSel.getPropertyTable();
			assert( !propTbl.isNull() );

			AccessorPropertyTable_YIterator yit    = propTbl.begin();
			assert( ! yit.isNull() );

			while (!yit.isEnd() && (cnt < LIM))
			{
				//printf("SUPPLIERNO %d\n", yit.getInt32("supplierno"));
				++yit;
				++cnt;
			}

			assert(yit.isEnd() || (cnt == LIM));

			ftime(&stop);
		}
	}
	catch (basar::Exception & ex)
    {
		std::cout << "basar exception: " << ex.what().c_str() << std::endl;
    }
    catch (...)
    {
        std::cout << "unknown exception" << std::endl;
    }


	printf("measure_cics: rows %d\n", cnt);
	printf("----------------------------------\n");
	printf("          %10d.%03d sec\n", 
		   (start.millitm > stop.millitm) ? (stop.time - start.time - 1): (stop.time - start.time),
		   (start.millitm > stop.millitm) ? (stop.millitm + 1000 - start.millitm): (stop.millitm - start.millitm));

}