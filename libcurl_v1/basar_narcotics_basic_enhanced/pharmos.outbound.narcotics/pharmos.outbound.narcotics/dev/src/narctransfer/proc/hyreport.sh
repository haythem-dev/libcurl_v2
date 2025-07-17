#! /usr/bin/ksh
 
 ###############################################################################################
 # HEADER-DATA 
 ###############################################################################################
 #Author:            b.bischof                                                                                 
 #Creation-Date:     29.12.2008                                                                                
 #Creation-Time:     22:27                                                                                     
 #Purpose:           Starts, stops and status the exemplar binary hyreport for presentation only.              
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
 readonly BINARY=hyreport.bin
 process="${BIN_PATH}/${BINARY}"
 

 #--------------------------------------------------------------------#
 # INDIVIDUAL VARIABLES
 #--------------------------------------------------------------------#
 #--- begin declaration procedure variables ---#
 db="";
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
 readonly DB_OPTION="-db";

 ###############################################################################################
 # F U N C T I O N S
 ###############################################################################################
 #====================================#
 # PRINT START-/ STOP-MELDUNG
 #====================================#
 function printStartStop
 {
 	 if [[ "$2" == "" ]]; then set $1 `date +"%d.%m.%y-%H:%M:%S"`; fi
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
 	 printf "%-30s%-90s\n" "\$MARATHON_SSUK_VERSION:" ">$MARATHON_SSUK_VERSION<"
 	 printf "%-30s%-90s\n" "\$MARATHON_SSUK_LIBRARY_PATH:" ">$MARATHON_SSUK_LIBRARY_PATH<"
 	 printf "%-30s%-90s\n" "\$LD_LIBRARY_PATH:" ">$LD_LIBRARY_PATH<"
 	 printf "%-30s%-90s\n" "\$BINARY:" ">$BINARY<"
 	 printf "%-30s%-90s\n" "\$STARTMSG:" ">$STARTMSG<"
 	 printf "%-30s%-90s\n" "\$STOPMSG:" ">$STOPMSG<"
 	 printf "%-30s%-90s\n" "\$START_OPTION:" ">$START_OPTION<"
 	 printf "%-30s%-90s\n" "\$STOP_OPTION:" ">$STOP_OPTION<"
 	 printf "%-30s%-90s\n" "\$STATUS_OPTION:" ">$STATUS_OPTION<"
	 printf "%-30s%-90s\n" "\$DB_OPTION:" ">$DB_OPTION<"

 	 printf "%-30s%-90s\n" "\$process:" ">$process<"
 	 printf "%-30s%-90s\n" "\$start:" ">$start<"
 	 printf "%-30s%-90s\n" "\$stop:" ">$stop<"
 	 printf "%-30s%-90s\n" "\$status:" ">$status<"
	 printf "%-30s%-90s\n" "\$db:" ">$db<"
 	 echo "--------------------------"
 	 echo "EXIT $FUNCNAME"
 	 echo "--------------------------"
 }
 #=============================================#
 # CHECKING IF BINARY is EXISTING 
 #=============================================#
 function check_existence_of_binary
 {
 	 local _PROCESS_2_CHECK=${1};
 	 if [[ ! -f ${_PROCESS_2_CHECK} ]]; then
 		 printStartStop "$STARTMSG"
 		 rc=8;
 		 echo "Error: Binary \"${_PROCESS_2_CHECK}\" does not exist!" | tee -a $LOG_RESOURCE
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
 	 #-------------------------------#
 	 # read in options / parameters
 	 #-------------------------------#
 	 while (( $# > 0 )) 
 	 do
 		 #read in options
 		 if [[ "$1" == "$START_OPTION" ]] ; then start="YES"; shift; continue; fi
 		 if [[ "$1" == "$STOP_OPTION" ]] ; then stop="YES"; shift; continue; fi
 		 if [[ "$1" == "$STATUS_OPTION" ]] ; then status="YES"; shift; continue; fi

 		 #read in parameters
 		 if [[ "$1" == "$DB_OPTION" ]]; then
 			 shift;
 			 if (( $# > 0 )) ; then
 				 db=$1;
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
 	 if [[ "$db" == "" ]] || (( 1 != `check_string_parameter "$db"` )); then
 		 printStartStop "$STARTMSG"
 		 rc=12;
 		 echo "$PROCNAME: Parameter value missed or invalid value \$db: >$db<!" | tee -a $LOG_RESOURCE
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

 	 is_numeric "$1" "-NO_VERBOSE" # do the checks considered to be necessary
 	 if (( $? == 1 )); then
 		 _rc=1
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
 		 _parameters="${_parameters} $DB_OPTION $db"
 	 else
 		 _parameters="$DB_OPTION $db";
 	 fi

 	 if (( ${#_parameters} > 0 )); then
 		 process="$process ${_parameters}"
 	 fi

 }
 #######################################################################################################################################
 # BEGIN: INDIVIDUAL FUNCTION SPECIFIC IMPLEMENTATION --> CANNOT BE GENERATED
 #######################################################################################################################################
 #======================================================#
 # 
 #======================================================#
 function get_branches
 {
 	local readonly _FUNCNAME="get_branches";
	local _branches="";

	#------------------------------------------------------------#
	# All branches: DB=ode21;;
	#------------------------------------------------------------#
	if [[ "$1" = "ode21" ]]; then
        _branches="5,6,8,9,21,31,33,40,44,46,51,52,54,56,57,58,59,70,78,81,82"
	else
		echo "Only db <ode21> is valid!"
		exit 99;
	fi
	
	echo ${_branches}
	
	return ${_branches};
 }
 #======================================================#
 # Initializing of the procedure specific variables 
 #======================================================#
 function init_spec_variables
 {
 	local readonly _FUNCNAME="init_spec_variables";

    readonly BRANCHES="`get_branches ${db}`";
 }
 #======================================================#
 # Executing the sql 
 #======================================================#
 function doSql
 {
	typeset -i _rc=0;
	
	_SQL_FILE=${LOG_PATH}/${PROCNAME}.sql
	_LOG_FILE=${LOG_PATH}/${PROCNAME}.log
	
	local _DB=${1}

	echo ${SQL} >${_SQL_FILE}
	echo ${SQL} >${_LOG_FILE}
	
	dbaccess ${_DB} ${_SQL_FILE} 1>>${_LOG_FILE} 2>>${_LOG_FILE}
	_rc=${?};
		
	if (( ${_rc} != 0 )); then return ${_rc}; fi
	
	rm ${_SQL_FILE};
	return ${_rc};
 }

 #######################################################################################################################################
 # END: INDIVIDUAL FUNCTION SPECIFIC IMPLEMENTATION --> CANNOT BE GENERATED
 #######################################################################################################################################
 
 ###############################################################################################
 # H E L P & V E R S I O N 
 ###############################################################################################
 . $(dirname $0)/hyreport.option
 
 ###############################################################################################
 # S H E L L - L O G I C 
 ###############################################################################################
 check_parameters "$@";
 eval_parameters "$@";
 print_msg "$STARTMSG" "$*"
 

 #######################################################################################################################################
 # BEGIN: INDIVIDUAL SHELL LOGIC IMPLEMENTATION --> CANNOT BE GENERATED
 #######################################################################################################################################
 #---------------------------------------------------------------------------------#
 msg="Initializing procedure specific variables ..."; logging "${msg}";
 #---------------------------------------------------------------------------------#
 init_spec_variables;

 #------------------------------------------------------------#
 MSG="Checking start date (only the 1st of january or july valid)!"; logging "${MSG}";
 #------------------------------------------------------------#
 CUR_DATE=`date +%d%m`
 if [ "$CUR_DATE" != "0101" -a "$CUR_DATE" != "0107" ]; then
        MSG="Start of programme is only valid on the 1st of january and july!"; logging "${MSG}";
 	print_msg "$STOPMSG" "$*" "<99>"
        exit 99;
 fi

 #------------------------------------------------------------#
 MSG="INSERT of the still open narc orders!"; logging "${MSG}";
 #------------------------------------------------------------#
 SQL="insert into narcopenorders "
 SQL=${SQL}" SELECT "
 SQL=${SQL}" k.filialnr, p.artikel_nr, k.datum_bestell, k.bestell_nr, l.liefer_besla_nr, p.menge_bestell "
 SQL=${SQL}" FROM "
 SQL=${SQL}" beskopf k, besposi p, zartikel z,  bliefer l "
 SQL=${SQL}" WHERE "
 SQL=${SQL}" k.bestell_nr = p.bestell_nr "
 SQL=${SQL}" AND "
 SQL=${SQL}" k.filialnr = p.filialnr "
 SQL=${SQL}" AND "
 SQL=${SQL}" p.artikel_nr = z.artikel_nr "
 SQL=${SQL}" AND "
 SQL=${SQL}" k.besla_nr = l.besla_nr "
 SQL=${SQL}" AND "
 SQL=${SQL}" z.btm = 1 "
 SQL=${SQL}" AND "
 SQL=${SQL}" k.status_akt = 'A' "
 SQL=${SQL}" AND "
 SQL=${SQL}" k.filialnr in ( ${BRANCHES} )"
 doSql ${db};
 
 rc=${?};
 #######################################################################################################################################
 # END: INDIVIDUAL SHELL LOGIC IMPLEMENTATION --> CANNOT BE GENERATED
 #######################################################################################################################################

 print_msg "$STOPMSG" "$*" "<$rc>"
 
 exit $rc;
