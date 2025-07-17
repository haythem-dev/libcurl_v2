##############################################################
#                                                            #
#    makefile for basar                                      #
#                                                            #
##############################################################


dbg all rel clean clean_rel clean_dbg clean_prof dep: FORCE
	cd ./libbasardebug       ; $(MAKE) $@;
	cd ./libopensslwrap      ; $(MAKE) $@;
	cd ./libbasarcmnutil     ; $(MAKE) $@;
	cd ./libbasarproperty    ; $(MAKE) $@;
	cd ./libbasardbsql       ; $(MAKE) $@;
	cd ./libbasardbaspect    ; $(MAKE) $@;
	cd ./libbasarappl        ; $(MAKE) $@;
	cd ./libbasarlogin       ; $(MAKE) $@;
	cd ./libbasarprinting    ; $(MAKE) $@;
	cd ./test/unit           ; $(MAKE) $@;

prof: FORCE
	cd ./libbasardebug       ; $(MAKE) $@;
	cd ./libopensslwrap      ; $(MAKE) $@;
	cd ./libbasarcmnutil     ; $(MAKE) $@;
	cd ./libbasarproperty    ; $(MAKE) $@;
	cd ./libbasardbsql       ; $(MAKE) $@;
	cd ./libbasardbaspect    ; $(MAKE) $@;
	cd ./libbasarappl        ; $(MAKE) $@;
	cd ./libbasarlogin       ; $(MAKE) $@;
	cd ./libbasarprinting    ; $(MAKE) $@;

test: dbg FORCE
	proc/run_ut.sh

help: FORCE
	@echo "  make              : update (compile and link)  (debug)";
	@echo "  make all          : update (compile and link)  (all versions)";
	@echo "  make rel          : create release version   ";
	@echo "  make dbg          : create debug version  ";
	@echo "  make prof         : create static lib version for profiling ";
	@echo "  make clean        : delete all versions";
	@echo "  make clean_rel    : delete release version ";
	@echo "  make clean_dbg    : delete debug version ";
	@echo "  make clean_prof   : delete profiling version ";
	@echo "";
	@echo "  make test         : run tests (debug version)";
	@echo "";
	@echo "  make dep          : scan source files to create dependencies in file \"deps\"";
	@echo "";
	
FORCE:
