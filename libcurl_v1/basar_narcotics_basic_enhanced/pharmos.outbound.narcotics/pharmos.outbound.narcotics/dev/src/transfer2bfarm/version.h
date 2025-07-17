//---------------------------------------------------------------------------------------------------------//
/*! \file
 *  \brief  definition narcotics version
 *  \author Thomas Hörath
 *  \date   10.02.2006
 */
//---------------------------------------------------------------------------------------------------------//

#ifndef GUARD_TRANSFER_BFARM_VERSION_H
#define GUARD_TRANSFER_BFARM_VERSION_H

/*!
\page page2 transfer2bfarm

________________________________________________________________________________________
\version	01.04.00.00
\date		XX.XX.XXXX
\internal
- CPR-240685 Lieferscheindoppel Ordner pro Filiale fortlaufend nummerieren.
- SSUK update auf ssuk-01.13.02.00.
\endinternal

________________________________________________________________________________________
\version	01.03.00.00
\date		06.07.2018
\internal
- SR-17004581 - Zurücksetzen des Übertragungsstatus bei Änderung auf Vernichtung, so dass keine fälschliche Übertragung eines Abgabebelegs mehr passieren kann.
\endinternal

________________________________________________________________________________________
\version	01.02.01.00
\date		17.01.2017
\internal
- SR-17004581 - Zurücksetzen des Übertragungsstatus bei Änderung auf Vernichtung, so dass keine fälschliche Übertragung eines Abgabebelegs mehr passieren kann.
\endinternal
________________________________________________________________________________________
\version	01.02.00.03
\date		18.12.2015
\internal
- SR-15045484: BTM-Lieferscheindoppel nicht im BI / COLD.
  Wenn die Zeitspanne zwischen Abgabebeleg und Lieferscheindoppel zu groß war, wurden die
  Lieferscheindoppel nicht ins BI übertragen.
\endinternal

________________________________________________________________________________________
\version	01.02.00.02
\date		08.09.2014
\internal
- Archivierung Abgabebelege: Vorbereitung für Urladung Belege ins COLD:
  Neuer optionaler Parameter "-contributionvoucherdir" beim Programmstart.
\endinternal

________________________________________________________________________________________
\version	01.02.00.01
\date		01.09.2014
\internal
- I-14007640 BTM-Programm - Reklamation der Halbjahresmeldung (bugfix im crosscheck und verbessertes logging).
\endinternal

________________________________________________________________________________________
\version	01.02.00.00 18.08.2014
\internal
- neue SSUK 01.10.00.00.
- Archivierung Abgabebelege CR.
\endinternal

________________________________________________________________________________________
\version	01.01.00.00
\date		09.04.2014
\internal
- Umbau auf UADM Parameter (ini file obsolet).
- Kein externes Shellskript mehr zur Übertragung zur Bfarm, wird alles im TransferToBfarmUC abgewickelt.
- Parameter geändert, jetzt -crossckeck -transfer (dann gleiches Verhalten wie bisher) -archive (zusätzlich noch Dateistruktur für Cold Archivierung erzeugen).
- Archivierung Abgabebelege CR.
\endinternal

________________________________________________________________________________________
\version	01.00.00.00
\date		29.10.2012
\internal
- Initial Version.
\endinternal
 */


//! major version number of transfer2bfarm
#define VERSION_MAJORNO           "01"		

//! minor version number of transfer2bfarm
#define VERSION_MINORNO           "04"		

//! extension version number of transfer2bfarm
#define VERSION_EXTENSION         "00"		

//! build version number of transfer2bfarm
#define VERSION_BUILDNO           "00"		

//! version number string of transfer2bfarm
#define VERSIONNO				VERSION_MAJORNO "." VERSION_MINORNO "." VERSION_EXTENSION "." VERSION_BUILDNO


#ifdef NDEBUG
//! version number string release of transfer2bfarm
#define VERSION_NUMBER			VERSIONNO" rel"		
#else
//! version number string debug of transfer2bfarm
#define VERSION_NUMBER			VERSIONNO" dbg"		
#endif

//! transfer2bfarm name
#define PROGRAM_NAME			"transfer2bfarm"	

#endif //GUARD_TRANSFER_BFARM_VERSION_H
