
//------------------------------------------------------------------------------------------------//
/*! \file
 *  \brief  main interface for domain class narcotics
 *  \author Roland Kiefert
 *  \date   14.02.2006
 */
//------------------------------------------------------------------------------------------------//
#ifndef GUARD_NARCOTICSMODULE_H
#define GUARD_NARCOTICSMODULE_H

//------------------------------------------------------------------------------------------------//
// include section
//------------------------------------------------------------------------------------------------//
#include "logmodule.h"
#include "accessor_definitions.h"
#include "executereturntype.h"
#include <lockmanager/branchlockmanager.h>

//------------------------------------------------------------------------------------------------//
// namespaces section
//------------------------------------------------------------------------------------------------//
namespace domMod
{
//------------------------------------------------------------------------------------------------//
/*! \brief  domain class for narcotics module.

	\n This is a final class.  
	\n throws no-throw */
//------------------------------------------------------------------------------------------------//
// class NarcoticsDM
//------------------------------------------------------------------------------------------------//
class NarcoticsDM
{
public:

	/*!\brief Default constructor.
		\n no-throw */
	NarcoticsDM();

	/*!\brief Default destructor.
		\n no-throw */
	virtual ~NarcoticsDM();

	/*! \brief	this method initializes the module narcoticsModule. 
				Creates several accessor instance member variables. \n no-throw */
	void													init(	const basar::Int16 branchNo,
																	const basar::db::aspect::ConnectionRef activeConn,
																	const basar::VarString& userName
																);

	//! \brief	this method sets the state of every property to SS_UNSET except for the brancho no. \n no-throw
	void													resetSearchIterator();

	//! \brief the passed yiterator will be copied to the back up yiterator member. \n no-throw
	const basar::db::aspect::AccessorPropertyTable_YIterator deepCopy( basar::db::aspect::AccessorPropertyTable_YIterator yit );

	//! \brief	resets all AccessorInstanceRef members. \n no-throw
	void													shutdown();

	/*! \brief	clears the AccessorPropertyTable of narcotic bookings and inserts a new one with the insert type 'basar::FOR_INSERT'.  
				\n no-throw */
	basar::db::aspect::AccessorPropertyTable_YIterator		addEmptyBooking();

	//-------------------------------------------------------------------------------------------------------------------------//
	//	find - methods
	//-------------------------------------------------------------------------------------------------------------------------//
	//! \brief one or more narcotics bookings are loaded depending on the search pattern. \n no-throw
	basar::db::aspect::AccessorPropertyTableRef			findBookingControlByPattern( 
																basar::db::aspect::AccessorPropertyTable_YIterator yit //!< yiterator, containing where-statement
															);
	
	//! \brief one or more narcotics bookings are loaded depending on the search pattern. \n no-throw
	basar::db::aspect::AccessorPropertyTableRef			findCatalogByPattern(
															basar::db::aspect::AccessorPropertyTable_YIterator yit //!< yiterator, containing where-statement
														);
	
    //! \brief one or more narcotics bookings are loaded depending on the search pattern. \n no-throw
	basar::db::aspect::AccessorPropertyTableRef			findDoubleBookedBookings(
															basar::db::aspect::AccessorPropertyTable_YIterator yit //!< yiterator, containing where-statement
														);
	
	//! \brief one or more narcotics bookings are loaded depending if the booking is not complete. \n no-throw
	basar::db::aspect::AccessorPropertyTableRef			findUncompleteBookings(
															basar::db::aspect::AccessorPropertyTable_YIterator yit //!< yiterator, containing where-statement
														);

	//! \brief one or more narcotics bookings are loaded in a temporary property table depending on the search pattern. \n no-throw
	basar::db::aspect::AccessorPropertyTableRef			findTempCatalogByPattern(
															basar::db::aspect::AccessorPropertyTable_YIterator yit		//!< yiterator, containing where-statement
														);

	//! \brief searches for qty in stock which the passed article has had in stock initially. \n no-throw
	basar::db::aspect::AccessorPropertyTable_YIterator	findInitialInventoryByArticleNo( const basar::Int32 articleNo, 
																						 const basar::Int16 branchNo,
																						 const basar::Long32 fromDate);

	//! \brief searches for the latest dataset showing differences in qty in stock. \n no-throw
	basar::db::aspect::AccessorPropertyTable_YIterator	findLatestDiffQtyByArticleNo( const basar::Int32 articleNo, 
																					  const basar::Int16 branchNo );

	//! \brief searches for all available supplier providing narcotics. \n no-throw
	basar::db::aspect::AccessorPropertyTableRef			findNarcoticsSupplier();
	
	//! \brief searches for one certain supplier providing narcotics. \n no-throw
	basar::db::aspect::AccessorPropertyTable_YIterator	findNarcoticsSupplierByBgaNo( const basar::Int32 bgaNo );

	//! \brief searches for a quantity of suppliers fullfilling given searchname criteria. \n no-throw
	basar::db::aspect::AccessorPropertyTableRef			findNarcoticsSupplierByName( const basar::VarString suppliername );

	//! \brief searches all article of a branch showing differences in qty in stock. \n no-throw
	basar::db::aspect::AccessorPropertyTableRef			findDifferencesToOriginal();
	
	//! \brief searches all article of a branch showing differences in qty in stock. \n no-throw
	basar::db::aspect::AccessorPropertyTableRef			findDifferencesToOriginalArticleNosOnly();

	//! \brief counts the unprocessed incoming goods (suppliers) as well as the unprocessed return goods (customers). \n no-throw
	basar::db::aspect::AccessorPropertyTableRef			findNoIncomingGoods();

	/*! \brief	search for one definite narcotic booking according to given branch and article no which is a previous one 
				in comparison to the given date and time parameter. \n no-throw */
	basar::db::aspect::AccessorPropertyTable_YIterator	findPreviousBooking(
															basar::db::aspect::AccessorPropertyTable_YIterator yit //!< parameter-list, containing where-statement
														);
	
	/*! \brief	select data for printing inventory list for count list. Only one parameter: branchid. \n no-throw */
	basar::db::aspect::AccessorPropertyTableRef			findData4InventoryCountPrint();

    /*! \brief	select data for printing inventory list for stock list. Parameters: branchid, transactiondate, transactiontime \n no-throw */
    basar::db::aspect::AccessorPropertyTableRef         findData4InventoryStockPrint( basar::db::aspect::AccessorPropertyTable_YIterator yit );

	/*! \brief	select all narcotics articles from artikelzentral. \n no-throw */
	basar::db::aspect::AccessorPropertyTableRef			findNarcArticles(
															basar::db::aspect::AccessorPropertyTable_YIterator yit //!< yiterator, containing specific articleno (might be empty -> than all narcotic articles are selected!)
														);

	basar::db::aspect::AccessorPropertyTableRef			findArticleNoByName( 
															basar::db::aspect::AccessorPropertyTable_YIterator searchIt //searchIt containing given articlenname or scheme to search articleno for
														);

	const narcotics::ExecuteReturnType					checkValidateBooking(
															basar::db::aspect::AccessorPropertyTable_YIterator yit //!< yiterator, if it correct values
														);
	/*! \brief	check if there are any bookings matching parameters in given search iterator. \n no-throw */
	const bool											checkBookingsExistance(
															basar::db::aspect::AccessorPropertyTable_YIterator yIt //!< parameter-list, containing where-statement
														);
	/*! \brief	select data for printing narcotics book. \n no-throw */
	basar::db::aspect::AccessorPropertyTableRef			findBookings4PrintCatalogByPattern(
															basar::db::aspect::AccessorPropertyTable_YIterator yit //!< yiterator, containing where-statement
														);

	/*! \brief	a report is printed over a period of time of a half year for the institution 'Bundesopiumstelle'. \n no-throw */
	basar::db::aspect::AccessorPropertyTableRef			findBookings4HalfYearlyReport(
															basar::Int32 fromDate,	//!< first date from the half year
															basar::Int32 toDate,	//!< last date from the half year
															basar::Int16 sort		//!< 1 for C1 and 2 C2 - report
														);
	//-------------------------------------------------------------------------------------------------------------------------//
	//	save - methods (updates, inserts)
	//-------------------------------------------------------------------------------------------------------------------------//
private:
	/*! \brief	if a narcotic booking is deleted, restored or manipulated regarding the attribuites 
				"qtytransaction", "transactiondate", "transactiontime" and "transactiontype", a stock update 
				(deu: Bestandsfortschreibung) has to be initiated, because there is a corresonding quantity in stock 
				with every single narcotic booking. \n no-throw. */
	const narcotics::ExecuteReturnType					stockUpdate(
															basar::db::aspect::AccessorPropertyTable_YIterator changedBooking,	//!< the change booking
															bool checkOnly = false, 												//!< if the parameter is set true then with a negative qty in stock calculation no data will be saved.
															bool onTheBasisOfUpdate = true //!< controls if the deep copy has to be used for updating the qty in stock
														);
public:
	/*! \brief	Internally is differentiated if an update or an insert has to be done. 
				Furthermore it is checked if the user has an exclusive access to the narcotic booking article 
				[ here: implemented with a pessimistic lock mechanism (only lock with writing) ]. \n no-throw. */
	const narcotics::ExecuteReturnType					saveBooking(
															basar::db::aspect::AccessorPropertyTable_YIterator yit, //!< the booking who will save
															bool checkOnly,									//!< if the parameter is set true then with a negative qty in stock calculation no data will be saved.
															bool valid 												//!< if the parameter are check
														);
	/*! \brief	Changed properties which therefore have to be updated are marked with SS_UPDATE. \n no-throw. */
	const narcotics::ExecuteReturnType                  saveOrderChanges(basar::db::aspect::AccessorPropertyTableRef probTab, basar::db::aspect::AccessorPropertyTable_YIterator firstPosYit);
	//Papierkorb
	/*! \brief	To mark a narcotic booking as deleted the attribute 'deletedflag' is set. Its a kind of electronic 'waste paper basket'.
				Again with checking of the locking mechanism. A stock update mechanism has to initiated. \n no-throw. */
	const narcotics::ExecuteReturnType					strikeOut(
															basar::db::aspect::AccessorPropertyTable_YIterator yit,	//!< the booking who strike out
															bool checkOnly = false									//!< if the parameter is set true then with a negative qty in stock calculation no data will be saved.
														);

	/*! \brief	If a narcotic booking is marked as deleted this deleted flag is reset to '0', i.e. is not regarded as deleted anymore.
				Again with checking of the locking mechanism. A stock update mechanism has to initiated. \n no-throw. */
	const narcotics::ExecuteReturnType					restoreStrikedOut(
															basar::db::aspect::AccessorPropertyTable_YIterator yit,	//!< the booking who restore
															bool checkOnly = false									//!< if the parameter is set true then with a negative qty in stock calculation no data will be saved.
														);
	//! For the given ordernumber the supplier will change to the given suppliernumber for all not processed bookings.
	const narcotics::ExecuteReturnType					saveSupplier(
															basar::VarString orderNo,	//!< number of the order in which the supplier will be replace
															basar::Int32 supplierNo	//!< number of the supplier you will be replace
														);
	
private:
	/*! \brief	With the given article no, the branch no and the username (both member variables - set by init()-method) 
				an insert is made in the table 'narclock'. 
				No transaction is used here as transactions are used in the invoking methods. \n no-throw */
	const narcotics::ExecuteReturnType					lock( const basar::Int32 articleNo );

	/*! \brief	To unlock the article with the passed article no, the concerning dataset in the table 'narclock' will be erased. 
				Considering the branchNo (member variable).
				No transaction is used here as transactions are used in the invoking methods. \n no-throw */
	const narcotics::ExecuteReturnType					unlock( const basar::Int32 articleNo );

public:
	const basar::db::aspect::ExecuteResultInfo			updateDiffQtyInStock();

	//-------------------------------------------------------------------------------------------------------------------------//
	//	check - methods
	//-------------------------------------------------------------------------------------------------------------------------//
	//! \brief	checks if there are any negative quantities in stock after the recording any narcotic booking manually. \no-throw 
	const bool											checkForNegativeStock( 
															basar::db::aspect::AccessorPropertyTable_YIterator yit 
														);
	
	//! \brief	checks if there are any narcotic bookings with diverging quantity of stock. \no-throw 
	const bool											checkExistDiffQtyInStock();

	//! \brief	checks if the given article is locked (table: narclock). \no-throw 
	const bool											checkIsArticleLocked( const basar::Int32 articleNo );

    const bool                                          isSupplierReplaceable();



	//-------------------------------------------------------------------------------------------------------------------------//
	//	get - methods
	//-------------------------------------------------------------------------------------------------------------------------//
	//! \brief returns the search iterator. \n no-throw.
	basar::db::aspect::AccessorPropertyTable_YIterator			getSearchIterator();

	//! \brief returns the back up yiterator. It will be used in the case of an update. \n no-throw.
	const  basar::db::aspect::AccessorPropertyTable_YIterator	getBackUpCurrentIterator() const;

	//! \brief AccessorPropertyTableRef for narcotic bookings used for investigations. \n no-throw
	basar::db::aspect::AccessorPropertyTableRef					getAccBookingControlPropTab();

	//! \brief AccessorPropertyTableRef for narcotic bookings. \n no-throw.
	basar::db::aspect::AccessorPropertyTableRef					getAccNarCatalogPropTab();

	//! \brief AccessorPropertyTableRef for all narcotic suppliers. \n no-throw.
	basar::db::aspect::AccessorPropertyTableRef					getAccNarSupplierPropTab();

	//! \brief AccessorPropertyTableRef for all returning googs (customer). \n no-throw.
	basar::db::aspect::AccessorPropertyTableRef					getAccNoReturnGoodsPropTab();
	
	//! \brief AccessorPropertyTableRef for all incoming goods (supplier). \n no-throw.
	basar::db::aspect::AccessorPropertyTableRef					getAccNoIncomingGoodsPropTab();

	/*! \brief	AccessorPropertyTableRef for all narcotic bookings that differ in the stock quantity 
				comparing original (PHARMOS) and narcotics bookings. \n no-throw. */
	basar::db::aspect::AccessorPropertyTableRef					getAccDiffQtyInStock();

	/*! \brief	AccessorPropertyTableRef for all narcotic bookings that differ in the stock quantity 
				comparing original (PHARMOS) and narcotics bookings. \n no-throw. */
	basar::db::aspect::AccessorPropertyTableRef					getAccArticleNosDiffQtyInStock();

	basar::db::aspect::AccessorPropertyTableRef					getAccNarInitialInventoryPropTab();

	//-------------------------------------------------------------------------------------------------------------------------//
	//	set - methods
	//-------------------------------------------------------------------------------------------------------------------------//
	void														resetAccPrintBookingsList() { m_accPrintBookingsList.clear(); }

private:
	//---------------------//
	// private methods
	//---------------------//
	/*!	\brief	the deep copy stores the values of a yiterator right before the new values of the View connector are matched.
				This deep copy is compared value based with current values of the database the same database set concerned, 
				to decide if an update is necessary. \n no-throw. */
	const bool													isDeepCopyEqualToCurrentDatasetOfDB();
	const narcotics::ExecuteReturnType							queryCurrentDatasetOfDB();
	
//	getter-methods for initializing and returning accessor instances of NarcoticsDM
// ToDo: accessor macros have to be revised

	basar::db::aspect::AccessorInstanceRef						getNarCatalogACC();
	basar::db::aspect::AccessorInstanceRef						getInitialInventoryACC();
	basar::db::aspect::AccessorInstanceRef						getNumberIncomingGoodsACC();
	basar::db::aspect::AccessorInstanceRef						getNumberReturnGoodsACC();
	basar::db::aspect::AccessorInstanceRef						getNarcoticsSupplierACC();
	basar::db::aspect::AccessorInstanceRef						getUpdQtyInStockACC();
	basar::db::aspect::AccessorInstanceRef						getDiffQtyInStockACC();
	basar::db::aspect::AccessorInstanceRef						getArticleNosDiffQtyInStockACC();
	basar::db::aspect::AccessorInstanceRef						getNarcLockACC();
	basar::db::aspect::AccessorInstanceRef						getCompareBookingACC();
	basar::db::aspect::AccessorInstanceRef						getInventoryCountDataACC();
	basar::db::aspect::AccessorInstanceRef						getPrintNarcBookACC();
	basar::db::aspect::AccessorInstanceRef						getPrevBookingACC();
	basar::db::aspect::AccessorInstanceRef						getNarcArticleACC();
	basar::db::aspect::AccessorInstanceRef						getBookingControlACC();
	basar::db::aspect::AccessorInstanceRef						getTempCatalogACC();

	//---------------------//
	// private variables
	//---------------------//
// accessor members created by using accessor macro from accessor_definitions.h

	CREATE_ACCESSOR_INSTANCE_DECL( NarCatalog )					//!< instance of narcotics-booking-accessor
	
	CREATE_ACCESSOR_INSTANCE_DECL( InitialInventory )			//!< instance of accessor for initial inventory
	
	CREATE_ACCESSOR_INSTANCE_DECL( NumberIncomingGoods )		//!< instance of number-incoming-goods-accessor
	
	CREATE_ACCESSOR_INSTANCE_DECL( NumberReturnGoods )			//!< instance of number-return-goods-accessor
	
	CREATE_ACCESSOR_INSTANCE_DECL( NarcoticsSupplier )			//!< instance of narcotics supplier
	
	CREATE_ACCESSOR_INSTANCE_DECL( UpdQtyInStock )				//!< instance of narcotics-booking-accessor for updating qtyinstock
	
	CREATE_ACCESSOR_INSTANCE_DECL( DiffQtyInStock )				//!< instance of difference-quantity-in-stock-accessor
	
	CREATE_ACCESSOR_INSTANCE_DECL( ArticleNosDiffQtyInStock )	//!< instance of difference-quantity-in-stock-accessor
	
	CREATE_ACCESSOR_INSTANCE_DECL( NarcLock )					//!< instance of article-locked-accessor
	
	CREATE_ACCESSOR_INSTANCE_DECL( CompareBooking )				//!< instance to compare the deep copy with the db state of the current dataset.
	
	CREATE_ACCESSOR_INSTANCE_DECL( InventoryCountData )			//!< instance of narcotics-inventory-list-accessor for selecting current stock (and further info) of all narcotics of a branch
	
	CREATE_ACCESSOR_INSTANCE_DECL( PrintNarcBook )				//!< instance of narcotics-bookings-list-accessor for selecting data to print narcotics book
	
	CREATE_ACCESSOR_INSTANCE_DECL( PrevBooking )				//!< instance of accessor for selecting previous booking
	
	CREATE_ACCESSOR_INSTANCE_DECL( NarcArticle )				//!< instance of accessor for selecting all narc articles
	
	CREATE_ACCESSOR_INSTANCE_DECL( BookingControl )				//!< instance of accessor for selecting all narc booking for inquiry

	CREATE_ACCESSOR_INSTANCE_DECL( TempCatalog )				//!< instance of accessor for selecting all row of TempCatalog

	basar::db::aspect::ConnectionRef							m_ConnectionRef;

	basar::Int16												m_BranchNo;					//!< branch number 
	basar::db::aspect::AccessorPropertyTable_YIterator			m_searchIterator;			//!< search iterator
	basar::db::aspect::AccessorPropertyTable_YIterator			m_YIteratorBackUp;			//!< saves a deep copy of the yiterator values of the currently processed row
	domMod::LogDM												m_LogDM;					//!< domain module class for log
	basar::Int32												m_Initialized;				//!< two meanings: a) to find out if the module is initialized at all and b) the method 'shutdown' is only run completly through if the module is not referenced any more.
	std::list<basar::db::aspect::AccessorInstanceRef>			m_accPrintBookingsList;
	boost::shared_ptr<libnarcotics::lockmanager::BranchLockManager>	m_LockManager;
};

} //namespace domMod

#endif //GUARD_NARCOTICSMODULE_H