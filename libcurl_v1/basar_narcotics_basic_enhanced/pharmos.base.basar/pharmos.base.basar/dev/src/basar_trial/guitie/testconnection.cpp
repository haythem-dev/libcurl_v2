/** $Id$                                          
*                                                  
* @file                                            
* @author Anke Klink                      
* @date 2008                            
*/    

#include "testconnection.h"

namespace basar_trial
{
    namespace guitie
    {
        basar::db::aspect::ConnectionRef TestConnection::getBasarTestConnection()
        {

            basar::db::sql::DatabaseInfo  dbinfo;
            dbinfo.dbServer     = "zdev21_tcp";
            dbinfo.eConnectType = basar::INFORMIX;
            dbinfo.database     = "ode21";		     //infx
            dbinfo.user         = "";
            dbinfo.passwd       = "";
            basar::db::aspect::ConnectionRef   connection = 
                basar::db::aspect::Manager::getInstance().createConnect(dbinfo);                 
            return connection;
        }

    }
}
// ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~  ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~
