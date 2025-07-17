#!/usr/bin/ksh

###############################################################################################
# HEADER-DATA 
###############################################################################################
#Author:            d.zdjelar
#Creation-Date:     14.02.2025
#Purpose:           starts reorg of narcotic table narctransactiontransfer
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
date_older_than=0;
#--- end declaration procedure variables ---#
start="";
stop="";
status="";
typeset -i rc=0;

#--------------------------------------------------------------------#
# L I T E R A L S --> ATTENTION: INDIVIDUAL MODIFICATION REQUIRED <--
#--------------------------------------------------------------------#
readonly STARTMSG="START";
readonly STOPMSG="STOP";
readonly START_OPTION="-start";
readonly STOP_OPTION="-stop";
readonly STATUS_OPTION="-status";
readonly DATE_OLDER_THAN_OPTION="-date_older_than";

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
    _LOG_FILE=${PROCNAME}.log
    
    echo `log "O" 0 "================================================" "${LOG_PATH}/${_LOG_FILE}"`
    echo `log "O" 0 "'$1'" "${LOG_PATH}/${_LOG_FILE}"`
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
         if [[ "$1" == "$DATE_OLDER_THAN_OPTION" ]]; then
             shift
             if (( $# > 0 )) ; then
                 date_older_than=$1;
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
     if [[ "$date_older_than" == "" ]] || (( 1 != `check_numeric_parameter "${DATE_OLDER_THAN_OPTION}" "$date_older_than"` )) || [[ "$date_older_than" == "0" ]]; then
         printStartStop "$STARTMSG"
         rc=12;
         echo "$PROCNAME: Parameter value missed or invalid value \$date_older_safekeeping_period: >$date_older_than<!" | tee -a $LOG_RESOURCE
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

     if [[ "${_OPTION}" == "${DATE_OLDER_THAN_OPTION}" ]]; then
         # do the possibly necessary checks with parameter ${_ARG}
         _rc=1;
     fi

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
    
    _LOG_FILE=${PROCNAME}.log
    _SQL_FILE=${PROCNAME}.sql

	local DB="ode21";

    echo "${SQL}" >${_SQL_FILE}
    echo "${SQL}" >>${LOG_PATH}/${_LOG_FILE}
    
    dbaccess ${DB} ${_SQL_FILE} 1>>${LOG_PATH}/${_LOG_FILE} 2>>${LOG_PATH}/${_LOG_FILE}
    
     _rc=${?};(( $rc != 0 )) && exit $rc;

    rm ${_SQL_FILE};
    return ${_rc};
}


#======================================================#
# deleting narctransactiontransfer
#======================================================#
function delete_narctransactiontransfer
{ 
    typeset -i _rc=0;
    typeset -i _date_deletion=$1;
    
    #----------------------------------------------------------------------------#
    MSG="deleting narctransactiontransfer...";
    logging "$MSG";
    #----------------------------------------------------------------------------#

    # delete all orders in narctransactiontransfer that have completedflag = 1 and are older than x days
    SQL="delete narctransactiontransfer where completedflag = 1 and transactiondate < TO_CHAR(TODAY - ${_date_deletion},'%Y%m%d')::INTEGER;"
	
	echo "${SQL}"

    doSql;
    _rc=${?}; (( $rc != 0 )) && exit $rc;
    
    return $_rc;
}

###############################################################################################
# H E L P & V E R S I O N 
###############################################################################################
. $(dirname $0)/reorg_narctransactiontransfer.option

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
    
    delete_narctransactiontransfer "${date_older_than}";
    rc=$?; (( $rc != 0 )) && print_msg "$STOPMSG" "$*" "<$rc>" && exit $rc;
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