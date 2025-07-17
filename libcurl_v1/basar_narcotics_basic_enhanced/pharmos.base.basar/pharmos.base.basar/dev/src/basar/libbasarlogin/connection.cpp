//----------------------------------------------------------------------------
/*! \file
 *  \brief  connection class implemtation
 *  \author	Thomas Hörath
 *  \date   12.12.2005
 */
//----------------------------------------------------------------------------

#include "connection.h"

#include "libbasarcmnutil_functions.h"
#include "libbasarcmnutil_config.h"
#include "libbasardbaspect_manager.h"
#include "libbasarlogin_exceptions.h"
#include "loggerpool.h"

#include <boost/filesystem.hpp>

using boost::filesystem::path;
using boost::filesystem::current_path;

//----------------------------------------------------------------------------
namespace basar
{
    namespace login
    {

//----------------------------------------------------------------------------
        static basar::ConstString LOGIN4TEST_FILE               = "login4test.cfg"; //!< file to search for in exe's directory; if exists (no content required), login is connected to test/dev

        static basar::ConstString MAIN_HOST                     = "ode21_tcp";      //!< default database server for main connection
        static basar::ConstString BACKUP_HOST                   = "ide21_tcp";      //!< default database server for backup connection
        static basar::ConstString DB_MAIN                       = "zpcua1";         //!< default main database
        static basar::ConstString DB_BACKUP                     = "zpcua2";         //!< default backup database

        static basar::ConstString DB_MAIN_AD                    = "uadm1";         //!< default ad main database
        static basar::ConstString DB_BACKUP_AD                  = "uadm2";         //!< default ad backup database


        static basar::ConstString TEST_HOST                     = "zdev21_tcp";     //!< database server for test/dev connection

        static basar::ConstString  CONN_TIMEOUT                 = "12";             //!< DB server connection timeout in seconds
        static basar::ConstString  CONN_RETRY                   = "3";              //!< additional DB server connection retries

        static basar::ConstString  CONFIGKEY_DBSERVER_TIMEOUT   = "basar.login.dbserver.timeout";         //!< configuration key connection timeout
        static basar::ConstString  CONFIGKEY_DBSERVER_RETRY     = "basar.login.dbserver.retry";          //!< configuration key connection retry

        static basar::ConstString  CONFIGKEY_DBSERVER_MAIN      = "basar.login.dbserver.main";   //!< configuration key primary database server
        static basar::ConstString  CONFIGKEY_DBSERVER_BACKUP    = "basar.login.dbserver.backup";    //!< configuration key backup database server

        static basar::ConstString  CONFIGKEY_DATABASE_MAIN      = "basar.login.database.main";  //!< configuration key primary database
        static basar::ConstString  CONFIGKEY_DATABASE_BACKUP    = "basar.login.database.backup"; //!< configuration key backup database


        static basar::ConstString  CONFIGKEY_DBSERVER_MAIN_AD   = "basar.login.adlogin.dbserver.main";      //!< configuration key primary database server
        static basar::ConstString  CONFIGKEY_DBSERVER_BACKUP_AD = "basar.login.adlogin.dbserver.backup";    //!< configuration key backup database server

        static basar::ConstString  CONFIGKEY_DATABASE_MAIN_AD   = "basar.login.adlogin.database.main";      //!< configuration key primary database
        static basar::ConstString  CONFIGKEY_DATABASE_BACKUP_AD = "basar.login.adlogin.database.backup";    //!< configuration key backup database

        using basar::cmnutil::Config;

//----------------------------------------------------------------------------
        //! \throw no-throw
        //! \param config - shared pointer to configuration data
        Connection::Connection(  Configurationptr config ) : m_Backup ( false ), m_Configuration( config )
        {
            evalConfig();
        }

//----------------------------------------------------------------------------
        /*! \throw no-throw */
        Connection::~Connection()
        {
        }

//----------------------------------------------------------------------------
        /*! 1) sets default parameters (prod):
                - database server connection timeout
                - primary database server
                - backup database server
                - primary database
                - backup database

            2) tests file "login4test.cfg" and sets parameters (test/dev):
                - primary database server
                - backup database server

            3) evaluates parameters from config file "basar.config.file":
                - basar.login.dbserver.timeout
                - basar.login.dbserver.retry
                - basar.login.dbserver.main
                - basar.login.dbserver.backup
                - basar.login.database.main
                - basar.login.database.backup

            => precedence:
                - config file
                - test file
                - default values

            \throw no-throw */
        void Connection::evalConfig()
        {
            evalConfigTimeout();

            // --- default:
            m_HostMain   = MAIN_HOST;
            m_HostBackup = BACKUP_HOST;
            m_DbMain     = DB_MAIN;
            m_DbBackup   = DB_BACKUP;

            VarString val;

            bool      cfgFound = false;

            if ( m_Configuration->isADLogin() )
            {
                m_DbMain     = DB_MAIN_AD;
                m_DbBackup   = DB_BACKUP_AD;

                if ( Config::getInstance().getValue( CONFIGKEY_DATABASE_MAIN_AD, val ) )
                {
                    m_DbMain = val;
                    cfgFound = true;
                }

                if ( Config::getInstance().getValue( CONFIGKEY_DATABASE_BACKUP_AD, val ) )
                {
                    m_DbBackup = val;
                    cfgFound   = true;
                }

                if ( Config::getInstance().getValue( CONFIGKEY_DBSERVER_MAIN_AD, val ) )
                {
                    m_HostMain = val;
                    cfgFound   = true;
                }

                if ( Config::getInstance().getValue( CONFIGKEY_DBSERVER_BACKUP_AD, val ) )
                {
                    m_HostBackup = val;
                    cfgFound     = true;
                }
            }
            else
            {
                if ( Config::getInstance().getValue( CONFIGKEY_DATABASE_MAIN, val ) )
                {
                    m_DbMain = val;
                    cfgFound = true;
                }

                if ( Config::getInstance().getValue( CONFIGKEY_DATABASE_BACKUP, val ) )
                {
                    m_DbBackup = val;
                    cfgFound   = true;
                }

                if ( Config::getInstance().getValue( CONFIGKEY_DBSERVER_MAIN, val ) )
                {
                    m_HostMain = val;
                    cfgFound   = true;
                }

                if ( Config::getInstance().getValue( CONFIGKEY_DBSERVER_BACKUP, val ) )
                {
                    m_HostBackup = val;
                    cfgFound     = true;
                }
            }

            // --- test file if no config item is found
            if ( !cfgFound && isLogin4Test() )
            {
                m_HostMain   = TEST_HOST;
                m_HostBackup = TEST_HOST;
            }
        }

//----------------------------------------------------------------------------
        /*  \throw no-throw */
        void Connection::evalConfigTimeout()
        {
            VarString timeout = CONN_TIMEOUT;
            VarString retry   = CONN_RETRY;

            VarString val;

            if ( Config::getInstance().getValue( CONFIGKEY_DBSERVER_TIMEOUT, val ) )
            {
                timeout = val;
            }

            if ( Config::getInstance().getValue( CONFIGKEY_DBSERVER_RETRY, val ) )
            {
                retry = val;
            }

            m_DbInfo.setInfxTimeout( timeout, retry );
        }

//----------------------------------------------------------------------------
//! \throw no-throw
//! \retval true  file exists
//! \retval false file doesn't exist
        bool Connection::isLogin4Test()
        {
            VarString file;

            // --- search in executable's directory
            VarString execdir = cmnutil::getExecDir();

            file  = execdir;
            file += "/";
            file += LOGIN4TEST_FILE;

            if ( boost::filesystem::exists( path( file.c_str() ) ) )
            {
                BLOG_DEBUG_STREAM( LoggerPool::loginConn(), "file <" << file << "> found in executable's directory" );

                return true;
            }

            // --- search in working directory
            VarString workdir = current_path().string();

            file  = workdir;
            file += "/";
            file += LOGIN4TEST_FILE;

            if ( boost::filesystem::exists( path( file.c_str() ) ) )
            {
                BLOG_DEBUG_STREAM( LoggerPool::loginConn(), "file <" << file << "> found in current working directory" );

                return true;
            }

            if ( LoggerPool::loginConn().isEnabledFor( log4cplus::DEBUG_LOG_LEVEL ) )
            {
                BLOG_DEBUG_STREAM( LoggerPool::loginConn(), "file <" << file << "> found neither in executable's (" << execdir <<") nor in current working (" << workdir << ") directory" );
            }

            return false;
        }

//----------------------------------------------------------------------------
        /*! \throw no-throw */
        void Connection::connect()
        {
            setMainInfo();

            bool fConnected = false;

            try
            {
                m_ActivConn = db::aspect::Manager::getInstance().createConnect( m_DbInfo );

                fConnected  = true;

                BLOG_DEBUG_STREAM( LoggerPool::loginConn(), "main connect succeeded < " << m_HostMain << ", " << m_DbMain << ">" );
            }
            catch ( const basar::Exception& ex )
            {
                BLOG_ERROR_STREAM( LoggerPool::loginConn(), "main connect failed < " << m_HostMain << ", " << m_DbMain << " > - basar exception caught: " << ex.what() );
            }
            catch ( ... )
            {
                BLOG_ERROR_STREAM( LoggerPool::loginConn(), "main connect failed < " << m_HostMain << ", " << m_DbMain << " > - exception caught" );
            }

            // because of timeout or caught exception
            if ( ! fConnected )
            {
                BLOG_WARN( LoggerPool::loginConn(), "trying to connect to backup DB" );
                connectBackup();
            }
        }

//----------------------------------------------------------------------------
        /*! \throw BasarException */
        void Connection::connectBackup()
        {
            setBackupInfo();

            m_ConnToRelease = m_ActivConn;

            bool fConnected = false;

            try
            {
                m_ActivConn = db::aspect::Manager::getInstance().createConnect( m_DbInfo );

                fConnected  = true;
                m_Backup    = true;

                BLOG_DEBUG_STREAM( LoggerPool::loginConn(), "backup connect succeeded < " << m_HostBackup << ", " << m_DbBackup << ">" );
            }
            catch ( const basar::Exception& ex )
            {
                BLOG_ERROR_STREAM( LoggerPool::loginConn(), "backup connect failed < " << m_HostBackup << ", " << m_DbBackup << " > - basar exception caught: " << ex.what() );
            }
            catch ( ... )
            {
                BLOG_ERROR_STREAM( LoggerPool::loginConn(), "backup connect failed < " << m_HostBackup << ", " << m_DbBackup << " > - exception caught" );
            }

            // because of timeout or caught exception
            if ( !fConnected )
            {
                VarString msg;
                msg.format( "backup connect failed < %s, %s > - no login connection possible",
                            m_HostBackup.c_str(),
                            m_DbBackup  .c_str() );
                throw BackupDBException( ExceptInfo( "basar.login.Connection.connectBackup",
                                                     msg,
                                                     __FILE__, __LINE__ ) );
            }
        }

//----------------------------------------------------------------------------
        /*! \throw no-throw
            \return reference to active connection */
        const basar::db::aspect::ConnectionRef Connection::getActiveConn() const
        {
            return m_ActivConn;
        }

//----------------------------------------------------------------------------
        /*! \throw no-throw */
        void Connection::setMainInfo()
        {
            m_DbInfo.setInfx( m_HostMain, m_DbMain );
        }

//----------------------------------------------------------------------------
        /*! \throw no-throw */
        void Connection::setBackupInfo()
        {
            m_DbInfo.setInfx( m_HostBackup, m_DbBackup );
        }

//----------------------------------------------------------------------------
        /*! \throw BasarException */
        void Connection::disconnect()
        {
            VarString server = "no connected database server";
            VarString db     = "no connected database";

            try
            {
                if ( !m_ActivConn.isNull() )
                {
                    server = m_ActivConn.getDbInfo().dbServer;
                    db     = m_ActivConn.getDbInfo().database;

                    m_ActivConn.close();

                    BLOG_DEBUG_STREAM( LoggerPool::loginConn(), "disconnected successfully < " << server << ", " << db << ">" );
                }

                // close former main connection, has to be open till all domain modules are reset!
                if ( !m_ConnToRelease.isNull() )
                {
                    m_ConnToRelease.close();
                }
            }
            catch ( basar::Exception& e )
            {
                BLOG_ERROR_STREAM( LoggerPool::loginConn(), "disconnecting failed < " << server << ", " << db << ">: got exception: " << e.what() );
                throw;
            }
            catch ( ... )
            {
                BLOG_ERROR_STREAM( LoggerPool::loginConn(), "disconnecting failed < " << server << ", " << db << ">: got unspecified exception" );
                throw;
            }
        }

//----------------------------------------------------------------------------
        /*! \throw no-throw
            \retval true if backup connection is active
            \return otherwise returns false */
        bool Connection::isBackup() const
        {
            return m_Backup;
        }

//----------------------------------------------------------------------------
        /*! \throw no-throw
            \return main host name */
        const basar::VarString  Connection::getHostname         () const
        {
            return m_HostMain;
        }

//----------------------------------------------------------------------------
        /*! \throw no-throw
            \return main database name */
        const basar::VarString  Connection::getDatabase         () const
        {
            return m_DbMain;
        }

//----------------------------------------------------------------------------
        /*! \throw no-throw
            \return main host name */
        const basar::VarString  Connection::getHostnameBackup   () const
        {
            return m_HostBackup;
        }

//----------------------------------------------------------------------------
        /*! \throw no-throw
            \return main database name */
        const basar::VarString  Connection::getDatabaseBackup   () const
        {
            return m_DbBackup;
        }

//----------------------------------------------------------------------------
        /*! \throw no-throw */
        void Connection::setHostname        ( const basar::VarString& rHostMain )
        {
            m_HostMain = rHostMain;
        }

//----------------------------------------------------------------------------
        /*! \throw no-throw */
        void Connection::setHostnameBackup  ( const basar::VarString& rHostBackup )
        {
            m_HostBackup = rHostBackup;
        }

//----------------------------------------------------------------------------
        /*! \throw no-throw */
        void Connection::setDatabase        ( const basar::VarString& rDbMain )
        {
            m_DbMain = rDbMain;
        }

//----------------------------------------------------------------------------
        /*! \throw no-throw */
        void Connection::setDatabaseBackup  ( const basar::VarString& rDbBackup )
        {
            m_DbBackup = rDbBackup;
        }

//----------------------------------------------------------------------------
    }//login
}//basar
