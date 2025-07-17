 ###############################################################################################
 # HEADER-DATA 
 ###############################################################################################
 #Author:			t.hoerath
 #Creation-Date:	20.08.2013
 #Purpose:			starts reorg of narcotic tables
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
 # INDIVIDUAL VARIABLES
 #--------------------------------------------------------------------#
 #--- begin declaration procedure variables ---#
 branchno=0;
 date_older_safekeeping_period=0;
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
 readonly DATE_OLDER_SAFEKEEPING_PERIOD_OPTION="-date_older_safekeeping_period";

 ###############################################################################################
 # F U N C T I O N S
 ###############################################################################################
 #====================================#
 # PRINT START-/ STOP-MELDUNG
 #====================================#
 function printStartStop
 {
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
	 
	 _LOG_FILE=${PROCNAME}.${_branchno}.log
	
 	 echo `log "O" "$_branchno" "================================================" "${LOG_PATH}/${_LOG_FILE}"`
 	 echo `log "O" "$_branchno" "'$1'" "${LOG_PATH}/${_LOG_FILE}"`
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

 		 if [[ "$1" == "$DATE_OLDER_SAFEKEEPING_PERIOD_OPTION" ]]; then
 			 shift
 			 if (( $# > 0 )) ; then
			 	 date_older_safekeeping_period=$1;
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
 
	 if [[ "$date_older_safekeeping_period" == "" ]] || (( 1 != `check_numeric_parameter "${DATE_OLDER_SAFEKEEPING_PERIOD_OPTION}" "$date_older_safekeeping_period"` )) || [[ "$date_older_safekeeping_period" == "0" ]]; then
 		 printStartStop "$STARTMSG"
 		 rc=12;
 		 echo "$PROCNAME: Parameter value missed or invalid value \$date_older_safekeeping_period: >$date_older_safekeeping_period<!" | tee -a $LOG_RESOURCE
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
 ###############################################################################################
 # S P E C I A L I Z E D - L O G I C - F U N C T I O N S
 ###############################################################################################
 #======================================================#
 # Execution of SQL
 #======================================================#
 function doSql
 {
	typeset -i _rc=0;
	local _branchno=$1;
	
	_LOG_FILE=${PROCNAME}.${_branchno}.log
	_SQL_FILE=${PROCNAME}.${_branchno}.sql
	local DB="`get_ksc_db ${_branchno}`";

	echo "${SQL}" >${_SQL_FILE}
	echo "${SQL}" >>${LOG_PATH}/${_LOG_FILE}
	
	dbaccess ${DB} ${_SQL_FILE} 1>>${LOG_PATH}/${_LOG_FILE} 2>>${LOG_PATH}/${_LOG_FILE}
	
	 _rc=${?};(( $rc != 0 )) && exit $rc;

	rm ${_SQL_FILE};
	return ${_rc};
 }
 #==========================================================================================#
 # check if given date is in format YYYYMMDD and at least 3 years in the past !
 #==========================================================================================#
 function evaluate_deletion_date
 {
	typeset -i _date_to_check="$1";
	typeset -i _rc=0;
	
	#----------------------------------------------------------------------------#
	MSG="checking deletion date ${_date_to_check} ...";
	logging "$MSG";
	#----------------------------------------------------------------------------#

	if [ "${_date_to_check}" -lt 20000000 ] || [ "${_date_to_check}" -gt 21000000 ]
	then
		MSG="deletion date ${date_to_check} not in format YYYYMMDD.";
		logging "$MSG";
		_rc=1;
	else
		typeset -i _date_current=`date +"%Y%m%d"`;
		
		((_date_to_check+=30000)); # 3 Jahre addieren
		
		if [ "${_date_to_check}" -gt "${_date_current}" ] 
		then
			MSG="deletion date must be at least 3 years in the past!";
			logging "$MSG";
			_rc=1;
		else
		#----------------------------------------------------------------------------#
		MSG="deletion date OK." ;
		logging "$MSG";
		#----------------------------------------------------------------------------#
		fi	
	fi
	
	return $_rc;
 }
 #============================================================================#
 # Identify transactions from narctransactioncatalog which must not be deleted !
 #============================================================================#
 function identify_transactions_forbidden_to_delete
 { 
	typeset -i _rc=0;
	typeset -i _branchno=$1;
	typeset -i _date_deletion=$2;

	#----------------------------------------------------------------------------#
	MSG="Identifying catalog transactions which must not be deleted for branch: ${branchno} ...";
	logging "$MSG";
	#----------------------------------------------------------------------------#

	# für jede PZN die letzte Buchung VOR dem Löschzeitraum behalten, da diese den Anfangsbestand für die HJM repräsentiert
	SQL="${SQL} 
		select articleno, max(catalogid) as catid, transactiondate from narctransactioncatalog c 
			where branchno = ${_branchno} 
				and (transactiondate*1000000+transactiontime) = 
				(select max (transactiondate*1000000 + transactiontime) from narctransactioncatalog 
				where articleno = c.articleno and branchno = c.branchno and deletedflag = '0' AND processedflag = '1' and transactiondate < ${_date_deletion})
			group by 1, 3 into temp tmptab_narcreorg_3 with no log;
		create unique index i_narcreorg_3 on tmptab_narcreorg_3 (catid);
		create unique index i_narcreorg_33 on tmptab_narcreorg_3 (articleno);"
	
	# ??? Komplettes Löschen von "alten" Artikeln mit Bestand = 0; wenn nein, dann dieses Statement NICHT ausführen ???
	#SQL="${SQL} 
	#	delete from tmptab_narcreorg_3 where catid in 
	#		(select catalogid from narctransactioncatalog where branchno = ${_branchno} and  qtyinstock = 0); "
	
	# neueste Original- ID der "alten" Artikel finden
	SQL="${SQL} 
		select articleno, max(originalid) as origid from narctransactionoriginal where branchno = ${_branchno}
			and articleno in (select articleno from tmptab_narcreorg_3)
				and transactiondate = (select transactiondate from tmptab_narcreorg_3 where articleno = narctransactionoriginal.articleno)
				group by 1 into temp tmptab_narcreorg_4 with no log; 
		create unique index i_narcreorg_4 on tmptab_narcreorg_4 (origid);
		create unique index i_narcreorg_44 on tmptab_narcreorg_4 (articleno); "
		
	return $_rc;
 }
 #======================================================#
 # unloading / deleting narctransactioncatalog
 #======================================================#
 function unload_delete_catalog
 { 
	typeset -i _rc=0;
	typeset -i _branchno=$1;
	typeset -i _date_deletion=$2;
	
	#----------------------------------------------------------------------------#
	MSG="unloading / deleting narctransactioncatalog for branch: ${branchno} ...";
	logging "$MSG";
	#----------------------------------------------------------------------------#

	typeset    _where_stmt=" from narctransactioncatalog where branchno = ${_branchno} and transactiondate < ${_date_deletion} 
								and catalogid not in (select catid from tmptab_narcreorg_3) "
	
	# vorher Daten sichern!
	typeset _timestmp_current=`date +"%Y%m%d-%H%M%S"`;
	typeset -r _unload_file="${DATA_PATH}/reorg_catalog_${_branchno}_${_date_deletion}_${_timestmp_current}.txt";
	
	SQL="${SQL} 
		unload to '${_unload_file}' select * ${_where_stmt} order by articleno, transactiondate, transactiontime;"
	
	# Löschen aller Transaktionen kleiner Stichtag (Ausnahme: jeweils neuester Satz der "alten" Artikel)
	SQL="${SQL} 
		delete ${_where_stmt};"

	return $_rc;
 }
 #======================================================#
 # unloading / deleting narctransactionoriginal
 #======================================================#
 function unload_delete_original
 { 
	typeset -i _rc=0;
	local _branchno=$1;
	typeset -i _date_deletion=$2;
	
	#----------------------------------------------------------------------------#
	MSG="unloading / deleting narctransactionoriginal for branch: ${branchno} ...";
	logging "$MSG";
	#----------------------------------------------------------------------------#

	typeset    _where_stmt=" from narctransactionoriginal where branchno = ${_branchno} and transactiondate < ${_date_deletion} 
								and originalid not in (select origid from tmptab_narcreorg_4)  "
	
	# vorher Daten sichern!
	typeset _timestmp_current=`date +"%Y%m%d-%H%M%S"`;
	typeset -r _unload_file="${DATA_PATH}/reorg_original_${_branchno}_${_date_deletion}_${_timestmp_current}.txt";
	
	SQL="${SQL} 
		unload to '${_unload_file}' select * ${_where_stmt} order by articleno, transactiondate, transactiontime;"

	# Löschen aller Transaktionen kleiner Stichtag (Ausnahme: jeweils neuester Satz der "alten" Artikel)
	SQL="${SQL} 
		delete ${_where_stmt};"

	doSql "${_branchno}";
	_rc=${?}; (( $rc != 0 )) && exit $rc;
	
	return $_rc;
 }
 #======================================================#
 # unloading / deleting narcopenorders
 #======================================================#
 function unload_delete_openorders
 { 
	typeset -i _rc=0;
	local _branchno=$1;
	typeset -i _date_deletion=$2;
	
	#----------------------------------------------------------------------------#
	MSG="unloading / deleting narcopenorders for branch: ${branchno} ...";
	logging "$MSG";
	#----------------------------------------------------------------------------#

	typeset    _where_stmt="  from narcopenorders where branchno = ${_branchno} and orderdate < ${_date_deletion} "
	
	# vorher Daten sichern!
	typeset _timestmp_current=`date +"%Y%m%d-%H%M%S"`;
	typeset -r _unload_file="${DATA_PATH}/reorg_openorders_${_branchno}_${_date_deletion}_${_timestmp_current}.txt";
	
	SQL="unload to '${_unload_file}' select * ${_where_stmt} order by orderdate;"

	# Löschen aller offener Bestellungen zum Halbjahres- Stichtag
	SQL="${SQL} 
		delete ${_where_stmt};"

	doSql "${_branchno}";
	_rc=${?}; (( $rc != 0 )) && exit $rc;

	#----------------------------------------------------------------------------#
	MSG="narcopenorders unloaded / deleted.";
	logging "$MSG";
	#----------------------------------------------------------------------------#
	
	return $_rc;
 }
 #======================================================#
 # unloading / deleting narcprotocl
 #======================================================#
 function unload_delete_protocol
 { 
	typeset -i _rc=0;
	local _branchno=$1;
	typeset -i _date_deletion=$2;
	
	#----------------------------------------------------------------------------#
	MSG="unloading / deleting narcprotocl for branch: ${branchno} ...";
	logging "$MSG";
	#----------------------------------------------------------------------------#

	typeset    _where_stmt=" from NARCPROTOCOL where catalogid not in (select catalogid from narctransactioncatalog) "
	
	# vorher Daten sichern!
	typeset _timestmp_current=`date +"%Y%m%d-%H%M%S"`;
	typeset -r _unload_file="${DATA_PATH}/reorg_protocol_${_branchno}_${_date_deletion}_${_timestmp_current}.txt";
	
	SQL="unload to '${_unload_file}' select * ${_where_stmt} order by catalogid;"

	# Löschen aller Protokolle ohne zugehörigen catalog- Eintrag
	SQL="${SQL} 
		delete ${_where_stmt};"

	doSql "${_branchno}";
	_rc=${?}; (( $rc != 0 )) && exit $rc;

	#----------------------------------------------------------------------------#
	MSG="narcprotocl unloaded / deleted.";
	logging "$MSG";
	#----------------------------------------------------------------------------#
	
	return $_rc;
 }
 
 ###############################################################################################
 # H E L P & V E R S I O N 
 ###############################################################################################
 . $(dirname $0)/narcreorg.option
 
 ###############################################################################################
 # S H E L L - L O G I C 
 ###############################################################################################
 check_parameters "$@";
 eval_parameters "$@";
 print_msg "$STARTMSG" "$*"
 
 #======================================================#
 # CALLS METHOD START-PROCESS
 #======================================================#
 if [[ "$start" == "YES" ]]; then

 	evaluate_deletion_date "${date_older_safekeeping_period}";
	rc=$?; (( $rc != 0 )) && print_msg "$STOPMSG" "$*" "<$rc>" && exit $rc;

	identify_transactions_forbidden_to_delete "${branchno}" "${date_older_safekeeping_period}";
	rc=$?; (( $rc != 0 )) && print_msg "$STOPMSG" "$*" "<$rc>" && exit $rc;

	unload_delete_catalog "${branchno}" "${date_older_safekeeping_period}";
	rc=$?; (( $rc != 0 )) && print_msg "$STOPMSG" "$*" "<$rc>" && exit $rc;
	
	unload_delete_original "${branchno}" "${date_older_safekeeping_period}";
	rc=$?; (( $rc != 0 )) && print_msg "$STOPMSG" "$*" "<$rc>" && exit $rc;
	
	#----------------------------------------------------------------------------#
	# all SQL- statements above have to be processed in one single dbaccess session,
	# otherwise temp tables don´t live anymore when they are needed !
	MSG="narctransactioncatalog + narctransactionoriginal unloaded / deleted.";
	logging "$MSG";
	#----------------------------------------------------------------------------#
		
	unload_delete_openorders "${branchno}" "${date_older_safekeeping_period}";
	rc=$?; (( $rc != 0 )) && print_msg "$STOPMSG" "$*" "<$rc>" && exit $rc;

	unload_delete_protocol "${branchno}" "${date_older_safekeeping_period}";
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
