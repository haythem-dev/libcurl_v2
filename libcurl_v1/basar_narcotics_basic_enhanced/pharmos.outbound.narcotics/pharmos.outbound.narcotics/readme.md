
# Narcotics 

## ERWIN 
erwin files can be found in the "Database models" channel of Outbound Logistics:
[Outbound Logistics - Database models](https://phoenixonline.sharepoint.com/:f:/r/sites/CS-CIT-ORG-CITOutboundLogistics/Shared%20Documents/Database%20models?csf=1&web=1&e=bsdDUa)

<br>
<br>

## How to's

<br>
<br>

**add new signatures**
>Assign the ticket to Klaus Tischhöfer, he will add the signature.  
>Klaus will assign it back to you, once he is finished with his part of the task.  
>You should see a new commit with the changed files in the Narcotics repository.  

>In the next step you need to upload the new signature to COIN (needed for RFC and ITQM):  
>https://coin-collaboration.phoenix.loc/communities/Outbound/Narcotics/User_signature_list_refering_to_FSPHANarcotics01_/Forms/RecentDocuments.aspx

>After that, copy the changed .ps file(s) into the deployment share directory:  
>\\DENU00MS5010.phoenix.loc\Citrix_NX_Delivery$\Phoenix\Marathon\PROD\narcotics\print  

>In the last step you have to create the RFC for the deployment of the new .ps file(s)  
>The quicket way to do this is to copy an already existing one and adjust the information.

<br>
<br>


## Solution to problems

<br>
<br>

**LD mistakenly created**
>Delete the Lieferscheindoppel from transfer folder. 

>Update the acknowledgementstatus from 5 (LD created) to 2 (contributionvoucher created) or 3 (contributionvoucher received) 

>If Belegstorno has been selected in the LD, it has to be restored. 
>In NA03 (BTM Buch) click on 'Wiederherstellen', select PZN and click on 'Wiederherstellen' 
>Now the booking is no longer deleted and the stock should be fine. 

<br>

**stock difference**
>Check the timestamp in the narctransactioncatalog and narctransactionoriginal 

>There may be 2 orders  with the same article that have the same time in the field transactiontime
>In this case the solution would be to update the transactiontype of the newer order,
>e.g. add 1 second the the transactiontime. Do this in both tables narctransactioncatalog and narctransactionoriginal. 

>The stock difference should be solved at the next day as the calculation of the stock difference takes place at night.
>It can also be trigged during the day, but it is not recommended as it takes some time 
>and errors might happen! 

<br>

**order is not being printed**
>One of the reasons why an order is not being printed could be the customersupplierno.  
>The printthread selects all entries in narctransactioncatalog with deletedflag = 0, transactiontype = a or d 
>and the customersupllierno must be lower then the internals booking number, 
>which is defined in the UADM parameters.  
>Currently the internal booking number is set to 60000000.  
>If the customersupplierno is above this value, it will not be printed!  

>Why is that?  
>Numbers above this value are internal IDF numbers.  
>When the order is being processed from narctransactiontransfer to narctransactioncatalog,  
>the mapping of internal and external IDFs will be checked, which is maintained in the customernomapping table.  
>Sometimes the mapping for the internal IDF is not existing and the internal numbers is being set in the customersupplier no.  

>The Narcotics users can update the IDF from the BTM-Buch dialog.  
>The users should also check the IDF mapping and adapt it.  


<br>

**C4 Halbjahresmeldung is missing an order**
>C4 Halbjahresmeldung contains orders from the supplier/manufacturer,  
>that have not arrived yet. This is very crucial, BfArm has to know that the supplier/manufactuer sent the goods,  
>and that we haven't received it yet (before 01.07 or 31.12).  

>Open orders for narcotics can be found in the narcopenorders table.  
>The orders are being inserted by pharmos.outbound.narcotics\dev\src\narctransfer\proc\hyreport.sh  
>You will find insert and select statements in there. You can copy and adjust the select statement to see if you can find the desired order.  
>If not it may be that the status_akt has not been correct at the time this script run.  
>If so, you can adjust the where statement and find the correct order and insert it into the narcopenorder table by yourself.  

>The user can try again to print the C4 Halbjahresmeldung,  
>after all missing orders have been inserted into the narcopenorders table. Now everything should be correct. 
 

<br>