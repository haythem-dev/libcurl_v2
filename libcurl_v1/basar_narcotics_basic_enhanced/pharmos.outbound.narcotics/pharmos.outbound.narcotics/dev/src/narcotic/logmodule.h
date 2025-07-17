//------------------------------------------------------------------------------------------------//
/*! \file
 *  \brief  main interface for logging any changes of narcotic bookings
 *  \author Roland Kiefert
 *  \date   14.02.2006
 */
//------------------------------------------------------------------------------------------------//
#ifndef GUARD_LOGMODULE_H
#define GUARD_LOGMODULE_H

//------------------------------------------------------------------------------------------------//
// namespaces section
//------------------------------------------------------------------------------------------------//
namespace domMod
{

//------------------------------------------------------------------------------------------------//
/*! \brief	class logs every update, delete or restore changes of every narcotic booking.

	\n This is a final class.  
	\n throws no-throw */
//------------------------------------------------------------------------------------------------//
// class NarcoticsDM
//------------------------------------------------------------------------------------------------//
class LogDM
{
public:
	LogDM();
	
	//! \brief function where the Search Iterator and Instances of the Accessor Definitions are created. \n no-throw.
	void init( const basar::db::aspect::ConnectionRef activeConn, const basar::Int16 branchNo, const basar::VarString userName );

    bool isInitialized();

	//! \brief	resets all AccessorInstanceRef members. \n no-throw.
	void												shutdown();
	
	/*! \brief	method that inserts an empty row in the AccessorPropertyTable.
				The PropertyTable corresponds to the DB-Table narcprotocol. \n no-throw.*/
	basar::db::aspect::AccessorPropertyTable_YIterator	addEmptyProtocol();

	//---------------------------------------------------------------------------------------------------------//
	//	 get - methods
	//---------------------------------------------------------------------------------------------------------//
	/*! \brief	method that returns the AccessorPropertyTableRef corresponding to the DB-table narcprotocol. \n no-throw.*/
	basar::db::aspect::AccessorPropertyTableRef			getAccNarProtocolPropTab();		

	/*! \brief	method that returns the AccessorPropertyTableRef corresponding to the DB-table narctransactioncatalog.\n no-throw.*/
	basar::db::aspect::AccessorPropertyTableRef			getAccNarTransactionPropTab();	
	
	//! \brief	returns the search iterator. \n no-throw.
	basar::db::aspect::AccessorPropertyTable_YIterator	getSearchIterator();

	//! \brief	clears (every single attribute is set to the state SS_UNSET) the search iterator. \n no-throw.
	void												clearSearchIterator();

	//---------------------------------------------------------------------------------------------------------//
	//	 find - methods
	//---------------------------------------------------------------------------------------------------------//
	//! \brief	search for log entries depending on the search criteria in the passed yiterator. \n no-throw.
	basar::db::aspect::AccessorPropertyTableRef			findLogEntryByPattern(
															basar::db::aspect::AccessorPropertyTable_YIterator yit //!< yiterator, containing where-statement 
														);

	/*! \brief	search a definite narcotic booking depending on the search criteria in the passed yiterator. \n no-throw. */
	basar::db::aspect::AccessorPropertyTableRef			findCatalogByPattern(
															basar::db::aspect::AccessorPropertyTable_YIterator yit //!< yiterator, containing where-statement
														);
	//! \brief	search for log entries depending on the search criteria in the passed yiterator. \n no-throw.
	basar::db::aspect::AccessorPropertyTableRef			findLogsByPattern(
															const basar::db::aspect::ConnectionRef activeConn,	//!< current connection, for creating new temporary accessor instance at each function call
															basar::db::aspect::AccessorPropertyTable_YIterator yit //!< yiterator, containing where-statement
														);

	//---------------------------------------------------------------------------------------------------------//
	//	 save - methods
	//---------------------------------------------------------------------------------------------------------//
	/*! \brief	method that executes the SQL-Statement on narcprotocol when a record is changed. 
				An empty row is added to the PropertyTable, the values in this row are set and the Insert
				is executed on the DB. throw no-throw. */
	bool												saveLogEntryUpdate(		basar::Int32 catalogid, 
																				basar::VarString changedAttribute, 
																				basar::VarString valueOld, 
																				basar::VarString valueNew );

	/*! \brief	method that executes the SQL-Statement on narcprotocol when a record is deleted.
				An empty row is added to the PropertyTable, the values in this row are set and the Insert
				is executed on the DB. throw no-throw. */
	bool												saveLogEntryDelete(		basar::Int32 catalogid );
	
	/*! \brief	method that executes the SQL-Statement on narcprotocol when a record is restored.
				An empty row is added to the PropertyTable, the values in this row are set and the Insert
				is executed on the DB. throw no-throw. */
	bool												saveLogEntryRestore(	basar::Int32 catalogid );

	void												resetAccPrintProtList() { m_accPrintProtList.clear(); }
	
private:
	//---------------------------------------------------------------------------------------------------------//
	//	 members
	//---------------------------------------------------------------------------------------------------------//
	basar::db::aspect::AccessorInstanceRef					m_accNarProtocol;		//!< instance of narcotics-protocol-accessor
	basar::db::aspect::AccessorInstanceRef					m_accNarTransaction;	//!< instance of narcotics-protocol-accessor
	basar::Int16											m_BranchNo;				//!< branch number 
	basar::db::aspect::AccessorPropertyTable_YIterator		m_searchIterator;		//!< search iterator
	basar::db::aspect::AccessorPropertyTable_YIterator		m_insertIterator;		//!< insert iterator
	basar::VarString										m_userName;				//!< Username
	basar::Int32											m_Initialized;			//!< two meanings: a) to find out if the module is initialized at all and b) the method 'shutdown' is only run completly through if the module is not referenced any more.
	std::list<basar::db::aspect::AccessorInstanceRef>       m_accPrintProtList;
};

} //namespace domMod

#endif //GUARD_LOGMODULE_H