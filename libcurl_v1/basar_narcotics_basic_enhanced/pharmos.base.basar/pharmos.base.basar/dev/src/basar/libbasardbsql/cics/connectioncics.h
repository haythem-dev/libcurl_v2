/** $Id$
*
* @file ConnectionCics.h
* @brief ...
* @author Anke Klink
* @date 2007
*/

#ifndef __GUARD_BASAR_LIBBASARDBSQL_CICS_ConnectionCics_H__ 
#define __GUARD_BASAR_LIBBASARDBSQL_CICS_ConnectionCics_H__ 

//----------------------------------------------------------------------------
#include "cicsdefinitions.h"
#include "connection.h"

//----------------------------------------------------------------------------
namespace basar
{
namespace db
{
namespace sql
{

//! CICS Connction  
class ConnectionCics : public Connection
{
public:
    // --- method section ----------------------------------------------------

    //! create Connection implementation object by database specific connection data \n no-throw
    static ConnectionSharedPtr		create		(const DatabaseInfo & rDbInfo ,		//!< database specific connection data
												 bool                 openConn		//!< open connection to database
												);

    //! constructor
    ConnectionCics								(const DatabaseInfo & rDbInfo ,		//!< database specific connection data NULL
												 bool                 openConn		//!< open connection to database
												);
	//! close connection \n no-throw
    virtual ~ConnectionCics						(); 

    //! \brief open connection to database \n throw InvalidStateException
    virtual void					open					();
    //! \brief close connection to database \n no-throw
    virtual void					close					();
    //! \brief begin transaction \n throw InvalidStateException
    virtual void					begin					();
    //! \brief commit transaction \n throw InvalidStateException
    virtual void					commit					();
    //! \brief rollback transaction \n throw InvalidStateException
    virtual void					rollback				();

	//! \brief check if connection is technically open	\n no-throw
	virtual bool					isValid					();	


    //! \brief create statement reference object \n no-throw
    virtual StatementRef			createStatement();

    //! \brief create prepared statement reference object \n throw InvalidStateException
    virtual PreparedStatementRef	createPreparedStatement	(const VarString & rSql,			//!< Sql-String for preparing statement
												  			 CursorTypeEnum    eCursorType	//!< cursor type
															);
private:

    //! find suitable slot	\n no-throw
    void				initConnInfo			();

						ConnectionCics			();									//!< forbidden
						ConnectionCics			(const ConnectionCics & rCopy);		//!< forbidden
    ConnectionCics &    operator =				(const ConnectionCics & rCopy);     //!< forbidden

    CicsConnectionInfo		m_connInfo;				//!< Connection Info CICS Values
    const bool				m_hasTransactions;		//!< always false at present.
};

}
}
}

#endif 
