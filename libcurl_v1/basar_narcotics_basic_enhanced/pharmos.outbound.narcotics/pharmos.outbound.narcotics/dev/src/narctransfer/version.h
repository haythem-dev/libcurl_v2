//----------------------------------------------------------------------------
/*! \file
 *  \brief  version constants
 *  \author Michael Eichenlaub
 *  \date   28.02.2006 (created)
 */

#ifndef GUARD_TRANSFER_VERSION_H
#define GUARD_TRANSFER_VERSION_H

//----------------------------------------------------------------------------
//! major version number
#define VERSION_MAJORNO				"01"

//! minor version number
#define VERSION_MINORNO				"06"

//! sprint version number
#define VERSION_EXTENSION			"05"

//! revision version number
#define VERSION_BUILDNO				"00"

#define PROGRAM_NAME				"narctransfer"				//!< module name

//----------------------------------------------------------------------------

//! compound version string
#define VERSIONNO					VERSION_MAJORNO   "." \
									VERSION_MINORNO   "." \
									VERSION_EXTENSION "." \
									VERSION_BUILDNO

/*! \ingroup DIFF_DBG_REL */ 
#ifdef NDEBUG
//! release version number
#define VERSION_NUMBER				VERSIONNO" rel"
#else
//! debug version number  
#define VERSION_NUMBER				VERSIONNO" dbg"
#endif

//----------------------------------------------------------------------------
#endif	// GUARD

/*!
\page page1 narctransfer
\include svntags

________________________________________________________________________________________
\version	01.06.05.00
\date		11.03.2025
\internal
- P-23000125: bugfix acknowledgementstatus = 0 
\endinternal
________________________________________________________________________________________
\version	01.06.04.00
\date		13.04.2023
\internal
- CPR-230255: customersuppliercip will not be filled in table narctransactioncatalog
\endinternal
________________________________________________________________________________________
\version	01.06.03.00
\date		20.09.2016
\internal
- CR-16D048: Berücksichtigung von completedflag=2 (abhängig von UADM RELEVANT_COMPLETED_STATE)
\endinternal

________________________________________________________________________________________
\version	01.06.02.00
\date		23.07.2015
\internal
- CR-13D084: avoid identical timestamp for articleno / branchno
\endinternal

________________________________________________________________________________________
\version	01.06.01.00
\date		08.12.2014
\internal
- CR-14D063: customernomapping
\endinternal

________________________________________________________________________________________
\version	01.06.00.00
\date		28.01.2014
\internal
- SR-111842: m_ControlBreak.init()
- ssuk-01.09.01.00
\endinternal

________________________________________________________________________________________
\version	01.05.07.00
\date		26.11.2013
\internal
- avoid purchaseorderno = 0
\endinternal

________________________________________________________________________________________
\version	01.05.06.01
\date		24.04.2013
\internal
- Outage solution.
\endinternal

________________________________________________________________________________________
\version	01.05.06.00
\date		08.03.2013
\internal
 - CR-13D002(SOS): removed calculation error when article is locked and article was overtaken in KSC by another order before.
\endinternal

________________________________________________________________________________________
\version	01.05.05.00
\date		10.12.2012
\internal
- narctransfer fills new database field 'customersupplierstreet'
\endinternal

________________________________________________________________________________________
\version	01.05.04.00
\date		04.09.2012
\internal
- added faster query for selecting previous quantity in stock
\endinternal

________________________________________________________________________________________
\version	01.05.03.00
\date		27.08.2012
\internal
- try catch um unlock
\endinternal

________________________________________________________________________________________
\version	01.05.02.00
\date		10.08.2012
\internal
- try catch block around ret = basar::appl::SystemEventManager::getInstance().fire("TransferNarcTransactions", yitFromEvSrc, rSource);
  no more database exceptions -> should run
\endinternal

________________________________________________________________________________________
\version	01.05.01.00
\date		09.08.2012
\internal
- narctransfer is again branch non-specific
- narctransfer is now a full fledged background process - listening on SIGTERM
- narctransfer now holds his database connection permanently
\endinternal

________________________________________________________________________________________
\version	01.04.00.00
\date		07.08.2012
\internal
- narctransfer is again branch specific
\endinternal

________________________________________________________________________________________
\version	01.03.00.00
\date		10.07.2012
\internal
- New locking method for printing and locking of articles.
\endinternal

________________________________________________________________________________________
\version	01.02.02.00
\date		02.07.2012
\internal
- Added LTRIM RTRIM for selecting packageunit. Otherwise there are blanks in the string what leads to problems with halfyearly report.
\endinternal

________________________________________________________________________________________
\version	01.02.01.02
\date		04.06.2012
\internal
- Synchronization bug with printthread solved.
\endinternal

________________________________________________________________________________________
\version	01.02.01.01
\date		04.06.2012
\internal
- Synchronization bug with printthread solved.
\endinternal

________________________________________________________________________________________
\version	01.02.01.00
\date		04.04.2012
\internal
- Integration of tourid from KSC to print on delivery note.
\endinternal

________________________________________________________________________________________
\version	01.02.00.00
\date		20.04.2011
\internal
- Adaption for electronic contribution voucher.
\endinternal

*/
/*
 *  \version 01.01.01.00 30.12.2008 - 12:25h
 *           <ul><li>With consequence of the migrated incoming goods module it is necessary 
 *                   to accumulate all equal (purchase order no, transactiontype, supplierno, article no) unprocessed incoming goods positions. </li></ul>
 *  \version 01.01.00.06 27.10.2008 - 20:35h
 *           <ul><li>Little Adjustment regarding the new fields in the table narctransactioncatalog 'narc_factor'. </li></ul>
 *  \version 01.01.00.05 27.10.2008 - 11:10h
 *           <ul><li>Little Adjustment regarding the new fields in the table narctransactioncatalog 'narc_factor'. </li></ul>
 *  \version 01.01.00.04 27.06.2008 - 11:10h
 *           <ul><li>Little Adjustment regarding the new fields in the table narctransactioncatalog 'narc_factor'. </li></ul>
 *  \version 01.01.00.03 09.05.2008 - 11:10h
 *           <ul><li>Little Adjustment regarding new fields in the table narctransactioncatalog. </li></ul>
 *  \version 01.01.00.02 09.05.2008 - 11:10h
 *           <ul><li>Little Adjustment regarding unvalid supplier numbers in the method
                     'bool fetchPersonalData(basar::db::aspect::AccessorPropertyTable_YIterator& yit)'. </li></ul>
 *  \version 01.01.00.01 16.04.2008 - 11:10h
 *           <ul><li>Little Adjustment regarding negative return values of the method
                     'basar::Int32 TransferDM::makeInventoryUpdate()'. </li></ul>
 *  \version 01.01.00.00 10.04.2008 - 16:16h
 *           <ul><li>New programm version with the basar layer basar::db::aspect. </li></ul>
 *  \version 01.00.01.08 27.10.2006 
 *           <ul><li>Logging for change of transactiontype (from original to catalog) is built in
 *					 also dircet after the catalog insert execute(). </li></ul>
 *  \version 01.00.01.07 17.10.2006 
 *           <ul><li>Logging for change of transactiontype (from original to catalog) is built in 
 *					 before the catalog insert execute().
 *  \version 01.00.01.06 25.09.2006 
 *           <ul><li>TransactionType 's' (deu: Nachlieferung) will be regarded.
 *                   It will be shifted to the transactiontype 'a'. </li></ul>
 *  \version 01.00.01.05 18.07.2006 
 *           <ul><li>Table narctransactioncatalog was expanded by two new attributes.
 *                   These two attributes have to be provided with data. </li></ul>
 *  \version 01.00.01.04 27.06.2006 
 *           <ul><li>Bug fix: sql function NVL()-function integrated in DECODE-function </li></ul>
 *  \version 01.00.01.03 19.06.2006 
 *           <ul><li>versionision is integrated in the logging file. </li></ul>
 *  \version 01.00.01.02 19.06.2006 
 *           <ul><li>merging qtytransaction of incoming goods bookings with the same order. </li>
 *           <li>The transactiontypes (a,b,c) are transfered into the catalog tabel only. </li></ul>
 *  \version 01.00.01.01 14.06.2006 
 *           <ul><li>substitution of customersupplierno by bgano if transactiontype = 'b' (WE)</li></ul>
 *  \version 01.00.01.00 07.06.2006 
 *           <ul><li>added stock update</li></ul>
 *  \version 01.00.00.02 18.04.2006 
 *           <ul><li>adapted to ssuk.00.99.02.00</li></ul>
 *  \version 01.00.00.01 29.03.2006 
 *           <ul><li>transtype must be right trimmed</li></ul>
 *  \version 01.00.00.00 21.03.2006 
 *           <ul><li>first release</li></ul>
 *  \version 00.99.00.00 23.02.2006 
 *           <ul><li>first issue</li></ul>
 */
//----------------------------------------------------------------------------
