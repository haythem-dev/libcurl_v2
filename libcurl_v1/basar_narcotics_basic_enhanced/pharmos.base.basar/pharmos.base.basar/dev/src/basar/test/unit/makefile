##############################################################
#                                                            #
#    makefile for basar unit test                            #
#                                                            #
##############################################################


dbg all rel clean clean_rel clean_dbg stdbg strel dep: FORCE
	cd ./cmnutil   ; $(MAKE) $@;
	cd ./dbsql     ; $(MAKE) $@;
	cd ./property  ; $(MAKE) $@;
	cd ./dbaspect  ; $(MAKE) $@;
	cd ./appl      ; $(MAKE) $@;
	cd ./login     ; $(MAKE) $@;

#-------------------------------------------------------------
#   Usage
#-------------------------------------------------------------
help: FORCE
	@echo "  make              : update (compile and link)   (debug)";
	@echo "  make all          : update (compile and link)   (all versions)";
	@echo "  make rel          : compile and link release version";
	@echo "  make dbg          : compile and link debug version";
	@echo "  make clean        : delete all versions";
	@echo "  make clean_rel    : delete release version";
	@echo "  make clean_dbg    : delete debug version";
	@echo "  make stdbg        : start debug";
	@echo "  make strel        : start release";
	@echo "";
	@echo "  make gdb          : start gdb debug session";
	@echo "";
	@echo "  make dep          : scan source files to create dependencies in file \"deps\"";
	@echo "";
	
FORCE:

