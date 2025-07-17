#!/usr/bin/ksh

#NARCOTICS_TRANSFER_DIR=/transfer/ftp/narcotics
NARCOTICS_TRANSFER_DIR=/software/home/h.bayer/bfarmtest
#NARCOTICS_TRANSFER_DIR=/software/home/m.koeppe/bfarmtest
NARCOTICS_OUTGOING_DIR=${NARCOTICS_TRANSFER_DIR}/outgoing
NARCOTICS_SAVE_DIR=${NARCOTICS_TRANSFER_DIR}/save
NARCOTICS_VOUCHERSTOOMUCHDIR=${NARCOTICS_SAVE_DIR}/voucherstoomuch

NARCOTICS_BFARMLOGFILE="ftp-protokoll_phoenix.log"
NARCOTICS_TMPDIR="${NARCOTICS_TRANSFER_DIR}/tmp/"
NARCOTICS_LOGDIR="${NARCOTICS_TRANSFER_DIR}/log/"
NARCOTICS_LOGFILE="transfer_bfarm.log"

LOG_RESOURCE="${NARCOTICS_TRANSFER_DIR}/log/${NARCOTICS_LOGFILE}"
LOG_MISSING_VOUCHER_DIR="${NARCOTICS_TRANSFER_DIR}/log/missingvouchers/"
LOG_MISSING_VOUCHER="${LOG_MISSING_VOUCHER_DIR}/missingvouchers.log"
LOG_VOUCHERS_TOO_MUCH_DIR="${NARCOTICS_TRANSFER_DIR}/log/voucherstoomuch/" 
LOG_VOUCHERS_TOO_MUCH="${LOG_VOUCHERS_TOO_MUCH_DIR}/voucherstoomuch.log" 

set -A contributionvouchernos;
set -A bganos;
set -A vouchersfromfile;

typeset -i branchno=0;

workingdir="";

db="";

dbzpos4de="dbzpos1de";
dbzpos1de="dbzpos1de";

function log_voucher_too_much
{   
    if [ ! -d $LOG_VOUCHERS_TOO_MUCH_DIR ]; then
	mkdir -p $LOG_VOUCHERS_TOO_MUCH_DIR;
    fi
    
    if [ ! -f $LOG_VOUCHERS_TOO_MUCH ]; then
	touch $LOG_VOUCHERS_TOO_MUCH;
	chmod 777 $LOG_VOUCHERS_TOO_MUCH;
    fi
    
     echo "$(date) $1 $2"|tee -a $LOG_VOUCHERS_TOO_MUCH    
}

function log_missing_voucher
{
    if [ ! -d $LOG_MISSING_VOUCHER_DIR ]; then
	mkdir -p $LOG_MISSING_VOUCHER_DIR
    fi    

    if [ ! -f $LOG_MISSING_VOUCHER ]; then
	touch $LOG_MISSING_VOUCHER;
	chmod 777 $LOG_MISSING_VOUCHER;
    fi
    
     echo "$(date) $1 $2"|tee -a $LOG_MISSING_VOUCHER    
}

 function log_message
 {
	 echo "$(date) $1"|tee -a $LOG_RESOURCE
 }
  
 function set_db
 {
 local _BRANCHNO=$1;
 
 case ${_BRANCHNO} in
		06|08|09|6|8|9|31|33|40|46|56|59|70|78)		db=$dbzpos1de
						;;
		05|5|21|44|51|52|54|57|58|81|82)   		db=$dbzpos4de
						;;
		*)	echo "No valid branch no <${_BRANCHNO}> passed! Branch no could not be matched to a database!";
			return -1;
			;;
	esac
	return 0;
 }
 
 
 
 function get_contributionvouchernos
 {
    log_message "get_contributionvouchernos for $1 $2 $3 "

    local _branchno=$1;
    local _fromdate=$2;
    local _todate=$3;
    local _resultfile="result.tmp";
    local _cleanresult="cleanresult.txt"
    local _errorfile="error.tmp";
    

    local _sql="select distinct contributionvoucherno from narctransactioncatalog where deletedflag=0 and branchno=$_branchno and transactiondate>=$_fromdate and transactiondate<=$_todate";
    
    log_message "local sql: $_sql with db $db";
    
    #echo "$_sql" | dbaccess ${_db} 1>${_resultfile} 2>${_errorfile};
    
    #echo "$_sql" | dbzpos1de 1>${_resultfile} 2>${_errorfile};
    
    echo "$_sql" | $db 1>${_resultfile} 2>${_errorfile};
    
    log_message "get result from file";
    

#deletes all trailing and flushing whitespaces and also teh tableheader and emptylines
    sed 's/^[ \t]*//;s/[ \t]*$//' $_resultfile|sed '/^$/d;/[a-zA-Z\+]+/d' > $_cleanresult

#not more than 4000 entries expected -> should fit in array
    set -A contributionvouchernos `cat ${_cleanresult}`;

    log_message "found vals:  ${contributionvouchernos[*]}"
    rm $_resultfile;
    rm $_errorfile;
    rm $_cleanresult;
 }
 
 
 function get_bganos_for_db
 {
    log_message "get_bganos_for_db with db $db"
    local _sql="select distinct bganr_ek from filiale"; 
    local _resultfile="resultbga.tmp";
    local _cleanresult="cleanresultbga.tmp";
    
    #echo "$_sql" | dbzpos1de 1>${_resultfile} 2>${_errorfile};
    
    echo "$_sql" | $db 1>${_resultfile} 2>${_errorfile};
    
    sed 's/^[ \t]*//;s/[ \t]*$//' $_resultfile|sed '/^$/d;/bganr_ek/d' > $_cleanresult
    
    set -A bganos `cat $_cleanresult`;    
        
    if [[ ${#bganos[*]} -eq 0 ]]; then
	log_message "no bganos found!!";
	return -1;
    fi
    
    rm $_resultfile;
    rm $_cleanresult;
    
    log_message "found bganos $bganos"
    
    
    
    return 0;
    
 }
 
 function resolvebgano
 {
    log_message "resolvebgano with bgano $1 db $db"
    local _bgano=$1;
    local _sql="select filialnr from filiale where bganr_ek = $_bgano"; 
    local _resultfile="resultbga.tmp";
    local _cleanresult="cleanresultbga.tmp";
    
    echo "$_sql" | $db 1>${_resultfile} 2>${_errorfile};
    
    sed 's/^[ \t]*//;s/[ \t]*$//' $_resultfile|sed '/^$/d;/filialnr/d' > $_cleanresult
    
    branchno=`cat $_cleanresult`;    
    
    if [[ branchno -eq 0 ]]; then
	log_message "no branchno found for bgano $_bgano";
	return -1;
    else
	log_message "found branchno $branchno"
    fi
    
    rm $_resultfile;
    
    rm $_cleanresult;
        
    return 0;
 }
 
 function contributionvoucherno_exists_in_db
 {
     local passedvoucherno=$1;
     passedvoucherno=`echo "$passedvoucherno"|cut -c1-8`
     log_message "contributionvoucherno_exists_in_db $1"
	    
     for voucherno in ${contributionvouchernos[*]}; do
        log_message "checked voucherno $voucherno and passesvoucherno $passedvoucherno"
        if [[ $passedvoucherno = $voucherno  ]]; then
	    log_message "valid:$passedvoucherno = $voucherno " 
	    return 0;
	fi
     done
     
     return -1;      
 }

    
 function backup_and_move_file
 {
    log_message "backup_and_move_file $2 with dir $1"
    
    local _file=$2;
    local _destfolder=$1;
    
    _destfolder=${_folder##*/}
    
    local _backupfolder="$NARCOTICS_VOUCHERSTOOMUCHDIR/$_destfolder"
    
    
    if [ ! -d "$_backupfolder" ]; then
	mkdir -p "$_backupfolder";  
    fi 
    
    mv "$_folder/$_file" "$_backupfolder/$_file.`date '+20%y%m%d%H:%M:%S'`"    
    
    if [[ $? -eq 0 ]]; then
	return 0;
    fi
    
    return -1
 }
 
 function checkforvouchertoomuch
 {
    log_message "checkfortoomuchvouchers $_folder"
    
    

    if [[ ${#contributionvouchernos[*]} -eq 0 ]]; then
	log_message "no contributionvouchernos found!!"
	return;
    fi
    
    i=0;
    for file in `ls $_folder|grep pdf`; do
	log_message "found file: $file"
	
	local _voucherno=`echo "$file"|cut -c1-8`	
	
	vouchersfromfile[$i]=$_voucherno;
	(( i=i+1 ))
		
	if contributionvoucherno_exists_in_db $file; then
	    log_message "db entry for file $file exists"
	else	    
	    log_voucher_too_much $_folder $file 
	    backup_and_move_file $_folder $file	       	
	fi
    done;
    
 }
 
 function doesentryexist
 {
    _entrytosearch=$1
    log_message "doesentryexist $1"
    
    i=0
    
    for fileentry in ${vouchersfromfile[*]}; do
	
	echo "fileentry $fileentry to search $_entrytosearch"
	
	if [[ $fileentry = $_entrytosearch ]]; then 
	    return 0
	fi
	
	(( i=i+1 ))
    done
    
    return -1
 }
 
 function checkformissingvouchers
 {
    i=0;
    
    for file in ${contributionvouchernos[*]}; do
			
	local _voucherno=`echo "$file"|cut -c1-8`	
				
	if doesentryexist $file; then
	    log_message "db entry for file $file exists"
	else	    
	    log_missing_voucher $_folder $file 
	fi
    done;
 }
 
 function checkfolder
 {
    local _folder=$1;
    local _voucherno=`echo "$passedvoucherno"|cut -c1-8`
    log_message "checkfolder $_folder"
    
    checkforvouchertoomuch _folder
    checkformissingvouchers    
 } 
 
 
 function isbganovalidfordb
 {
    local bgano=$1;
    
    log_message "isbganovalidfordb for $bgano"
    
    for validbgano in ${bganos[*]}; do	
	if [[ $1 = $validbgano ]]; then
	    log_message "validbgano: $validbgano"
	    return 0;
	fi
    done
    
    return -1;
 }
 
 
 function set_db_for_bgano
 {
    log_message "set_db_for_bga_no $1"
    
    local bgano=$1;
    
    db=$dbzpos4de;
    
    resolvebgano $bgano;
    
    if [[ $? -ne 0 ]]; then
	return -1;
    fi
    
    #as both databases have entries in their tables but are inconsistent-> try bothz databases
    if [[ $branchno -eq 0 ]]; then
	db=$dbzpos1de;
	
	resolvebgano $bgano
	
	if [[ $? -ne 0 ]]; then
	    return -1;
	fi	
    fi
    
    db="";
    
    if [[ $branchno -eq 0 ]]; then
	log_message "no branchno found for bgano $bgano";
	return -1;
    fi
    
    set_db $branchno
    
    if [[ $? -ne 0 ]]; then
	return -1;
    fi
    return 0;    
 }
 
 function cleanup
 {
    branchno=0;
    db="";
    set -A contributionvouchers;
    set -A vouchersfromfile;
 }
    
 
 function process_contributionvouchers 
 {
    
    log_message "process_contributionvouchers for $1"
    local _workingdir=$1;
        
    
        
    for bgafolder in `ls $_workingdir`; do
	log_message "found folder: $bgafolder"   
	 
	bgano=`echo $bgafolder|cut -c1-7`;
	
	set_db_for_bgano $bgano;
	
	if [[ $? -ne 0 ]]; then
	    cleanup
	    return -1;	    
	fi
			
	local _enddate=`date '+20%y%m%d'`
	local _startdate=$(($_enddate-7));
	
	get_contributionvouchernos $branchno $_startdate $_enddate;	    	        
	
	if [[ $? -ne 0 ]]; then
	    cleanup
	    return -1;
	fi
			
	checkfolder "$_workingdir/$bgafolder"				
	
	if [[ $? -ne 0 ]]; then
	    cleanup
	    return -1;
	fi
		
	cleanup 
    done
    
 }
 
 
 export INFORMIXDIR="/opt/lib/informix/bin/";
 
 process_contributionvouchers "/software/home/h.bayer/bfarmtest/outgoing" 