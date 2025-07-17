//---------------------------------------------------------------------------------------------------------//
/*! \file
 *  \brief  definition narcotics version
 *  \author Thomas Hörath
 *  \date   10.02.2006
 */
//---------------------------------------------------------------------------------------------------------//

#ifndef GUARD_NARCOTICS_VERSION_H
#define GUARD_NARCOTICS_VERSION_H

//! major version number of narcotics
#define VERSION_MAJORNO           "01"

//! minor version number of narcotics
#define VERSION_MINORNO           "10"

//! extension version number of narcotics
#define VERSION_EXTENSION         "09"

//! build version number of narcotics
#define VERSION_BUILDNO           "01"

//! version number string of narcotics
#define VERSIONNO				VERSION_MAJORNO "." VERSION_MINORNO "." VERSION_EXTENSION "." VERSION_BUILDNO

//! version number string of basar to check
#define BASAR_VERSIONNO			"01.14.00.01"


#ifdef NDEBUG
//! version number string release of narcotics
#define VERSION_NUMBER			VERSIONNO" rel"
#else
//! version number string debug of narcotics
#define VERSION_NUMBER			VERSIONNO" dbg"
#endif

//! narcotics name
#define PROGRAM_NAME			"narcotic"

#endif

/*******************************************************************************************/
/*!
\mainpage Narcotics
\include svntags
________________________________________________________________________________________
\version	01.10.09.01
\date		27.05.2025
\internal
- CPR-250122: move company name to UADM Parameter REPORT_COMPANY_NAME - Hageda Name Change
\endinternal

________________________________________________________________________________________
\version	01.10.09.00
\date		11.03.2025
\internal
- P-23000125: narctransfer bugfix acknowledgementstatus = 0
\endinternal
________________________________________________________________________________________
\version	01.10.08.00
\date		18.02.2025
\internal
- CPR-230901: new SSUK version 01.14.00.01 with new ghostscript version
\endinternal
________________________________________________________________________________________
\version	01.10.07.01
\date		07.02.2024
\internal
- CPR-240065: Optimization Narcotics - New dialog for checking connection to printer (after bugfix)
\endinternal
________________________________________________________________________________________
\version	01.10.07.00
\date		03.07.2024
\internal
- CPR-240065: Optimization Narcotics - New dialog for checking connection to printer
\endinternal
________________________________________________________________________________________
\version	01.10.06.00
\date		03.07.2024
\internal
- I-24018092: Bugfix Bestandsliste/Zaehlliste
- SR-24026985: Bugfix: NA02/NA04 Retoure/Wareneingang Adresse wird nicht aktualisiert
\endinternal
________________________________________________________________________________________
\version	01.10.04.00
\date		04.10.2023
\internal
- I-23054633: NA12 PLZ wird beim ändern der Satzart nicht aktualisiert
\endinternal
________________________________________________________________________________________
\version	01.10.03.00
\date		10.08.2023
\internal
- I-23031211: NA03 Ausdruck anpassen
- CPR-230255 - Adresserweiterung Narcotics NA03 BTM-Buch Narcotics Part2
\endinternal
________________________________________________________________________________________
\version	01.10.02.00
\date		13.04.2023
\internal
- Add 2 new columns street and zip code in the NA01 and NA03 dialogs. Adapt narctransfer so the column customersuppliercip is being populated.
\endinternal
________________________________________________________________________________________
\version	01.10.01.03
\date		30.09.2021
\internal
- Process All function, it should take in to account the date processing range for the previous half and next half of year.
  After process The Grid needs to be refrehed with the next set of articles to be processed.
\endinternal
________________________________________________________________________________________
\version	01.10.01.00
\date		03.09.2020
\internal
- SR-20096030: Fehlermeldung, wenn Aktualisierung in NA09 ausgewählt wurde.
btm Feld in artikelzentral nicht enthalten - geändert auf bitand
\endinternal
________________________________________________________________________________________
\version	01.10.00.00
\date		21.08.2020
\internal
- CPR-190821: Optimierung BTM Ebene 2
Sortierung der BTM-Scheine aus Narcotics nach LANR
Angefordert wird der Druck des BTM-Scheins durch Narcotics über Pharmos (DRKSC / Phadex)
I.A. erfolgt der Druck über Kundenauftragsnummer.
Dies erschwert die Verarbeitung im BTM-Raum da der Druck der Wannenlabels und der Druck der BTM Scheine nicht synchron erfolgt;
sobald mehrere Belege/Aufträge  innerhalb kürzester Zeit  gedruckt werden.
Ziel ist dass Narcotics die Belege nach Lagerauftrag an den Drucker sendet.
\endinternal
________________________________________________________________________________________
\version	01.09.05.00
\date		14.08.2020
\internal
- CPR-171361 Informix Upgrade Step 3 - anstatt zartwg, zdkbtm und bliefer (Inbound Tabellen) werden jetzt die neuen Outbound Tabellen tradegroup, narcgroup und pusupplier verwendet.
\endinternal
________________________________________________________________________________________
\version	01.09.04.00
\date		18.06.2020
\internal
- CPR-200146 (Anpassung der Satzarten im BTM-Buch). Diesbezüglich muss im BTM Buch nur eine Vorzeichen (+ / -) vor der entsprechen Mengenbuchung angezeigt werden.
\endinternal
________________________________________________________________________________________
\version	01.09.03.00
\date		26.02.2019
\internal
- SR-16060981 - blauer Balken verschwindet, wenn Druckprozess mittlerweile von einem anderen User gestartet wurde und somit auf dem eigenen Rechner nicht mehr arbeitet.
- SR-16088880 - Storno in NA12 auch für transactiontype "d"
- SR-16095617 - Bugfix NA12: korrekte Bestandsrechnung bei Abgängen und Änderung Zeitstempel
- SR-16104459 - Berücksichtigung customernomapping auch bei Nachdruck Lieferschein / Behebung Druckfehler
- SR-17004581 - Zurücksetzen des Übertragungsstatus bei Änderung auf Vernichtung, so dass keine fälschliche Übertragung eines Abgabebelegs mehr passieren kann.
- NA18 / LS- Doppel: Anzeige / Druck von PZN 8 statt interner Artikelnummer
- SR-18004790 - Wiederherstellen von transactiontype = 'd', acknowledgmentstatus = 5 ist erlaubt
- ssuk-01.12.01.01
\endinternal

________________________________________________________________________________________
\version	01.09.02.01
\date		18.05.2016
\internal
- NA12: Abfangen, dass bei Positionsstorno mind. 1 Zeile angehakt sein muss.
\endinternal

________________________________________________________________________________________
\version	01.09.02.00
\date		27.04.2016
\internal
- Nachdruck Abgabebeleg aus NA03 nicht nur bei Änderung TransactionType.
- SR-16028041: Anzeige PZN8 beim Beheben von Druckfehlern
- Protokollierung (narcprotocol) von Änderungen in NA12
\endinternal
________________________________________________________________________________________
\version	01.09.01.02
\date		11.03.2016
\internal
- INC-16008444: init BranchDM before printing destruction.
\endinternal
________________________________________________________________________________________
\version	01.09.01.01
\date		08.02.2016
\internal
- NA08: Bei Suche nach Vernichtung in der Ergebnisliste PZN (nicht Kunde) anzeigen.
\endinternal

________________________________________________________________________________________
\version	01.09.01.00
\date		03.02.2016
\internal
- Vernichtungsbeleg nur noch über NA12 (= im Rahmen eines Auftrags) druckbar.
\endinternal

________________________________________________________________________________________
\version	01.09.00.00
\date		19.01.2016
\internal
- SR-15032343: acknowledgementstatus auch nur für Teile eines Auftrags automatisch von 6 auf 1 zurücksetzen (bei contributionvoucherno = 0), wenn es keine anderen Positionen mit status=6 und contributionvoucherno != 0 gibt.
- SR-15047863: auch beim Beheben von Druckfehlern abfangen, ob das Übertragen der Datei geklappt hat!
- CR-15D047: Änderungen in NA12 (Auftragsübersicht) + Default- Button
- unnötige Buttons entfernt (NA03, NA12, NA17, Hauptmenü)
- NA17: Sortierung auch "nur nach Tour" (ohne Datum)
- Sortierung Vernichtungsbeleg nach Artikelname
- NA08: Suche nach Satzart
\endinternal

________________________________________________________________________________________
\version	01.08.01.03
\date		05.03.2015
\internal
- I-15002671: qtytransaction=0 erlauben.
\endinternal

________________________________________________________________________________________
\version	01.08.01.02
\date		16.02.2015
\internal
- SR-15004967: customersupplierlocation instead of customersuppliercity.
\endinternal

________________________________________________________________________________________
\version	01.08.01.01
\date		02.01.2015
\internal
- SR-14034248: no default-text for theft / lost.
\endinternal

________________________________________________________________________________________
\version	01.08.01.00
\date		15.12.2014
\internal
- new dialog NA22 for maintaining customernomapping.
\endinternal

________________________________________________________________________________________
\version	01.08.00.02
\date		28.10.2014
\internal
- bugfix NA12 detected by RC.
\endinternal

________________________________________________________________________________________
\version	01.08.00.01
\date		18.09.2014
\internal
- bugfix NA12 detected by testteam.
\endinternal

________________________________________________________________________________________
\version	01.08.00.00
\date		18.08.2014
\internal
- Visual Studio 2010 + ssuk-01.10.00.00 (Qt5).
- BI server as UADM parameter.
\endinternal

________________________________________________________________________________________
\version	01.07.00.03
\date		08.08.2014
\internal
- validators for orderno reset to length 7 (because of scanning of GI label).
- PZN8 bugfix in compare bookings.
\endinternal

________________________________________________________________________________________
\version	01.07.00.02
\date		05.08.2014
\internal
- NA01 / NA02: validators.
- minor BugFixes detected by test team.
- NA12 button activation.
- NA01 When a different quantity is entered, the question message box defaults to "No" instead of "Yes" (SR-112299).
- NA17 It is now possible to select multiple lines and reprint delivery note / mark as received / unmark as received for those (CR13D090).
- NA03 initial stock only selected from not deleted transactions (I-14001194).
- NA17 New whorkflow during creation of delivery note copy (CR14D001).
\endinternal

________________________________________________________________________________________
\version	01.07.00.01
\date		15.01.2014
\internal
- BugFixes detected by test team.
- INC-043940: Korrektur bei Berechnung der laufenden Wochennummer bei Jahreswechsel.
- INC-044028: Marathon Listendruck nicht möglich (NA03 - Problem mit führenden Nullen im Artikelcode).
- INC-044101: Ausdruck C4- Bericht für HJM - Select korrigiert.
\endinternal

________________________________________________________________________________________
\version	01.07.00.00
\date		19.12.2013
\internal
- PrintThread: PrintedTime will be stored in the database and shown in NA17 & NA20.
- UnixFoundation: Linking against new SSUK 01.09.01.00.
- UnixFoundation: Linking against new SSUK 01.09.00.02.
- UnixFoundation: Linking against new SSUK 01.09.00.00.
- NA19: Show only contribution voucher numbers != 0 to resolve.
- NA01: Better visibility that the input focus is in the quantity column.
- NA07: article number validator corrected (NA07_TC05/07).
- minimum date for all date edits set to 01/01/2000.
- NA03: button activations corrected.
- NA10: button activations corrected.
- NA10: button activations corrected.
- BTM_RT_NA06_DEF01: No more unknown exception when a blank line is selected and the "x" button is pressed.
- SR-107444: also replace customer supplier street.
- CR-13D083: new button in NA17 to reset status from "received" to "printed".
- CR-13D109 (1st part): create delivery note copy in NA17 also if former status is "printed".
- CR-13D085: column order and headers changed in NA17.
- CR-13D089: cursor behaviour NA01.
- CR-13D087: NA13 displays additional article master data now.
- CR-13D109 (2nd part): disable changes of orders for which a delivery note copy was created before in NA03 + NA12.
- CR-13D086: search for / order by tour in NA17.
- CR-13D088: skip locked customers when searching from NA08.
- ssuk-01.09.01.00
\endinternal

________________________________________________________________________________________
\version	01.06.11.00
\date		20.08.2013
\internal
- BTM_NA19_TC04: Added info message box to NA19 for button 'show details'
- BTM_NA03_TC34: NA14 / NA15 removed at all.
- BTM_NA02_DEF03: Articleno size limit set on 8 digits.
- NA06: narcotic group will be increased to 5 digits.
- BTM_NA08_DEF02: Search function will be not consider the date if it isn´t checked. INC-039728
- NA21: INC-039728 Return or enter key will not close an error message box anymore (F1 or ESC closes the msg box), message box will blink to warn the user.
- NA19: SR-102524 Improved error handling (basar::exception instead of unknown exception).
- SR-102259: Kommentar für Lieferscheindoppel nun max. 150 Zeichen und ohne Zeilenumbruch.
- SR-101952: keine MsgBox, wenn Nachdruck aus NA17 erfolgreich.
- CR-12D139: Berechnung Bestandsdifferenzen.
\endinternal

________________________________________________________________________________________
\version	01.06.10.01
\date		10.07.2013
\internal
- BTM_NA21_TC06: Connect function for pressedEnter() deleted.
- BTM_NA19_TC06: Connect functions will be called correctly now.
- BTM_NA05_Delete_Option: Deleting causing negative qtyinstock fixed.
- Print buttons on the mainwindow will be disabled correctly after user relog, if the user has no print permissions.
\endinternal

________________________________________________________________________________________
\version	01.06.10.00
\date		03.07.2013
\internal
- BTM_NA03_TC08: articleno size limit set on 8 digits.
- BTM_NA03_TC20: implement date validator. from and to date can´t be greater then the current date.
- BTM_NA03_TC26: error message shows correct articleno now, when the system don´t find any record.
- BTM_NA03_TC32: cancellation is deactivated if transactiontype is customer order or supplier returns.
- Bugfix INC-037819 - update customersupplierstreet.
- NA01 Search by ArticleNo: articleno size limit set on 8 digits.
- BTM_NA08_TC08: articleno size limit set on 8 digits.
- BTM_NA17_TC09: implement date validator.
- BTM_NA17_TC13: implement date validator.
- BTM_NA08_TC34: changed QIntValidator to QRegExpValidator for bgano
- BTM_NA08_TC23: changed QIntValidator to QRegExpValidator for supplierno
- BTM_NA05_TC09: changed QIntValidator to QRegExpValidator for articleno
- BTM_NA05_TC09: implement date validator.
- BTM_NA08_TC74, BTM_NA06_TC07, BTM_NA08_TC48, BTM_NA08_TC56, BTM_NA08_TC57: Take over button in NA13 is disabled now, when selected row is empty.
- BTM_NA05_TC09: changed ARTICLENO to ARTICLECODE
- Add maximum date to QDateEdit fields. (current date + one year)
- BTM_NA21_TC06: alert for scanner input less than 16 signs.
- BTM_NA19_TC06: resolving of print errors only for transactiontypes a + d. Initialization of SupplierDM.
- BTM_NA03_TC31: reload table widget after deleting of an transaction.
- BTM_NA10_TC12: search in NA03 after coming back from NA10 only if search button is activated.
- BTM_NA07_TC09: attribute names can now be entered as database column names directly (e.g. qtytransaction) OR as translated attribute name (e.g. Menge).
- Add "- NAxx" to main menue shortcut names
- Qt auto connects deletet. Added wireEvent-functions and connect-functions to VCs.
- SR-098939: Tour-ID will be shown on NA17 acknowledgement list.
\endinternal

________________________________________________________________________________________
\version	01.06.09.01
\date		14.06.2013
\internal
- INC-038370 + INC-038372: Nachdruck Lieferschein enthält nun wieder alle Artikel.
\endinternal

________________________________________________________________________________________
\version	01.06.09.00
\date		24.05.2013
\internal
 - SR-095421: new column tourid in NA17.
 - SR-095834: column "status" also printed on NA17-list (pp5).
 - SR-094300: Refactoring logging.
 - Printthread does not open / close connection every 5 seconds anymore.
 - INC-037437: 1st part = update narctransactioncatalog.customersupplierstreet.
 - NA12: bugfix, when only time ist changed.
 - INC-037437: 2nd part = add supplier select at customermodule
 - fixed SelAcknowledgementsUnion's SQL query, it's using transactiontype to decode if customersupplier lookup should happen in customer or supplier table;
 - it should not display two entries for one contributionvoucher when customer is also in suppliertable
\endinternal

________________________________________________________________________________________
\version	01.06.08.11
\date		03.04.2013
 \internal
 - article locks are removed at the end of narcoticsDM's saveOrderChanges (business type: transfer_booking).
 - added print list feature to NA17
 - search fields in NA01 search also on pressing enterkey
 - if results are found in NA01 first qty cell is selected for input - also after saves
 \endinternal

________________________________________________________________________________________
\version	01.06.08.10
\date		18.02.2013
 \internal
 - fixed mark received button
 - possibility to unlock articles (business type: transfer_booking).
 - removed typo in acc_bookingcatalog's UpdSupplier
 \endinternal

________________________________________________________________________________________
\version	01.06.08.09
\date		30.01.2013
 \internal
 - reset focus to NA21's line edit field
 - dissolve receipt now uses basar::VarString for formating
 \endinternal

________________________________________________________________________________________
\version	01.06.08.08 29.01.2013
 \internal
 - button close in NA21 won't close frame on enter keypress anymore
 \endinternal

________________________________________________________________________________________
\version	01.06.08.07 22.01.2013
 \internal
 - finished adding Contribution Voucher Scan dialog
 - finished redesigning NA17
 - further changes to order states (see version 01.06.07.03 and 01.06.05.06)
 \endinternal

________________________________________________________________________________________
\version	01.06.08.06 04.01.2013
 \internal
 - more robust statements for supplier selection
 \endinternal

________________________________________________________________________________________
\version	01.06.08.05 03.01.2013
 \internal
 - dialog NA03 removes leading zero on search
 - dialog NA03 allows printing narc book only for one PZN and in from-to date intervall again
 - Bugfixes PZN8: Search for article number with leading zeros now possible (NA01, NA02).
 - dialog NA12: upon changing transactiondate and/or transactiontime for whole order recalc of qty in stock is done
 - made fast version of stockcountlist even faster
 - fixed display issue in narcprintbook pdf with shorter than max article numbers, furthermore all articlenumbers are now 8 characters long
 - fixed rare week calculation bug, if 31.12. is in first calendar week of next year, result was negative
 \endinternal

________________________________________________________________________________________
\version	01.06.08.04 21.12.2012
 \internal
 - restored sort order of positions in incominggoodsoverview
 - fixed permissions of deliverynotecopy folder, defaulting to 777
 \endinternal

________________________________________________________________________________________
\version	01.06.08.03 18.12.2012
 \internal
 - fixed article locking issue in dialog NA10
 \endinternal

________________________________________________________________________________________
\version	01.06.08.02 17.12.2012
 \internal
 - resolveprinterrors dialog recolors all tablewidget rows, fixable rows should be displayed in black font
 - fixed statement bug in acc_contributionvoucher, statement was not using preferred_flag
 \endinternal

________________________________________________________________________________________
\version	01.06.08.01 13.12.2012
 \internal
 - fixed dialog NA04 when used from booking catalog
 \endinternal

________________________________________________________________________________________
\version	01.06.08.00 13.12.2012
 \internal
 - merged in PZN8 changes
 - added faster query for stock list generation
 - changed orderoverview dialog to allow line code changes for all order positions
 \endinternal

________________________________________________________________________________________
\version	01.06.07.05 27.11.2012
 \internal
 - re-reverted union selection in contribution voucher dialog
 \endinternal

________________________________________________________________________________________
\version	01.06.07.05 27.11.2012
 \internal
 - re-reverted union selection in contribution voucher dialog
 \endinternal

________________________________________________________________________________________
\version	01.06.07.04 27.11.2012
 \internal
 - reverted union selection in contribution voucher dialog
 \endinternal

________________________________________________________________________________________
\version	01.06.07.03 22.11.2012
 \internal
 - changed orders state to INVALID (-1) for fixing NA03's and NA12's behaviour, see version 01.06.05.06
 - setting an order to status IN_PRINT now observes the deletedflag, deleted positions will not be set to IN_PRINT
 \endinternal

________________________________________________________________________________________
\version	01.06.07.02 29.10.2012
 \internal
 - improved performance of robust printing
 - added check if voucher is still IN_PRINT before voucher is fixed in resolveprintingerror dialog
 - ensured that printthread is stopped on mainwindow close
 - changed communtication between application and printthread to Qt Signals
 \endinternal

________________________________________________________________________________________
\version	01.06.07.00 19.10.2012
 \internal
 - added a more robust kind of printing:
 - printthread checkes before printing if printlock is still held by logged in user
 - printthread reads only one order from datebase
 - printthread marks order as 'in_print' before printing
 - printthread tries to restart failed prints (contributionvoucherno still 0)
 - printthread notifies user if 'in_print' datasets are found
 - added new dialog enabling user to fix 'in_print' orders
 - fixing of 'in_print' orders observes transfer to bfarm, meaning no order before last sunday can be fixed
 \endinternal

________________________________________________________________________________________
\version	01.06.05.06 08.09.2012
 \internal
 - dialogs NA03 and NA12 now observe user's wish if no contributionvoucher has to be printed
 \endinternal

________________________________________________________________________________________
\version	01.06.05.04 10.09.2012
 \internal
 - reprint of contributionvoucher now uses customername from narctransactioncatalog instead of customername from table kunde, needed for reprint when customname has been changed - replacement takes place in accessor on usecase level
 - fixed delivery note copies for contributionvouchers for suppliers, names were not set/empty
 - delivery note copies also use names form narctransactioncatalog instead from table kunde (both customer and supplier)
 \endinternal

________________________________________________________________________________________
\version	01.06.05.03 06.09.2012
 \internal
 - fixed bug in inventroyprintdata
 \endinternal

________________________________________________________________________________________
\version	01.06.05.02 05.09.2012
 \internal
 - Removed (hopefully) all I18nString.formats from objects accessed by printthread due to threading issues. PrintThread does not use loginmanager any more, it get's it's params by narcotics appl
 \endinternal

________________________________________________________________________________________
\version	01.06.05.01 03.09.2012
 \internal
 - Removed I18nString.formats from object accessed by printthread due to threading issues. Objects use VarString.formats now.
 - fixed check for messagebox return value in bookingcatalog.cpp line 410
 \endinternal

________________________________________________________________________________________
\version	01.06.05.00 22.08.2012
 \internal
 - When contribuitionvoucher is scanned, acknowledgement receiving button got the focus automatically
 - Application Controller is now initialized immediately after relogin. Otherwise Lockmanager crashes because of invalid connection
 \endinternal

________________________________________________________________________________________
\version	01.06.04.00 16.08.2012
 \internal
 - reprinting adapted
 \endinternal

________________________________________________________________________________________
\version	01.06.03.00 14.08.2012
 \internal
 - transactiontyxpe d will be also printed
 \endinternal

________________________________________________________________________________________
\version	01.06.02.00 14.08.2012
 \internal
 - made cancelation and stock update more robust
 - removed mismatching on narcunit
 - fixed generation of contribution voucher - will contain only one position
 \endinternal

________________________________________________________________________________________
\version	01.06.01.00 08.08.2012
 \internal
 - bufixes
 \endinternal

________________________________________________________________________________________
\version	01.06.00.00 08.08.2012
 \internal
 - added cancelation of order with removing contributionvouchers, both whole order and any number of positions
 \endinternal

________________________________________________________________________________________
\version	01.05.07.00 01.08.2012
 \internal
 - changed sortorder of contributionvouchers from sorting by order to sorting by date
 \endinternal

________________________________________________________________________________________
\version	01.05.06.00 26.07.2012
 \internal
 - Bugfix when reprinting deliverynotes -> workaround call updatedata
 \endinternal

________________________________________________________________________________________
\version	01.05.05.00 26.07.2012
 \internal
 - Select row in contributionvouchervc adapted
 \endinternal

________________________________________________________________________________________
\version	01.05.04.00 26.07.2012
 \internal
 - Select row in contributionvouchervc adapted
 \endinternal

________________________________________________________________________________________
\version	01.05.03.00 26.07.2012
 \internal
 - select complete text in contributionvoucher
 \endinternal

________________________________________________________________________________________
\version	01.05.02.00 25.07.2012
 \internal
 - Bugfix in setting acknowledgementstatus
 \endinternal

________________________________________________________________________________________
\version	01.05.01.00 20.07.2012
 \internal
 - Printing of contributionvouchers is now cancelable
 - Fixed bug in TimeChange-Diablog: Matcher used whole field instead of unit (e.g. "50 GR" instead of "GR")
 - Added username and branchno to various log messages
 \endinternal

________________________________________________________________________________________
\version	01.05.00.00 10.07.2012
 \internal
 - New locking method for printing and locking of articles
 - cleaned code of printthread.cpp
 \endinternal

________________________________________________________________________________________
\version	01.04.05.00 26.06.2012
 \internal
 - No "automatic enter" after scanning contributionvoucherno in contributionvouchervc, because no reprinting of deliverynote is possible otherwise
 \endinternal

________________________________________________________________________________________
\version	01.04.04.04 24.05.2012
 \internal
 - Cancellation of bookings in narcoticbook has been corrected. New contribution voucher is now created for the other (uncancelled) bookings.
 - If print file doesn't contain user's signature, button start print in main dialog won't be enabled anymore.
 - Progress bar in main dialog added in order to show user when printthread is active.
 \endinternal

________________________________________________________________________________________
\version	01.04.04.03 18.05.2012
 \internal
 - Cancellation of bookings in narcoticbook has been corrected. New contribution voucher is now created for the other (uncancelled) bookings.
 - If print file doesn't contain user's signature, button start print in main dialog won't be enabled anymore.
 - Progress bar in main dialog added in order to show user when printthread is active.
 \endinternal

________________________________________________________________________________________
\version	01.04.04.02 07.05.2012
 \internal
 - Bugfixe in incomingreturngoods. ok button not enabled when pharmaform empty, but articles existing without pharmaform
 \endinternal

________________________________________________________________________________________
\version	01.04.04.01 23.04.2012
 \internal
 - Bugfixes in den Dialogen NA14 BTM-Ausgabebeleg und NA15 Manuelle Erfassung / Bearbeitung von Ausgabebelegen durchgeführt.
 \endinternal

________________________________________________________________________________________
\version	01.04.04.00 04.04.2012
 \internal
 - Druck der tourid aus dem KSC auf die Empfangsbestätigung implementiert.
 - Anpassung der postscript-Dateien der digitalen Abgabebelege (Löschen der Testdaten).
 \endinternal

________________________________________________________________________________________
\version	01.04.03.00 30.03.2012
 \internal
 - Anpassung des Vernichtungsbelegs an neue BfArM-Anforderungen.
 - div. Bugfixes
 \endinternal

________________________________________________________________________________________
\version	01.04.02.01 05.03.2012
 \internal
 - Überprüfung in createprocessnarcoticbooking auf darreichform entfernt (savingAllowed()). Artikel ohne Darreichform konnten so nocht gespeichert werden.
 \endinternal

________________________________________________________________________________________
\version	01.04.02.00 20.02.2012
 \internal
 - Umstellung auf elektronische Uebertragung der Abgabemeldungen zur BfArM
 - Der printhread wird durch anklicken eines buttons gestartet, da nicht für alle benutzer der druck benötigt wird (CCT,  kontrolleuer,..)
 \endinternal

________________________________________________________________________________________
\version	01.04.01.00 26.01.2012
 \internal
 - Umstellung auf elektronische Uebertragung der Abgabemeldungen zur BfArM
 - div. Bugfixes
 - neue Formulare
 - leere Meldungen koennen ueber einen button aufgerufen werden. pdfs muessen in print liegen. auf selber ebene wie exe
 - es kann bei wareneingaengen auf bestellebene das datum geaendert werden
 \endinternal

________________________________________________________________________________________
\version	01.03.01.01 XX.XX.2011
 \internal
 - Beim Drucken Vergleich der Kundennummer (falls Auftragsnummer fälschlicherweise noch auf 0 steht)!
 \endinternal

________________________________________________________________________________________
\version	01.04.00.00 20.12.2011
 \internal
 - Umstellung auf elektronische Uebertragung der Abgabemeldungen zur BfArM
 - div. Bugfixes
 - neue Formulare
 \endinternal

________________________________________________________________________________________
\version	01.03.01.00 21.06.2011
 \internal
 - Scannen von Abgabebelegnummern eingebaut
 - Registrierung fuer Lieferscheindoppel entfernt (Maske zu gross fuer Bildschirm Hr. Hans
 \endinternal

________________________________________________________________________________________
\version	01.03.00.01 20.04.2011
 \internal
 - Verschiedene Prozessanpassungen für elektonischen Abgabebeleg.
 \endinternal

________________________________________________________________________________________
\version	01.03.00.00 17.02.2011 - 17:35
 \internal
 - merge between trunk and servicepack executed successfully
 \endinternal

________________________________________________________________________________________
\version	01.02.00.00 07.05.2010 - 16:17
 \internal
 - changes in dialog NA01! Change the data from the the dialog NA01
 - changes in dialog NA03! Change the data from the the dialog NA03
 - changes in dialog NA06! Change the data from the the dialog NA06
 - changes in dialog NA08! Change the data from the the dialog NA08
 - new dialog NA12! Order Overview
 - new dialog NA13! ResultSet Overview
 - new dialog NA14! Record Edit Contribution Voucher
 - new dialog NA15! Narcotics Contribution Voucher
 - upgrade ssuk to 01.08.00.00
 \endinternal

________________________________________________________________________________________
\version	01.01.01.00 30.12.2008 - 13:22
 \internal
 - change the dialog NA01! Change the data from the the dialog NA08
 \endinternal

________________________________________________________________________________________
\version	01.01.00.03 12.11.2008 - 13:22
 \internal
 - Go with the new user admin!
 \endinternal

________________________________________________________________________________________
\version	01.01.00.02 24.10.2008 - 17:38
 \internal
 - Error elimination for migration!
 \endinternal

________________________________________________________________________________________
\version	01.01.00.01 23.10.2008
 \internal
 - Upgrade on basar lib 01.05.01.00 - Return values.
 \endinternal

________________________________________________________________________________________
\version	01.01.00.00 22.10.2008
 \internal
 - Upgrade on basar lib 01.05.01.00 - migration background.
 \endinternal

________________________________________________________________________________________
\version	01.00.07.05 09.06.2008
 \internal
 - the print data for the desolve receipt are allow only the current date. The date must come from
 - 	from the current booking item.
 \endinternal

________________________________________________________________________________________
\version	01.00.07.04 08.03.2008
 \internal
 - internal change in file 'narcoticsgroupmodule.cpp'::NarcoticsGroupDM::findStockLocationByArticleNo()
 - if a article is not longer listed as a narcotics article.
 \endinternal

________________________________________________________________________________________
 \version	01.00.07.03 24.05.2007
 \internal
 - internal change in file 'narcoticsmodule.cpp::'::NarcoticsDM::updateDiffQtyInStock()
 - Transaction is closed after deleting (all branch specific differences),
 - so that no key value is locked for the new inserts (new differences in stock).
 - Field BGANR_EK of table filiale.
 \endinternal

________________________________________________________________________________________
 \version	01.00.07.01 08.03.2007
 \internal
 - internal change in file 'createprocessnarcoticsbooking.cpp':
					combobox is initialized with the right index.
 \endinternal

 */


 /*
 Here is the template for a version description.
 Copy this section to the top of the version history to create a new version history entry.

________________________________________________________________________________________
\version XX.XX.XX.XX
\date XXXX-XX-XX
\internal
\endinternal
\~german
\~english
\~

 */

