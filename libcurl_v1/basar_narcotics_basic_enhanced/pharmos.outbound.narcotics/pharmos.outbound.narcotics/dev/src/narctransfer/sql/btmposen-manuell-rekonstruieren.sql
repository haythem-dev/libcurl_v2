unload to "c:\zzz"
select * from parameterkd 
order by programmname, zweck, parametername, auspraeg


select * from lgtextkd

select vorwahl_fax, telefax, * from bliefer





======================================================================================================================
select * from parameterkd 
where programmname = 'KD2CLIENT'
order by filialnr, zweck, parametername, auspraeg;

update parameterkd 
set auspraeg  = 'Ref. Mme. '
where filialnr = 38	
and  programmname = 'KD2ORDER'
and  zweck = 'SUPPLIER_DELIVERY'
and  parametername = 'CONTACT_PERSON'
and  auspraeg = 'Ref. Mme. Comte'

delete from parameterkd 
where filialnr = 38 and 
programmname = 'KD2CLIENT'

insert into parameterkd values( 38, 'KD2CLIENT', 'STRINGS',  'APPL_DISABLED' , '0' );
insert into parameterkd values( 38, 'KD2CLIENT', 'STRINGS',  'COMPANY' , 'AMEDIS' );
insert into parameterkd values( 38, 'KD2CLIENT', 'STRINGS',  'CURRENCY' , 'EUR' );
insert into parameterkd values( 38, 'KD2CLIENT', 'STRINGS',  'DOKUURL' , 'http://ntfuln1/doku/public/anwendung/kundendispo/ver2/' );
insert into parameterkd values( 38, 'KD2CLIENT', 'STRINGS',  'LOGINTABLE' , 'pos2fr@db1_pos2_tcp' );
insert into parameterkd values( 38, 'KD2CLIENT', 'STRINGS',  'PRINTDLG' ,  '0' );