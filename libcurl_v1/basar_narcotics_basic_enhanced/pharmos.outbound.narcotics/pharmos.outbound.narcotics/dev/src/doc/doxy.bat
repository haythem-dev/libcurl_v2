set PATH_DOCU=.\codedocu\internal
if not exist %PATH_DOCU% mkdir %PATH_DOCU%
rmdir /s /q %PATH_DOCU%
C:\Programme\doxygen\bin\doxygen.exe -d Preprocessor narcotics_internal.doxyfile > doxygen_log.txt

set PATH_DOCU=.\codedocu\customer
if not exist %PATH_DOCU% mkdir %PATH_DOCU%
rmdir /s /q %PATH_DOCU%
C:\Programme\doxygen\bin\doxygen.exe -d Preprocessor narcotics_customer.doxyfile > doxygen_log.txt
ftp -i -s:doxy_ftp.txt

pause

