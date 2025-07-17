#! /usr/bin/ksh
 
 ###############################################################################################
 # HEADER-DATA 
 ###############################################################################################
 #Author:			b.bischof 
 #Creation-Date:	22.10.2008
 #Creation-Time:	23:55
 #Purpose:			starts, stops and status the exemplar binary diffqtyinstock
 ###############################################################################################
 
 ###############################################################################################
 # set options
 ###############################################################################################
 #set -a #automatischer Export aller neu angelegten oder veraenderten Variablen (export all);
 #set -e #beendet die Shell, wenn ein Befehl nicht 0 zurueckgibt (exit on error);
 #set -h #sucht und merkt sich die Lage von Kommandos innerhalb von Funktionen (hash function commands)
 #set -n #lese und teste auf syntaktische Korrektheit; fuehre nicht aus (no execution);
 #set -u #gebe eine Fehlermeldung aus bei der Verwendung von undefinierten Variablen, anstatt eine leere Zeichenkette einzusetzen (unset);
 #set -v #zeigt jede Zeile vor ihrer Ausfuehrung unveraendert an (verbose);
 #set -x #zeigt jede Zeile vor ihrer Ausfuehrung jedoch nach ihren Ersetzungen an (xtrace);
 

 ###############################################################################################
 # VARIABLES
 ###############################################################################################
 #--------------------------------------------------------------------#
 # READ IN GLOBAL VARIABLES
 #--------------------------------------------------------------------#
 . $(dirname $0)/../vars/narcotics_global.vars

 #--------------------------------------------------------------------#
 # LIBRARY / BINARY VARIABLES
 #--------------------------------------------------------------------#
 # INFORMIXDIR umbiegen von client sdk directory (csdk) auf Hauptverzeichnis
 # (nötig zur Verwendung von dbaccess)
 export INFORMIXDIR=$(dirname $(dirname $(which dbaccess)))
 readonly BINARY=diffqtyinstock.bin
 #process="${BIN_PATH}/${BINARY}"
 

 #--------------------------------------------------------------------#
 # INDIVIDUAL VARIABLES
 #--------------------------------------------------------------------#
 #--- begin declaration procedure variables ---#
 branchno=0;
 date_after_midnight=0;
 #--- end declaration procedure variables ---#
 start="";
 stop="";
 status="";
 process_count=0; # for checking the number of process starts
 typeset -i rc=0;
 
 #--------------------------------------------------------------------#
 # L I T E R A L S --> ATTENTION: INDIVIDUAL MODIFICATION REQUIRED <--
 #--------------------------------------------------------------------#
 readonly STARTMSG="START";
 readonly STOPMSG="STOP";
 readonly START_OPTION="-start";
 readonly STOP_OPTION="-stop";
 readonly STATUS_OPTION="-status";
 readonly BRANCHNO_OPTION="-branchno";
 readonly DATE_AFTER_MIDNIGHT_OPTION="-date_after_midnight";

 ###############################################################################################
 # F U N C T I O N S
 ###############################################################################################
 #====================================#
 # PRINT START-/ STOP-MELDUNG
 #====================================#
 function printStartStop
 {
 	 if [[ "$2" == "" ]]; then set $1 "22.10.08 23:55:11"; fi
 	 echo "=====================================================================================" | tee -a $LOG_RESOURCE
 	 echo "SH: $1 `pwd`/$PROCNAME $2" | tee -a $LOG_RESOURCE
 	 echo "=====================================================================================" | tee -a $LOG_RESOURCE
 }
 #====================================#
 # LOG START-/ STOP-MELDUNG
 #====================================#
 function logging
 {
 	 local _branchno=0;
 	 if [[ "${branchno}" != "" ]]; then
 		 _branchno=${branchno};
 	 fi
 	 echo `log "O" "$_branchno" "================================================" "${LOG_RESOURCE}"`
 	 echo `log "O" "$_branchno" "$1" "${LOG_RESOURCE}"`
 	 echo `log "O" "$_branchno" "================================================" "${LOG_RESOURCE}"`
 }
 #======================================================#
 # logging start - printing on console
 #======================================================#
 function print_msg
 {
 	 if [[ "$1" == "$STARTMSG" ]]; then 
 		 printStartStop "$1" "$2" 
 		 logging "$1"; 
 	 else 
 		 local readonly MSG="$1 with Return-Code: <${3}>."
 		 logging "${MSG}";
 		 printStartStop "$1" "$2"
 	 fi
 }
 #====================================#
 # PRINT LOCAL VARIABLES --> ATTENTION: INDIVIDUAL MODIFICATION REQUIRED (new variables) <-- 
 #====================================#
 function print_local_var
 {
 	 local FUNCNAME="print_local_var";
 	 echo "--------------------------"
 	 echo "ENTER $FUNCNAME"
 	 echo "--------------------------"
 	 echo "\$MARATHON_SSUK_VERSION:		>$MARATHON_SSUK_VERSION<"
 	 echo "\$MARATHON_SSUK_LIBRARY_PATH:	>$MARATHON_SSUK_LIBRARY_PATH<"
 	 echo "\$LD_LIBRARY_PATH:		>$LD_LIBRARY_PATH<"
 	 echo "\$BINARY:			>$BINARY<"
 	 echo "\$STARTMSG:			>$STARTMSG<"
 	 echo "\$STOPMSG:			>$STOPMSG<"
 	 echo "\$START_OPTION:			>$START_OPTION<"
 	 echo "\$STOP_OPTION:			>$STOP_OPTION<"
 	 echo "\$STATUS_OPTION:			>$STATUS_OPTION<"
	 echo "\$BRANCHNO_OPTION:		>$BRANCHNO_OPTION<"
	 echo "\$DATE_AFTER_MIDNIGHT_OPTION:	>$DATE_AFTER_MIDNIGHT_OPTION<"

 	 echo "\$para:				>$para<"
 	 echo "\$process:			>$process<"
 	 echo "\$start:				>$start<"
 	 echo "\$stop:				>$stop<"
 	 echo "\$status:			>$status<"
	 echo "\$branchno:			>$branchno<"
	 echo "\$date_after_midnight:		>$date_after_midnight<"

	 echo "";

 	 echo "--------------------------"
 	 echo "EXIT $FUNCNAME"
 	 echo "--------------------------"
 }
 #=============================================#
 # CHECKING IF BINARY is EXISTING 
 #=============================================#
 function check_existence_of_binary
 {
 	 if [[ ! -f ${BIN_PATH}/${BINARY} ]]; then
 		 printStartStop "$STARTMSG"
 		 rc=8;
 		 echo "Error: Binary \"${BIN_PATH}/${BINARY}\" does not exist!" | tee -a $LOG_RESOURCE
 		 printStartStop "$STOPMSG" "with Exit-Code: <${rc}>"
 		 exit ${rc};
 	 fi
}
 #=============================================#
 # CHECKING IF PARAMETERS ARE BASICALLY SET 
 #=============================================#
 function check_parameters
 {
 	 if (( $# == 0 )); then
 		 printStartStop "$STARTMSG"
 		 rc=9;
 		 echo "Error: No Parameters passed!" | tee -a $LOG_RESOURCE
 		 echo ${USAGE} | tee -a $LOG_RESOURCE
 		 printStartStop "$STOPMSG" "with Exit-Code: <${rc}>"
 		 exit ${rc};
 	 fi
 }
 #=============================================#
 # READ IN PARAMETERS DISREGARDING THE ORDER --> ATTENTION: INDIVIDUAL MODIFICATION REQUIRED <-- 
 #=============================================#
 function eval_parameters
 {
 	 while (( $# > 0 )) 
 	 do
 		 #read in option/prz/dbzpps1de
 		 if [[ "$1" == "$START_OPTION" ]] ; then start="YES"; shift; continue; fi
 		 if [[ "$1" == "$STOP_OPTION" ]] ; then stop="YES"; shift; continue; fi
 		 if [[ "$1" == "$STATUS_OPTION" ]] ; then status="YES"; shift; continue; fi

 		 #read in parameters
 		 if [[ "$1" == "$BRANCHNO_OPTION" ]]; then
 			 shift
 			 if (( $# > 0 )) ; then
 				 branchno=$1;
 				 shift;
 				 continue;
 			 else 
 				 break;
 			 fi
 		 fi

 		 if [[ "$1" == "$DATE_AFTER_MIDNIGHT_OPTION" ]]; then
 			 shift
 			 if (( $# > 0 )) ; then
 				 date_after_midnight=$1;
 				 shift;
 				 continue;
 			 else 
 				 break;
 			 fi
 		 fi

		break;
 	 done

 	 #--------------------------#
 	 # evaluation of options
 	 #--------------------------#
 	 if [ ! "$start$stop$status" ] ; then 
 		 printStartStop "$STARTMSG"
 		 rc=10;
 		 echo "$PROCNAME: No options set in command line!" | tee -a $LOG_RESOURCE
 		 echo ${USAGE} | tee -a $LOG_RESOURCE
 		 printStartStop "$STOPMSG" "with Exit-Code: <${rc}>"
 		 exit ${rc};
 	 else 
 		 typeset -i options_counter=0;
 		 if [[ "$start" == "YES" ]] ; then
 			 options_counter=$options_counter+1
 		 fi
 		 if [[ "$stop" == "YES" ]] ; then
 			 options_counter=$options_counter+1
 		 fi
 		 if [[ "$status" == "YES" ]] ; then
 			 options_counter=$options_counter+1
 		 fi
 	 fi
 	 if (( $options_counter > 1 )) ; then
 		 printStartStop "$STARTMSG"
 		 rc=11;
 		 echo "$PROCNAME: More than one Option is set in command Line!" | tee -a $LOG_RESOURCE
 		 echo ${USAGE} | tee -a $LOG_RESOURCE
 		 printStartStop "$STOPMSG" "with Exit-Code: <${rc}>"
 		 exit ${rc};
 	 fi

	 #--------------------------#
 	 # evaluation of parameters
 	 #--------------------------#
 	 if [[ "$branchno" == "" ]] || (( 1 != `check_numeric_parameter "${BRANCHNO_OPTION}" "$branchno"` )) || [[ "$branchno" == "0" ]]; then
 		 printStartStop "$STARTMSG"
 		 rc=12;
 		 echo "$PROCNAME: Parameter value missed or invalid value \$branchno: >$branchno<!" | tee -a $LOG_RESOURCE
 		 echo ${USAGE} | tee -a $LOG_RESOURCE
 		 printStartStop "$STOPMSG" "with Exit-Code: <${rc}>"
 		 exit ${rc};
 	 fi
 

 	 if [[ "$date_after_midnight" == "" ]] || (( 1 != `check_numeric_parameter "${DATE_AFTER_MIDNIGHT_OPTION}" "$date_after_midnight"` )) || [[ "$date_after_midnight" == "0" ]]; then
 		 printStartStop "$STARTMSG"
 		 rc=12;
 		 echo "$PROCNAME: Parameter value missed or invalid value \$date_after_midnight: >$date_after_midnight<!" | tee -a $LOG_RESOURCE
 		 echo ${USAGE} | tee -a $LOG_RESOURCE
 		 printStartStop "$STOPMSG" "with Exit-Code: <${rc}>"
 		 exit ${rc};
 	 fi
 

 }
 #======================================================#
 # checks numeric parameter --> ATTENTION: INDIVIDUAL MODIFICATION REQUIRED (separate validity) <-- 
 #======================================================#
 function check_numeric_parameter
 {
 	 local readonly FUNCNAME=check_numeric_parameter;
 	 local _rc=0;

 	 local readonly _OPTION=${1};
 	 local readonly _ARG=${2};

 	 is_numeric "${_ARG}" "-NO_VERBOSE" # do the checks considered to be necessary
 	 if (( $? == 0 )); then return ${_rc}; fi
 	 _rc=1;

 	 if [[ "${_OPTION}" == "${BRANCHNO_OPTION}" ]]; then
 		 # do the possibly necessary checks with parameter ${_ARG}
 		 _rc=1;
 	 fi

 	 echo ${_rc}
 	 return ${_rc}
 }
 #======================================================#
 # checks string parameter --> ATTENTION: INDIVIDUAL MODIFICATION REQUIRED (separate validity) <-- 
 #======================================================#
 function check_string_parameter
 {
 	 local readonly FUNCNAME=check_string_parameter;
 	 local _rc=0;

 	 # do some checks here - e.g. database server validity: [a-z]*([a-z]|[1-9])@+([a-z]|[1-9]|[_])_@(shm|tcp)

 	 _rc=1
 	 echo ${_rc}
 	 return ${_rc}
 }
 #======================================================#
 # set parameters with new values of the command line --> ATTENTION: INDIVIDUAL MODIFICATION REQUIRED ([optional] parameters) <-- 
 #======================================================#
 function assembling_process
 {
	 local _parameters="";
 	 if (( ${#_parameters} > 0 )); then
 		 _parameters="${_parameters%?}"
 		 _parameters="${_parameters#?}"
 	 fi

 	 if (( ${#_parameters} > 0 )); then
 		 _parameters="${_parameters} $BRANCHNO_OPTION $branchno $DATE_AFTER_MIDNIGHT_OPTION $date_after_midnight"
 	 else
 		 _parameters="$BRANCHNO_OPTION $branchno $DATE_AFTER_MIDNIGHT_OPTION $date_after_midnight";
 	 fi

 	 if (( ${#_parameters} > 0 )); then
 		 process="$process ${_parameters}"
 	 fi

 }
 ###############################################################################################
 # S P E C I A L I Z E D - L O G I C - F U N C T I O N S
 ###############################################################################################
 #======================================================#
 # Execution of SQL
 #======================================================#
 function doSql
 {
	typeset -i _branchno=$1;
	_LOG_FILE=${PROCNAME}.${_branchno}.log
	_SQL_FILE=${PROCNAME}.${_branchno}.sql
	local DB="`get_ksc_db ${_branchno}`";

	echo $SQL >${_SQL_FILE}
	logging ">>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>"
	logging "$MSG"
	logging "$SQL"
	dbaccess ${DB} ${_SQL_FILE} 1>>${LOG_PATH}/${_LOG_FILE} 2>>${LOG_PATH}/${_LOG_FILE}
	logging "<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<"

	rm ${_SQL_FILE};
 }
 #======================================================#
 # Finding out stock discrepancy of table 'artikellokal'
 #======================================================#
 function evaluate_stock_reconciliation
 {

	typeset -i _branchno=$1;
	typeset -i _date_after_midnight=$2;

	#----------------------------------------------------------------------------#
	MSG="START: Protokollierung Bestandsabweichung?"; echo $MSG
	#----------------------------------------------------------------------------#

	#---------------------------------------------------------------------------------------------#
	# Protokollierung: 
	# 'artikellokal:bestandtagesanfang' != 'artikellokal:bestand' und 'artikellokal:bestandreserviert'
	#---------------------------------------------------------------------------------------------#
	DIFF_FILE_TEMP=${PROCNAME}.${_branchno}.diff.temp

	SQL="
UNLOAD TO ${DATA_PATH}/${DIFF_FILE_TEMP} 
	SELECT ${_date_after_midnight}, al.filialnr, al.artikel_nr, al.bestand, NVL(SUM(r.reservedqty), 0), al.bestandtagesanfang,  
	FROM artikellokal as al
		INNER JOIN artikelzentral as az ON al.artikel_nr = az.artikel_nr
		LEFT OUTER JOIN cststockreserved r ON al.artikel_nr = r.articleno AND al.filialnr = r.branchno
	WHERE 
		bitand(az.etartklasse1, 512) = 512 AND
		al.filialnr = ${_branchno} 
	GROUP BY 1,2,3,4,6
	HAVING (al.bestand + NVL(SUM(r.reservedqty), 0)) != al.bestandtagesanfang"

	doSql ${_branchno};

	# Nur existierende Bestands-Abweichungen werde umgelenkt und protokolliert.
	if [[ -f ${DATA_PATH}/${DIFF_FILE_TEMP} ]]; then
		ATTRIBUTES="Datum, VZ, ArtNr, Bestand, BestandReserviert, BestandTagesanfang"
		ATT_FILE=${PROCNAME}.att
		echo ${ATTRIBUTES} > ${DATA_PATH}/${ATT_FILE};

		DIFF_FILE=${PROCNAME}.${_branchno}.diff
		cat ${DATA_PATH}/${ATT_FILE} ${DATA_PATH}/${DIFF_FILE_TEMP} > ${DATA_PATH}/${DIFF_FILE};

		rm ${DATA_PATH}/${ATT_FILE};       2>/dev/null
		rm ${DATA_PATH}/${DIFF_FILE_TEMP}; 2>/dev/null

		MSG="Es existieren Bestandsabweichungen innerhalb der Tabelle 'artikellokal' ";
		MSG=${MSG}"'(bestand + bestandreserviert) != al.bestandtagesanfang. ";
		MSG=${MSG}"Protokolliert in der Datei '${DATA_PATH}/${DIFF_FILE}'!";
		logging "${MSG}"
	fi

	#----------------------------------------------------------------------------#
	MSG="ENDE: Protokollierung Bestandsabweichung?" ; echo $MSG
	#----------------------------------------------------------------------------#

	return 0;
 }
 #======================================================#
 # Must the programm start   
 #======================================================#
 function is_start_required
 { 
	#----------------------------------------------------------------------------#
	MSG="START: Programm-Lauf erforderlich?"; echo $MSG
	#----------------------------------------------------------------------------#
	typeset -i _rc=0;
	typeset -i _branchno=$1;
	typeset -i _date_after_midnight=$2;

	local _RES_FILE=${DATA_PATH}/${PROCNAME}.${_branchno}.res
	SQL="
UNLOAD TO ${_RES_FILE} 
	SELECT count(*) FROM narcdiffqtyinstock WHERE branchno = ${_branchno} AND creationdate = ${_date_after_midnight}"
	doSql ${_branchno};

	COUNT=999;
	COUNT=`cat $_RES_FILE | nawk '{ if( "0.0" == substr($0,1,3) ){ print "0" } else { print "1" }; }'`;

	if [ "$COUNT" -eq 1 ]
	then
		MSG="Treffer gefunden fuer folgende Parameter VZ: ${_branchno} und Datum: ${_date_after_midnight}! ";
		MSG=${MSG}"--> Kein Lauf erforderlich!"; echo $MSG;
		#print_msg "$STOPMSG" "$*" "<$rc>"
		_rc=1;
	fi
	rm ${_RES_FILE};
	
	#----------------------------------------------------------------------------#
	MSG="ENDE: Programm-Lauf erforderlich?"; echo $MSG
	#----------------------------------------------------------------------------#
	logging "$MSG"

	return $_rc;
 }
 #======================================================#
 # Deletion of branch specific narc positions
 #======================================================#
 function delete_narc_diff
 { 
	typeset -i _rc=0;
	typeset -i _branchno=$1;

	#----------------------------------------------------------------------------#
	MSG="START - Loeschen der alten Artikel-Differenzdaten fuer das VZ: ${branchno}!"; echo $MSG
	#----------------------------------------------------------------------------#

	SQL="DELETE FROM narcdiffqtyinstock WHERE branchno = ${_branchno}"
	doSql ${_branchno};

	#----------------------------------------------------------------------------#
	MSG="ENDE - Loeschen!"; echo $MSG
	#----------------------------------------------------------------------------#
	logging "$MSG"

	return $_rc;
 }
 #======================================================#
 # Inserting branch specific nwe narc positions
 #======================================================#
 function insert_narc_diff
 { 
	typeset -i _rc=0;
	typeset -i _branchno=$1;
	typeset -i _date_after_midnight=$2;

	#----------------------------------------------------------------------------#
	MSG="START - Eintragen der Bestands-Differenzen fuer VZ: ${branchno}!"; echo $MSG
	#----------------------------------------------------------------------------#

	SQL="
CREATE TEMP TABLE tmp_btm
(
  articleno INTEGER DEFAULT 0 NOT NULL,
  catalogid INT8 DEFAULT 0 NOT NULL,
  qtyinstock INTEGER DEFAULT 0 NOT NULL
) WITH NO LOG;

INSERT INTO tmp_btm
	SELECT z.artikel_nr, NVL(c.catalogid, 0), NVL(c.qtyinstock, 0)
	FROM artikelzentral z, outer narctransactioncatalog c
	WHERE c.branchno= ${_branchno} AND 
		z.artikel_nr = c.articleno AND bitand(az.etartklasse1, 512) = 512
		and (c.transactiondate*1000000+c.transactiontime) = 
			(select max (transactiondate*1000000 + transactiontime) from narctransactioncatalog 
			where articleno = c.articleno and branchno = c.branchno and deletedflag = '0' AND processedflag = '1' AND transactiondate < ${_date_after_midnight});

CREATE INDEX i_tmp_btm ON tmp_btm (articleno) USING BTREE;

INSERT INTO narcdiffqtyinstock (branchno, articleno, creationdate, diffqtyinstock)
	SELECT ${_branchno}, articleno, ${_date_after_midnight}, bestandtagesanfang-qtyinstock as diff
	FROM tmp_btm, artikellokal
	WHERE filialnr= ${_branchno} AND qtyinstock <> bestandtagesanfang and articleno=artikel_nr;

DROP TABLE tmp_btm;"

	doSql ${_branchno};

	#----------------------------------------------------------------------------#
	MSG="ENDE - Eintragen der Bestands-Differenzen!"; echo $MSG
	#----------------------------------------------------------------------------#
	logging $MSG

	return $_rc;
 }
 #======================================================#
 # 
 #======================================================#
 function insert_historical_narc_diff
 { 
	typeset -i _rc=0;
	typeset -i _branchno=$1;
	typeset -i _date_after_midnight=$2;

	#----------------------------------------------------------------------------#
	MSG="START - Eintragen der historischen NEGATIV-Bestaenden!"; echo $MSG
	#----------------------------------------------------------------------------#
	SQL="
INSERT INTO narcdiffqtyinstock (branchno, articleno, creationdate, diffqtyinstock)
	SELECT ${_branchno}, articleno, ${_date_after_midnight}, 0
	FROM narctransactioncatalog
	WHERE branchno = ${_branchno} AND
		qtyinstock < 0 AND
		processedflag = '1' AND
		deletedflag = '0' AND
		articleno NOT IN (SELECT articleno FROM narcdiffqtyinstock)
	GROUP BY articleno;"

	doSql ${_branchno};
	
	#----------------------------------------------------------------------------#
	MSG="ENDE- Eintragen der historischen NEGATIV-Bestaenden!"; echo $MSG
	#----------------------------------------------------------------------------#
	logging $MSG

	return $_rc;

 }
 ###############################################################################################
 # H E L P & V E R S I O N 
 ###############################################################################################
 . $(dirname $0)/diffqtyinstock.option
 
 ###############################################################################################
 # S H E L L - L O G I C 
 ###############################################################################################
 check_parameters "$@";
 eval_parameters "$@";
 #check_existence_of_binary;
 #assembling_process;
 print_msg "$STARTMSG" "$*"
 
 #======================================================#
 # CALLS METHOD START-PROCESS
 #======================================================#
 if [[ "$start" == "YES" ]]; then

 	evaluate_stock_reconciliation ${branchno} ${date_after_midnight};

	is_start_required ${branchno} ${date_after_midnight};
	rc=$?; (( $rc != 0 )) && print_msg "$STOPMSG" "$*" "<$rc>" && exit $rc;

	delete_narc_diff ${branchno} 
	rc=$?; (( $rc != 0 )) && print_msg "$STOPMSG" "$*" "<$rc>" && exit 0; #$rc;

	insert_narc_diff ${branchno} ${date_after_midnight};
	rc=$?; (( $rc != 0 )) && print_msg "$STOPMSG" "$*" "<$rc>" && exit $rc;

	insert_historical_narc_diff ${branchno} ${date_after_midnight};
	rc=$?;
 fi

 #======================================================#
 # CALLS METHOD STOP-PROCESS
 #======================================================#
 if [[ "$stop" == "YES" ]]; then
	 msg="STOP Not implemented!";
 	 rc=1;
 	 echo "$msg" | tee -a $LOG_RESOURCE 

 fi

 #======================================================#
 # CALLS METHOD STATUS-PROCESS
 #======================================================#
 if [[ "$status" == "YES" ]]; then
	 msg="STATUS Not implemented!";
 	 rc=1;
 	 echo "$msg" | tee -a $LOG_RESOURCE 

 fi
 
 print_msg "$STOPMSG" "$*" "<$rc>"
 exit $rc;
