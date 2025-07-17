#! /usr/bin/ksh

LOG_DIR=/software/wss/de/narcotics/log
INI_DIR=/software/wss/de/narcotics/ini

LOG_FILE="$LOG_DIR/narcoticsalerting.error.log"
INI_FILE="$INI_DIR/narcoticsalerting_mail_branch.ini"

if [ -f ${LOG_FILE} ] ; then
   echo "log directory and file '${LOG_FILE}' used for checking missing vouchers..."
else
   echo "Problems accessing log directory and file '${LOG_FILE}'. Please check!"
   exit 1
fi

if [ -f ${INI_FILE} ] ; then
   echo "ini directory and file '${INI_FILE}' used for branch and mail adresses..."
else
   echo "Problems accessing ini directory and file '${INI_FILE}'. Please check!"
   exit 1
fi

while IFS= read -r line
do
    IFS=';'
    set -A colarray $line
    branch=${colarray[0]}
    title=${colarray[1]}
    mailto=${colarray[2]}
    echo "Scanning logfile for branch ${branch}..."
    data=$(sed -n -e "s/^.*missingvoucher: ${branch}; //p" $LOG_FILE)
    if [[ ! -z "$data" ]]; then
	echo "$data"
        export TIMESTAMP=$(date '+%Y%m%d_%H%M%S')
        export MAILTO="t.arnold@externals.phoenixgroup.eu"
        export SUBJECT="Narcotics Alerting missing Vouchers for Branch ${branch}"
        export ATTACH=$data
        (
        echo "To: $MAILTO"
        echo "Subject: $SUBJECT"
        echo "MIME-Version: 1.0"
        echo 'Content-Type: multipart/mixed; boundary="-q1w2e3r4t5"'
        echo
        echo '---q1w2e3r4t5'
        echo "Content-Type: text/html"
        echo "Content-Disposition: inline\n"
	echo "Sehr geehrtes ${title},<br><br>"
        echo "das Narcotics Alerting Logfile: ${LOG_FILE} wurde geprüft.<br>"
	echo "Eine Liste aller fehlenden Vouchers für die Filiale ${branch} ist dieser Mail angehängt.<br>"
        echo "<hr><br>"
	echo "Dear ${title},<br><br>"
        echo "the Narcotics Alerting Log file: ${LOG_FILE} was checked.<br>"
        echo "A list of missing vouchers for branch ${branch} is attached to this mail.<br>"
        echo '---q1w2e3r4t5'
        echo 'Content-Type: application; name="missingvouchers_'${branch}'.csv"'
        echo 'Content-Disposition: attachment; filename="missingvouchers_'${branch}'.csv"'
        echo
	echo "CustomerSupplierNo;VoucherNo;TransactionDate;PrintDate;"
        echo "${ATTACH}"
        echo '---q1w2e3r4t5--'
	) | /usr/sbin/sendmail $MAILTO
    fi
done < ${INI_FILE}
