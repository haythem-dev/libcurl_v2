#pragma once

#include <libbasardbsql.h>

#define VC_EXTRALEAN            //!< less Visual C headers
#define WIN32_LEAN_AND_MEAN     //!< less Windows headers
#include <windows.h>

namespace helpers
{
    static const basar::db::sql::ConnectionRef getDBConnection()
    {
        basar::db::sql::DatabaseInfo dbInfo;
        dbInfo.setInfxConcurrent("zdev21_tcp", "zpcua1");

        return basar::db::sql::Manager::getInstance().createConnect(dbInfo);
    }

    static const basar::VarString getLoggedOnUserName()
    {
        char globalUser[256] = {0};
        DWORD dwSize = sizeof( globalUser );

        ::GetUserName( globalUser, &dwSize );

        return globalUser;
    }
}