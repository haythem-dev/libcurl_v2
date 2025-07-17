#include <libbasardbsql.h>
#include <libbasardbaspect.h>

#include <windows.h>


using basar::db::aspect::ConnectionRef;
using basar::db::aspect::ConnectionRef::DatabaseInfo;
using basar::db::aspect::Manager;

#define INFX_DBSERVER		"zdev21_tcp"
#define INFX_DB_WEST		"ode21"

#define LIM 2000

//------------------------------------------------------------------------------
void connect_mem()
{
	try
	{
		DatabaseInfo  dbInfo;
		dbInfo.setInfx(INFX_DBSERVER, INFX_DB_WEST);

		ConnectionRef conn =  Manager::getInstance().createConnect( dbInfo, false );

		conn.open();

		printf("press key ...\n");
		getchar();

		conn.close();

		DWORD d1 = GetTickCount();

		for (int i = 0; i < LIM; ++i)
		{
			conn.open();

			conn.close();

			//Sleep(1000);
		}

		DWORD d2 = GetTickCount();

		printf("duration: %d\n", (d2 - d1) / 1000);

	}
	catch (basar::Exception & ex)
	{
		std::cout << ex.what() << std::endl;
	}
	catch (...)
	{
		std::cout << "unknown exception" << std::endl;
	}
}

